#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "lodepng.h"

#define ONE_BILLION (double)1000000000.0
#define BYTES_PER_PIXEL 4
#define RED_OFFSET 0
#define GREEN_OFFSET 1
#define BLUE_OFFSET 2
#define ALPHA_OFFSET 3

#define IMG_BYTE(columns, r, c, b) ((columns * BYTES_PER_PIXEL * r) + (BYTES_PER_PIXEL * c) + b)
#define CLAMP(val, min, max) (val < min ? min : val > max ? max : val)

typedef unsigned char pixel_t;
pthread_mutex_t match_count_mutex;
typedef struct {
  pixel_t *pixels;
  unsigned int rows;
  unsigned int columns;
} image_t;

#define KERNEL_DIM 3
typedef int kernel_t[KERNEL_DIM][KERNEL_DIM];
typedef int kernel_t2[KERNEL_DIM];
typedef struct {
  int tid; 
  image_t *ini;
  image_t *outi;
  unsigned int start;
  unsigned int end;
  kernel_t2 *k;
} targs;
/* Load PNG image from 'file_name' into 'image'. 
 */
void
load_and_decode(image_t *image, const char *file_name)
{
  unsigned int error =
	lodepng_decode32_file(&image->pixels, &image->columns, &image->rows, file_name);
  if (error) {
	fprintf(stderr, "error %u: %s\n", error, lodepng_error_text(error));
  }
  printf("Loaded %s (%dx%d)\n", file_name, image->columns, image->rows);
}

/* Encode image in PNG format into file 'file_name'. 
 */
void
encode_and_store(image_t *image, const char *file_name)
{
  unsigned int error =
	lodepng_encode32_file(file_name, image->pixels, image->columns, image->rows);
  if (error) {
	fprintf(stderr, "error %u: %s\n", error, lodepng_error_text(error));
  }
  printf("Stored %s (%dx%d)\n", file_name, image->columns, image->rows);
}

/* Initialize an image_t structure 'image' for an image of size 'rows' by 'columns'. 
 */
void
init_image(image_t *image, int rows, int columns)
{
  image->rows = rows;
  image->columns = columns;
  image->pixels = (pixel_t *)malloc(image->columns * image->rows * BYTES_PER_PIXEL);
}

double
now(void)
{ //copy of time function
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);
  return current_time.tv_sec + (current_time.tv_nsec / ONE_BILLION);
}

/* Free a previously initialized image.
 */
void
free_image(image_t *image)
{
  free(image->pixels);
}

void
check_thread_manage_t(char *msge, int manage_t) { //pulled from last assignment to manage the threads
  if (manage_t) {
    fprintf(stderr, "ERROR: %s (%d)\n", msge, manage_t);
    exit(1);
  }
}
/* Copy an existing 'input' image to an 'output' image. Initializes the
   'output' image, on which you should later invoke 'free_image'.
 */
void
copy(image_t *output, image_t *input)
{
  int rows = input->rows;
  int columns = input->columns;
  init_image(output, rows, columns);

  for (int r = 0;  r < rows;  r++) {
	for (int c = 0;  c < columns;  c++) {
	  for (int b = 0;  b < BYTES_PER_PIXEL;  b++) {
		output->pixels[IMG_BYTE(columns, r, c, b)] = input->pixels[IMG_BYTE(columns, r, c, b)];
	  }
	}
  }
}

/* Compute the normal value of the given 'kernel'. 
 */
int
normalize_kernel(kernel_t kernel)
{
  int norm = 0;

  for (int r = 0;  r < KERNEL_DIM;  r++) {
	for (int c = 0;  c < KERNEL_DIM;  c++) {
	  norm += kernel[r][c];
	}
  }
  if (norm == 0) {
	norm = 1;
  }

  return norm;
}

/* Convolve image 'input' with 'kernel' into image 'output', which is
   allocated here and should later be freed.
 */
void *
convolve(void *thread_params)
{
  targs *args = (targs *)thread_params;

  int half_dim = KERNEL_DIM / 2;
  int kernel_norm = normalize_kernel(args->k);

  for (int r = args->start;  r < (args->end-1);  r++) {
	for (int c = 1;  c < (args->ini->columns - 1);  c++) {
	  for (int b = 0;  b < BYTES_PER_PIXEL;  b++) {
		int value = 0;

		if (b == ALPHA_OFFSET) {
		  /* Retain the alpha channel. */
		  value = args->ini->pixels[IMG_BYTE(args->ini->columns, r, c, b)];
		} else {
		  /* Convolve red, green, and blue. */
		  for (int kr = 0;  kr < KERNEL_DIM;  kr++) {
			for (int kc = 0;  kc < KERNEL_DIM;  kc++) {
			  int R = r + (kr - half_dim);
			  int C = c + (kc - half_dim);
			  value += args->k[kr][kc] * args->ini->pixels[IMG_BYTE(args->ini->columns, R, C, b)];
			}
		  }

		  value /= kernel_norm;
		  value = CLAMP(value, 0, 0xFF);
		}
		args->outi->pixels[IMG_BYTE(args->ini->columns, r, c, b)] = value;
	  }
	}
  }
  return (void *)NULL;
}

/* Catalog of kernels; allows user to select the kernel to use by name at run
   time. If you add more kernels, make sure the "null" kernel remains at the
   end of the list.
 */
