#include "deflate.h"
#include "lzw.h"
#include "static_huffman.h"
#include "adaptive_huffman.h"

void generate_random_filename(char* filename_buffer) {
    const char* prefix = "tmpfile";
    int fd;


    size_t filename_len = strlen(prefix) + 10;
    char filename_template[filename_len + 1];
    snprintf(filename_template, filename_len + 1, "%s-XXXXXX", prefix);


    fd = mkstemp(filename_template);
    if (fd == -1) {
        printf("FAILED TO CREATE TEMPORARY FILE\n");
        exit(3);
    }

    strncpy(filename_buffer, filename_template, filename_len + 1);
}

void deflate_static_compression(char *file_to_read, char *file_to_write) {
    char filename_buffer[100];

    generate_random_filename(filename_buffer);
    compress_lzw(file_to_read, filename_buffer);
    static_huffman_encode(filename_buffer, file_to_write);

    return;
}

void deflate_static_decompression(char *file_to_read, char *file_to_write) {
    char filename_buffer[100];

    generate_random_filename(filename_buffer);
    static_huffman_decode(file_to_read, filename_buffer);
    decompress_lzw(filename_buffer, file_to_write);

    return;
}
