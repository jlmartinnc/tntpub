/*
 * Copyright (C) 2016 Tommi Maekitalo
 *
 */

#include <tntpub/server.h>
#include <tntpub/responder.h>

#include <cxxtools/log.h>

log_define("tntpub.server")

namespace tntpub
{

////////////////////////////////////////////////////////////////////////
// Server
//
Server::Server(cxxtools::SelectorBase& selector)
{
    cxxtools::connect(_server.connectionPending, *this, &Server::onConnectionPending);

    _server.setSelector(&selector);
}

Server::Server(cxxtools::SelectorBase& selector, const std::string& ip, unsigned short port)
{
    cxxtools::connect(_server.connectionPending, *this, &Server::onConnectionPending);

    _server.setSelector(&selector);

    listen(ip, port);
}

void Server::listen(const std::string& ip, unsigned short port)
{
    log_info("listen on " << ip << ':' << port);
    _server.listen(ip, port);
}

void Server::onConnectionPending(cxxtools::net::TcpServer&)
{
    clientConnected(*createResponder());
}

Responder* Server::createResponder()
{
    return new Responder(*this);
}

void Server::processMessage(Responder& responder, DataMessage& dataMessage)
{
    if (onDataMessageReceived(responder, dataMessage))
    {
        doSendMessage(dataMessage);
        onDataMessageProcessed(responder, dataMessage);
    }
}

void Server::doSendMessage(const DataMessage& dataMessage)
{
    dispatchMessage(dataMessage);
}

unsigned Server::maxOBuf()
{
    return Responder::maxOBuf();
}

void Server::maxOBuf(unsigned n)
{
    Responder::maxOBuf(n);
}

}
