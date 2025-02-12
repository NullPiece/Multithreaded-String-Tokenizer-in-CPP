// TOKENIZER_HPP
// declaration file for tokenizer.cpp

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include<vector>
#include<string>
#include<map>

using _type_token = std::vector<std::string>;

class tokenizer {

    public :
        _type_token tokens;

    public :
        _type_token tokenize(std::string _content);
        _type_token tokenize_multithreaded(std::string _content);
        
};

#endif // TOKENIZER_HPP