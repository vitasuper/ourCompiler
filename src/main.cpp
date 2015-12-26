//
//  main.cpp
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Tokenizer.h"
#include "Lexer.h"

using namespace std;

int main(int argc, const char * argv[]) {
    // Test Lexer
//    string file_path = "/Users/vitasuper/Desktop/Revenue.aql";
//    Lexer l = Lexer(file_path);
//    vector<Token> v = l.get_tokens();
//    
//    for (int i = 0; i < v.size(); ++i) {
//        cout << "value: " << setw(15) << left << v[i].value;
//        cout << left << "Type: " << setw(10)  << v[i].type;
//        cout << endl;
//    }
    
    // Test Tokenizer
    string file_path = "/Users/vitasuper/Desktop/Revenue.input";
    Tokenizer tn = Tokenizer(file_path);
    vector<Word> v = tn.get_words();
    
    for (int i = 0; i < v.size(); ++i) {
        cout << "value: " << setw(15) << left << v[i].value;
        printf("(%d, %d)\n", v[i].start_pos, v[i].end_pos);
    }
    
    return 0;
}
