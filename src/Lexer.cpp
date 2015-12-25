//
//  Lexer.cpp
//

#include <vector>
#include <string>
#include "Lexer.h"
#include "Tokenizer.h"

Lexer::Lexer(string filePath) {
    Tokenizer tn = Tokenizer(filePath);
    vector<string> simpleTokens = tn.getTokens();
    setTokens(simpleTokens);
}

void Lexer::setTokens(vector<string> simpleTokens) {
    vector<string>::iterator it;
    for (it = simpleTokens.begin(); it != simpleTokens.end(); ++it) {
        string type = getType(*it);
        tokens.push_back(Token(*it, type));
    }
}

// Type list:
//
// Identifiers:
// simpleTokens:  create, view, as, output, select, from, extract, regex, on, return, group, and, Token, pattern
// Types:         Corresponding uppercase of the identifiers
//
// Symbols:
// simpleTokens:  . ; ( ) < > { } ,
// Types:         DOT SEMICOLON LEFTBRACKET RIGHTBRACKET LEFTPOINTBRACKET RIGHTPOINTBRACKET LEFTBRACE RIGHTBRACE COMMA
//
// Numbers
// simpleTokens:  eg. 1 23 456
// Types:         NUMBERS
//
// Regular expressions
// simpleTokens:  eg. /[A-Z][a-z]*/
// Types:         REALREGEX
//
// Others (normal words, normal symbols)
// simpleTokens:  eg. /[A-Z][a-z]*/
// Types:         REALREGEX

string getType(string str) {
    string type = "ID";
    if (str == "create") {
        type = "CREATE";
    } else if (str == "view") {
        type = "VIEW";
    } else if (str == "as") {
        type = "AS";
    } else if (str == "output") {
        type = "output";
    } else if (str == "select") {
        type = "SELECT";
    } else if (str == "from") {
        type = "FROM";
    } else if (str == "extract") {
        type = "EXTRACT";
    } else if (str == "regex") {
        type = "REGEX";
    } else if (str == "on") {
        type = "ON";
    } else if (str == "return") {
        type = "RETURN";
    } else if (str == "group") {
        type = "GROUP";
    } else if (str == "and") {
        type = "AND";
    } else if (str == "Token") {
        type = "TOKEN";
    } else if (str == "pattern") {
        type = "PATTERN";
    } else if (str == ".") {
        type = "DOT";
    } else if (str == ";") {
        type = "SEMICOLON";
    } else if (str == "(") {
        type = "LEFTBRACKET";
    } else if (str == ")") {
        type = "RIGHTBRACKET";
    } else if (str == "<") {
        type = "LEFTPOINTBRACKET";
    } else if (str == ">") {
        type = "RIGHTPOINTBRACKET";
    } else if (str == "{") {
        type = "LEFTBRACE";
    } else if (str == "}") {
        type = "RIGHTBRACE";
    } else if (str == ",") {
        type = "COMMA";
    } else if (str[0] == '/' && str[str.size()-1] == '/') {
        type = "REALREGEX";
    } else if (isNumber(str)) {
        type = "NUMBER";
    } else {
        type = "ID";
    }
    return type;
}

vector<Token> Lexer::getTokens() {
    return tokens;
}

bool isNumber(string str) {
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}
