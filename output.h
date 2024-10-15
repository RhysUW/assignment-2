#ifndef OUTPUT_H
#define OUTPUT_H

#include "input_handling.h"

void print_percentages(char **questions, int num_questions, char **likert_items, int num_likert_items,
                       int counts[][MAX_LIKERT_ITEMS], int num_respondents, int input_type);

void print_scores(int num_respondents, Response *responses, int input_type);

#endif // OUTPUT_H
