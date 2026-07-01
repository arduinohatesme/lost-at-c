#include <stdio.h>

int nextF(int num, int i, int j) {
  if (num == 1) return i + j;

  return nextF(num - 1, j, i + j);
}

int fibonacci(int num) {
  return nextF(num - 1, 0, 1);
}

int main() {
  printf("Enter fibonacci number to calculate: ");

  int toCalc = 0;
  scanf("%d", &toCalc);
  
  int fibNum = fibonacci(toCalc);
  printf("Fibonacci number %d is: %d\n", toCalc, fibNum);

  return 0;
}
