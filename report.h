#ifndef report_H_
#define report_H_

#include "traversal.h"

void print_report_lines(struct indiv_file_report *report_buffer, int buffer_spots_taken);
void print_header(char *search_string, char *cwd);
void print_footer();
// void print_report_line();
#endif