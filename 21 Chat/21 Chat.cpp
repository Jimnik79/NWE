//--------------------------------------------------------------------------------------
// File: 21 Chat.cpp
// Description: Sample of work chat with registration and authorization.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// Server displays all chat messages, including personal.
// Client displays only their own.
// When connecting to a server, the client receives only the last 100 chat messages.

// This example shows how to work with Acoounts and chat:
// - start server;
// - cannect to server;
// - registration;
// - authorization;
// - chat messages.

// NOTE:
// - Must be connected to the Internet or a local network.
// - Don't forget to open the port in the firewall when the server starts and the client connects.
// - Need that the client version matches the server version.
// - For all messages from the client in MMO should be used: 0 and 1 byte - short id of message, 2 and 3 byte - short id of player.
// - The client and server do not necessarily develop in the same application, it's only done here for the convenience of demonstration, in the final commercial versions, client and server desirable to have separately.


#include "..\Engine\NetWars.h"	// Engine

#include <stdio.h>	// swprintf_s
#include <time.h>	// struct tm, _time32, _localtime32_s


CEngine	g_pAppl;	// Engine class
CText	g_pText;	// Text class
CUI		g_pUI;		// UI class
CDB		g_pDB;		// Database class
CNet	g_pNet;		// Network class


#define	ID_WINDOW_MENU		0	// START CHAT
#define	ID_STATIC_SQL_NAME	1	// DataBase name:
#define	ID_EDIT_SQL_NAME	2	// DataBase name
#define	ID_STATIC_SERVER	3	// SQL Server name:
#define	ID_EDIT_SERVER		4	// SQL Server name
#define	ID_STATIC_SQL_LOGIN	5	// Login for SQL Server authentication:
#define	ID_EDIT_SQL_LOGIN	6	// Login for SQL Server authentication
#define	ID_STATIC_SQL_PASS	7	// Password for SQL Server authentication:
#define	ID_EDIT_SQL_PASS	8	// Password for SQL Server authentication
#define	ID_STATIC_IP		9	// IP:
#define	ID_EDIT_IP			10	// IP
#define	ID_STATIC_PORT		11	// Port:
#define	ID_EDIT_PORT		12	// Port
#define	ID_STATIC_LOGIN		13	// Name:
#define	ID_EDIT_LOGIN		14	// Name
#define	ID_STATIC_PASSWORD	15	// Password:
#define	ID_EDIT_PASSWORD	16	// Password
#define	ID_BUTTON_SERVER	17	// Start server
#define	ID_BUTTON_CONNECT	18	// Connect to the Server
#define	ID_BUTTON_REG		19	// Registraion on the Server
#define	ID_WINDOW_REG		20	// REGISTRATION
#define	ID_STATIC_NAME		21	// Name:
#define	ID_EDIT_NAME		22	// Name
#define	ID_STATIC_PASS		23	// Password:
#define	ID_EDIT_PASS		24	// Password
#define	ID_STATIC_MAIL		25	// E-mail:
#define	ID_EDIT_MAIL		26	// E-mail
#define	ID_STATIC_SKYPE		27	// Skype:
#define ID_EDIT_SKYPE		28	// Skype
#define	ID_STATIC_COUNTRY	29	// Country:
#define ID_EDIT_COUNTRY		30	// Country
#define	ID_STATIC_CITY		31	// City:
#define ID_EDIT_CITY		32	// City
#define	ID_STATIC_AGE		33	// Age:
#define ID_EDIT_AGE			34	// Age
#define ID_RADIO_MALE		35	// Male
#define ID_RADIO_FEMALE		36	// Female
#define	ID_STATIC_MYSELF	37	// About my self:
#define ID_EDIT_MYSELF		38	// About my self
#define	ID_BUTTON_REGISTER	39	// Register on the Server
#define	ID_WINDOW_CHAT		40	// CHAT
#define	ID_STATIC_PLAYERS	41	// List of players
#define	ID_LIST_PLAYERS		42	// List of players
#define	ID_STATIC_CHAT		43	// Chat with ...
#define	ID_LIST_CHAT		44	// List of chat
#define	ID_STATIC_MESSAGE	45	// Message:
#define	ID_EDIT_MESSAGE		46	// Edit message
#define	ID_STATIC_GUESTS	47	// Number of guests (not registered clients)
#define	ID_BUTTON_SEND		48	// Send
#define	ID_LIST_IP			49	// List of IP-addresses
#define	ID_BUTTON_EXIT		50	// Exit
#define	ID_MESSAGE_WAIT		51	// Wait
#define	ID_MESSAGE_ERROR	52	// Message with text of error
#define	ID_LIST_ACC			53	// List of accounts
#define	ID_LIST_REG			54	// List of registrations

#define	ID_FONT_16	0

#define	GROUP_GENDER	0

float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background

UINT	g_nidServer = 0;// Network Server ID
WORD	g_id = 0;		// Player ID
int		g_acc = 0;		// Player Account: -2 - an incorrect login, -3 - an incorrect password, -4 - the player has already been activated, -5 - ended billable time

BYTE	g_data[1000];	// Message type (2 bytes); Id of player (0 - server message)(2 bytes)
bool	g_bMsgError = false;	// Flag of message with text of error
UINT	g_nGuests = 0;	// Number of guests (not registered clients)
wchar_t	g_sName[16];	// The player name

wchar_t	g_sChat[] = L"Chat with";
wchar_t	g_sMsgTitle[32];	// To Recipient:
wchar_t	g_sMessage[162];	// Full message, including Sender and Recipient

enum state	// State machine
{
	MENU,
	REG,
	WAIT,
	SERVER,
	CHAT,
};
enum state g_eState = MENU;	// State machine

void ChangeState(state eState)
{
	g_eState = eState;	// New state

	g_pUI.visible(ID_WINDOW_MENU, eState == MENU);	// START CHAT
	g_pUI.visible(ID_WINDOW_REG, eState == REG);	// REGISTRATION
	g_pUI.visible(ID_MESSAGE_WAIT, eState == WAIT);	// WAIT
	g_pUI.visible(ID_WINDOW_CHAT, eState > WAIT);	// CHAT
	g_pUI.visible(ID_MESSAGE_ERROR, g_bMsgError);	// ERROR
	g_pUI.visible(ID_LIST_IP, eState == SERVER);	// List of IP-addresses
	g_pUI.visible(ID_LIST_ACC, eState == SERVER);	// List of accounts
	g_pUI.visible(ID_LIST_REG, eState == SERVER);	// List of registrations
}


