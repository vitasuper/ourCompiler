//
//  Tokenizer.h
//

#ifndef Tokenizer_h
#define Tokenizer_h

#include <vector>
#include <string>

using namespace std;

class Tokenizer {
  public:
    Tokenizer(string filePath);
    vector<string> getTokens();
  
  private:
    string filePath;
    vector<string> tokens;
};

bool isNumberOrLetter(char c);

#endif /* Tokenizer_h */
