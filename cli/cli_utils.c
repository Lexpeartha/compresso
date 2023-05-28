#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli_utils.h"
#include "../core/lzw.h"
#include "../core/file_utils.h"

int command_check(char* input, char* cmd_name, int cmd_aliases_len, char* cmd_aliases[cmd_aliases_len]) {
    if (strcmp(input, cmd_name) == 0) {
        return 1;
    }

    for (int i = 0; i < cmd_aliases_len; i++) {
        if (strcmp(input, cmd_aliases[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

file_configuration* init_file_configuration_struct() {
    file_configuration* config = malloc(sizeof(file_configuration));
    config->program_mode = malloc(sizeof(char) * (MAX_LINE_LENGTH / 4));
    config->algorithm = malloc(sizeof(char) * (MAX_LINE_LENGTH / 4));
    config->log_path = malloc(sizeof(char) * (MAX_LINE_LENGTH / 4));
    config->output_path = malloc(sizeof(char) * (MAX_LINE_LENGTH / 4));
    return config;
}

void free_file_configuration_struct(file_configuration* config) {
    if (config == NULL) {
        return;
    }

    if (config->program_mode != NULL) {
        free(config->program_mode);
    }

    if (config->algorithm != NULL) {
        free(config->algorithm);
    }

    if (config->log_path != NULL) {
        free(config->log_path);
    }

    if (config->output_path != NULL) {
        free(config->output_path);
    }

    free(config);
}

int read_config_file(const char* filename, file_configuration* config) {
    printf("Reading config file %s\n", filename);



    char full_path[MAX_LINE_LENGTH/4];
    sprintf(full_path, "../%s", filename);
    FILE* fp = fopen(full_path, "r");
    if (fp == NULL) {
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), fp)) {
        // Remove carriage return character if present (Windows-style line terminator)
        char* p = strchr(line, '\r');
        if (p != NULL) {
            *p = '\0';
        }

        // Parse line content
        char key[32], value[128];
        if (sscanf(line, "%31s \"%127[^\"]\"", key, value) == 2) {
            if (strcmp(key, "program_mode") == 0) {
                strcpy(config->program_mode, value);
            } else if (strcmp(key, "algorithm") == 0) {
                strcpy(config->algorithm, value);
            } else if (strcmp(key, "log_path") == 0) {
                strcpy(config->log_path, value);
            } else if (strcmp(key, "output_path") == 0) {
                strcpy(config->output_path, value);
            }
        }
    }

    fclose(fp);

    return 0;
}

int update_config_file(const char* filename, file_configuration* config) {
    printf("Updating config file %s\n", filename);

    char full_path[MAX_LINE_LENGTH/4];
    sprintf(full_path, "../%s", filename);

    FILE* fp = fopen(full_path, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open file %s\n", filename);
        return 1;
    }

    fprintf(fp, "program_mode \"%s\"\n", config->program_mode);
    fprintf(fp, "algorithm \"%s\"\n", config->algorithm);
    fprintf(fp, "log_path \"%s\"\n", config->log_path);
    fprintf(fp, "output_path \"%s\"\n", config->output_path);

    fclose(fp);

    return 0;
}

int compress(char* target_file, flag* flags, unsigned int flags_num) {
    printf("Compressing file %s\n", target_file);

    compress_lzw(target_file);

    return 0;
}

int decompress(char* target_file, flag* flags, unsigned int flags_num) {
    printf("Decompressing file %s\n", target_file);

    decompress_lzw(target_file, "decompressed.txt");

    return 0;
}

int verify_argument(const char* arg, const char* starts_with) {
    if (strncmp(arg, starts_with, strlen(starts_with)) == 0) {
        return 1;
    }
    return 0;
}