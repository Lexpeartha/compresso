
#include"lzw.h"
hash_entry *hash_table = NULL;
void add_entry(char *key, long int value) {
    hash_entry *entry = malloc(sizeof(hash_entry));
    entry->key = key;
    entry->value = value;
    HASH_ADD_STR(hash_table, key, entry);
}
long int find_entry(char *key) {
    hash_entry *entry;

    // Use UT hash to find the entry with the given key in the global hash table
    HASH_FIND_STR(hash_table, key, entry);

    // If the entry is found, return its value; otherwise, return -1
    if (entry) {
        return entry->value;
    } else {
        return -1;
    }
}
void free_hash_table() {
    hash_entry *current_entry, *tmp;

    // Iterate over each entry in the hash table
    HASH_ITER(hh, hash_table, current_entry, tmp) {
        // Remove the current entry from the hash table
        HASH_DEL(hash_table, current_entry);

        // Free the key and value associated with the entry
        free(current_entry->key);
        free(current_entry);

        // Note: If your value contains dynamically allocated memory,
        // make sure to free it before freeing the hash_entry struct.
    }
}
char* make_key(long int value1, long int value2) {
    char* separator = "_";
    char* key = malloc(sizeof(char) * (strlen(separator) + 2*sizeof(long int)));
    sprintf(key, "%ld%s%ld", value1, separator, value2);
    return key;
}
void generate_random_filename(char* filename_buffer) {
    const char* prefix = "tmpfile";
    long int fd;


    size_t filename_len = strlen(prefix) + 10;
    char filename_template[filename_len + 1];
    snprintf(filename_template, filename_len + 1, "%s-XXXXXX", prefix);


    fd = mkstemp(filename_template);
    if (fd == -1) {
        perror("mkstemp");
        exit(1);
    }

    strncpy(filename_buffer, filename_template, filename_len + 1);
}
void free_dictionary(dictionary *dict) {
    if (dict != NULL) {
        if (dict->table != NULL) {
            free(dict->table); // free the memory allocated for the dict_element array
            dict->table = NULL; // set the array polong inter to NULL to avoid dangling polong inter
        }
        free(dict); // free the memory allocated for the dictionary structure
    }
}
void free_array(array *a) {
    free(a->arr);
    a->arr = NULL;
    free(a);
}
void add_to_dict(dictionary *dict, long int prefix, long int new_char) {
    if (dict->len >= dict->allocated) {
        dict->allocated += 1000000;
        dict->table = (dict_element*) realloc(dict->table, dict->allocated * sizeof(dict_element));
    }
    dict->table[dict->len].new_char = new_char;
    dict->table[dict->len].prefix = prefix;
    add_entry(make_key(prefix, new_char), dict->len);
    dict->len++;
}
long int return_prefix(dictionary *dict, long int code){
    return dict->table[code].prefix;
}
long int return_new_char(dictionary *dict, long int code){
    return dict->table[code].new_char;
}
array* print_string(dictionary *dict, long int code){
    array *arr;
    arr = (array*)malloc(sizeof(array));
    arr->len = 1;
    arr->arr = (long int*)malloc(sizeof(long int));
    arr->arr[0] = return_new_char(dict, code);
    long int tmp_prefix = return_prefix(dict, code);
    long int tmp_char = return_new_char(dict, code);
    while(tmp_prefix != -1){
        tmp_char = return_new_char(dict, tmp_prefix);
        tmp_prefix = return_prefix(dict, tmp_prefix);
        arr->len++;
        arr->arr = realloc(arr->arr, arr->len * sizeof(long int));
        arr->arr[arr->len-1] = tmp_char;
    }
    array *return_arr = (array*)malloc(sizeof(array));
    return_arr->arr = (long int*)malloc(arr->len*sizeof(long int));
    return_arr->len = arr->len;
    long int x = 0;
    long int i;
    for(i = arr->len-1;i >= 0;i--){
        return_arr->arr[x] = arr->arr[i];
        x++;
    }
    free(arr->arr);


    return return_arr;



}
long int return_first(dictionary *dict, long int code){
    long int tmp_prefix = return_prefix(dict, code);
    long int tmp_char = return_new_char(dict, code);
    while(tmp_prefix != -1){
        tmp_char = return_new_char(dict, tmp_prefix);
        tmp_prefix = return_prefix(dict, tmp_prefix);
    }
    return tmp_char;
}
char* compress_lzw(char* input_file_name){
    dictionary *dict;
    dict = (dictionary*)malloc(sizeof(dictionary));
    dict->table = (dict_element*)malloc(256*sizeof(dict_element));
    dict->len = 256;
    dict->allocated = 256;
    for(long int i = 0;i < dict->len;i++){
        dict->table[i].prefix = -1;
        dict->table[i].new_char = i;
        add_entry(make_key(-1, i) , i);
    }

    char *file_name;
    file_name = (char*)malloc(20*sizeof(char));
    generate_random_filename(file_name);

    size_t size;

    FILE* fp = fopen(input_file_name, "rb");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file '%s'\n", input_file_name);
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    FILE* fp_write = fopen(file_name, "wb");
    size_t size_write;
    fseek(fp_write, 0, SEEK_END);
    size_write = ftell(fp_write);
    fseek(fp_write, 0, SEEK_SET);


    long int string = (long int) fgetc(fp);
    long int x = 1;
    long int character = 0;
    long int tmp;
    while(x < size) {
        character = (long int) fgetc(fp);
        tmp = find_entry(make_key(string, character));
        if (tmp != -1)
            string = tmp;
        else {
            fwrite(&string, sizeof(long int), 1, fp_write);
            add_to_dict(dict, string, character);
            string = character;
        }
        x++;
    }
    double dict_size = dict->len;
    dict_size = dict_size*8/1048576;
    printf("Size of dictionary: %lfMB", dict_size);
    fwrite(&string, sizeof(long int), 1, fp_write);
    free_dictionary(dict);
    fclose(fp_write);
    free_hash_table();
    return file_name;

}
void decompress(char* input_file_name, char *output_file_name){

    FILE* fp_write = fopen(input_file_name, "rb");
    long int size_write;
    fseek(fp_write, 0, SEEK_END);
    size_write = ftell(fp_write);
    fseek(fp_write, 0, SEEK_SET);

    FILE* output_file = fopen(output_file_name, "wb");
    long int output_size;
    fseek(output_file, 0, SEEK_END);
    output_size = ftell(output_file);
    fseek(output_file, 0, SEEK_SET);


    dictionary *dict;
    dict = (dictionary*)malloc(sizeof(dictionary));
    dict->table = (dict_element*)malloc(256*sizeof(dict_element));
    dict->len = 256;
    dict->allocated = 256;



    for(long int i = 0;i < dict->len;i++){
        dict->table[i].prefix = -1;
        dict->table[i].new_char = i;
    }
    long int tmp;
    long int old_code;
    long int new_code;
    long int character = 0;
    long int len = size_write/8;
    array* tmp_arr;
    uint8_t convertor;
    fread(&old_code, sizeof(long int), 1, fp_write);
    convertor = old_code;
    fwrite(&convertor, sizeof(uint8_t) , 1, output_file);
    for(long int i = 1; i < len;i++){
        fread(&new_code, sizeof(long int), 1, fp_write);
        if(new_code >= dict->len){
            tmp_arr = print_string(dict, old_code);
            for(long int i = 0;i < tmp_arr->len;i++){
                convertor = tmp_arr->arr[i];
                fwrite(&convertor, sizeof(uint8_t) , 1, output_file);
            }


            tmp_arr = print_string(dict, character);
            for(long int i = 0;i < tmp_arr->len;i++){
                convertor = tmp_arr->arr[i];
                fwrite(&convertor, sizeof(uint8_t) , 1, output_file);
            }

        }
        else{
            tmp_arr = print_string(dict, new_code);
            for(long int i = 0;i < tmp_arr->len;i++){
                convertor = tmp_arr->arr[i];
                fwrite(&convertor, sizeof(uint8_t), 1, output_file);
            }
            character = return_first(dict, new_code);
        }
        add_to_dict(dict, old_code, character);
        old_code = new_code;
    }

    free_array(tmp_arr);
    free_dictionary(dict);
    free_hash_table();
    remove(input_file_name);
}

