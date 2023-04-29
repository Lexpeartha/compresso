#include"lzw.h"


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
long int is_in_dict(dictionary *dict, long int prefix, long int new_char){
    for(long int i = 0;i < dict->len;i++){
        if(dict->table[i].prefix == prefix)
            if(dict->table[i].new_char == new_char)
                return dict->table[i].code;
    }
    return -1;
}
void add_to_dict(dictionary *dict,  long int prefix, long int new_char){
    dict->len++;
    dict->table = (dict_element*) realloc(dict->table, dict->len * sizeof(dict_element));
    dict->table[dict->len-1].code = dict->len-1;
    dict->table[dict->len-1].new_char = new_char;
    dict->table[dict->len-1].prefix = prefix;
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
    for(long int i = 0;i < dict->len;i++){
        dict->table[i].code = i;
        dict->table[i].prefix = -1;
        dict->table[i].new_char = i;
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


    long int string = (long) fgetc(fp);
    long int x = 1;
    long int character = 0;
    while(x < size) {
        character = (long) fgetc(fp);
        long int tmp = is_in_dict(dict, string, character);
        if (tmp != -1)
            string = tmp;
        else {
            fwrite(&string, sizeof(long int), 1, fp_write);
            add_to_dict(dict, string, character);
            string = character;
        }
        x++;
    }

    fwrite(&string, sizeof(long int), 1, fp_write);
    free_dictionary(dict);
    fclose(fp_write);

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


    for(long int i = 0;i < dict->len;i++){
        dict->table[i].code = i;
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
    fwrite(&convertor, sizeof(uint8_t), 1, output_file);
    for(long int i = 1; i < len;i++){
        fread(&new_code, sizeof(long int), 1, fp_write);
        printf("%ld ", new_code);
        if(new_code >= dict->len){
            tmp_arr = print_string(dict, old_code);
            for(long int i = 0;i < tmp_arr->len;i++){
                convertor = tmp_arr->arr[i];
                fwrite(&convertor, sizeof(uint8_t), 1, output_file);
            }


            tmp_arr = print_string(dict, character);
            for(long int i = 0;i < tmp_arr->len;i++){
                convertor = tmp_arr->arr[i];
                fwrite(&convertor, sizeof(uint8_t), 1, output_file);
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
    //remove(input_file_name);
}