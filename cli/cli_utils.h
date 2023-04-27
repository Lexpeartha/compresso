#ifndef COMPRESSO_CLI_UTILS_H
#define COMPRESSO_CLI_UTILS_H

/*
 *
 * Header file containing definitions for all
 * structures and functions used in `cli` folder
 *
*/


/* DEFINITIONS FOR MAIN FILE */

/// @brief Enum representing all possible commands
typedef enum command_code_e {
    HELP = 0,
    ABOUT,
    COMPRESS,
    DECOMPRESS,
} command_code;

/// @brief Enum representing all possible flags
typedef enum flag_code_e {
    OUTPUT = 0,
    LOG,
} flag_code;

/// @brief Structure representing a flag
typedef struct flag_t {
    flag_code code;
    char* parameter;
} flag;

/// @brief Function to check if passed input is a valid command
/// @param input String to check
/// @param cmd_name Main name of the command
/// @param cmd_aliases_len Number of aliases for the command
/// @param cmd_aliases Array of aliases for the command
/// @return Returns 1 if input is a valid command, 0 otherwise
int command_check(char* input, char* cmd_name, int cmd_aliases_len, char* cmd_aliases[cmd_aliases_len]);

int compress(char* target_file, flag* flags, unsigned int flags_num);

int decompress(char* target_file, flag* flags, unsigned int flags_num);

/// @brief Main function for CLI
int cli_main(int argc, char *argv[]);

/* DEFINITIONS FOR CLI UTILS FILES */

// empty for now

#endif
