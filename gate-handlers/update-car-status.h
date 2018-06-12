#ifndef __REQUEST_HANDLER_UPDATE_STATUS__
#define __REQUEST_HANDLER_UPDATE_STATUS__

#include"base.h"

class UpdateCarStatus :public BaseRequestHandler {
public:
	UpdateCarStatus(GateRegistry &registry) :BaseRequestHandler(registry) {}
	void handleRequest(Poco::Net::HTTPServerRequest &resquest, Poco::Net::HTTPServerResponse &response);
};
#endif // !__UPDATE_CAR_STATUS__