#define DEFAULT_KERNEL_NAME "identity"

typedef struct {
  char *name;					/* Kernel name */
  kernel_t kernel;				/* Kernel itself */
} catalog_entry_t;

catalog_entry_t kernel_catalog[] =
  {
   {
	DEFAULT_KERNEL_NAME,
	{ { 0, 0, 0 },
	  { 0, 1, 0 },
	  { 0, 0, 0 } }
   },
   {
	"edge-detect",
	{ { -1, -1, -1 },
	  { -1, +8, -1 },
	  { -1, -1, -1 } }
   },
   {
	"sharpen",
	{ { +0, -1, +0 },
	  { -1, +5, -1 },
	  { +0, -1, +0 } }
   },
   {
	"emboss",
	{ { -2, -1, +0 },
	  { -1, +1, +1 },
	  { +0, -2, +2 } }
   },
   {
	"gaussian-blur",
	{ { 1, 2, 1 },
	  { 2, 4, 2 },
	  { 1, 2, 1 } }
   },
   { NULL, {} }					/* Must be last! */
  };

/* Locate an entry in the kernel catalog by name. Returns a null pointer if no
   kernel found.
 */
catalog_entry_t *
find_entry_by_name(char *name)
{
  for (catalog_entry_t *cp = kernel_catalog;  cp->name;  cp++) {
	if (strcmp(cp->name, name) == 0) {
	  return(cp);
	}
  }
  return (catalog_entry_t *) NULL;
}

/* Print an optional message, usage information, and exit in error.
 */
void
usage(char *prog_name, char *msge)
{
  if (msge && strlen(msge)) {
	fprintf(stderr, "\n%s\n\n", msge);
  }
  
  fprintf(stderr, "usage: %s [flags]\n", prog_name);
  fprintf(stderr, "  -h                print help\n");
  fprintf(stderr, "  -i <input file>   set input file\n");
  fprintf(stderr, "  -o <output file>  set output file\n");
  fprintf(stderr, "  -k <kernel>       kernel from:\n");

  for (int i = 0;  kernel_catalog[i].name;  i++) {
	char *name = kernel_catalog[i].name;
	fprintf(stderr, "       %s%s\n", name,
			strcmp(name, DEFAULT_KERNEL_NAME) ? "" : " (default)");
  }
  exit(1);
}

int
main(int argc, char **argv)
{
  char *prog_name = argv[0];	/* Convenience */
#define ERR_MSGE_LEN 128
  char err_msge[ERR_MSGE_LEN];	/* Dynamic error messages */
  
  catalog_entry_t *selected_entry = find_entry_by_name(DEFAULT_KERNEL_NAME);
  char *input_file_name = NULL;
  char *output_file_name = NULL;
  int num_t; //number of threads
  int trows; //total rows
  int rpt; //rows per thread
  double stime, etime;

  int ch;
  while ((ch = getopt(argc, argv, "t:hi:k:o:")) != -1) {
	switch (ch) {
  case 't':
    num_t = atol(optarg);
    break;
	case 'i':
	  input_file_name = optarg;
	  break;
	case 'k':
	  selected_entry = find_entry_by_name(optarg);
	  if (selected_entry == (catalog_entry_t *)NULL) {
		snprintf(err_msge, ERR_MSGE_LEN, "no kernel named '%s'", optarg);
		usage(prog_name, err_msge);
	  }
	  break;
	case 'o':
	  output_file_name = optarg;
	  break;
	case 'h':
	default:
	  usage(prog_name, "");
	}
  }
  int start_point=1;
  int end_point=2;

  if (!input_file_name) {
	usage(prog_name, "No input file specified");
  }
  if (!output_file_name) {
	usage(prog_name, "No output file specified");
  }
  if (strcmp(input_file_name, output_file_name) == 0) {
	usage(prog_name, "Input and output file can't be the same");
  }

  image_t input;
  image_t output;
  load_and_decode(&input, input_file_name);
  init_image(&output, (&input)->rows, (&input)->columns);

  pthread_t threads[num_t];
  targs thread_args[num_t];

  int manage_t = pthread_mutex_init(&match_count_mutex, NULL);
  check_thread_manage_t("mutex init", manage_t);

  trows = (&input)->rows;
  rpt = trows/num_t;
  stime = now();
  for (int i = 0;  i < num_t;  i++) {
    end_point+=rpt;
    if ((i+1)==num_t){
      end_point=end_point-1;
    }
    thread_args[i].tid = i;
    thread_args[i].ini = &input;
    thread_args[i].outi = &output;
    thread_args[i].start = start_point;
    thread_args[i].end = end_point;
    thread_args[i].k = selected_entry->kernel;;

    start_point+=rpt;

    manage_t = pthread_create(&threads[i], NULL,
                         convolve, (void *)&thread_args[i]);
    check_thread_manage_t("create", manage_t);

  }

  //clean up the threads after they are done.

  for (int i = 0;  i < num_t;  i++) {
    manage_t = pthread_join(threads[i], NULL);
    check_thread_manage_t("join", manage_t);
  }
  etime = now();
  printf("    TOOK %5.3fs\n", etime - stime);
  encode_and_store(&output, output_file_name);

  free_image(&input);
  free_image(&output);
}