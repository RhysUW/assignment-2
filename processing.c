// processing.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "processing.h"
#include "input_handling.h"

/**
 * This function reads the survey data from the standard input, processes
 * the questions, Likert items, respondents' responses, and applies any
 * filters. It computes the counts for each Likert item per question and
 * calculates the average scores per respondent.
 *
 * @param questions Array to store the survey questions.
 * @param num_questions Pointer to store the number of questions.
 * @param likert_items Array to store the Likert scale items.
 * @param num_likert_items Pointer to store the number of Likert items.
 * @param direct_reverse Array to store whether each question is direct or reverse.
 * @param counts 2D array to store counts of responses for each question and Likert item.
 * @param num_respondents Pointer to store the number of respondents.
 * @param responses Array to store the processed responses and computed scores.
 * @param input_type Pointer to store the input type (1, 2, or 3).
 */
void process_data(char **questions, int *num_questions, char **likert_items, int *num_likert_items,
                  char **direct_reverse, int counts[][MAX_LIKERT_ITEMS], int *num_respondents,
                  Response *responses, int *input_type) {
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

    // Initialize filter variables
    Filter filters[MAX_FILTERS];
    int num_filters = 0; // Number of filters applied

    // Temporary storage for respondents
    Response temp_responses[MAX_RESPONDENTS];
    int total_respondents = 0; // Total number of respondents read

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline character
        line[strcspn(line, "\n")] = '\0';

        // Skip lines starting with '#'
        if (line[0] == '#') {
            continue;
        }

        // Check for input type
        if (strstr(line, "1,0,0") != NULL) {
            *input_type = 1;
            continue;
        } else if (strstr(line, "0,1,0") != NULL) {
            *input_type = 2;
            continue;
        } else if (strstr(line, "0,0,1") != NULL) {
            *input_type = 3;
            continue;
        } else if (strstr(line, "1,1,1") != NULL) {
            *input_type = 3;
            continue;
        }

        // Check for the question section
        if (!in_question_section && strstr(line, "C1.") != NULL) {
            in_question_section = 1;
        }

        // Process question section
        if (in_question_section && !in_direct_reverse_section) {
            // Existing code to process questions...
            // [Include the code as before]
            // [No changes needed in this section]
            char *token = strtok(line, ";");
            while (token != NULL && *num_questions < MAX_QUESTIONS) {
                while (*token == ' ') token++;
                size_t len = strlen(token);
                while (len > 0 && (token[len - 1] == ' ' || token[len - 1] == '\n')) {
                    token[--len] = '\0';
                }
                questions[*num_questions] = malloc(len + 1);
                strcpy(questions[*num_questions], token);

                // Extract question label
                char *label_end = strstr(token, ".");
                if (label_end != NULL) {
                    size_t label_len = label_end - token;
                    question_labels[*num_questions] = malloc(label_len + 1);
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
            // Existing code to process direct/reverse...
            // [No changes needed in this section]
            char *token = strtok(line, ";");
            int i = 0;
            while (token != NULL && i < *num_questions) {
                while (*token == ' ') token++;
                size_t len = strlen(token);
                while (len > 0 && (token[len - 1] == ' ' || token[len - 1] == '\n')) {
                    token[--len] = '\0';
                }
                direct_reverse[i] = malloc(len + 1);
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
            // Existing code to process Likert items...
            // [No changes needed in this section]
            char *token = strtok(line, ",");
            int value = 1;
            while (token != NULL && *num_likert_items < MAX_LIKERT_ITEMS) {
                size_t len = strlen(token);
                while (len > 0 && (token[len - 1] == ' ' || token[len - 1] == '\n')) {
                    token[--len] = '\0';
                }

                likert_items[*num_likert_items] = malloc(len + 1);
                strcpy(likert_items[*num_likert_items], token);

                // Assign numerical value
                likert_values[*num_likert_items] = value;
                value++;
                (*num_likert_items)++;
                token = strtok(NULL, ",");
            }
            in_likert_section = 0;
            in_response_section = 1;
            continue;
        }

        // Check for filter lines
        if (in_response_section && isdigit(line[0]) && line[1] == ',') {
            // This is a filter line
            if (num_filters < MAX_FILTERS) {
                char *token = strtok(line, ",");
                if (token != NULL) {
                    filters[num_filters].field_index = atoi(token);
                    token = strtok(NULL, ",");
                    if (token != NULL) {
                        strncpy(filters[num_filters].value, token, MAX_DEMOGRAPHIC_LENGTH - 1);
                        filters[num_filters].value[MAX_DEMOGRAPHIC_LENGTH - 1] = '\0';
                        num_filters++;
                    }
                }
            } else {
                fprintf(stderr, "Maximum number of filters exceeded.\n");
            }
            continue; // Skip further processing of this line
        }

        // Process respondents
        if (in_response_section) {
            if (line[0] == '#' || strlen(line) == 0) {
                continue;
            }

            char *token;
            char *saveptr;
            token = strtok_r(line, ",", &saveptr);

            // Use a temporary Response object
            Response temp_response;

            // Process demographic data into temp_response
            if (token != NULL) {
                strncpy(temp_response.respondent.major, token, MAX_DEMOGRAPHIC_LENGTH - 1);
                temp_response.respondent.major[MAX_DEMOGRAPHIC_LENGTH - 1] = '\0';
                token = strtok_r(NULL, ",", &saveptr);
            }
            if (token != NULL) {
                strncpy(temp_response.respondent.yes_no, token, MAX_DEMOGRAPHIC_LENGTH - 1);
                temp_response.respondent.yes_no[MAX_DEMOGRAPHIC_LENGTH - 1] = '\0';
                token = strtok_r(NULL, ",", &saveptr);
            }
            if (token != NULL) {
                strncpy(temp_response.respondent.date, token, MAX_DEMOGRAPHIC_LENGTH - 1);
                temp_response.respondent.date[MAX_DEMOGRAPHIC_LENGTH - 1] = '\0';
                token = strtok_r(NULL, ",", &saveptr);
            }

            // Process responses into temp_response
            int valid_response = 1;
            for (int q = 0; q < *num_questions; q++) {
                if (token == NULL) {
                    valid_response = 0;
                    break;
                }
                // Find the index of the response in likert_items
                int response_index = -1;
                for (int l = 0; l < *num_likert_items; l++) {
                    if (strcmp(token, likert_items[l]) == 0) {
                        response_index = l;
                        break;
                    }
                }

                if (response_index == -1) {
                    valid_response = 0;
                    break;
                }

                // Get the numerical value
                int numerical_value = likert_values[response_index];

                // Check if question is Reverse
                if (strcmp(direct_reverse[q], "Reverse") == 0) {
                    numerical_value = 7 - numerical_value;
                }

                temp_response.respondent.responses[q] = numerical_value;

                token = strtok_r(NULL, ",", &saveptr);
            }

            if (!valid_response) {
                continue; // Skip invalid response
            }

            // Store the respondent temporarily
            temp_responses[total_respondents++] = temp_response;
        }
    }

    // Now process the stored respondents, applying filters
    for (int i = 0; i < total_respondents; i++) {
        int include_respondent = 1;
        for (int f = 0; f < num_filters; f++) {
            if (!respondent_matches_filter(&temp_responses[i].respondent, &filters[f])) {
                include_respondent = 0;
                break; // No need to check other filters if one fails
            }
        }

        if (include_respondent) {
            // Include the respondent
            responses[*num_respondents] = temp_responses[i];

            // Update counts
            for (int q = 0; q < *num_questions; q++) {
                int numerical_value = responses[*num_respondents].respondent.responses[q];

                // Reverse the numerical value if necessary to get the original response index
                int value_to_match = numerical_value;
                if (strcmp(direct_reverse[q], "Reverse") == 0) {
                    value_to_match = 7 - numerical_value;
                }

                // Find the response index
                for (int l = 0; l < *num_likert_items; l++) {
                    if (likert_values[l] == value_to_match) {
                        counts[q][l]++;
                        break;
                    }
                }
            }

            // Compute scores
            if (*input_type == 2 || *input_type == 3) {
                double C_sum = 0.0, I_sum = 0.0, G_sum = 0.0, U_sum = 0.0, P_sum = 0.0;
                for (int j = 0; j < num_C; j++) {
                    C_sum += responses[*num_respondents].respondent.responses[C_indices[j]];
                }
                for (int j = 0; j < num_I; j++) {
                    I_sum += responses[*num_respondents].respondent.responses[I_indices[j]];
                }
                for (int j = 0; j < num_G; j++) {
                    G_sum += responses[*num_respondents].respondent.responses[G_indices[j]];
                }
                for (int j = 0; j < num_U; j++) {
                    U_sum += responses[*num_respondents].respondent.responses[U_indices[j]];
                }
                for (int j = 0; j < num_P; j++) {
                    P_sum += responses[*num_respondents].respondent.responses[P_indices[j]];
                }

                // Compute average scores
                responses[*num_respondents].scores[0] = C_sum / num_C;
                responses[*num_respondents].scores[1] = I_sum / num_I;
                responses[*num_respondents].scores[2] = G_sum / num_G;
                responses[*num_respondents].scores[3] = U_sum / num_U;
                responses[*num_respondents].scores[4] = P_sum / num_P;
            }

            // Increment num_respondents
            (*num_respondents)++;
        }
    }
}
