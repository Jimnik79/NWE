//--------------------------------------------------------------------------------------
// File: 22 Admin Panel.cpp
// Description: Sample of work with Admin Panel.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with Admin Panel:
// - create;
// - add and delete data.

// The client every frame sends to the server a test message to create the appearance of the network load

// Additional job trainee (consolidate the material) to complete the work with the field "Unknown".

// In the next version of SDK, maybe, will be added the ban and removing the ban.
// In the next version of SDK, maybe, will be added the ban and removing the ban.
// Much later, maybe, will be added analysis with SQL Server OLAP cubes.



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
//#define	ID_BUTTON_REG		19	// Registraion on the Server

#define	ID_WINDOW_PANEL		19	// ADMIN PANEL
#define	ID_STATIC_IPLIST	20	// List of IP-addresses
#define	ID_LIST_IP			21	// List of IP-addresses
#define	ID_STATIC_PLAYERS	22	// List of players (guests N)
#define	ID_LIST_PLAYERS		23	// List of players
#define	ID_STATIC_ACC		24	// List of accounts
#define	ID_LIST_ACC			25	// List of accounts
#define	ID_STATIC_REG		26	// List of registrations
#define	ID_LIST_REG			27	// List of registrations
#define	ID_STATIC_CHAT		28	// Chat with ...
#define	ID_LIST_CHAT		29	// List of chat
#define	ID_STATIC_MESSAGE	30	// Message:
#define	ID_EDIT_MESSAGE		31	// Edit message
#define	ID_BUTTON_CHAT_SEND	32	// Send
#define	ID_BUTTON_CHAT_UPDT	33	// Update
#define	ID_BUTTON_CHAT_DEL	34	// Delete

#define	ID_STATIC_REGPAR	35	// Registration params
#define	ID_STATIC_ID		36	// ID:
#define	ID_EDIT_ID			37	// ID
#define	ID_STATIC_NAME		38	// Name:
#define	ID_EDIT_NAME		39	// Name
#define	ID_STATIC_PASS		40	// Password:
#define	ID_EDIT_PASS		41	// Password
#define	ID_STATIC_MAIL		42	// E-mail:
#define	ID_EDIT_MAIL		43	// E-mail
#define	ID_STATIC_SKYPE		44	// Skype:
#define ID_EDIT_SKYPE		45	// Skype
#define	ID_STATIC_COUNTRY	46	// Country:
#define ID_EDIT_COUNTRY		47	// Country
#define	ID_STATIC_CITY		48	// City:
#define ID_EDIT_CITY		49	// City
#define	ID_STATIC_AGE		50	// Age:
#define ID_EDIT_AGE			51	// Age
#define ID_RADIO_MALE		52	// Male
#define ID_RADIO_FEMALE		53	// Female
#define	ID_STATIC_MYSELF	54	// About my self:
#define ID_EDIT_MYSELF		55	// About my self
#define	ID_BUTTON_REG_NEW	56	// New
#define	ID_BUTTON_REG_UPDT	57	// Update on the Server
#define	ID_BUTTON_REG_DEL	58	// Delete
#define	ID_STATIC_UNIQUE	59	// Unique client params
#define	ID_BUTTON_MONITOR	60	// Monitor

#define	ID_WINDOW_PARAMS	61	// CLIENT PARAMS
#define	ID_STATIC_STRING	62	// String:
#define	ID_EDIT_STRING		63	// String
#define	ID_STATIC_FLOAT		64	// Float:
#define	ID_EDIT_FLOAT		65	// Float
#define	ID_STATIC_SYSTIME	66	// SYSTEMTIME:
#define	ID_EDIT_SYSTIME		67	// SYSTEMTIME
#define	ID_STATIC_INT64		68	// __int64:
#define	ID_EDIT_INT64		69	// __int64
#define	ID_STATIC_SHORT		70	// Short:
#define	ID_EDIT_SHORT		71	// Short
#define	ID_STATIC_BYTE		72	// Byte:
#define	ID_EDIT_BYTE		73	// Byte
#define	ID_STATIC_TIME		74	// __time32_t:
#define	ID_EDIT_TIME		75	// __time32_t
#define	ID_STATIC_DOUBLE	76	// Double:
#define	ID_EDIT_DOUBLE		77	// Double
#define	ID_STATIC_UNKNOWN	78	// Unknown:
#define	ID_EDIT_UNKNOWN		79	// Unknown
#define	ID_STATIC_UNKNOWN2	80	// Unknown2:
#define	ID_EDIT_UNKNOWN2	81	// Unknown2
#define	ID_BUTTON_PAR_UPDT	82	// Update params on the Server

#define	ID_CHART			83	// Chart for traffic

#define	ID_BUTTON_EXIT		84	// Exit
#define	ID_MESSAGE_WAIT		85	// Wait
#define	ID_MESSAGE_ERROR	86	// Message with text of error

#define	ID_FONT_16			0

#define	GROUP_GENDER		0

#define	TABLE_CLIENT_PARAMS	3

#define	ID_SERIES_RED		0	// Red line on the chart
#define	ID_SERIES_GREEN		1	// Green line on the chart
#define	ID_SERIES_BLUE		2	// Blue line on the chart

enum state	// State machine
{
	MENU,
	//REG,
	WAIT,
	SERVER,
	CLIENT,
};
enum state g_eState = MENU;	// State machine

float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background

UINT	g_nidServer = 0;// Network Server ID
WORD	g_id = 0;		// Player ID
int		g_acc = 0;		// Player Account: -2 - an incorrect login, -3 - an incorrect password, -4 - the player has already been activated, -5 - ended billable time

BYTE	g_data[1000];	// Message type (2 bytes); Id of player (0 - server message)(2 bytes)
bool	g_bMsgError = false;	// Flag of message with text of error
UINT	g_nGuests = 0;	// Number of guests (not registered clients)
wchar_t	g_sName[16];	// The player name

int		g_hw = 512;		// Half of Width
int		g_hh = 300;		// Half of Height

bool	g_bMonitor = false;	// Flag of traffic monitoring on the server


