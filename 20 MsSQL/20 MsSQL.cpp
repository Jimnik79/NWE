//--------------------------------------------------------------------------------------
// File: 20 MsSQL.cpp
// Description: Sample of work Microsoft SQL Server DataBase.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// Download SQL server express and SQL Management Studio
// SQL server 2012 express sp1: http://www.microsoft.com/en-us/download/details.aspx?id=35579 
// SQL server 2014 express: http://www.microsoft.com/en-us/download/details.aspx?id=42299
// Choose language and use the default installation. For English: SQLEXPR_x64_ENU.exe or SQLEXPR_x86_ENU.exe, SQLManagementStudio_x64_ENU.exe or SQLManagementStudio_x86_ENU.exe

// You must have install MS SQL Server.
// Check install and connect: MVS -> TOOLS -> Connect to Databse ... -> Add Connection / Server name -> Choose COMP_NAME\SQLEXPRESS -> OK
// Manual view and edit database: MVS -> VIEW -> SQL Server Object Explorer (Ctrl+\, Ctrl+S) -> SQL Server -> COMP_NAME\SQLEXPRESS -> 
// -> Databases -> Name of your database -> Tables -> dbo.(name of your table) -> Right Mouse Click -> View Data -> Edit data if necessary
// Or use SQL Management Studio (don't forget to download and install it).

// This example shows how to work with SQL server database:
// - setting fields and tables;
// - connect to database (Windows or SQL Server authentication);
// - getting data on request (with SQL query);
// - adding, updating and deletion string.

// NOTE:
// - The structure of the obtained data fields must comply the structure of the table.
// - BOOL is used as a BYTE.
// - In the database you can not create a field with some titles, such as, "Double".


// In the next version of SDK, maybe, will be added obtaining data on an SQL query.


#include "..\Engine\NetWars.h"	// Engine

#include <stdio.h>	// swprintf_s
#include <time.h>	// _time32(NULL);


CEngine	g_pAppl;	// Base Engine class
CText	g_pText;	// Text class
CUI		g_pUI;		// UI class
CDB		g_pDB;		// Database class


#define	ID_WINDOW_CONNECT	0	// CONNECT TO SQL SERVER
#define	ID_STATIC_NAME		1	// DataBase name:
#define	ID_EDIT_NAME		2	// DataBase name
#define	ID_STATIC_SERVER	3	// SQL Server name:
#define	ID_EDIT_SERVER		4	// SQL Server name
#define	ID_STATIC_AUTH		5	// For SQL Server authentication : "
#define	ID_STATIC_LOGIN		6	// Login for SQL Server authentication:
#define	ID_EDIT_LOGIN		7	// Login for SQL Server authentication
#define	ID_STATIC_PASSWORD	8	// Password for SQL Server authentication:
#define	ID_EDIT_PASSWORD	9	// Password for SQL Server authentication
#define	ID_BUTTON_CONNECT	10	// Connect to SQL Server
#define	ID_BUTTON_EXIT		11	// Exit
#define	ID_WINDOW_ACCOUNTS	12	// EDIT ACCOUNTS
#define	ID_STATIC_ACCOUNTS	13	// ID Login Password
#define	ID_LIST_ACCOUNTS	14	// Accounts list
#define	ID_STATIC_ACC_ID	15	// ID:
#define	ID_EDIT_ACC_ID		16	// Account ID
#define	ID_STATIC_ACC_NAME	17	// Login:
#define	ID_EDIT_ACC_NAME	18	// Account Name
#define	ID_STATIC_ACC_PASS	19	// Password:
#define	ID_EDIT_ACC_PASS	20	// Account Password
#define	ID_BUTTON_ADD		21	// Add record
#define	ID_BUTTON_DELETE	22	// Delete record
#define	ID_BUTTON_FREE		23	// Free record
#define	ID_BUTTON_UPDATE	24	// Update record

#define	ID_FONT16	0
#define	ID_FONT20	1

#define	TABLE_DEMO_ACCOUNTS	0

float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background

UINT	g_nAcc;

#pragma pack(push, 1)	// For correct read/write in the database
struct TableAccounts
{
	int acc;
	wchar_t sName[16];	// 15 rms values ​​+ newline
	wchar_t sPass[16];
	float	f;
	SYSTEMTIME	st;
	__int64	i64;
	short	is;
	byte	i;
	__time32_t	t;
	double	fd;
};
#pragma pack(pop)	// For correct read/write in the database

