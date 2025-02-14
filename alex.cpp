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

string CleanSentence(const string& sentence, vector<string>& keys){
  cout << sentence << ":   " << endl;
  string compact, spaced;

  bool firstCharFound = false;
  char prev = ' ';
  
  for(int i=0; i<sentence.size(); i++){
    char c = sentence[i];

    // check for special symbols. expected outcome for compact = "a+b", for spaced = "a + b"
    if(c == '+' || c == '-'){
      // check for space before symbol, remove it then add the symbol
      if(compact.back() == ' ' && !compact.empty()){
        compact.pop_back();
      }
      compact += c;
      // do not need for space if we dont care about spaces bc ss parses it out
      // add before symbol if no space present, else just add symbol and space
      // if(spaced.back() != ' ' && !spaced.empty()){
      //   spaced += ' '; 
      // }
      // spaced += c;
      // if(sentence[i+1] != ' ' && i+1 < sentence.size()){
      //   spaced += ' '; 
      // }
      spaced += ' ';
    }

    // if the curr char is a space and previous char was also space
    else if(isspace(c)){
      if(prev != ' ' && firstCharFound){
        spaced += ' ';
        compact += ' ';
        
        if(prev == '+' || prev == '-'){
          compact.pop_back();
        }
      }
    }

    //otherwise add normal char and then update previous char to curr right before switch
    else {
      if(!isspace(c)){
        compact += c;
        spaced += c;
        firstCharFound = true;
      }
    }
    prev = c;
  }  
  // cout << "compact: |" << compact << endl;
  // cout << "Spaced: |" << spaced << endl;

  // directly modify the vector of words, and then return the compact string with no spaces
  keys = turnStringToVect(spaced);
  return compact;
}

// lets    see-if     this+ works   or -not
vector<string> turnStringToVect(const string& sentence){
  stringstream ss(sentence);
  string word;
  vector<string> ordered_input;

  while(ss >> word){      
    ordered_input.push_back(cleanToken(word));
  }
  return ordered_input;
}

string cleanToken(const string& token) {
  int firstAlpha = -1, lastAlpha = -1;
  bool alphaPresent = false, haveChar = false;

  // find index of first alpha char
  for (int i = 0; i < token.size(); i++) {
    if (isalnum(token[i])) {
        if(haveChar == false){
          firstAlpha = i;
          haveChar = true;
        }
      if(isalpha(token[i])){
        alphaPresent = true;
        break;
      }
    }
  }

  for (int i = token.size()-1; i >= 0; i--) {
    if(isalnum(token[i])){
      lastAlpha = i;
      break;
    }
  }
  
  if(firstAlpha == -1 || alphaPresent == false){return "";}
  if(lastAlpha == -1 || alphaPresent == false){return "";}

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

  return words;
}


// int buildIndex(const string& filename, map<string, set<string> >& index) {
//   fstream file("./data/"+filename);
//   string line, url;
//   int numWebp = 0;

//   if(file.is_open()){
//     int i=1;
//     while(getline(file, line)){
//       if(i%2 ==0){
//         // cout << "line: " << i << " " << line;
//         set<string> tokens = gatherTokens(line);

//         for(string s : tokens){
//           index[s].insert(url);
//         }
//       }
//       else{
//         url = line;
//         numWebp ++;
//         // cout << "url: " << url;
//       }
//       i++;
//     }
//   }

//   /// i am wondering whether order matters inside the map's set
//   // in my test case for tiny.txt, say i have fish: www.shoppinglist.com, www.dr.seuss.net
//   // ...nvm i think it depends on order bc sets are alpha ordered.
//   cout << "whats in our map: " << endl;
//   int k=0;
//   for(pair<string, set<string> > p : index){
//       cout << p.first << ": ";

//       for(string url : p.second){
//         cout << url << ", ";
//       }
//       cout << endl;
//       k++;
//   }
//   cout << "\n\n" << k;
//   return numWebp;
// }

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

