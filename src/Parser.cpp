#include "Parser.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <assert.h>

using namespace std;

vector<vector<int> > findall(const char *regex, const char *content);

Parser::Parser(vector<Token> _lex_tokens, const char *_input_path, const char *_output_path) {
    lex_tokens = _lex_tokens;
    input_path = _input_path;
    output_path = _output_path;
    current_num = 0;

    std::ifstream file(_input_path, ios::in);
    istreambuf_iterator<char> begin(file), end;
    input_document = string(begin, end);
    file.close();
}

void Parser::start() {
    // start processing
    cout << "Processing " << input_path << endl;
    
    while (current_num < lex_tokens.size()) {
        if (lex_tokens.at(current_num).type == "CREATE") {
            current_num++;
            create_stmt();

            if (lex_tokens.at(current_num).type == "SEMICOLON")
                current_num++;
        }
        else if (lex_tokens.at(current_num).type == "OUTPUT") {
            current_num++;
            output_stmt();

            if (lex_tokens.at(current_num).type == "SEMICOLON")
                current_num++;
        }
    }
}

void Parser::create_stmt() {
    if (lex_tokens.at(current_num).type == "VIEW") {
        current_num++;
    }

    View new_view;
    if (lex_tokens.at(current_num).type == "ID") {
        /** Create a new view **/
        new_view = View(lex_tokens.at(current_num).value);
        current_num++;
    }

    if (lex_tokens.at(current_num).type == "AS") {
        current_num++;
    }

    vector<View_col> view_cols = view_stmt();
    for (int i = 0; i < view_cols.size(); i++) {
        new_view.add_col(view_cols.at(i));
    }

    this->add_view(new_view);
}

vector<View_col> Parser::view_stmt() {
    if (lex_tokens.at(current_num).type == "SELECT") {
        current_num++;
        return select_stmt();
    }
    else if (lex_tokens.at(current_num).type == "EXTRACT") {
        current_num++;
        return extract_stmt();
    }
    vector<View_col> v;
    return v;
}

/** obtain alias name **/
const string Parser::alias() {
    string alias_name = "";
    if (lex_tokens.at(current_num).type == "AS") {
        current_num++;
        
        if (lex_tokens.at(current_num).type == "ID") {
            /** obtain alias id **/
            alias_name = lex_tokens.at(current_num).value;
            current_num++;
        }
    }
    else {
        /** empty alias **/
    }
    return alias_name;
}

vector<View_col> Parser::select_stmt() {
    vector< vector<Token> > select_token_list = select_list();
    if (lex_tokens.at(current_num).type == "FROM")
        current_num++;
    
    vector<Token> from_token = from_list();
    
    vector<View_col> select_col;
    
    vector<View> all_views = get_views();
    int view_cols_num = select_token_list.at(0).size() / 3;
    
    View selected_view = get_view_by_view_name(from_token.at(0).value);
    
    for (int i = 0; i < view_cols_num; i++) {
        View_col selected_view_col = selected_view.get_view_col_by_view_col_name(select_token_list.at(0).at(i*3 + 1).value);
        selected_view_col.set_view_col_name(select_token_list.at(0).at(i*3 + 2).value);
        select_col.push_back(selected_view_col);
    }
    
    sort(select_col.begin(), select_col.end(), select_cmp);
    return select_col;
}

