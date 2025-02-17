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
    
    if (!ispunct(token[i])) {
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
    if (!ispunct(token[i])) {
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

  while (ss >> curr) {
    words.insert(cleanToken(curr));
  }

  // we do not want to keep spaces as a token
  words.erase("");
  return words;
}

int buildIndex(const string& filename, map<string, set<string> >& index) {
  fstream file(filename);
  string line, url;
  int numWebp = 0;

  if (file.is_open()) {
    int i = 1;    //line 1 of file
    while (getline(file, line)) {
      // we know that the tokens are on even lines and urls are on odd lines
      if (i % 2 == 0) {
        set<string> tokens = gatherTokens(line);

        for (string s : tokens) {
          index[s].insert(url);
        }
      }
      else {
        url = line;
        numWebp++;
      }
      i++;
    }
  }
  return numWebp;
}

//want to keep + - in vector
set<string> findQueryMatches(const map<string, set<string> >& index, const string& sentence) {
  stringstream ss(sentence);
  vector<string> keysToCompare;
  string cleaned, word;
  set<string> rv;

  while(ss >> word){
    keysToCompare.push_back(word);
  }

  if(index.find(cleanToken(keysToCompare[0])) != index.end()) {
    rv = index.at(cleanToken(keysToCompare[0]));
    
    if (keysToCompare.size() == 1) {return rv;}
  }

  for(int i=1; i<keysToCompare.size(); i++){
    // intersect operation
    if(keysToCompare[i][0] == '+'){
      if(index.find(cleanToken(keysToCompare[i])) != index.end()){
        set<string> a = rv, b = index.at(cleanToken(keysToCompare[i])), temp;

        set_intersection(a.begin(), a.end(), b.begin(), b.end(), inserter(temp, temp.begin()));
        rv = temp;
      }
      else{
        set<string> a = rv, b = {}, temp;

        set_intersection(a.begin(), a.end(), b.begin(), b.end(), inserter(temp, temp.begin()));
        rv = temp;
      }
    }
    // difference operation
    else if(keysToCompare[i][0] == '-' && index.find(cleanToken(keysToCompare[i])) != index.end()){
      set<string> a = rv, b = index.at(cleanToken(keysToCompare[i])), temp;
      
      set_difference(a.begin(), a.end(), b.begin(), b.end(), inserter(temp, temp.begin()));
      rv = temp;
    }
    //otherwise has to be a space: union operation
    else{
      if(index.find(cleanToken(keysToCompare[i])) != index.end()){
        set<string> a = rv, b = index.at(cleanToken(keysToCompare[i])), temp;
        
        set_union(a.begin(), a.end(), b.begin(), b.end(), inserter(temp, temp.begin()));
        rv = temp;   
      }
    }
  }
  return rv;
}


void searchEngine(const string& filename) {
  map<string, set<string> > index;
  int urlsProcessed = buildIndex(filename, index);
  
  if(urlsProcessed == 0){
    cout << "Invalid filename." << endl;
  } 
  cout << "Stand by while building index..." << endl;
  cout << "Indexed " << urlsProcessed << " pages containing " << index.size() << " unique terms" << endl;

  string query = "-1";
  do{
    cout << "Enter query sentence (press enter to quit): ";
    getline(cin, query);
    if(query == ""){break;}
    
    set<string> totalUrls = findQueryMatches(index, query);
    cout << "Found " << totalUrls.size() << " matching pages" << endl;
    

    for(string url : totalUrls){
      cout << url << endl;
    }
    
    }while(query != "");  
    cout << "Thank you for searching!" << endl;
}