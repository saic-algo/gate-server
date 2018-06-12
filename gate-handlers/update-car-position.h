#ifndef __REQUEST_HANDLER_UPDATE_POSITION__
#define __REQUEST_HANDLER_UPDATE_POSITION__

#include"base.h"

class UpdateCarPosition:public BaseRequestHandler{
    public:
        UpdateCarPosition(GateRegistry &registry):BaseRequestHandler(registry){}
        void handleRequest(Poco::Net::HTTPServerRequest &request,Poco::Net::HTTPServerResponse &response);
};

#endif