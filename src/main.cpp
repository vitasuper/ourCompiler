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
    string codePath = "dataset/PerLoc.aql";
    string inputPath = "dataset/PerLoc.input";
    Lexer l = Lexer(codePath);
    vector<Token> v = l.getTokens();

    Parser p = Parser(v, inputPath.c_str(), "asd");
    p.start();
    p.print_views();

    return 0;
}
