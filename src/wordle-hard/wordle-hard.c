#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "./cJSON.h"

void getWord(char *dest) {
  srand(time(NULL));

  FILE *file = fopen("./src/wordle-hard/wordle-words.json", "r");
  if (!file) {
    printf("Error: could not access words file.");
    return;
  }

  fseek(file, 0, SEEK_END);
  unsigned long len = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *json_str = malloc(len + 1);
  fread(json_str, 1, len, file);
  json_str[len] = '\0';
  fclose(file);

  cJSON *arr = cJSON_Parse(json_str);
  free(json_str);

  if (arr == NULL) {
    printf("Error: Array is NULL");
    return;
  }

  if (cJSON_IsArray(arr)) {
    cJSON *word_item = cJSON_GetArrayItem(arr, rand() % cJSON_GetArraySize(arr));

    if (cJSON_IsString(word_item) && (word_item->valuestring != NULL)) {
      snprintf(dest, 6, "%s", word_item->valuestring);
    }
  }

  cJSON_Delete(arr);
}

bool singleGuess(char word[6]) {
  printf("> ");

  char input[6];
  scanf("%5s", input);

  bool correct = true;
  printf("\033[1A\033[2K> ");
  for (int i = 0; i < 5; i++) {
    if (word[i] == input[i]) {
      printf("\x1b[32m%c\x1b[0m", input[i]);
    } else {
      printf("%c", input[i]);
      correct = false;
    }
  }
  printf("\n");

  return correct;
}

int main() {
  printf("---WORDLE---\n");
  char word[6];
  getWord(word);

  for (int i = 0; i < 6; i++) {
    if (singleGuess(word)) {
      printf("Great job!");
      return 0;
    }
  }

  prinf("Better luck next time!");
}
