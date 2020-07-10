#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ctype.h"
#include <assert.h>

size_t dict_size;
char** dict;
size_t data_size;   // size of "data" including the terminating null character, i.e. number of bytes allocated
char* data;         // null-terminated string

size_t text_size;
char* text;
size_t key_len;
char* key;
char* key_lc;

char return_code;

// determin the number of entries in the dictionary by counting the NL characters
// simultaneously, make sure that the dictionary only consists of NLs, colons and lowercase letters
void wb_size(){
  dict_size = 0;
  for (size_t i = 0; i < data_size - 1; ++i) {
    if(data[i] == 10){
      ++dict_size;
      continue;
    }
    else if(data[i] == 58){
      continue;
    }
    else{
      if(data[i] < 97 || data[i] > 122){
        fprintf(stderr, "Error: invalid character in wb (%u)\n", (unsigned char)data[i]);
        free(data);
        exit(2);
      }
    }
  }
  
  if(data_size > 1){
    // check wether the dictionary ends with a newline character
    if(data[data_size-2] == '\n'){
      // replace the newline character with the null character
      data[data_size-2] = '\0';
    }
    else{
      // count the uncounted line
      ++dict_size;
    }
  }
}

// direct the pointers in dict at an element right after NL
void fill_dict(){
  dict = malloc(dict_size * sizeof *dict);
  if(dict == 0){
    fprintf(stderr, "Error: allocating %lu byte failed (for dict)\n", sizeof *data);
    free(data);
    exit(2);
  }
  dict[0] = &data[0];
  size_t idx = 1;
  for (size_t i = 0; i < data_size; ++i) {
    if(data[i] == 10){
      data[i] = '\0';       /* replace NL with '\0' */
      if(idx < dict_size){
        dict[idx++] = &data[i+1];
      }
    }
  }
}

// detect misplaced colons
void is_valid_wb(){
  for (size_t i = 0; i < dict_size; ++i) {
    char* first = strchr(dict[i], ':');
    if(first == NULL){
      fprintf(stderr, "Error: dictionary contains line without colon\n");
      free(data);
      free(dict);
      exit(2);
    }
    char* last = strrchr(dict[i], ':');
    if(first != last){
      fprintf(stderr, "Error: dictionary contains line with multiple colons\n");
      free(data);
      free(dict);
      exit(2);
    }
    if(first == dict[i]){
      fprintf(stderr, "Error: empty key\n");
      free(data);
      free(dict);
      exit(2);
    }
    if(last == dict[i] + strlen(dict[i]) - 1){
      fprintf(stderr, "Error: empty value\n");
      free(data);
      free(dict);
      exit(2);
    }
    *first = '\0';    /* replace ':' with '\0' */
  }
}

void read_wb(char const *file) {
  
  // read the wb
  FILE* fp = fopen(file, "r");
  if(!fp) {
    fprintf(stderr, "Error opening %s\n", file);
    exit(2);
  }
  else{
    data_size = 1;
    data = malloc(sizeof *data);
    if(data == 0){
      fprintf(stderr, "Error: allocating %lu byte failed (for data)\n", sizeof *data);
      fclose(fp);
      free(data);
      exit(2);
    }
    data[0] = '\0';
    
    size_t buf_size = 4 * 1024;
    char buf[buf_size];
    size_t read;
    while(1){
      read = fread(buf, sizeof *buf, buf_size - 1, fp);
      if(read != 0){
        buf[read] = '\0';
        data_size += read;
        data = realloc(data, data_size * sizeof *data);
        if(data == 0){
          fprintf(stderr, "Error: allocating %lu byte failed (for data)\n", data_size * sizeof *data);
          fclose(fp);
          free(data);
          exit(2);
        }
        strcat(data, buf);
      }
      if(ferror(fp)){
        fprintf(stderr, "Error reading %s\n", file);
        fclose(fp);
        free(data);
        exit(2);
      }
      else if(feof(fp)){
        break;
      }
    }
    fclose(fp);
  }
  
  wb_size();
  
  if(dict_size > 0){
    fill_dict();
    is_valid_wb();
  }
}

int cmpstr(const void* a, const void* b){
  return strcmp(*(const char**)a, *(const char**)b);
}

void sort_wb(){
  qsort(dict, dict_size, sizeof(char*), cmpstr);
}

void find_duplicates(){
  if(dict_size > 1){
    for (size_t i = 0; i < dict_size - 1; i++) {
      if(strcmp(dict[i], dict[i+1]) == 0){
        fprintf(stderr, "Error: duplicate key in wb\n%s:%s\n%s:%s\n", dict[i], dict[i] + strlen(dict[i]) + 1, dict[i+1], dict[i+1] + strlen(dict[i+1]) + 1);
        free(data);
        free(dict);
        exit(2);
      }
    }
  }
}

void terminate(char ret_code){
  free(data);
  free(dict);
  free(text);
  free(key);
  free(key_lc);
  exit(ret_code);
}

