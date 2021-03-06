//--------------------------------------------------------------------------------------
// File: 19 BlackJack.cpp
// Description: Sample of network game, BlackJack with whores.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to create network 3D game:
// - connect, disconnect, messages;
// - 3D graphics.

// Gaming state machine contains 5 states:
// 1. MENU - when the program starts, after exit (exit button click or escape key) or 2 players game over.
// 2. HELP - after Help button click.
// 3. HOST - after "Single Game" button click
// 4. CLIENT - after "2 Players Game" button click

// NOTE:
// - Must be connected to the Internet or a local network.
// - Don't forget to open the port in the firewall when the host starts or the client connects.

// Used content:
// - Content\\Texture\\BlackJack.jpg
// - Content\\Mesh\\BlackJack\\Table.dds
// - Content\\Mesh\\BlackJack\\Cards.dds
// - Content\\Mesh\\BlackJack\\Table.sdkmesh
// - Content\\Mesh\\BlackJack\\Cards.sdkmesh


// In the next version of SDK, maybe, will be added in the next version of engine:
// - stable local network;
// - peering messaging;
// - host migration;
// - players name;
// - double split;
// - short rules to help;
// - simple chat;
// - protocol (write, what is the protocol and how it works) and UML 2.x diagrams.


#include "..\Engine\NetWars.h"	// Engine

#include <stdio.h>	// swprintf_s

#include "BJ.h"	// Class of BlackJack


CEngine	g_pGame;	// Base Engine class
CTex	g_pTex;		// Texture class
C2D		g_p2D;		// 2D class
CText	g_pText;	// Text class
CUI		g_pUI;		// UI class
CSound	g_pSound;	// Sound class
CMedia	g_pMedia;	// Media class
CNet	g_pNet;		// Network class
C3D		g_p3D;		// 3D class
CBlackJack	g_pBJ;	// BlackJack class


#define	ID_WINDOW_MENU		0	// MENU
#define	ID_STATIC_IP		1	// IP-address
#define	ID_EDIT_IP			2	// IP-address
#define	ID_STATIC_PORT		3	// Port
#define	ID_EDIT_PORT		4	// Port
#define	ID_STATIC_NAME		5	// Name
#define	ID_EDIT_NAME		6	// Name
#define	ID_STATIC_PASS		7	// Password
#define	ID_EDIT_PASS		8	// Password
#define	ID_STATIC_DECKS		9	// Decks
#define	ID_COMBO_DECKS		10	// Decks
#define	ID_CHECK_FULLSCREEN 11	// FullScreen mode
#define	ID_BUTTON_ONLINE	12	// Online
#define	ID_BUTTON_HOST		13	// Host
#define	ID_BUTTON_HELP		14	// Help
#define	ID_BUTTON_EXIT		15	// Exit
#define	ID_WINDOW_IP		16	// LIST OF IP-ADDRESS
#define	ID_LIST_IP			17	// IP
#define	ID_STATIC_COPY		18	// Copy IP
#define	ID_WINDOW_HELP		19	// HELP
#define	ID_LINK_RULES		20	// Link to rules
#define	ID_BUTTON_HIT		21	// Hit
#define	ID_BUTTON_STAND		22	// Stand
#define	ID_BUTTON_DOUBLE	23	// Double
#define	ID_BUTTON_SPLIT		24	// Split
#define	ID_BUTTON_INSURANCE	25	// Insurance
#define	ID_BUTTON_SURRENDER	26	// Surrender
#define	ID_BUTTON_NEWDEAL	27	// New deal
#define	ID_BUTTON_MONEY100	28	// Money +100

#define	ID_TEXTURE_BACK		0	// BackGround texture
#define	ID_TEXTURE_TABLE	1	// Table texture
#define	ID_TEXTURE_CARDS	2	// Cards texture

#define	ID_FONT16	0
#define	ID_FONT20	1
#define	ID_FONT48	2


enum state	// State machine
{
	MENU,
	HELP,
	HOST,
	CLIENT,
};
enum state g_eState = MENU;	// State machine

