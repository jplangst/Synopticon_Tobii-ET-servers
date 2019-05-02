#include "stdafx.h"
#include "UDPClient.h"

UDPClient::~UDPClient()
{
	socket_.close();
}

void UDPClient::send(const std::string& msg) 
{
	socket_.send_to(boost::asio::buffer(msg, msg.size()), endpoint_);
}

TobiiControlUnitInfo UDPClient::receiveDiscovery()
{
	char _buffer[1024];
	size_t buffer_size = socket_.receive_from(boost::asio::buffer(_buffer), endpoint_);
	std::stringstream ss;
	ss.write(_buffer, buffer_size);
	ptree receivedJson;
	boost::property_tree::read_json(ss, receivedJson);
	TobiiControlUnitInfo result;
	result.discoveryResponse = receivedJson;
	result.info = endpoint_;
	return result;
}

ptree UDPClient::receive()
{
	char _buffer[1024];
	size_t buffer_size = socket_.receive_from(boost::asio::buffer(_buffer), endpoint_);
	std::stringstream ss;
	ss.write(_buffer, buffer_size);
	ptree receivedJson;
	boost::property_tree::read_json(ss, receivedJson);
	return receivedJson;
}

void UDPClient::setupForMulticast(std::string localInterfaceAddressV6, int interfaceIndex) {
	socket_.set_option(ip::multicast::outbound_interface(interfaceIndex));
}