//--------------------------------------------------------------------------------------
// SEND MESSAGE
//--------------------------------------------------------------------------------------
void sendMessage()
{
	wchar_t	sMsg[128];
	swprintf_s(sMsg, 128, g_pUI.getText(ID_EDIT_MESSAGE));
	if (sMsg[0])	// If there is a message
	{
		if (g_eState == SERVER)	// Server: write to the DB
		{
			wchar_t	sMessage[162];
			swprintf_s(sMessage, 162, L"%s%s", g_sMsgTitle, sMsg);
			g_pUI.addItem(ID_LIST_CHAT, sMessage, 0);	// 
			g_pNet.sendChatMsgToClient(g_pUI.getSelectedData(ID_LIST_PLAYERS), sMsg);
		}
		else	// Client: send to the server
		{
			swprintf_s(g_sMessage, 162, L"%s%s", g_sMsgTitle, sMsg);
			g_pNet.sendChatMsgToServer(g_pUI.getSelectedData(ID_LIST_PLAYERS), sMsg);
		}
	}
	g_pUI.text(ID_EDIT_MESSAGE, L"");	// Clear message editbox
}


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	g_pAppl.setCallbackGUI();				// Set callback for GUI event
	g_pAppl.setCallbackInitialize();		// Set callback for Initialize
	g_pAppl.setCallbackDeviceCreated();		// Set callback for Create Device
	g_pAppl.setCallbackSwapChainResized();	// Set callback for Resize
	g_pAppl.setCallbackFrameRender();		// Set callback for Render Frame

	g_pText.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_pUI.setHModule(g_pAppl.getHModule());		// Set NetWars.dll
	g_pDB.setHModule(g_pAppl.getHModule());		// Set NetWars.dll
	g_pNet.setHModule(g_pAppl.getHModule());	// Set NetWars.dll

	g_pNet.setCallbackConnect();			// Set callback function for connect the player to the server (host) or connect the server (host) to the player
	g_pNet.setCallbackDisconnect();			// Set callback function for disconnect the player from the server (host) or disconnect the server (host) from the player
	g_pNet.setCallbackNetMsg();				// Set callback function for get the network message

	// Run Engine
	return g_pAppl.run(L"21 Chat", 1024, 600);
}