bool	g_bPause = false;	// Flag of pause
bool	g_bVisualIP = true;	// Flag of visual IP for HOST state
BYTE	g_msgButton[2];		// Id of player (0 - server message); message type
UINT	g_nidHost = 0;		// Network ID Host
byte	g_idPlayer = 0;		// Player ID (1-5)


void ChangeState(state newState)
{
	g_eState = newState;	// New state

	g_pUI.visible(ID_WINDOW_MENU, g_eState == MENU);	// MENU
	g_pUI.visible(ID_WINDOW_IP, g_eState == HOST);		// IP
	g_pUI.visible(ID_WINDOW_HELP, g_eState == HELP);	// HOST
}

void ChangeButton()
{
	g_pUI.visible(ID_BUTTON_HIT, g_pBJ.GetHit(g_idPlayer));				// Hit
	g_pUI.visible(ID_BUTTON_STAND, g_pBJ.GetStand(g_idPlayer));			// Stand
	g_pUI.visible(ID_BUTTON_DOUBLE, g_pBJ.GetDouble(g_idPlayer));		// Double
	g_pUI.visible(ID_BUTTON_SPLIT, g_pBJ.GetSplit(g_idPlayer));			// Split
	g_pUI.visible(ID_BUTTON_INSURANCE, g_pBJ.GetInsurance(g_idPlayer));	// Insurance
	g_pUI.visible(ID_BUTTON_SURRENDER, g_pBJ.GetSurrender(g_idPlayer));	// Surrender
	g_pUI.visible(ID_BUTTON_NEWDEAL, g_pBJ.GetNewDeal(g_idPlayer));		// New deal
	g_pUI.visible(ID_BUTTON_MONEY100, g_pBJ.GetAddMoney(g_idPlayer));	// Money +100
}


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	// Set UI File

	//g_pGame.addLOG(sFolder);	// Add logging

	g_pGame.setCallbackInitialize();		// Set callback for Initialize
	g_pGame.setCallbackGUI();				// Set callback for GUI event
	g_pGame.setCallbackKeyboard();			// Set callback for Keyboard event
	g_pGame.setCallbackDeviceCreated();		// Set callback for Create Device
	g_pGame.setCallbackSwapChainResized();	// Set callback for Resize
	g_pGame.setCallbackFrameRender();		// Set callback for Render Frame

	g_pTex.setHModule(g_pGame.getHModule());	// Set NetWars.dll
	g_p2D.setHModule(g_pGame.getHModule());		// Set NetWars.dll
	g_pText.setHModule(g_pGame.getHModule());	// Set NetWars.dll
	g_pUI.setHModule(g_pGame.getHModule());		// Set NetWars.dll
	g_pSound.setHModule(g_pGame.getHModule());	// Set NetWars.dll
	g_pMedia.setHModule(g_pGame.getHModule());	// Set NetWars.dll
	g_pNet.setHModule(g_pGame.getHModule());	// Set NetWars.dll
	g_p3D.setHModule(g_pGame.getHModule());		// Set NetWars.dll

	g_pNet.setCallbackConnect();			// Set callback function for connect the player to the server (host) or connect the server (host) to the player
	g_pNet.setCallbackDisconnect();			// Set callback function for disconnect the player from the server (host) or disconnect the server (host) from the player
	g_pNet.setCallbackNetMsg();				// Set callback function for get the network message

	// Run Engine
	return g_pGame.run(L"19 BlackJack", 1024, 600);
}


