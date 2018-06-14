#ifndef __QUERY_CARS__
#define __QUERT_CARS__

#include"base.h"

class QueryCars :public BaseRequestHandler {
public:
	QueryCars(GateRegistry &registry) :BaseRequestHandler(registry) {}
	void handleRequest(Poco::Net::HTTPServerRequest &, Poco::Net::HTTPServerResponse &);
};

#endif // !__QUERY_CARS__