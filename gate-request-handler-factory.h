#ifndef __GATE_REQUEST_HANDLER_FACTORY__
#define __GATE_REQUEST_HANDLER_FACTORY__

#include<Poco/Net/HTTPRequestHandler.h>
#include<Poco/Net/HTTPRequestHandlerFactory.h>
#include<Poco/Net/HTTPServerRequest.h>

#include"gate-registry.h"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;

class GateRequestHandlerfactory : public HTTPRequestHandlerFactory{
    private:
        GateRegistry &m_registry;
    public:
		GateRequestHandlerfactory(GateRegistry &registry) :m_registry(registry) {}

        HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &request);
};

#endif