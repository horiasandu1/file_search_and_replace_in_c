#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

#include "text.h"
#include "report.h"
#include "traversal.h"

static const char *FILE_SEP = "/"; 

void traverse_and_process_all_files(char *path_to_search);

static struct indiv_file_report* process_txt_files_only(char *filename, char *filepath);
static struct indiv_file_report *current_struct_ptr;
const size_t struct_size = sizeof(struct indiv_file_report);

static int allocated_buffer_index = -1;
static int buffer_size = 5;
static int avail_buffer_size = 5;

int buffer_spots_taken = 0;
struct indiv_file_report *report_buffer;

void initialize_and_start_processing(char *path_to_search) {
    // Start with 5 and resize if needed since I've read realloc calls are expensive
    report_buffer = malloc(buffer_size * struct_size);
    // struct indiv_file_report* ptr_ptr = report_buffer;

    traverse_and_process_all_files(path_to_search);


    print_report_lines(report_buffer, buffer_spots_taken);
    free(report_buffer);
}




// Problem to solve: recursive realloc


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
            current_struct_ptr = process_txt_files_only(entry_name, path_to_search);

            if (current_struct_ptr->replace_counter > -1) {

                allocated_buffer_index++;
                // report_buffer[allocated_buffer_index] = *current_struct_ptr;

                
                memcpy(&report_buffer[allocated_buffer_index], current_struct_ptr, struct_size);
                buffer_spots_taken++;
                avail_buffer_size--;

                if (avail_buffer_size < 2) {
                    buffer_size = buffer_size + 5;
                    avail_buffer_size = avail_buffer_size + 5;

                    report_buffer = realloc(report_buffer, (buffer_size * struct_size));

                    if (report_buffer == NULL) {
                        fprintf(stderr, "ERROR: %s. Realloc failed\n\n", strerror(errno));
                        exit(1);
                    }
                }
                
            }
        }

    }
    closedir(directory_stream);
}

static struct indiv_file_report* process_txt_files_only(char *filename, char *filepath) {

    int file_len = strlen(filename);
    struct indiv_file_report current_report_struct = {.file_path = "foo", .replace_counter = -1};

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
                // print_report_line(replacements_done, absolute_path);

                // current_report_struct.file_path = absolute_path;
                strcpy(current_report_struct.file_path, absolute_path);


                current_report_struct.replace_counter = replacements_done;

                // Delete tmp file
                strcat(absolute_path, "_tmp");
                remove(absolute_path);
            }
            // printf("done");
        }
    }
    current_struct_ptr = &current_report_struct;
    return current_struct_ptr;
}