#include<stdio.h>

int main(void){
  int sum = 18;
  int max=15;
  while(sum>max){
    printf("(sum & max)  = %d\n", (sum & max));
    printf("(sum >> seglen) = %d\n", (sum >> 4));
    sum = (sum & max) + (sum >> 4);
  }
  printf("Final sum = %d\n", sum);

  return 0;
}