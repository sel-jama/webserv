#include "includes/WebServer.hpp"
#include "includes/infra.hpp"

int main(int ac, char **av)
{
    try
    {
        validArg(ac, av);
        infra InfraStruct((configFile(av[1]).getConfigfile()));
        // InfraStruct.printInfra();
        InfraStruct.initservers();
    }
    catch(const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

}