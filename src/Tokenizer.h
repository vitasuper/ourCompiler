//
//  Tokenizer.h
//

#ifndef Tokenizer_h
#define Tokenizer_h

#include <vector>
#include <string>

using namespace std;

struct Word {
    string value;
    int start_pos;
    int end_pos;
    Word(string _value, int _start_pos, int _end_pos) {
        value = _value;
        start_pos = _start_pos;
        end_pos = _end_pos;
    }
};

class Tokenizer {
  public:
    Tokenizer(string file_path);
    vector<Word> get_words();
  
  private:
    vector<Word> words;
};

bool is_number_or_letter(char c);

#endif /* Tokenizer_h */
