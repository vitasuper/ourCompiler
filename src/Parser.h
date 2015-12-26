#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "Lexer.h"
#include "View.h"

class Parser {
  public:
    Parser(vector<Token> _lex_tokens, const char *_input_path, const char *_output_path);
    void start();
    void create_stmt();
    vector<View_col> view_stmt();
    vector<View_col> select_stmt();

    vector<View_col> extract_stmt();
    vector< vector<Token> > extract_spec();
    vector< vector<Token> > regex_spec();
    vector<Token> column();
    vector<Token> name_spec();
    vector<Token> group_spec();
    Token single_group();
    vector<Token> pattern_spec();
    vector<Token> from_list();
    vector<Token> from_item();

    bool output_stmt();

    string find_token(string input,int start_pos, int end_pos);
    void add_view(View new_view);

    void print_views();
  private:
    vector<Token> lex_tokens;
    int current_num;
    const char *input_path;
    const char *output_path;

    string input_document;

    vector<View> views;

};

#endif // PARSER_H_INCLUDED
