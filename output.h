#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dyn_survey.h"

/* add your include and prototypes here*/

void print_percentages(char **questions, int num_questions, char **likert_items, int num_likert_items,
                       int counts[][MAX_LIKERT_ITEMS], int num_respondents, int input_type) {
    printf("Examining Science and Engineering Students' Attitudes Towards Computer Science\n");
    printf("\nSURVEY RESPONSE STATISTICS\n");
    printf("\nNUMBER OF RESPONDENTS: %d\n", num_respondents);
    if(input_type != 1){
    printf("\nFOR EACH QUESTION BELOW, RELATIVE PERCENTUAL FREQUENCIES ARE COMPUTED FOR EACH LEVEL OF AGREEMENT\n");
    }
    for (int i = 0; i < num_questions; i++) {
        printf("\n%s\n", questions[i]);
        for (int j = 0; j < num_likert_items; j++) {
            double percentage = 0.0;
            if (num_respondents > 0) {
                percentage = ((double)counts[i][j] / num_respondents) * 100.0;
            }
            printf("%.2f: %s\n", percentage, likert_items[j]);
        }
        printf("\n");
    }
}

void print_scores(int num_respondents, double respondent_scores[][5], int input_type) {
    if(input_type == 3){
    printf("Examining Science and Engineering Students' Attitudes Towards Computer Science\n");
    printf("\nSURVEY RESPONSE STATISTICS\n");
    printf("\nNUMBER OF RESPONDENTS: %d\n", num_respondents);
    printf("\nSCORES FOR ALL THE RESPONDENTS\n");
    }
    // Print scores for each respondent
    for (int i = 0; i < num_respondents; i++) {
        printf("C:%.2f,I:%.2f,G:%.2f,U:%.2f,P:%.2f\n",
               respondent_scores[i][0],
               respondent_scores[i][1],
               respondent_scores[i][2],
               respondent_scores[i][3],
               respondent_scores[i][4]);
    }

    if(input_type == 4){
    float average1, average2, average3, average4, average5 = 0.0;
    for(int i =0; i < num_respondents; i++){
        average1 += respondent_scores[i][0];
        average2 += respondent_scores[i][1];
        average3 += respondent_scores[i][2];
        average4 += respondent_scores[i][3];
        average5 += respondent_scores[i][4];
    }
    average1 = average1/num_respondents;
    average2 = average2/num_respondents;
    average3 = average3/num_respondents;
    average4 = average4/num_respondents;
    average5 = average5/num_respondents;
    printf("\nC: %.2f,I: %.2f, G: %.2f, U: %.2f, P: %.2f\n", average1, average2,
        average3, average4, average5);
    }
}


#endif