vector<View_col> Parser::extract_stmt() {
    vector< vector<Token> > extract_token_list = extract_spec();

    if (lex_tokens.at(current_num).type == "FROM")
        current_num++;

    vector<Token> from_token = from_list();

    vector<View_col> extract_col;

    // if 3, it comes from regex_spec
    if (extract_token_list.size() == 3) {
        string REG = extract_token_list.at(0).at(0).value;

        string wanted_view_alias = extract_token_list.at(1).at(0).value;
        string wanted_view_col_name = extract_token_list.at(1).at(1).value;

        string new_view_col_name = extract_token_list.at(2).at(0).value;

        string from_view_name = from_token.at(0).value;
        string from_view_alias = from_token.at(1).value;

        View_col new_view_col = View_col(new_view_col_name);

        if (from_view_alias == wanted_view_alias) {
            if (wanted_view_col_name == "text" && from_view_name == "Document") {
                vector< vector<int> > output_tokens = findall(REG.c_str(), input_document.c_str());

                for (int i = 0; i < output_tokens.size(); i++) {
                    int start_pos = output_tokens.at(i).at(0);
                    int end_pos = output_tokens.at(i).at(1);
                    string temp_token = input_document.substr(start_pos, end_pos - start_pos);
                    Span temp_span = Span(temp_token, start_pos, end_pos);

                    new_view_col.add_span(temp_span);
                }
            }
        }

        extract_col.push_back(new_view_col);

    } else {  // else comes from pattern_spec
        // For debugging...
//        for (int i = 0; i < extract_token_list.at(0).size(); ++i) {
//            cout << extract_token_list.at(0).at(i).value;
//        }
        
        vector<Token> pattern_expression = extract_token_list.at(0);
        vector<Token> group_names = extract_token_list.at(1);
        int i = 0;
        int group_num = 0;
        vector<Block> possessed_pattern;
        
        while (i < pattern_expression.size()) {
            if (pattern_expression.at(i).type == "LEFTBRACKET") {
                ++group_num;
                
                ++i;
                if (pattern_expression.at(i).type == "ID") {
                    string view_alias_name = pattern_expression.at(i).value;
                    
                    ++i;
                    string view_col_name = pattern_expression.at(i).value;
                    
                    Block current_block = Block("ID");
                    current_block.view_name = get_view_name_from_view_alias_name(view_alias_name, from_token);
                    current_block.view_col_name = view_col_name;
                    current_block.group_num = group_num;
                    
                    possessed_pattern.push_back(current_block);
                    
                    
                } else if (pattern_expression.at(i).type == "REG") {
                    Block current_block = Block("REG");
                    current_block.reg_value = pattern_expression.at(i).value;
                    current_block.group_num = group_num;
                    
                    possessed_pattern.push_back(current_block);
                }
                
                i += 3; // Pass EMPTY ) EMPTY
                
            } else if (pattern_expression.at(i).type == "ID") {
                string view_alias_name = pattern_expression.at(i).value;
                
                ++i;
                string view_col_name = pattern_expression.at(i).value;
                
                Block current_block = Block("ID");
                current_block.view_name = get_view_name_from_view_alias_name(view_alias_name, from_token);
                current_block.view_col_name = view_col_name;
                
                possessed_pattern.push_back(current_block);
                
                ++i; // Pass EMPTY
            } else if (pattern_expression.at(i).type == "REG") {
                Block current_block = Block("REG");
                current_block.reg_value = pattern_expression.at(i).value;
                
                possessed_pattern.push_back(current_block);
                
                ++i; // Pass EMPTY
            } else if (pattern_expression.at(i).type == "TOKEN") {
                ++i;
                int min = convert_string_to_num(pattern_expression.at(i).value);
                
                ++i;
                int max = convert_string_to_num(pattern_expression.at(i).value);
                
                Block current_block = Block("TOKEN");
                current_block.min = min;
                current_block.max = max;
                
                possessed_pattern.push_back(current_block);
                
                ++i; // Pass EMPTY
            }
            ++i;
        }
        
        for (int i = 0; i < possessed_pattern.size(); ++i) {
            if (possessed_pattern.at(i).group_num != -1) {
                int num = possessed_pattern.at(i).group_num;
                possessed_pattern.at(i).group_name = group_names.at(num).value;
            }
        }
        
//        for (int i = 0; i < possessed_pattern.size(); ++i) {
//            if (possessed_pattern.at(i).group_num != -1) {
//                cout << possessed_pattern.at(i).group_name << endl;
//            }
//        }
        
    }

    return extract_col;
}

string Parser::get_view_name_from_view_alias_name(string view_alias_name, vector<Token> from_token) {
    // Get real view_name instead of alias name from from_token.
    string view_name = "";
    for (unsigned i = 0; i < from_token.size(); ++i) {
        if (from_token.at(i).value == view_alias_name) {
            // Find view_alias_name in from_token
            view_name = from_token.at(i-1).value;
            break;
        }
    }
    return view_name;
}

vector< vector<Token> > Parser::extract_spec() {
    if (lex_tokens.at(current_num).type == "REGEX") {
        current_num++;
        return regex_spec();
    } else if (lex_tokens.at(current_num).type == "PATTERN") {
        current_num++;
        return pattern_spec();
    }
    vector< vector<Token> > v;
    return v;
}

