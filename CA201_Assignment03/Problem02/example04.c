void f() {
  int a;
  {
    int b;
    a = 10;
    {
      int a;
      {
	int c;
	c = a;
      }
      a = c;
    }
  }
  b = a;
}

/*
  Expected Output:
  
  Errors. 
  Line:12 "a = c" ==> c is not defined
  Line:15 "b = a" ==> b is not defined

*/