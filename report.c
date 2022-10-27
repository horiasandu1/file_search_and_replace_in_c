#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "traversal.h"
#include "replace.h"

// FIX WHEN PHRASE IS JUST ONE CHAR LONG

void print_header(char *search_string, char *cwd) {
    char updates_header[10] = "Updates";
    char filename_header[15] = "File Name";

    printf("Target string: %s\n\n", search_string);
    printf("Search begins in current folder: %s\n\n\n", cwd);

    printf("** Search Report **\n");
    printf("\n%-15s%-15s", updates_header, filename_header);
}

void print_footer() {
    // printf("\n\nTRAVERSAL DONE\n\n");
    printf("\n\n");
}

static int compare_func (const void *cpr_1, const void *cpr_2) {
    const struct indiv_file_report *ptr_a = (struct indiv_file_report *)cpr_1;
    const struct indiv_file_report *ptr_b = (struct indiv_file_report *)cpr_2;


    if (ptr_a->replace_counter > ptr_b->replace_counter)
        return -1;
    else if (ptr_a->replace_counter < ptr_b->replace_counter)
        return +1;
    else
        return 0;
}

void print_report_lines(struct indiv_file_report *report_buffer, int buffer_spots_taken) {
    // int str_len;
    // str_len = strlen(cwd);

    // // Add 1 for the first separator
    // str_len++;

    // // Get size of the cwd. Advance the absolute path by x amt of chars to get the path we want to display
    // absolute_path = absolute_path + str_len;

    qsort(report_buffer, buffer_spots_taken, struct_size, compare_func);
    for (int i=0; i<buffer_spots_taken; i++) {

        int str_len;
        str_len = strlen(cwd);

        // Add 1 for the first separator
        str_len++;

        // Get size of the cwd. Advance the absolute path by x amt of chars to get the path we want to display

        printf("\n%-15ld%-15s", report_buffer[i].replace_counter, (report_buffer[i].file_path + str_len));

        // printf("\nENTRY: file_path: %s       replace_counter: %ld\n", report_buffer[i].file_path, report_buffer[i].replace_counter);
    }
}

// void print_report_line(long int replacements_done, char absolute_path[PATH_MAX]) {
//     int str_len;
//     str_len = strlen(cwd);

//     // Add 1 for the first separator
//     str_len++;

//     // Get size of the cwd. Advance the absolute path by x amt of chars to get the path we want to display
//     absolute_path = absolute_path + str_len;

//     printf("\n%-15ld%-15s", replacements_done, absolute_path);
// }