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

vector<string> turnStringToVect(const string& sentence);
string cleanToken(const string& token);


// parameters: index: the map os words[KEYS] and their respective urls[VALUES], 
//             keysToCompare: our cleaned vector with just words
//             numOperations: the number of +,- or spaces there are
//             foundOperator: determine what we use for set a. If true, we use previous out rv.
//             rv: our return string we use to also directly access what was previously there if foundOperator is true
set<string> unionOperation(const map<string, set<string> >& index, vector<string> keysToCompare, int numOperations, bool foundOperator, set<string>& rv){
  if(!foundOperator){
    cout << "not found in union" << endl;
    set<string> a, b;

    for (pair<string, set<string> > p : index) {
      if (numOperations >= 1 && keysToCompare[numOperations - 1] == p.first) {
        for (string url : p.second) {
          a.insert(url);
        }
      } 
      if (keysToCompare[numOperations] == p.first) {
        for (string url : p.second) {
          b.insert(url);
        }
      }
    }
    set_union(a.begin(), a.end(), b.begin(), b.end(),inserter(rv, rv.begin()));
  }
  else{
    cout << "found in union" << endl;
    set<string> a = rv, b;

    for (pair<string, set<string> > p : index) {
      if (keysToCompare[numOperations] == p.first) {
        for (string url : p.second) {
          b.insert(url);
        }
      }
    }
    set_union(a.begin(), a.end(), b.begin(), b.end(),inserter(rv, rv.begin()));
  }

  return rv;
}


// parameters: index: the map os words[KEYS] and their respective urls[VALUES], 
//             keysToCompare: our cleaned vector with just words
//             numOperations: the number of +,- or spaces there are
//             foundOperator: determine what we use for set a. If true, we use previous out rv.
//             rv: our return string we use to also directly access what was previously there if foundOperator is true
set<string> intersectionOperation(const map<string, set<string> >& index, vector<string> keysToCompare, int numOperations, bool foundOperator, set<string>& rv){
  if(!foundOperator){
    cout << "not found in intersection" << endl;
    set<string> a, b;

    for (pair<string, set<string> > p : index) {
      if (numOperations >= 1 && keysToCompare[numOperations - 1] == p.first) {
        for (string url : p.second) {
          a.insert(url);
        }
      } else if (keysToCompare[numOperations] == p.first) {
        for (string url : p.second) {
          b.insert(url);
        }
      }
    }
    set_intersection(a.begin(), a.end(), b.begin(), b.end(), inserter(rv, rv.begin()));
  }
  else{
    cout << "found in intersection" << endl;
    set<string> a = rv, b;

    for (pair<string, set<string> > p : index) {
      if (keysToCompare[numOperations] == p.first) {
        for (string url : p.second) {
          b.insert(url);
        }
      }
    }
    set_intersection(a.begin(), a.end(), b.begin(), b.end(), inserter(rv, rv.begin()));
  }
  
  return rv;
}

