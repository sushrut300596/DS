void f() {
  int a;
  {
    int b;
    a = 10;
    {
      int c;
    }
  }
  b = a;
}

/*
  Expected Output:
  
  Errors. 
  Line:10 "b = a" ==> b is not defined
*/
