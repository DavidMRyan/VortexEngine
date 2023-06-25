#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "../include/util.h"

char* read_shader(const char* p_infile_name) {
    FILE* p_infile = fopen(p_infile_name, "r");
    char* final = NULL;
    size_t final_size = 0;

    if (p_infile != NULL) {
        char line[255];
        while (fgets(line, sizeof(line), p_infile)) {
            // Calculate the new size required for 'final'
            size_t line_length = strlen(line);
            size_t new_size = final_size + line_length + 1;

            // Reallocate memory for 'final' to accommodate the new line
            char* new_final = realloc(final, new_size);
            if (new_final == NULL) {
                perror("Memory allocation error");
                free(final); // Free the previously allocated memory
                fclose(p_infile);
                return NULL;
            }

            final = new_final;
            strcat(final, line); // Concatenate 'line' to 'final'
            final_size = new_size;
        }

        fclose(p_infile);

        // Remove the first character from 'final'
        if (final_size > 1) {
            memmove(final, final + 1, final_size - 1);
            final = realloc(final, final_size);  // Resize the string to remove the extra character
        } else {
            // The string is empty
            *final = '\0';
        }

        return final;
    } else {
        perror("Error opening input file");
        return NULL;
    }
}