#include<iostream>
#include<map>
#include<string>
#include<cctype>
using namespace std;

map<char,char> getMapTable(){
  map<char,char> m;
  string plain="abcdefghijklmnopqrstuvwxyz";
  string cipher="yashvnthkrupemzbwodxvqclgf";
  for(int i=0;i<26;i++){
    m[plain[i]]=cipher[i];
  }
  m[':']='.';  
  m[',']=',';  
  m['.']='+';  
  m[' ']='\'';  
  m['+']=':'; 
  m['\'']=' ';
  return m;
}

int main(){
  string plaintext="Yashvanth Karunakaran is my name, and I am a computer science student interested in data security. This assignment requires me to implement two historical ciphers using C++ programming. I'll implement two classical encryption methods: the Monoalphabetic cipher and the Playfair cipher. The Monoalphabetic approach's unique feature is that it maps every character to other character. The Playfair cipher, on the other hand, encrypts letter pairs using a key matrix. I'm also required to analyze letter frequencies from a large text file to find patterns. These exercises help me to understand the foundations of modern cryptography.";
  string encryptedtext="";
  map<char,char> mpp=getMapTable();
  for(int i=0;i<plaintext.length();i++){
    char c=tolower(plaintext[i]);
    if(mpp.find(c)!=mpp.end()){
      encryptedtext+=mpp[c];
    }else{
      encryptedtext+=c;
    }
  }
  cout<<encryptedtext<<endl;
  return 0;
}