//--------------------------------------------------------------------------------------
// GUI EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onGUI(UINT idEvent, int idEl)
{
	switch (idEl)
	{
	case ID_BUTTON_SERVER: {	// Start Server
		wchar_t s[256];
		g_acc = g_id = 0;	// Admin ID and account
		memcpy(&g_data[2], &g_id, 2);	// Player ID

		// Add work with account and chat on the server (DB tables and protocol), Connect to SQL Server (with Accounts and Chat), Activate the server database, Return number of accounts in the DataBase
		UINT nAcc = g_pNet.addAccountAndChat(g_pUI.getText(ID_EDIT_SQL_NAME), g_pUI.getText(ID_EDIT_SERVER), g_pUI.getText(ID_EDIT_SQL_LOGIN), g_pUI.getText(ID_EDIT_SQL_PASS));

		if (nAcc)	// If Admin registered then read accounts
		{
			ACCOUNT* pAcc = new ACCOUNT[nAcc];	// Allocate memory for accounts data
			g_pDB.getData(TABLE_ACCOUNT, (BYTE*)pAcc);	// Get data for accounts table (all items)
			for (UINT i = 0; i < nAcc; i++)
			{
				swprintf_s(s, 64, L"%d %s %s", pAcc[i].acc, pAcc[i].sName, pAcc[i].sPass);
				g_pUI.addItem(ID_LIST_ACC, s, i);	// Add account item to the account list
			}
			swprintf_s(g_sName, 16, pAcc[0].sName);
			SAFE_DELETE_ARRAY(pAcc);	// Free memory for accounts data

			// Read registrations
			REGISTR* pReg = new REGISTR[nAcc];	// Allocate memory for registrations data
			g_pDB.getData(TABLE_REGISTR, (BYTE*)pReg);	// Get data for registrations table (all items)
			for (UINT i = 0; i < nAcc; i++)
			{
				swprintf_s(s, 256, L"%d %s %s %s %s %d %s %s", pReg[i].acc, pReg[i].sMail, pReg[i].sSkype, pReg[i].sCountry, pReg[i].sCity, pReg[i].nAgeSex % 128, pReg[i].nAgeSex > 127 ? L"Male" : L"Female", pReg[i].sMyself);
				g_pUI.addItem(ID_LIST_REG, s, i);	// Add registration item to the registration list
			}
			SAFE_DELETE_ARRAY(pReg);	// Free memory for registrations data

			// Read chat
			UINT nMsg = g_pDB.getSize(TABLE_MESSAGE);	// Get chat message table size (new ID)
			if (nMsg)
			{
				MESSAGE* pMsg = new MESSAGE[nMsg];	// Allocate memory for chat messages data
				g_pDB.getData(TABLE_MESSAGE, (BYTE*)pMsg);	// Get data for chat messages table (all items)
				//UINT nMsg = g_pDB.getDataLast(TABLE_MESSAGE, (BYTE*)g_Msg, 1000);	// Get chat (last 1000 messages)
				for (UINT i = 0; i < nMsg; i++)
				{
					swprintf_s(s, 162, L"From %s To %s : %s", g_pDB.getName(pMsg[i].accSender), pMsg[i].accRecipient == -1 ? L"ALL" : g_pDB.getName(pMsg[i].accRecipient), pMsg[i].str);
					g_pUI.addItem(ID_LIST_CHAT, s, i);	// Add chat message item to the chat message list
				}
				SAFE_DELETE_ARRAY(pMsg);	// Free memory for chat messages data
			}
		}
		else	// Admin Registration (first start)
		{
			swprintf_s(g_sName, 16, L"Admin");
			g_pDB.createAdmin(L"Admin", L"Admin", L"Admin@gmail.com", L"AdminSkype", L"AdminCountry", L"AdminCity", 163, L"AdminMyself");	// Create Admin, 163 = 35 + 128 (male)
			g_pUI.addItem(ID_LIST_ACC, L"0 Admin Admin", 0);	// 
			g_pUI.addItem(ID_LIST_REG, L"0 Admin Admin Admin@gmail.com AdminSkype AdminCountry AdminCity 35 Male AdminMyself", 0);	// 
		}

		// List of players
		swprintf_s(s, 32, L"0 %s", g_pDB.getName(0));	// Player account and name
		g_pUI.addItem(ID_LIST_PLAYERS, s, 0);	// 
		g_pUI.selItem(ID_LIST_PLAYERS, 0);
		swprintf_s(g_sMsgTitle, 32, L"To ALL: ");	// To Recipient:

		// Start MMO Server
		UINT port;
		swscanf_s(g_pUI.getText(ID_EDIT_PORT), L"%d", &port);
		ChangeState(SERVER);	// State: SERVER
		UINT nIP = g_pNet.startServer((WORD)port);	// Start server
		for (UINT i = 0; i < nIP; i++)
			g_pUI.addItem(ID_LIST_IP, g_pNet.getIP(i), i);

	}	break;

	case ID_BUTTON_CONNECT: {	// Connect to Server
		/// CHECK THE NETWORK OR NOT, THAT TO SECOND TIME NOT CONNECTED, BUT ONLY SEND MESSAGE
		g_pNet.setAccountAndChat();	// Set work with account and chat on the client (only protocol)
		UINT port;
		swscanf_s(g_pUI.getText(ID_EDIT_PORT), L"%d", &port);
		g_pNet.connectToServer(g_pUI.getText(ID_EDIT_IP), (WORD)port, g_pUI.getText(ID_EDIT_LOGIN), g_pUI.getText(ID_EDIT_PASSWORD));	// Connect to the Server
		g_pUI.text(ID_STATIC_GUESTS, L"");	// Hide guests string, which only for server
		swprintf_s(g_sName, 16, g_pUI.getText(ID_EDIT_LOGIN));
		ChangeState(WAIT);	// Go to the state WAIT
		break;
	}

	case ID_BUTTON_REG: {	// Connect to Server and go to the Registration
		/// CHECK THE NETWORK OR NOT, THAT TO SECOND TIME NOT CONNECTED, BUT ONLY SEND MESSAGE
		g_pNet.setAccountAndChat();	// Set work with account and chat on the client (only protocol)
		UINT port;
		swscanf_s(g_pUI.getText(ID_EDIT_PORT), L"%d", &port);
		g_pNet.connectToServer(g_pUI.getText(ID_EDIT_IP), (WORD)port, L"", L"");	// Connect to the Server
		ChangeState(WAIT);	// Go to the state WAIT
		g_pUI.text(ID_EDIT_NAME, g_pUI.getText(ID_EDIT_LOGIN));
		g_pUI.text(ID_EDIT_PASS, g_pUI.getText(ID_EDIT_PASSWORD));
		g_pUI.text(ID_EDIT_LOGIN, L"");
		g_sName[0] = L'\0';
	}	break;

	case ID_BUTTON_REGISTER:	// Register
		if (g_pUI.checkMail(g_pUI.getText(ID_EDIT_MAIL)))
		{
			int	nAgeSex;
			swscanf_s(g_pUI.getText(ID_EDIT_AGE), L"%d", &nAgeSex);
			nAgeSex += g_pUI.getChecked(ID_RADIO_MALE) ? 128 : 0;
			g_pNet.sendRegistrToServer(g_pUI.getText(ID_EDIT_NAME), g_pUI.getText(ID_EDIT_PASS), g_pUI.getText(ID_EDIT_MAIL), g_pUI.getText(ID_EDIT_SKYPE),
				g_pUI.getText(ID_EDIT_COUNTRY), g_pUI.getText(ID_EDIT_CITY), (BYTE)nAgeSex, g_pUI.getText(ID_EDIT_MYSELF));
			ChangeState(WAIT);	// 
		}
		else
		{
			g_pUI.textMsg(ID_MESSAGE_ERROR, L"Invalid email address", L"", L"", L"");	// Message with text of error
			g_pUI.visible(ID_MESSAGE_ERROR, g_bMsgError = true);
		}
		break;

	case ID_BUTTON_EXIT:	// Connect to SQL Server
		g_pAppl.destroy();	// Exit programm
		break;

	case ID_LIST_IP:
		if (OpenClipboard(NULL))	// Copy the selection text to the clipboard
		{
			EmptyClipboard();

			wchar_t	s[MAX_PATH];
			swprintf_s(s, MAX_PATH, g_pUI.getSelText(ID_LIST_IP));	// 
			size_t size = sizeof(WCHAR) * (wcslen(s) + 1);
			HGLOBAL hBlock = GlobalAlloc(GMEM_MOVEABLE, size);
			if (hBlock)
			{
				WCHAR* pwszText = (WCHAR*)GlobalLock(hBlock);
				if (pwszText)
				{
					CopyMemory(pwszText, s, size);
					GlobalUnlock(hBlock);
				}
				SetClipboardData(CF_UNICODETEXT, hBlock);
			}
			CloseClipboard();
			// We must not free the object until CloseClipboard is called.
			if (hBlock)
				GlobalFree(hBlock);
		}
		break;

	case ID_LIST_CHAT:
		break;

	case ID_LIST_PLAYERS:
		if (idEvent == EVENT_SELECT)	// Selecting other player
		{
			if (g_pUI.getSelectedData(ID_LIST_PLAYERS) == g_acc)
				g_pUI.text(ID_STATIC_CHAT, L"Chat with ALL");	// Chat with ALL
			else
			{
				wchar_t	s[40];
				swprintf_s(s, 40, L"Chat with %s", g_pUI.getSelText(ID_LIST_PLAYERS));
				g_pUI.text(ID_STATIC_CHAT, s);	// Chat with player
				swprintf_s(g_sMsgTitle, 32, L"To %s: ", g_pUI.getSelIndex(ID_LIST_PLAYERS) ? g_pUI.getSelText(ID_LIST_PLAYERS) : L"ALL");	// To Recipient:
			}
		}
		break;

	case ID_EDIT_MESSAGE:	// Chat Message
		if (idEvent == EVENT_ENTER)	// if was pressed 'Enter'
			sendMessage();	// Send message
		break;

	case ID_BUTTON_SEND:	// Send
		sendMessage();	// Send message
		break;

	case ID_MESSAGE_WAIT:	// Close message with text of error
		if (idEvent == EVENT_CLICK)	// Clicked button Event
			g_pNet.stopConnect();
		break;

	case ID_MESSAGE_ERROR:	// Close message with text of error
		if (idEvent == EVENT_CLICK)	// Clicked button Event
			g_pUI.visible(ID_MESSAGE_ERROR, g_bMsgError = false);
		break;
	}
}


