#include "rand_story.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//void cp_str(char* story, char* src){
//  story = realloc()
//}

void parse_template(char * line) {
  size_t idx = 0, cat_len = 0;
  char * cat = NULL;
  bool blank = false;

  while (line[idx] != '\0') {
    if (line[idx] != '_' && !blank) {  //not in the blank
      printf("%c", line[idx]);
      idx += 1;
    }
    else if (line[idx] == '_' && !blank) {  //here is a new blank
      idx += 1;
      blank = true;
    }
    else if (line[idx] != '_' && blank) {  //in the blank
      //do something
      size_t start = idx;
      size_t tmp_i = 0;
      while (line[idx] != '\0' && line[idx] != '_') {
        cat_len++;
        idx++;
      }
      cat = realloc(cat, (cat_len + 1) * sizeof(*cat));
      while (start < idx) {
        cat[tmp_i] = line[start];
        tmp_i++;
        start++;
      }
      cat[cat_len] = '\0';
      //idx -= 1;
    }
    else if (line[idx] == '_' && blank) {  //end of blank
      //do something
      //printf("123 \n");
      const char * word = chooseWord(cat, NULL);
      cat_len = 0;
      blank = false;
      idx++;
      printf("%s", word);
    }
  }
  free(cat);
  if (blank == true) {  //no matching '_'
    fprintf(stderr, "Cannot match the '_': %s \n", line);
    exit(EXIT_FAILURE);
  }
}

void read_template(FILE * f) {
  char * line = NULL;
  size_t size_ = 0;
  while (getline(&line, &size_, f) >= 0) {
    parse_template(line);
  }
  free(line);
}
