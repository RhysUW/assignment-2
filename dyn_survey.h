#ifndef _DYN_SURVEY_H_
#define _DYN_SURVEY_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 10000    // Maximum length of a line
#define MAX_QUESTIONS 100        // Maximum number of questions
#define MAX_LIKERT_ITEMS 10      // Maximum number of Likert items
#define MAX_RESPONDENTS 1000     // Maximum number of respondents

void process_data(char **questions, int *num_questions, char **likert_items, int *num_likert_items,
                  char **direct_reverse, int counts[][MAX_LIKERT_ITEMS], int *num_respondents,
                  double respondent_scores[][5], int *input_type);

void print_percentages(char **questions, int num_questions, char **likert_items, int num_likert_items,
                       int counts[][MAX_LIKERT_ITEMS], int num_respondents, int input_type);

void print_scores(int num_respondents, double respondent_scores[][5], int input_type);

/* add your library includes, constants and typedefs here*/
int main() {
    char *questions[MAX_QUESTIONS];
    int num_questions = 0;
    char *direct_reverse[MAX_QUESTIONS];
    char *likert_items[MAX_LIKERT_ITEMS];
    int num_likert_items = 0;
    int counts[MAX_QUESTIONS][MAX_LIKERT_ITEMS] = {0};
    int num_respondents = 0;
    double respondent_scores[MAX_RESPONDENTS][5];

    int input_type = 0;

    // Process the data
    process_data(questions, &num_questions, likert_items, &num_likert_items,
                 direct_reverse, counts, &num_respondents, respondent_scores, &input_type);

    if (input_type == 1 || input_type == 2) {
        print_percentages(questions, num_questions, likert_items, num_likert_items,
                          counts, num_respondents, input_type);
    } else if (input_type == 3) {
        print_scores(num_respondents, respondent_scores, input_type);
    } else {
        print_percentages(questions, num_questions, likert_items, num_likert_items,
            counts, num_respondents, input_type);
        print_scores(num_respondents, respondent_scores, input_type);

    }

    return 0;
}




#endif
