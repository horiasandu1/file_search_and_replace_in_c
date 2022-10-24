#include <stdio.h>



void print_header(char *search_string, char *cwd) {
    printf("** Search Report **\n\n");

    printf("Target string: %s\n\n", search_string);
    printf("Search begins in current folder: %s\n\n\n", cwd);
}

void print_footer() {
    printf("\n\nTRAVERSAL DONE\n\n");
    printf("\n\n\n");
}