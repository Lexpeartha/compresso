
#include"lzw.h"

hash_entry *hash_table = NULL;
void add_entry(char *key, int value) {
    hash_entry *entry = malloc(sizeof(hash_entry));
    entry->key = key;
    entry->value = value;
    HASH_ADD_STR(hash_table, key, entry);
}
int find_entry(char *key) {
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
char* make_key(int value1, int value2) {
    char* separator = "_";
    char* key = malloc(sizeof(char) * (strlen(separator) + 2*sizeof(int)));
    sprintf(key, "%d%s%d", value1, separator, value2);
    return key;
}
void generate_random_filename(char* filename_buffer) {
    const char* prefix = "tmpfile";
    int fd;


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
            dict->table = NULL; // set the array pointer to NULL to avoid dangling pointer
        }
        free(dict); // free the memory allocated for the dictionary structure
    }
}
void free_array(array *a) {
    free(a->arr);
    a->arr = NULL;
    free(a);
}
void add_to_dict(dictionary *dict, int prefix, int new_char) {
    if (dict->len >= dict->allocated) {
        dict->allocated += 1000000;
        dict->table = (dict_element*) realloc(dict->table, dict->allocated * sizeof(dict_element));
    }
    dict->table[dict->len].new_char = new_char;
    dict->table[dict->len].prefix = prefix;
    add_entry(make_key(prefix, new_char), dict->len);
    dict->len++;
}
int return_prefix(dictionary *dict, int code){
    return dict->table[code].prefix;
}
int return_new_char(dictionary *dict, int code){
    return dict->table[code].new_char;
}
array* print_string(dictionary *dict, int code){
    array *arr;
    arr = (array*)malloc(sizeof(array));
    arr->len = 1;
    arr->arr = (int*)malloc(sizeof(int));
    arr->arr[0] = return_new_char(dict, code);
    int tmp_prefix = return_prefix(dict, code);
    int tmp_char = return_new_char(dict, code);
    while(tmp_prefix != -1){
        tmp_char = return_new_char(dict, tmp_prefix);
        tmp_prefix = return_prefix(dict, tmp_prefix);
        arr->len++;
        arr->arr = realloc(arr->arr, arr->len * sizeof(int));
        arr->arr[arr->len-1] = tmp_char;
    }
    array *return_arr = (array*)malloc(sizeof(array));
    return_arr->arr = (int*)malloc(arr->len*sizeof(int));
    return_arr->len = arr->len;
    int x = 0;
    int i;
    for(i = arr->len-1;i >= 0;i--){
        return_arr->arr[x] = arr->arr[i];
        x++;
    }
    free(arr->arr);


    return return_arr;



}
int return_first(dictionary *dict, int code){
    int tmp_prefix = return_prefix(dict, code);
    int tmp_char = return_new_char(dict, code);
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
    for(int i = 0;i < dict->len;i++){
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


    int string = (int) fgetc(fp);
    int x = 1;
    int character = 0;
    int tmp;
    while(x < size) {
        character = (int) fgetc(fp);
        tmp = find_entry(make_key(string, character));
        if (tmp != -1)
            string = tmp;
        else {
            fwrite(&string, sizeof(int), 1, fp_write);
            add_to_dict(dict, string, character);
            string = character;
        }
        x++;
    }

    fwrite(&string, sizeof(int), 1, fp_write);
    free_dictionary(dict);
    fclose(fp_write);
    free_hash_table();
    return file_name;

}
void decompress(char* input_file_name, char *output_file_name){

    FILE* fp_write = fopen(input_file_name, "rb");
    int size_write;
    fseek(fp_write, 0, SEEK_END);
    size_write = ftell(fp_write);
    fseek(fp_write, 0, SEEK_SET);

    FILE* output_file = fopen(output_file_name, "wb");
    int output_size;
    fseek(output_file, 0, SEEK_END);
    output_size = ftell(output_file);
    fseek(output_file, 0, SEEK_SET);


    dictionary *dict;
    dict = (dictionary*)malloc(sizeof(dictionary));
    dict->table = (dict_element*)malloc(256*sizeof(dict_element));
    dict->len = 256;
    dict->allocated = 256;



    for(int i = 0;i < dict->len;i++){
        dict->table[i].prefix = -1;
        dict->table[i].new_char = i;
    }
    int tmp;
    int old_code;
    int new_code;
    int character = 0;
    int len = size_write/4;
    array* tmp_arr;
    uint8_t convertor;
    fread(&old_code, sizeof(int), 1, fp_write);
    convertor = old_code;
    fwrite(&convertor, sizeof(uint8_t) , 1, output_file);
    for(int j = 1; j < len;j++){
        fread(&new_code, sizeof(int), 1, fp_write);
        if(new_code >= dict->len){
            tmp_arr = print_string(dict, old_code);
            for(int i = 0;i < tmp_arr->len;i++){
                convertor = tmp_arr->arr[i];
                fwrite(&convertor, sizeof(uint8_t) , 1, output_file);
            }


            tmp_arr = print_string(dict, character);
            for(int i = 0;i < tmp_arr->len;i++){
                convertor = tmp_arr->arr[i];
                fwrite(&convertor, sizeof(uint8_t) , 1, output_file);
            }

        }
        else{
            tmp_arr = print_string(dict, new_code);
            for(int i = 0;i < tmp_arr->len;i++){
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