//--------------------------------------------------------------------------------------
// INITIALIZE
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	// Adding font
	g_pAppl.addFont(L"Arial", 16, FW_MEDIUM);	// Return ID_FONT_16 = 0, add font for text

	// Adding start window
	g_pUI.addWindow(ID_FONT_16, L"START CHAT", 510, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_MENU = 0
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_16, L"DataBase Name:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_SQL_NAME = 1
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_16, L"AccountAndChat0.44b", 200, 0.5f);	// DataBase name, return ID_EDIT_SQL_NAME = 2
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_16, L"SQL Server Name:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_SERVER = 3
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_16, L".\\sqlexpress", 200, 0.5f);	// SQL Server name, return ID_EDIT_SERVER = 4
	//g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_16, L"For SQL Server authentication:", DT_LEFT, COLOR_GOLD, 0.5f);
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_16, L"SQL Server Login:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_SQL_LOGIN = 5
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_16, L"", 200, 0.5f);	// Login for SQL Server authentication, return ID_EDIT_SQL_LOGIN = 6
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_16, L"SQL Server Password:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_SQL_PASS = 7
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_16, L"", 200, 0.5f);	// Password for SQL Server authentication, return ID_EDIT_SQL_PASS = 8
	g_pUI.setHide(ID_EDIT_SQL_PASS, true);							// Hide password
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_16, L"IP:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_IP = 9
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_16, L"127.0.0.1", 200, 0.5f);	// IP, return ID_EDIT_IP = 10
	g_pUI.setIP(ID_EDIT_IP, true);											// IP
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_16, L"Port:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_PORT = 11
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_16, L"1234", 200, 0.5f);	// Port, return ID_STATIC_PORT = 12
	g_pUI.setInt(ID_EDIT_PORT, 65535);									// Max Port = 65535
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_16, L"Name:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_LOGIN = 13
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_16, L"", 200, 0.5f);	// Name, return ID_EDIT_LOGIN = 14
	g_pUI.setMaxSymbols(ID_EDIT_LOGIN, 15);							// Name: max 15 symbols (16 with end of the string)
	g_pUI.setMaxPixels(ID_EDIT_LOGIN, 150);							// Name: max 150 pixels
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_16, L"Password:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_PASSWORD = 15
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_16, L"", 200, 0.5f);	// Password, return ID_EDIT_PASSWORD = 16
	g_pUI.setMaxSymbols(ID_EDIT_PASSWORD, 15);						// Password: max 15 symbols (16 with end of the string)
	g_pUI.setMaxPixels(ID_EDIT_PASSWORD, 150);						// Password: max 150 pixels
	g_pUI.setHide(ID_EDIT_PASSWORD, true);							// Hide password
	g_pUI.addButton(ID_WINDOW_MENU, ID_FONT_16, L"Server", 100, 3, 0.5f);	// Start server, return ID_BUTTON_SERVER = 17
	g_pUI.addButton(ID_WINDOW_MENU, ID_FONT_16, L"Connect", 100, 1, 0.5f);	// Connect to the Server, return ID_BUTTON_CONNECT = 18
	g_pUI.addButton(ID_WINDOW_MENU, ID_FONT_16, L"Registration", 100, 0, 0.5f);	// Registration on the Server, return ID_BUTTON_REG = 19

	// Adding registration window
	g_pUI.addWindow(ID_FONT_16, L"REGISTRATION", 510, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_REG = 20
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_16, L"Name:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_NAME = 21
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_16, L"", 200, 0.5f);	// Name, return ID_EDIT_NAME = 22
	g_pUI.setMaxSymbols(ID_EDIT_NAME, 15);							// Name: max 15 symbols (16 with end of the string)
	g_pUI.setMaxPixels(ID_EDIT_NAME, 150);							// Name: max 150 pixels
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_16, L"Password:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_PASS = 23
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_16, L"", 200, 0.5f);	// Password, return ID_EDIT_PASS = 24
	g_pUI.setMaxSymbols(ID_EDIT_PASS, 15);						// Password: max 15 symbols (16 with end of the string)
	g_pUI.setMaxPixels(ID_EDIT_PASS, 150);						// Password: max 150 pixels
	g_pUI.setHide(ID_EDIT_PASS, true);							// Hide password
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_16, L"E-mail:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_MAIL = 25
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_16, L"", 200, 0.5f);	// E-mail, return ID_EDIT_MAIL = 26
	g_pUI.setMaxSymbols(ID_EDIT_MAIL, 63);
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_16, L"Skype:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_SKYPE = 27
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_16, L"", 200, 0.5f);	// Skype, return ID_EDIT_SKYPE = 28
	g_pUI.setMaxSymbols(ID_EDIT_SKYPE, 63);
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_16, L"Country:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_COUNTRY = 29
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_16, L"", 330, 0.5f);	// Country, return ID_EDIT_COUNTRY = 30
	g_pUI.setMaxSymbols(ID_EDIT_COUNTRY, 63);
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_16, L"City:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_CITY = 31
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_16, L"", 330, 0.5f);	// City, return ID_EDIT_CITY = 32
	g_pUI.setMaxSymbols(ID_EDIT_CITY, 31);
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_16, L"Age:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_AGE = 33
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_16, L"0", 100, 0.5f);	// Age, return ID_EDIT_AGE = 34
	g_pUI.setInt(ID_EDIT_AGE, 99);									// Max Age = 99
	g_pUI.addRadioButton(ID_WINDOW_REG, ID_FONT_16, GROUP_GENDER, L"Male", COLOR_GOLD, 0.5f);	// Male, return ID_RADIO_MALE = 35
	g_pUI.addRadioButton(ID_WINDOW_REG, ID_FONT_16, GROUP_GENDER, L"Female", COLOR_GOLD, 0.5f);	// Female, return ID_RADIO_FEMALE = 36
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_16, L"About me:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_MYSELF = 37
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_16, L"", 330, 0.5f);	// About my self, return ID_EDIT_MYSELF = 38
	g_pUI.setMaxSymbols(ID_EDIT_MYSELF, 95);
	g_pUI.addButton(ID_WINDOW_REG, ID_FONT_16, L"Register", 100, 0, 0.5f);	// Register on the Server, return ID_BUTTON_REGISTER = 39

	// Adding chat window
	g_pUI.addWindow(ID_FONT_16, L"CHAT", 510, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_CHAT = 40
	g_pUI.addStatic(ID_WINDOW_CHAT, ID_FONT_16, L"Players list:", DT_CENTER, COLOR_GOLD, 0.5f);	// Return ID_STATIC_PLAYERS = 41
	g_pUI.addListBox(ID_WINDOW_CHAT, ID_FONT_16, 106, 15, 0.5f);	// List of players, return ID_LIST_PLAYERS = 42
	//g_pUI.set3Column(ID_LIST_PLAYERS, 50, 170);		// 3 column: ID, name, password
	g_pUI.addStatic(ID_WINDOW_CHAT, ID_FONT_16, L"Chat with ALL", DT_CENTER, COLOR_GOLD, 0.5f);	// Chat with ..., return ID_STATIC_CHAT = 43
	g_pUI.addListBox(ID_WINDOW_CHAT, ID_FONT_16, 300, 15, 0.5f);	// List of chat, return ID_LIST_CHAT = 44
	//g_pUI.set3Column(ID_LIST_ACCOUNTS, 50, 170);		// 3 column: account, name, number of messages
	g_pUI.addStatic(ID_WINDOW_CHAT, ID_FONT_16, L"Message:", DT_LEFT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_MESSAGE = 45
	g_pUI.addEditBox(ID_WINDOW_CHAT, ID_FONT_16, L"", 300, 0.5f);	// Edit message, return ID_EDIT_MESSAGE = 46
	g_pUI.setMaxSymbols(ID_EDIT_MESSAGE, 127);
	g_pUI.addStatic(ID_WINDOW_CHAT, ID_FONT_16, L"Guests: 0", DT_CENTER, COLOR_GOLD, 0.5f);	// Number of guests (not registered clients), return ID_STATIC_GUESTS = 47
	g_pUI.addButton(ID_WINDOW_CHAT, ID_FONT_16, L"Send", 106, 1, 0.5f);	// Send, return ID_BUTTON_SEND = 48
	
	g_pUI.addListBox(-1, ID_FONT_16, 320, 5, 0.4f);		// List of IP-addresses, return ID_LIST_IP = 49
	g_pUI.addButton(-1, ID_FONT_16, L"Exit", 100, 2, 0.4f);	// Exit, return ID_BUTTON_EXIT = 50
	g_pUI.addMessage(ID_FONT_16, L"Awaiting a response from the server.", L"Cancel", 500, 160, COLOR_GOLD, true, 0.3f);	// Return ID_MESSAGE_WAIT = 51
	g_pUI.addMessage(ID_FONT_16, L"", L"OK", 500, 160, COLOR_GOLD, true, 0.3f);	// Message with text of error, return ID_MESSAGE_ERROR = 52
	g_pUI.addListBox(-1, ID_FONT_16, 200, 15, 0.4f);	// List of accounts, return ID_LIST_ACC = 53
	g_pUI.addListBox(-1, ID_FONT_16, 800, 5, 0.4f);	// List of registrations, return ID_LIST_REG = 54

	ChangeState(MENU);	// Begin state - MENU
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.2f);	// 0 - add space for FPS
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.2f);	// 1 - add space for timer
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int w, int h)
{
	g_pText.pos(0, 5, 2);					// fps
	g_pText.pos(1, 105, 2);					// timer

	int hw = w / 2;
	int hh = h / 2;

	g_pUI.pos(ID_WINDOW_MENU, hw - 250, hh - 225);		// START CHAT
	g_pUI.pos(ID_STATIC_SQL_NAME, hw - 5, hh - 144);	// DataBase name:
	g_pUI.pos(ID_EDIT_SQL_NAME, hw, hh - 150);			// DataBase name
	g_pUI.pos(ID_STATIC_SERVER, hw - 5, hh - 109);		// SQL Server name:
	g_pUI.pos(ID_EDIT_SERVER, hw, hh - 115);			// SQL Server name
	g_pUI.pos(ID_STATIC_SQL_LOGIN, hw - 5, hh - 74);	// Login for SQL Server authentication:
	g_pUI.pos(ID_EDIT_SQL_LOGIN, hw, hh - 80);			// Login for SQL Server authentication
	g_pUI.pos(ID_STATIC_SQL_PASS, hw - 5, hh - 39);		// Password for SQL Server authentication:
	g_pUI.pos(ID_EDIT_SQL_PASS, hw, hh - 45);			// Password for SQL Server authentication
	g_pUI.pos(ID_STATIC_IP, hw - 5, hh - 4);			// IP
	g_pUI.pos(ID_EDIT_IP, hw, hh - 10);					// IP
	g_pUI.pos(ID_STATIC_PORT, hw - 5, hh + 31);			// Port
	g_pUI.pos(ID_EDIT_PORT, hw, hh + 25);				// Port
	g_pUI.pos(ID_STATIC_LOGIN, hw - 5, hh + 66);		// Name
	g_pUI.pos(ID_EDIT_LOGIN, hw, hh + 60);				// Name
	g_pUI.pos(ID_STATIC_PASSWORD, hw - 5, hh + 101);	// Password
	g_pUI.pos(ID_EDIT_PASSWORD, hw, hh + 95);			// Password
	g_pUI.pos(ID_BUTTON_SERVER, hw - 200, hh + 146);	// Start server
	g_pUI.pos(ID_BUTTON_CONNECT, hw - 50, hh + 146);	// Connect to the Server
	g_pUI.pos(ID_BUTTON_REG, hw + 100, hh + 146);		// Registration on the Server

	g_pUI.pos(ID_WINDOW_REG, hw - 250, hh - 225);		// REGISTRATION
	g_pUI.pos(ID_STATIC_NAME, hw - 135, hh - 144);		// Name
	g_pUI.pos(ID_EDIT_NAME, hw - 130, hh - 150);		// Name
	g_pUI.pos(ID_STATIC_PASS, hw - 135, hh - 109);		// Password
	g_pUI.pos(ID_EDIT_PASS, hw - 130, hh - 115);		// Password
	g_pUI.pos(ID_STATIC_MAIL, hw - 135, hh - 74);		// E-mail
	g_pUI.pos(ID_EDIT_MAIL, hw - 130, hh - 80);			// E-mail
	g_pUI.pos(ID_STATIC_SKYPE, hw - 135, hh - 39);		// Skype
	g_pUI.pos(ID_EDIT_SKYPE, hw - 130, hh - 45);		// Skype
	g_pUI.pos(ID_STATIC_COUNTRY, hw - 135, hh - 4);		// Country
	g_pUI.pos(ID_EDIT_COUNTRY, hw - 130, hh - 10);		// Country
	g_pUI.pos(ID_STATIC_CITY, hw - 135, hh + 31);		// City
	g_pUI.pos(ID_EDIT_CITY, hw - 130, hh + 25);			// City
	g_pUI.pos(ID_STATIC_AGE, hw - 135, hh + 66);		// Age
	g_pUI.pos(ID_EDIT_AGE, hw - 130, hh + 60);			// Age
	g_pUI.pos(ID_RADIO_MALE, hw, hh + 66);				// Male
	g_pUI.pos(ID_RADIO_FEMALE, hw + 100, hh + 66);		// Female
	g_pUI.pos(ID_STATIC_MYSELF, hw - 135, hh + 101);	// About my self
	g_pUI.pos(ID_EDIT_MYSELF, hw - 130, hh + 95);		// About my self
	g_pUI.pos(ID_BUTTON_REGISTER, hw - 50, hh + 146);	// Register on the Server

	g_pUI.pos(ID_WINDOW_CHAT, hw - 250, hh - 225);		// CHAT
	g_pUI.pos(ID_STATIC_PLAYERS, hw + 153, hh - 150);	// Players list
	g_pUI.pos(ID_LIST_PLAYERS, hw + 100, hh - 134);		// List of players
	g_pUI.pos(ID_STATIC_CHAT, hw - 103, hh - 150);		// Chat with ...
	g_pUI.pos(ID_LIST_CHAT, hw - 206, hh - 134);		// List of chat
	g_pUI.pos(ID_STATIC_MESSAGE, hw - 206, hh + 130);	// Message
	g_pUI.pos(ID_EDIT_MESSAGE, hw - 206, hh + 146);		// Edit message
	g_pUI.pos(ID_STATIC_GUESTS, hw + 153, hh + 118);	// Guests
	g_pUI.pos(ID_BUTTON_SEND, hw + 100, hh + 146);		// Send
	
	g_pUI.pos(ID_LIST_IP, hw - 160, 0);					// List of IP-addresses
	g_pUI.pos(ID_BUTTON_EXIT, w - 104, h - 36);			// Exit
	g_pUI.pos(ID_MESSAGE_WAIT, hw - 250, hh - 80);		// Wait
	g_pUI.pos(ID_MESSAGE_ERROR, hw - 250, hh - 80);		// Error
	g_pUI.pos(ID_LIST_ACC, hw + 300, hh - 134);			// List of accounts
	g_pUI.pos(ID_LIST_REG, hw - 400, hh + 208);			// List of registrations
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double, float)
{
	g_pText.fps(ID_FONT_16, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT_16, 1, DT_LEFT, COLOR_ORANGE);	// timer

	g_pAppl.clearScreen(g_cClear);

	// Render UI
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pUI.render();	// Render UI

	// Render texts
	g_pAppl.setRenderTexts();
	g_pText.draw(0, 2);	// Render texts
}




