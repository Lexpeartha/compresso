#include <stdlib.h>
#include <time.h>

#ifndef COMPRESSO_DEFLATE_H
#define COMPRESSO_DEFLATE_H

typedef void (*append_to_buffer_fn)(char *text);

void generate_random_filename(char* filename_buffer);
void deflate_static_compression(char *file_to_read, append_to_buffer_fn fn);
void deflate_static_decompression(char *file_to_read, append_to_buffer_fn fn);
void deflate_compression(char *file_to_read, append_to_buffer_fn fn);
void deflate_decompression(char *file_to_read, append_to_buffer_fn fn);
void static_huffman_compression(char *file_to_read, append_to_buffer_fn fn);
void static_huffman_decompression(char *file_to_read, append_to_buffer_fn fn);
void adaptive_huffman_compression(char *file_to_read, append_to_buffer_fn fn);
void adaptive_huffman_decompression(char *file_to_read, append_to_buffer_fn fn);
void lzw_compression(char *file_to_read, append_to_buffer_fn fn);
void lzw_decompression(char *file_to_read, append_to_buffer_fn fn);

#endif
