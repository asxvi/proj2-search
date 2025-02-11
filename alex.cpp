#include <iostream>
#include <string>

// #include "include/search.h"

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
;
  return (token.substr(firstAlpha, lastAlpha-firstAlpha));
}




// string cleanToken(const string& token) {
  
//   int firstAlpha;
//   // find index of first alpha char
//   for (int i = 0; i < token.size(); i++) {
//     if (isalpha(token[i])) {
//       firstAlpha = i;
//       break;
//     }
//   }

//   string parsed;
//   // find index of last alpha char
//   // for every substring 
//   bool noAlpha;
//   for (int i = firstAlpha; i < token.size(); i++) {
    
//     if(isalnum(token[i])){
//       cout << token[i];
//     }
//   }
//   return "";
// }




int main() {
  string uin;
  cout << "enter string: ";
  getline(cin, uin);

  string uo = cleanToken(uin);
  cout << endl << uo << endl;
  return 0;
}