vector< vector<Token> >  Parser::select_list() {
    vector< vector<Token> > token_list;
    vector<Token> select_list_token;
    
    Token default_alias = Token("", "");
    if (lex_tokens.at(current_num).type == "ID") {
        select_list_token.push_back(lex_tokens.at(current_num));
        current_num++;
    }
    
    if (lex_tokens.at(current_num).type == "DOT")
        current_num++;
    
    if (lex_tokens.at(current_num).type == "ID") {
        select_list_token.push_back(lex_tokens.at(current_num));
        
        default_alias.type = lex_tokens.at(current_num).type;
        default_alias.value = lex_tokens.at(current_num).value;
        
        current_num++;
    }
    
    if (lex_tokens.at(current_num).type == "AS") {
        current_num++;
        if (lex_tokens.at(current_num).type == "ID") {
            select_list_token.push_back(lex_tokens.at(current_num));
            current_num++;
        }
    } else {
        select_list_token.push_back(default_alias);
    }
    
    while (lex_tokens.at(current_num).type == "COMMA") {
        current_num++;
        
        if (lex_tokens.at(current_num).type == "ID") {
            select_list_token.push_back(lex_tokens.at(current_num));
            current_num++;
        }
        
        if (lex_tokens.at(current_num).type == "DOT")
            current_num++;
        
        if (lex_tokens.at(current_num).type == "ID") {
            select_list_token.push_back(lex_tokens.at(current_num));
            
            default_alias.type = lex_tokens.at(current_num).type;
            default_alias.value = lex_tokens.at(current_num).value;
            
            current_num++;
        }
        
        if (lex_tokens.at(current_num).type == "AS") {
            current_num++;
            if (lex_tokens.at(current_num).type == "ID") {
                select_list_token.push_back(lex_tokens.at(current_num));
                current_num++;
            }
        } else {
            select_list_token.push_back(default_alias);
        }
    }
    
    token_list.push_back(select_list_token);
    
    return token_list;
}

vector< vector<Token> > Parser::regex_spec() {
    vector< vector<Token> > token_list;

    vector<Token> regex_token;
    if (lex_tokens.at(current_num).type == "REG") {
        regex_token.push_back(lex_tokens.at(current_num));
        current_num++;
    }
    token_list.push_back(regex_token);

    if (lex_tokens.at(current_num).type == "ON")
        current_num++;

    token_list.push_back(column());

    token_list.push_back(name_spec());

    return token_list;
}


vector< vector<Token> > Parser::pattern_spec() {
    vector< vector<Token> > token_list;

    token_list.push_back(pattern_expr());
    
    
    token_list.push_back(name_spec());
    
    return token_list;
}

vector<Token> Parser::pattern_expr() {
    vector<Token> pattern_expr_tokens;
    pattern_expr_tokens = pattern_pkg();
    pattern_expr_tokens.push_back(Token(" ", "EMPTY"));
    
    while (lex_tokens.at(current_num).type == "LEFTPOINTBRACKET" || lex_tokens.at(current_num).type == "LEFTBRACKET" || lex_tokens.at(current_num).type == "REG") {
        vector<Token> temp_tokens;
        temp_tokens = pattern_pkg();
        for (int i = 0; i < temp_tokens.size(); ++i) {
            pattern_expr_tokens.push_back(temp_tokens.at(i));
        }
        pattern_expr_tokens.push_back(Token(" ", "EMPTY"));
    }
    return pattern_expr_tokens;
}

