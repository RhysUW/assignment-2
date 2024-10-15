#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "output.h"
#include "dyn_survey.h"
#include "input_handling.h"

/**
 * Prints the relative percentage frequencies of responses for each survey question.
 *
 * @param questions Array of survey questions.
 * @param num_questions Number of survey questions.
 * @param likert_items Array of Likert scale items.
 * @param num_likert_items Number of Likert scale items.
 * @param counts 2D array where counts[i][j] represents the number of respondents
 *               who selected likert_items[j] for questions[i].
 * @param num_respondents Total number of respondents included in the survey.
 * @param input_type Input type indicating the type of analysis (1, 2, or 3).
 */
void print_percentages(char **questions, int num_questions, char **likert_items, int num_likert_items,
                       int counts[][MAX_LIKERT_ITEMS], int num_respondents, int input_type) {
    printf("Examining Science and Engineering Students' Attitudes Towards Computer Science\n");
    printf("\nSURVEY RESPONSE STATISTICS\n");
    printf("\nNUMBER OF RESPONDENTS: %d\n", num_respondents);
    if(input_type != 2){
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
        //printf("\n input_type = %d\n", input_type);
    }
}

/**
 * @brief Prints the computed scores for each respondent and the average scores.
 * 
 * @param num_respondents Number of respondents included in the survey.
 * @param responses Array of Response structs containing respondents' data and scores.
 * @param input_type Input type indicating the type of analysis (1, 2, or 3).
 */
void print_scores(int num_respondents, Response *responses, int input_type) {
    if(input_type == 2){
        printf("Examining Science and Engineering Students' Attitudes Towards Computer Science\n");
        printf("\nSURVEY RESPONSE STATISTICS\n");
        printf("\nNUMBER OF RESPONDENTS: %d\n", num_respondents);
        printf("\nSCORES FOR ALL THE RESPONDENTS\n");
    }else if(input_type == 3){
        printf("\nSCORES FOR ALL THE RESPONDENTS\n\n");
    }
    // Print scores for each respondent
    for (int i = 0; i < num_respondents; i++) {
        printf("C:%.2f,I:%.2f,G:%.2f,U:%.2f,P:%.2f\n",
               responses[i].scores[0],
               responses[i].scores[1],
               responses[i].scores[2],
               responses[i].scores[3],
               responses[i].scores[4]);
    }

    if(input_type == 3){
        
        float average[5] = {0.0};
        for(int i = 0; i < num_respondents; i++){
            average[0] += responses[i].scores[0];
            average[1] += responses[i].scores[1];
            average[2] += responses[i].scores[2];
            average[3] += responses[i].scores[3];
            average[4] += responses[i].scores[4];
        }
        for (int i = 0; i < 5; i++) {
            average[i] /= num_respondents;
        }
        printf("\nAVERAGE SCORES PER RESPONDENT\n");
        printf("\nC: %.2f,I: %.2f, G: %.2f, U: %.2f, P: %.2f\n", average[0], average[1], average[2], average[3], average[4]);
    }
}