//--------------------------------------------------------------------------------------
// CONNECT TO THE NETWORK
//--------------------------------------------------------------------------------------
void CALLBACK onConnect(DWORD nid, WORD id, byte iAuthError)
{
	if (g_eState == SERVER)	// If server
	{
		// Check the registration on the server
		if (id > 20000 && iAuthError)
		{
			wchar_t s[16];
			swprintf_s(s, 16, L"Guests: %d", ++g_nGuests);
			g_pUI.text(ID_STATIC_GUESTS, s);
		}
		else
		{
			// Send new player params to all players
		}
	}
	else	// Client
	{
		g_nidServer = nid;	// Network Server ID
	}
}


//--------------------------------------------------------------------------------------
// DISCONNECT FROM THE NETWORK
//--------------------------------------------------------------------------------------
void CALLBACK onDisconnect(DWORD acc, byte nReason)	// NOTE: if authorization and chat has been added, then the function will use the account (DWORD acc)
{
	if (g_eState == SERVER)	// If server
	{
		if (acc)
		{
			UINT nPlayers = (UINT)g_pUI.getSize(ID_LIST_PLAYERS);
			for (UINT i = 0; i < nPlayers; i++)
				if (acc == (DWORD)g_pUI.getData(ID_LIST_PLAYERS, i))
					g_pUI.deleteItem(ID_LIST_PLAYERS, i);

			/// REPORT TO ALL PLAYERS about disconnected player - should be automatically
		}
		else
		{
			wchar_t s[16];
			swprintf_s(s, 16, L"Guests: %d", --g_nGuests);
			g_pUI.text(ID_STATIC_GUESTS, s);
		}
	}
	else	// Client
	{
		///Client is informed only about disconnected authorized player, about disconnected a guest is not reported
		if (nReason)
		{
			g_bMsgError = true;	// Flag of message with text of error
			g_pUI.textMsg(ID_MESSAGE_ERROR, L"No connect to the Server", L"", L"", L"");	// Message with text of error
			ChangeState(MENU);	// 
		}
		else
		{
			g_bMsgError = true;	// Flag of message with text of error
			g_pUI.textMsg(ID_MESSAGE_ERROR, L"Disconnect from the Server", L"", L"", L"");	// Message with text of error
			ChangeState(MENU);	// 
		}
	}
}


