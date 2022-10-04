/*
 * Copyright (C) 2016 Tommi Maekitalo
 *
 */

#ifndef TNTPUB_RESPONDER_H
#define TNTPUB_RESPONDER_H

#include <tntpub/subscription.h>
#include <cxxtools/connectable.h>
#include <cxxtools/signal.h>

#include <cxxtools/bin/deserializer.h>

#include <cxxtools/net/tcpstream.h>

#include <vector>

namespace tntpub
{

class DataMessage;
class SubscribeMessage;
class Server;

////////////////////////////////////////////////////////////////////////
// Responder
//
// A Responder may send messages to this server or subscribe
// for messages
//
class Responder : public cxxtools::Connectable
{
    class DestructionSentry
    {
        bool _deleted;
        Responder* _responder;

    public:
        explicit DestructionSentry(Responder* responder)
            : _deleted(false),
              _responder(responder)
            { _responder->_sentry = this; }

        ~DestructionSentry()
            { if (!_deleted) _responder->_sentry = 0; }

        void detach()         { _deleted = true; }
        bool deleted() const  { return _deleted; }
    };

    cxxtools::IOStream* _stream;
    Server& _pubSubServer;
    cxxtools::bin::Deserializer _deserializer;
    DestructionSentry* _sentry;

    std::vector<Subscription> _subscriptions;

    void onInput(cxxtools::StreamBuffer&);
    void onOutput(cxxtools::StreamBuffer&);

    void closeClient();

protected:
    void init(cxxtools::IOStream& stream);
    virtual ~Responder();

    virtual void subscribeMessageReceived(const SubscribeMessage& subscribeMessage);

public:
    explicit Responder(Server& pubSubServer);

    void subscribe(const SubscribeMessage& subscribeMessage);
    void subscribe(const std::string& topic, Subscription::Type type = Subscription::Type::Full);

    bool isSubscribed(const std::string& topic);
    const std::vector<Subscription>& subscriptions() const  { return _subscriptions; }

    void onDataMessageReceived(const DataMessage&);
    void sendMessage(const DataMessage& dataMessage);

    cxxtools::StreamBuffer& buffer()
    { return _stream->buffer(); }

    // signals that all messages has been sent to the peer
    cxxtools::Signal<Responder&> outputBufferEmpty;
};

class TcpResponder : public Responder
{
    cxxtools::net::TcpStream _netstream;

public:
    explicit TcpResponder(Server& pubSubServer);
};

}

#endif