TableAccounts* g_Acc;	// List of accounts


bool	g_bEditDB = false;

void ChangeState(bool bEditDB)
{
	g_bEditDB = bEditDB;	// Flag of edit database
	g_pUI.visible(ID_WINDOW_CONNECT, !bEditDB);	// CONNECT TO SQL SERVER
	g_pUI.visible(ID_WINDOW_ACCOUNTS, bEditDB);	// EDIT ACCOUNTS
}


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	g_pAppl.setCallbackInitialize();		// Set callback for Initialize
	g_pAppl.setCallbackGUI();				// Set callback for GUI event
	g_pAppl.setCallbackDeviceCreated();		// Set callback for Create Device
	g_pAppl.setCallbackSwapChainResized();	// Set callback for Resize
	g_pAppl.setCallbackFrameRender();		// Set callback for Render Frame

	g_pText.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_pUI.setHModule(g_pAppl.getHModule());		// Set NetWars.dll
	g_pDB.setHModule(g_pAppl.getHModule());		// Set NetWars.dll

	// Run Engine
 	return g_pAppl.run(L"20 MsSQL", 1024, 600);
}


//--------------------------------------------------------------------------------------
// INITIALIZE
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	// Adding fonts
	g_pAppl.addFont(L"Arial", 16, FW_MEDIUM);	// Return ID_FONT16 = 0, add font for text
	g_pAppl.addFont(L"Arial", 20, FW_MEDIUM);	// Return ID_FONT20 = 1, add font for text

	// Adding UI controls
	g_pUI.addWindow(ID_FONT20, L"CONNECT TO SQL SERVER", 510, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_CONNECT = 0
	g_pUI.addStatic(ID_WINDOW_CONNECT, ID_FONT20, L"DataBase Name:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addEditBox(ID_WINDOW_CONNECT, ID_FONT20, L"MsSQL0.44b", 200, 0.5f);	// DataBase name
	g_pUI.addStatic(ID_WINDOW_CONNECT, ID_FONT20, L"SQL Server Name:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addEditBox(ID_WINDOW_CONNECT, ID_FONT20, L".\\sqlexpress", 200, 0.5f);	// SQL Server name
	g_pUI.addStatic(ID_WINDOW_CONNECT, ID_FONT20, L"For SQL Server authentication:", DT_LEFT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addStatic(ID_WINDOW_CONNECT, ID_FONT20, L"SQL Server Login:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addEditBox(ID_WINDOW_CONNECT, ID_FONT20, L"", 200, 0.5f);	// Login for SQL Server authentication
	g_pUI.addStatic(ID_WINDOW_CONNECT, ID_FONT20, L"SQL Server Password:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addEditBox(ID_WINDOW_CONNECT, ID_FONT20, L"", 200, 0.5f);	// Password for SQL Server authentication
	g_pUI.setHide(ID_EDIT_PASSWORD, true);							// Hide password
	g_pUI.addButton(ID_WINDOW_CONNECT, ID_FONT20, L"Connect", 200, 1, 0.5f);	// Connect to SQL Server
	g_pUI.addButton(-1, ID_FONT20, L"Exit", 100, 2, 0.5f);	// Exit

	g_pUI.addWindow(ID_FONT20, L"EDIT ACCOUNTS", 610, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_CONNECT = 0
	g_pUI.addStatic(ID_WINDOW_ACCOUNTS, ID_FONT20, L"ID       Login                Password", DT_LEFT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addListBox(ID_WINDOW_ACCOUNTS, ID_FONT20, 320, 12, 0.5f);		// Accounts list
	g_pUI.set3Column(ID_LIST_ACCOUNTS, 50, 120);	// 3 column: ID, name, password
	g_pUI.addStatic(ID_WINDOW_ACCOUNTS, ID_FONT20, L"ID:", DT_LEFT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addEditBox(ID_WINDOW_ACCOUNTS, ID_FONT20, L"", 150, 0.5f);	// Account ID
	g_pUI.enable(ID_EDIT_ACC_ID, false);	// Account ID not edit (only visible)
	g_pUI.addStatic(ID_WINDOW_ACCOUNTS, ID_FONT20, L"Login:", DT_LEFT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addEditBox(ID_WINDOW_ACCOUNTS, ID_FONT20, L"", 150, 0.5f);	// Account Name
	g_pUI.setMaxSymbols(ID_EDIT_ACC_NAME, 15);	// Account Name: max 15 symbols (16 with end of the string)
	g_pUI.setMaxPixels(ID_EDIT_ACC_NAME, 150);	// Account Name: max 150 pixels
	g_pUI.addStatic(ID_WINDOW_ACCOUNTS, ID_FONT20, L"Password:", DT_LEFT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addEditBox(ID_WINDOW_ACCOUNTS, ID_FONT20, L"", 150, 0.5f);	// Account Password
	g_pUI.setMaxSymbols(ID_EDIT_ACC_PASS, 15);	// Account Password: max 15 symbols (16 with end of the string)
	g_pUI.setMaxPixels(ID_EDIT_ACC_PASS, 150);	// Account Password: max 150 pixels
	//g_pUI.setHide(ID_EDIT_ACC_PASS, true);		// Hide password
	g_pUI.addButton(ID_WINDOW_ACCOUNTS, ID_FONT20, L"Add", 100, 1, 0.5f);	// Add
	g_pUI.addButton(ID_WINDOW_ACCOUNTS, ID_FONT20, L"Delete", 100, 2, 0.5f);	// Delete
	g_pUI.addButton(ID_WINDOW_ACCOUNTS, ID_FONT20, L"Free", 100, 2, 0.5f);	// Free
	g_pUI.addButton(ID_WINDOW_ACCOUNTS, ID_FONT20, L"Update", 100, 3, 0.5f);	// Update

	// Table of accounts
	g_pDB.addField(DB_INT, L"ID", 0);		// int
	g_pDB.addField(DB_WCHAR, L"Name", 16);	// wchar_t[]
	g_pDB.addField(DB_WCHAR, L"Pass", 16);	// wchar_t[]
	g_pDB.addField(DB_FLOAT, L"Float", 0);	// float
	g_pDB.addField(DB_SYSTIME, L"Time", 0);	// SYSTEMTIME
	g_pDB.addField(DB_INT64, L"Int64", 0);	// __int64
	g_pDB.addField(DB_SHORT, L"Short", 0);	// short
	g_pDB.addField(DB_BYTE, L"Byte", 0);	// byte
	g_pDB.addField(DB_INT, L"Time32", 0);	// __time32_t
	g_pDB.addField(DB_DOUBLE, L"Float8", 0);// double	// Name of "Double" can not be used
	g_pDB.addTable(L"Accounts", 1);	// Return TABLE_DEMO_ACCOUNTS = 0

	ChangeState(false);	// Switch to connect database
}


//--------------------------------------------------------------------------------------
// GUI EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onGUI(UINT nEvent, int idEl)
{
	switch (idEl)
	{
	case ID_BUTTON_CONNECT:	// Connect to SQL Server
	{
		g_nAcc = g_pDB.connect(g_pUI.getText(ID_EDIT_NAME), g_pUI.getText(ID_EDIT_SERVER), g_pUI.getText(ID_EDIT_LOGIN), g_pUI.getText(ID_EDIT_PASSWORD));	// Connect to SQL Server
		if (g_nAcc)
		{
			SAFE_DELETE_ARRAY(g_Acc);
			g_Acc = new TableAccounts[g_nAcc + 10];
			g_pDB.getData(TABLE_DEMO_ACCOUNTS, (BYTE*)g_Acc);
		}

		ChangeState(true);	// Switch to edit database
		wchar_t s[12];
		for (UINT i = 0; i < g_nAcc; i++)
		{
			swprintf_s(s, 12, L"%i", g_Acc[i].acc);
			g_pUI.addItem3(ID_LIST_ACCOUNTS, s, g_Acc[i].sName, g_Acc[i].sPass, g_Acc[i].acc);
		}
		if (g_nAcc)
		{
			g_pUI.selItem(ID_LIST_ACCOUNTS, 0);
			swprintf_s(s, 12, L"%i", g_Acc[0].acc);
			g_pUI.text(ID_EDIT_ACC_ID, s);
			g_pUI.text(ID_EDIT_ACC_NAME, g_Acc[0].sName);
			g_pUI.text(ID_EDIT_ACC_PASS, g_Acc[0].sPass);
		}
		SAFE_DELETE_ARRAY(g_Acc);
		break;
	}

	case ID_BUTTON_EXIT:	// Connect to SQL Server
		if (g_bEditDB)
		{
			g_pUI.deleteAllItems(ID_LIST_ACCOUNTS);
			g_pDB.disconnect();
			ChangeState(false);	// Switch to connect database
		}
		else
			g_pAppl.destroy();	// Exit programm
		break;

	case ID_LIST_ACCOUNTS:
		if (nEvent == EVENT_SELECT)
		{
			g_pUI.text(ID_EDIT_ACC_ID, g_pUI.getSelText(ID_LIST_ACCOUNTS));
			g_pUI.text(ID_EDIT_ACC_NAME, g_pUI.getSelText2(ID_LIST_ACCOUNTS));
			g_pUI.text(ID_EDIT_ACC_PASS, g_pUI.getSelText3(ID_LIST_ACCOUNTS));
		}
		break;

	case ID_BUTTON_ADD:	// Add item
	{
		wchar_t s[8];
		TableAccounts acc;
		acc.acc = -1;
		if (g_nAcc)
			swscanf_s(g_pUI.getIndexText(ID_LIST_ACCOUNTS, g_pUI.getSize(ID_LIST_ACCOUNTS) - 1), L"%i", &acc.acc);
		swprintf_s(s, 8, L"%i", ++acc.acc);
		swprintf_s(acc.sName, 16, L"Name%i", acc.acc);
		swprintf_s(acc.sPass, 16, L"Pass%i", acc.acc);
		g_pUI.text(ID_EDIT_ACC_ID, s);
		g_pUI.text(ID_EDIT_ACC_NAME, acc.sName);
		g_pUI.text(ID_EDIT_ACC_PASS, acc.sPass);
		acc.f = 9.87654321f;
		GetLocalTime(&acc.st);
		acc.i64 = 1234567890123456;	// _time64(NULL);
		acc.t = 10000000;
		acc.i = 35;
		acc.is = -3;
		acc.fd = 9.876543210987654321;
		g_pDB.add(TABLE_DEMO_ACCOUNTS, (BYTE*)&acc);
		g_pUI.addItem3(ID_LIST_ACCOUNTS, s, acc.sName, acc.sPass, acc.acc);
		g_nAcc++;
	}	break;

	case ID_BUTTON_DELETE:	// Delete record
		if (g_pUI.getSelIndex(ID_LIST_ACCOUNTS) >= 0)
		{
			int id;
			swscanf_s(g_pUI.getText(ID_EDIT_ACC_ID), L"%i", &id);
			g_pDB.remove(0, id);
			//id = g_pUI.getSelIndex(ID_LIST_ACCOUNTS);
			g_pUI.deleteItem(ID_LIST_ACCOUNTS, g_pUI.getSelIndex(ID_LIST_ACCOUNTS));	// Delete selected item
			g_nAcc--;
		}
		break;

	case ID_BUTTON_FREE:	// Free record
	{
		TableAccounts acc;
		swscanf_s(g_pUI.getText(ID_EDIT_ACC_ID), L"%i", &acc.acc);
		swprintf_s(acc.sName, 16, L"-");
		swprintf_s(acc.sPass, 16, L"-");
		acc.f = 0.0f;
		GetLocalTime(&acc.st);
		acc.i64 = _time64(NULL);
		acc.t = 0;
		acc.i = 0;
		acc.is = 0;
		acc.fd = 9.876543210987654321;
		g_pDB.update(0, (BYTE*)&acc);
		g_pUI.updateItem3(ID_LIST_ACCOUNTS, g_pUI.getSelIndex(ID_LIST_ACCOUNTS), g_pUI.getText(ID_EDIT_ACC_ID), L"-", L"-", acc.acc);
		g_pUI.text(ID_EDIT_ACC_NAME, L"-");
		g_pUI.text(ID_EDIT_ACC_PASS, L"-");
	}	break;

	case ID_BUTTON_UPDATE:	// Update record
	{
		TableAccounts acc;
		swscanf_s(g_pUI.getText(ID_EDIT_ACC_ID), L"%i", &acc.acc);
		swprintf_s(acc.sName, 16, g_pUI.getText(ID_EDIT_ACC_NAME));
		swprintf_s(acc.sPass, 16, g_pUI.getText(ID_EDIT_ACC_PASS));
		acc.f = 1.23456789f;
		GetLocalTime(&acc.st);
		acc.i64 = _time64(NULL);
		acc.t = _time32(NULL);
		acc.i = 8;
		acc.is = -4;
		acc.fd = 9.876543210987654321;
		g_pDB.update(0, (BYTE*)&acc);
		g_pUI.updateItem3(ID_LIST_ACCOUNTS, g_pUI.getSelIndex(ID_LIST_ACCOUNTS), g_pUI.getText(ID_EDIT_ACC_ID), acc.sName, acc.sPass, acc.acc);
	}	break;
	}
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT20, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// 0 - add space for FPS
	g_pText.addSpace(ID_FONT20, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// 1 - add space for timer
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int w, int h)
{
	int hw = w / 2;
	int hh = h / 2;

	g_pText.pos(0, 5, 2);	// fps
	g_pText.pos(1, 105, 2);	// timer

	g_pUI.pos(ID_WINDOW_CONNECT, hw - 250, hh - 225);	// CONNECT TO SQL SERVER
	g_pUI.pos(ID_STATIC_NAME, hw - 5, hh - 94);			// DataBase name:
	g_pUI.pos(ID_EDIT_NAME, hw, hh - 100);				// DataBase name
	//g_pUI.pos(ID_STATIC_TABLE, hw - 5, hh - 94);		// Table name:
	//g_pUI.pos(ID_EDIT_TABLE, hw, hh - 100);			// Table name
	g_pUI.pos(ID_STATIC_SERVER, hw - 5, hh - 54);		// SQL Server name:
	g_pUI.pos(ID_EDIT_SERVER, hw, hh - 60);				// SQL Server name
	g_pUI.pos(ID_STATIC_AUTH, hw - 100, hh - 10);		// For SQL Server authentication:
	g_pUI.pos(ID_STATIC_LOGIN, hw - 5, hh + 26);		// Login:
	g_pUI.pos(ID_EDIT_LOGIN, hw, hh + 20);				// Login
	g_pUI.pos(ID_STATIC_PASSWORD, hw - 5, hh + 66);		// Password:
	g_pUI.pos(ID_EDIT_PASSWORD, hw, hh + 60);			// Password
	g_pUI.pos(ID_BUTTON_CONNECT, hw - 100, hh + 120);	// Connect to SQL Server
	g_pUI.pos(ID_BUTTON_EXIT, w - 104, h - 36);			// Exit
	g_pUI.pos(ID_WINDOW_ACCOUNTS, hw - 300, hh - 225);	// EDIT ACCOUNTS
	g_pUI.pos(ID_STATIC_ACCOUNTS, hw - 244, hh - 140);	// Accounts list
	g_pUI.pos(ID_LIST_ACCOUNTS, hw - 250, hh - 120);	// Accounts list
	g_pUI.pos(ID_STATIC_ACC_ID, hw + 90, hh - 100);		// Account ID
	g_pUI.pos(ID_EDIT_ACC_ID, hw + 90, hh - 80);		// Account ID
	g_pUI.pos(ID_STATIC_ACC_NAME, hw + 90, hh - 40);	// Account Name
	g_pUI.pos(ID_EDIT_ACC_NAME, hw + 90, hh - 20);		// Account Name
	g_pUI.pos(ID_STATIC_ACC_PASS, hw + 90, hh + 20);	// Account Password
	g_pUI.pos(ID_EDIT_ACC_PASS, hw + 90, hh + 40);		// Account Password
	g_pUI.pos(ID_BUTTON_ADD, hw - 230, hh + 140);		// Add
	g_pUI.pos(ID_BUTTON_DELETE, hw - 110, hh + 140);	// Delete
	g_pUI.pos(ID_BUTTON_FREE, hw + 10, hh + 140);		// Free
	g_pUI.pos(ID_BUTTON_UPDATE, hw + 130, hh + 140);	// Update
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double, float)
{
	g_pText.fps(2, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(2, 1, DT_LEFT, COLOR_ORANGE);	// timer

	g_pAppl.clearScreen(g_cClear);

	// Render UI
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pUI.render();

	// Texts
	g_pAppl.setRenderTexts();
	g_pText.draw(0, 2);	// Render text
}