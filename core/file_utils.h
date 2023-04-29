#ifndef COMPRESSO_FILE_UTILS_H
#define COMPRESSO_FILE_UTILS_H

/// @brief Function to read a file
char* read_file(const char* filename);

/// @brief Function to write a file
int write_file(const char* filename, const char* content);

#endif //COMPRESSO_FILE_UTILS_H
