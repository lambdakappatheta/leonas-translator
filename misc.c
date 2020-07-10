#include <stdio.h>
#include <string.h>
#include "misc.h"

void print_ch(char c){
  if((c >= 97 && c <= 122) || (c == 58)){
    fprintf(stderr, "%c", c);
  }
  else if(c == 10){
    fprintf(stderr, "!");
  }
  else{
    fprintf(stderr, "\ninvalid character (%c (%d))", c, c);
  }
}

void print_dict(char** dict, const size_t dict_size){
  fprintf(stderr, "dict_size: %lu\n", dict_size);
  for (size_t i = 0; i < dict_size; i++) {
    fprintf(stderr, "%s", dict[i]);
    fprintf(stderr, ":");
    fprintf(stderr, "%s", dict[i] + strlen(dict[i]) + 1);
    fprintf(stderr, "\n");
  }
}

void print_data(const char* data, const size_t data_size){
  fprintf(stderr, "data_size: %lu\n", data_size);
  for (size_t i = 0; i < data_size; i++) {
    print_ch(data[i]);
  }
  fprintf(stderr, "\n");
}
