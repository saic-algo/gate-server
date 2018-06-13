#include"update-car-status.h"
#include<Poco/Net/HTTPResponse.h>

using Poco::JSON::Object;
using Poco::JSON::Array;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;

bool handler(const std::string &s, std::string &cid, int &status) {
	const std::string header = "/GPS-Server/status/car?";
	std::string::size_type b = 0, len = s.size();
	for (; b < header.size(); ++b) if (header[b] != s[b]) return false;
	if (s.substr(b, 4) != "cid=") return false;
	b += 4;
	int e = b;
	while (e < s.size()) { if (s[e] == '&') break; ++e; }
	cid = s.substr(b, e - b);
	b = e + 1;
	if (s.substr(b, 7) != "status=") return false;
	b += 7;
	status = std::stoi(s.substr(b));
	return true;
}

void UpdateCarStatus::handleRequest(HTTPServerRequest &request,HTTPServerResponse &response){
	response.setStatus(HTTPResponse::HTTP_OK);
	auto t=request.getURI();
	//std::cout << "UpdateCarStatus_URL:" << t << std::endl;
	std::string cid;
	int status=0;
	bool ok=handler(t,cid,status);
	//std::cout<<"Status:"<<ok<<std::endl;
	//std::cout<<"cid="<<cid<<" status:"<<status<<std::endl;

	int rt=m_registry.UpdateCarStatus(cid,status);
	std::ostream &ost = response.send();
	ost << "ok\n";
}