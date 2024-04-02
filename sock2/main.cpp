#include "includes/WebServer.hpp"
#include "includes/infra.hpp"
#include "../Response/Response.hpp"
#include "../post/post.hpp"
// #include "../Request/Request.hpp"

int main(int ac, char **av)
{
    try
    {
        validArg(ac, av);
        infra InfraStruct((configFile(av[1]).getConfigfile()));
        // InfraStruct.printInfra();
        // InfraStruct.initservers();
        Request req;
        Post pos;

        main2(InfraStruct, req);
    }
    catch(const std::exception &e)
    {
        std::cout <<"Main Exception: " << e.what() << std::endl;
    }

}
