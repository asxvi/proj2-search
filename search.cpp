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
  // TODO student
  return {};
}

int buildIndex(const string& filename, map<string, set<string>>& index) {
  // TODO student
  return 0;
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  // TODO student
  return {};
}

void searchEngine(const string& filename) {
  // TODO student
}