set<string> findQueryMatches(const map<string, set<string> >& index, const string& sentence) {
  stringstream ss(sentence);
  string word;
  vector<string> ordered_input;

  // our return set of urls after complete query is finished
  set<string> rv;
  
  string cleanedS = cleanToken(sentence);
  cout << cleanedS;
  
  //create a vector with all of the words in user sentence input
  while(ss >> word){
    // if(index.find(word) != index.end()){
      ordered_input.push_back(cleanToken(word));
      cout << word;
    // }
  }
  
  // special case where there is only 1 string input. 
  // go through every word[KEY] and their url(s)[VALUE]. if the key matches the sentence string
  // then we add every url[VALUE] that has that KEY to our return set, rv
  // if(ordered_input.size() == 1){
  // index.at(ordered_input[0]);
  //     for(pair<string, set<string> > p : index){
  //       if(ordered_input[0] == p.first){
  //         for(string url : p.second){
  //           rv.insert(url);  
  //         }
  //     }
  //   }
  // }

  int numOperations=0;
  if (ordered_input.size() == 1) {
    cout << "size 1";
    return index.at(ordered_input[0]);  // Direct lookup
  }
  else{

    // go through every character in sentence, and stop at 3 specific chars: ' ', -, +
    for(int i=0; i<sentence.size(); i++){
      if(sentence[i] == ' '){
        set<string> a, b;
        numOperations++;

        for(pair<string, set<string> > p : index){
          if(ordered_input[numOperations-1] == p.first){
            for(string url : p.second){
              a.insert(url);  
            }
          }
          else if(ordered_input[numOperations] == p.first){
            for(string url : p.second){
              b.insert(url);  
            }
          }
        }
        set_union(a.begin(), a.end(), b.begin(), b.end(), inserter(rv, rv.begin()));
      }

      //special case, when numOperations is zero, we need to intersect the set of the first 2 words
    //   else if(sentence[i] == '+'){
    //     numOperations++;
    //     if(numOperations == 1){
    //       set<string> a,b;

    //       for(pair<string, set<string> > p : index){
    //         if(ordered_input[numOperations-1] == p.first){
    //           for(string url : p.second){
    //             cout << "yo";
    //             a.insert(url);  
    //           }
    //         }
    //         else if(ordered_input[numOperations] == p.first){
    //           for(string url : p.second){
    //             cout << "ea";
    //             b.insert(url);  
    //           }
    //         }
    //       }
    //       // set_intersection(a.begin(), a.end(), b.begin(), b.end(), inserter(rv, rv.begin()));
    //     // }
    //     // else{
    //     //   set<string> a = rv;
    //     //   set<string> b;

    //     //   for(pair<string, set<string> > p : index){
    //     //     if(ordered_input[numOperations] == p.first){
    //     //       for(string url : p.second){
    //     //         b.insert(url);  
    //     //       }
    //     //     }
    //     //   }
    //     //   set_intersection(a.begin(), a.end(), b.begin(), b.end(), inserter(rv, rv.begin()));
    //     // }
    //   }
    //   else if(sentence[i] == '-'){
    //     //set union
    //   }
    // }
  }


  // might parse out by characters searching for + - ' '
  // for(int i=0; i<sentence.size(); i++){
  //   if(sentence[i] == ' '){

  //   }
}

    int i=0;
    for(string word : rv){
      cout << i << " :" << word << "." << endl;
      i++;
    }

  return rv;
}


int main() {
  // string uin;
  // cout << "enter string: ";
  // getline(cin, uin);

  // string uo = cleanToken(uin);
  // cout << endl << uo << endl;

  // cout << "Enter string: " ;
  // string in;
  // getline(cin, in);

  // set<string> returns = gatherTokens(in); 

  string filename = "./data/tiny.txt";
  map<string, set<string> > index;
  int a = buildIndex(filename, index);

  // cout << a;

  // cout << "enter a string: ";
  // string ui; 
  // getline(cin, ui);  
  // set<string> matches = findQueryMatches(index, ui);

  // for(string url : matches){
  //   cout << url << ", ";
  // }

  string inp;
  cout << "input sentence: ";
  getline(cin, inp);
  // inp = "        hello my    name+is alex -lets run+ this   ok";
  vector<string> alexander;
  string b = CleanSentence(inp, alexander);
  cout << "\n\n" << b;
  return 0;
}
