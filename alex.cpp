// #include "include/search.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  int firstAlpha = -1, lastAlpha = -1;
  bool alphaPresent = false, alphaPresent2 = false;

  // find index of first alpha char
  for (int i = 0; i < token.size(); i++) {
    if (isalpha(token[i])) {
      firstAlpha = i;
      break;
    }
  }

  for (int i = token.size()-1; i >= 0; i--) {
    if(isalpha(token[i])){
      lastAlpha = i;
      break;
    }
  }
  
  if(firstAlpha == -1){return "";}
  if(lastAlpha == -1){return "";}

  string concated;
  for(int i=firstAlpha; i<=lastAlpha; i++){
    concated += tolower(token[i]);
  }
  
  return concated;
}

set<string> gatherTokens(const string& text) {
  set<string> words;
  stringstream ss(text);
  string curr;

  while(ss >> curr && curr!= ""){
    words.insert(cleanToken(curr));
  }

  words.erase("");

  for(string s : words){
    cout << s << endl;
  }

  return words;
}

int main() {
  // string uin;
  // cout << "enter string: ";
  // getline(cin, uin);

  // string uo = cleanToken(uin);
  // cout << endl << uo << endl;

  cout << "Enter string: " ;
  string in;
  getline(cin, in);

  set<string> returns = gatherTokens(in); 




  return 0;
}
