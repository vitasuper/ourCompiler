//
//  Tokenizer.cpp
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Tokenizer.h"

Tokenizer::Tokenizer(string filePath) {
    // Open the file with the ReadOnly mode.
    std::ifstream file(filePath.c_str(), ios::in);

    char c;
    string word = "";
    string regex = "";
    bool regexFlag = false;
    while(!file.eof()) {
        file.get(c);
        if (regexFlag) {
            regex += c;
            if (c == '/') {
                tokens.push_back(regex);
                regex = "";
                regexFlag = false;
            }
        } else {
            // While meeting a blank, push the word into the tokens vector.
            if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
                if (word.size() > 0) {
                    tokens.push_back(word);
                    word = "";
                }
            } else {
                if (isNumberOrLetter(c)) {
                    if (word.size() > 0 && !isNumberOrLetter(word[0])) {
                        // Push the special character such as "," "'" into the tokens vector.
                        tokens.push_back(word);
                        word = "";
                        word += c;
                    } else {
                        // Continue spanning the word.
                        word += c;
                    }
                } else {
                    // If meet some special character, push the existed word into the tokens vector.
                    if (word.size() > 0) {
                        tokens.push_back(word);
                        word = "";
                    }
                    if (c != '/') {
                        word += c;
                    } else {
                        regex += c;
                        regexFlag = true;
                    }
                }
            }
        }
    }
    file.close();
}

vector<string> Tokenizer::getTokens() {
    return tokens;
}

bool isNumberOrLetter(char c) {
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return true;
    else return false;
}
