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

set<string> findQueryMatches(const map<string, set<string> >& index,
                             const string& sentence) {
  vector<string> keysToCompare;
  string cleaned, compact;
  cleaned = CleanSentence(sentence, keysToCompare, compact);
  set<string> rv;
  // our return set of urls after complete query is finished

  // the number of +, - or spaces (|)
  int numOperations = 0;

  if (keysToCompare.size() == 1) {
    cout << "size 1" << endl;           ////////
    return index.at(keysToCompare[0]);  // Direct lookup
  }

  // once youve done an operation, the first set will always be a copy of rv and
  // we can either |, +, or - to it

  else {
    cout << "\n\n\nmore than one word.      " << compact << endl;

    int count = 0;
    // go through every character in sentence, and stop at 3 specific chars: '
    // ', -, +
    for (int i = 0; i < compact.size(); i++) {
      // UNION looking for SPACES
      // union can only happen between 2 words. can not happen in end cases
      if (compact[i] == ' ' && i >= 1) {
        set<string> a, b;
        numOperations++;

        for (pair<string, set<string> > p : index) {
          if (keysToCompare[numOperations - 1] == p.first) {
            for (string url : p.second) {
              a.insert(url);
            }
          } else if (keysToCompare[numOperations] == p.first) {
            for (string url : p.second) {
              b.insert(url);
            }
          }
        }
        set_union(a.begin(), a.end(), b.begin(), b.end(),
                  inserter(rv, rv.begin()));
      }

      // INTERSECTION looking for PLUS +
      else if (compact[i] == '+' && i >= 1) {
        set<string> a, b;
        numOperations++;

        for (pair<string, set<string> > p : index) {
          if (keysToCompare[numOperations - 1] == p.first) {
            for (string url : p.second) {
              a.insert(url);
            }
          } else if (keysToCompare[numOperations] == p.first) {
            for (string url : p.second) {
              b.insert(url);
            }
          }
        }
        set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                         inserter(rv, rv.begin()));
      }

      // SET DIFFERENCE looking for MINUS -
      else if (compact[i] == '-' && i >= 1) {
        set<string> a, b;
        numOperations++;

        for (pair<string, set<string> > p : index) {
          if (keysToCompare[numOperations - 1] == p.first) {
            for (string url : p.second) {
              a.insert(url);
            }
          } else if (keysToCompare[numOperations] == p.first) {
            for (string url : p.second) {
              b.insert(url);
            }
          }
        }
        set_difference(a.begin(), a.end(), b.begin(), b.end(),
                       inserter(rv, rv.begin()));
      }

    }  // for iter every char
  }  // else statement

  // int i=0;
  // for(string word : rv){
  //   cout << i << " :" << word << "." << endl;
  //   i++;
  // }

  return rv;
}  // main close

int main() {
  string filename = "./data/tiny.txt";
  map<string, set<string> > index;
  int a = buildIndex(filename, index);

  string ui;
  // cout << "enter a string: ";
  // getline(cin, ui);
  // ui = "   i    +   figure    lets see-how    this+    works right-now at
  // -moment    "; ui = "       red     gre-en    blue    milk"; ui = "
  // fish+bread";
  ui = "       fish- bread";
  set<string> matches = findQueryMatches(index, ui);

  cout << "corresponding urls in earnest: " << endl;
  for (string match : matches) {
    cout << match << endl;
  }

  return 0;
}