//--------------------------------------------------------------------------------------
// INITIALIZE
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	// Adding fonts
	g_pGame.addFont(L"Arial", 16, FW_MEDIUM);	// Return ID_FONT16 = 0, add font for text
	g_pGame.addFont(L"Arial", 20, FW_MEDIUM);	// Return ID_FONT20 = 1, add font for text
	g_pGame.addFont(L"Arial", 48, FW_MEDIUM);	// Return ID_FONT48 = 2, add font for text

	// Adding UI controls
	g_pUI.addWindow(ID_FONT20, L"MENU", 410, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_MENU = 0

	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT20, L"IP:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT20, L"127.0.0.1", 200, 0.5f);	// IP-address
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT20, L"Port:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT20, L"1234", 200, 0.5f);	// Port
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT20, L"Name:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT20, L"", 200, 0.5f);	// Name
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT20, L"Password:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT20, L"", 200, 0.5f);	// Password
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT20, L"Decks:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return 
	g_pUI.addComboBox(ID_WINDOW_MENU, ID_FONT20, 120, 0.5f);	// Decks
	g_pUI.addItem(ID_COMBO_DECKS, L"1", 1);
	g_pUI.addItem(ID_COMBO_DECKS, L"2", 2);
	g_pUI.addItem(ID_COMBO_DECKS, L"3", 3);
	g_pUI.addItem(ID_COMBO_DECKS, L"4", 4);
	g_pUI.addItem(ID_COMBO_DECKS, L"5", 5);
	g_pUI.addItem(ID_COMBO_DECKS, L"6", 6);
	g_pUI.addItem(ID_COMBO_DECKS, L"7", 7);
	g_pUI.addItem(ID_COMBO_DECKS, L"8", 8);
	g_pUI.selItem(ID_COMBO_DECKS, 5);	// 6 deals

	bool bFullScreen = true;
	g_pUI.addCheckBox(ID_WINDOW_MENU, ID_FONT20, L"FullScreen Mode", COLOR_GOLD, bFullScreen ? true : false, 0.4f);	// Return ID_CHECK_FULLSCREEN = 7
 
	g_pUI.addButton(ID_WINDOW_MENU, ID_FONT20, L"Online", 100, 1, 0.5f);	// Online
	g_pUI.addButton(ID_WINDOW_MENU, ID_FONT20, L"Server", 100, 3, 0.5f);	// Server
	g_pUI.addButton(ID_WINDOW_MENU, ID_FONT20, L"Help", 80, 0, 0.5f);		// Help
	g_pUI.addButton(-1, ID_FONT20, L"Exit", 100, 2, 0.5f);		// Exit

	g_pUI.addWindow(ID_FONT20, L"LIST OF IP-ADDRESS", 410, 255, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_IP = 15
	g_pUI.addListBox(ID_WINDOW_IP, ID_FONT20, 320, 5, 0.5f);	// IP list
	g_pUI.addStatic(ID_WINDOW_IP, ID_FONT16, L"Copy IP to clipboard and click Esc to close.", DT_CENTER, COLOR_GOLD, 0.5f);	// Return 

	g_pUI.addWindow(ID_FONT20, L"HELP", 560, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_HELP = 11
	g_pUI.addLink(ID_WINDOW_HELP, ID_FONT20, L"Wikipedia: rules of BlackJack", COLOR_LINK, 0.8f);	// Return ID_LINK = 17

	g_pUI.addButton(-1, ID_FONT20, L"Hit", 100, 1, 0.5f);		// Hit
	g_pUI.addButton(-1, ID_FONT20, L"Stand", 100, 2, 0.5f);		// Stand
	g_pUI.addButton(-1, ID_FONT20, L"Double", 100, 3, 0.5f);	// Double
	g_pUI.addButton(-1, ID_FONT20, L"Split", 100, 3, 0.5f);		// Split
	g_pUI.addButton(-1, ID_FONT20, L"Insurance", 100, 0, 0.5f);	// Insurance
	g_pUI.addButton(-1, ID_FONT20, L"Surrender", 100, 2, 0.5f);	// Surrender
	g_pUI.addButton(-1, ID_FONT20, L"New deal", 100, 1, 0.5f);	// New deal
	g_pUI.addButton(-1, ID_FONT20, L"Money +100", 100, 1, 0.5f);// Money +100

	g_pUI.visible(ID_BUTTON_HIT, false);		// Hit
	g_pUI.visible(ID_BUTTON_STAND, false);		// Stand
	g_pUI.visible(ID_BUTTON_DOUBLE, false);		// Double
	g_pUI.visible(ID_BUTTON_SPLIT, false);		// Split
	g_pUI.visible(ID_BUTTON_INSURANCE, false);	// Insurance
	g_pUI.visible(ID_BUTTON_SURRENDER, false);	// Surrender
	g_pUI.visible(ID_BUTTON_NEWDEAL, false);	// New deal
	g_pUI.visible(ID_BUTTON_MONEY100, false);	// Money +100

	//g_pUI.addListBox(-1, ID_FONT20, 320, 5, 0.5f);	// Chat

	ChangeState(MENU);
}


//--------------------------------------------------------------------------------------
// GUI EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onGUI(UINT, int idEl)
{
	switch (idEl)
	{
	case ID_BUTTON_HOST:	{	// Start Host
		g_msgButton[0] = g_idPlayer = 1;	// Player ID
		g_pBJ.Start((byte)g_pUI.getSelectedData(ID_COMBO_DECKS));
		ChangeButton();

		UINT port;
		swscanf_s(g_pUI.getText(ID_EDIT_PORT), L"%d", &port);
		ChangeState(HOST);	// HOST
		UINT nIP = g_pNet.startHost((WORD)port, MAX_PLAYERS);	// Start Host
		for (UINT i = 0; i < nIP; i++)
			g_pUI.addItem(ID_LIST_IP, g_pNet.getIP(i), i);
	}	break;

	case ID_BUTTON_ONLINE:	{	// Connect to the Host
		UINT port;
		swscanf_s(g_pUI.getText(ID_EDIT_PORT), L"%d", &port);
		g_pNet.connectToHost(g_pUI.getText(ID_EDIT_IP), (WORD)port, g_pUI.getText(ID_EDIT_NAME));	// Connect to the Host
	}	break;

	case ID_BUTTON_HELP:	// Help
		ChangeState(HELP);
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

	case ID_BUTTON_EXIT:
		if (g_eState == HELP)
			ChangeState(MENU);
		else
			g_pGame.destroy();	// Close the application
		break;

	case ID_BUTTON_HIT:	// Hit
		g_msgButton[1] = 1;	// Message type - Hit
		g_pNet.msg(g_nidHost, g_msgButton, 2);
		break;

	case ID_BUTTON_STAND:	// Stand
		g_msgButton[1] = 2;	// Message type - Stand
		g_pNet.msg(g_nidHost, g_msgButton, 2);
		break;

	case ID_BUTTON_DOUBLE:	// Double
		g_msgButton[1] = 3;	// Message type - Double
		g_pNet.msg(g_nidHost, g_msgButton, 2);
		break;

	case ID_BUTTON_SPLIT:	// Split
		g_msgButton[1] = 4;	// Message type - Split
		g_pNet.msg(g_nidHost, g_msgButton, 2);
		break;

	case ID_BUTTON_INSURANCE:	// Insurance
		g_msgButton[1] = 5;	// Message type - Insurance
		g_pNet.msg(g_nidHost, g_msgButton, 2);
		break;

	case ID_BUTTON_SURRENDER:	// Surrender
		g_msgButton[1] = 6;	// Message type - Surrender
		g_pNet.msg(g_nidHost, g_msgButton, 2);
		break;

	case ID_BUTTON_NEWDEAL:	// New deal
		g_msgButton[1] = 7;	// Message type - Ready to new deal
		g_pNet.msg(g_nidHost, g_msgButton, 2);
		break;

	case ID_BUTTON_MONEY100:	// Money +100
		g_msgButton[1] = 8;	// Message type - Money +100
		g_pNet.msg(g_nidHost, g_msgButton, 2);
		break;
	}
}


//--------------------------------------------------------------------------------------
// KEYBOARD EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onKeyboard(UINT nChar, bool bKeyDown)	//, bool bAltDown
{
	if (bKeyDown)
	{
		switch (nChar)
		{
		case VK_RETURN:
			//g_pMedia.prepare(g_idMedia1);
			//g_pMedia.run(g_idMedia1);
			break;	// 

		case VK_ESCAPE:
			//Exit();
			if (g_eState == HOST)
				g_pUI.visible(ID_WINDOW_IP, g_bVisualIP = !g_bVisualIP);
			break;
		}
	}

}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 0, add space for fps
	g_pText.addSpace(ID_FONT16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 1, add space for timer

	for (int i = 0; i <= 2*MAX_PLAYERS; i++)	// Dealer + all players
		g_pText.addSpace(ID_FONT20, TEXT_SHADOW, 200, DT_LEFT, 0.5f);	// Sum + Money

	// Load Texture from file
	g_pTex.add(L"..\\Content\\Texture\\BlackJack.jpg");	// BackGround
	g_pTex.add(L"..\\Content\\Mesh\\BlackJack\\Table.dds");	// Table
	g_pTex.add(L"..\\Content\\Mesh\\BlackJack\\Cards.dds");	// Cards

	g_p3D.init(4.5f, 0.0f, 0.3f, 1.0f, 50.0f, 0.1f, 1.5f);
	g_p3D.addMesh(L"..\\Content\\Mesh\\BlackJack\\Table.sdkmesh");	// Add mesh of table
	g_p3D.addMesh(L"..\\Content\\Mesh\\BlackJack\\Cards.sdkmesh");	// Add mesh of cards
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

	for (int i = 2; i < 2*MAX_PLAYERS+3; i++)	// Dealer + all players
		g_pText.pos(i, 5, 20 * i);	// Sum + Money

	g_p2D.addF(0.0f, 1.0f, 170 / 1024.0f, 770 / 1024.0f, 0.0f, (float)w, 0.0f, (float)h, 0.9f);	// BackGround

	g_pUI.pos(ID_WINDOW_MENU, hw - 200, hh - 225);		// MENU
	g_pUI.pos(ID_STATIC_IP, hw - 55, hh - 129);			// IP-address
	g_pUI.pos(ID_EDIT_IP, hw - 50, hh - 135);			// IP-address
	g_pUI.pos(ID_STATIC_PORT, hw - 55, hh - 89);		// Port
	g_pUI.pos(ID_EDIT_PORT, hw - 50, hh - 95);			// Port
	g_pUI.pos(ID_STATIC_NAME, hw - 55, hh - 49);		// Name
	g_pUI.pos(ID_EDIT_NAME, hw - 50, hh - 55);			// Name
	g_pUI.pos(ID_STATIC_PASS, hw - 55, hh - 9);			// Password
	g_pUI.pos(ID_EDIT_PASS, hw - 50, hh - 15);			// Password
	g_pUI.pos(ID_STATIC_DECKS, hw - 55, hh + 31);		// Decks
	g_pUI.pos(ID_COMBO_DECKS, hw - 50, hh + 25);		// Decks
	g_pUI.pos(ID_CHECK_FULLSCREEN, hw - 50, hh + 75);	// FullScreen mode
	g_pUI.pos(ID_BUTTON_ONLINE, hw - 150, hh + 110);	// Online
	g_pUI.pos(ID_BUTTON_HELP, hw - 40, hh + 110);		// Help
	g_pUI.pos(ID_BUTTON_HOST, hw + 50, hh + 110);		// Host
	g_pUI.pos(ID_BUTTON_EXIT, w - 104, h - 36);			// Exit
	g_pUI.pos(ID_WINDOW_IP, hw - 200, -10);				// LIST OF IP-ADDRESS
	g_pUI.pos(ID_LIST_IP, hw - 160, 68);				// IP
	g_pUI.pos(ID_STATIC_COPY, hw, 180);					// Copy IP
	g_pUI.pos(ID_WINDOW_HELP, hw - 200, hh - 225);		// HELP
	g_pUI.pos(ID_LINK_RULES, hw - 120, hh - 25);		// Link to rules
	g_pUI.pos(ID_BUTTON_HIT, w - 104, hh - 120);		// Hit
	g_pUI.pos(ID_BUTTON_STAND, w - 104, hh - 80);		// Stand
	g_pUI.pos(ID_BUTTON_DOUBLE, w - 104, hh - 40);		// Double
	g_pUI.pos(ID_BUTTON_SPLIT, w - 104, hh);			// Split
	g_pUI.pos(ID_BUTTON_INSURANCE, w - 104, hh + 40);	// Insurance
	g_pUI.pos(ID_BUTTON_SURRENDER, w - 104, hh + 80);	// Surrender
	g_pUI.pos(ID_BUTTON_NEWDEAL, w - 104, hh + 120);	// New deal
	g_pUI.pos(ID_BUTTON_MONEY100, w - 104, hh + 160);	// Money +100
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double, float)
{
	g_pText.fps(0, 0, DT_LEFT, COLOR_GREEN);	// fps
	g_pText.timer(0, 1, DT_LEFT, COLOR_GREEN);	// timer

	g_pGame.setRenderSprites(RENDER_ONLY_TEXTURE, ID_TEXTURE_BACK);	// BackGround texture
	g_p2D.draw(0, 1);	// Background

	g_p3D.setRender3D();
	g_pTex.render(ID_TEXTURE_TABLE);	// Table texture
	g_p3D.renderMesh(0, 0.0f, 0.0f, 0.0f);
	g_pTex.render(ID_TEXTURE_CARDS);	// Cards texture
	g_p3D.renderMeshSubsetInit(1);

	g_pBJ.lock();	// LOCK

	wchar_t s[32];
	if (g_eState > HELP)
	{
		swprintf_s(s, 32, L"Dealer: %i", g_pBJ.GetSum(0));
		g_pText.dynamic(0, 2, TEXT_SHADOW, DT_LEFT, COLOR_GREEN, s);

		for (int i = 1; i < ALL; i++)
		{
			if (g_pBJ.GetActive(i))
			{
				swprintf_s(s, 32, g_pBJ.GetSum2(i) ? L"Player %i: %i %i" : L"Player %i: %i", i, g_pBJ.GetSum(i), g_pBJ.GetSum2(i));
				g_pText.dynamic(0, 2 + i, TEXT_SHADOW, DT_LEFT, COLOR_GREEN, s);
				swprintf_s(s, 32, L"%i Money / Bid :  %i / %i %i %i", i, g_pBJ.GetMoney(i), g_pBJ.GetBid(i), g_pBJ.GetBid2(i), g_pBJ.GetInsur(i));
				g_pText.dynamic(0, 2 + MAX_PLAYERS + i, TEXT_SHADOW, DT_LEFT, COLOR_GREEN, s);
			}
			else
			{
				swprintf_s(s, 32, L"Not player %i", i);
				g_pText.dynamic(0, 2 + i, TEXT_SHADOW, DT_LEFT, COLOR_GREEN, s);
				swprintf_s(s, 32, L"%i Not Money", i);
				g_pText.dynamic(0, 2 + MAX_PLAYERS + i, TEXT_SHADOW, DT_LEFT, COLOR_GREEN, s);
			}
		}

		if (g_pBJ.GetHand(0))	// If the dealer played
			for (int j = 0; j < g_pBJ.GetNCard(0); j++)	// All dealer cards
				g_p3D.renderMeshSubset(1, g_pBJ.GetCard(0, j), g_pBJ.GetPosX(0, j), g_pBJ.GetPosY(0, j), g_pBJ.GetPosZ(0, j));
		else
		{
			g_p3D.renderMeshSubset(1, g_pBJ.GetCard(0, 0), g_pBJ.GetPosX(0, 0), 0.003f, g_pBJ.GetPosZ(0, 0));
			g_p3D.renderMeshSubsetRotateX(1, 53, g_pBJ.GetPosX(0, 1), 0.003f, g_pBJ.GetPosZ(0, 1), PI);
		}

		for (int i = 1; i < ALL; i++)	// All players
		{
			if (g_pBJ.GetActive(i))	// If the player is active
			{
				for (int j = 0; g_pBJ.GetCard(i, j) < 52 && j < MAX_CARDS; j++)	// First hand of the player
					g_p3D.renderMeshSubset(1, g_pBJ.GetCard(i, j), g_pBJ.GetPosX(i, j), g_pBJ.GetPosY(i, j), g_pBJ.GetPosZ(i, j));

				if (g_pBJ.GetCard2(i, 0) < 52)	// If the player did split
					for (int j = 0; g_pBJ.GetCard2(i, j) < 52 && j < MAX_CARDS; j++)	// Second hand of the dealer
						g_p3D.renderMeshSubset(1, g_pBJ.GetCard2(i, j), g_pBJ.GetPosX2(i, j), g_pBJ.GetPosY2(i, j), g_pBJ.GetPosZ2(i, j));
			}
		}
	}

	g_pBJ.unlock();	// UNLOCK

	// Render UI
	g_pGame.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pUI.render();

	// Render text
	g_pGame.setRenderTexts();
	g_pText.draw(0, 2);
	if (g_eState > HELP)
		g_pText.draw(2, 2 * MAX_PLAYERS + 1);	// Dealer + all players (Sum + Money)
}


//--------------------------------------------------------------------------------------
// CONNECT TO THE NETWORK
//--------------------------------------------------------------------------------------
void CALLBACK onConnect(DWORD nid, WORD, byte)
{
	if (g_eState == HOST)	// Host
	{
		BYTE msg[1000];
		DWORD iData = g_pBJ.MsgConnectPlayer(msg, nid);
		if (iData)
			g_pNet.msg(nid, msg, iData);
	}
	else	// Client
	{	
		g_nidHost = nid;	// Network host ID
	}
}


//--------------------------------------------------------------------------------------
// DISCONNECT FROM THE NETWORK
//--------------------------------------------------------------------------------------
void CALLBACK onDisconnect(DWORD nid, byte)
{
	if (g_eState == HOST)	// Host
	{
		g_pBJ.DisconnectPlayer(nid);
	}
	else	// Client
	{
		ChangeState(MENU);	// 
	}
}


void Update(bool bChangeButton)
{
	if (g_pBJ.GetHand(0))
	{
		BYTE msga[6];	// Answer message
		msga[0] = 0;
		msga[1] = 17;	// Dealer play
		g_pBJ.GetDealerCards(msga);	// Get dealer cards
		g_pNet.msgToAll(msga, 6);	// Send dealer cards to all
	}

	if (bChangeButton)
		ChangeButton();
}


//--------------------------------------------------------------------------------------
// GET THE NETWORK MESSAGE
//--------------------------------------------------------------------------------------
void CALLBACK onNetMsg(BYTE *msg)
{
	if (msg[0] < ALL)	// Messages from the client
		switch (msg[1])	{
		case 1:	// Hit
			if (g_pBJ.GetHit(msg[0]))
			{
				BYTE msga[4];	// Answer message
				msga[0] = 11;	// Hit answer
				msga[1] = msg[0];	// Hit ID
				g_pBJ.Hit(msg[0], msga);	// Card for hit
				g_pNet.msgToAll(msga, 4);
				Update(g_idPlayer == msg[0]);
			}
			break;
		case 2:	// Stand
			if (g_pBJ.GetStand(msg[0]))
			{
				BYTE msga[3];	// Answer message
				msga[0] = 12;	// Stand answer
				msga[1] = msg[0];	// Stand ID
				msga[2] = g_pBJ.Stand(msg[0]); 
				g_pNet.msgToAll(msga, 3);
				Update(g_idPlayer == msg[0]);
			}
			break;
		case 3:	// Double
			if (g_pBJ.GetDouble(msg[0]))
			{
				BYTE msga[4];	// Answer message
				msga[0] = 13;	// Double answer
				msga[1] = msg[0];	// Double ID
				g_pBJ.Double(msg[0], msga);	// Card for double
				g_pNet.msgToAll(msga, 4);
				Update(g_idPlayer == msg[0]);
			}
			break;
		case 4:	// Split
			if (g_pBJ.GetSplit(msg[0]))
			{
				BYTE msga[4];	// Answer message
				msga[0] = 14;	// Split answer
				msga[1] = msg[0];	// Split ID
				g_pBJ.Split(msg[0], msga);	// Card for Split
				g_pNet.msgToAll(msga, 4);
				Update(g_idPlayer == msg[0]);
			}
			break;
		case 5:	// Insurance
			if (g_pBJ.GetInsurance(msg[0]))
			{
				g_pBJ.Insurance(msg[0]);
				BYTE msga[2];	// Answer message
				msga[0] = 15;	// Insurance answer
				msga[1] = msg[0];	// Insurance ID
				g_pNet.msgToAll(msga, 2);
				Update(g_idPlayer == msg[0]);
			}
			break;
		case 6:	// Surrender
			if (g_pBJ.GetSurrender(msg[0]))
			{
				g_pBJ.Surrender(msg[0]);
				BYTE msga[2];	// Answer message
				msga[0] = 16;	// Surrender answer
				msga[1] = msg[0];	// Surrender ID
				g_pNet.msgToAll(msga, 2);
				Update(g_idPlayer == msg[0]);
			}
			break;
		case 7:	// New deal
			if (g_pBJ.GetNewDeal(msg[0]))
			{
				g_pBJ.NewDeal(msg[0]);
				Update(g_idPlayer == msg[0]);
				if (g_pBJ.GetHand(1))
				{
					BYTE msga[20];	// New deal message
					UINT iData = g_pBJ.MsgNewDeal(msga);
					g_pNet.msgToAll(msga, iData);
				}
			}
			break;
		case 8:	// Money +100
			if (g_pBJ.GetAddMoney(msg[0]))
			{
				g_pBJ.AddMoney(msg[0]);
				BYTE msga[2];	// Answer message
				msga[0] = 17;	// AddMoney answer
				msga[1] = msg[0];	// AddMoney ID
				g_pNet.msgToAll(msga, 2);
				Update(g_idPlayer == msg[0]);
			}
			break;
		}
	else	// Messages from the server
		switch (msg[0])	{
		case 10:
			/// Sent out a simple message that player connected
			g_msgButton[0] = g_idPlayer = g_pBJ.ParserConnectPlayer(msg);	// Player ID + parser for current game
			ChangeState(CLIENT);	// Client
			ChangeButton();
			break;
		
		case 11:	// Hit
			g_pBJ.SetHit(msg[1], msg[2], msg[3]);
			if (g_idPlayer==msg[2])
				ChangeButton();
			break;
		case 12:	// Stand
			g_pBJ.SetStand(msg[1], msg[2]);
			if (g_idPlayer == msg[1])
				ChangeButton();
			break;
		case 13:	// Double
			g_pBJ.SetDouble(msg[1], msg[2], msg[3]);
			if (g_idPlayer == msg[1])
				ChangeButton();
			break;
		case 14:	// Split
			g_pBJ.SetSplit(msg[1], msg[2], msg[3]);
			if (g_idPlayer == msg[1])
				ChangeButton();
			break;
		case 15:	// Insurance
			g_pBJ.SetInsurance(msg[1]);
			if (g_idPlayer == msg[1])
				ChangeButton();
			break;
		case 16:	// Surrender
			g_pBJ.SetSurrender(msg[1]);
			if (g_idPlayer == msg[1])
				ChangeButton();
			break;
		case 17:	// Money +100
			g_pBJ.SetAddMoney(msg[1]);
			if (g_idPlayer == msg[1])
				ChangeButton();
			break;
		case 18:	// Dealer cards
			g_pBJ.SetDealerCards(msg);
			if (g_idPlayer == msg[1])
				ChangeButton();
			break;
		case 19:	// New deal
			g_pBJ.ParserNewDeal(msg);
			ChangeButton();
			break;
		}
}
