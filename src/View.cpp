#include "View.h"
#include <iostream>

using namespace std;

const string span_to_string(const string& token, int start, int end) {
    /**
     The string form of a Span is following:
     "Token:(start_pos, end_pos)"
     **/
    string span_ = token + ":";
    char start_pos_[12];
    snprintf(start_pos_, sizeof(start_pos_), "%d", start);
    span_ += "(" + string(start_pos_) + ",";
    char end_pos_[12];
    snprintf(end_pos_, sizeof(end_pos_), "%d", end);
    span_ += string(end_pos_) + ")";
    return span_;
}

View::View(string name) {
    view_name = name;
}

void View::add_col(View_col new_col) {
    view_cols.push_back(new_col);
}

void View::print_view() {
    cout << view_name << endl;
    for (int i = 0; i < view_cols.size(); i++) {
        view_cols.at(i).print_col();
    }
}

View_col View::get_view_col_by_view_col_name(string view_col_name) {
    vector<View_col> all_view_cols = get_view_cols();
    int view_cols_num = all_view_cols.size();
    for (int i = 0; i < view_cols_num; i++) {
        if (all_view_cols.at(i).get_view_col_name() == view_col_name) {
            return all_view_cols.at(i);
            break;
        }
        
    }
    View_col empty_view_col = View_col("");
    return empty_view_col;
}

string View::get_view_name() {
    return view_name;
}

vector<View_col> View::get_view_cols() {
    return view_cols;
}

View_col::View_col(string name) {
    view_col_name = name;
}

void View_col::set_view_col_name(string new_view_col_name) {
    view_col_name = new_view_col_name;
}

void View_col::add_span(Span new_span) {
    spans.push_back(new_span);
}

void View_col::print_col() {
    cout << view_col_name << endl;
    for (int i = 0; i < spans.size(); i++) {
        cout << spans.at(i).token << ":(" << spans.at(i).start_pos << ", " << spans.at(i).end_pos << ")\n";
    }
}

string View_col::get_view_col_name() {
    return view_col_name;
}

vector<Span> View_col::get_spans() {
    return spans;
}


