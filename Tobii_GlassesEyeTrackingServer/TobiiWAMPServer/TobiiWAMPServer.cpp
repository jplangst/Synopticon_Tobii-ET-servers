// TobiiWAMPServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CommCtrl.h"
#include "TobiiWAMPServer.h"
#include "CustomTypes.h"

#define MAX_LOADSTRING 100

using boost::property_tree::ptree;
using namespace boost::locale::conv;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	/*json::value outJson;
	TobiiAPI::Discovery(outJson);*/
	



    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TOBIIWAMPSERVER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TOBIIWAMPSERVER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOBIIWAMPSERVER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TOBIIWAMPSERVER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HWND hwndComboBox;
HWND addressComboBox;
HWND crossbarAdressTextBox;
HWND glassesIDTextBox;
HWND hWnd;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   HWND hwndButton = CreateWindow(
	   L"BUTTON",  // Predefined class; Unicode assumed 
	   L"DISCOVER",      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
	   10,         // x position 
	   70,         // y position 
	   100,        // Button width
	   50,        // Button height
	   hWnd,     // Parent window
	   (HMENU) BTN_DISCOVERY,       // No menu.
	   (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),
	   nullptr);      // Pointer not needed.
   
   HWND selectInterfaceButton = CreateWindow(
	   L"BUTTON",  // Predefined class; Unicode assumed 
	   L"SELECT",      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
	   310,         // x position 
	   10,         // y position 
	   100,        // Button width
	   50,        // Button height
	   hWnd,     // Parent window
	   (HMENU)BTN_SELECT_INTERFACE,       // No menu.
	   (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),
	   nullptr);      // Pointer not needed.

   hwndComboBox = CreateWindow(
	   WC_COMBOBOX, 
	   L"",
	   CBS_DROPDOWN | CBS_HASSTRINGS | WS_VISIBLE | WS_CHILD | WS_BORDER,
	   110, 70,
	   200,
	   250,
	   hWnd,
	   (HMENU) CBOX_AVAILABLE_DEVICES,
	   (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),
	   nullptr);

   addressComboBox = CreateWindow(
	   WC_COMBOBOX,
	   L"",
	   CBS_DROPDOWN | CBS_HASSTRINGS | WS_VISIBLE | WS_CHILD | WS_BORDER,
	   110, 10,
	   200,
	   250,
	   hWnd,
	   (HMENU) CBOX_AVAILABLE_ADDRESSES,
	   (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),
	   nullptr);

   FillAdressComboBox();

   HWND hwndButtonConnect = CreateWindow(
	   L"BUTTON",  // Predefined class; Unicode assumed https://docs.microsoft.com/en-us/windows/desktop/winmsg/about-window-classes#types-of-window-classes
	   L"CONNECT",      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
	   320,         // x position 
	   70,         // y position 
	   100,        // Button width
	   50,        // Button height
	   hWnd,     // Parent window
	   (HMENU)BTN_CONNECT,       // No menu.
	   (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),
	   nullptr);      // Pointer not needed.

   HWND hwndButtonDisconnect = CreateWindow(
	   L"BUTTON",  // Predefined class; Unicode assumed 
	   L"DISCONNECT",      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
	   420,         // x position 
	   70,         // y position 
	   100,        // Button width
	   50,        // Button height
	   hWnd,     // Parent window
	   (HMENU)BTN_DISCONNECT,       // No menu.
	   (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),
	   nullptr);      // Pointer not needed.

   HWND crossbarAdressLabel = CreateWindow(
	   L"STATIC",
	   L"Crossbar Adress",
	   WS_BORDER | WS_CHILD | WS_VISIBLE,
	   10,
	   200,
	   100,
	   50,
	   hWnd,
	   NULL,
	   NULL,
	   nullptr);

   crossbarAdressTextBox = CreateWindow(
	   L"EDIT",
	   L"lupin.ad.ife.no:9080",
	   WS_BORDER | WS_CHILD | WS_VISIBLE,
	   110, 
	   200, 
	   150, 
	   50,
	   hWnd, 
	   NULL, 
	   NULL, 
	   nullptr);

   //HWND crossbarAdressLabel = CreateWindow(
	  // L"STATIC",
	  // L"Crossbar Adress",
	  // WS_BORDER | WS_CHILD | WS_VISIBLE,
	  // 10,
	  // 200,
	  // 100,
	  // 50,
	  // hWnd,
	  // NULL,
	  // NULL,
	  // nullptr);

   //crossbarAdressTextBox = CreateWindow(
	  // L"EDIT",
	  // L"127.0.0.1:8080",
	  // WS_BORDER | WS_CHILD | WS_VISIBLE,
	  // 110,
	  // 200,
	  // 150,
	  // 50,
	  // hWnd,
	  // NULL,
	  // NULL,
	  // nullptr);

   HWND glassesIDLabel = CreateWindow(
	   L"STATIC",
	   L"Glasses ID",
	   WS_BORDER | WS_CHILD | WS_VISIBLE,
	   10,
	   300,
	   100,
	   50,
	   hWnd,
	   NULL,
	   NULL,
	   nullptr);

   glassesIDTextBox = CreateWindow(
	   L"EDIT",
	   L"TobiiGlasses1",
	   WS_BORDER | WS_CHILD | WS_VISIBLE,
	   110,
	   300,
	   150,
	   50,
	   hWnd,
	   NULL,
	   NULL,
	   nullptr);

   HWND hwndWAMPDisconnect = CreateWindow(
	   L"BUTTON",  // Predefined class; Unicode assumed 
	   L"BROADCAST TO WAMP",      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
	   320,         // x position 
	   200,         // y position 
	   200,        // Button width
	   50,        // Button height
	   hWnd,     // Parent window
	   (HMENU)BTN_WAMPCONNECT,       // No menu.
	   (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),
	   nullptr);      // Pointer not needed.

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void FillAdressComboBox() {
	TobiiAPI::AdressOptions.empty();
	FindValidInterfaceOptions();


	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//std::string narrow = converter.to_bytes(wide_utf16_source_string);
	

	for (TobiiWAMP::ValidAddressOption option : TobiiAPI::AdressOptions) {
		std::wstring optionName = converter.from_bytes(option.Friendlyname);
		SendMessage(
			addressComboBox,
			(UINT)CB_ADDSTRING,
			(WPARAM)0, (LPARAM)optionName.c_str());
	}
	SendMessage(addressComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}

void FindValidInterfaceOptions() {
	FILE *stream;
	char buff[100];
	DWORD bufflen = 100;

	//FOR TESTING ONLY, DEBUG CONSOLE
	BOOL chk = AllocConsole();
	if (chk)
	{
		freopen_s(&stream, "CONOUT$", "w", stdout);
		printf(" printing to console");
	}
	else
	{
		//throw new SomeException();
	}
	// END HERE

/* Declare and initialize variables */

	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i = 0;

	// Set the flags to pass to GetAdaptersAddresses
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

	// default to unspecified address family (both)
	ULONG family = AF_UNSPEC;

	LPVOID lpMsgBuf = NULL;

	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 0;
	ULONG Iterations = 0;

	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
	PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
	PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
	PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
	IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
	IP_ADAPTER_PREFIX *pPrefix = NULL;

	//family = AF_INET;
	family = AF_INET6;

	printf("Calling GetAdaptersAddresses function with family = ");
	if (family == AF_INET)
		printf("AF_INET\n");
	if (family == AF_INET6)
		printf("AF_INET6\n");
	if (family == AF_UNSPEC)
		printf("AF_UNSPEC\n\n");

	// Allocate a 15 KB buffer to start with.
	outBufLen = WORKING_BUFFER_SIZE;

	do {

		pAddresses = (IP_ADAPTER_ADDRESSES *)MALLOC(outBufLen);
		if (pAddresses == NULL) {
			printf
			("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
			exit(1);
		}

		dwRetVal =
			GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

		if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
			FREE(pAddresses);
			pAddresses = NULL;
		}
		else {
			break;
		}

		Iterations++;

	} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));
	if (dwRetVal == NO_ERROR) {
		// If successful, output some information from the data we received
		pCurrAddresses = pAddresses;
		while (pCurrAddresses) {

			const std::locale loc("C");
			std::string friendlyName = "................................";
			std::wstring tmp = pCurrAddresses->FriendlyName;
			std::use_facet<std::ctype<wchar_t> >(loc).narrow(tmp.c_str(), tmp.c_str() + tmp.size(), '?', &*friendlyName.begin());
			//std::replace(friendlyName.begin(), friendlyName.end(), ".", ""); // replace all 'x' to 'y'
			friendlyName.erase(std::remove(friendlyName.begin(), friendlyName.end(), '.'), friendlyName.end());

			//Filter to only show the sensible choices
			/*if (friendlyName.find("Local") != std::string::npos || friendlyName.find("Loopback") != std::string::npos) {
				pCurrAddresses = pCurrAddresses->Next;
				continue;
			}*/

			printf("\tLength of the IP_ADAPTER_ADDRESS struct: %ld\n",
				pCurrAddresses->Length);
			printf("\tIfIndex (IPv4 interface): %u\n", pCurrAddresses->IfIndex);
			printf("\tAdapter name: %s\n", pCurrAddresses->AdapterName);

			pUnicast = pCurrAddresses->FirstUnicastAddress;
			if (pUnicast != NULL) {
				
				//for (i = 0; pUnicast != NULL; i++)

					if (pUnicast->Address.lpSockaddr->sa_family == AF_INET)
					{
						sockaddr_in *sa_in = (sockaddr_in *)pUnicast->Address.lpSockaddr;
						printf("\tIPV4:%s\n", inet_ntop(AF_INET, &(sa_in->sin_addr), buff, bufflen));

						TobiiWAMP::ValidAddressOption NewOption;
						NewOption.Friendlyname = friendlyName;
						NewOption.IsIPV6 = false;
						NewOption.IPAddress = inet_ntop(AF_INET, &(sa_in->sin_addr), buff, bufflen);
						TobiiAPI::AdressOptions.push_back(NewOption);
					}
					else if (pUnicast->Address.lpSockaddr->sa_family == AF_INET6)
					{
						sockaddr_in6 *sa_in6 = (sockaddr_in6 *)pUnicast->Address.lpSockaddr;
						printf("\tIPV6:%s\n", inet_ntop(AF_INET6, &(sa_in6->sin6_addr), buff, bufflen));

						TobiiWAMP::ValidAddressOption NewOption;
						NewOption.Friendlyname = friendlyName;
						NewOption.IsIPV6 = true;
						NewOption.IPv6InterfaceIndex = pCurrAddresses->Ipv6IfIndex;
						NewOption.IPAddress = inet_ntop(AF_INET6, &(sa_in6->sin6_addr), buff, bufflen);
						TobiiAPI::AdressOptions.push_back(NewOption);
					}
					else
					{
						printf("\tUNSPEC");
					}

					pUnicast = pUnicast->Next;
					printf("\tNumber of Unicast Addresses: %d\n", i);
			}
			else
				printf("\tNo Unicast Addresses\n");

			pAnycast = pCurrAddresses->FirstAnycastAddress;
			if (pAnycast) {
				for (i = 0; pAnycast != NULL; i++)
					pAnycast = pAnycast->Next;
				printf("\tNumber of Anycast Addresses: %d\n", i);
			}
			else
				printf("\tNo Anycast Addresses\n");

			pMulticast = pCurrAddresses->FirstMulticastAddress;
			if (pMulticast) {
				for (i = 0; pMulticast != NULL; i++)
					pMulticast = pMulticast->Next;
				printf("\tNumber of Multicast Addresses: %d\n", i);
			}
			else
				printf("\tNo Multicast Addresses\n");

			pDnServer = pCurrAddresses->FirstDnsServerAddress;
			if (pDnServer) {
				for (i = 0; pDnServer != NULL; i++)
					pDnServer = pDnServer->Next;
				printf("\tNumber of DNS Server Addresses: %d\n", i);
			}
			else
				printf("\tNo DNS Server Addresses\n");

			printf("\tDNS Suffix: %wS\n", pCurrAddresses->DnsSuffix);
			printf("\tDescription: %wS\n", pCurrAddresses->Description);
			printf("\tFriendly name: %wS\n", pCurrAddresses->FriendlyName);

			if (pCurrAddresses->PhysicalAddressLength != 0) {
				printf("\tPhysical address: ");
				for (i = 0; i < (int)pCurrAddresses->PhysicalAddressLength;
					i++) {
					if (i == (pCurrAddresses->PhysicalAddressLength - 1))
						printf("%.2X\n",
						(int)pCurrAddresses->PhysicalAddress[i]);
					else
						printf("%.2X-",
						(int)pCurrAddresses->PhysicalAddress[i]);
				}
			}
			printf("\tFlags: %ld\n", pCurrAddresses->Flags);
			printf("\tMtu: %lu\n", pCurrAddresses->Mtu);
			printf("\tIfType: %ld\n", pCurrAddresses->IfType);
			printf("\tOperStatus: %ld\n", pCurrAddresses->OperStatus);
			printf("\tIpv6IfIndex (IPv6 interface): %u\n",
				pCurrAddresses->Ipv6IfIndex);
			printf("\tZoneIndices (hex): ");
			for (i = 0; i < 16; i++)
				printf("%lx ", pCurrAddresses->ZoneIndices[i]);
			printf("\n");

			printf("\tTransmit link speed: %I64u\n", pCurrAddresses->TransmitLinkSpeed);
			printf("\tReceive link speed: %I64u\n", pCurrAddresses->ReceiveLinkSpeed);

			pPrefix = pCurrAddresses->FirstPrefix;
			if (pPrefix) {
				for (i = 0; pPrefix != NULL; i++)
					pPrefix = pPrefix->Next;
				printf("\tNumber of IP Adapter Prefix entries: %d\n", i);
			}
			else
				printf("\tNumber of IP Adapter Prefix entries: 0\n");

			printf("\n");

			pCurrAddresses = pCurrAddresses->Next;
		}
	}
	else {
		printf("Call to GetAdaptersAddresses failed with error: %d\n",
			dwRetVal);
		if (dwRetVal == ERROR_NO_DATA)
			printf("\tNo addresses were found for the requested parameters\n");
		else {

			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				// Default language
				(LPTSTR)& lpMsgBuf, 0, NULL)) {
				printf("\tError: %s", lpMsgBuf);
				LocalFree(lpMsgBuf);
				if (pAddresses)
					FREE(pAddresses);
				exit(1);
			}
		}
	}

	if (pAddresses) {
		FREE(pAddresses);
	}
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case BTN_SELECT_INTERFACE:
			{
				//Get the selected index of the addressComboBox 
				TobiiAPI::selectedAddressOption = ComboBox_GetCurSel(addressComboBox);
				//Start the discovery thread
				TobiiAPI::Discovery();
				break;
			}
			case BTN_DISCOVERY:
			{			
				

				try
				{
					std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
					map <string, TobiiControlUnitInfo> ::iterator itr;
					for (itr = TobiiAPI::discoverdGlasses.begin(); itr != TobiiAPI::discoverdGlasses.end(); ++itr)
					{
						std::wstring optionName = converter.from_bytes(itr->first);

						SendMessage(
							hwndComboBox,
							(UINT)CB_ADDSTRING,
							(WPARAM)0, (LPARAM)optionName.c_str());
					}
					SendMessage(hwndComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		
				}
				catch (std::system_error &ex)
				{

				}

				break;
			}
			case BTN_CONNECT:
			{
				try {
					wchar_t *t = new wchar_t[50];
					GetWindowText(hwndComboBox, t, 50);
					char* crossbar = new char[50];
					wcstombs(crossbar, t, 50);
					string glassesName = crossbar;
					map <string, TobiiControlUnitInfo> ::iterator itr = TobiiAPI::discoverdGlasses.find(glassesName);
					if (itr != TobiiAPI::discoverdGlasses.end())
					{
						TobiiControlUnitInfo controlUnitInfo = itr->second;
						boost::asio::ip::address ipAdress = controlUnitInfo.info.address();
						string IP = ipAdress.to_v6().to_string();
						IP = IP.substr(0, IP.find('%'));
						
						string port = "49152";//std::to_string(controlUnitInfo.info.port());
						TobiiAPI::SetupEyeDataStream(IP, port);
					}
				}
				catch (std::system_error &ex)
				{

				}
				

				break;
			}
			case BTN_DISCONNECT:
			{
				TobiiAPI::StopEyeDataStream();
				break;
			}
			case BTN_WAMPCONNECT:
			{
				wchar_t *t = new wchar_t[50];
				GetWindowText(crossbarAdressTextBox, t, 50);
				char* crossbar = new char[50];
				wcstombs(crossbar, t, 50);
				string crossbarAdress = crossbar;

				GetWindowText(glassesIDTextBox, t, 50);
				char* glasses = new char[50];
				wcstombs(glasses, t, 50);
				string glassesID = glasses;

				TobiiAPI::StartWAMPStream(crossbarAdress, glassesID);

				delete t;
				delete crossbar;
				delete glasses;
				break;
			}
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
