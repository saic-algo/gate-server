#include "gate_registry.h"

int GateRegistry::UpdateCarPosition(std::string cid,double lat,double lng,long long time){
    std::lock_guard<std::mutex> gurad(m_lock);
    if(!m_map.count(cid)) return -1;
    CarInfo &car=m_map[cid];
    if(time<car.time) return 0;
    car.time=time;
    car.lat=lat;
    car.lng=lng;
    return 0;
}

int GateRegistry::UpdateCarStatus(std::string cid,int status){
    std::lock_guard<std::mutex> gurad(m_lock);
    if(!m_map.count(cid)){m_map[cid]=CarInfo(cid,status);return 1;}
    m_map[cid].status=status;
    return 0;
}

void GateRegistry::getMap(std::map<std::string, CarInfo> &t_map) {
    std::lock_guard<std::mutex> gurad(m_lock);
    t_map = m_map;
}
