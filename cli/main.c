#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli_utils.h"

int cli_main(int argc, char *argv[]) {
    // If not specified, by default the program will compress the file
    // TODO: Update based on configuration
    command_code command = UNKNOWN;
    // Flag vector to store all flags used in the command
    unsigned int flags_num = 0;
    flag* flags = calloc(0, sizeof(flag));
    if (flags == NULL) {
        printf("MEMORY ALLOCATION FAILED\n");
        exit(1);
    }
    // Whitelist of possible non-commands on which we don't want program to break
    char* cmd_exceptions[] = {"-DUSE_CLI", "-DUSE_UI"};
    // Default file config path
    const char* config_path = "compresso.cfg";
    file_configuration* config = init_file_configuration_struct();
    int error_while_reading = read_config_file(config_path, config);

    if (error_while_reading) {
        printf("FAILED TO READ CONFIG FILE\n");
        exit(4);
    }

    /* IMPORTANT FLAGS THAT SKIP REGULAR EXECUTION */
    for (int i = 1; i < argc; i++) {
        char* current_arg = argv[i];
        if (command_check(current_arg, "--help", 1, (char *[]) {"-h"})) {
            // Detecting help command will print help info and finish the program
            // Makes flag queue irrelevant
            command = HELP;
            printf("Usage: ./compresso [cmd] [options]\n");
            printf("Options:\n");
            printf("  --help, -h    Display this help message\n");
            printf("  --about, -a   Display information about this program\n");
            exit(0);
        } else if (command_check(current_arg, "--about", 1, (char *[]) {"-a"})) {
            // Detecting about command will print about info and finish the program
            // Makes flag queue irrelevant
            command = ABOUT;
            printf("Compresso is a compression tool written in C.\n");
            printf("It is a university project at Faculty Of Electrical Engineering for a course"
                   " called Practicum in Programming 2, which covers practical usages of C programming language.\n");
            printf("\n");
            printf("It is written by:\n");
            printf("  - Aleksa Sević\n");
            printf("  - Lazar Senović\n");
            printf("  - Petar Rančić\n");
            printf("\n");
            printf("Under the mentorship of:\n");
            printf("  - professor Marko Mišić\n");
            printf("  - tutoring assistant Tamara Šekularac\n");
            printf("  - tutoring assistant Miloš Obradović\n");
            exit(0);
        }
    }
    /* REGULAR COMMANDS AND FLAGS */
    char* target_file = argv[1];
    for (int i = 2; i < argc; i++) {
        char *current_arg = argv[i];
        if (command_check(current_arg, "--compress", 1, (char*[]){"-c"})) {
            if (command == DECOMPRESS) {
                printf("FALSE USAGE: Cannot use both --compress and --decompress flags\n");
                exit(5);
            }
            command = COMPRESS;
        }
        else if (command_check(current_arg, "--decompress", 1, (char*[]){"-d"})) {
            if (command == COMPRESS) {
                printf("FALSE USAGE: Cannot use both --compress and --decompress flags\n");
                exit(5);
            }
            command = DECOMPRESS;
        }
        else if (command_check(current_arg, "--output", 1, (char*[]){"-o"})) {
            flag* tmp = realloc(flags, (flags_num + 1) * sizeof(flag));
            if (tmp == NULL) {
                printf("FAILED MEMORY ALLOCATION\n");
                exit(1);
            }
            flags = tmp;
            flags[flags_num].code = OUTPUT;
            if (i + 1 > argc || verify_argument(argv[i + 1], "-")) {
                // Place for argument is not found or it is a flag, so we use default config
                flags[flags_num].parameter = config->output_path;
            } else {
                // If argument is found, we use that
                flags[flags_num].parameter = argv[i + 1];
                i++;
            }
            flags_num++;
        }
        else if (command_check(current_arg, "--log", 1, (char*[]){"-l"})) {
            flag* tmp = realloc(flags, (flags_num + 1) * sizeof(flag));
            if (tmp == NULL) {
                printf("FAILED MEMORY ALLOCATION\n");
                exit(1);
            }
            flags = tmp;
            flags[flags_num].code = LOG;
            if (i + 1 > argc || verify_argument(argv[i + 1], "-")) {
                // Place for argument is not found or it is a flag, so we use default config
                flags[flags_num].parameter = config->log_path;
            } else {
                // If argument is found, we use that
                flags[flags_num].parameter = argv[i + 1];
                i++;
            }
            flags_num++;
        }
        // TODO: match wildcard symbols as valid input as well
        else {
            // If the current argument is not a command, and it is not in the whitelist of exceptions, then it is an invalid command
            int is_exception = 0;
            for (int j = 0; j < sizeof(cmd_exceptions) / sizeof(cmd_exceptions[0]); j++) {
                if (strcmp(current_arg, cmd_exceptions[j]) == 0) {
                    is_exception = 1;
                    break;
                }
            }
            if (!is_exception) {
                printf("FALSE USAGE: Invalid command detected:%s\n", current_arg);
                exit(5);
            }
        }
    }

    switch (command) {
        case UNKNOWN:
            printf("No valid command detected, skipping regular execution\n");
            break;
        case COMPRESS:
            compress(target_file, flags, flags_num);
            break;
        case DECOMPRESS:
            decompress(target_file, flags, flags_num);
            break;
        case HELP:
            printf("Help command detected, skipping regular execution\n");
            break;
        case ABOUT:
            printf("About command detected, skipping regular execution\n");
            break;
    }

    update_config_file(config_path, config);

    free(flags);
    free_file_configuration_struct(config);

    return 0;
}