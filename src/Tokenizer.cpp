//
//  Tokenizer.cpp
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Tokenizer.h"

Tokenizer::Tokenizer(string file_path) {
    // Open the file with the ReadOnly mode.
    std::ifstream file(file_path.c_str(), ios::in);
    
    char c;
    string word = "";
    int cnt = 0;
    int start_pos = 0;
    while(!file.eof()) {
        file.get(c);
        // While meeting a blank, push the word into the words vector.
        if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
            if (word.size() > 0) {
                words.push_back(Word(word, start_pos, cnt));
                word = "";
                start_pos = cnt + 1;
            }
        } else {
            if (is_number_or_letter(c)) {
                if (word.size() > 0 && !is_number_or_letter(word[0])) {
                    // Push the special character such as "," "'" into the words vector.
                    words.push_back(Word(word, start_pos, cnt));
                    word = "";
                    word += c;
                    start_pos = cnt;
                } else {
                    // Continue spanning the word.
                    word += c;
                }
            } else {
                // If meet some special character, push the existed word into the words vector.
                if (word.size() > 0) {
                    words.push_back(Word(word, start_pos, cnt));
                    word = "";
                    start_pos = cnt;
                }
                word += c;
            }
        }
        ++cnt;
    }
    file.close();
}

vector<Word> Tokenizer::get_words() {
    return words;
}
