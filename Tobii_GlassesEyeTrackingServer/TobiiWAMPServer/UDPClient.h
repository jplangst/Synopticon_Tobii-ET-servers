#pragma once

#include <iostream>
#include "CustomTypes.h"

namespace ip = boost::asio::ip;
using ip::udp;

using namespace std;
class UDPClient
{
public:
	UDPClient(
		boost::asio::io_service& io_service,
		const std::string& host,
		const std::string& port
	) : io_service_(io_service), socket_(io_service, udp::endpoint(udp::v6(), 0)) {
		
		udp::resolver resolver(io_service_);
		udp::resolver::query query(udp::v6(), host, port);
		udp::resolver::iterator iter = resolver.resolve(query);
		endpoint_ = *iter;
	}

	~UDPClient();

	void setupForMulticast(std::string localInterfaceAddress, int interfaceIndex);
	void send(const std::string& msg);
	ptree receive();
	TobiiControlUnitInfo receiveDiscovery();

private:
	boost::asio::io_service& io_service_;
	udp::socket socket_;
	udp::endpoint endpoint_;
};

