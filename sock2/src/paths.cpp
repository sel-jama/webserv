#include "../includes/WebServer.hpp"

std::string ltrim(const std::string& str)
{
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "": str.substr(start);
}

std::string rtrim(const std::string& str)
{
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (end == std::string::npos) ? "": str.substr(0, end + 1);
}

std::string trim(const std::string& str)
{
    return (ltrim(rtrim(str)));
}

void validPath(char *str)
{
    std::string name = trim(str);
    std::ifstream file(name);
    if (!file.is_open())throw(std::runtime_error("Error: can't open/find your file !!"));
}

void validNaming(char *str)
{
    size_t len = strlen(str);
    if (len < 6) throw(std::runtime_error("Error: wrong config file name !!"));
    if (str[len - 1] != 'f' || str[len - 2] != 'n' || str[len - 3] != 'o' || str[len - 4] != 'c'
     || str[len - 5] != '.') throw(std::runtime_error("Error: wrong config file name !!"));
}

void validNum(int ac, int num)
{
    if (ac != num) throw(std::runtime_error("wrong number of arguments !!"));
}

const char *validArg(int ac, char **av)
{
    const char *default_conf = "sock2/configfiles/config2.conf";
    if (ac == 1)
        return (default_conf);
    validNum(ac, 2);
    validNaming(av[1]);
    validPath(av[1]);
    return (av[1]);
}