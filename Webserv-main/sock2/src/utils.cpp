#include "../includes/utils.hpp"

int iskey(const std::string &key)
{
    std::string keys[] = 
    {
        "listen",
        "server_name",
        "location",
        "client_max_body_size",
        "error_page"
    };
    size_t numKeys = sizeof(keys) / sizeof(keys[0]);
    for (size_t i = 0; i < numKeys; ++i)
    {if (key == keys[i]) return 1;} return 0;
}

int iskey2(const std::string & key)
{
    std::string keys[]=
    {
        "upload",
        "autoindex",
        "http_methods",
        "root",
        "cgi",
        "return",
    };
    size_t numKeys = sizeof(keys)/sizeof(keys[0]);
    for(size_t i = 0; i < numKeys; ++i)
    {if(key == keys[i]) return 1;} return 0;
}

int containalpha(const std::string &word)
{
    for (size_t i = 0; i < word.size(); ++i)
    {
        if (std::isalpha(word[i])) return 1;
    }
    return 0;
}

int alldigit(const std::string &number)
{
    for (size_t i = 0; i < number.size(); ++i)
    {
        if (!std::isdigit(number[i])) return 0; 
    }
    return 1;
}