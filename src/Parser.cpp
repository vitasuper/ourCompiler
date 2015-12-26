#include "Parser.h"
#include <fstream>
#include <iostream>

using namespace std;

vector<vector<int> > findall(const char *regex, const char *content);

Parser::Parser(vector<Token> _lex_tokens, const char *_input_path, const char *_output_path) {
    lex_tokens = _lex_tokens;
    input_path = _input_path;
    output_path = _output_path;
    current_num = 0;

    std::ifstream file(_input_path, ios::in);
    input_document = string();
    char c;
    while(!file.eof()) {
        file.get(c);
        input_document += c;
    }
    file.close();
}

void Parser::start() {
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
    } else {
        false;
    }
}

vector<View_col> Parser::select_stmt() {

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

    }

    return extract_col;
}

vector< vector<Token> >  Parser::extract_spec() {
    if (lex_tokens.at(current_num).type == "REGEX") {
        current_num++;
        return regex_spec();
    } else if (lex_tokens.at(current_num).type == "PATTERN") {
        current_num++;
        pattern_spec();
        // Un do...
    }
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


vector<Token> Parser::pattern_spec() {

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

bool Parser::output_stmt() {

}


void Parser::add_view(View new_view) {
    views.push_back(new_view);
}

void Parser::print_views() {
    for (int i = 0; i < views.size(); i++) {
        views.at(i).print_view();
    }
}
