#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

#include "traversal.h"
#include "report.h"
#include "replace.h"

static int validate_search_string(int arg_count, char *search_string);

char *search_string;

int main(int argc, char *argv[]) {
    
    // Get the string to search. Limit this to one arg
    search_string = argv[1];
    
    int val_res = validate_search_string(argc, search_string);
    if (val_res != 0) {
        exit(val_res);
    }
    // Use PATH_MAX from the limits lib for a sane default max size
    char cwd[PATH_MAX];

    // Error handle finding the cwd
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        fprintf(stderr, "ERROR: %s. The current working directory could not be determined or the size of the buffer to hold the result was too small.\n\n", strerror(errno));
        exit(1);
    }

    print_header(search_string, cwd);
    
    traverse_and_process_all_files(cwd);
    print_footer();
    exit(0);
}

static int validate_search_string(int arg_count, char *search_string) { 
    if (arg_count != 2) {
        printf("This program takes exactly one argument.\n\n");
        return 1;
    }

    char first_char = search_string[0];

    if (first_char == '\0') {
        printf("The argument provided was empty\n\n");
        return 1;   
    }
    return 0;
}
