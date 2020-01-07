#include "paMain.h"

void function1(char* s) {
  printf("--- %s ---\n", __func__);
  printf("%s\n", s);
  printf("---\t---\t---\n");
}

void function2(int n) {
  printf("--- %s ---\n", __func__);
  printf("value x2 %d\n", n*2);
  printf("---\t---\t---\n");
}
