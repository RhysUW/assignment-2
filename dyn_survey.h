#ifndef _DYN_SURVEY_H_
#define _DYN_SURVEY_H_

/* add your library includes, constants and typedefs here*/

#define MAX_LINE_LENGTH 10000    // Maximum length of a line
#define MAX_QUESTIONS 100        // Maximum number of questions
#define MAX_LIKERT_ITEMS 10      // Maximum number of Likert items
#define MAX_RESPONDENTS 1000     // Maximum number of respondents
#define MAX_DEMOGRAPHIC_LENGTH 50
#define MAX_FILTERS 10  // Maximum number of filters

/**
 * Stores demographic data and responses for a single respondent.
 */
typedef struct {
    char major[MAX_DEMOGRAPHIC_LENGTH];
    char yes_no[MAX_DEMOGRAPHIC_LENGTH];
    char date[MAX_DEMOGRAPHIC_LENGTH];
    int responses[MAX_QUESTIONS]; // Stores numerical responses to questions
} Respondent;

/**
 * Stores a respondent's data along with computed scores.
 */
typedef struct {
    Respondent respondent; // Contains demographic data and responses
    double scores[5];      // Stores computed scores (C, I, G, U, P)
} Response;

/**
 * Represents a filter to be applied to the respondents' data.
 *
 * This struct contains the field index to filter on and the value to match.
 * Field indices correspond to:
 * 0 - major
 * 1 - yes_no
 * 2 - date
 */
typedef struct {
    int field_index; // 0-based index of the field to filter on
    char value[MAX_DEMOGRAPHIC_LENGTH];
} Filter;

#endif