size_t find_max_key_length(){
  size_t max_key_length = 0;
  for (size_t i = 0; i < dict_size; i++) {
    size_t len = strlen(dict[i]);
    if(len > max_key_length){
      max_key_length = len;
    }
  }
  return max_key_length;
}

void init(size_t max_key_length){
  text_size = max_key_length + 1;
  if(text_size < 8 * 1024){
    text_size = 8 * 1024;
  }
  
  text = malloc(text_size * sizeof *text);
  if(text == 0){
    free(data);
    free(dict);
    fprintf(stderr, "Error: allocating %lu byte failed (for text)\n", text_size * sizeof *text);
    exit(2);
  }
  text[text_size - 1] = '\0';
  
  key_len = 0;
  key = malloc((max_key_length + 1) * sizeof *key);
  if(text == 0){
    fprintf(stderr, "Error: allocating %lu byte failed (for key)\n", (max_key_length + 1) * sizeof *key);
    free(data);
    free(dict);
    free(text);
    exit(2);
  }
  key[max_key_length] = '\0';
  
  key_lc = malloc((max_key_length + 1) * sizeof *key_lc);
  if(text == 0){
    fprintf(stderr, "Error: allocating %lu byte failed (for key_lc)\n", (max_key_length + 1) * sizeof *key_lc);
    free(data);
    free(dict);
    free(text);
    free(key);
    exit(2);
  }
  key_lc[max_key_length] = '\0';
}

void is_valid_text(size_t read){
  for (size_t i = 0; i < read; i++) {
    if((text[i] < 32 && text[i] != 10) || text[i] > 126){
      fprintf(stderr, "Error: text contains non-printable char (%u)\n", (unsigned char)text[i]);
      terminate(2);
    }
  }
}

void match(){
  for (size_t i = 0; i < key_len; i++) {
    key_lc[i] = tolower(key[i]);
  }
  key_lc[key_len] = '\0';
  
  char** val = bsearch(&key_lc, dict, dict_size, sizeof *dict, cmpstr);
  if(val){
    if(isupper(key[0])){
      fprintf(stdout, "%c", toupper(*(*val + key_len + 1)));
    }
    else{
      fprintf(stdout, "%c", *(*val + key_len + 1));
    }
    fprintf(stdout, "%s", *val + key_len + 2);
  }
  else{
    return_code = 1;
    fprintf(stdout, "%c", '<');
    fprintf(stdout, "%s", key);
    fprintf(stdout, "%c", '>');
  }
}

void translate(){
  size_t max_key_length = find_max_key_length();
  init(max_key_length);
  
  char trans = 1;
  size_t begin = 0;
  size_t read = 0;
  
  while(1){
    begin = 0;
    read  = fread(text, sizeof *text, text_size, stdin);
    if(read != 0){
      is_valid_text(read);
      
      for (size_t i = 0; i < read; i++) {
        
        // neither lowercase nor uppercase but valid
        if(text[i] < 65 || text[i] > 122 || (text[i] > 90 && text[i] < 97)){
          // a word that is longer than the longest key (that has already been identified as such and partly printed to stdout) has to be ended
          if(trans == 0){
            assert(key_len == 0);
            fprintf(stdout, ">");
            key_len = 0;
            key[0] = '\0';
            trans = 1;
          }
          // try to match a possible key
          else{
            if(key_len + (i - begin) != 0){
              assert(key_len + (i - begin) > 0);
              strncpy(key + key_len, &text[begin], i - begin);
              key[key_len + (i - begin)] = '\0';
              key_len += (i - begin);
              match();
            }
          }
          
          key_len = 0;
          key[0] = '\0';
          fprintf(stdout, "%c", text[i]); // print the current char
          begin = i + 1;
        }
        
        // lowercase or uppercase (65–90 || 97–122)
        else{
          // longer than the longest key
          if((trans == 1) && (key_len + (i - begin) + 1 > max_key_length)){
            return_code = 1;
            fprintf(stdout, "<");
            if(key_len > 0){
              fprintf(stdout, "%s", key);
            }
            fwrite(text + begin, sizeof *text, i - begin, stdout);
            
            key_len = 0;
            key[0] = '\0';
            trans = 0;
          }
          
          if(trans == 0){
            fprintf(stdout, "%c", text[i]);
          }
        }
      }
      
      // store the reamining part of the input in key
      if(trans){
        strncpy(key + key_len, &text[begin], read - begin);
        key[key_len + (read - begin)] = '\0';
        key_len += read - begin;
      }
    }
    
    if(ferror(stdin)){
      fprintf(stderr, "Error reading stdin\n");
      terminate(2);
    }
    if(feof(stdin)){
      if(trans == 0){
        assert(key_len == 0);
        fprintf(stdout, ">");
        break;
      }
      if(key_len == 0){
        break;
      }
      else{
        match();
        break;
      }
    }
  }
}

int main(int argc, char const *argv[]) {
  
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    exit(2);
  }
  
  return_code = 0;
  read_wb(argv[1]);
  sort_wb();
  find_duplicates();
  translate();
  
  free(data);
  free(dict);
  free(text);
  free(key);
  free(key_lc);
  
  return return_code;
}
