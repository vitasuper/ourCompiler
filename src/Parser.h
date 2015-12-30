#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "Lexer.h"
#include "View.h"
#include "Tokenizer.h"

struct Block {
    string type; // REG or ID ID or TOKEN
    int group_num; // If not grouped, group_num equals to -1
    string group_name;
    // REG
    string reg_value;
    
    // ID ID
    string view_name;
    string view_col_name;
    
    // TOKEN
    int min, max;
    
    vector<Span> view_col;
    
    Block(string _type) {
        type = _type;
        group_num = -1;
    }
};

struct Single_group {
    int group_num;
    Token group_id;
    Single_group(int _group_num, Token _group_id) : group_id(_group_id) {
        group_num = _group_num;
    }
};

class Parser {
  public:
    Parser(vector<Token> _lex_tokens, const char *_input_path, const char *_output_path);
    void start();
    void create_stmt();
    vector<View_col> view_stmt();
    const string alias();
    vector<View_col> select_stmt();
    vector< vector<Token> > select_list();

    vector<View_col> extract_stmt();
    string get_view_name_from_view_alias_name(string view_alias_name, vector<Token> from_token);
    vector< vector<Span> > pattern_matching_recursion(vector< vector<Span> > _group, unsigned& block_index, int _min, int _max, const vector<Block>& processed_pattern);
    vector<View_col> pattern_matching(const vector<Block>& processed_pattern, const vector<Token>& group_names);
    vector< vector<Token> > extract_spec();
    vector< vector<Token> > regex_spec();
    vector<Token> pattern_expr();
    vector<Token> pattern_pkg();
    vector<Token> atom();
    vector<Token> pattern_group();
    vector<Token> column();
    vector<Token> name_spec();
    vector<Token> group_spec();
    Single_group single_group();
    vector< vector<Token> > pattern_spec();
    vector<Token> from_list();
    vector<Token> from_item();

    void output_stmt();
    void output_view(const string& view_name, const string& alias_name);

    string find_token(string input,int start_pos, int end_pos);
    void add_view(View new_view);
    
    View get_view_by_view_name(string view_name);
    vector<View> get_views();
    

    void print_views();
  private:
    vector<Token> lex_tokens;
    int current_num;
    const char *input_path;
    const char *output_path;

    string input_document;
    Tokenizer tokenizer;

    vector<View> views;

};

bool view_col_cmp(View_col a, View_col b);

bool single_group_cmp(Single_group a, Single_group b);

int convert_string_to_num(string str);

#endif // PARSER_H_INCLUDED