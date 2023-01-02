void f() {
  int a;
  {
    int b;
    a = 10;
    b = 5;
    {
      int a;
      a = b;
    }
  }
  a = 0;
}

/*
  Expected Output:
  Valid
*/