//--------------------------------------------------------------------------------------
// NETWORK MESSAGE EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onNetMsg(BYTE *msg)
{
	WORD iMsg;
	memcpy(&iMsg, msg, 2);	// ID message

	switch (iMsg) {
	case MSG_AUTHORIZE: {	// Authorize (valid)
		wchar_t	s[256];
		if (g_eState == SERVER)	// Player logged on to the server
		{
			WORD id;
			DWORD acc;
			memcpy(&id, msg + 2, 2);	// Player ID
			memcpy(&acc, msg + 4, 4);	// Player Account
			swprintf_s(s, 32, L"%d %s", acc, g_pDB.getName(acc));	// Account and name of authorized player
			g_pUI.insItem(ID_LIST_PLAYERS, id, s, acc);	// Insert item into the players list
		}
		else	// Validation
		{
			memcpy(&g_id, &msg[2], 2);		// Player ID
			memcpy(&g_acc, &msg[4], 4);		// Player Account
			memcpy(&g_data[2], &g_id, 2);	// Player/Guests ID
			//memcpy(&g_version, msg + 8, 2);	// Application version
			if (g_acc > 0)	// If authorized
			{
				swprintf_s(g_sMsgTitle, 32, L"To ALL: ");	// To Recipient:
				swprintf_s(s, 32, L"%d %s", g_acc, g_pUI.getText(ID_EDIT_LOGIN));	// Account and name of authorized player
				g_pUI.addItem(ID_LIST_PLAYERS, s, g_acc);	// Insert item into the players list
				g_pUI.selItem(ID_LIST_PLAYERS, 0);	// Select own account for chat with all
				ChangeState(CHAT);
			}
			else
			{
				if (g_sName[0])	// If the name was entered
				{
					g_bMsgError = true;
					switch (g_acc)
					{
					case -1: g_pUI.textMsg(ID_MESSAGE_ERROR, L"No this login.", L"", L"", L""); break;
					case -2: g_pUI.textMsg(ID_MESSAGE_ERROR, L"Password error.", L"", L"", L""); break;
					case -3: g_pUI.textMsg(ID_MESSAGE_ERROR, L"Already in the network.", L"", L"", L""); break;
					case -4: g_pUI.textMsg(ID_MESSAGE_ERROR, L"Playtime is over.", L"", L"", L""); break;
					case -5: g_pUI.textMsg(ID_MESSAGE_ERROR, L"Account banned.", L"", L"", L""); break;
					case -6: g_pUI.textMsg(ID_MESSAGE_ERROR, L"Incorrect game version.", L"", L"", L""); break;
					default: g_pUI.textMsg(ID_MESSAGE_ERROR, L"Unknown error.", L"", L"", L""); break;
					}
				}
				ChangeState(REG);
			}
		}
	}	break;

	case MSG_REGISTRATE:	// Registrate (valid)
		if (g_eState == SERVER)	// Player logged on to the server
		{
			WORD id;
			DWORD acc;
			memcpy(&id, msg + 2, 2);	// Player ID
			memcpy(&acc, msg + 4, 4);	// Player Account

			if (id < 20000)
			{
				ACCOUNT	Acc;	// Account item
				REGISTR	Reg;	// Registrations item
				Acc.acc = Reg.acc = acc;
				g_pDB.getAccount(id, &Acc, &Reg);

				wchar_t	s[256];
				swprintf_s(s, 256, L"%d %s", Acc.acc, Acc.sName);
				g_pUI.insItem(ID_LIST_PLAYERS, id, s, Acc.acc);	// Added to players list
				swprintf_s(s, 256, L"%d %s %s", Acc.acc, Acc.sName, Acc.sPass);
				g_pUI.addItem(ID_LIST_ACC, s, Acc.acc);	// Added to account list
				swprintf_s(s, 256, L"%d %s %s %s %s %d %s %s", Reg.acc, Reg.sMail, Reg.sSkype, Reg.sCountry, Reg.sCity, Reg.nAgeSex % 128, Reg.nAgeSex > 127 ? L"Male" : L"Female", Reg.sMyself);
				g_pUI.addItem(ID_LIST_REG, s, Reg.acc);	// Added to registration list

				swprintf_s(s, 16, L"Guests: %d", --g_nGuests);	// Take away one guest
				g_pUI.text(ID_STATIC_GUESTS, s);
			}
		}
		else	// Validation
		{
			memcpy(&g_id, msg + 2, 2);		// Player ID
			memcpy(&g_acc, msg + 4, 4);		// Player Account
			memcpy(&g_data[2], &g_id, 2);	// Player/Guests ID
			//memcpy(&g_version, msg + 8, 2);	// Application version
			if (g_id < 20000)	// If authorized
			{
				swprintf_s(g_sMsgTitle, 32, L"To ALL: ");	// To Recipient:
				swprintf_s(g_sName, 16, g_pUI.getText(ID_EDIT_NAME));
				wchar_t	s[32];
				swprintf_s(s, 32, L"%d %s", g_acc, g_pUI.getText(ID_EDIT_NAME));	// Account and name of authorized player
				g_pUI.addItem(ID_LIST_PLAYERS, s, g_acc);	// Insert item into the players list
				g_pUI.selItem(ID_LIST_PLAYERS, 0);	// Select own account for chat with all
				ChangeState(CHAT);
			}
			else
			{
				g_bMsgError = true;
				switch (g_id)
				{
				case -1: g_pUI.textMsg(ID_MESSAGE_ERROR, L"Busy login.", L"", L"", L""); break;
				case -2: g_pUI.textMsg(ID_MESSAGE_ERROR, L"Invalid login.", L"", L"", L""); break;
				case -3: g_pUI.textMsg(ID_MESSAGE_ERROR, L"Invalid password.", L"", L"", L""); break;
				case -4: g_pUI.textMsg(ID_MESSAGE_ERROR, L"Invalid e-mail.", L"", L"", L""); break;
				default: g_pUI.textMsg(ID_MESSAGE_ERROR, L"Unknown error.", L"", L"", L""); break;
				}
				ChangeState(REG);
			}
		}
		break;

	case MSG_INFOPLAYER: {	// Other player params (only on the client)
		DWORD acc;
		wchar_t	sName[16], s[32];
		memcpy(&acc, msg + 2, 4);	// Player Account
		memcpy(&sName, msg + 6, 32);	// Player Name
		swprintf_s(s, 32, L"%d %s", acc, sName);
		g_pUI.addItem(ID_LIST_PLAYERS, s, acc);	// Added to players list
	}	break;

	case MSG_INFOPLAYERS: {	// Other players params (only on the client)
		DWORD acc, iData = 3;
		wchar_t	sName[16], s[32];
		BYTE nAuth = msg[2], len;
		for (BYTE i = 0; i < nAuth; i++)
		{
			memcpy(&acc, msg + iData, 4);	iData += 4;	// Account
			len = msg[iData++];
			memcpy(&sName, msg + iData, len);	iData += len + 4;	// Name (4 bytes rating is not used in this sample)
			sName[len >> 1] = L'\0';
			swprintf_s(s, 32, L"%i %s", acc, sName);
			g_pUI.addItem(ID_LIST_PLAYERS, s, acc);	// Added to players list
		}
	}	break;

	case MSG_CHATSER: {	// Chat message from the client to the server
		WORD idSender;
		DWORD accRecipient;
		wchar_t	sMsg[128], sMessage[162];
		memcpy(&idSender, &msg[2], 2);	// Sender ID
		memcpy(&accRecipient, &msg[4], 4);	// Recipient ACC
		BYTE len = msg[8];
		memcpy(sMsg, &msg[9], len);	// Message string
		sMsg[len >> 1] = L'\0';
		swprintf_s(sMessage, 162, L"From %s to %s: %s", g_pDB.getNameID(idSender), accRecipient == -1 ? L"ALL" : g_pDB.getName(accRecipient), sMsg);
		g_pUI.addItem(ID_LIST_CHAT, sMessage, 0);	// 
	}	break;

	case MSG_CHAT: {	// Chat message from/through the server to the client
		DWORD accSender;
		wchar_t	sMsg[128], sMessage[162];
		__time32_t	t;
		struct	tm	ts;
		memcpy(&accSender, &msg[2], 4);	// Sender ACC
		memcpy(&t, &msg[6], 4);	_localtime32_s(&ts, &t);	// 	// Server time
		bool bPersonal = msg[10] % 2 ? true : false;
		BYTE len = bPersonal ? --msg[10] : msg[10];
		memcpy(sMsg, &msg[11], len);	// Message string
		sMsg[len >> 1] = L'\0';

		////// Find Sender
		swprintf_s(sMessage, 162, L"%02i:%02i:%02i  from %i to %s: %s", ts.tm_hour, ts.tm_min, ts.tm_sec, accSender, bPersonal ? L"ME" : L"ALL", sMsg);
		g_pUI.addItem(ID_LIST_CHAT, sMessage, 0);	// 
	}	break;

	case MSG_CHAT_VALID: {	// Chat message valid
		__time32_t	t;
		struct	tm	ts;
		wchar_t	s[192];
		memcpy(&t, &msg[2], 4);	_localtime32_s(&ts, &t);	// 	// Server time
		swprintf_s(s, 192, L"%02i:%02i:%02i  %s", ts.tm_hour, ts.tm_min, ts.tm_sec, g_sMessage);
		g_pUI.addItem(ID_LIST_CHAT, s, 0);	// 
	}	break;

	case MSG_CHATBLOCK:	// Chat
		BYTE iChat = msg[2];	// Number of chat messages
		if (iChat)	// If chat message to come
		{
			size_t	iData = 3;
			BYTE	len;
			wchar_t	sName[16];
			struct	tm	ts;
			wchar_t	s[192], sMsg[128];
			BYTE	iType;
			__time32_t	t;

			for (BYTE i = 0; i < iChat; i++)
			{
				memcpy(&t, &msg[iData], 4);	iData += 4;	// Time of getting messages on the server
				_localtime32_s(&ts, &t);	// Time of message
				iType = msg[iData++];	// Type of message
				if (iType != 2)	// If do not send all
				{
					len = msg[iData++];
					memcpy(sName, &msg[iData], len);	iData += len;	// The name of the sender/receiver
					sName[len >> 1] = L'\0';
				}
				len = msg[iData++];// << 1;
				memcpy(sMsg, &msg[iData], len);	iData += len;	// Message
				sMsg[len >> 1] = L'\0';
				switch (iType)
				{
				case 3:	swprintf_s(s, 192, L"%02i:%02i:%02i  to %s:  %s", ts.tm_hour, ts.tm_min, ts.tm_sec, sName, sMsg);			break;	// Sending a message personally
				case 2:	swprintf_s(s, 192, L"%02i:%02i:%02i  to ALL:  %s", ts.tm_hour, ts.tm_min, ts.tm_sec, sMsg);					break;	// Sending a message to all
				case 1: swprintf_s(s, 192, L"%02i:%02i:%02i  from %s:  %s", ts.tm_hour, ts.tm_min, ts.tm_sec, sName, sMsg);			break;	// Receiving a personal message
				case 0: swprintf_s(s, 192, L"%02i:%02i:%02i  from %s to ALL:  %s", ts.tm_hour, ts.tm_min, ts.tm_sec, sName, sMsg);	break;	// Receiving message for all
				}
				g_pUI.addItem(ID_LIST_CHAT, s, 0);	// 
			}
		}

		break;
	}

}
