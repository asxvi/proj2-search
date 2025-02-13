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
  bool alphaPresent = false, haveChar = false;

  // find index of first alpha char
  for (int i = 0; i < token.size(); i++) {
    if (isalnum(token[i])) {
      if (haveChar == false) {
        firstAlpha = i;
        haveChar = true;
      }
      if (isalpha(token[i])) {
        alphaPresent = true;
        break;
      }
    }
  }

  for (int i = token.size() - 1; i >= 0; i--) {
    if (isalnum(token[i])) {
      lastAlpha = i;
      break;
    }
  }

  if (firstAlpha == -1 || alphaPresent == false) {
    return "";
  }
  if (lastAlpha == -1 || alphaPresent == false) {
    return "";
  }

  string concated;
  for (int i = firstAlpha; i <= lastAlpha; i++) {
    concated += tolower(token[i]);
  }

  return concated;
}

set<string> gatherTokens(const string& text) {
  set<string> words;
  stringstream ss(text);
  string curr;

  while (ss >> curr && curr != "") {
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

  if (file.is_open()) {
    int i = 1;
    while (getline(file, line)) {
      if (i % 2 == 0) {
        set<string> tokens = gatherTokens(line);

        for (string s : tokens) {
          index[s].insert(url);
        }
      } else {
        url = line;
        numWebp++;
      }
      i++;
    }
  }
  return numWebp;
}

set<string> findQueryMatches(const map<string, set<string> >& index, const string& sentence) {
  stringstream ss(sentence);
  string word;
  vector<string> ordered_input;

  // our return set of urls after complete query is finished
  set<string> rv;

  // create a vector with all of the words in user sentence input
  while (ss >> word) {
    if (index.find(word) != index.end()) {
      ordered_input.push_back(cleanToken(word));
    }
  }

  // special case where there is only 1 string input.
  // go through every word[KEY] and their url(s)[VALUE]. if the key matches the
  // sentence string then we add every url[VALUE] that has that KEY to our
  // return set, rv
  if (ordered_input.size() == 1) {
    for (pair<string, set<string> > p : index) {
      if (ordered_input[0] == p.first) {
        for (string url : p.second) {
          rv.insert(url);
        }
      }
    }
  }
  return rv;
}

void searchEngine(const string& filename) {
  // TODO student
}
