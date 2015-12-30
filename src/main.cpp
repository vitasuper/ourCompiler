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
//#define TEST1 1
using namespace std;

vector<vector<int> > findall(const char *regex, const char *content);

int main(int argc, const char * argv[]) {

#ifdef TEST1
    char regex[] = "(hello)[a-z]+(what)";
    char content[] = "helloaa0awhat";
    vector< vector<int> > result;
    result = findall(regex, content);
    
    if (result.size() > 0) {
        for (int i = 0; i < result[0].size(); i += 2) {
            printf("(%d, %d)", result[0][i], result[0][i+1]);
        }
        printf("\n");
    } else {
        cout << "No match!" << endl;
    }

#else
//    Token t = Token("a", "b");
//    Token xx = t;
//    
//    cout << xx.value << " " << xx.type;
//    
    string code_path = "/Users/vitasuper/Desktop/PerLoc.aql";
    string input_path = "/Users/vitasuper/Desktop/PerLoc.input";
//    Tokenizer tn = Tokenizer(input_path);
//    for (int i = 0; i < tn.get_words().size(); ++i) {
//        cout << tn.get_words().at(i).value;
//        printf("(%d, %d)\n", tn.get_words().at(i).start_pos, tn.get_words().at(i).end_pos);
//    }
//    cout << tn.get_words_num_between(6, 57) << endl;
    
    Lexer l = Lexer(code_path);
    vector<Token> v = l.get_tokens();
    
    Parser p = Parser(v, input_path.c_str(), "asd");
    p.start();
    //p.print_views();
#endif
    return 0;
}
