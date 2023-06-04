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
    UNKNOWN = -1,
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

/// @brief Structure representing a file configuration
typedef struct file_configuration_t {
    char* program_mode;
    char* algorithm;
    char* log_path;
    char* output_path;
} file_configuration;

/// @brief Main function for CLI
int cli_main(int argc, char *argv[]);

/* DEFINITIONS FOR CLI UTILS FILES */

/// @brief Maximum length of a line in a file
#define MAX_LINE_LENGTH 512

/// @brief Function to check if passed input is a valid command
/// @param input String to check
/// @param cmd_name Main name of the command
/// @param cmd_aliases_len Number of aliases for the command
/// @param cmd_aliases Array of aliases for the command
/// @return Returns 1 if input is a valid command, 0 otherwise
int command_check(char* input, char* cmd_name, int cmd_aliases_len, char* cmd_aliases[cmd_aliases_len]);

/// @brief Function to initialize a file_configuration structure
file_configuration* init_file_configuration_struct();

/// @brief Function to free a file_configuration structure
void free_file_configuration_struct(file_configuration* config);

/// @brief Function to parse a config file into a file_configuration structure
int read_config_file(const char* filename, file_configuration* config);

/// @brief Function to update a config file with a file_configuration structure
int update_config_file(const char* filename, file_configuration* config);

/// @brief Function that compresses a file based on the flags passed
int compress(char* target_file, flag* flags, unsigned int flags_num);

/// @brief Function that decompresses a file based on the flags passed
int decompress(char* target_file, flag* flags, unsigned int flags_num);

/// @brief Function that verifies if an argument satisfies a condition
int verify_argument(const char* arg, const char* starts_with);

#endif