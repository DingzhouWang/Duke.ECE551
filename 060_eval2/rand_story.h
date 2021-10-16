#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>
#include <stdlib.h>

#include "provided.h"

//any functions you want your main to use
//step1
void parse_template(char * line, catarray_t * cats, category_t * record, int reuse);
void read_template(FILE * f, catarray_t * cats, int reuse);
void freeRecord(category_t * record);

//step2
char * parse_input_inst(char * line);
char * parse_input_cat(char * line);
int contain_cat(catarray_t * cats, char * cat);
catarray_t * parse_cat_file(FILE * f);
void freeCat(catarray_t * cats);

//step3
const char * My_Choose_Word(char * cat, catarray_t * cats, category_t * record);

//step4
void update_cats(catarray_t * cats, char * cat, const char * word);
#endif