void ChangeState(state eState)
{
	g_eState = eState;	// New state

	g_pUI.visible(ID_WINDOW_MENU, eState == MENU);		// START CHAT
	g_pUI.visible(ID_MESSAGE_WAIT, eState == WAIT);		// WAIT
	g_pUI.visible(ID_WINDOW_PANEL, eState == SERVER);	// ADMIN PANEL
	g_pUI.visible(ID_WINDOW_PARAMS, eState == CLIENT);	// CLIENT PARAMS
	g_pUI.visible(ID_MESSAGE_ERROR, g_bMsgError);		// ERROR
	g_pUI.visible(ID_LIST_IP, eState == SERVER);		// List of IP-addresses
	g_pUI.visible(ID_LIST_ACC, eState == SERVER);		// List of accounts
	g_pUI.visible(ID_LIST_REG, eState == SERVER);		// List of registrations
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
			swprintf_s(sMessage, 162, L"To %d: %s", g_pUI.getSelectedData(ID_LIST_PLAYERS), sMsg);
			g_pUI.addItem(ID_LIST_CHAT, sMessage, 0);	// 
			g_pNet.sendChatMsgToClient(g_pUI.getSelectedData(ID_LIST_PLAYERS), sMsg);
		}
		else	// Client: send to the server
		{
			//swprintf_s(g_sMessage, 162, L"%s%s", g_sMsgTitle, sMsg);
			g_pNet.sendChatMsgToServer(g_pUI.getSelectedData(ID_LIST_PLAYERS), sMsg);
		}
	}
	g_pUI.text(ID_EDIT_MESSAGE, L"");	// Clear message editbox
}


//--------------------------------------------------------------------------------------
// SET POSITIONS FOR UNIQUE CLIENT PARAMS
//--------------------------------------------------------------------------------------
void setPosParams()
{
	g_pUI.pos(ID_STATIC_STRING, g_hw + 325, g_hh - 214);	// String
	g_pUI.pos(ID_EDIT_STRING, g_hw + 330, g_hh - 220);		// String
	g_pUI.pos(ID_STATIC_FLOAT, g_hw + 325, g_hh - 184);		// Float
	g_pUI.pos(ID_EDIT_FLOAT, g_hw + 330, g_hh - 190);		// Float
	g_pUI.pos(ID_STATIC_SYSTIME, g_hw + 325, g_hh - 154);	// SYSTEMTIME
	g_pUI.pos(ID_EDIT_SYSTIME, g_hw + 330, g_hh - 160);		// SYSTEMTIME
	g_pUI.pos(ID_STATIC_INT64, g_hw + 325, g_hh - 124);		// __int64
	g_pUI.pos(ID_EDIT_INT64, g_hw + 330, g_hh - 130);		// __int64
	g_pUI.pos(ID_STATIC_SHORT, g_hw + 325, g_hh - 94);		// Short
	g_pUI.pos(ID_EDIT_SHORT, g_hw + 330, g_hh - 100);		// Short
	g_pUI.pos(ID_STATIC_BYTE, g_hw + 325, g_hh - 64);		// Byte
	g_pUI.pos(ID_EDIT_BYTE, g_hw + 330, g_hh - 70);			// Byte
	g_pUI.pos(ID_STATIC_TIME, g_hw + 325, g_hh - 34);		// __time32_t
	g_pUI.pos(ID_EDIT_TIME, g_hw + 330, g_hh - 40);			// __time32_t
	g_pUI.pos(ID_STATIC_DOUBLE, g_hw + 325, g_hh - 4);		// Double
	g_pUI.pos(ID_EDIT_DOUBLE, g_hw + 330, g_hh - 10);		// Double
	g_pUI.pos(ID_STATIC_UNKNOWN, g_hw + 325, g_hh + 26);	// Unknown
	g_pUI.pos(ID_EDIT_UNKNOWN, g_hw + 330, g_hh + 20);		// Unknown
	g_pUI.pos(ID_STATIC_UNKNOWN2, g_hw + 325, g_hh + 56);	// Unknown2
	g_pUI.pos(ID_EDIT_UNKNOWN2, g_hw + 330, g_hh + 50);		// Unknown2
	g_pUI.pos(ID_BUTTON_PAR_UPDT, g_hw + 300, g_hh + 82);	// Update params on the Server
}

