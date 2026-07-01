#include "./cJSON.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void getWord(char *dest) {
  srand(time(NULL));

  FILE *file = fopen("./src/wordle/wordle-answers.json", "r");
  if (!file) {
    printf("Error: could not access answers file.");
    return;
  }

  fseek(file, 0, SEEK_END);
  unsigned long len = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *json_str = malloc(len + 1);
  if (fread(json_str, 1, len, file) < len) {
    printf("Error: reading answers file failed");
  }
  json_str[len] = '\0';
  fclose(file);

  cJSON *arr = cJSON_Parse(json_str);
  free(json_str);

  if (arr == NULL) {
    printf("Error: Array is NULL");
    return;
  }

  if (cJSON_IsArray(arr)) {
    cJSON *word_item =
        cJSON_GetArrayItem(arr, rand() % cJSON_GetArraySize(arr));

    if (cJSON_IsString(word_item) && (word_item->valuestring != NULL)) {
      snprintf(dest, 6, "%s", word_item->valuestring);
    }
  }

  cJSON_Delete(arr);
}

bool binSearch(char (*arr)[6], int i, int j, char tgt[6]) {
  if (j < i) {
    return false;
  }

  int mid = i + (j - i) / 2;
  int cmp = strcmp(arr[mid], tgt);

  if (cmp == 0)
    return true;

  if (cmp > 0)
    return binSearch(arr, i, mid - 1, tgt);

  return binSearch(arr, mid + 1, j, tgt);
}

bool checkInWordList(char word[6]) {
  FILE *file = fopen("./src/wordle/wordle-words.json", "r");
  if (!file) {
    printf("Error: could not access words file.");
    return false;
  }

  fseek(file, 0, SEEK_END);
  unsigned long len = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *json_str = malloc(len + 1);
  if (fread(json_str, 1, len, file) < len) {
    printf("Error: reading words file failed");
  }
  json_str[len] = '\0';
  fclose(file);

  cJSON *arr = cJSON_Parse(json_str);
  int num_words = cJSON_GetArraySize(arr);
  free(json_str);

  if (arr == NULL) {
    printf("Error: Array is NULL");
    return false;
  }

  if (!cJSON_IsArray(arr)) {
    printf("Error: Array is not of type cJSON Array");
    return false;
  }

  char (*conv_arr)[6] = malloc(num_words * sizeof(*conv_arr));
  for (int i = 0; i < num_words; i++) {
    cJSON *item = cJSON_GetArrayItem(arr, i);

    if (cJSON_IsString(item) && (item->valuestring != NULL)) {
      snprintf(conv_arr[i], sizeof(conv_arr[i]), "%s", item->valuestring);
    }
  }

  cJSON_Delete(arr);
  bool res = binSearch(conv_arr, 0, num_words - 1, word);
  free(conv_arr);
  return res;
}

bool singleGuess(char word[6]) {
  printf("> ");

  char input[6];
  bool inWordList = false;

  do {
    if (scanf("%5s", input) != 1) {
      printf("Input only one word > ");
      continue;
    }

    inWordList = checkInWordList(input);

    if (!inWordList) {
      printf("\033[1A\033[2KNot in word list > ");
    }
  } while (!inWordList);

  bool correct = true;
  char *tmpword = malloc(strlen(word) + 1);
  strncpy(tmpword, word, strlen(word));
  printf("\033[1A\033[2K> ");
  for (int i = 0; i < 5; i++) {
    if (tmpword[i] == input[i]) {
      printf("\x1b[32m%c\x1b[0m", input[i]);
      tmpword[i] = '.';

    } else if (strchr(tmpword, input[i])) {
      printf("\x1b[33m%c\x1b[0m", input[i]);
      tmpword[i] = '.';
      correct = false;

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
    bool correct = singleGuess(word);

    if (correct) {
      printf("Great job!");
      return 0;
    }
  }

  printf("Better luck next time! The word was %s.", word);
}
