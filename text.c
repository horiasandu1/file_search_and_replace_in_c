#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "replace.h"

static void replace_occurence(int found_start_pos, int found_end_pos, char *filepath);
static void replace_original_file_with_tmpchar(char *filepath, char *tmp_file);
static void process_single_char_replacement(char *filepath, char *tmp_file, char *search_string);

static FILE *original_file;
static FILE *tmp_file;
static long int pos;
static char current_char;
static char upper_search_str_comp_str;
static char replacement_char;

int occurence_found;
long int replacements_done;

void search_and_replace(char *filepath) {
    // printf("Searching file %s", filepath);

    char current_search_string_char;
    char pre_lower_char;
    
    // Check if this is really necessary
    char *new_ptr = search_string;

    long int found_start_pos = -1;
    long int found_end_pos = -1;

    original_file = fopen(filepath, "a+");
    fseek(original_file, 0, SEEK_SET);
    if (original_file == NULL) {
        fprintf(stderr, "ERROR: %s. Could not open file.\n\n", strerror(errno));
        exit(1);
    }

    do {
        pos = ftell(original_file);
        // Get current char
        int search_str_len = strlen(new_ptr);
        int search_pos_span = (search_str_len -1);
        int is_fully_upper = 1;

        current_search_string_char = tolower(new_ptr[0]);

        // guaranteed to be lowercase already MAYBE
        pre_lower_char = fgetc(original_file);
        current_char = tolower(pre_lower_char);

        if ( current_char == current_search_string_char ) {

            int substr_index = 1;
            found_start_pos = pos;
            upper_search_str_comp_str = toupper(new_ptr[0]);
            if (upper_search_str_comp_str != pre_lower_char) {
                is_fully_upper = 0;
            }

            if (search_str_len == 1){
                char tmp_path[PATH_MAX];
                strcpy(tmp_path, filepath);
                strcat(tmp_path, "_tmp");
                fclose(original_file);
                process_single_char_replacement(filepath, tmp_path, new_ptr);
                break;
            }

            while (substr_index < search_str_len) {

                // printf("iteration");
                found_end_pos = ftell(original_file);

                current_search_string_char = tolower(new_ptr[substr_index]);
                upper_search_str_comp_str = toupper(new_ptr[substr_index]);

                pre_lower_char = fgetc(original_file);
                current_char = tolower(pre_lower_char);


                if ( current_char == current_search_string_char) {
                    substr_index++;

                    if (upper_search_str_comp_str != pre_lower_char) {
                        is_fully_upper = 0;
                    }
                    
                }
                else {
                    substr_index = 1;
                    found_start_pos = -1;
                    found_end_pos = -1;
                    break;
                }

                if (found_end_pos - found_start_pos == search_pos_span) {
                    // printf("Found an occurence starting at position %ld and ending at %ld\n", found_start_pos, found_end_pos);
                    // Potential match, check if all are uppercase

                    // Found but already is uppercase. Restart loop with the position at the end of the str
                    if (is_fully_upper == 1) {
                        fseek(original_file, found_end_pos, SEEK_SET);
                        substr_index = 1;
                        found_start_pos = -1;
                        found_end_pos = -1;
                    }
                    else {
                        occurence_found = 1;
                    }
                    
                    break;
                }

            }
        }

        if (feof(original_file) || occurence_found == 1) {
            break;
        }
    }
    while (1);
    fclose(original_file);

    if (occurence_found == 1) {
        replace_occurence(found_start_pos, found_end_pos, filepath);
        occurence_found = 0;
    }
    else {
        occurence_found = -1;
    }

}

static void replace_occurence(int found_start_pos, int found_end_pos, char *filepath) {
    // Create temp absolute file path
    char tmp_path[PATH_MAX];
    strcpy(tmp_path, filepath);
    strcat(tmp_path, "_tmp");

    // Open both files, one read one write
    original_file = fopen(filepath, "r");
    if (original_file == NULL) {
        fprintf(stderr, "ERROR: %s. Could not open file.\n\n", strerror(errno));
        exit(1);
    }

    tmp_file = fopen(tmp_path, "w");
    if (tmp_file == NULL) {
        fprintf(stderr, "ERROR: %s. Could not open file.\n\n", strerror(errno));
        exit(1);
    }
    
    
    do {
        pos = ftell(original_file);
        // Get current char
        current_char = fgetc(original_file);
        // printf("%c", current_char);

        if (feof(original_file)) {
            break;
        }
        
        if (pos >= found_start_pos && pos <= found_end_pos) {
            // Get upper char and write that to tmp file
            replacement_char = toupper(current_char);
            fputc(replacement_char, tmp_file);
        } else {
            fputc(current_char, tmp_file);
        }


    }
    while (1);
    fclose(original_file);
    fclose(tmp_file);

    replace_original_file_with_tmpchar(filepath, tmp_path);

}

static void replace_original_file_with_tmpchar(char *filepath, char *tmp_path) {
    original_file = fopen(filepath, "w");
    ++replacements_done;
    if (original_file == NULL) {
        fprintf(stderr, "ERROR: %s. Could not open file.\n\n", strerror(errno));
        exit(1);
    }

    tmp_file = fopen(tmp_path, "r");
    if (tmp_file == NULL) {
        fprintf(stderr, "ERROR: %s. Could not open file.\n\n", strerror(errno));
        exit(1);
    }
    
    
    do {
        current_char = fgetc(tmp_file);
        // printf("%c", current_char);


        if (feof(tmp_file)) {
            break;
        }
        
        fputc(current_char, original_file);
    }
    while (1);
    fclose(original_file);
    fclose(tmp_file);
}

static void process_single_char_replacement(char *filepath, char *tmp_path, char *search_string) {
    // Leave original file open at end, is closed by breaking out of the outer loop


    original_file = fopen(filepath, "r");
    if (original_file == NULL) {
        fprintf(stderr, "ERROR: %s. Could not open file.\n\n", strerror(errno));
        exit(1);
    }

    tmp_file = fopen(tmp_path, "w");
    if (tmp_file == NULL) {
        fprintf(stderr, "ERROR: %s. Could not open file.\n\n", strerror(errno));
        exit(1);
    }
    
    upper_search_str_comp_str = toupper(*search_string);
    
    do {
        current_char = fgetc(original_file);
        // printf("%c", current_char);


        if (feof(original_file)) {
            break;
        }

        char lower_char = tolower(*search_string);

        // If true, is lowercase
        if (current_char == lower_char) {
            ++replacements_done;
            fputc(upper_search_str_comp_str, tmp_file);
        }
        else{
          fputc(current_char, tmp_file);
        }
        
    }
    while (1);
    fclose(original_file);
    fclose(tmp_file);

    original_file = fopen(filepath, "w");
    if (original_file == NULL) {
        fprintf(stderr, "ERROR: %s. Could not open file.\n\n", strerror(errno));
        exit(1);
    }

    tmp_file = fopen(tmp_path, "r");
    if (tmp_file == NULL) {
        fprintf(stderr, "ERROR: %s. Could not open file.\n\n", strerror(errno));
        exit(1);
    }
    
    
    do {
        current_char = fgetc(tmp_file);

        if (feof(tmp_file)) {
            break;
        }
        
        fputc(current_char, original_file);
    }
    while (1);
    fclose(tmp_file);
}