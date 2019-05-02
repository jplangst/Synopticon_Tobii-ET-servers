#pragma once

//#include "SocketWrapper.h"

#include <WinSock2.h>
#include <vector>

#include <WS2tcpip.h>
#include <system_error>
#include <string>
#include <iostream>
#include <codecvt>

#include <thread>
#include <chrono>

//Get adapters
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

#include "UDPClient.h"
#include "CustomTypes.h"
#include "DataContainer.h"
#include "WAMPServer.h"                          

// Link with Iphlpapi.lib
#pragma comment(lib, "IPHLPAPI.lib")
#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
//

namespace TobiiWAMP {
	struct ValidAddressOption {
		std::string Friendlyname;
		bool IsIPV6;                                           
		std::string IPAddress;
		DWORD IPv6InterfaceIndex;
	};
};

using namespace std;
class TobiiAPI
{
	static int SelectedOption;

	//static SOCKET eyeDataUDPSocket;

	static UDPClient* eyeDataLiveStream;

	static bool Connected;
	static bool ReceivedThreadStarted;
	static thread SendDiscoveryThread;
	static thread KeepAliveEyeDataThread;
	static thread RecieveEyeDataThread;
	static thread WAMPThread;
	static thread ReceiveDiscoveryThread;
	static thread ReconnectThread;
	static WAMPServer* wampServer;
	static bool StopEyeDataStreaming;
	static void SendKeepAliveMsg(string msg);
	static void RecieveEyeData();
	static void SendDiscovery();
	static void ReceiveDiscovery();
	static boost::asio::io_service io_service;
	static bool IsDiscovering;
	static UDPClient* discovery;
	static void WhileStreaming(string crossbar, string glassesID);
public:
	static map<string, TobiiControlUnitInfo> discoverdGlasses;
	static int selectedAddressOption;
	static std::vector<TobiiWAMP::ValidAddressOption> AdressOptions;
	static void Discovery();
	static bool Connect();
	static void SetupEyeDataStream(string IP, string port);

	static void StopEyeDataStream();
	static void StartWAMPStream(string crossbar, string glassesID);
	static void StopWAMPStream();
	//static void RESTCreateNewProject(wstring IP);
	TobiiAPI();
	~TobiiAPI();
};

