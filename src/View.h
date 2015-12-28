#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

const string span_to_string(const string& token, int start, int end);

struct Span {
    string token;
    int start_pos;
    int end_pos;
    // string form of the span
    string as_string;
    Span(string _token, int start, int end) {
        token = _token;
        start_pos = start;
        end_pos = end;
        as_string = span_to_string(token, start_pos, end_pos);
    }
};

class View_col {
public:
    View_col(string name);
    void add_span(Span new_span);
    void print_col();
    void set_view_col_name(string new_view_col_name);
    string get_view_col_name();
    vector<Span> get_spans();
private:
    string view_col_name;
    vector<Span> spans;
};

class View {
public:
    View() {};
    View(string name);
    void add_col(View_col new_col);
    void print_view();
    View_col get_view_col_by_view_col_name(string view_col_name);
    string get_view_name();
    vector<View_col> get_view_cols();
private:
    string view_name;
    vector<View_col> view_cols;
};

#endif // VIEW_H_INCLUDED
