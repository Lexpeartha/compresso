#include "deflate.h"
#include "lzw.h"
#include "log.h"
#include "static_huffman.h"
#include "adaptive_huffman.h"

void generate_random_filename(char *filename_buffer) {
    const char *prefix = "tmpfile";
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

char *extract_file_from_path(char *string) {
    int len = 0;
    int slash_index = 0;
    while (1) {
        if (string[len] == '\0')
            break;
        if (string[len] == '/')
            slash_index = len;
        len++;
    }
    char *name = malloc(sizeof(char) * len);
    int j = 0;
    for (int i = slash_index + 1; i < len; i++) {
        name[j] = string[i];
        name[j + 1] = '\0';
        j++;
    }

    return name;
}

char *extract_path_from_path(char *string) {
    int len = 0;
    int slash_index = 0;
    while (1) {
        if (string[len] == '\0')
            break;
        if (string[len] == '/')
            slash_index = len;
        len++;
    }
    char *name = malloc(sizeof(char) * len);
    for (int i = 0; i < slash_index; i++) {
        name[i] = string[i];
        name[i + 1] = '\0';
    }

    return name;
}

char *extract_path_from_compressed(char *string) {
    int len = 0;
    int slash_index = 0;
    while (1) {
        if (string[len] == '\0')
            break;
        if (string[len] == '.')
            slash_index = len;
        len++;
    }
    char *name = malloc(sizeof(char) * len);
    for (int i = 0; i < slash_index; i++) {
        name[i] = string[i];
        name[i + 1] = '\0';
    }

    return name;
}

void deflate_compression(char *file_to_read, append_to_buffer_fn fn) {
    char filename_buffer[256];
    generate_random_filename(filename_buffer);

    // Start counting time
    clock_t start, end;
    char *current_time, *log_line;

    char time_taken_str[128];
    char *extracted_filename = extract_file_from_path(file_to_read);
    char *path = extract_path_from_path(file_to_read);
    char *original_filename = extract_file_from_path(file_to_read);
    char new_path[256];
    char old_path[256];
    double time_taken;

    // LZW COMPRESSION
    start = clock();
    current_time = get_current_timestamp();
    sprintf(time_taken_str, "LZW started on %s", extracted_filename);
    log_line = get_log_line(current_time, "LZW compression", time_taken_str);
    fn(log_line);
    free(current_time);
    free(log_line);

    strcpy(new_path, path);
    strcat(new_path, "/");
    strcat(new_path, filename_buffer);
    compress_lzw(file_to_read, new_path);
    end = clock();
    current_time = get_current_timestamp();
    sprintf(time_taken_str, "LZW finished on %s", extracted_filename);
    log_line = get_log_line(current_time, "LZW compression", time_taken_str);
    fn(log_line);
    free(current_time);
    free(log_line);

    time_taken = ((double) end - (double) start) / CLOCKS_PER_SEC;
    sprintf(time_taken_str, "LZW on %s completed in %f seconds", extracted_filename, time_taken);
    current_time = get_current_timestamp();
    log_line = get_log_line(current_time, "LZW Finished", time_taken_str);
    fn(log_line);
    free(current_time);
    free(log_line);

    // HUFFMAN COMPRESSION
    start = clock();
    current_time = get_current_timestamp();
    sprintf(time_taken_str, "Adaptive Huffman started on %s", extracted_filename);
    log_line = get_log_line(current_time, "Adaptive Huffman compression", time_taken_str);
    fn(log_line);
    free(current_time);
    free(log_line);

    strcpy(old_path, new_path);
    strcpy(new_path, path);
    strcat(new_path, "/");
    strcat(new_path, original_filename);
    strcat(new_path, ".bin"); // ADDING EXTENSION ON FINAL ALGORITHM
    adaptive_huffman_encode(old_path, new_path);
    end = clock();
    current_time = get_current_timestamp();
    sprintf(time_taken_str, "Adaptive Huffman finished on %s", extracted_filename);
    log_line = get_log_line(current_time, "Adaptive Huffman compression", time_taken_str);
    fn(log_line);
    free(current_time);
    free(log_line);

    time_taken = ((double) end - (double) start) / CLOCKS_PER_SEC;
    sprintf(time_taken_str, "Adaptive Huffman on %s completed in %f seconds", extracted_filename, time_taken);
    current_time = get_current_timestamp();
    log_line = get_log_line(current_time, "Adaptive Huffman Finished", time_taken_str);
    fn(log_line);
    free(current_time);
    free(log_line);

    free(path);
    free(original_filename);
    free(extracted_filename);
}

void deflate_decompression(char *file_to_read, append_to_buffer_fn fn) {
    char filename_buffer[256];
    generate_random_filename(filename_buffer);

    // Start counting time
    clock_t start, end;
    char *current_time, *log_line;

    char time_taken_str[128];
    char *path = extract_path_from_path(file_to_read);
    char *extracted_filename = extract_file_from_path(file_to_read);
    char new_path[256];
    double time_taken;
    char *target_file_output = extract_path_from_compressed(file_to_read);

    // HUFFMAN DECOMPRESSION
    start = clock();
    current_time = get_current_timestamp();
    sprintf(time_taken_str, "Adaptive Huffman started on %s", extracted_filename);
    log_line = get_log_line(current_time, "Adaptive Huffman decompression", time_taken_str);
    fn(log_line);
    free(current_time);
    free(log_line);

    strcpy(new_path, path);
    strcat(new_path, "/");
    strcat(new_path, filename_buffer);
    adaptive_huffman_decode(file_to_read, new_path);
    end = clock();
    current_time = get_current_timestamp();
    sprintf(time_taken_str, "Adaptive Huffman finished on %s", extracted_filename);
    log_line = get_log_line(current_time, "Adaptive Huffman decompression", time_taken_str);
    fn(log_line);
    free(current_time);
    free(log_line);

    time_taken = ((double) end - (double) start) / CLOCKS_PER_SEC;
    sprintf(time_taken_str, "Adaptive Huffman on %s completed in %f seconds", extracted_filename, time_taken);
    current_time = get_current_timestamp();
    log_line = get_log_line(current_time, "Adaptive Huffman Finished", time_taken_str);
    fn(log_line);
    free(current_time);
    free(log_line);

    // LZW DECOMPRESSION
    start = clock();
    current_time = get_current_timestamp();
    sprintf(time_taken_str, "LZW started on %s", extracted_filename);
    log_line = get_log_line(current_time, "LZW decompression", time_taken_str);
    fn(log_line);
    free(current_time);
    free(log_line);

    decompress_lzw(new_path, target_file_output);
    end = clock();
    current_time = get_current_timestamp();
    sprintf(time_taken_str, "LZW finished on %s", extracted_filename);
    log_line = get_log_line(current_time, "LZW decompression", time_taken_str);
    fn(log_line);
    free(current_time);
    free(log_line);

    time_taken = ((double) end - (double) start) / CLOCKS_PER_SEC;
    sprintf(time_taken_str, "LZW on %s completed in %f seconds", extracted_filename, time_taken);
    current_time = get_current_timestamp();
    log_line = get_log_line(current_time, "LZW Finished", time_taken_str);
    fn(log_line);
    free(current_time);
    free(log_line);

    free(path);
    free(target_file_output);
    free(extracted_filename);
}

void deflate_static_compression(char *file_to_read, append_to_buffer_fn fn) {
//    char filename_buffer[100];
//
//    generate_random_filename(filename_buffer);
//    compress_lzw(file_to_read, filename_buffer);
//    static_huffman_encode(filename_buffer, file_to_write);

    return;
}

void deflate_static_decompression(char *file_to_read, append_to_buffer_fn fn) {
//    char filename_buffer[100];
//
//    generate_random_filename(filename_buffer);
//    static_huffman_decode(file_to_read, filename_buffer);
//    decompress_lzw(filename_buffer, file_to_write);

    return;
}
