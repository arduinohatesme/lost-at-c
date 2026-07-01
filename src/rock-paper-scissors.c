#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main() {
  printf("Welcome to Rock, Paper, Scissors!\n");
  srand(time(NULL));

  char *poss = "rps";
  char user = 'x';
  char comp = poss[rand() % strlen(poss)];

  while (strchr(poss, user) == NULL) {
    printf("Enter r, p, or s: ");
    scanf(" %c", &user);
  }

  if (user == comp) {
    printf("Well, we tied. Good game!");
    return 0;
  } else if (user >= comp) {
    printf("Wait! That wasn't supposed to happen!");
    return 1;
  } else {
    printf("ezpz.");
    return 0;
  }
}
