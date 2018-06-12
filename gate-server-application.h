#ifndef __GATE_SERVER_APPLICATION__
#define __GATE_SERVER_APPLICATION__

#include<Poco/Util/ServerApplication.h>
#include<Poco/Util/OptionSet.h>
#include<vector>
#include<string>
#include<thread>
#include"gate-registry.h"

using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::OptionSet;

class GateServerApplication: public ServerApplication{
    private:
        GateRegistry m_registry;
        bool _helpRequested;
    public:
        GateServerApplication():_helpRequested(false){}
        ~GateServerApplication(){}
    protected:
        void initialize(Application &self);
        void uninitialize();
        void defineOptions(OptionSet &options);
        void handleHelp(const std::string&,const std::string&);
        int main(const std::vector<std::string> &);
		//void sendCarData();
};

#endif  