void changeMonitor()
{
	g_bMonitor = !g_bMonitor;	// Change flag of traffic monitoring on the server

	g_pUI.visible(ID_STATIC_REG, !g_bMonitor);		// List of registrations
	g_pUI.visible(ID_LIST_REG, !g_bMonitor);		// List of registrations
	g_pUI.visible(ID_STATIC_CHAT, !g_bMonitor);		// Chat with ...
	g_pUI.visible(ID_LIST_CHAT, !g_bMonitor);		// List of chat
	g_pUI.visible(ID_STATIC_MESSAGE, !g_bMonitor);	// Message:
	g_pUI.visible(ID_EDIT_MESSAGE, !g_bMonitor);	// Edit message
	g_pUI.visible(ID_BUTTON_CHAT_SEND, !g_bMonitor);// Send
	g_pUI.visible(ID_BUTTON_CHAT_UPDT, !g_bMonitor);// Update
	g_pUI.visible(ID_BUTTON_CHAT_DEL, !g_bMonitor);	// Delete

	g_pUI.visible(ID_STATIC_REGPAR, !g_bMonitor);	// Registration params
	g_pUI.visible(ID_STATIC_ID, !g_bMonitor);		// ID:
	g_pUI.visible(ID_EDIT_ID, !g_bMonitor);			// ID
	g_pUI.visible(ID_STATIC_NAME, !g_bMonitor);		// Name:
	g_pUI.visible(ID_EDIT_NAME, !g_bMonitor);		// Name
	g_pUI.visible(ID_STATIC_PASS, !g_bMonitor);		// Password:
	g_pUI.visible(ID_EDIT_PASS, !g_bMonitor);		// Password
	g_pUI.visible(ID_STATIC_MAIL, !g_bMonitor);		// E-mail:
	g_pUI.visible(ID_EDIT_MAIL, !g_bMonitor);		// E-mail
	g_pUI.visible(ID_STATIC_SKYPE, !g_bMonitor);	// Skype:
	g_pUI.visible(ID_EDIT_SKYPE, !g_bMonitor);		// Skype
	g_pUI.visible(ID_STATIC_COUNTRY, !g_bMonitor);	// Country:
	g_pUI.visible(ID_EDIT_COUNTRY, !g_bMonitor);	// Country
	g_pUI.visible(ID_STATIC_CITY, !g_bMonitor);		// City:
	g_pUI.visible(ID_EDIT_CITY, !g_bMonitor);		// City
	g_pUI.visible(ID_STATIC_AGE, !g_bMonitor);		// Age:
	g_pUI.visible(ID_EDIT_AGE, !g_bMonitor);		// Age
	g_pUI.visible(ID_RADIO_MALE, !g_bMonitor);		// Male
	g_pUI.visible(ID_RADIO_FEMALE, !g_bMonitor);	// Female
	g_pUI.visible(ID_STATIC_MYSELF, !g_bMonitor);	// About my self:
	g_pUI.visible(ID_EDIT_MYSELF, !g_bMonitor);		// About my self
	g_pUI.visible(ID_BUTTON_REG_NEW, !g_bMonitor);	// New
	g_pUI.visible(ID_BUTTON_REG_UPDT, !g_bMonitor);	// Update on the Server
	g_pUI.visible(ID_BUTTON_REG_DEL, !g_bMonitor);	// Delete
	g_pUI.visible(ID_STATIC_UNIQUE, !g_bMonitor);	// Unique client params

	g_pUI.visible(ID_STATIC_STRING, !g_bMonitor);	// String:
	g_pUI.visible(ID_EDIT_STRING, !g_bMonitor);		// String
	g_pUI.visible(ID_STATIC_FLOAT, !g_bMonitor);	// Float:
	g_pUI.visible(ID_EDIT_FLOAT, !g_bMonitor);		// Float
	g_pUI.visible(ID_STATIC_SYSTIME, !g_bMonitor);	// SYSTEMTIME:
	g_pUI.visible(ID_EDIT_SYSTIME, !g_bMonitor);	// SYSTEMTIME
	g_pUI.visible(ID_STATIC_INT64, !g_bMonitor);	// __int64:
	g_pUI.visible(ID_EDIT_INT64, !g_bMonitor);		// __int64
	g_pUI.visible(ID_STATIC_SHORT, !g_bMonitor);	// Short:
	g_pUI.visible(ID_EDIT_SHORT, !g_bMonitor);		// Short
	g_pUI.visible(ID_STATIC_BYTE, !g_bMonitor);		// Byte:
	g_pUI.visible(ID_EDIT_BYTE, !g_bMonitor);		// Byte
	g_pUI.visible(ID_STATIC_TIME, !g_bMonitor);		// __time32_t:
	g_pUI.visible(ID_EDIT_TIME, !g_bMonitor);		// __time32_t
	g_pUI.visible(ID_STATIC_DOUBLE, !g_bMonitor);	// Double:
	g_pUI.visible(ID_EDIT_DOUBLE, !g_bMonitor);		// Double
	g_pUI.visible(ID_STATIC_UNKNOWN, !g_bMonitor);	// Unknown:
	g_pUI.visible(ID_EDIT_UNKNOWN, !g_bMonitor);	// Unknown
	g_pUI.visible(ID_STATIC_UNKNOWN2, !g_bMonitor);	// Unknown2:
	g_pUI.visible(ID_EDIT_UNKNOWN2, !g_bMonitor);	// Unknown2
	g_pUI.visible(ID_BUTTON_PAR_UPDT, !g_bMonitor);	// Update params on the Server

	g_pUI.visible(ID_CHART, g_bMonitor);			// Chart for traffic

	g_pUI.text(ID_BUTTON_MONITOR, g_bMonitor ? L"Back" : L"Traffic Monitoring");	// Traffic Monitoring

	////// Change the button color to red
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
	return g_pAppl.run(L"22 Admin Panel", 1024, 600);
}


