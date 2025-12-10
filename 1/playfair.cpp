#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

char matrix[5][5]={
  {'y','a','s','h','v'},
  {'n','t','b','c','d'},
  {'e','f','g','i','k'},
  {'l','m','o','p','q'},
  {'r','u','w','x','z'}
};

void getPosition(char c, int &row, int &col) {
    if (c == 'j') c = 'i';
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        if (matrix[i][j] == c) {
          row = i;
          col = j;
          return;
        }
      }
    }
}

string prepareText(string input) {
  string clean = "";
  for (char c : input) {
    if (isalpha(c)) {
      clean += tolower(c);
    }
  }
  string pairs="";
  for (int i=0;i<clean.length();i++) {
    char a=clean[i];
    char b=(i+1<clean.length())?clean[i+1]:'x';
    if (a == b) {
      pairs+=a;
      pairs+='x';
    } else {
      pairs+=a;
      pairs+=b;
      i++;
    }
  }
  if (pairs.length()%2!=0) {
    pairs+='x';
  }
  return pairs;
}

string encryptPlayfair(string text) {
  string processed=prepareText(text);
  string cipher="";
  for (int i=0;i<processed.length();i+=2) {
    char a=processed[i];
    char b=processed[i+1];
    int r1,c1,r2,c2;
    getPosition(a,r1,c1);
    getPosition(b,r2,c2);
    if(r1==r2){
      cipher+=matrix[r1][(c1+1)%5];
      cipher+=matrix[r2][(c2+1)%5];
    }else if(c1 == c2) {
      cipher+=matrix[(r1+1)%5][c1];
      cipher+=matrix[(r2+1) %5][c2];
    }else{
      cipher+=matrix[r1][c2];
      cipher+=matrix[r2][c1];
    }
  }
  return cipher;
}

int main() {
  string plaintext = "Yashvanth Karunakaran is my name, and I am a computer science student interested in data security. This assignment requires me to implement two historical ciphers using C++ programming. I'll implement two classical encryption methods: the Monoalphabetic cipher and the Playfair cipher. The Monoalphabetic approach's unique feature is that it maps every character to other character. The Playfair cipher, on the other hand, encrypts letter pairs using a key matrix. I'm also required to analyze letter frequencies from a large text file to find patterns. These exercises help me to understand the foundations of modern cryptography.";
  for(int i=0;i<5;i++) {
    for(int j=0;j<5;j++) cout<<matrix[i][j]<<" ";
    cout<<endl;
  }
  string encrypted=encryptPlayfair(plaintext);
  cout<<encrypted<<endl;
  return 0;
}

