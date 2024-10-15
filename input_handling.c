#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processing.h"
#include "input_handling.h"

/**
 * Checks if a respondent matches a given filter criterion.
 *
 * @param respondent Pointer to the `Respondent` struct containing demographic data.
 * @param filter Pointer to the `Filter` struct specifying the field and value to match.
 * @return `1` if the respondent matches the filter; `0` otherwise.
 */
int respondent_matches_filter(Respondent *respondent, Filter *filter) {
    switch (filter->field_index) {
        case 0:
            // Major
            return strcmp(respondent->major, filter->value) == 0;
        case 1:
            // yes_no
            return strcmp(respondent->yes_no, filter->value) == 0;
        case 2:
            // date
            return strcmp(respondent->date, filter->value) == 0;
        default:
            // Invalid field index
            return 0;
    }
}