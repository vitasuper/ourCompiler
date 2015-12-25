//
//  main.cpp
//

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Tokenizer.h"
#include "Lexer.h"

using namespace std;

int main(int argc, const char * argv[]) {
    string filePath = "/Users/vitasuper/Desktop/PerLoc.aql";
    Lexer l = Lexer(filePath);
    vector<Token> v = l.getTokens();
    
    for (int i = 0; i < v.size(); ++i) {
        cout << "value: " << setw(15) << left << v[i].value;
        cout << left << "Type: " << setw(10)  << v[i].type;
        cout << endl;
    }
    
    return 0;
}
