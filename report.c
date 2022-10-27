#include <limits.h>
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

void print_report_lines(struct indiv_file_report *report_buffer, int buffer_size) {
    // int str_len;
    // str_len = strlen(cwd);

    // // Add 1 for the first separator
    // str_len++;

    // // Get size of the cwd. Advance the absolute path by x amt of chars to get the path we want to display
    // absolute_path = absolute_path + str_len;

    
    for (int i=0; i<buffer_size; i++) {
        printf("\nENTRY: file_path: %s       replace_counter: %ld\n", report_buffer[i].file_path, report_buffer[i].replace_counter);
    }
}

void print_report_line(long int replacements_done, char absolute_path[PATH_MAX]) {
    int str_len;
    str_len = strlen(cwd);

    // Add 1 for the first separator
    str_len++;

    // Get size of the cwd. Advance the absolute path by x amt of chars to get the path we want to display
    absolute_path = absolute_path + str_len;

    printf("\n%-15ld%-15s", replacements_done, absolute_path);
}