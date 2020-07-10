#include <iostream>
#include <fstream>
#include <random>

int main(int argc, char const *argv[]) {
  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> dis_len(16, 256);
  std::uniform_int_distribution<> dis_ch(89, 122);    // 97 - 122
  
  size_t n = atoi(argv[1]);
  std::cerr << "n: " << n << '\n';
  for (size_t i = 0; i < n; i++) {
    std::ofstream file;
    std:: string s;
    s = "wb" + std::to_string(i) + ".wb";
    file.open(s, std::ios::binary);
    
    size_t len = dis_len(gen);
    /*std::cerr << "len: " << len << '\n';*/
    
    for (size_t j = 0; j < len; j++) {
      char r = dis_ch(gen);
      /*std::cerr << "r: " << r << '\n';*/
      
      char c;
      // 89 - 92
      if(r <= 92){
        c = 10; file.write(reinterpret_cast<char*>(&c), sizeof c);
      }
      // 93 - 96
      else if(r <= 96){
        c = 58; file.write(reinterpret_cast<char*>(&c), sizeof c);
      }
      else{
        file.write(reinterpret_cast<char*>(&r), sizeof r);
      }
    }
  }
  
  return 0;
}

/*
switch (r){
  case 91: std::cerr << "case 95" << '\n'; c = 10; file.write(reinterpret_cast<char*>(&c), sizeof c);; break;
  case 96: std::cerr << "case 96" << '\n'; c = 58; file.write(reinterpret_cast<char*>(&c), sizeof c);; break;
  default: std::cerr << "default case" << '\n'; file.write(reinterpret_cast<char*>(&r), sizeof r);
}
*/
