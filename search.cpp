#include "include/search.h"

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

  while(ss >> curr && curr != ""){
    words.insert(cleanToken(curr));
  }
  words.erase("");

  // for(string s : words){
  //   cout << s << endl;
  // }

  return words;
}

int buildIndex(const string& filename, map<string, set<string> >& index) {
  fstream file(filename);
  string line, url;
  int numWebp = 0;

  if(file.is_open()){
    int i=1;
    while(getline(file, line)){
      if(i%2 ==0){
        set<string> tokens = gatherTokens(line);

        for(string s : tokens){
          index[s].insert(url);
        }
      }
      else{
        url = line;
        numWebp ++;
      }
      i++;
    }
  }


  return numWebp;
}

set<string> findQueryMatches(const map<string, set<string>>& index, const string& sentence) {
  // TODO student
  return {};
}

void searchEngine(const string& filename) {
  // TODO student
}
