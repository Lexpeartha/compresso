//
// Created by aleksa on 4/24/23.
//

#include"lzw.h"

int write_binary_file(const uint8_t *filename, const uint8_t *content, size_t size) {
    FILE *fp;

    fp = fopen((char *)filename, "wb"); // cast uint8_t* to char* for filename
    if (!fp) {
        return 0;
    }

    fwrite(content, sizeof(uint8_t), size, fp); // write binary data

    fclose(fp);

    return 1;
}
int is_in_dict(dictionary *dict, unsigned int prefix, unsigned int new_char){
    for(int i = 0;i < dict->len;i++){
        if(dict->table[i].prefix == prefix && dict->table[i].new_char == new_char)
            return dict->table[i].code;
    }
    return -1;
}
void add_to_dict(dictionary *dict, unsigned int prefix, unsigned int new_char){
    dict->len++;
    dict->table = (dict_element*) realloc(dict->table, dict->len * sizeof(dict_element));
    dict->table[dict->len-1].code = dict->len-1;
    dict->table[dict->len-1].new_char = new_char;
    dict->table[dict->len-1].prefix = prefix;
}
int return_prefix(dictionary *dict, unsigned int code){
    return dict->table[code].prefix;
}
int return_new_char(dictionary *dict, unsigned int code){
    return dict->table[code].new_char;
}
array* print_string(dictionary *dict, unsigned int code){
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
    return_arr->arr = (int*)malloc(arr->len*sizeof(array));
    return_arr->len = arr->len;
    int x = 0;
    for(int i = arr->len-1;i >= 0;i--){
        return_arr->arr[x] = arr->arr[i];
        x++;
    }

    return return_arr;



}
int return_first(dictionary *dict, unsigned int code){
    int tmp_prefix = return_prefix(dict, code);
    int tmp_char = return_new_char(dict, code);
    while(tmp_prefix != -1){
        tmp_char = return_new_char(dict, tmp_prefix);
        tmp_prefix = return_prefix(dict, tmp_prefix);
    }
    return tmp_char;
}
void array_extend(array *arr1, array *arr2){
    for(int i = 0;i < arr2->len;i++){
        arr1->len++;
        arr1->arr = realloc(arr1->arr, arr1->len*(sizeof(int)));
        arr1->arr[arr1->len-1] = arr2->arr[i];
    }

}
unsigned char* read_binary_file(const char* filename, size_t* size) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file '%s'\n", filename);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    *size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    unsigned char* buffer = (unsigned char*) malloc(*size);
    if (!buffer) {
        fprintf(stderr, "Error: Failed to allocate memory\n");
        fclose(fp);
        return NULL;
    }

    if (fread(buffer, 1, *size, fp) != *size) {
        fprintf(stderr, "Error: Failed to read file '%s'\n", filename);
        free(buffer);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return buffer;
}
array* compress_lzw(char* input_file_name){
    dictionary *dict;
    dict = (dictionary*)malloc(sizeof(dictionary));
    dict->table = (dict_element*)malloc(256*sizeof(dict_element));
    dict->len = 256;

    for(int i = 0;i < dict->len;i++){
        dict->table[i].code = i;
        dict->table[i].prefix = -1;
        dict->table[i].new_char = i;
    }

    array *buffer;
    buffer = (array*)malloc(sizeof(array));
    buffer->arr = (int*)malloc(sizeof(int));
    buffer->len = 0;


    size_t size;
    unsigned char* file_content = read_binary_file(input_file_name, &size);

    int string = file_content[0];
    int x = 1;
    int character = 0;
    while(x <= size) {
        character = file_content[x];
        int tmp = is_in_dict(dict, string, character);
        if (tmp != -1)
            string = tmp;
        else {
            buffer->len++;
            buffer->arr[buffer->len - 1] = string;
            buffer->arr = realloc(buffer->arr, (buffer->len + 1) * sizeof(unsigned int));
            add_to_dict(dict, string, character);
            string = character;
        }
        x++;
    }


    return buffer;

}
void decompress(array *input_array, char *output_file_name){
    uint8_t *return_buffer;

    dictionary *dict;
    dict = (dictionary*)malloc(sizeof(dictionary));
    dict->table = (dict_element*)malloc(256*sizeof(dict_element));
    dict->len = 256;

    array *return_arr;
    return_arr = (array*)malloc(sizeof(array));
    return_arr->arr = (int*)malloc(sizeof(int));
    return_arr->len = 1;

    for(int i = 0;i < dict->len;i++){
        dict->table[i].code = i;
        dict->table[i].prefix = -1;
        dict->table[i].new_char = i;
    }
    int old_code;
    int new_code;
    int character = 0;
    old_code = input_array->arr[0];
    return_arr->arr[0] = old_code;
    for(int i = 1; i < input_array->len;i++){
        new_code = input_array->arr[i];
        if(new_code >= dict->len){
            array_extend(return_arr,print_string(dict, old_code));
            array_extend(return_arr, print_string(dict, character));
        }
        else{
            array_extend(return_arr, print_string(dict, new_code));
            character = return_first(dict, new_code);
        }
        add_to_dict(dict, old_code, character);
        old_code = new_code;
    }

    return_buffer = (uint8_t*)malloc((return_arr->len+1)*sizeof(uint8_t));

    for(int i = 0;i < return_arr->len;i++){
        return_buffer[i] = (uint8_t)return_arr->arr[i];
    }

    write_binary_file(output_file_name, return_buffer, return_arr->len);

}

