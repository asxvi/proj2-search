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
  // for(string s : words){
  //   cout << s << endl;
  // }
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
  fstream file("./data/"+filename);
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

  set<string> rv;
  
  while(ss >> word){
    if(index.find(word) != index.end()){
      cout << word;
      
      // rv.insert(index[word]);
    }
  }

  return rv;
}


int main() {
  string uin;
  cout << "enter string: ";
  getline(cin, uin);

  string uo = cleanToken(uin);
  cout << endl << uo << endl;

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
  //   cout << url << " ";
  // }

  return 0;
}
