//
//  Lexer.cpp
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Lexer.h"

Lexer::Lexer(string file_path) {
    // Open the file with the ReadOnly mode.
    std::ifstream file(file_path.c_str(), ios::in);
    
    char c;
    string token = "";
    string regex = "";
    bool regex_flag = false;
    while(!file.eof()) {
        file.get(c);
        if (regex_flag) {
            regex += c;
            if (c == '/') {
                simple_tokens.push_back(regex);
                regex = "";
                regex_flag = false;
            }
        } else {
            // While meeting a blank, push the word into the simple_tokens vector.
            if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
                if (token.size() > 0) {
                    simple_tokens.push_back(token);
                    token = "";
                }
            } else {
                if (is_number_or_letter(c)) {
                    if (token.size() > 0 && !is_number_or_letter(token[0])) {
                        // Push the special character such as "," "'" into the simple_tokens vector.
                        simple_tokens.push_back(token);
                        token = "";
                        token += c;
                    } else {
                        // Continue spanning the word.
                        token += c;
                    }
                } else {
                    // If meet some special character, push the existed word into the simple_tokens vector.
                    if (token.size() > 0) {
                        simple_tokens.push_back(token);
                        token = "";
                    }
                    if (c != '/') {
                        token += c;
                    } else {
                        // Meeting the beginning of regular expression.
                        regex += c;
                        regex_flag = true;
                    }
                }
            }
        }
    }
    file.close();
    set_tokens(simple_tokens);
}

void Lexer::set_tokens(vector<string> _simple_tokens) {
    vector<string>::iterator it;
    for (it = _simple_tokens.begin(); it != _simple_tokens.end(); ++it) {
        string type = get_type(*it);
        if (type == "REG") {
            (*it).erase((*it).begin());
            (*it).erase((*it).end() - 1);
        }
        tokens.push_back(Token(*it, type));
    }
}

//
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
// Types:         REG
//
// Others (normal words, normal symbols)
// simpleTokens:  eg. /[A-Z][a-z]*/
// Types:         REALREGEX
//

string get_type(string str) {
    string type = "ID";
    if (str == "create") {
        type = "CREATE";
    } else if (str == "view") {
        type = "VIEW";
    } else if (str == "as") {
        type = "AS";
    } else if (str == "output") {
        type = "OUTPUT";
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
        type = "REG";
    } else if (is_number(str)) {
        type = "NUMBER";
    } else {
        type = "ID";
    }
    return type;
}

vector<Token> Lexer::get_tokens() {
    return tokens;
}

bool is_number_or_letter(char c) {
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return true;
    else return false;
}

bool is_number(string str) {
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}
