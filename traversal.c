#include <stdio.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>

#include "text.h"
#include "report.h"

static const char *FILE_SEP = "/"; 

static void process_txt_files_only(char *filename, char *filepath);

void traverse_and_process_all_files(char *path_to_search) {
    DIR *directory_stream;
    struct dirent *directory_entry;

    directory_stream = opendir(path_to_search);

    while ((directory_entry = readdir(directory_stream)) != NULL) {
        char *entry_name = directory_entry->d_name;
        

        if (directory_entry->d_type == DT_DIR) {

            // Ignoring these links for the purpose of this assignment, would cause an infinite loop
            if ((strcmp(entry_name, ".") != 0) && (strcmp(entry_name, "..") != 0)) {
                // printf("\nENTRY: %s TYPE: %s", entry_name, DIRECTORY_TYPE_STR);

                // Make a copy of path to preserve original path even with recursion
                char new_path[PATH_MAX];
                strcpy(new_path, path_to_search);

                // Add file separator
                strcat(new_path, FILE_SEP);

                // Add subdir
                strcat(new_path, entry_name);

                traverse_and_process_all_files(new_path);
            }
        } else {
            process_txt_files_only(entry_name, path_to_search);
        }

    }

    closedir(directory_stream);
}

static void process_txt_files_only(char *filename, char *filepath) {

    int file_len = strlen(filename);

    if (file_len > 3) {

        // Pointer to memory addr where the string starts
        // char *full_addr = &filename[0];

        // Pointer to memory addr 4 chars before the string ends
        char *last_four = &filename[file_len - 4];

        if ((strcmp(last_four, ".txt") == 0) || (strcmp(last_four, ".TXT") == 0)) {
            char absolute_path[PATH_MAX];
            strcpy(absolute_path, filepath);

            // Add file separator
            strcat(absolute_path, FILE_SEP);

            // Add filename
            strcat(absolute_path, filename);

            occurence_found = 0;
            replacements_done = 0;
            // printf("\nENTRY: %s\n", absolute_path);
            // printf("\nOccurence found ? %d\n", occurence_found);

            while (occurence_found >= 0 ){
                search_and_replace(absolute_path);
            }

            // Print report line here if relevant
            if (replacements_done > 0){
                print_report_line(replacements_done, absolute_path);

                // Delete tmp file
                strcat(absolute_path, "_tmp");
                remove(absolute_path);
            }
            // printf("done");
        }
        
        // printf("\nENTRY: %s TYPE: %s", filename, FILE_TYPE_STR);
    }
}