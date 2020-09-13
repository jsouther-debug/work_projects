import 'dart:io';
import 'dart:math';

import 'common/image.dart';
import 'common/maths.dart';


// ELECTIVE / EXTRA CREDIT
// - composite functions properly handle alpha in _both_ images
// - other composite functions
//     - multiply / divide
//     - brightness
//     - additive / subtractive
//     - color
// - modify composite to handle images of different sizes
//     - ex: add optional `left` and `top` parameters to position imageA

// Render a vertical gradient.
Image renderGradient(
    Color top,      // color seen at top of gradient
    Color bottom,   // color seen at bottom of gradient
    int   steps,    // number of distinct steps seen in gradient
) {
    var image = Image(512, 512);

    // foreach row in image
    //     foreach pixel in row
    //         assign color to pixel so that:
    //             top row is `top`
    //             bottom row is `bottom`
    //             rows between have `steps` evenly spaced colors

    return image;
}

// Render an image according to algorithm specified in write up.
Image renderAlgorithm(
    int iterations,     // number of times to repeat the algorithm
) {
    var image = Image(512, 512);

    // Define the 3 points and their corresponding colors
    var points = [Point2(255, 40), Point2(506, 437), Point2(6, 437)];
    var colors = [Color.red(),     Color.green(),    Color.blue()  ];

    // Find starting position and color
    var rnd      = Random();        // create a random number generator
    var position = Point2(
        rnd.nextDouble() * 512,     // choose a random x for position
        rnd.nextDouble() * 512,     // choose a random y for position
    );
    var color    = Color.white();   // current color

    // repeat iterations
    //     write current color value into image at current position
    //     choose random corner position and its corresponding color
    //         tip: to choose a random index in [0,2]: `var i = rnd.nextInt(3);`
    //     update current position to be average of current and random
    //     update current color to be average of current and random

    return image;
}

// Return result of "over" composite operation, of `above` over `below`.
// see: https://gfx.cse.taylor.edu/courses/cos350/slides/01_Images.md.html#compositing
Color colorOver(
    Color above,    // above color
    Color below,    // below color.  can assume below.alpha==1, but extra credit for
                    // correctly handling the case when below.alpha < 1
) {

    // The following is only a placeholder
    return Color.white();
}

// Return result of "blend" composite operation, of `above` blended into `below` by `factor`.
// see: https://gfx.cse.taylor.edu/courses/cos350/slides/01_Images.md.html#compositing
Color colorBlend(
    Color  from,    // color to mix into `to`
    Color  to,      // color with which to mix
    double factor,  // the amount of `from` to mix into `to`
                    // factor == 0.0 --> result == `to`
                    // factor == 1.0 --> result == `from`
                    // factor == 0.5 --> result is average of `to` and `from`
) {

    // The following is only a placeholder
    return Color.white();
}

void main() {
    Image image;

    // Make sure images folder exists, because this is where all generated images will be saved
    Directory('images').createSync();

    // Generate images that will be used later
    Image imageA = Image.generateImageA();
    Image imageB = Image.generateImageB(withAlpha:false); // for extra credit, make compositing work when imageB has alpha <= 1

    // NOTE: Passing `saveMask:true` to `saveImage` will save the alpha channel to a separate image.
    //       The mask image will add  `_mask` to the path.
    // NOTE: Passing `withAlpha:true` to `saveImage` will save the image as PAM, the P7 version of PNM, which stores alpha values.
    //       However, most image editors are not able to load PAM files.
    //       You can convert the PAM file to a PNG using ImageMagick or https://gfx.cse.taylor.edu/courses/cos350/projects/netpbm/.


    // Generate gradients with steps

    print('Rendering gradient with 8 steps');
    image = renderGradient(Color.black(), Color.white(), 8);
    image.saveImage('images/00_gradient_008.ppm');

    print('Rendering gradient with 16 steps');
    image = renderGradient(Color.black(), Color.white(), 16);
    image.saveImage('images/01_gradient_016.ppm');

    print('Rendering gradient with 256 steps');
    image = renderGradient(Color.black(), Color.white(), 256);
    image.saveImage('images/02_gradient_256.ppm');


    // Generate image using algorithm

    print('Rendering algorithm');
    image = renderAlgorithm(10);
    image.saveImage('images/03_algorithm.ppm');


    // Generate images using compositing with over and blend operations

    print('Compositing A over B');
    image = Image.composite(imageA, imageB, colorOver);
    image.saveImage('images/04_a_over_b.ppm', saveMask:true);

    print('Compositing B over A');
    image = Image.composite(imageB, imageA, colorOver);
    image.saveImage('images/05_b_over_a.ppm', saveMask:true);

    print('Compositing A blend B 0.00');
    image = Image.composite(imageA, imageB, (a, b) => colorBlend(a, b, 0.00));
    image.saveImage('images/06_a_blend000_b.ppm', saveMask:true);

    print('Compositing A blend B 0.25');
    image = Image.composite(imageA, imageB, (a, b) => colorBlend(a, b, 0.25));
    image.saveImage('images/07_a_blend025_b.ppm', saveMask:true);

    print('Compositing A blend B 0.50');
    image = Image.composite(imageA, imageB, (a, b) => colorBlend(a, b, 0.50));
    image.saveImage('images/08_a_blend050_b.ppm', saveMask:true);

    print('Compositing A blend B 0.75');
    image = Image.composite(imageA, imageB, (a, b) => colorBlend(a, b, 0.75));
    image.saveImage('images/09_a_blend075_b.ppm', saveMask:true);

    print('Compositing A blend B 1.00');
    image = Image.composite(imageA, imageB, (a, b) => colorBlend(a, b, 1.00));
    image.saveImage('images/10_a_blend100_b.ppm', saveMask:true);
}