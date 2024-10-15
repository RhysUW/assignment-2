#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "input_handling.h"
#include "processing.h"
#include "output.h"
#include "input_handling.h"
#include "dyn_survey.h"
#include "input_handling.h"


int main() {
    char *questions[MAX_QUESTIONS];
    int num_questions = 0;
    char *direct_reverse[MAX_QUESTIONS];
    char *likert_items[MAX_LIKERT_ITEMS];
    int num_likert_items = 0;
    int counts[MAX_QUESTIONS][MAX_LIKERT_ITEMS] = {0};
    int num_respondents = 0;
    //double respondent_scores[MAX_RESPONDENTS][5];
    Response responses[MAX_RESPONDENTS]; // Array of Response structs


    int input_type = 0;

    // Process the data
    process_data(questions, &num_questions, likert_items, &num_likert_items,
                 direct_reverse, counts, &num_respondents, responses, &input_type);
    //printf("input type: %d", input_type);

    if (input_type == 1) {
        print_percentages(questions, num_questions, likert_items, num_likert_items,
                          counts, num_respondents, input_type);
    } else if (input_type == 2) {
        print_scores(num_respondents, responses, input_type);
    } else {
        print_percentages(questions, num_questions, likert_items, num_likert_items,
            counts, num_respondents, input_type);
        print_scores(num_respondents, responses, input_type);

    }

    return 0;
}
