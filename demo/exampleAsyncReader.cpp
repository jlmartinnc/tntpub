/*
 * Copyright (C) 2016 Tommi Maekitalo
 *
 */

#include "mymessage.h"

#include <tntpub/client.h>

#include <cxxtools/eventloop.h>
#include <cxxtools/net/tcpstream.h>
#include <cxxtools/json.h>
#include <cxxtools/arg.h>
#include <cxxtools/log.h>

#include <exception>
#include <iostream>

cxxtools::EventLoop loop;

void onMessageReceived(tntpub::DataMessage& message)
{
    MyMessage msg;
    message.get(msg);
    std::cout << cxxtools::Json(msg).beautify(true);
}

void onCloseClient(tntpub::Client&)
{
    loop.exit();
}

int main(int argc, char* argv[])
{
    try
    {
        log_init();

        cxxtools::Arg<std::string> ip(argc, argv, 'i');
        cxxtools::Arg<unsigned short> port(argc, argv, 'p', 9001);

        cxxtools::net::TcpStream peer(ip, port);
        peer.attachedDevice()->setSelector(&loop);
        tntpub::Client client(peer);

        for (int a = 1; a < argc; ++a)
            client.subscribe(argv[a]);

        cxxtools::connect(client.messageReceived, onMessageReceived);
        cxxtools::connect(client.closed, onCloseClient);

        loop.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
