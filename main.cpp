#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float C(float u){
  return (u==0)? (1/sqrt(2)) : 1;
}

int main(){
  signed char qntChar[4];
  std::ifstream dctFile;
  dctFile.open("input.dct",std::ios::binary);
  std::vector<signed char> buffer(std::istreambuf_iterator<char>(dctFile), {});
  dctFile.close();
  int i;
  float qnt;
  unsigned int *j;
  for (i = 0; i<4; i++){
    qntChar[i] = buffer[i];
  }
  j = (unsigned int*) qntChar;
  std::cout << (*j) << std::endl << std::endl;
  qnt = *j;
  std::vector<int> amostras;
  for(i = 4; i < buffer.size(); i++){
    amostras.push_back(buffer[i]);
    std::cout << amostras[i-4] << std::endl;
  }


  std::vector<float> dct;
  std::cout << std::endl << C(2) << std::endl << C(0) << std::endl;
  for(int u = 0; u < qnt; u++){
    float soma = 0;
    for(int i = 0; i < qnt; i++){
      soma += C(u) * amostras[i] * cos(((M_PI*u)/(2*qnt))*(2*i + 1));
    }
    dct.push_back(sqrt(2/qnt)*soma);
  }

  for(i = 0; i < dct.size(); i++){
    std::cout << "dct "<< i << " " << dct[i] << std::endl;
  }

  std::vector<float> idct;
  for(int i = 0; i < qnt; i++){
    float soma = 0;
    for(int u = 0; u < qnt; u++){
      soma += C(u) * dct[u] * cos(((M_PI*u)/(2*qnt))*(2*i + 1));
    }
    idct.push_back(sqrt(2/qnt)*soma);
  }

  for(i = 0; i < idct.size(); i++){
    std::cout << "idct "<< i << " " << idct[i] << std::endl;
  }

  std::cout << std::endl;
  std::vector<float> diff;
  for(i = 0; i < qnt; i++){
    diff.push_back(amostras[i] - idct[i]);
    std::cout << diff[i] << std::endl;
  }



  return 0;
}
