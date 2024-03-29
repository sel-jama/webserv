#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "WebServer.hpp"


class configFile
{
    private:
        std::vector<std::string> tokens;
        configFile();
        configFile(const configFile &);
        const configFile &operator=(const configFile &);

    public:
        ~configFile();
        configFile(char *);

        //getter
        const std::vector<std::string> &getConfigfile()const;

        // debug - print
        void printTokens();
        void checkTokens();

};

#endif