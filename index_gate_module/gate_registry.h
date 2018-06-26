#ifndef __GATE_REGISTRY__
#define __GATE_REGISTRY__

#include<map>
#include<string>
#include<mutex>

struct CarInfo{
    std::string cid;
    double lat,lng;
    int status;
    long long time;
	const static int free=1;
	const static int busy=2;
	const static int offline=3;

    CarInfo(std::string cid,double lat,double lng,int status,long long time):cid(cid),lat(lat),lng(lng),status(status),time(time){}
	CarInfo(std::string cid) :CarInfo(cid, 0.0, 0.0, 0, 0) {}
	CarInfo(std::string cid, int status) :CarInfo(cid, 0.0, 0.0, status, 0) {}
	CarInfo() :CarInfo(std::string()) {}
};

class GateRegistry{
    private:
        std::map<std::string,CarInfo> m_map;
		std::mutex m_lock;
    public:
        int UpdateCarPosition(std::string cid,double lat,double lng,long long time);

        int UpdateCarStatus(std::string cid,int status);

		void getMap(std::map<std::string, CarInfo> &t_map);
};

#endif
