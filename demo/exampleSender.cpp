/*
 * Copyright (C) 2016 Tommi Maekitalo
 *
 */

#include "mymessage.h"

#include <tntpub/client.h>

#include <cxxtools/json.h>
#include <cxxtools/arg.h>
#include <cxxtools/log.h>

#include <exception>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        log_init();

        cxxtools::Arg<std::string> ip(argc, argv, 'i');
        cxxtools::Arg<unsigned short> port(argc, argv, 'p', 9001);
        cxxtools::Arg<unsigned> count(argc, argv, 'n', 1);

        if (argc != 2)
        {
            std::cerr << "usage: " << argv[0] << " [-i ip] [-p port] [-n count] topic\n";
            return 1;
        }

        std::string topic = argv[1];

        tntpub::Client client(ip, port);

        MyMessage msg;

        try
        {
            // read messages from cin until it fails

            while (std::cin >> cxxtools::Json(msg))
            {
                for (unsigned n = 0; n < count; ++n)
                    client.sendMessage(topic, msg);
            }
        }
        catch (const cxxtools::SerializationError&)
        {
        }

        client.flush();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
