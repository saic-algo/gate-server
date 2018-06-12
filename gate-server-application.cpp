#include <iostream>
#include <Poco/Util/Option.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include"gate-server-application.h"
#include"gate-request-handler-factory.h"

#include <windows.h>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;

using Poco::Net::ServerSocket;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;

void GateServerApplication::initialize(Application &app){
    loadConfiguration();
    ServerApplication::initialize(app);
}

void GateServerApplication::uninitialize(){
    ServerApplication::uninitialize();
}

void GateServerApplication::defineOptions(OptionSet &options){
    ServerApplication::defineOptions(options);

    options.addOption(
        Option("help","h","display argument help information")
        .required(false)
        .repeatable(false)
        .callback(OptionCallback<GateServerApplication>(this,&GateServerApplication::handleHelp))
    );

    options.addOption(
        Option("port","p","the port of the server")
        .required(false)
        .repeatable(false)
        .argument("PORT")
        .binding("prot")
    );
}

void GateServerApplication::handleHelp(const std::string&,const std::string&){
    _helpRequested=true;
    stopOptionsProcessing();
}

void sendCarData(GateRegistry *m_registry) {
	for (;;) {
		std::map<std::string, CarInfo> t_map;
		m_registry->getMap(t_map);
		std::vector<CarInfo> vc;
		for (auto &v : t_map) {
			if (v.second.status == CarInfo::busy) continue;
			if(v.second.status==CarInfo::offline) continue;
			//balala
			vc.emplace_back(v.second);
		}
		//std::cout << "deal with over" << std::endl;
		std::chrono::milliseconds dura(5000);
		std::this_thread::sleep_for(dura);
	}
}

int GateServerApplication::main(const std::vector<std::string> &){
	printf("status: %d\n",(int)_helpRequested);fflush(stdout);
    if(_helpRequested){
        HelpFormatter helpFormatter(options());

        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTIONS");
        helpFormatter.setHeader("An HTTP server that maintains Geo Index using S2");
        helpFormatter.format(std::cout);
    }
    else{
        unsigned short port=(unsigned short)config().getInt("port",8000);
        ServerSocket socket(port);
        HTTPServer server(new GateRequestHandlerfactory(m_registry), ServerSocket(8080),new HTTPServerParams);
        server.start();
		std::cout<<"Server started"<<std::endl;
		std::thread m_thread(sendCarData,&m_registry);
        waitForTerminationRequest();
		std::cout<<"Server Stop"<<std::endl;
        server.stop();	
    }
    return Application::EXIT_OK;
}

int main(int argc,char **argv){
    GateServerApplication app;
    app.run(argc,argv);
    return 0;
}