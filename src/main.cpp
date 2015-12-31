//
//  main.cpp
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <dirent.h>
#include "Tokenizer.h"
#include "Lexer.h"
#include "Parser.h"
using namespace std;

int main(int argc, const char * argv[]) {
    if (argc != 4) {
        // If the number of the parameter is not correct, stop the program and print the following sentence.
        cout << "The number of parameter is not correct! Please check out and try again!" << endl;
        return 0;
    }
    
    string code_path = argv[1];
    string input_path = argv[2];
    string output_path = argv[3];
    
    if (input_path.substr(input_path.size() - 6, 6) == ".input") {
        // If input_path is a *.input file
        Lexer l = Lexer(code_path);
        vector<Token> v = l.get_tokens();
        Parser p = Parser(v, input_path.c_str(), output_path.c_str());
        p.start();
        return 0;
    } else {
        // If input_path is a directory
        DIR *dp;
        struct dirent *dirp;
        if ((dp = opendir(input_path.c_str())) == NULL) {
            cout << "Can't open " << input_path << endl;
            return 0;
        }
        
        while ((dirp = readdir(dp)) != NULL) {
            char file_name[200], suffix[200];
            
            char input_suffix[] = ".input";
            char output_suffix[] = ".output";
            
            if (strlen(dirp->d_name) >= 6 && strcmp(strcpy(suffix, (dirp->d_name + strlen(dirp->d_name) - 6)), input_suffix) == 0) {
                // Get file name.  e.g. PerLoc
                int i = 0;
                for (; i < strlen(dirp->d_name); ++i) {
                    if (dirp->d_name[i] != '.') {
                        file_name[i] = dirp->d_name[i];
                    } else {
                        break;
                    }
                }
                file_name[i] = '\0';
                
                Lexer l = Lexer(code_path);
                vector<Token> v = l.get_tokens();
                
                // Get total_input_path.  e.g. ../dataset/PerLocInput/
                char total_input_path[200];
                int j = 0;
                for (; j < strlen(input_path.c_str()); ++j) {
                    total_input_path[j] = input_path.c_str()[j];
                }
                total_input_path[j] = '\0';
                
                // Get total_output_path.  e.g. ../dataset/PerLocOutput/
                char total_output_path[200];
                j = 0;
                for (; j < strlen(output_path.c_str()); ++j) {
                    total_output_path[j] = output_path.c_str()[j];
                }
                total_output_path[j] = '\0';
                
                Parser p = Parser(v, strcat(strcat(total_input_path, file_name), input_suffix), strcat(strcat(total_output_path, file_name), output_suffix));
                p.start();
            }
        }
        return 0;
    }
    return 0;
}
