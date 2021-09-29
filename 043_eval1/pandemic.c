#include "pandemic.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
check_country_name is used to check whether the country name is valid
to be sure the length of the name is less than 64
*/
bool check_country_name(char * begin, char * end) {
  if (end - begin >= 63 || end - begin <= 0) {
    return false;
  }
  return true;
}

/*
isNumber
*/
bool isNumber(char c) {
  if ((c - '0') >= 0 && (c - '0') <= 9) {
    return true;
  }
  return false;
}

/*
check_population is used to check whether the population is valid
to be sure the population is valid(no char in it) 
*/
bool check_population(char * start, char * end) {
  bool num_flag = false;
  bool sign_ = false;
  while (start <= end) {
    if (*start == ' ' && !num_flag) {
      start++;
    }
    else if (!sign_ && *start == '-' && isNumber(*(start + 1))) {
      sign_ = true;
      return true;
    }
    else if (!isNumber(*start) && !num_flag) {
      fprintf(stderr, "The beginning of population is not number! Please check it. \n");
      return false;
    }
    else if (isNumber(*start)) {
      num_flag = true;
      start++;
      return true;
    }
    else if (!isNumber(*start) && num_flag) {
      return true;
    }
  }
  return false;
}

/*
copy country name from line to name
*/
void copy_name(char * start, char * end, char * name) {
  size_t i = 0;
  while (start != end) {
    name[i] = *start;
    start++;
    i++;
  }
  name[i] = '\0';
}

/*
convert
*/
unsigned long convert_population(char * population_start, char * ending) {
  char * s;
  //bool num_flag = false;
  while (population_start != ending) {
    if (isNumber(*population_start) ||
        (*population_start == '-' && isNumber(*(population_start + 1)))) {
      s = population_start;
      //num_flag = true;
      break;
    }
    population_start++;
  }
  unsigned long population = strtoul(s, NULL, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "The population is too large!\n");
    exit(EXIT_FAILURE);
  }
  errno = 0;
  if (population == 0) {
    fprintf(stderr, "The population is 0! Please check it. \n");
    exit(EXIT_FAILURE);
  }
  return population;
}

country_t parseLine(char * line) {
  //WRITE ME
  country_t ans;
  ans.name[0] = '\0';
  ans.population = 0;

  //check the input line is valid
  if (!line) {
    fprintf(stderr, "Input line is empty! Please check it. \n");
    exit(EXIT_FAILURE);
  }

  //try to find the comma
  char * comma_pos = strchr(line, ',');

  //if there is no comma, it's an invalid input
  if (comma_pos == NULL) {
    fprintf(stderr, "Input line don't have comma! Please check it. \n");
    exit(EXIT_FAILURE);
  }

  //check name
  if (check_country_name(line, comma_pos)) {
    char * start = line;
    copy_name(start, comma_pos, ans.name);
  }
  else {
    fprintf(stderr, "name lenght is invalid (too long or empty)! \n");
    exit(EXIT_FAILURE);
  }

  //check population
  char * ending = strchr(line, '\0');
  char * population_start = comma_pos;

  if (check_population(++population_start, ending)) {
    ans.population = convert_population(population_start, ending);
  }
  else {
    fprintf(stderr, "population is invalid! Please check it. \n");
    exit(EXIT_FAILURE);
  }
  //char* end_p;
  //long population_ = strtoul(++comma_pos, &end_p, 10);

  return ans;
}

void calcRunningAvg(unsigned * data, size_t n_days, double * avg) {
  //WRITE ME
  if (data == NULL) {
    fprintf(stderr, "The input data is invalid! Please check it. \n");
    exit(EXIT_FAILURE);
  }
  if (n_days < 7) {
    fprintf(stderr, "The n_days is invalid. Please check it. \n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < (n_days - 6); i++) {
    double x;
    double total = 0;
    for (size_t j = 0; j < 7; j++) {
      x = (double)data[i + j];
      if (x < 0) {
        fprintf(stderr, "The input data is negative! \n");
        exit(EXIT_FAILURE);
      }
      else if (x > 0 && (total + x) <= total) {
        fprintf(stderr, "Double Overflow! \n");
        exit(EXIT_FAILURE);
      }
      else {
        total += x;
      }
    }
    avg[i] = (double)total / 7;
  }
}

void calcCumulative(unsigned * data, size_t n_days, uint64_t pop, double * cum) {
  //WRITE ME
}

void printCountryWithMax(country_t * countries,
                         size_t n_countries,
                         unsigned ** data,
                         size_t n_days) {
  //WRITE ME
}
