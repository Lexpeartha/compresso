#include <stdlib.h>
#include <time.h>

#ifndef COMPRESSO_DEFLATE_H
#define COMPRESSO_DEFLATE_H

typedef void (*append_to_buffer_fn)(char *text);

typedef void (*algorithm_fn)(char *file_to_read, append_to_buffer_fn fn);

char *extract_file_from_path(const char *string);

char *extract_path_from_path(const char *string);

char *extract_path_from_compressed(const char *string);

void generate_random_filename(char *filename_buffer);

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
