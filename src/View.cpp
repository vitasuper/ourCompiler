#include "View.h"
#include <iostream>

using namespace std;

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
View_col::View_col(string name) {
    view_col_name = name;
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
