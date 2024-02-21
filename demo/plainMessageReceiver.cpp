#include <tntpub/client.h>
#include <cxxtools/arg.h>
#include <cxxtools/log.h>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        log_init();
        cxxtools::Arg<std::string> ip(argc, argv, 'i');
        cxxtools::Arg<unsigned short> port(argc, argv, 'p', 9001);

        tntpub::Client client(ip, port);

        if (argc == 1)
            client.subscribe("plainText");
        else
            for (int a = 1; a < argc; ++a)
                client.subscribe(argv[a]);

        while (true)
            std::cout << client.readMessage().data() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
