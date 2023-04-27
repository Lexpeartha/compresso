#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli_utils.h"

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

int compress(char* target_file, flag* flags, unsigned int flags_num) {
    printf("Compressing file %s\n", target_file);
    for (int i = 0; i < flags_num; i++) {
        printf("Flag %d: %d, %s\n", i, flags[i].code, flags[i].parameter);
    }
    return 0;
}

int decompress(char* target_file, flag* flags, unsigned int flags_num) {
    printf("Decompressing file %s\n", target_file);
    for (int i = 0; i < flags_num; i++) {
        printf("Flag %d: %d, %s\n", i, flags[i].code, flags[i].parameter);
    }
    return 0;
}