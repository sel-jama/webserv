#include "../includes/config.hpp"

configFile::~configFile(){}

configFile::configFile(const char *str)
{
    std::ifstream name(str);
    std::string line, token;
    size_t pos, comment_pos;
    while(getline(name, line))
    {
        comment_pos = line.find('#');
        if (comment_pos != std::string::npos) line = line.substr(0, comment_pos);
        std::istringstream iss(line);
        while (iss >> token)
        {
            while ((pos = token.find_first_of("{};")) != std::string::npos)
            {
                if (pos > 0)
                    tokens.push_back(token.substr(0, pos));
                tokens.push_back(token.substr(pos, 1));
                token = token.substr(pos + 1);
            }
            if (!token.empty())
                tokens.push_back(token);
        }
    }
}

const std::vector<std::string> & configFile::getConfigfile()const
{
    return (tokens);
}

void configFile::checkTokens()
{
    if (getConfigfile().empty())throw(std::runtime_error("Error: Empty config file !!"));
}

void configFile::printTokens()
{
    checkTokens();
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
    { std::cout << *it << std::endl;}
}