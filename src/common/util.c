#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/util.h"

char* read_shader(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return NULL;
    }

    char* buffer = NULL;
    char* temp = NULL;
    size_t buffer_size = 0;
    size_t line_length;
    char line[128];  // Adjust the buffer size as needed

    while (fgets(line, sizeof(line), file) != NULL) {
        line_length = strlen(line);
        temp = realloc(buffer, buffer_size + line_length + 1);

        if (temp == NULL) {
            fclose(file);
            free(buffer);
            fprintf(stderr, "Failed to allocate memory.\n");
            return NULL;
        }

        buffer = temp;
        strcpy(buffer + buffer_size, line);
        buffer_size += line_length;
    }

    fclose(file);
    return buffer;
}

char* get_dynamic_title(char* additional) {
    char* base = "Vortex Engine (OpenGL 4.6 Renderer) | ";

    unsigned int base_len = strlen(base);
    unsigned int additional_len = strlen(additional);
    char* result = (char*)malloc((base_len + additional_len + 1) * sizeof(char));

    strcpy(result, base);
    strcat(result, additional);
    return result;
}
