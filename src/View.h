#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Span {
    string token;
    int start_pos;
    int end_pos;
    Span(string _token, int start, int end) {
        token = _token;
        start_pos = start;
        end_pos = end;
    }
};

class View_col {
    public:
        View_col(string name);
        void add_span(Span new_span);
        void print_col();
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
    private:
        string view_name;
        vector<View_col> view_cols;
};

#endif // VIEW_H_INCLUDED
