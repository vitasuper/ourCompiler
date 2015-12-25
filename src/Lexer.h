//
//  Lexer.h
//

#ifndef Lexer_h
#define Lexer_h

#include <vector>
#include <string>

using namespace std;

struct Token {
    string value;
    string type;
    Token(string v, string t) {
        value = v;
        type = t;
    }
    bool operator == (const Token& t) const {
        return value == t.value && type == t.type;
    }
};

class Lexer {
  public:
    Lexer(string filePath);
    void setTokens(vector<string> simpleTokens);
    vector<Token> getTokens();
  private:
    vector<Token> tokens;
};

string getType(string str);

bool isNumber(string str);

#endif /* Lexer_h */
