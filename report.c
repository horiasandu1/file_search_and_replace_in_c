#include <stdio.h>



void print_header(char *search_string, char *cwd) {
    char updates_header[10] = "Updates";
    char filename_header[15] = "File Name";

    printf("Target string: %s\n\n", search_string);
    printf("Search begins in current folder: %s\n\n\n", cwd);

    printf("** Search Report **\n");
    printf("\n%-15s%-15s\n", updates_header, filename_header);
}

void print_footer() {
    printf("\n\nTRAVERSAL DONE\n\n");
    printf("\n\n\n");
}

void print_report_line() {
    char updates_header[10] = "Updates";
    char filename_header[15] = "File Name";
    printf("\n%-15s%-15s\n", updates_header, filename_header);
}