//--------------------------------------------------------------------------------------
// GUI EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onGUI(UINT idEvent, int idEl)
{
	switch (idEl)
	{
	case ID_BUTTON_SERVER: {	// Start Server
		
		g_pUI.setParentWindow(ID_STATIC_STRING, ID_WINDOW_PANEL);	// String
		g_pUI.setParentWindow(ID_EDIT_STRING, ID_WINDOW_PANEL);		// String
		g_pUI.setParentWindow(ID_STATIC_FLOAT, ID_WINDOW_PANEL);	// Float:
		g_pUI.setParentWindow(ID_EDIT_FLOAT, ID_WINDOW_PANEL);		// Float
		g_pUI.setParentWindow(ID_STATIC_SYSTIME, ID_WINDOW_PANEL);	// SYSTEMTIME:
		g_pUI.setParentWindow(ID_EDIT_SYSTIME, ID_WINDOW_PANEL);	// SYSTEMTIME
		g_pUI.setParentWindow(ID_STATIC_INT64, ID_WINDOW_PANEL);	// __int64:
		g_pUI.setParentWindow(ID_EDIT_INT64, ID_WINDOW_PANEL);		// __int64
		g_pUI.setParentWindow(ID_STATIC_SHORT, ID_WINDOW_PANEL);	// Short:
		g_pUI.setParentWindow(ID_EDIT_SHORT, ID_WINDOW_PANEL);		// Short
		g_pUI.setParentWindow(ID_STATIC_BYTE, ID_WINDOW_PANEL);		// Byte:
		g_pUI.setParentWindow(ID_EDIT_BYTE, ID_WINDOW_PANEL);		// Byte
		g_pUI.setParentWindow(ID_STATIC_TIME, ID_WINDOW_PANEL);		// __time32_t:
		g_pUI.setParentWindow(ID_EDIT_TIME, ID_WINDOW_PANEL);		// __time32_t
		g_pUI.setParentWindow(ID_STATIC_DOUBLE, ID_WINDOW_PANEL);	// Double:
		g_pUI.setParentWindow(ID_EDIT_DOUBLE, ID_WINDOW_PANEL);		// Double
		g_pUI.setParentWindow(ID_STATIC_UNKNOWN, ID_WINDOW_PANEL);	// Unknown:
		g_pUI.setParentWindow(ID_EDIT_UNKNOWN, ID_WINDOW_PANEL);	// Unknown
		g_pUI.setParentWindow(ID_STATIC_UNKNOWN2, ID_WINDOW_PANEL);	// Unknown2:
		g_pUI.setParentWindow(ID_EDIT_UNKNOWN2, ID_WINDOW_PANEL);	// Unknown2
		g_pUI.setParentWindow(ID_BUTTON_PAR_UPDT, ID_WINDOW_PANEL);	// Update params on the Server

		setPosParams();	// Set positions for unique client params

		wchar_t s[256];
		g_acc = g_id = 0;	// Admin ID and account
		memcpy(&g_data[2], &g_id, 2);	// Player ID

		// Add work with account and chat on the server (DB tables and protocol), Connect to SQL Server (with Accounts and Chat), Activate the server database, Return number of accounts in the DataBase
		g_pNet.setLoginAlways(true);	// Support always login on the server
		g_pDB.addAccountAndChat();	// Add fields and tables for work with accounts and chat (must be used prior to the addition of other tables; add 3 table: Accounts, Registration, Chat)

		// Table of client params
		g_pDB.addField(DB_INT, L"ID", 0);		// ID of client
		g_pDB.addField(DB_WCHAR, L"Str", 256);	// wchar_t[]
		g_pDB.addField(DB_FLOAT, L"Float", 0);	// float
		g_pDB.addField(DB_SYSTIME, L"Time", 0);	// SYSTEMTIME
		g_pDB.addField(DB_INT64, L"Int64", 0);	// __int64
		g_pDB.addField(DB_SHORT, L"Short", 0);	// short
		g_pDB.addField(DB_BYTE, L"Byte", 0);	// byte
		g_pDB.addField(DB_INT, L"Time32", 0);	// __time32_t
		g_pDB.addField(DB_DOUBLE, L"Float8", 0);// double	// Name of "Double" can not be used
		g_pDB.addTable(L"Params", 1);	// Return TABLE_CLIENT_PARAMS = 3

		UINT nAcc = g_pDB.connect(g_pUI.getText(ID_EDIT_SQL_NAME), g_pUI.getText(ID_EDIT_SERVER), g_pUI.getText(ID_EDIT_SQL_LOGIN), g_pUI.getText(ID_EDIT_SQL_PASS));

		g_pDB.activateAccountAndChat(nAcc);	// Activate accounts and chat on the database server

		//UINT nAcc = g_pNet.addAccountAndChat(g_pUI.getText(ID_EDIT_SQL_NAME), g_pUI.getText(ID_EDIT_SERVER), g_pUI.getText(ID_EDIT_SQL_LOGIN), g_pUI.getText(ID_EDIT_SQL_PASS));

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

		////// If connected to the server, simply send login and password to check
		g_pNet.connectToServer(g_pUI.getText(ID_EDIT_IP), (WORD)port, g_pUI.getText(ID_EDIT_LOGIN), g_pUI.getText(ID_EDIT_PASSWORD));	// Connect to the Server
		swprintf_s(g_sName, 16, g_pUI.getText(ID_EDIT_LOGIN));
		ChangeState(WAIT);	// Go to the state WAIT
		break;
	}

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

	case ID_BUTTON_MONITOR:	// Go to the traffic monitoring
		changeMonitor();
		break;

	case ID_LIST_CHAT: {
		MESSAGE pMsg = { g_pUI.getSelectedData(ID_LIST_CHAT) };	// Chat message with ID from the chat list
		g_pDB.getItem(ID_LIST_CHAT, (BYTE*)&pMsg);	// Get chat message from the DB using ID
	}	break;

	case ID_LIST_PLAYERS:
		break;

	case ID_BUTTON_CHAT_SEND:	// Send Chat Message
		sendMessage();	// Send message
		break;

	case ID_BUTTON_CHAT_UPDT: {	// Update Chat Message
		MESSAGE pMsg = { g_pUI.getSelectedData(ID_LIST_CHAT) };	// Chat message with ID from the chat list
		g_pDB.getItem(ID_LIST_CHAT, (BYTE*)&pMsg);	// Get chat message from the DB using ID
		swprintf_s(pMsg.str, 128, g_pUI.getText(ID_EDIT_MESSAGE));
		g_pDB.update(ID_LIST_CHAT, (BYTE*)&pMsg);
	}	break;

	case ID_BUTTON_CHAT_DEL:	// Delete Chat Message
		g_pDB.remove(ID_LIST_CHAT, g_pUI.getSelectedData(ID_LIST_CHAT));
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
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_16, L"AdminPanel0.44b", 200, 0.5f);	// DataBase name, return ID_EDIT_SQL_NAME = 2
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
	//g_pUI.addButton(ID_WINDOW_MENU, ID_FONT_16, L"Registration", 100, 0, 0.5f);	// Registration on the Server, return ID_BUTTON_REG = 19

	// Adding admin panel
	g_pUI.addWindow(ID_FONT_16, L"ADMIN PANEL", 1074, 650, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_PANEL = 20
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"IP addresses:", DT_CENTER, COLOR_GOLD, 0.5f);	// Return ID_STATIC_IPLIST = 21
	g_pUI.addListBox(ID_WINDOW_PANEL, ID_FONT_16, 200, 5, 0.4f);	// List of IP-addresses, return ID_LIST_IP = 22
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"Players list (guests 0):", DT_CENTER, COLOR_GOLD, 0.5f);	// Return ID_STATIC_PLAYERS = 23
	g_pUI.addListBox(ID_WINDOW_PANEL, ID_FONT_16, 200, 12, 0.5f);	// List of players, return ID_LIST_PLAYERS = 24
	//g_pUI.set3Column(ID_LIST_PLAYERS, 50, 170);		// 3 column: ID, name, password
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"List of accounts:", DT_CENTER, COLOR_GOLD, 0.5f);	// Return ID_STATIC_ACC = 25
	g_pUI.addListBox(ID_WINDOW_PANEL, ID_FONT_16, 200, 8, 0.4f);	// List of accounts, return ID_LIST_ACC = 26
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"List of registrations:", DT_LEFT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_REG = 27
	g_pUI.addListBox(ID_WINDOW_PANEL, ID_FONT_16, 760, 8, 0.4f);	// List of registrations, return ID_LIST_REG = 28
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"Chat with ALL", DT_CENTER, COLOR_GOLD, 0.5f);	// Chat with ..., return ID_STATIC_CHAT = 29
	g_pUI.addListBox(ID_WINDOW_PANEL, ID_FONT_16, 270, 16, 0.5f);	// List of chat, return ID_LIST_CHAT = 30
	//g_pUI.set3Column(ID_LIST_ACCOUNTS, 50, 170);		// 3 column: account, name, number of messages
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"Message:", DT_LEFT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_MESSAGE = 31
	g_pUI.addEditBox(ID_WINDOW_PANEL, ID_FONT_16, L"", 270, 0.5f);	// Edit message, return ID_EDIT_MESSAGE = 32
	g_pUI.setMaxSymbols(ID_EDIT_MESSAGE, 127);
	//g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"Guests: 0", DT_CENTER, COLOR_GOLD, 0.5f);	// Number of guests (not registered clients), return ID_STATIC_GUESTS = 47
	g_pUI.addButton(ID_WINDOW_PANEL, ID_FONT_16, L"Send", 75, 1, 0.5f);	// Send, return ID_BUTTON_CHAT_SEND = 33
	g_pUI.addButton(ID_WINDOW_PANEL, ID_FONT_16, L"Update", 75, 0, 0.5f);	// Update, return ID_BUTTON_CHAT_UPDT = 34
	g_pUI.addButton(ID_WINDOW_PANEL, ID_FONT_16, L"Delete", 75, 2, 0.5f);	// Delete, return ID_BUTTON_CHAT_DEL = 35

	// Adding registration params
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"Registration params", DT_LEFT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_REGPAR = 35
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"ID:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_ID = 36
	g_pUI.addEditBox(ID_WINDOW_PANEL, ID_FONT_16, L"", 150, 0.5f);	// Name, return ID_EDIT_ID = 37
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"Name:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_NAME = 38
	g_pUI.addEditBox(ID_WINDOW_PANEL, ID_FONT_16, L"", 150, 0.5f);	// Name, return ID_EDIT_NAME = 39
	g_pUI.setMaxSymbols(ID_EDIT_NAME, 15);							// Name: max 15 symbols (16 with end of the string)
	g_pUI.setMaxPixels(ID_EDIT_NAME, 150);							// Name: max 150 pixels
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"Password:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_PASS = 40
	g_pUI.addEditBox(ID_WINDOW_PANEL, ID_FONT_16, L"", 150, 0.5f);	// Password, return ID_EDIT_PASS = 41
	g_pUI.setMaxSymbols(ID_EDIT_PASS, 15);						// Password: max 15 symbols (16 with end of the string)
	g_pUI.setMaxPixels(ID_EDIT_PASS, 150);						// Password: max 150 pixels
	g_pUI.setHide(ID_EDIT_PASS, true);							// Hide password
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"E-mail:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_MAIL = 42
	g_pUI.addEditBox(ID_WINDOW_PANEL, ID_FONT_16, L"", 150, 0.5f);	// E-mail, return ID_EDIT_MAIL = 43
	g_pUI.setMaxSymbols(ID_EDIT_MAIL, 63);
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"Skype:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_SKYPE = 44
	g_pUI.addEditBox(ID_WINDOW_PANEL, ID_FONT_16, L"", 150, 0.5f);	// Skype, return ID_EDIT_SKYPE = 45
	g_pUI.setMaxSymbols(ID_EDIT_SKYPE, 63);
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"Country:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_COUNTRY = 46
	g_pUI.addEditBox(ID_WINDOW_PANEL, ID_FONT_16, L"", 150, 0.5f);	// Country, return ID_EDIT_COUNTRY = 47
	g_pUI.setMaxSymbols(ID_EDIT_COUNTRY, 63);
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"City:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_CITY = 48
	g_pUI.addEditBox(ID_WINDOW_PANEL, ID_FONT_16, L"", 150, 0.5f);	// City, return ID_EDIT_CITY = 49
	g_pUI.setMaxSymbols(ID_EDIT_CITY, 31);
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"Age:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_AGE = 50
	g_pUI.addEditBox(ID_WINDOW_PANEL, ID_FONT_16, L"0", 150, 0.5f);	// Age, return ID_EDIT_AGE = 51
	g_pUI.setInt(ID_EDIT_AGE, 99);									// Max Age = 99
	g_pUI.addRadioButton(ID_WINDOW_PANEL, ID_FONT_16, GROUP_GENDER, L"Male", COLOR_GOLD, 0.5f);	// Male, return ID_RADIO_MALE = 52
	g_pUI.addRadioButton(ID_WINDOW_PANEL, ID_FONT_16, GROUP_GENDER, L"Female", COLOR_GOLD, 0.5f);	// Female, return ID_RADIO_FEMALE = 53
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"About me:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_MYSELF = 54
	g_pUI.addEditBox(ID_WINDOW_PANEL, ID_FONT_16, L"", 150, 0.5f);	// About my self, return ID_EDIT_MYSELF = 55
	g_pUI.setMaxSymbols(ID_EDIT_MYSELF, 95);
	g_pUI.addButton(ID_WINDOW_PANEL, ID_FONT_16, L"New", 65, 1, 0.5f);	// New, return ID_BUTTON_REG_NEW = 56
	g_pUI.addButton(ID_WINDOW_PANEL, ID_FONT_16, L"Update", 65, 0, 0.5f);	// Update, return ID_BUTTON_REG_UPDT = 57
	g_pUI.addButton(ID_WINDOW_PANEL, ID_FONT_16, L"Delete", 65, 2, 0.5f);	// Delete, return ID_BUTTON_REG_DEL = 58
	g_pUI.addStatic(ID_WINDOW_PANEL, ID_FONT_16, L"Unique client params", DT_LEFT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_UNIQUE = 59
	g_pUI.addButton(ID_WINDOW_PANEL, ID_FONT_16, L"Traffic Monitoring", 200, 1, 0.5f);	// Traffic Monitoring, return ID_BUTTON_MONITOR = 60

	// Adding client params window
	g_pUI.addWindow(ID_FONT_16, L"UNIQUE CLIENT PARAMS", 510, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_PARAMS = 60
	g_pUI.addStatic(ID_WINDOW_PARAMS, ID_FONT_16, L"String:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_STRING = 60
	g_pUI.addEditBox(ID_WINDOW_PARAMS, ID_FONT_16, L"String", 150, 0.5f);	// Name, return ID_EDIT_STRING = 61
	g_pUI.setMaxSymbols(ID_EDIT_STRING, 255);							// Name: max 255 symbols (256 with end of the string)
	g_pUI.addStatic(ID_WINDOW_PARAMS, ID_FONT_16, L"Float:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_FLOAT = 62
	g_pUI.addEditBox(ID_WINDOW_PARAMS, ID_FONT_16, L"2.345678", 150, 0.5f);	// Password, return ID_EDIT_FLOAT = 63
	g_pUI.setFloat(ID_EDIT_FLOAT, 0, 0);						////// TO SET
	g_pUI.addStatic(ID_WINDOW_PARAMS, ID_FONT_16, L"System time:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_SYSTEMTIME = 64
	g_pUI.addEditBox(ID_WINDOW_PARAMS, ID_FONT_16, L"", 150, 0.5f);	// E-mail, return ID_EDIT_SYSTEMTIME = 65
	g_pUI.addStatic(ID_WINDOW_PARAMS, ID_FONT_16, L"__int64:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_INT64 = 66
	g_pUI.addEditBox(ID_WINDOW_PARAMS, ID_FONT_16, L"1234567890123456", 150, 0.5f);	// __int64, return ID_EDIT_INT64 = 67
	g_pUI.setMaxSymbols(ID_EDIT_INT64, 21);
	g_pUI.addStatic(ID_WINDOW_PARAMS, ID_FONT_16, L"Short:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_SHORT = 68
	g_pUI.addEditBox(ID_WINDOW_PARAMS, ID_FONT_16, L"65535", 150, 0.5f);	// Short, return ID_EDIT_SHORT = 69
	g_pUI.setInt(ID_EDIT_SHORT, 65535);
	g_pUI.addStatic(ID_WINDOW_PARAMS, ID_FONT_16, L"Byte:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_BYTE = 70
	g_pUI.addEditBox(ID_WINDOW_PARAMS, ID_FONT_16, L"", 150, 0.5f);	// Byte, return ID_EDIT_BYTE = 71
	g_pUI.setInt(ID_EDIT_BYTE, 255);
	g_pUI.addStatic(ID_WINDOW_PARAMS, ID_FONT_16, L"Time:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_TIME = 72
	g_pUI.addEditBox(ID_WINDOW_PARAMS, ID_FONT_16, L"0", 150, 0.5f);	// Time, return ID_EDIT_TIME = 73
	//g_pUI.setInt(ID_EDIT_TIME, 2000000000);							// Max Age = 99
	g_pUI.addStatic(ID_WINDOW_PARAMS, ID_FONT_16, L"Double:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_DOUBLE = 74
	g_pUI.addEditBox(ID_WINDOW_PARAMS, ID_FONT_16, L"9.12345678", 150, 0.5f);	// Double, return ID_EDIT_DOUBLE = 75
	g_pUI.setFloat(ID_EDIT_DOUBLE, 0, 0);
	g_pUI.addStatic(ID_WINDOW_PARAMS, ID_FONT_16, L"Unknown:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_UNKNOWN = 76
	g_pUI.addEditBox(ID_WINDOW_PARAMS, ID_FONT_16, L"Unknown text", 150, 0.5f);	// Unknown, return ID_EDIT_UNKNOWN = 77
	g_pUI.setMaxSymbols(ID_EDIT_UNKNOWN, 255);							// Name: max 255 symbols (256 with end of the string)
	g_pUI.addStatic(ID_WINDOW_PARAMS, ID_FONT_16, L"Unknown2:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_UNKNOWN2 = 76
	g_pUI.addEditBox(ID_WINDOW_PARAMS, ID_FONT_16, L"Unknown2 text", 150, 0.5f);	// Unknown2, return ID_EDIT_UNKNOWN2 = 77
	g_pUI.setMaxSymbols(ID_EDIT_UNKNOWN2, 255);							// Name: max 255 symbols (256 with end of the string)
	g_pUI.addButton(ID_WINDOW_PARAMS, ID_FONT_16, L"Update", 100, 0, 0.5f);	// Update params on the Server, return ID_BUTTON_PAR_UPDT = 78

	g_pUI.addChart(ID_WINDOW_PARAMS, ID_FONT_16, L"Chart for traffic", 770, 520, 0.9f);	// Return ID_CHART = 83
	g_pUI.setXText(ID_CHART, L"X parametrs");	// Set the title text on the X axis
	g_pUI.setYText(ID_CHART, L"Y parametrs");	// Set the title text on the Y axis
	g_pUI.setBackGroundColor(ID_CHART, 0xff80c0ff);
	g_pUI.setBorderColor(ID_CHART, 0xffff00ff);
	g_pUI.setBarLinesColor(ID_CHART, 0xffff8080);
	g_pUI.setTitleColor(ID_CHART, 0xff0000ff);
	g_pUI.setXTextColor(ID_CHART, 0xff00ff00);
	g_pUI.setYTextColor(ID_CHART, 0xffff0000);
	g_pUI.setYTextColor(ID_CHART, 0xffff0000);
	g_pUI.addSeries(ID_CHART, COLOR_RED, 0, 0);	// Return ID_SERIES_RED = 0
	for (int i = 1; i < 102; i++)
		g_pUI.addPoint(ID_CHART, ID_SERIES_RED, float(i));
	g_pUI.addSeries(ID_CHART, COLOR_GREEN, 256, 100);	// Return ID_SERIES_GREEN = 1
	for (int i = 1; i < 151; i++)
		g_pUI.addPoint(ID_CHART, ID_SERIES_GREEN, float(100 - i));
	g_pUI.addSeries(ID_CHART, COLOR_BLUE, 256, 20);	// Return ID_SERIES_BLUE = 2
	for (float f = 0.2f; f < 25.1f; f += 0.2f)
		g_pUI.addPoint(ID_CHART, ID_SERIES_BLUE, float(20.0f + 50.0f*sin(f)));

	g_pUI.addButton(-1, ID_FONT_16, L"Exit", 100, 2, 0.4f);	// Exit, return ID_BUTTON_EXIT = 84
	g_pUI.addMessage(ID_FONT_16, L"Awaiting a response from the server.", L"Cancel", 500, 160, COLOR_GOLD, true, 0.3f);	// Return ID_MESSAGE_WAIT = 85
	g_pUI.addMessage(ID_FONT_16, L"", L"OK", 500, 160, COLOR_GOLD, true, 0.3f);	// Message with text of error, return ID_MESSAGE_ERROR = 86

	ChangeState(MENU);	// Begin state - MENU
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.2f);	// Return 0, add space for fps
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.2f);	// Return 1, add space for timer
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int w, int h)
{
	g_pText.pos(0, 5, 2);	// fps
	g_pText.pos(1, 105, 2);	// timer

	g_hw = w / 2;
	g_hh = h / 2;

	g_pUI.pos(ID_WINDOW_MENU, g_hw - 250, g_hh - 225);		// START CHAT
	g_pUI.pos(ID_STATIC_SQL_NAME, g_hw - 5, g_hh - 144);	// DataBase name:
	g_pUI.pos(ID_EDIT_SQL_NAME, g_hw, g_hh - 150);			// DataBase name
	g_pUI.pos(ID_STATIC_SERVER, g_hw - 5, g_hh - 109);		// SQL Server name:
	g_pUI.pos(ID_EDIT_SERVER, g_hw, g_hh - 115);			// SQL Server name
	g_pUI.pos(ID_STATIC_SQL_LOGIN, g_hw - 5, g_hh - 74);	// Login for SQL Server authentication:
	g_pUI.pos(ID_EDIT_SQL_LOGIN, g_hw, g_hh - 80);			// Login for SQL Server authentication
	g_pUI.pos(ID_STATIC_SQL_PASS, g_hw - 5, g_hh - 39);		// Password for SQL Server authentication:
	g_pUI.pos(ID_EDIT_SQL_PASS, g_hw, g_hh - 45);			// Password for SQL Server authentication
	g_pUI.pos(ID_STATIC_IP, g_hw - 5, g_hh - 4);			// IP
	g_pUI.pos(ID_EDIT_IP, g_hw, g_hh - 10);					// IP
	g_pUI.pos(ID_STATIC_PORT, g_hw - 5, g_hh + 31);			// Port
	g_pUI.pos(ID_EDIT_PORT, g_hw, g_hh + 25);				// Port
	g_pUI.pos(ID_STATIC_LOGIN, g_hw - 5, g_hh + 66);		// Name
	g_pUI.pos(ID_EDIT_LOGIN, g_hw, g_hh + 60);				// Name
	g_pUI.pos(ID_STATIC_PASSWORD, g_hw - 5, g_hh + 101);	// Password
	g_pUI.pos(ID_EDIT_PASSWORD, g_hw, g_hh + 95);			// Password
	g_pUI.pos(ID_BUTTON_SERVER, g_hw - 150, g_hh + 146);	// Start server
	g_pUI.pos(ID_BUTTON_CONNECT, g_hw + 50, g_hh + 146);	// Connect to the Server
	//g_pUI.pos(ID_BUTTON_REG, g_hw + 100, g_hh + 146);		// Registration on the Server

	g_pUI.pos(ID_WINDOW_PANEL, g_hw - 532, g_hh - 320);		// ADMIN PANEL
	g_pUI.pos(ID_STATIC_IPLIST, g_hw - 390, g_hh - 215);	// List of IP-addresses
	g_pUI.pos(ID_LIST_IP, g_hw - 490, g_hh - 200);			// List of IP-addresses
	g_pUI.pos(ID_STATIC_PLAYERS, g_hw - 390, g_hh - 100);	// List of players (guests N)
	g_pUI.pos(ID_LIST_PLAYERS, g_hw - 490, g_hh - 85);		// List of players
	g_pUI.pos(ID_STATIC_ACC, g_hw - 390, g_hh + 125);		// List of accounts
	g_pUI.pos(ID_LIST_ACC, g_hw - 490, g_hh + 140);			// List of accounts
	g_pUI.pos(ID_STATIC_REG, g_hw - 280, g_hh + 125);		// List of registrations
	g_pUI.pos(ID_LIST_REG, g_hw - 280, g_hh + 140);			// List of registrations
	g_pUI.pos(ID_STATIC_CHAT, g_hw - 180, g_hh - 245);		// Chat with ...
	g_pUI.pos(ID_LIST_CHAT, g_hw - 280, g_hh - 230);		// List of chat
	g_pUI.pos(ID_STATIC_MESSAGE, g_hw - 280, g_hh + 35);	// Message
	g_pUI.pos(ID_EDIT_MESSAGE, g_hw - 280, g_hh + 50);		// Edit message
	//g_pUI.pos(ID_STATIC_GUESTS, g_hw + 153, g_hh + 118);	// Guests
	g_pUI.pos(ID_BUTTON_CHAT_SEND, g_hw - 275, g_hh + 82);	// Send
	g_pUI.pos(ID_BUTTON_CHAT_UPDT, g_hw - 185, g_hh + 82);	// Update
	g_pUI.pos(ID_BUTTON_CHAT_DEL, g_hw - 95, g_hh + 82);	// Delete

	g_pUI.pos(ID_STATIC_REGPAR, g_hw + 70, g_hh - 245);		// Registration params
	g_pUI.pos(ID_STATIC_ID, g_hw + 65, g_hh - 214);			// ID
	g_pUI.pos(ID_EDIT_ID, g_hw + 70, g_hh - 220);			// ID
	g_pUI.pos(ID_STATIC_NAME, g_hw + 65, g_hh - 184);		// Name
	g_pUI.pos(ID_EDIT_NAME, g_hw + 70, g_hh - 190);			// Name
	g_pUI.pos(ID_STATIC_PASS, g_hw + 65, g_hh - 154);		// Password
	g_pUI.pos(ID_EDIT_PASS, g_hw + 70, g_hh - 160);			// Password
	g_pUI.pos(ID_STATIC_MAIL, g_hw + 65, g_hh - 124);		// E-mail
	g_pUI.pos(ID_EDIT_MAIL, g_hw + 70, g_hh - 130);			// E-mail
	g_pUI.pos(ID_STATIC_SKYPE, g_hw + 65, g_hh - 94);		// Skype
	g_pUI.pos(ID_EDIT_SKYPE, g_hw + 70, g_hh - 100);		// Skype
	g_pUI.pos(ID_STATIC_COUNTRY, g_hw + 65, g_hh - 64);		// Country
	g_pUI.pos(ID_EDIT_COUNTRY, g_hw + 70, g_hh - 70);		// Country
	g_pUI.pos(ID_STATIC_CITY, g_hw + 65, g_hh - 34);		// City
	g_pUI.pos(ID_EDIT_CITY, g_hw + 70, g_hh - 40);			// City
	g_pUI.pos(ID_STATIC_AGE, g_hw + 65, g_hh - 4);			// Age
	g_pUI.pos(ID_EDIT_AGE, g_hw + 70, g_hh - 10);			// Age
	g_pUI.pos(ID_RADIO_MALE, g_hw + 50, g_hh + 24);			// Male
	g_pUI.pos(ID_RADIO_FEMALE, g_hw + 120, g_hh + 24);		// Female
	g_pUI.pos(ID_STATIC_MYSELF, g_hw + 65, g_hh + 56);		// About my self
	g_pUI.pos(ID_EDIT_MYSELF, g_hw + 70, g_hh + 50);		// About my self
	g_pUI.pos(ID_BUTTON_REG_UPDT, g_hw + 5, g_hh + 82);		// Update on the Server
	g_pUI.pos(ID_BUTTON_REG_NEW, g_hw + 80, g_hh + 82);		// New
	g_pUI.pos(ID_BUTTON_REG_DEL, g_hw + 155, g_hh + 82);	// Delete
	g_pUI.pos(ID_STATIC_UNIQUE, g_hw + 330, g_hh - 245);	// Unique client params
	g_pUI.pos(ID_BUTTON_MONITOR, g_hw - 490, g_hh - 245);	// Traffic Monitoring

	g_pUI.pos(ID_WINDOW_PARAMS, g_hw - 250, g_hh - 225);	// CLIENT PARAMS
	if (g_eState == SERVER)
		setPosParams();	// Set positions for unique client params
	else
	{
		g_pUI.pos(ID_STATIC_STRING, g_hw - 135, g_hh - 144);	// String
		g_pUI.pos(ID_EDIT_STRING, g_hw - 130, g_hh - 150);		// String
		g_pUI.pos(ID_STATIC_FLOAT, g_hw - 135, g_hh - 109);		// Float
		g_pUI.pos(ID_EDIT_FLOAT, g_hw - 130, g_hh - 115);		// Float
		g_pUI.pos(ID_STATIC_SYSTIME, g_hw - 135, g_hh - 74);	// SYSTEMTIME
		g_pUI.pos(ID_EDIT_SYSTIME, g_hw - 130, g_hh - 80);		// SYSTEMTIME
		g_pUI.pos(ID_STATIC_INT64, g_hw - 135, g_hh - 39);		// __int64
		g_pUI.pos(ID_EDIT_INT64, g_hw - 130, g_hh - 45);		// __int64
		g_pUI.pos(ID_STATIC_SHORT, g_hw - 135, g_hh - 4);		// Short
		g_pUI.pos(ID_EDIT_SHORT, g_hw - 130, g_hh - 10);		// Short
		g_pUI.pos(ID_STATIC_BYTE, g_hw - 135, g_hh + 31);		// Byte
		g_pUI.pos(ID_EDIT_BYTE, g_hw - 130, g_hh + 25);			// Byte
		g_pUI.pos(ID_STATIC_TIME, g_hw - 135, g_hh + 66);		// __time32_t
		g_pUI.pos(ID_EDIT_TIME, g_hw - 130, g_hh + 60);			// __time32_t
		g_pUI.pos(ID_STATIC_DOUBLE, g_hw, g_hh + 66);			// Double
		g_pUI.pos(ID_EDIT_DOUBLE, g_hw + 100, g_hh + 66);		// Double
		g_pUI.pos(ID_STATIC_UNKNOWN, g_hw - 135, g_hh + 101);	// Unknown
		g_pUI.pos(ID_EDIT_UNKNOWN, g_hw - 130, g_hh + 95);		// Unknown
		g_pUI.pos(ID_STATIC_UNKNOWN2, g_hw - 135, g_hh + 131);	// Unknown2
		g_pUI.pos(ID_EDIT_UNKNOWN2, g_hw - 130, g_hh + 125);	// Unknown2
		g_pUI.pos(ID_BUTTON_PAR_UPDT, g_hw - 50, g_hh + 176);	// Update params on the Server
	}

	g_pUI.pos(ID_CHART, g_hw - 282, g_hh - 242);			// Chart for traffic

	g_pUI.pos(ID_BUTTON_EXIT, w - 104, h - 36);				// Exit
	g_pUI.pos(ID_MESSAGE_WAIT, g_hw - 250, g_hh - 80);		// Wait
	g_pUI.pos(ID_MESSAGE_ERROR, g_hw - 250, g_hh - 80);		// Error
	
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
			//g_pUI.text(ID_STATIC_GUESTS, s);
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
			//g_pUI.text(ID_STATIC_GUESTS, s);
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
				//swprintf_s(g_sMsgTitle, 32, L"To ALL: ");	// To Recipient:
				swprintf_s(s, 32, L"%d %s", g_acc, g_pUI.getText(ID_EDIT_LOGIN));	// Account and name of authorized player
				g_pUI.addItem(ID_LIST_PLAYERS, s, g_acc);	// Insert item into the players list
				g_pUI.selItem(ID_LIST_PLAYERS, 0);	// Select own account for chat with all
				ChangeState(CLIENT);
			}
			else
			{
				if (g_sName[0])	// If entered you name 
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
				ChangeState(MENU);
			}
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

	}
}