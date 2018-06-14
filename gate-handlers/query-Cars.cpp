#include"query-Cars.h"

using Poco::JSON::Object;
using Poco::JSON::Array;

void QueryCars::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
	Object::Ptr objres(new Object);
	Array::Ptr arrres(new Array);
	std::cout<<"Called query cars"<<std::endl;
	//objres->set("type","s2");

	std::map<std::string, CarInfo> t_map;
	m_registry.getMap(t_map);

	for (auto &v : t_map) {
		if (v.second.status == CarInfo::busy) continue;
		if (v.second.status == CarInfo::offline) continue;
		//balala
		Array point;
		point.add(v.second.cid);
		point.add(v.second.lat);
		point.add(v.second.lng);
		arrres->add(point);
	}
	std::cout<<arrres->size()<<std::endl;
	objres->set("points", arrres);

	//SendResponse(response, objres);
	auto &os=response.send();
	objres->stringify(os);
}