// parameters: index: the map os words[KEYS] and their respective urls[VALUES], 
//             keysToCompare: our cleaned vector with just words
//             numOperations: the number of +,- or spaces there are
//             foundOperator: determine what we use for set a. If true, we use previous out rv.
set<string> differenceOperation(const map<string, set<string> >& index, vector<string> keysToCompare, int numOperations, bool foundOperator, set<string>& rv){
  
  if(!foundOperator){
    cout << "not found in diff" << endl;
    set<string> a, b;

    for (pair<string, set<string> > p : index) {
      if (numOperations >= 1 && keysToCompare[numOperations - 1] == p.first) {
        for (string url : p.second) {
          a.insert(url);
        }
      } else if (keysToCompare[numOperations] == p.first) {
        for (string url : p.second) {
          b.insert(url);
        }
      }
    }
    set_difference(a.begin(), a.end(), b.begin(), b.end(), inserter(rv, rv.begin()));
  }
  else{
    cout << "found in diff" << endl;
    set<string> a = rv, b;

    for (pair<string, set<string> > p : index) {
      if (keysToCompare[numOperations] == p.first) {
        for (string url : p.second) {
          b.insert(url);
        }
      }
    }
    set_difference(a.begin(), a.end(), b.begin(), b.end(), inserter(rv, rv.begin()));

  }
  return rv;
}


 
string CleanSentence(const string& sentence, vector<string>& keys,
                     string& compactString) {
  cout << "our dirty sentence: " << sentence << ":   " << endl << endl;
  string compact, spaced;

  bool firstCharFound = false;
  char prev = ' ';

  for (int i = 0; i < sentence.size(); i++) {
    char c = sentence[i];

    // Check for special symbols (+, -)
    if (c == '+' || c == '-') {
      if (i > 0 && i + 1 < sentence.size() && isalnum(sentence[i - 1]) &&
          isalnum(sentence[i + 1])) {
        compact += c;
        spaced += c;
      } else {
        // Remove space before symbol in compact version
        if (!compact.empty() && compact.back() == ' ') {
          compact.pop_back();
        }
        compact += c;

        // Handle spacing rules for spaced version
        if (!spaced.empty() && spaced.back() != ' ') {
          spaced += ' ';
        }
        spaced += c;

        // Ensure only one space after if needed
        if (i + 1 < sentence.size() && sentence[i + 1] != ' ') {
          spaced += ' ';
        }
      }
    }

    // Handle spaces
    else if (isspace(c)) {
      // if (i > 0 && i + 1 < sentence.size() && isalnum(sentence[i - 1]) &&
      // isalnum(sentence[i + 1])) {
      if (prev != ' ' && firstCharFound) {
        spaced += ' ';
        compact += ' ';
      }
      // i believe this should work, removing the space after the + or -
      // //////////////////
      if (prev == '+' || prev == '-') {
        compact.pop_back();
      }
    }

    // Normal characters
    else {
      compact += c;
      spaced += c;
      firstCharFound = true;
    }

    prev = c;
  }

  cout << "Spaced::" << spaced << endl;
  cout << "Compact::" << compact << endl;
  // Convert the spaced version into a vector of words
  keys = turnStringToVect(spaced);
  compactString = compact;
  return compact;
}

// lets    see-if     this+ works   or -not
// add the cleaned and nicely spaced out words to vector. This includes words
// like: alex+stan, but excludes words like alex +stan or alex+ stan... bc our
// original CleanToken function
vector<string> turnStringToVect(const string& sentence) {
  vector<string> ordered_input;
  stringstream ss(sentence);
  string word;

  while (ss >> word) {
    // only add real words to vector, symbols like +,- get converted into ""
    // empty string
    if (cleanToken(word) != "") {
      ordered_input.push_back(cleanToken(word));
    }
  }

  return ordered_input;
}

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

  // we do not want to keep spaces as a token
  words.erase("");
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
  
  cout << "Number of web pages processed: " << urlsProcessed << endl;
  cout << "Number of distinct words: " << index.size() << endl;
  
  string query;
  cout << "Enter Query: ";
  getline(cin, query);
  set<string> totalUrls = findQueryMatches(index, query);

  if(totalUrls.size() == 0){
    return;
  }
  else{
    for(string url : totalUrls){
      cout << url << endl;
    }
  }
}

int main() {
  // string filename = "./data/tiny.txt";
  // map<string, set<string> > index;
  // int a = buildIndex(filename, index);
 
  // // shopping list, wolf shop, seus, shop list seus
  // string inters = "fish +eggs", uni = " eat eggs", dif = "red -violet", comb = "fish +eggs red -violet", onee= "fish";

  // set<string> matches = findQueryMatches(index, onee);

  // cout << "corresponding urls in earnest: " << endl;
  // for (string match : matches) {
  //   cout << cleanToken(match) << endl;
  // }

  searchEngine("");
  return 0;
}