vector<Token> Parser::pattern_pkg() {
    vector<Token> pattern_pkg_tokens;
    if (lex_tokens.at(current_num).type == "LEFTPOINTBRACKET") {
        //pattern_pkg_tokens.push_back(lex_tokens.at(current_num));
        current_num++;
        vector<Token> temp_pattern_pkg_tokens = atom();
        
        for (int i = 0; i < temp_pattern_pkg_tokens.size(); ++i) {
            pattern_pkg_tokens.push_back(temp_pattern_pkg_tokens.at(i));
        }
        
        if (lex_tokens.at(current_num).type == "RIGHTPOINTBRACKET") {
            //pattern_pkg_tokens.push_back(lex_tokens.at(current_num));
            current_num++;
        }
        
        if (lex_tokens.at(current_num).type == "LEFTBRACE") {
            //pattern_pkg_tokens.push_back(lex_tokens.at(current_num));
            current_num++;
        }
        if (lex_tokens.at(current_num).type == "NUMBER") {
            pattern_pkg_tokens.push_back(lex_tokens.at(current_num));
            current_num++;
        }
        if (lex_tokens.at(current_num).type == "COMMA") {
            current_num++;
        }
        if (lex_tokens.at(current_num).type == "NUMBER") {
            pattern_pkg_tokens.push_back(lex_tokens.at(current_num));
            current_num++;
        }
        if (lex_tokens.at(current_num).type == "RIGHTBRACE") {
            //pattern_pkg_tokens.push_back(lex_tokens.at(current_num));
            current_num++;
        }
    } else if (lex_tokens.at(current_num).type == "REG") {
        pattern_pkg_tokens.push_back(lex_tokens.at(current_num));
        current_num++;
    } else if (lex_tokens.at(current_num).type == "LEFTBRACKET") {
        pattern_pkg_tokens.push_back(lex_tokens.at(current_num));
        current_num++;
        
        vector<Token> temp_pattern_pkg_tokens = pattern_group();
        for (int i = 0; i < temp_pattern_pkg_tokens.size(); ++i) {
            pattern_pkg_tokens.push_back(temp_pattern_pkg_tokens.at(i));
        }
        
        if (lex_tokens.at(current_num).type == "RIGHTBRACKET") {
            pattern_pkg_tokens.push_back(lex_tokens.at(current_num));
            current_num++;
        }
    }
    return pattern_pkg_tokens;
}

        
vector<Token> Parser::atom() {
    vector<Token> atom_tokens;
    if (lex_tokens.at(current_num).type == "TOKEN") {
        atom_tokens.push_back(lex_tokens.at(current_num));
        current_num++;
    } else {
        atom_tokens = column();
    }
    return atom_tokens;
}

vector<Token> Parser::pattern_group() {
    return pattern_expr();
}

vector<Token> Parser::column() {
    vector<Token> column_token;
    if (lex_tokens.at(current_num).type == "ID") {
        column_token.push_back(lex_tokens.at(current_num));
        current_num++;
    }

    if (lex_tokens.at(current_num).type == "DOT")
        current_num++;

    if (lex_tokens.at(current_num).type == "ID") {
        column_token.push_back(lex_tokens.at(current_num));
        current_num++;
    }

    return column_token;
}



vector<Token> Parser::name_spec() {
    vector<Token> name_token;
    if (lex_tokens.at(current_num).type == "AS") {
        current_num++;
        if (lex_tokens.at(current_num).type == "ID") {
            name_token.push_back(lex_tokens.at(current_num));
            current_num++;
            return name_token;
        }
    } else if (lex_tokens.at(current_num).type == "RETURN") {
        current_num++;
        name_token.push_back(single_group());
        while (lex_tokens.at(current_num).type == "AND") {
            current_num++;
            name_token.push_back(single_group());
        }
    }
    
    return name_token;
}

Token Parser::single_group() {
    if (lex_tokens.at(current_num).type == "GROUP")
        current_num++;

    if (lex_tokens.at(current_num).type == "NUMBER")
        current_num++;

    if (lex_tokens.at(current_num).type == "AS")
        current_num++;

    if (lex_tokens.at(current_num).type == "ID") {
        int temp_num = current_num;
        current_num++;
        return lex_tokens.at(temp_num);
    }

    Token t = Token("", "");
    return t;
}

vector<Token> Parser::from_list() {
    vector<Token> from_token;
    if (lex_tokens.at(current_num).type == "ID") {
        from_token.push_back(lex_tokens.at(current_num));
        current_num++;
    }

    if (lex_tokens.at(current_num).type == "ID") {
        from_token.push_back(lex_tokens.at(current_num));
        current_num++;
    }

    while (lex_tokens.at(current_num).type == "COMMA") {
        current_num++;

        if (lex_tokens.at(current_num).type == "ID") {
            from_token.push_back(lex_tokens.at(current_num));
            current_num++;
        }

        if (lex_tokens.at(current_num).type == "ID") {
            from_token.push_back(lex_tokens.at(current_num));
            current_num++;
        }
    }

    return from_token;
}

void Parser::output_stmt() {
    if (lex_tokens.at(current_num).type == "VIEW") {
        current_num++;
    }
    
    string view_name;
    if (lex_tokens.at(current_num).type == "ID") {
        /** obtain view's id **/
        view_name = lex_tokens.at(current_num).value;
        current_num++;
    }
    
    /** capture alias **/
    string alias_name = alias();
    
    /** output the specified view **/
    output_view(view_name, alias_name);
}

