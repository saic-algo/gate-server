#include <iostream>
#include <regex>

#include"gate-request-handler-factory.h"
#include"gate-handlers/update-car-position.h"
#include"gate-handlers/update-car-status.h"

#define H(n) "[0-9a-f]{" #n "}"
#define REGEX_UUID H(8) "-" H(4) "-" H(4) "-" H(4) "-" H(12)

const std::regex REGEX_POSITION("^/GPS-Server/position/.*$");
const std::regex REGEX_STATUS("^/GPS-Server/status/.*$");

HTTPRequestHandler* GateRequestHandlerfactory::createRequestHandler(const HTTPServerRequest &request) {
	const std::string &uri = request.getURI();
	//std::cout<<uri<<std::endl;
	if(std::regex_match(uri,REGEX_POSITION)) return new UpdateCarPosition(m_registry);
	else if(std::regex_match(uri,REGEX_STATUS)) return new UpdateCarStatus(m_registry);
	return 0;
}