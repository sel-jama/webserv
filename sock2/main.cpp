#include "includes/WebServer.hpp"
#include "includes/infra.hpp"


int main(int ac, char **av)
{
	try
	{
		const char *patho;
		if (ac <= 2)
			patho = validArg(ac, av);
		else throw (std::runtime_error("give valid arguments please"));
		std::cout << patho << std::endl;
		infra InfraStruct((configFile(patho).getConfigfile()));
		InfraStruct.printInfra();
		InfraStruct.initservers();
	}
	catch(const std::exception &e)
	{
		std::cout <<"Main Exception: " << e.what() << std::endl;
	}

}
