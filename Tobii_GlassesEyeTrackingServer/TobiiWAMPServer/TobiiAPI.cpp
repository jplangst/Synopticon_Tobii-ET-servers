#include "stdafx.h"
#include "TobiiAPI.h"

const string DISCOVERY_MSG = "{\"type\":\"discover\"}";
const string KA_DATA_MSG = "{\"type\": \"live.data.unicast\", \"key\": \"62b3a246-2e4c-46ab-8082-f3ed7094e553\", \"op\": \"start\"}";
const string STOP_DATA_MSG = "{\"type\": \"live.data.unicast\", \"key\": \"62b3a246-2e4c-46ab-8082-f3ed7094e553\", \"op\": \"stop\"}";
const string message = "{\"type\": \"live.data.unicast\", \"key\": \"some_GUID\", \"op\": \"start\"}";
const string KA_VIDEO_MSG = "{\"type\": \"live.video.unicast\", \"key\": \"62b3a246-2e4c-46ab-8082-f3ed7094e553\", \"op\": \"start\"}";
const string DISCOVERY_ADRESS = "ff02::1";
const string DISCOVERY_PORT = "13006";

int TobiiAPI::selectedAddressOption = -1; //Not selected

bool TobiiAPI::Connected = false;
bool TobiiAPI::ReceivedThreadStarted = false;
bool TobiiAPI::IsDiscovering = false;

thread TobiiAPI::SendDiscoveryThread;
thread TobiiAPI::RecieveEyeDataThread;
thread TobiiAPI::KeepAliveEyeDataThread;
thread TobiiAPI::ReceiveDiscoveryThread;
thread TobiiAPI::WAMPThread;
thread TobiiAPI::ReconnectThread;
WAMPServer* TobiiAPI::wampServer = nullptr;
UDPClient* TobiiAPI::discovery = nullptr;

//SOCKET TobiiAPI::eyeDataUDPSocket;

UDPClient* TobiiAPI::eyeDataLiveStream = nullptr;

std::vector<TobiiWAMP::ValidAddressOption> TobiiAPI::AdressOptions;

bool TobiiAPI::StopEyeDataStreaming = false;

map<string, TobiiControlUnitInfo> TobiiAPI::discoverdGlasses;

boost::asio::io_service TobiiAPI::io_service;

TobiiAPI::TobiiAPI()
{
}


TobiiAPI::~TobiiAPI()
{

}

void TobiiAPI::Discovery()
{
	if (!IsDiscovering)
	{
		IsDiscovering = true;
		SendDiscoveryThread = thread(&TobiiAPI::SendDiscovery);
		ReceiveDiscoveryThread = thread(&TobiiAPI::ReceiveDiscovery);
	}
} 

void TobiiAPI::SendDiscovery()
{
	if (discovery)
	{
		delete discovery;
	}
	discovery = new UDPClient(io_service, DISCOVERY_ADRESS, DISCOVERY_PORT);
	discovery->setupForMulticast(AdressOptions[selectedAddressOption].IPAddress, AdressOptions[selectedAddressOption].IPv6InterfaceIndex);

	while (IsDiscovering)
	{
		try
		{
			discovery->send(DISCOVERY_MSG);
		}
		catch (std::system_error &ex)
		{

		}
		this_thread::sleep_for(5s);
	}
	delete discovery;
	//DiscoveryThread.join();
}

void TobiiAPI::ReceiveDiscovery()
{
	while (IsDiscovering)
	{
		try
		{
			if (discovery)
			{
				TobiiControlUnitInfo received = discovery->receiveDiscovery();
				string newGlassesID = received.discoveryResponse.get<string>("name");

				if (discoverdGlasses.find(newGlassesID) == discoverdGlasses.end())
				{
					discoverdGlasses.insert(pair<string, TobiiControlUnitInfo>(newGlassesID, received));
				}
			}
			
		}
		catch (std::system_error &ex)
		{

		}
		this_thread::sleep_for(0.1s);
	}
}

bool TobiiAPI::Connect()
{
	return true;
}

