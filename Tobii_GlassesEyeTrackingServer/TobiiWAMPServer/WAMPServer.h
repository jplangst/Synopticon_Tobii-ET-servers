#pragma once

#include <autobahn.hpp>
#include <wamp_websocketpp_websocket_transport.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <boost/version.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <msgpack.hpp>

#include <math.h>
#include "DataContainer.h"

#include <thread>     
#include <vector>

#include "CustomTypes.h"
#include "DataContainer.h"

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef msgpack::v1::object object;

class WAMPServer
{
private:

	//Tobii Package handling
	long packageID; //The current id of the package we are working on

	bool closedManually;
	bool shouldLeave;
	static TobiiEyeSample currentSample;
	bool bStopped;

	bool StoppedUnexpectedly;

	std::thread vpxThread;
	std::thread imagerServerThread;

	void StreamGazeDataSample(std::shared_ptr<autobahn::wamp_session> session);

	std::string glassesID;

	boost::asio::io_service io;
public:
	void WebsocketStreamer(string crossbarAddress, string glassesID);
	void LeaveSession();

	bool IsStoppedUnexpectedly();
	bool IsStopped();

	WAMPServer();
	~WAMPServer();
};