/** Output the view according to the view's name and alias name **/
void Parser::output_view(const string& view_name, const string& alias_name) {
    View specified_view = get_view_by_view_name(view_name);
    if (specified_view.get_view_name().compare("") == 0) {
        /** Such a view does not exist **/
        /* some information */
        cout << "The View named " << view_name << " has not been created" << endl << endl;
        return;
    }
    /** output format **/
    /**
     View: view_name/alias_name
     +--------------------+
     | view_col_name      |
     +--------------------+
     | view_col_elements  |
     |        :           |
     |        :           |
     +--------------------+
     n rows in set
     **/
    cout << "View: " << (alias_name.compare("") == 0 ? view_name : alias_name) << endl;
    // current view
    vector<View_col> view_cols = specified_view.get_view_cols();
    // current view size
    unsigned view_size = view_cols.size();
    // longest span's length array
    unsigned longest_span_length[view_size];
    for (unsigned i = 0; i < view_size; i++) longest_span_length[i] = 0;
    // biggest view_col size
    unsigned biggest_view_col_size = 0;
    /* find the longest span's length for each view_col and the biggest view_col size*/
    for (unsigned i = 0; i < view_size; i++) {
        vector<Span> current_view_col = view_cols.at(i).get_spans();
        // find the longest span's length for each view_col
        for (unsigned j = 0; j < current_view_col.size(); j++) {
            unsigned span_length = current_view_col.at(j).as_string.length();
            if (span_length > longest_span_length[i]) {
                longest_span_length[i] = span_length;
            }
        }
        if (view_cols.at(i).get_view_col_name().length() > longest_span_length[i]) {
            // check whether current view_col's name is longest
            longest_span_length[i] = view_cols.at(i).get_view_col_name().length();
        }
        
        // find the biggest view_col size
        if (current_view_col.size() > biggest_view_col_size) {
            biggest_view_col_size = current_view_col.size();
        }
    }
    
    // index for spans in view_cols
    unsigned span_index = 0;
    /* print the view in proper format */
    for (unsigned row = 0; row < biggest_view_col_size + 4; row++) {
        for (unsigned col = 0; col < view_size; col++) {
            
            if (row == 0 || row == 2 || row == biggest_view_col_size + 3) {
                /**
                 Format 1 (Appear in the first, third and last lines)
                 The format is like "+--------+--------+------+"
                 **/
                cout << setw(longest_span_length[col] + 3) << setfill('-') << left << "+";
            }
            
            else {
                // get current view_col
                View_col view_col = view_cols.at(col);
                if (row == 1) {
                    /**
                     Format 2 (Appear only in the second line)
                     The format is like "| view_col_name1       | view_col_name2         | view_col_name3     |"
                     **/
                    cout << setw(longest_span_length[col] + 3) << setfill(' ') << left << "| " + view_col.get_view_col_name();
                }
                else {
                    // get spans in current view_col
                    vector<Span> current_spans = view_col.get_spans();
                    /**
                     Format 3 (Appear in the rest lines)
                     The format is like "| span1           | span2         | span3       |"
                     **/
                    cout << setw(longest_span_length[col] + 3) << setfill(' ') << left << "| " + (span_index < current_spans.size() ? current_spans.at(span_index).as_string : "");
                }
            }
            
        }
        // process the line end
        if (row == 0 || row == 2 || row == biggest_view_col_size + 3) {
            cout << "+" << endl;
        }
        else {
            cout << "|" << endl;
            if (row != 1) {
                // add 1 to span index
                span_index++;
            }
        }
    }
    // for debug
    assert(span_index == biggest_view_col_size);
    
    cout << biggest_view_col_size << (biggest_view_col_size > 1 ? " rows" : " row") << " in set" << endl << endl;
}

void Parser::add_view(View new_view) {
    views.push_back(new_view);
}

View Parser::get_view_by_view_name(string view_name) {
    vector<View> all_views = get_views();
    int views_num = all_views.size();
    for (int i = 0; i < views_num; i++) {
        if (all_views.at(i).get_view_name() == view_name) {
            return all_views.at(i);
            break;
        }
    }
    View empty_view = View("");
    return empty_view;
}

vector<View> Parser::get_views() {
    return views;
}

void Parser::print_views() {
    for (int i = 0; i < views.size(); i++) {
        views.at(i).print_view();
    }
}

bool select_cmp(View_col a, View_col b) {
    return a.get_view_col_name() < b.get_view_col_name();
}

int convert_string_to_num(string str) {
    int result = 0;
    for (int i = 0; i < str.size(); ++i) {
        result = result * 10 + (str[i] - '0');
    }
    return result;
}

