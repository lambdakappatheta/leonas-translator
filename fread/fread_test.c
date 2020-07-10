#include <stdio.h>

int main(int argc, char const *argv[]) {
  
  if(argc == 1){
    fprintf(stderr, "usage freadtest <input>\n");
  }
  
  FILE* fp = fopen(argv[1], "r");
  
  size_t buf_size = 3;
  char buf[buf_size];
  size_t read;
  
  while(1){
    read = fread(buf, sizeof *buf, buf_size - 1, fp);
    fprintf(stderr, "read: %lu\n", read);
    for (size_t i = 0; i < read; i++) {
      fprintf(stderr, "%d\n", buf[i]);
    }
    fprintf(stderr, "feof(fp): %d\n", feof(fp));
    fprintf(stderr, "\n");
    if(feof(fp)){
      break;
    }
  }
  
  
  
  return 0;
}
