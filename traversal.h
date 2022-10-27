#ifndef traversal_H_
#define traversal_H_

#include <limits.h>
void initialize_and_start_processing(char *path_to_search);

struct indiv_file_report {
    char file_path[PATH_MAX];
    long int replace_counter;
};

#endif