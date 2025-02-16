#pragma once

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

set<string> unionOperation(const map<string, set<string> >& index, vector<string> keysToCompare, int numOperations, bool foundOperator, set<string>& rv);

set<string> intersectionOperation(const map<string, set<string> >& index, vector<string> keysToCompare, int numOperations, bool foundOperator, set<string>& rv);

set<string> differenceOperation(const map<string, set<string> >& index, vector<string> keysToCompare, int numOperations, bool foundOperator, set<string>& rv);

string CleanSentence(const string& sentence, vector<string>& keys, string& compactString);

vector<string> turnStringToVect(const string& sentence);
