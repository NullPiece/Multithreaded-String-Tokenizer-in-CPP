// declaration file for tokenizer.cpp
// TOKENIZER_HPP

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include<vector>
#include<string>
#include<map>

using _type_token = std::vector<std::string>;

class tokenizer {

    public :
        _type_token tokens;    // vector of tokens

    public :

        /*
        * @param _content Typically a string of a corpus to be tokenized
        *
        */
        _type_token tokenize(std::string &_content);

        /*
        * @param _content Typically a string of a corpus to be tokenized
        * @param _num_threads Number of threads for tokenization
        */
        _type_token tokenize_multithreaded(std::string &_content, int _num_threads);
        
};

#endif // TOKENIZER_HPP