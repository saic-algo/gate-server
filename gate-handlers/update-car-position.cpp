#include"update-car-position.h"
#include<Poco/Net/HTTPResponse.h>
#include<iostream>

using Poco::JSON::Object;
using Poco::JSON::Array;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;

#define PV(name) std::cout<<#name" = "<<name<<std::endl;

bool handler(const std::string &s, std::string &cid, double &lat, double &lng, long long &time) {
	const std::string header = "/GPS-Server/position/car?";
	std::string::size_type b = 0, len = s.size();
	for (; b < header.size(); ++b) if (header[b] != s[b]) return false;
	if (s.substr(b, 4) != "cid=") return false;
	b += 4;
	int e = b;
	while (e < s.size()) { if (s[e] == '&') break; ++e; }
	cid = s.substr(b, e - b);
	b = e+1;
	if (s.substr(b, 4) != "lat=") return false;
	b += 4;e=b;
	while (e < s.size()) { if (s[e] == '&') break; ++e; }
	lat = std::stod(s.substr(b,e-b));
	b=e;
	if (s.substr(b, 5) != "&lng=") return false;
	b += 5;
	e=b;
	while (e < s.size()) { if (s[e] == '&') break; ++e; }
	lng = std::stod(s.substr(b,e-b));
	b=e;
	if (s.substr(b, 11) != "&timestamp=") return false;
	b += 11;
	time = std::stoll(s.substr(b));
	return true;
}

void UpdateCarPosition::handleRequest(HTTPServerRequest &request,HTTPServerResponse &response){
    response.setStatus(HTTPResponse::HTTP_OK);
	const std::string t=request.getURI();
    std::cout<<"UpdateCarPosition_URL:"<<t<<std::endl;
	std::string cid;
	double lat=0.0,lng=0.0;
	long long time=0;
	bool ok=handler(t,cid,lat,lng,time);
	std::cout<<"Statue:"<<ok<<std::endl;
	std::cout<<"cid="<<cid<<" lat="<<lat<<" lng"<<lng<<" time="<<time<<std::endl;

	int status=m_registry.UpdateCarPosition(cid,lat,lng,time);
	std::ostream &ost=response.send();
	ost<<"ok\n";
}