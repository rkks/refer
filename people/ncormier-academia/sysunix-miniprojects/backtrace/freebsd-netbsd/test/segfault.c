int segfault()
{
  char* ptr = 0x0;

  *ptr = 0;
}

int main()
{
  segfault();
}
