#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utils.h"

char* read_file(const char* filename) {
    FILE* fp;
    char* buffer;
    long file_size;

    fp = fopen(filename, "rb");
    if (!fp) {
        return NULL;
    }

    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    buffer = (char*) malloc(file_size + 1);
    if (!buffer) {
        fclose(fp);
        return NULL;
    }

    if (fread(buffer, file_size, 1, fp) != 1) {
        fclose(fp);
        free(buffer);
        return NULL;
    }

    fclose(fp);
    buffer[file_size] = '\0';

    return buffer;
}

int write_file(const char* filename, const char* content) {
    FILE* fp;

    fp = fopen(filename, "w");
    if (!fp) {
        return 0;
    }

    fputs(content, fp);

    fclose(fp);

    return 1;
}