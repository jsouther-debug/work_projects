
undefined8 main(void)

{
  uint uVar1;
  uint local_18;
  uint local_10;

  local_18 = 0;
  printf("\n%d %d",0,1);
  local_10 = 1;
  while ((int)local_10 < 1000) {
    printf(" %d",(ulong)local_10);
    uVar1 = local_10 + local_18;
    local_18 = local_10;
    local_10 = uVar1;
  }
  return 0;
}
