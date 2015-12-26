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
    Token(string _value, string _type) {
        value = _value;
        type = _type;
    }
    bool operator == (const Token& _token) const {
        return value == _token.value && type == _token.type;
    }
};

class Lexer {
  public:
    Lexer(string file_path);
    void set_tokens(vector<string> simple_tokens);
    vector<Token> get_tokens();
  private:
    vector<string> simple_tokens;
    vector<Token> tokens;
};

string get_type(string str);

bool is_number_or_letter(char c);

bool is_number(string str);

#endif /* Lexer_h */