void TobiiAPI::SetupEyeDataStream(string IP, string port) {
	if (Connected) {
		printf("Already connected");
		return;
	}

	if (eyeDataLiveStream)
	{
		delete eyeDataLiveStream;
		eyeDataLiveStream = nullptr;
	}
	eyeDataLiveStream = new UDPClient(io_service, IP, port);

	KeepAliveEyeDataThread = thread(&TobiiAPI::SendKeepAliveMsg, KA_DATA_MSG);
	//RecieveEyeDataThread = thread(&TobiiAPI::RecieveEyeData);

	Connected = true;
}

void TobiiAPI::StopEyeDataStream()
{
	if (eyeDataLiveStream)
	{
		StopEyeDataStreaming = true;
		this_thread::sleep_for(2s);
		eyeDataLiveStream->send(STOP_DATA_MSG);
		delete eyeDataLiveStream;
		eyeDataLiveStream = nullptr;
		Connected = false;
		ReceivedThreadStarted = false;
		KeepAliveEyeDataThread.join();
		RecieveEyeDataThread.join();
	}
}

void TobiiAPI::SendKeepAliveMsg(string msg)
{
	printf("Sendalive port in function is:");
	
	while (!StopEyeDataStreaming)
	{
		try
		{
			eyeDataLiveStream->send(msg);
			printf("Keep alive sent\n");

			if (!ReceivedThreadStarted)
			{
				RecieveEyeDataThread = thread(&TobiiAPI::RecieveEyeData);
				ReceivedThreadStarted = true;
			}				
		}
		catch (std::system_error &ex)
		{
			printf("Keep alive send failed\n");
		}

		this_thread::sleep_for(2s);
	}
}

void TobiiAPI::RecieveEyeData() 
{
	while (!StopEyeDataStreaming)
	{
		try 
		{
			/*the package contains eye tracking data and gaze vector
			 All messages that belong to a single eye-tracking event 
			 or gaze-event are accompanied by a gaze counter. This counter 
			 is represented by the gidx (gaze-index) property
			 - gidx: pupil center(pc), pupil diameter(pd), gaze direction(gd), gaze position(gp), gaze position 3d(gp3)
			 - mems: gyroscope(gy), accelerometer(ac)
			*/
			ptree package = eyeDataLiveStream->receive();
			DataContainer::getInstance()->addGazeSample(package);
			//string latency = package.get<string>("l");
			
		}
		catch (std::system_error &ex)
		{
			
		}
		//this_thread::sleep_for(1s);
	}
}

void TobiiAPI::StartWAMPStream(string crossbar, string glassesID)
{
	if (wampServer)
	{
		wampServer->LeaveSession();
		delete wampServer;
	}
	wampServer = new WAMPServer();

	//const char* defaultcrossbarAddress = "127.0.0.1:8080";
	//const char* defaultglass = "TobiiGlasses";
	//char* crossbarAddress = new char[50];
	//char* glasses = new char[50];
	//strcpy(crossbarAddress, defaultcrossbarAddress);
	//strcpy(glasses, defaultglass);
	WAMPThread = thread(&WAMPServer::WebsocketStreamer, TobiiAPI::wampServer, crossbar, glassesID);

	//ReconnectThread = thread(&TobiiAPI::WhileStreaming, crossbar, glassesID);
}

void TobiiAPI::WhileStreaming(string crossbar, string glassesID) {
	bool bAttemptingToRecconect = true;
	while (bAttemptingToRecconect) {
		if (wampServer->IsStoppedUnexpectedly()) {
			TobiiAPI::StopWAMPStream();
			WAMPThread = thread(&WAMPServer::WebsocketStreamer, TobiiAPI::wampServer, crossbar, glassesID);
		}
		//Cleanup this thread
		else if (wampServer->IsStopped()) {
			
			bAttemptingToRecconect = false;
		}

		//sleep for x seconds
		this_thread::sleep_for(1s);
	}
	ReconnectThread.join();
}

void TobiiAPI::StopWAMPStream()
{
	wampServer->LeaveSession();
	while (!wampServer->IsStopped()) {

	}



	delete wampServer;
	wampServer = nullptr;
	WAMPThread.join();
}