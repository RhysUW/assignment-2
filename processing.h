#ifndef PROCESSING_H
#define PROCESSING_H

#include "input_handling.h"

void process_data(char **questions, int *num_questions, char **likert_items, int *num_likert_items,
                  char **direct_reverse, int counts[][MAX_LIKERT_ITEMS], int *num_respondents,
                  Response *responses, int *input_type);

#endif // PROCESSING_H
