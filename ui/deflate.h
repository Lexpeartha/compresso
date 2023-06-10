#include <stdlib.h>

#ifndef COMPRESSO_DEFLATE_H
#define COMPRESSO_DEFLATE_H

void generate_random_filename(char* filename_buffer);
void deflate_static_compression(char *file_to_read, char *file_to_write);
void deflate_static_decompression(char *file_to_read, char *file_to_write);
void deflate_compression(char *file_to_read, char *file_to_write);
void deflate_decompression(char *file_to_read, char *file_to_write);
void static_huffman_compression(char *file_to_read, char *file_to_write);
void static_huffman_decompression(char *file_to_read, char *file_to_write);
void adaptive_huffman_compression(char *file_to_read, char *file_to_write);
void adaptive_huffman_decompression(char *file_to_read, char *file_to_write);
void lzw_compression(char *file_to_read, char *file_to_write);
void lzw_decompression(char *file_to_read, char *file_to_write);

#endif
