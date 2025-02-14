#include "./tokenizer.hpp"
#include<iostream>
#include<map>
#include<thread>
#include<functional>
#include<mutex>


// defining a struct which is not local to the tokenizer class.
// This struct holds the parameters required for toknizing the string

struct _thread {

    int t_id;                    // holds the id for an individual thread
    int s_index;                 // starting index of the string to be tokenized
    int e_index;                 // ending index of the string to be tokenized
    _type_token t_tokens;        // holds an vector of tokens
    std::thread t;               // the actual thread which will tokenize the segment of the string
};


_type_token tokenizer::tokenize(std::string &_content) {

    int start = 0;
    int char_ptr = start;
    int end = _content.length() - 1;

    this->tokens.reserve(_content.length() + 1);

    std::map<char, int> _specials;
    _specials['.'] = 1;
    _specials[','] = 1;
    _specials['!'] = 1;

    for(char_ptr = 0; char_ptr <= end; ++char_ptr) {
        
        if(_content[char_ptr] == ' ' && char_ptr + 1 < end && _content[char_ptr + 1] != ' ') {
            this->tokens.emplace_back(_content.substr(start, char_ptr - start));
            start = char_ptr + 1;
        }
        else if(_specials[_content[char_ptr]] == 1 && _content[char_ptr + 1] == ' ') {
            this->tokens.emplace_back(_content.substr(start, char_ptr - start));
            start = char_ptr + 2;
            char_ptr = start;
        }
        else if(_specials[_content[char_ptr]] == 1 && _content[char_ptr + 1] != ' ') {
            this->tokens.emplace_back(_content.substr(start, char_ptr - start));
            start = char_ptr + 1;
            char_ptr = start;
        }
    }

    if(_content[char_ptr] != ' ')
        this->tokens.emplace_back(_content.substr(start, char_ptr - start + 1));

    return this->tokens;
}

/*
 * @param thread Thread for tokenizing string or a corpus
 * @param _content Actual string or a corpus to be tokenized
 */
void thread_tokenizer(_thread &thread, const std::string &_content) {

    int new_end = thread.e_index;
    int new_start = thread.s_index;


    int local_char_ptr = thread.s_index;
    new_start = thread.s_index;

    for(local_char_ptr = thread.s_index; local_char_ptr < thread.e_index; ++local_char_ptr) {
        
        if(_content[local_char_ptr] == ' ') {
            thread.t_tokens.emplace_back(_content.substr(new_start, local_char_ptr - new_start));
            new_start = local_char_ptr + 1;
        }
    }

    thread.t_tokens.emplace_back(_content.substr(new_start, local_char_ptr - new_start));
}


_type_token tokenizer::tokenize_multithreaded(std::string &_content, int _num_threads) {

    int _t_count = _num_threads;
    int end = _content.length();
    int segment_len = _content.length() / _t_count;

    std::vector<_thread> threads;
    threads.reserve(_t_count);

    for(int i = 0; i < _t_count; ++i) {

        threads.push_back(_thread());
        threads[i].t_id = i + 1;
        threads[i].s_index = (_content.length() / _t_count) * i;        
        threads[i].e_index = threads[i].s_index + segment_len;
    }

    for(int i = 0; i < _t_count; ++i) {


        if(i == 0) {

            while(_content[threads[0].e_index] != ' ')
                ++threads[0].e_index;
        
            threads[1].s_index =  threads[0].e_index + 1;
        } else if(threads[i].t_id > 1 && threads[i].t_id < _t_count) {
        
            while(_content[threads[i].e_index] != ' ')
                ++threads[i].e_index;
            
            threads[i + 1].s_index = threads[i].e_index + 1;
        } else if(i == _t_count - 1) {
            threads[i].e_index = end;
            threads[i].s_index = threads[i - 1].e_index + 1;
        }

        threads[i].t = std::thread(thread_tokenizer, std::ref(threads[i]), std::ref(_content));
    }


    for(int i = 0; i < _t_count; ++i) {

        threads[i].t.join();
    }
    
    for(int i = 0; i < _t_count; ++i) {

        this->tokens.insert(this->tokens.end(), threads[i].t_tokens.begin(), threads[i].t_tokens.end());
    }

    return this->tokens;
}