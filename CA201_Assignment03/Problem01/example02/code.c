#include<stdio.h>
#include<stdlib.h>

typedef struct X {
  int _x;
  struct X *pp;
}X;

void f(X *xx) {
  if(!xx) {
    printf("END\n");
    return;
  }
  printf("%d,", xx->_x);
  f(xx->pp);
}

X *f__(X *xx) {
  if(!xx -> pp) {
    return xx;
  }
  if(!xx -> pp -> pp) {
    X *xx___ = xx -> pp;
    X *xx__ = xx;
    xx___ -> pp = xx__;
    xx__ -> pp = NULL;
    return xx___;   
  }
  X *_xx = xx -> pp;
  X *xx___ = f__(xx -> pp);
  _xx -> pp = xx;
  xx -> pp = NULL;
  return xx___;
}

void f_(int x, X *xx, int __x) {
  if(x && xx) {
    f_(x-1, xx->pp, __x);
    return;
  }
  if (xx) {
    xx->_x = __x;
  }
}


X *f___(int x[]) {
  if(*x==-1) {
    return NULL;
  }
  X *xx = (X *)malloc(sizeof(X));
  xx -> _x = *x;
  X *xx_ = f___(x+1);
  xx -> pp = xx_;
  return xx;  
}

int main(void) {
}
