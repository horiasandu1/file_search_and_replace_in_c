#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "replace.h"

static void replace_occurence(int found_start_pos, int found_end_pos, char *filepath);
static void replace_original_file_with_tmpchar(char *filepath, char *tmp_file);

static FILE *original_file;
static FILE *tmp_file;
static long int pos;
static char current_char;
static char upper_search_str_comp_str;
static char tmp_lower_char;
static char replacement_char;

void search_and_replace(char *filepath) {
    printf("Searching file %s", filepath);

    char current_search_string_char;
    char pre_lower_char;
    int occurence_found = 0;
    
    char *new_ptr = search_string;
    

    printf("search string: %s", search_string);

    printf("\nREAD FROM LOWERED TEMP FILE\n");

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
                    // place file position to beginning of occurence
                    // fseek(original_file, found_start_pos, SEEK_SET);
                    // pos = ftell(original_file);

                    // printf("\nOCCURENCE START:\n");
                    
                    // int loop_index;
                    // char occurence_char;

                    // for (loop_index=0; loop_index <= search_pos_span; ++loop_index) {
                    //     pos = ftell(original_file);
                    //     occurence_char = fgetc(original_file);
                    //     printf("%c", occurence_char);

                    //     fputc('x', original_file);

                    //     // Reset position to right before change
                    //     fseek(original_file, pos, SEEK_SET);
                    // }


                    printf("Found an occurence starting at position %ld and ending at %ld\n", found_start_pos, found_end_pos);
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



    printf("\nEOF\n");






    if (occurence_found == 1) {
        replace_occurence(found_start_pos, found_end_pos, filepath);
    }
















    // printf("\nREAD FROM LOWERED TEMP FILE\n");

    // long int found_start_pos = -1;
    // long int found_end_pos = -1;

    // tmp_file = fopen(tmp_path, "r");
    // if (tmp_file == NULL) {
    //     fprintf(stderr, "ERROR: %s. Could not open file.\n\n", strerror(errno));
    //     exit(1);
    // }

    // do {
    //     pos = ftell(tmp_file);
    //     // Get current char


    //     int search_str_len = strlen(new_ptr);
    //     int search_pos_span = (search_str_len -1);

    //     current_search_string_char = tolower(new_ptr[0]);

    //     // guaranteed to be lowercase already
    //     current_char = fgetc(tmp_file);

    //     if ( current_char == current_search_string_char ) {

    //         int substr_index = 1;
    //         found_start_pos = pos;

    //         while (substr_index < search_str_len) {

    //             printf("iteration");
    //             found_end_pos = ftell(tmp_file);
    //             current_search_string_char = tolower(new_ptr[substr_index]);
    //             current_char = fgetc(tmp_file);
    //             if ( current_char == current_search_string_char) {
    //                 substr_index++;
                    
    //             }
    //             else {
    //                 substr_index = 1;
    //                 found_start_pos = -1;
    //                 found_end_pos = -1;
    //                 break;
    //             }

    //             if (found_end_pos - found_start_pos == search_pos_span) {
    //                 // Potential match, check if all are uppercase
    //                 // place file position to beginning of occurence
    //                 fseek(tmp_file, found_start_pos, SEEK_SET);
    //                 pos = ftell(tmp_file);

    //                 printf("\nOCCURENCE START:\n");
                    
    //                 int loop_index;
    //                 char occurence_char;

    //                 for (loop_index=0; loop_index <= search_pos_span; ++loop_index) {
    //                     occurence_char = fgetc(tmp_file);
    //                     printf("%c", occurence_char);
    //                 }

    //                 printf("\nOCCURENCE END:\n");

    //                 printf("Found an occurence starting at position %ld and ending at %ld", found_start_pos, found_end_pos);
    //                 return;
    //             }

    //         }
    //         printf("%c", current_char);
    //     }



    //     if (feof(tmp_file)) {
    //         break;
    //     }
    // }
    // while (1);
    // fclose(tmp_file);

    // printf("\nEOF\n");








}

static void replace_occurence(int found_start_pos, int found_end_pos, char *filepath) {
    printf("\nFound start pos of %d\n", found_start_pos);
    printf("\nFound end pos of %d\n", found_end_pos);

    // Create temp absolute file path
    char tmp_path[PATH_MAX];
    strcpy(tmp_path, filepath);
    strcat(tmp_path, "_tmp");

    printf("\ntmp file: %s\n", tmp_path);

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
        printf("%c", current_char);


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
        printf("%c", current_char);


        if (feof(tmp_file)) {
            break;
        }
        
        fputc(current_char, original_file);
    }
    while (1);
    fclose(original_file);
    fclose(tmp_file);
}