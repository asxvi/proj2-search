// #include "include/search.h"
#include "personalFunctions.h"

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
//             foundOperator: determine what we use for set a. If true, we use
//             previous out rv. rv: our return string we use to also directly
//             access what was previously there if foundOperator is true
set<string> unionOperation(const map<string, set<string> >& index,
                           vector<string> keysToCompare, int numOperations,
                           bool foundOperator, set<string>& rv) {
  if (!foundOperator) {
    cout << "not found in union" << endl;
    set<string> a, b;

    for (pair<string, set<string> > p : index) {
      if (keysToCompare[numOperations - 1] == p.first) {
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
    set_union(a.begin(), a.end(), b.begin(), b.end(), inserter(rv, rv.begin()));
  } else {
    cout << "found in union" << endl;
    set<string> a = rv, b;

    for (pair<string, set<string> > p : index) {
      if (keysToCompare[numOperations] == p.first) {
        for (string url : p.second) {
          b.insert(url);
        }
      }
    }
    set_union(a.begin(), a.end(), b.begin(), b.end(), inserter(rv, rv.begin()));
  }

  return rv;
}

// parameters: index: the map os words[KEYS] and their respective urls[VALUES],
//             keysToCompare: our cleaned vector with just words
//             numOperations: the number of +,- or spaces there are
//             foundOperator: determine what we use for set a. If true, we use
//             previous out rv. rv: our return string we use to also directly
//             access what was previously there if foundOperator is true
set<string> intersectionOperation(const map<string, set<string> >& index,
                                  vector<string> keysToCompare,
                                  int numOperations, bool foundOperator,
                                  set<string>& rv) {
  if (!foundOperator) {
    cout << "not found in intersection" << endl;
    set<string> a, b;

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
  } else {
    cout << "found in intersection" << endl;
    set<string> a = rv, b;

    for (pair<string, set<string> > p : index) {
      if (keysToCompare[numOperations] == p.first) {
        for (string url : p.second) {
          b.insert(url);
        }
      }
    }
    set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                     inserter(rv, rv.begin()));
  }

  return rv;
}

// parameters: index: the map os words[KEYS] and their respective urls[VALUES],
//             keysToCompare: our cleaned vector with just words
//             numOperations: the number of +,- or spaces there are
//             foundOperator: determine what we use for set a. If true, we use
//             previous out rv.
set<string> differenceOperation(const map<string, set<string> >& index,
                                vector<string> keysToCompare, int numOperations,
                                bool foundOperator, set<string>& rv) {
  if (!foundOperator) {
    cout << "not found in diff" << endl;
    set<string> a, b;

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
  } else {
    cout << "found in diff" << endl;
    set<string> a = rv, b;

    for (pair<string, set<string> > p : index) {
      if (keysToCompare[numOperations] == p.first) {
        for (string url : p.second) {
          b.insert(url);
        }
      }
    }
    set_difference(a.begin(), a.end(), b.begin(), b.end(),
                   inserter(rv, rv.begin()));
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
