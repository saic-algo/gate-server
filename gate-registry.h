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
        int UpdateCarPosition(std::string cid,double lat,double lng,long long time){++count;
			std::lock_guard<std::mutex> gurad(m_lock);
            if(!m_map.count(cid)) return -1;
            CarInfo &car=m_map[cid];
            if(time<car.time) return 0;
            car.time=time;
            car.lat=lat;
            car.lng=lng;
            return 0;
        }

        int UpdateCarStatus(std::string cid,int status){++count;
			std::lock_guard<std::mutex> gurad(m_lock);
            if(!m_map.count(cid)){m_map[cid]=CarInfo(cid,status);return 1;}
            m_map[cid].status=status;
            return 0;
        }

		void getMap(std::map<std::string, CarInfo> &t_map) {
			std::lock_guard<std::mutex> gurad(m_lock);
			t_map = m_map;
		}

		long long count = 0;
};

#endif