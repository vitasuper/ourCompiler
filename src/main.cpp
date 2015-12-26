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
#include "Parser.h"

using namespace std;

int main(int argc, const char * argv[]) {
    string code_path = "/Users/vitasuper/Desktop/PerLoc.aql";
    string input_path = "/Users/vitasuper/Desktop/PerLoc.input";
    Lexer l = Lexer(code_path);
    vector<Token> v = l.get_tokens();
    
    Parser p = Parser(v, input_path.c_str(), "asd");
    p.start();
    p.print_views();
    
    return 0;
}
