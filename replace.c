#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

static int validate_search_string(int arg_count, char* search_string);

int main(int argc, char* argv[]) {
    
    // Get the string to search. Limit this to one arg
    char* search_string = argv[1];
    
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

    printf("Target string: %s\n\n", search_string);
    printf("Search beings in current folder: %s\n\n\n", cwd);
    printf("** Search Report **");






    printf("\n\n\n\n\n");
    exit(0);
}

static int validate_search_string(int arg_count, char* search_string) { 
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
