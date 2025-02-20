#ifndef _PROCESSING_H_
#define _PROCESSING_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dyn_survey.h"
#include "emalloc.h"

/* add your include and prototypes here*/

void process_data(char **questions, int *num_questions, char **likert_items, int *num_likert_items,
                  char **direct_reverse, int counts[][MAX_LIKERT_ITEMS], int *num_respondents,
                  double respondent_scores[][5], int *input_type) {
    char line[MAX_LINE_LENGTH];
    int in_question_section = 0;
    int in_direct_reverse_section = 0;
    int in_likert_section = 0;
    int in_response_section = 0;
    // Arrays to store indices of questions for each score category
    int C_indices[MAX_QUESTIONS], I_indices[MAX_QUESTIONS], G_indices[MAX_QUESTIONS], U_indices[MAX_QUESTIONS], P_indices[MAX_QUESTIONS];
    int num_C = 0, num_I = 0, num_G = 0, num_U = 0, num_P = 0;
    char *question_labels[MAX_QUESTIONS];
    int likert_values[MAX_LIKERT_ITEMS]; 

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline character
        line[strcspn(line, "\n")] = '\0';

        // Skip lines starting with '#'
        if (line[0] == '#') {
            continue;
        }

        //checking to swee what the input is
        if (strstr(line, "1,0,0,0") != NULL) {
            *input_type = 1;
            continue;
        } else if (strstr(line, "0,1,0,0") != NULL) {
            *input_type = 2;
            continue;
        } else if (strstr(line, "0,0,1,0") != NULL) {
            *input_type = 3;
            continue;
        } else if (strstr(line, "0,1,1,1") != NULL) {
            *input_type = 4;
            continue;
        }

        // Check for the question section
        if (!in_question_section && strstr(line, "C1.") != NULL) {
            in_question_section = 1;
        }

        // Process question section
        if (in_question_section && !in_direct_reverse_section) {
            char *token = strtok(line, ";");
            while (token != NULL && *num_questions < MAX_QUESTIONS) {
                while (*token == ' ') token++;
                size_t len = strlen(token);
                while (len > 0 && (token[len - 1] == ' ' || token[len - 1] == '\n')) {
                    token[--len] = '\0';
                }
                questions[*num_questions] = (char *)malloc(len + 1);
                strcpy(questions[*num_questions], token);

                // Extract question label
                char *label_end = strstr(token, ".");
                if (label_end != NULL) {
                    size_t label_len = label_end - token;
                    question_labels[*num_questions] = (char *)malloc(label_len + 1);
                    strncpy(question_labels[*num_questions], token, label_len);
                    question_labels[*num_questions][label_len] = '\0';
                } else {
                    question_labels[*num_questions] = NULL;
                }

                // Add question index to the appropriate category
                if (question_labels[*num_questions] != NULL) {
                    if (question_labels[*num_questions][0] == 'C') {
                        C_indices[num_C++] = *num_questions;
                    } else if (question_labels[*num_questions][0] == 'I') {
                        I_indices[num_I++] = *num_questions;
                    } else if (question_labels[*num_questions][0] == 'G') {
                        G_indices[num_G++] = *num_questions;
                    } else if (question_labels[*num_questions][0] == 'U') {
                        U_indices[num_U++] = *num_questions;
                    } else if (question_labels[*num_questions][0] == 'P') {
                        P_indices[num_P++] = *num_questions;
                    }
                }
                (*num_questions)++;
                token = strtok(NULL, ";");
            }
            in_question_section = 0;
            in_direct_reverse_section = 1;
            continue;
        }

        // Process direct/reverse section
        if (in_direct_reverse_section && !in_likert_section) {
            char *token = strtok(line, ";");
            int i = 0;
            while (token != NULL && i < *num_questions) {
                while (*token == ' ') token++; 
                size_t len = strlen(token);
                while (len > 0 && (token[len - 1] == ' ' || token[len - 1] == '\n')) {
                    token[--len] = '\0';
                }
                direct_reverse[i] = (char *)malloc(len + 1);
                strcpy(direct_reverse[i], token);
                i++;
                token = strtok(NULL, ";");
            }
            in_direct_reverse_section = 0;
            in_likert_section = 1;
            continue;
        }

        // Process Likert items
        if (in_likert_section && !in_response_section) {
            char *token = strtok(line, ",");
            int value = 1;
            while (token != NULL && *num_likert_items < MAX_LIKERT_ITEMS) {


                size_t len = strlen(token);
                while (len > 0 && (token[len - 1] == ' ' || token[len - 1] == '\n')) {
                    token[--len] = '\0';
                }

                likert_items[*num_likert_items] = (char *)malloc(len + 1);
                strcpy(likert_items[*num_likert_items], token);

                //Assign numerical value
                likert_values[*num_likert_items] = value;
                value++; 
                (*num_likert_items)++;
                token = strtok(NULL, ",");
            }
            in_likert_section = 0;
            in_response_section = 1;
            continue;
        }

        //Process respondent data
        if (in_response_section) {
            if (line[0] == '#') {
                continue;
            }
            char *token;
            char *saveptr;
            int token_index = 0;
            token = strtok_s(line, ",", &saveptr);

            //Skip the first 3 tokens (Major, Yes/No, Date)
            for (int i = 0; i < 3; i++) {
                token = strtok_s(NULL, ",", &saveptr);
                token_index++;
            }

            // Arrays to store numerical responses for this respondent
            int responses[MAX_QUESTIONS];

            // Process responses
            for (int q = 0; q < *num_questions; q++) {

                // Find the index of the response in likert_items
                int response_index = -1;
                for (int l = 0; l < *num_likert_items; l++) {
                    if (strcmp(token, likert_items[l]) == 0) {
                        response_index = l;
                        break;
                    }
                }

                // Get the numerical value
                int numerical_value = likert_values[response_index];

                // Check if question is Reverse
                if (strcmp(direct_reverse[q], "Reverse") == 0) {
                    numerical_value = 7 - numerical_value;
                }

                responses[q] = numerical_value;
                counts[q][response_index]++;
                token = strtok_s(NULL, ",", &saveptr);
                token_index++;
            }

            // Compute scores only if in03.txt or in04.txt are the inputs
            if (*input_type == 3 || *input_type == 4) {
                double C_sum = 0.0, I_sum = 0.0, G_sum = 0.0, U_sum = 0.0, P_sum = 0.0;
                for (int i = 0; i < num_C; i++) {
                    C_sum += responses[C_indices[i]];
                }
                for (int i = 0; i < num_I; i++) {
                    I_sum += responses[I_indices[i]];
                }
                for (int i = 0; i < num_G; i++) {
                    G_sum += responses[G_indices[i]];
                }
                for (int i = 0; i < num_U; i++) {
                    U_sum += responses[U_indices[i]];
                }
                for (int i = 0; i < num_P; i++) {
                    P_sum += responses[P_indices[i]];
                }

                // Compute average scores
                respondent_scores[*num_respondents][0] = C_sum / num_C; 
                respondent_scores[*num_respondents][1] = I_sum / num_I; 
                respondent_scores[*num_respondents][2] = G_sum / num_G; 
                respondent_scores[*num_respondents][3] = U_sum / num_U; 
                respondent_scores[*num_respondents][4] = P_sum / num_P; 
            }

            (*num_respondents)++;
        }
    }
}

#endif
