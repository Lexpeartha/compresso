#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli_utils.h"

int cli_main(int argc, char *argv[]) {
    // TODO: add flag queue or something similar so flags can be processed fully in any order
    command_code command = UNKNOWN;

    for (int i = 1; i < argc; i++) {
        char* current_arg = argv[i];
        /* IMPORTANT FLAGS THAT SKIP REGULAR EXECUTION */
        if (command_check(current_arg, "--help", 1, (char*[]){"-h"})) {
            // Detecting help command will print help info and finish the program
            // Makes flag queue irrelevant
            command = HELP;
            printf("Usage: ./compresso [cmd] [options]\n", argv[0]);
            printf("Options:\n");
            printf("  --help, -h    Display this help message\n");
            printf("  --about, -a   Display information about this program\n");
            exit(0);
        }
        else if (command_check(current_arg, "--about", 1, (char*[]){"-a"})) {
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
            printf("  - Marko Mišić\n");
            printf("  - Tamara Šekularac\n");
            printf("  - Miloš Obradović\n");
            exit(0);
        }
        /* REGULAR COMMANDS */
        else {
            // Empty for now
            command = UNKNOWN;
        }
    }

    // TODO: add command execution based on command_code, and flags from flag queue

    return 0;
}