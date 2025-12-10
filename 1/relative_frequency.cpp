#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
using namespace std;

void analyze(const string& text){
  ofstream out("output.txt");
  map<string,int> mono,di,tri;
  long tm=0,td=0,tt=0;
  string clean="";
  for(char c:text) if(isalpha(c)) clean+=tolower(c);
  if(clean.length()<3) return;
  for(size_t i=0;i<clean.length();i++){
    mono[clean.substr(i,1)]++; tm++;
    if(i+1<clean.length()){ di[clean.substr(i,2)]++; td++; }
    if(i+2<clean.length()){ tri[clean.substr(i,3)]++; tt++; }
  }
  out<<"Monograms\n";
  for(auto&p:mono){
    double f=(double)p.second/tm*100.0;
    out<<p.first<<" "<<f<<"\n";
  }
  out<<"\nDigrams\n";
  for(auto&p:di){
    double f=(double)p.second/td*100.0;
    if(f>0.1) out<<p.first<<" "<<f<<"\n";
  }
  out<<"\nTrigrams\n";
  for(auto&p:tri){
    double f=(double)p.second/tt*100.0;
    if(f>0.05) out<<p.first<<" "<<f<<"\n";
  }
  out.close();
}

int main(){
  ifstream file("corpus.txt");
  if(!file){
    cout<<"Error: corpus.txt not found.\n";
    return 1;
  }
  string text((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
  file.close();
  analyze(text);
  return 0;
}

