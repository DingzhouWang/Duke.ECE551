#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>
#include <stdlib.h>

#include "provided.h"

//any functions you want your main to use
//step1
void parse_template(char * line);
void read_template(FILE * f);

//step2
char * parse_input_inst(char * line);
char * parse_input_cat(char * line);
int contain_cat(catarray_t * cats, char * cat);
catarray_t * parse_cat_file(FILE * f);

//step3

//step4

#endif
