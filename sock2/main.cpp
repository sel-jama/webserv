#include "includes/WebServer.hpp"
#include "includes/infra.hpp"

#include "../Response/Response.hpp"
// #include "../Request/Request.hpp"

int main(int ac, char **av)
{
    try
    {
        validArg(ac, av);
        infra InfraStruct((configFile(av[1]).getConfigfile()));
        // InfraStruct.printInfra();
        InfraStruct.initservers();
        // main2(InfraStruct);
    }
    catch(const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

}