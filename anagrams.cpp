#include <iostream>
#include <fstream>
#include <istream>
#include <cstring>
#include <string>

#include <cassert>
#include <algorithm>
#include <sstream>

using namespace std;

const int MAXRESULTS   = 20;    // Max matches that can be found
const int MAXDICTWORDS = 30000; // Max words that can be read in

// PRIMARY FUNCTIONS

int lexiconBuilder(istream &dictfile, string dict[]);

int theJumbler(string word, const string dict[], int size,
string results[]);

void divulgeSolutions(const string results[], int size);


// HELPER FUNCTIONS

int lexiconBuilderHelper(istream &dictfile, string dict[], int count);

void storePermutations(string prefix, string rest, string results[], int& pos, const string dict[], int size);

void Loop(int i, size_t max, string& prefix, string& rest, string results[], int& pos, const string dict[], int size);

bool wordInArray(const string& word, const string dict[], int size, int pos);

void printArray(const string results[], int size, int pos);

int main()
{
      string results[MAXRESULTS];
      string dict[MAXDICTWORDS];
      ifstream dictfile; // file containing the list of
      int nwords;                // number of words read from
string word;
      dictfile.open("words.txt");
      if (!dictfile) {
             cout << "File not found!" << endl;
return (1); }
      nwords = lexiconBuilder(dictfile, dict);
#ifdef TEST1
    word = "rat";
#endif
    
#ifdef TEST2
    word = "babe";
#endif
    
#ifdef TEST3
    word = "plane";
#endif
    
#ifdef TEST4
    word = "maiden";
#endif
    
#ifdef TEST5
    word = "arrogant";
#endif
int numMatches = theJumbler(word, dict, nwords, results);
if (!numMatches)
 cout << "No matches found" << endl;
    
else divulgeSolutions(results, numMatches);
#ifdef TEST1
      assert(numMatches == 3 && (results[0] == "rat" ||
results[0] == "art"
             || results[0] == "tar"));
    
#endif
#ifdef TEST2
      assert(numMatches == 2 && (results[0] == "abbe" ||
results[0] == "babe"));
#endif
#ifdef TEST3
      assert(numMatches == 3 && (results[0] == "plane" ||
results[0] == "panel"
             || results[0] == "penal"));
#endif
#ifdef TEST4
      assert(numMatches == 2 && (results[0] == "maiden" ||
results[0] == "median"));
#endif
#ifdef TEST5
      assert(numMatches == 2 && (results[0] == "arrogant" ||
results[0] == "tarragon"));
#endif
return 0; }
// helper function with count parameter to prevent bad acess error if exceeding MAXDICTWORDS

int lexiconBuilderHelper(istream &dictfile, string dict[], int count){
    
    if (count >= MAXDICTWORDS) return 0; // if count exceeds, end
    
    string line;
    
    if (!getline(dictfile, line)) return 0; // if no line left, end
    
    else{
    
    dict[0] = line;
    count++;
    return 1 + lexiconBuilderHelper(dictfile, dict + 1, count); // if success, recursively calls function with incremented count
    
    }
}

int lexiconBuilder(istream &dictfile, string dict[]){
    return lexiconBuilderHelper(dictfile, dict, 0);
}

int theJumbler(string word, const string dict[], int size,
               string results[]){
    
    int pos = 0;
    storePermutations("", word, results, pos, dict, size);
    return pos;
} // most of the work is done by storePermutations. theJumbler only adds the functionality of returning the number of results stored (which is required since pos was passed in by reference so we need a variable pos outside storePermutation's call to it.

void divulgeSolutions(const string results[], int size){
    printArray(results, size, 0);
} // relies on printArray which has an extra input parameter for stopping condition

void storePermutations(string prefix, string rest, string results[], int& pos, const string dict[], int size) {
 if (rest.size() == 0) {
     
     
     if (!wordInArray(prefix, results, pos, 0) && pos < MAXRESULTS){ // only add it results if it's not already been added and if MAXRESULTS has not been exceeded
         if (wordInArray(prefix, dict, size, 0)){ // only add it to results if the permuation is in dict
             results[pos] = prefix;
             pos++; // increment the current pos to add in results also accordingly
         }
     }
  }
    
 else Loop(0, rest.size(), prefix, rest, results, pos, dict, size);
} // works co-recursively with Loop. Loop creates permutations while storePermutation verifies words, checks for duplicates and for exceeding MAXRESULTS

void Loop(int i, size_t max, string& prefix, string& rest, string results[], int& pos, const string dict[], int size){
    
    if (i>=max) return;
    
    string rest_0 = rest;
    string prefix_0 = prefix;
    
    prefix += rest[i];

    rest = rest.substr(0, i) + rest.substr(i+1);
    
    storePermutations(prefix, rest, results, pos, dict, size);
    
    rest = rest_0;
    prefix = prefix_0;
    
    Loop(i+1, max, prefix, rest, results, pos, dict, size);
    
} // translated from psuedocode in the FAQ

bool wordInArray(const string& word, const string dict[], int size, int pos){
    if (pos >= size) return false;

    else {
        if (dict[pos] == word) return true;
        return wordInArray(word, dict, size, pos+1);
    }
} // linear word search used twice in storePermutations

void printArray(const string results[], int size, int pos){
    if (pos >= size) return;

    else {
        cout << "Matching word " << results[pos] << endl;
        printArray(results, size, pos+1);
    }
} // simple print function to match expected output from results


