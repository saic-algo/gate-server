#ifndef __REQUEST__HANDLER_BASE__
#define __REQUEST__HANDLER_BASE__

#include<memory>
#include<ctime>
#include<string>
#include<iostream>

#include<Poco/Net/HTTPRequestHandler.h>
#include<Poco/Net/HTTPServerResponse.h>
#include<Poco/Net/HTTPServerRequest.h>

#include<Poco/JSON/Object.h>

#include"../gate-registry.h"

class BaseRequestHandler :public Poco::Net::HTTPRequestHandler{
    protected:
        GateRegistry &m_registry;
    public:
		BaseRequestHandler(GateRegistry &registry) :m_registry(registry) {}
		~BaseRequestHandler() {}
    
    protected:
        void SendResponse(Poco::Net::HTTPServerResponse &response,const Poco::JSON::Object::Ptr data);
        void BadRequest(Poco::Net::HTTPServerResponse &response);
        const Poco::JSON::Object::Ptr ReadRequestBody(Poco::Net::HTTPServerRequest &request);
};

#endif