//--------------------------------------------------------------------------------------
// File: 29 Prototype.cpp
// Description: Sample prototype of 3D MMOG.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with Unknown:

// IN DEVELOPING
// EXAMPLE NOT READY


// On the translucent map displays walls, 10 nearest players, and the distance to them.

// Add mandatory - culling not rendering objects (check for culling before rendering).

// You can trade during the breaks at death.

// When no players, online bots run under the guise of online players.

// You can create a game map in Example Map Editor and move it here.

// Preferably, use a binary version of the games maps, text maps only for debugging for easy viewing design.

// If a client comes to the server by using a valid username and password with no errors, then everything is OK, otherwise it goes into a guest mode, where it can repeat the entry or create an account.

// The system of dual calculation of physics (on the client and on the server/host).

// NOTE:
// - Don't forget all messages on the server to check for spoofing and register these cases.

// Used content:
// - Content\Map\Prototype.txtmap
// - Content\Sound\Weapon\Shot.wav
// - Content\Sound\Weapon\Reload.wav
// - Content\Texture\Ground.dds
// - Content\Texture\Wall.dds
// - Content\Texture\Pers.dds
// - Content\Texture\Sight.dds
// - Content\Mesh\Weapon\TT\
// - Content\Mesh\Weapon\Uzi\
// - Content\Mesh\Weapon\AKM\
// - Content\Mesh\Weapon\M16\
// - Content\Mesh\Weapon\Sniper\
// - Content\Mesh\Weapon\Negev_NG7\
// - Content\Mesh\Weapon\RPG\
// - Content\Mesh\Weapon\Grenade\
// - Content\Mesh\Weapon\Mine\
// - Content\Mesh\Soldier\


#include "Prototype.h"	// Global header

#include "NetMsg.h"	// Network message event handling


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	g_pGame.setCallbackKeyboard();			// Set callback for Keyboard event
	g_pGame.setCallbackMouse();				// Set callback for Mouse Event
	g_pGame.setCallbackGUI();				// Set callback for GUI event
	g_pGame.setCallbackInitialize();		// Set callback for Initialize
	g_pGame.setCallbackDeviceCreated();		// Set callback for Create Device
	g_pGame.setCallbackSwapChainResized();	// Set callback for Resize
	g_pGame.setCallbackFrameMove();			// Set callback for Frame Move
	g_pGame.setCallbackFrameRender();		// Set callback for Render Frame
	g_pGame.setCallbackDeinitialize();		// Set callback for Deinitialize

	g_p2D.setHModule(g_pGame.getHModule());		// Set NetWars.dll
	g_pTex.setHModule(g_pGame.getHModule());	// Set NetWars.dll
	g_pText.setHModule(g_pGame.getHModule());	// Set NetWars.dll
	g_pSound.setHModule(g_pGame.getHModule());	// Set NetWars.dll
	g_pMedia.setHModule(g_pGame.getHModule());	// Set NetWars.dll
	g_p3D.setHModule(g_pGame.getHModule());		// Set NetWars.dll
	g_pPhys.setHModule(g_pGame.getHModule());	// Set NetWars.dll
	g_pCam.setHModule(g_pGame.getHModule());	// Set NetWars.dll
	g_pUI.setHModule(g_pGame.getHModule());		// Set NetWars.dll
	g_pDB.setHModule(g_pGame.getHModule());		// Set NetWars.dll
	g_pNet.setHModule(g_pGame.getHModule());	// Set NetWars.dll

	g_pPhys.setCallbackBullet();			// Set callback for Contact of Bullets

	g_pNet.setCallbackConnect();	// Set callback function for connect the player to the server or connect the server to the player
	g_pNet.setCallbackDisconnect();	// Set callback function for disconnect the player from the server or disconnect the server from the player
	g_pNet.setCallbackNetMsg();		// Set callback function for get the network message

	// Run Engine
	return g_pGame.run(L"29 Prototype", g_width, g_height);	// default 1024 * 600
}

void changeWeapon(BYTE weapon)
{
	////// Verify with the level of pumping
	if (g_Ammo.level >= weapon)
	{
		g_weapon = g_plr1[g_id].weapon = weapon;	// Change weapon
		if (!g_Ammo.i[g_weapon] && g_Ammo.n[g_weapon])	// If there are no bullets in the cartridge clip, but there is in reserve.
			g_tReloadEnd = g_time + g_tReload;	// Start reloading clip.
	}
}


void startServer()
{
	InitializeCriticalSection(&m_csTrade);	// Initialize critical section for trading data

	wchar_t s[256];
	g_acc = g_id = 0;	// Admin ID and account
	g_msg[1] = g_msg[2] = g_msg[3] = 0;	// If only single-byte message identifiers are used	// Player ID

	// Add work with account and chat on the server (DB tables and protocol), Connect to SQL Server (with Accounts and Chat), Activate the server database, Return number of accounts in the DataBase
	g_pDB.addAccountAndChat();	// Add fields and tables for work with accounts and chat (must be used prior to the addition of other tables; add 3 table: Accounts, Registration, Chat)

	// Table of accounts
	g_pDB.addField(DB_INT, L"ID", 0);		// int
	g_pDB.addField(DB_BYTE, L"Level", 0);	// BYTE; The level of pumping (0-10)
	g_pDB.addField(DB_INT, L"Money", 0);	// UINT; Money/score, total
	//g_pDB.addField(DB_BYTE, L"Health", 0);	// BYTE; Health, percent
	g_pDB.addField(DB_SHORT, L"TT", 0);		// WORD; Bullets for TT, total (in a clip in the database do not need to be stored)
	g_pDB.addField(DB_SHORT, L"Uzi", 0);	// WORD; Bullets for Uzi, total (in a clip in the database do not need to be stored)
	g_pDB.addField(DB_SHORT, L"AKM", 0);	// WORD; Bullets for AKM, total (in a clip in the database do not need to be stored)
	g_pDB.addField(DB_SHORT, L"M16", 0);	// WORD; Bullets for M16, total (in a clip in the database do not need to be stored)
	g_pDB.addField(DB_SHORT, L"Sniper", 0);	// WORD; Bullets for the Sniper Rifle, total (in a clip in the database do not need to be stored)
	g_pDB.addField(DB_SHORT, L"NG7", 0);	// WORD; Bullets for Negev NG7, total (in a clip in the database do not need to be stored)
	g_pDB.addField(DB_BYTE, L"RPG", 0);		// BYTE; Grenades for RPG (rocket-propelled grenade)
	g_pDB.addField(DB_BYTE, L"Grenade", 0);	// BYTE; Hand Grenades
	g_pDB.addField(DB_BYTE, L"Mine", 0);	// BYTE; Land Mines (at the time, to contact)
	g_pDB.addField(DB_BYTE, L"AKMG", 0);	// BYTE; AKM grenades, total
	g_pDB.addField(DB_BYTE, L"M16G", 0);	// BYTE; M16 grenades, total
	g_pDB.addTable(L"Ammo", 1);				// Return TABLE_AMMO = 3

	g_pDB.addField(DB_INT, L"ID", 0);		// int; ID of bargain/transaction
	g_pDB.addField(DB_INT, L"AccOpen", 0);	// int; Account opened the deal/transaction
	g_pDB.addField(DB_INT, L"TimeOpen", 0);	// _time32; Opening time
	g_pDB.addField(DB_INT, L"AccClose", 0);	// int; Account closed the deal/transaction (-1 - the transaction is open, canceled or not took place in the allotted time)
	g_pDB.addField(DB_INT, L"TimeClose", 0);// _time32; Closing time (0 - still open)
	g_pDB.addField(DB_BYTE, L"State", 0);	// BYTE; Status of bargain/transaction (1 - open the deal/transaction to sell weapons, 2 - open the deal/transaction to buy weapons, 3.4 - not held for sale/buy 5.6 - held for sale/buy)
	g_pDB.addField(DB_BYTE, L"Type", 0);	// BYTE; Type of ammunition (0 - TT  Uzi  AKM  M16  Sniper  NG7  RPG  Grenade  Mine  AKMG  M16G)
	g_pDB.addField(DB_SHORT, L"Number", 0);	// WORD; The amount of ammunition
	g_pDB.addField(DB_INT, L"Money", 0);	// int; Money
	g_pDB.addTable(L"Trade", 1);			// Return TABLE_TRADE = 4

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

		// Read Ammo, extended player parameters
		UINT nAmmo = g_pDB.getSize(TABLE_AMMO);	// Get ammo table size
		if (nAmmo)
		{
			AMMO_DB* pAmmo = new AMMO_DB[nAmmo];	// Allocate memory for ammo
			g_pDB.getData(TABLE_AMMO, (BYTE*)pAmmo);	// Get data for ammo
			for (UINT i = 0; i < nAmmo; i++)
			{
				swprintf_s(s, 162, L"%d: %d %d %d %d %d %d %d %d %d %d %d %d %d", pAmmo[i].acc, pAmmo[i].level, pAmmo[i].nMoney,
					pAmmo[i].nTT, pAmmo[i].nUzi, pAmmo[i].nAKM, pAmmo[i].nM16, pAmmo[i].nSnip, pAmmo[i].nNG7, pAmmo[i].nRPG, pAmmo[i].nGren, pAmmo[i].nMine, pAmmo[i].nAKMG, pAmmo[i].nM16G);
				g_pUI.addItem(ID_LIST_AMMO, s, i);	// Add ammo item to the ammo list
			}
			SAFE_DELETE_ARRAY(pAmmo);	// Free memory for ammo
		}

		// Read Trade
		UINT nTrade = g_pDB.getSize(TABLE_TRADE);	// Get ammo table size
		if (nTrade)
		{
			g_idTrade = (int)nTrade;
			TRADE_DB* pTrade = new TRADE_DB[nTrade];	// Allocate memory for ammo
			g_pDB.getData(TABLE_TRADE, (BYTE*)pTrade);	// Get data for trade
			for (UINT i = 0; i < nTrade; i++)
			{
				swprintf_s(s, 162, L"%d: %d, %d, %d, %d, %d, %d, %d, $%d", pTrade[i].id, pTrade[i].accOpen, pTrade[i].tOpen, pTrade[i].accClose, pTrade[i].tClose, pTrade[i].iState, pTrade[i].iType, pTrade[i].num, pTrade[i].nMoney);
				g_pUI.addItem(ID_LIST_TRADE, s, i);	// Add trade item to the trade list

				wchar_t	s2[16];
				if (pTrade[i].iState == 1)	// Sell weapon
				{
					g_tradeSell[g_iSell].id = pTrade[i].id;
					g_tradeSell[g_iSell].accOpen = pTrade[i].accOpen;
					g_tradeSell[g_iSell].tOpen = pTrade[i].tOpen;
					g_tradeSell[g_iSell].iType = pTrade[i].iType;
					g_tradeSell[g_iSell].num = pTrade[i].num;
					g_tradeSell[g_iSell].nMoney = pTrade[i].nMoney;
					g_iSell++;
					if (pTrade[i].accOpen)	// Sale, does not own
					{
						swprintf_s(s, 16, L"%d", pTrade[i].num);
						swprintf_s(s2, 16, L"$%d", pTrade[i].nMoney);
						g_pUI.addItem3(ID_LIST_TRANSACT, g_sAmmoName[pTrade[i].iType], s, s2, pTrade[i].id);
					}
					else
					{
						swprintf_s(s, 16, L"SELL %d", pTrade[i].num);
						swprintf_s(s2, 16, L"$%d", pTrade[i].nMoney);
						g_pUI.addItem3(ID_LIST_MYTRANSACT, g_sAmmoName[pTrade[i].iType], s, s2, pTrade[i].id);
					}
				}
				if (pTrade[i].iState == 2)	// Buy weapon
				{
					g_tradeBuy[g_iBuy].id = pTrade[i].id;
					g_tradeBuy[g_iBuy].accOpen = pTrade[i].accOpen;
					g_tradeBuy[g_iBuy].tOpen = pTrade[i].tOpen;
					g_tradeBuy[g_iBuy].iType = pTrade[i].iType;
					g_tradeBuy[g_iBuy].num = pTrade[i].num;
					g_tradeBuy[g_iBuy].nMoney = pTrade[i].nMoney;
					g_iBuy++;
					if (!pTrade[i].accOpen)	// Buy, does not own
					{
						swprintf_s(s, 16, L"BUY %d", pTrade[i].num);
						swprintf_s(s2, 16, L"$%d", pTrade[i].nMoney);
						g_pUI.addItem3(ID_LIST_MYTRANSACT, g_sAmmoName[pTrade[i].iType], s, s2, pTrade[i].id);
					}
				}
			}
			g_nSell = g_iSell;
			g_nBuy = g_iBuy;
			SAFE_DELETE_ARRAY(pTrade);	// Free memory for trade
		}
	}
	else	// Admin Registration (first start)
	{
		swprintf_s(g_sName, 16, L"Admin");
		g_pDB.createAdmin(L"Admin", L"Admin", L"Admin@gmail.com", L"AdminSkype", L"AdminCountry", L"AdminCity", 163, L"AdminMyself");	// Create Admin, 163 = 35 + 128 (male)
		AMMO_DB ammo = { 0, 0, 10, 100, 200, 100, 100, 200, 100, 2, 2, 2, 2, 2 };
		g_pDB.add(TABLE_AMMO, (BYTE*)&ammo);
		g_pUI.addItem(ID_LIST_ACC, L"0 Admin Admin", 0);	// 
		g_pUI.addItem(ID_LIST_REG, L"0 Admin Admin Admin@gmail.com AdminSkype AdminCountry AdminCity 35 Male AdminMyself", 0);	// 
		g_pUI.addItem(ID_LIST_AMMO, L"0: 0, 10, 100, 200, 100, 100, 200, 100, 2, 2, 2, 2, 2", 0);	// 
	}

	AMMO_DB ammo = { 0 };	// Upload admin ammunition from the database
	g_pDB.getItem(TABLE_AMMO, (BYTE*)&ammo);	// Get record of the admin ammunition from the database 

	g_Plr[g_id].bActive = true;				// Activate the admin in the game
	g_Plr[g_id].ammo.nHealth = 100;			// Health, percent
	g_Plr[g_id].ammo.level = ammo.level;	// Level of progress
	g_Plr[g_id].ammo.nMoney = ammo.nMoney;	// Money/score
	g_Plr[g_id].ammo.n[0] = ammo.nTT;		// Bullets for TT, in stock
	g_Plr[g_id].ammo.n[1] = ammo.nUzi;		// Bullets for Uzi, in stock
	g_Plr[g_id].ammo.n[2] = ammo.nAKM;		// Bullets for AKM, in stock
	g_Plr[g_id].ammo.n[3] = ammo.nM16;		// Bullets for M16, in stock
	g_Plr[g_id].ammo.n[4] = ammo.nSnip;		// Bullets for SR, in stock
	g_Plr[g_id].ammo.n[5] = ammo.nNG7;		// Bullets for NG7, total
	g_Plr[g_id].ammo.n[6] = ammo.nRPG;		// Grenades for RPG (rocket-propelled grenade), total
	g_Plr[g_id].ammo.n[7] = ammo.nGren;		// Hand grenades, total
	g_Plr[g_id].ammo.n[8] = ammo.nMine;		// Land mines (at the time, to contact), total
	g_Plr[g_id].ammo.n[9] = ammo.nAKMG;		// AKM grenades, total
	g_Plr[g_id].ammo.n[10] = ammo.nM16G;	// M16 grenades, total


	// List of players
	swprintf_s(s, 32, L"0 %s", g_pDB.getName(0));	// Player account and name
	g_pUI.addItem(ID_LIST_PLAYERS, s, 0);	// 
	g_pUI.selItem(ID_LIST_PLAYERS, 0);
	//swprintf_s(g_sMsgTitle, 32, L"To ALL: ");	// To Recipient:

	// Start MMO Server
	UINT port;
	swscanf_s(g_pUI.getText(ID_EDIT_PORT), L"%d", &port);
	ChangeState(SERVER);	// State: SERVER;
	UINT nIP = g_pNet.startServer((WORD)port);	// Start server
	for (UINT i = 0; i < nIP; i++)
		g_pUI.addItem(ID_LIST_IP, g_pNet.getIP(i), i);

	g_bServer = true;	// Flag of Server initialization
	initPerses();	// 
}



//--------------------------------------------------------------------------------------
// KEYBOARD EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onKeyboard(UINT idKey, bool bKeyDown)
{
	if (bKeyDown)
		switch (idKey) {
		case VK_F1:	ChangeState(GAMES);	break;	// Go to the state GAMES

		case 0x30:	break;	// 0x30 0 key
		case 0x31:	changeWeapon(ID_TT);	break;	// 0x31 1 key
		case 0x32:	changeWeapon(ID_UZI);	break;	// 0x32 2 key
		case 0x33:	changeWeapon(ID_AKM);	break;	// 0x33 3 key
		case 0x34:	changeWeapon(ID_M16);	break;	// 0x34 4 key
		case 0x35:	changeWeapon(ID_RPG);	break;	// 0x35 5 key
		case 0x36:	changeWeapon(ID_SNIP);	break;	// 0x36 6 key
		case 0x37:	changeWeapon(ID_NG7);	break;	// 0x37 7 key
		case 0x38:	break;	// 0x38 8 key
		case 0x39:	break;	// 0x39 9 key

		case 0x52:	break;	// 0x52 R key	// Reloading clip
		case 0x47:	break;	// 0x47 G key	// Throw a grenade
		case 0x4D:	break;	// 0x4D M key	// Set land mine

		case VK_DELETE:	// 0x2E DEL key	Die and exit to the trading
			ChangeState(TRADE);	// Go to the state TRADE
			if (g_bClient)
				sendToServer_EnterTrade();
			else
			{
				g_pUI.deleteAllItems(ID_LIST_WEAPON);	// Clear weapons
				wchar_t	s[8], sMax[12];
				for (int i = 0; i < 11; i++)
				{
					swprintf_s(s, 8, L"%d", g_Plr[g_id].ammo.n[i]);
					swprintf_s(sMax, 12, L"%d / %d ", g_Plr[g_id].ammo.n[i], g_nBulletsInClip[i]);
					g_pUI.addItem3(ID_LIST_WEAPON, g_sAmmoName[i], s, sMax, i);
				}
			}
			break;
		}

}


//--------------------------------------------------------------------------------------
// MOUSE EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onMouse(bool, bool, bool, bool, bool, int, int, int)	// bLeftButtonDown
{
	// The left mouse button - the usual shooting
	// The right mouse button - barrel-attached grenade launcher

	//g_Xtek = xPos;
	//g_Ytek = yPos;
	switch (g_eState) {	// State machine
	case MENU:	// Menu
		break;

	case REG:	// Registration
		break;

	case WAIT:	// Menu
		break;

	case SERVER:	// Server Menu
		break;

	case GAME:	// Online client game
	case GAMES:
		break;
	}

	//g_bLMouse0 = bLeftButtonDown;
}


//--------------------------------------------------------------------------------------
// GUI EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onGUI(UINT idEvent, int idEl)
{
	switch (idEl)
	{
	case ID_BUTTON_SERVER:	// Start Server
		startServer();
		break;

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
		//ChangeState(GAMES);	// Go to the state GAMES

		wchar_t	s[8];
		for (int i = 0; i < 11; i++)
		{
			swprintf_s(s, 8, L"%d", g_Ammo.n[i]);
			g_pUI.setIndexText2(ID_LIST_WEAPON, i, s);
		}

		ChangeState(TRADES);	// Go to the state TRADES

		/*g_pNet.setAccountAndChat();	// Set work with account and chat on the client (only protocol)
		UINT port;
		swscanf_s(g_pUI.getText(ID_EDIT_PORT), L"%d", &port);
		g_pNet.connectToServer(g_pUI.getText(ID_EDIT_IP), (WORD)port, L"", L"");	// Connect to the Server
		ChangeState(WAIT);	// Go to the state WAIT
		g_pUI.text(ID_EDIT_NAME, g_pUI.getText(ID_EDIT_LOGIN));
		g_pUI.text(ID_EDIT_PASS, g_pUI.getText(ID_EDIT_PASSWORD));
		g_pUI.text(ID_EDIT_LOGIN, L"");
		g_sName[0] = L'\0';
		*/
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

	case ID_BUTTON_EXIT:	// Exit programm
		//g_pUI.clear(ID_LIST_ACCOUNTS);
		//g_pDB.disconnect();
		if (g_eState < TRADE)
			g_pGame.destroy();	// Exit programm
		else
			ChangeState(g_bServer ? SERVER : MENU);	// 
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
			if (g_pUI.getSelectedData(ID_LIST_PLAYERS) == g_acc)	// if (!getSelIndex(ID_LIST_PLAYERS))
				g_pUI.text(ID_STATIC_CHAT, L"Chat with ALL");	// Chat with ALL
			else
			{
				wchar_t	s[40];
				swprintf_s(s, 40, L"Chat with %s", g_pUI.getSelText(ID_LIST_PLAYERS));
				g_pUI.text(ID_STATIC_CHAT, s);	// Chat with player
				//swprintf_s(g_sMsgTitle, 32, L"To %s: ", g_pUI.getSelIndex(ID_LIST_PLAYERS) ? g_pUI.getSelText(ID_LIST_PLAYERS) : L"ALL");	// To Recipient:
			}
		}
		break;

	case ID_EDIT_MESSAGE:	// Chat Message
		//if (idEvent == EVENT_ENTER)	// if was pressed 'Enter'
		//	sendMessage();	// Send message
		break;

	case ID_BUTTON_TRADE: {	// PLAY ON THE SERVER
		ChangeState(TRADES);	// Go to the state TRADES

		/*g_pUI.deleteAllItems(ID_LIST_WEAPON);	// Clear weapons

		wchar_t	s[8], sMax[12];
		for (int i = 0; i < 11; i++)
		{
			swprintf_s(s, 8, L"%d", g_Plr[g_id].ammo.n[i]);
			swprintf_s(sMax, 12, L"%d / %d ", g_Plr[g_id].ammo.n[i], g_nBulletsInClip[i]);
			g_pUI.addItem3(ID_LIST_WEAPON, g_sAmmoName[i], s, sMax, i);
		}
		*/
		
		sendToServer_EnterTrade();
		// If necessary, create the appropriate network message
	}	break;

	case ID_MESSAGE_WAIT:	// Close message with text of error
		if (idEvent == EVENT_CLICK)	// Clicked button Event
			g_pNet.stopConnect();
		break;

	case ID_MESSAGE_ERROR:	// Close message with text of error
		if (idEvent == EVENT_CLICK)	// Clicked button Event
			g_pUI.visible(ID_MESSAGE_ERROR, g_bMsgError = false);
		break;


	case ID_BUTTON_SELL:	// Create transaction - Sell ammo
		if (g_bServer)	// On the Server
		{
			////// Checks, whether there is a given number of weapons, have not exceeded number of trades for this player
			UINT num = 0, money = 0;
			swscanf_s(g_pUI.getText(ID_EDIT_NUMBER), L"%d", &num);
			swscanf_s(g_pUI.getText(ID_EDIT_MONEY), L"%d", &money);
			TRADE_DB trade = { g_idTrade, (UINT)g_acc, _time32(NULL), UINT(NO_ACCOUNT), 0, AMMO_SELL, (BYTE)g_pUI.getSelIndex(ID_LIST_WEAPON), (WORD)num, money };
			g_pDB.add(TABLE_TRADE, (BYTE*)&trade);
			wchar_t	s[96], s2[96];
			swprintf_s(s, 96, L"%d, %d, %d, -1, 0, 1, %d, %d, $%d", trade.id, g_acc, trade.tOpen, trade.iType, num, money);
			g_pUI.addItem(ID_LIST_TRADE, s, g_idTrade);	// 
			swprintf_s(s, 96, L"SELL %d", num);
			swprintf_s(s2, 96, L"$%d", money);
			g_pUI.addItem3(ID_LIST_MYTRANSACT, g_sAmmoName[trade.iType], s, s2, g_idTrade++);

			g_tradeSell[g_iSell].id = trade.id;
			g_tradeSell[g_iSell].accOpen = g_acc;
			g_tradeSell[g_iSell].tOpen = trade.tOpen;
			g_tradeSell[g_iSell].iType = trade.iType;
			g_tradeSell[g_iSell].num = (WORD)num;
			g_tradeSell[g_iSell].nMoney = money;
			g_iSell++;
			g_nSell++;
			// Reserve these weapons
		}
		else	// On the Client
		{

		}
		break;
	case ID_BUTTON_BUY:	// Create transaction - Buy ammo
		if (g_bServer)	// On the Server
		{
			////// Checks, whether there is a given number of money, have not exceeded number of trades for this player
			UINT num = 0, money = 0;
			swscanf_s(g_pUI.getText(ID_EDIT_NUMBER), L"%d", &num);
			swscanf_s(g_pUI.getText(ID_EDIT_MONEY), L"%d", &money);
			TRADE_DB trade = { g_idTrade, (UINT)g_acc, _time32(NULL), UINT(NO_ACCOUNT), 0, AMMO_BUY, (BYTE)g_pUI.getSelIndex(ID_LIST_WEAPON), (WORD)num, money };
			g_pDB.add(TABLE_TRADE, (BYTE*)&trade);
			wchar_t	s[96], s2[96];
			swprintf_s(s, 96, L"%d, %d, %d, -1, 0, 2, %d, %d, $%d", trade.id, g_acc, trade.tOpen, trade.iType, num, money);
			g_pUI.addItem(ID_LIST_TRANSACT, s, g_idTrade);	// 
			swprintf_s(s, 96, L"BUY %d", num);
			swprintf_s(s2, 96, L"$%d", money);
			g_pUI.addItem3(ID_LIST_MYTRANSACT, g_sAmmoName[trade.iType], s, s2, g_idTrade++);

			g_tradeBuy[g_iBuy].id = trade.id;
			g_tradeBuy[g_iBuy].accOpen = g_acc;
			g_tradeBuy[g_iBuy].tOpen = trade.tOpen;
			g_tradeBuy[g_iBuy].iType = trade.iType;
			g_tradeBuy[g_iBuy].num = (WORD)num;
			g_tradeBuy[g_iBuy].nMoney = money;
			g_iBuy++;
			g_nBuy++;
			// Reserve these money
		}
		else	// On the Client
		{

		}
		break;
	case ID_BUTTON_CANCELT:	// Cancel transaction
		break;

	case ID_BUTTON_MAKEDEAL:	// Make a deal
		break;


	case ID_BUTTON_PLAY:	// 
		if (g_bServer)	// 
		{
			g_Plr[g_id].bInGame = true;	// Admin entered the game
			g_Plr[g_id].ammo.nHealth = 100;	// 100% health at the entrance to the game
		}
		
		if (!g_bServer)	// If the server is not initialized
			initPerses();	// 

		activatePersInGame(g_id, ID_TT);

		if (g_bClient)	// If start the game goes on initialized client
			sendToServer_EnterGame();

		ChangeState(g_bClient ? GAME : GAMES);	// Go to the state GAMES

		// If necessary, create the appropriate network message
		break;
	}
}


//--------------------------------------------------------------------------------------
// INITIALIZE
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	g_pGame.addFont(L"Arial", 15, FW_MEDIUM);	// Return ID_FONT_15 = 0, add font for text
	g_pGame.addFont(L"Arial", 30, FW_MEDIUM);	// Return ID_FONT_30 = 1, add font for text

	// Adding start window
	g_pUI.addWindow(ID_FONT_15, L"MENU", 510, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_MENU = 0
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_15, L"DataBase Name:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_SQL_NAME = 1
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_15, L"Prototype0.44b", 200, 0.5f);	// DataBase name, return ID_EDIT_SQL_NAME = 2
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_15, L"SQL Server Name:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_SERVER = 3
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_15, L".\\sqlexpress", 200, 0.5f);	// SQL Server name, return ID_EDIT_SERVER = 4
	//g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_15, L"For SQL Server authentication:", DT_LEFT, COLOR_GOLD, 0.5f);
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_15, L"SQL Server Login:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_SQL_LOGIN = 5
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_15, L"", 200, 0.5f);	// Login for SQL Server authentication, return ID_EDIT_SQL_LOGIN = 6
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_15, L"SQL Server Password:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_SQL_PASS = 7
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_15, L"", 200, 0.5f);	// Password for SQL Server authentication, return ID_EDIT_SQL_PASS = 8
	g_pUI.setHide(ID_EDIT_SQL_PASS, true);							// Hide password
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_15, L"IP:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_IP = 9
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_15, L"127.0.0.1", 200, 0.5f);	// IP, return ID_EDIT_IP = 10
	g_pUI.setIP(ID_EDIT_IP, true);											// IP
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_15, L"Port:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_PORT = 11
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_15, L"1234", 200, 0.5f);	// Port, return ID_STATIC_PORT = 12
	g_pUI.setInt(ID_EDIT_PORT, 65535);									// Max Port = 65535
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_15, L"Name:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_LOGIN = 13
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_15, L"Dmitry", 200, 0.5f);	// Name, return ID_EDIT_LOGIN = 14
	g_pUI.setMaxSymbols(ID_EDIT_LOGIN, 15);							// Name: max 15 symbols (16 with end of the string)
	g_pUI.setMaxPixels(ID_EDIT_LOGIN, 150);							// Name: max 150 pixels
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT_15, L"Password:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_PASSWORD = 15
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT_15, L"Dmitry", 200, 0.5f);	// Password, return ID_EDIT_PASSWORD = 16
	g_pUI.setMaxSymbols(ID_EDIT_PASSWORD, 15);						// Password: max 15 symbols (16 with end of the string)
	g_pUI.setMaxPixels(ID_EDIT_PASSWORD, 150);						// Password: max 150 pixels
	g_pUI.setHide(ID_EDIT_PASSWORD, true);							// Hide password
	g_pUI.addButton(ID_WINDOW_MENU, ID_FONT_15, L"Server", 100, 3, 0.5f);	// Start server, return ID_BUTTON_SERVER = 17
	g_pUI.addButton(ID_WINDOW_MENU, ID_FONT_15, L"Connect", 100, 1, 0.5f);	// Connect to the Server, return ID_BUTTON_CONNECT = 18
	g_pUI.addButton(ID_WINDOW_MENU, ID_FONT_15, L"Registration", 100, 0, 0.5f);	// Registration on the Server, return ID_BUTTON_REG = 19

	// Adding registration window
	g_pUI.addWindow(ID_FONT_15, L"REGISTRATION", 510, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_REG = 20
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_15, L"Name:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_NAME = 21
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_15, L"", 200, 0.5f);	// Name, return ID_EDIT_NAME = 22
	g_pUI.setMaxSymbols(ID_EDIT_NAME, 15);							// Name: max 15 symbols (16 with end of the string)
	g_pUI.setMaxPixels(ID_EDIT_NAME, 150);							// Name: max 150 pixels
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_15, L"Password:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_PASS = 23
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_15, L"", 200, 0.5f);	// Password, return ID_EDIT_PASS = 24
	g_pUI.setMaxSymbols(ID_EDIT_PASS, 15);						// Password: max 15 symbols (16 with end of the string)
	g_pUI.setMaxPixels(ID_EDIT_PASS, 150);						// Password: max 150 pixels
	g_pUI.setHide(ID_EDIT_PASS, true);							// Hide password
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_15, L"E-mail:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_MAIL = 25
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_15, L"", 200, 0.5f);	// E-mail, return ID_EDIT_MAIL = 26
	g_pUI.setMaxSymbols(ID_EDIT_MAIL, 63);
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_15, L"Skype:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_SKYPE = 27
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_15, L"", 200, 0.5f);	// Skype, return ID_EDIT_SKYPE = 28
	g_pUI.setMaxSymbols(ID_EDIT_SKYPE, 63);
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_15, L"Country:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_COUNTRY = 29
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_15, L"", 330, 0.5f);	// Country, return ID_EDIT_COUNTRY = 30
	g_pUI.setMaxSymbols(ID_EDIT_COUNTRY, 63);
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_15, L"City:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_CITY = 31
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_15, L"", 330, 0.5f);	// City, return ID_EDIT_CITY = 32
	g_pUI.setMaxSymbols(ID_EDIT_CITY, 31);
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_15, L"Age:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_AGE = 33
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_15, L"0", 100, 0.5f);	// Age, return ID_EDIT_AGE = 34
	g_pUI.setInt(ID_EDIT_AGE, 99);									// Max Age = 99
	g_pUI.addRadioButton(ID_WINDOW_REG, ID_FONT_15, GROUP_GENDER, L"Male", COLOR_GOLD, 0.5f);	// Male, return ID_RADIO_MALE = 35
	g_pUI.addRadioButton(ID_WINDOW_REG, ID_FONT_15, GROUP_GENDER, L"Female", COLOR_GOLD, 0.5f);	// Female, return ID_RADIO_FEMALE = 36
	g_pUI.addStatic(ID_WINDOW_REG, ID_FONT_15, L"About me:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_MYSELF = 37
	g_pUI.addEditBox(ID_WINDOW_REG, ID_FONT_15, L"", 330, 0.5f);	// About my self, return ID_EDIT_MYSELF = 38
	g_pUI.setMaxSymbols(ID_EDIT_MYSELF, 95);
	g_pUI.addButton(ID_WINDOW_REG, ID_FONT_15, L"Register", 100, 0, 0.5f);	// Register on the Server, return ID_BUTTON_REGISTER = 39

	// Adding admin panel
	g_pUI.addWindow(ID_FONT_15, L"ADMIN PANEL", 510, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_ADMIN = 40
	g_pUI.addStatic(ID_WINDOW_ADMIN, ID_FONT_15, L"Players list:", DT_CENTER, COLOR_GOLD, 0.5f);	// Return ID_STATIC_PLAYERS = 41
	g_pUI.addListBox(ID_WINDOW_ADMIN, ID_FONT_15, 106, 15, 0.5f);	// List of players, return ID_LIST_PLAYERS = 42
	//g_pUI.set3Column(ID_LIST_PLAYERS, 50, 170);		// 3 column: ID, name, password
	g_pUI.addStatic(ID_WINDOW_ADMIN, ID_FONT_15, L"Chat with ALL", DT_CENTER, COLOR_GOLD, 0.5f);	// Chat with ..., return ID_STATIC_CHAT = 43
	g_pUI.addListBox(ID_WINDOW_ADMIN, ID_FONT_15, 300, 15, 0.5f);	// List of chat, return ID_LIST_CHAT = 44
	//g_pUI.set3Column(ID_LIST_ACCOUNTS, 50, 170);		// 3 column: account, name, number of messages
	g_pUI.addStatic(ID_WINDOW_ADMIN, ID_FONT_15, L"Message:", DT_LEFT, COLOR_GOLD, 0.5f);	// Return ID_STATIC_MESSAGE = 45
	g_pUI.addEditBox(ID_WINDOW_ADMIN, ID_FONT_15, L"", 300, 0.5f);	// Edit message, return ID_EDIT_MESSAGE = 46
	g_pUI.setMaxSymbols(ID_EDIT_MESSAGE, 127);
	g_pUI.addStatic(ID_WINDOW_ADMIN, ID_FONT_15, L"Guests: 0", DT_CENTER, COLOR_GOLD, 0.5f);	// Number of guests (not registered clients), return ID_STATIC_GUESTS = 47
	g_pUI.addButton(ID_WINDOW_ADMIN, ID_FONT_15, L"TRADE", 106, 1, 0.5f);	// Trade on the Server, return ID_BUTTON_TRADE = 48
	////// ADD BUTTON FOR VIEW GAME WORLD IN REAL TIME

	g_pUI.addListBox(-1, ID_FONT_15, 320, 5, 0.4f);		// List of IP-addresses, return ID_LIST_IP = 49
	g_pUI.addButton(-1, ID_FONT_15, L"Exit", 100, 2, 0.4f);	// Exit, return ID_BUTTON_EXIT = 50
	g_pUI.addMessage(ID_FONT_15, L"Awaiting a response from the server.", L"Cancel", 500, 160, COLOR_GOLD, true, 0.3f);	// Return ID_MESSAGE_WAIT = 51
	g_pUI.addMessage(ID_FONT_15, L"", L"OK", 500, 160, COLOR_GOLD, true, 0.3f);	// Message with text of error, return ID_MESSAGE_ERROR = 52
	g_pUI.addListBox(-1, ID_FONT_15, 270, 15, 0.4f);	// List of accounts, return ID_LIST_ACC = 53
	g_pUI.addListBox(-1, ID_FONT_15, 800, 5, 0.4f);		// List of registrations, return ID_LIST_REG = 54
	g_pUI.addListBox(-1, ID_FONT_15, 270, 15, 0.4f);	// List of ammo, return ID_LIST_AMMO = 55
	g_pUI.addListBox(-1, ID_FONT_15, 600, 10, 0.4f);	// List of trade,  return ID_LIST_TRADE = 56

	// Adding trade panel
	g_pUI.addWindow(ID_FONT_15, L"TRADE PANEL", 1024, 600, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_TRADE = 56
	g_pUI.addListBox(ID_WINDOW_TRADE, ID_FONT_15, 300, 6, 0.5f);	// List of my open transactions, return ID_LIST_MYTRANSACT = 57
	g_pUI.set3Column(ID_LIST_MYTRANSACT, 100, 100);
	g_pUI.addListBox(ID_WINDOW_TRADE, ID_FONT_15, 300, 9, 0.5f);	// List of ammunition (ammo), return ID_LIST_WEAPON = 58
	g_pUI.set3Column(ID_LIST_WEAPON, 100, 100);
	wchar_t	sMax[12];
	for (int i = 0; i < 11; i++)
	{
		swprintf_s(sMax, 12, L"%d /%d ", g_nAmmoMax[i], g_nBulletsInClip[i]);
		g_pUI.addItem3(ID_LIST_WEAPON, g_sAmmoName[i], L"0", sMax, i);
	}
	g_pUI.addEditBox(ID_WINDOW_TRADE, ID_FONT_15, L"", 50, 0.5f);	// Number of ammo, return ID_EDIT_NUMBER = 59
	g_pUI.addEditBox(ID_WINDOW_TRADE, ID_FONT_15, L"", 50, 0.5f);	// Money for ammo, return ID_EDIT_MONEY = 60
	g_pUI.addButton(ID_WINDOW_TRADE, ID_FONT_15, L"SELL", 70, 2, 0.5f);	// Create transaction - Sell ammo, return ID_BUTTON_SELL = 61
	g_pUI.addButton(ID_WINDOW_TRADE, ID_FONT_15, L"BUY", 70, 1, 0.5f);	// Create transaction - Buy ammo, return ID_BUTTON_BUY = 62
	g_pUI.addButton(ID_WINDOW_TRADE, ID_FONT_15, L"CANCEL", 70, 1, 0.5f);	// Cancel transaction, return ID_BUTTON_CANCELT = 63
	g_pUI.addCheckBox(ID_WINDOW_TRADE, ID_FONT_15, L"Buy ammo", COLOR_GOLD, true, 0.8f);	// Return ID_CHECKBOX_BUY = 64
	g_pUI.addListBox(ID_WINDOW_TRADE, ID_FONT_15, 300, 20, 0.5f);	// List of open transactions, return ID_LIST_TRANSACT = 65
	g_pUI.set3Column(ID_LIST_TRANSACT, 50, 100);
	g_pUI.addButton(ID_WINDOW_TRADE, ID_FONT_15, L"MAKE A DEAL", 150, 1, 0.5f);	// Make a deal, return ID_BUTTON_MAKEDEALT = 66
	g_pUI.addButton(ID_WINDOW_TRADE, ID_FONT_15, L"CONTINUE TO PLAY", 200, 3, 0.5f);	// Continue to play, return ID_BUTTON_PLAY = 67

	g_pUI.addStatic(ID_WINDOW_TRADE, ID_FONT_15, L"List of my open transactions", DT_CENTER, COLOR_GOLD, 0.5f);	// List of my open transactions
	g_pUI.addStatic(ID_WINDOW_TRADE, ID_FONT_15, L"List of weapons", DT_CENTER, COLOR_GOLD, 0.5f);	// List of weapons
	g_pUI.addStatic(ID_WINDOW_TRADE, ID_FONT_15, L"Number:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Number of ammo
	g_pUI.addStatic(ID_WINDOW_TRADE, ID_FONT_15, L"Money:", DT_RIGHT, COLOR_GOLD, 0.5f);	// Money for ammo
	g_pUI.addStatic(ID_WINDOW_TRADE, ID_FONT_15, L"List of open transactions", DT_CENTER, COLOR_GOLD, 0.5f);	// List of open transactions

	ChangeState(MENU);	// Begin state - MENU

	// Create mesh of Ground
	g_pMap.load(L"..\\Content\\Map\\Prototype.txtmap");	// Load the map for the game
	g_pMap.loadPhysics();

	// Sound
	g_pSound.addEx(L"..\\Content\\Sound\\Weapon\\Shot.wav", 5);	// 5 threads for Negev NG7, 0.3f/0.06f, Return ID_SOUND = 0, add sound
	g_pSound.addEx(L"..\\Content\\Sound\\Weapon\\Reload.wav", 1);	// Return ID_SOUND2 = 1, add sound

	//g_pPhys.addPers(500.0f, 0.0f, 500.0f, 0.5f, 2.0f, 100.0f);	// Add own person in the physics subsystem
	//g_pPhys.addPers(495.0f, 0.0f, 500.0f, 0.5f, 2.0f, 100.0f);	// 
	//g_pPhys.addPers(505.0f, 0.0f, 500.0f, 0.5f, 2.0f, 100.0f);	// 
	//g_pPhys.addPers(500.0f, 0.0f, 495.0f, 0.5f, 2.0f, 100.0f);	// 
	//g_pPhys.addPers(500.0f, 0.0f, 505.0f, 0.5f, 2.0f, 100.0f);	// 

	//g_plr[0].weapon = ID_TT;
	//g_plr[1].weapon = ID_AKM;
	//g_plr[2].weapon = ID_AKM;
	//g_plr[3].weapon = ID_TT;
	//g_plr[4].weapon = ID_TT;
}


//--------------------------------------------------------------------------------------
// DEINITIALIZING
//--------------------------------------------------------------------------------------
void CALLBACK onDeinitialize()
{
	if (g_bServer)
		DeleteCriticalSection(&m_csTrade);	// Delete critical section for trading data
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT_15, TEXT_NORMAL, 100, DT_LEFT, 0.2f);	// Return 0, add space for fps
	g_pText.addSpace(ID_FONT_15, TEXT_NORMAL, 100, DT_LEFT, 0.2f);	// Return 1, add space for timer

	g_pText.addSpace(ID_FONT_30, TEXT_STROKE, 200, DT_CENTER, 0.2f);	// Money/score, total
	g_pText.addSpace(ID_FONT_30, TEXT_STROKE, 100, DT_CENTER, 0.2f);	// Health, percent
	g_pText.addSpace(ID_FONT_30, TEXT_STROKE, 100, DT_CENTER, 0.2f);	// Grenades, total
	g_pText.addSpace(ID_FONT_30, TEXT_STROKE, 100, DT_CENTER, 0.2f);	// Mines, total
	g_pText.addSpace(ID_FONT_30, TEXT_STROKE, 100, DT_CENTER, 0.2f);	// Bullets, total
	g_pText.addSpace(ID_FONT_30, TEXT_STROKE, 100, DT_CENTER, 0.2f);	// Bullets, int the clip

	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 64, DT_CENTER, 0.2f);	// TT
	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 64, DT_CENTER, 0.2f);	// Uzi
	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 64, DT_CENTER, 0.2f);	// AKM
	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 64, DT_CENTER, 0.2f);	// M16
	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 64, DT_CENTER, 0.2f);	// Sniper
	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 64, DT_CENTER, 0.2f);	// NG7
	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 64, DT_CENTER, 0.2f);	// RPG

	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 500, DT_LEFT, 0.2f);	// 
	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 500, DT_LEFT, 0.2f);	// 
	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 500, DT_LEFT, 0.2f);	// 
	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 500, DT_LEFT, 0.2f);	// 
	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 500, DT_LEFT, 0.2f);	// 
	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 500, DT_LEFT, 0.2f);	// 
	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 500, DT_LEFT, 0.2f);	// 
	g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 500, DT_LEFT, 0.2f);	// 

	for (int i = 0; i < 10; i++)	// 10 messages
		g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 500, DT_LEFT, 0.2f);	// i message

	for (int i = 0; i < 5; i++)	// 5 players
		g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 800, DT_LEFT, 0.2f);	// i player

	// Textures
	g_pTex.add(L"..\\Content\\Texture\\Ground.dds");	// Return ID_TEXTURE_GROUND = 0
	g_pTex.add(L"..\\Content\\Texture\\Wall.dds");	// Return ID_TEXTURE_WALL = 1
	g_pTex.add(L"..\\Content\\Texture\\Pers.dds");	// Return ID_TEXTURE_PERS = 2
	g_pTex.add(L"..\\Content\\Texture\\Sight.dds");	// Return ID_TEXTURE_SIGHT = 3

	// Create Dynamic Texture of Map
	g_pTex.addDynamic(2048, 2048);	// Return ID_TEXTURE_MAP = 4
	UINT* map = new UINT[4194304];	// 2048 * 2048
	for (int i = 0; i < 4194304; i++)
		map[i] = 0x8060ff60;	// Green field
	//*map++ = 0xff00ff00;	// Green field
	for (int id = 0; id < g_pMap.getNWalls(); id++)
	{
		for (int i = int(2.0f*g_pMap.getWZ(id)); i < int(2.0f*(g_pMap.getWZ(id) + g_pMap.getWW(id))); i++)
			for (int j = int(2.0f*g_pMap.getWX(id)); j < int(2.0f*(g_pMap.getWX(id) + g_pMap.getWL(id))); j++)
				map[2048 * i + j] = 0x806060ff;	// Red field
	}
	g_pTex.setMap(ID_TEXTURE_MAP, map);	// Set texture map
	delete[] map;

	// 3D initialization
	g_p3D.init(1.0f, 0.0f, 0.3f, 0.5f, 1.5f, 0.1f, 1.5f);	// 3D initialization
	g_pCam.setType(CAMERA_THIRDPERSON);			// Set the third person camera
	g_pCam.setPosition(500.0f, 1.5f, 500.0f);	// Set the camera position

	// Load mesh, including animated 
	g_p3D.addMesh(L"..\\Content\\Mesh\\Weapon\\TT\\TT.sdkmesh");	// Add TT, return ID_TT = 0
	g_p3D.addMesh(L"..\\Content\\Mesh\\Weapon\\Uzi\\Uzi.sdkmesh");	// Add Uzi, return ID_UZI = 1
	g_p3D.addMesh(L"..\\Content\\Mesh\\Weapon\\AKM\\AKM.sdkmesh");	// Add AKM, return ID_AKM = 2
	g_p3D.addMesh(L"..\\Content\\Mesh\\Weapon\\M16\\M16.sdkmesh");	// Add M16, return ID_M16 = 3
	g_p3D.addMesh(L"..\\Content\\Mesh\\Weapon\\Sniper\\Sniper.sdkmesh");	// Add Sniper, return ID_SNIPER = 4
	g_p3D.addMesh(L"..\\Content\\Mesh\\Weapon\\Negev_NG7\\Negev_NG7.sdkmesh");	// Add NG7, return ID_NG7 = 5
	g_p3D.addMesh(L"..\\Content\\Mesh\\Weapon\\RPG\\RPG.sdkmesh");	// Add RPG, return ID_RPG = 6
	g_p3D.addMesh(L"..\\Content\\Mesh\\Weapon\\Grenade\\Grenade.sdkmesh");	// Add Grenade, return ID_GRENADE = 7
	g_p3D.addMesh(L"..\\Content\\Mesh\\Weapon\\Mine\\Mine.sdkmesh");	// Add Mine, ID_MINE = 8
	g_p3D.addMeshAnim(L"..\\Content\\Mesh\\Soldier\\soldier.sdkmesh", L"..\\Content\\Mesh\\Soldier\\soldier.sdkmesh_anim");	// Add Mesh with Animation, return ID_MESH_SOLDIER = 9

	// Create PLAYING MAP
	g_p3D.addVertex(g_pMap.getVertexGround(), g_pMap.getNVertexGround() << 5);	// Create vertex buffer of ground, return ID_VERTEXBUFFER_GROUND = 0
	g_p3D.addIndex(g_pMap.getIndexGround(), g_pMap.getNIndexGround() << 1);		// Create index buffer of ground, return ID_INDEXBUFFER_GROUND = 1
	g_p3D.addVertex(g_pMap.getVertexWalls(), g_pMap.getNVertexWalls() << 5);	// Create vertex buffer of walls, return ID_VERTEXBUFFER_WALLS = 2
	g_p3D.addIndex(g_pMap.getIndexWalls(), g_pMap.getNIndexWalls() << 1);		// Create index buffer of walls, return ID_INDEXBUFFER_WALLS = 3
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int w, int h)
{
	int hw = w >> 1;	// Half width
	int hh = h >> 1;	// Half height

	g_pText.pos(0, 5, 2);	// fps
	g_pText.pos(1, 105, 2);	// timer

	g_pText.pos(2, w - 100, 5);			// Money/score, total
	g_pText.pos(3, w - 50, h - 70);		// Health, percent
	g_pText.pos(4, w - 350, h - 35);	// Grenades, total
	g_pText.pos(5, w - 250, h - 35);	// Mines, total
	g_pText.pos(6, w - 150, h - 35);	// Bullets, total
	g_pText.pos(7, w - 50, h - 35);		// Bullets, in the clip

	g_pText.pos(8, hw - 192, 5);	// TT
	g_pText.pos(9, hw - 128, 5);	// Uzi
	g_pText.pos(10, hw - 64, 5);	// AKM
	g_pText.pos(11, hw, 5);			// M16
	g_pText.pos(12, hw + 64, 5);	// Sniper
	g_pText.pos(13, hw + 128, 5);	// NG7
	g_pText.pos(14, hw + 192, 5);	// RPG

	g_pText.pos(15, 705, 350);
	g_pText.pos(16, 705, 370);
	g_pText.pos(17, 705, 390);
	g_pText.pos(18, 705, 410);
	g_pText.pos(19, 705, 430);
	g_pText.pos(20, 705, 450);
	g_pText.pos(21, 705, 470);
	g_pText.pos(22, 705, 490);

	for (int i = 0; i < 10; i++)	// 10 messages
		g_pText.pos(23 + i, 5, 230 + 16*i);

	for (int i = 0; i < 5; i++)	// 5 players
		g_pText.pos(33 + i, 5, 430 + 16 * i);

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

	g_pUI.pos(ID_WINDOW_ADMIN, hw - 250, hh - 225);		// ADMIN PANEL
	g_pUI.pos(ID_STATIC_PLAYERS, hw + 153, hh - 150);	// Players list
	g_pUI.pos(ID_LIST_PLAYERS, hw + 100, hh - 134);		// List of players
	g_pUI.pos(ID_STATIC_CHAT, hw - 103, hh - 150);		// Chat with ...
	g_pUI.pos(ID_LIST_CHAT, hw - 206, hh - 134);		// List of chat
	g_pUI.pos(ID_STATIC_MESSAGE, hw - 206, hh + 130);	// Message
	g_pUI.pos(ID_EDIT_MESSAGE, hw - 206, hh + 146);		// Edit message
	g_pUI.pos(ID_STATIC_GUESTS, hw + 153, hh + 118);	// Guests
	g_pUI.pos(ID_BUTTON_TRADE, hw + 100, hh + 146);		// Trade on the Server

	g_pUI.pos(ID_LIST_IP, 2, 20);						// List of IP-addresses
	g_pUI.pos(ID_BUTTON_EXIT, w - 104, h - 36);			// Exit
	g_pUI.pos(ID_MESSAGE_WAIT, hw - 250, hh - 80);		// Wait
	g_pUI.pos(ID_MESSAGE_ERROR, hw - 250, hh - 80);		// Error
	g_pUI.pos(ID_LIST_ACC, hw + 240, hh - 134);			// List of accounts
	g_pUI.pos(ID_LIST_REG, hw - 400, hh + 208);			// List of registrations
	g_pUI.pos(ID_LIST_AMMO, 0, hh - 134);				// List of ammo
	g_pUI.pos(ID_LIST_TRADE, 330, 0);					// List of trade

	g_pUI.pos(ID_WINDOW_TRADE, hw - 512, hh - 300);		// TRADE PANEL (trade ammunition)
	g_pUI.pos(ID_LIST_MYTRANSACT, hw - 150, hh - 200);	// List of my open transactions (max 5 buy + 5 sales)
	g_pUI.pos(ID_LIST_WEAPON, hw - 150, hh - 50);		// List of ammunition (ammo) (type, number/amount)
	g_pUI.pos(ID_EDIT_NUMBER, hw - 80, hh + 120);		// Number of ammunitions
	g_pUI.pos(ID_EDIT_MONEY, hw + 70, hh + 120);		// Money for ammo
	g_pUI.pos(ID_BUTTON_SELL, hw - 140, hh + 170);		// Create transaction - Sell ammo
	g_pUI.pos(ID_BUTTON_BUY, hw - 35, hh + 170);		// Create transaction - Buy ammo
	g_pUI.pos(ID_BUTTON_CANCELT, hw + 70, hh + 170);	// Cancel transaction

	g_pUI.pos(ID_CHECKBOX_BUY, hw + 260, hh - 220);		// Buy ammo
	g_pUI.pos(ID_LIST_TRANSACT, hw + 160, hh - 180);	// List of open transactions 
	g_pUI.pos(ID_BUTTON_MAKEDEAL, hw + 235, hh + 170);	// Make a deal
	g_pUI.pos(ID_BUTTON_PLAY, hw + 55, hh + 220);		// Continue to play

	g_pUI.pos(ID_STATIC_MYTRANS, hw, hh - 220);			// List of my open transactions
	g_pUI.pos(ID_STATIC_WEAPON, hw, hh - 70);			// List of weapons
	g_pUI.pos(ID_STATIC_NUMBER, hw - 85, hh + 126);		// Number of ammo
	g_pUI.pos(ID_STATIC_MONEY, hw + 65, hh + 126);		// Money for ammo
	g_pUI.pos(ID_STATIC_TRANS, hw + 310, hh - 200);		// List of open transactions

	// Map sprites
	g_p2D.addF(0.0f / 2048.0f, 200.0f / 2048.0f, 200.0f / 2048.0f, 0.0f / 2048.0f, 0.0f, 200.0f, 0.0f, 200.0f, 0.9f);	// Map, return 0
	g_p2D.addF(0.0f / 16.0f, 7.0f / 16.0f, 0.0f / 16.0f, 7.0f / 16.0f, 99.0f, 106.0f, 117.0f, 124.0f, 0.8f);	// Own pers, return 1
	g_p2D.addF(0.0f, 1.0f, 0.0f, 1.0f, (float)hw - 32.0f, (float)hw + 32.0f, (float)hh - 32.0f, (float)hh + 32.0f, 0.5f);	// Sight, return 2
}


//--------------------------------------------------------------------------------------
// UPDATES TO THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameMove(double, float)	// time, fTime
{
	if (g_eState > TRADES)	// If the GAME
	{
		g_plr1[g_id].bFire = false;	// By default, there were no shots in the current frame

		if (!g_Ammo.i[g_weapon] && g_time > g_tReloadEnd)	// If there are no bullets in the clip and reload the gun finished
		{
			g_tShot = g_time - g_tDelta;	// -g_tNewShot[g_weapon] - 0.001f
			if (g_Ammo.n[g_weapon] < g_nBulletsInClip[g_weapon])	// If bullets remains less than the capacity of the clip
			{
				g_Ammo.i[g_weapon] = (BYTE)g_Ammo.n[g_weapon];
				g_Ammo.n[g_weapon] = 0;
			}
			else	// If there is enough bullets to completely fill the clip
			{
				g_Ammo.i[g_weapon] = g_nBulletsInClip[g_weapon];
				g_Ammo.n[g_weapon] -= g_nBulletsInClip[g_weapon];
			}
		}

		if (g_pGame.getLMouseClick() && g_eState > TRADES && g_tShot < g_time)	// The first shot in the queue (during the game)
		{
			if (g_tShot < g_time - g_tNewShot[g_weapon] && g_Ammo.i[g_weapon])	// If the last shot was a "long time" and the bullet is in the clip, then make the shot
			{
				g_pSound.playWithReset(SOUND_SHOT);	// Play a sound with reset sound device
				g_tShot = g_time + g_tNewShot[g_weapon] - g_tDelta;	// Time of permission for next shot
				if (!--g_Ammo.i[g_weapon])	// Take away one bullet, which shot; if bullets have finished, then start reloading weapons
					g_tReloadEnd = g_time + g_tReload;

				g_plr1[g_id].bFire = true;	// Shot in the current frame
				g_pPhys.getPersViewNormal(g_id, &g_nxFire, &g_nyFire, &g_nzFire);	// Get XYZ-normal of person sight
				float x = g_pPhys.getPersX(g_id), y = 1.2f, z = g_pPhys.getPersZ(g_id);	// Weapon position
				g_pPhys.getBulletDirectionForThirdPersonCamera(x, y, z, &g_nxFire, &g_nyFire, &g_nzFire);	// Get the bullet direction
				g_pPhys.addBullet(x + g_nxFire, y + g_nyFire, z + g_nzFire, g_nxFire, g_nyFire, g_nzFire, g_damage[g_weapon]);	// Add bullet to physics subsystem
				wchar_t	sMsg[96];
				swprintf_s(sMsg, 96, L"Departure of bullet: %.02f  %.02f  %.02f  %.02f  %.02f  %.02f", x, y, z, g_nxFire, g_nyFire, g_nzFire);	// Departure of bullet
				addMessage(sMsg);
				// When the camera from a third party: finding the target center point, the calculation of the normal of the flighting bullet relative to the point of gun and the calculated point of a gun shot
				// UZI - 10 per second
				// AKM - 10 per second
				// M16 - 15 per second
			}
		}
		else if (g_pGame.getLMousePress() && g_eState > TRADES && g_tShot < g_time && g_Ammo.i[g_weapon])	// Subsequent shots in the queue (during the game)
		{
			g_pSound.playWithReset(SOUND_SHOT);	// Play a sound with reset sound device
			g_tShot += g_tNewShot[g_weapon];
			if (!--g_Ammo.i[g_weapon])	// Take away one bullet, which shot; if bullets have finished, then start reloading weapons
				g_tReloadEnd = g_time + g_tReload;

			//float nx, ny, nz;
			//g_p3D.getRayDirectionFromScreenPoint(xPos, yPos, &nx, &ny, &nz);
			g_pPhys.getPersViewNormal(g_id, &g_nxFire, &g_nyFire, &g_nzFire);	// Get XYZ-normal of person sight
			float x = g_pPhys.getPersX(g_id) + g_nxFire, y = 1.2f + g_nyFire, z = g_pPhys.getPersZ(g_id) + g_nxFire;	// Weapon position
			g_pPhys.getBulletDirectionForThirdPersonCamera(x, y, z, &g_nxFire, &g_nyFire, &g_nzFire);	// Get the bullet direction
			g_pPhys.addBullet(x + g_nxFire, y + g_nyFire, z + g_nzFire, g_nxFire, g_nyFire, g_nzFire, g_damage[g_weapon]);	// Add bullet to physics subsystem
			wchar_t	sMsg[96];
			swprintf_s(sMsg, 96, L"Departure of bullet: %.02f  %.02f  %.02f  %.02f  %.02f  %.02f", x, y, z, g_nxFire, g_nyFire, g_nzFire);	// Departure of bullet
			addMessage(sMsg);
		}
	}
	/*switch (g_weapon) {
	case ID_TT:	// TT
	case ID_UZI:	// Uzi
	case ID_AKM:	// AKM
	case ID_M16:	// M16
	case ID_SNIPER:	// Sniper
	case ID_NG7:	// Negev NG7
	*/
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double time, float fTime)
{
	g_pGame.clearScreen(g_cClear);

	g_time = (float)time;

	// Physics subsystem
	//if (g_bServer || g_bClient)	// 
	{
		g_pPhys.objectProcessing();	// Processing interaction of all objects
		g_pPhys.setCameraAfterObjectProcessing();	// Adjust the camera position after processing
	}
	
	if (g_bServer)	// If the server is up and running, then each frame send a message on the status of players 
		sendToClients_EveryFrame();

	// Printing parameters for first 5 players
	for (WORD id = 0; id < 5; id++)
		if (g_pPhys.getPersActive(id))
		{
			swprintf_s(g_sPers[id], 96, L"pos=%.02f  %.02f  %.02f,  sa=%.02f,  nor=%.02f  %.02f  %.02f,  sa=%.02f, speed=%d",
				g_pPhys.getPersX(id), g_pPhys.getPersY(id), g_pPhys.getPersZ(id), g_pPhys.getPersSA(id), g_pPhys.getPersNX(id), g_pPhys.getPersNY(id), g_pPhys.getPersNZ(id), g_pPhys.getPersNA(id), g_pPhys.getPersSpeedType(id));	// Contact the bullet or the end of flight
			g_pText.dynamic(ID_FONT_15, 18 + id, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sPers[id]);	// 
		}

	// Game messages
	for (int i = 0; i < 10; i++)
		g_pText.dynamic(ID_FONT_15, 23 + i, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sMessages[i]);	// 

	if (g_bServer)	// On the server
	{
		g_Plr[g_id].x = g_pPhys.getPersX(g_id);
		g_Plr[g_id].y = g_pPhys.getPersY(g_id);
		g_Plr[g_id].z = g_pPhys.getPersZ(g_id);
		
		for (int id = 0; id < 5; id++)
		{
			swprintf_s(g_sPlayers[id], 256, L"%d %d %d %.01f %.01f %.01f %d %d L=%d $%d %d Ammo: %d/%d %d/%d %d/%d %d/%d %d/%d %d/%d %d %d %d %d %d", id, g_Plr[id].bActive, g_Plr[id].bInGame, 
				g_Plr[id].x, g_Plr[id].y, g_Plr[id].z, g_Plr[id].weapon, g_Plr[id].bFire, g_Plr[id].ammo.level, g_Plr[id].ammo.nMoney, g_Plr[id].ammo.nHealth,
				g_Plr[id].ammo.n[0], g_Plr[id].ammo.i[0], g_Plr[id].ammo.n[1], g_Plr[id].ammo.i[1], g_Plr[id].ammo.n[2], g_Plr[id].ammo.i[2], g_Plr[id].ammo.n[3], g_Plr[id].ammo.i[3],
				g_Plr[id].ammo.n[4], g_Plr[id].ammo.i[4], g_Plr[id].ammo.n[5], g_Plr[id].ammo.i[5], g_Plr[id].ammo.n[6], g_Plr[id].ammo.n[7], g_Plr[id].ammo.n[8], g_Plr[id].ammo.n[9], g_Plr[id].ammo.n[10]);
			g_pText.dynamic(ID_FONT_15, 33 + id, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sPlayers[id]);	// Player parameters
		}
	}

	if (g_eState == GAME)
	{
	}

	g_pText.fps(ID_FONT_15, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT_15, 1, DT_LEFT, COLOR_ORANGE);	// timer

	switch (g_eState) {	// State machine
	case MENU:	// Menu
		break;

	case REG:	// Registration
		break;

	case WAIT:	// Menu
		break;

	case SERVER:	// Server Menu
		break;

	case GAME:	// Online client game
		//if (g_bClient)
		//	sendToServer_Action(fTime);	// Action
	case GAMES:	// Online server game
	{
		sendToServer_Action(fTime);	// Action
		swprintf_s(g_sMoney, 12, L"$%d", g_Ammo.nMoney);	// Money/score, total
		swprintf_s(g_sHealth, 8, L"%d%%", g_Ammo.nHealth);	// Health, percent
		swprintf_s(g_sNGrenades, 4, L"%d", g_Ammo.n[7]);	// Grenades, total
		swprintf_s(g_sNMines, 4, L"%d", g_Ammo.n[8]);		// Mines, total
		swprintf_s(g_sNBulletsAll, 8, L"%d", g_Ammo.n[g_plr1[g_id].weapon]);	// Bullets, total
		swprintf_s(g_sNBulletsClip, 4, L"%d", g_Ammo.i[g_plr1[g_id].weapon]);	// Bullets, in the clip
		if (g_plr1[g_id].weapon == ID_AKM)
			swprintf_s(g_sNBulletsAll, 8, L"%s/%d", g_sNBulletsAll, g_Ammo.n[9]);	// AKM grenades, total
		if (g_plr1[g_id].weapon == ID_M16)
			swprintf_s(g_sNBulletsAll, 8, L"%s/%d", g_sNBulletsAll, g_Ammo.n[10]);	// M16 grenades, total

		g_pText.dynamic(ID_FONT_30, 2, TEXT_STROKE, DT_CENTER, COLOR_ORANGE, g_sMoney);	// Money/score, total
		g_pText.dynamic(ID_FONT_30, 3, TEXT_STROKE, DT_CENTER, COLOR_ORANGE, g_sHealth);	// Health, percent
		g_pText.dynamic(ID_FONT_30, 4, TEXT_STROKE, DT_CENTER, COLOR_ORANGE, g_sNGrenades);	// Grenades, total
		g_pText.dynamic(ID_FONT_30, 5, TEXT_STROKE, DT_CENTER, COLOR_ORANGE, g_sNMines);	// Mines, total
		g_pText.dynamic(ID_FONT_30, 6, TEXT_STROKE, DT_CENTER, COLOR_ORANGE, g_sNBulletsAll);	// Bullets, total
		g_pText.dynamic(ID_FONT_30, 7, TEXT_STROKE, DT_CENTER, COLOR_ORANGE, g_sNBulletsClip);	// Bullets, in the clip

		for (int i = 0; i < 6; i++)
		{
			swprintf_s(g_sAmmo[i], 8, L"%d/%d", g_Ammo.n[i], g_Ammo.i[i]);
			g_pText.dynamic(ID_FONT_15, 8 + i, TEXT_SHADOW, DT_CENTER, COLOR_ORANGE, g_sAmmo[i]);
		}
		swprintf_s(g_sAmmo[6], 8, L"%d", g_Ammo.n[6]);
		g_pText.dynamic(ID_FONT_15, 14, TEXT_SHADOW, DT_CENTER, COLOR_ORANGE, g_sAmmo[6]);

		swprintf_s(g_sPosition, 96, L"%.02f  %.02f  %.02f  %.02f %.02f  %.02f  %.02f", g_pPhys.getPersX(g_id), g_pPhys.getPersY(g_id), g_pPhys.getPersZ(g_id), g_pPhys.getPersNA(g_id), g_pPhys.getPersNX(g_id), g_pPhys.getPersNY(g_id), g_pPhys.getPersNZ(g_id));	// Contact the bullet or the end of flight
		g_pText.dynamic(ID_FONT_15, 15, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sPosition);	// 
		g_pText.dynamic(ID_FONT_15, 16, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sBulletBeg);	// 
		g_pText.dynamic(ID_FONT_15, 17, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sBulletEnd);	// 

		g_p3D.setRender3D();	// Set 3D render

		// Render map (ground and walls)
		g_pTex.render(ID_TEXTURE_GROUND);
		g_p3D.renderIndex(ID_VERTEXBUFFER_GROUND, ID_INDEXBUFFER_GROUND, 0.0f, 0.0f, 0.0f);
		g_pTex.render(ID_TEXTURE_WALL);
		g_p3D.renderIndex(ID_VERTEXBUFFER_WALLS, ID_INDEXBUFFER_WALLS, 0.0f, 0.0f, 0.0f);

		//g_p3D.renderMeshTex(ID_TT, 500.0f, 1.0f, 500.0f);	// TT
		//g_p3D.renderMeshTex(ID_AKM, 501.0f, 1.0f, 500.0f);	// AKM

		for (WORD id = 0; id < g_pPhys.getNPers(); id++)
			if (g_pPhys.getPersActive(id))
				g_p3D.renderMeshTexA(g_plr1[id].weapon, g_pPhys.getPersX(id) + 0.5f * sinf(g_pPhys.getPersNA(id)), g_pPhys.getPersY(id) + g_scale / 1.5f, g_pPhys.getPersZ(id) + 0.5f * cosf(g_pPhys.getPersNA(id)), g_pPhys.getPersNA(id));	// TT

		// Render soldiers
		//g_p3D.renderMeshAnim(ID_MESH_SOLDIER, x, 0.0f, z, fAlpha);
		for (WORD id = 0; id < g_pPhys.getNPers(); id++)
			if (g_pPhys.getPersActive(id))
				g_p3D.renderMeshAnimScale(ID_MESH_SOLDIER, g_pPhys.getPersX(id), g_pPhys.getPersY(id), g_pPhys.getPersZ(id), g_pPhys.getPersNA(id) - 0.12f, g_scale);	// 2.76f - 2 meters, 0.12f - uneven mesh

		// Render Map
		g_p2D.updateF(0, (g_pPhys.getPersX(g_id) - 50.0f) / 1024.0f, (g_pPhys.getPersX(g_id) + 50.0f) / 1024.0f, (g_pPhys.getPersZ(g_id) + 50.0f) / 1024.0f, (g_pPhys.getPersZ(g_id) - 50.0f) / 1024.0f, 2.0f, 202.0f, 20.0f, 220.0f, 0.9f);	// Texture
		g_p2D.updateBuffer();	// Update sprite buffer after all sprite updates
		g_pGame.setRenderSprites(RENDER_ONLY_TEXTURE, ID_TEXTURE_MAP);
		g_p2D.draw(0, 1);
		g_pTex.render(ID_TEXTURE_PERS);
		g_p2D.draw(1, 1);
		g_pTex.render(ID_TEXTURE_SIGHT);
		g_p2D.draw(2, 1);

		g_pGame.setRenderTexts();
		g_pText.draw(2, 13);	// Additional information

	}	break;

	case TRADE:	// Trading on the client game
		break;

	case TRADES:	// Trading on the server game
		break;
	}

	// Render UI
	g_pGame.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pUI.render();	// Render UI

	// Render text
	g_pGame.setRenderTexts();
	g_pText.draw(0, 2);	// FPS, time
	g_pText.draw(15, 8);	// Additional information
	g_pText.draw(23, 10);	// Messages
	g_pText.draw(33, 5);	// Players
}



//--------------------------------------------------------------------------------------
// CONNECT TO THE NETWORK
//--------------------------------------------------------------------------------------
void CALLBACK onConnect(DWORD nid, WORD id, byte iAuthError)
{
	if (g_bServer)	// If server
	{
		// Check the registration on the server
		if (id > MAX_PLR && iAuthError)
		{
			//wchar_t s[16];
			//swprintf_s(s, 16, L"Guests: %d", ++g_nGuests);
			//g_pUI.text(ID_STATIC_GUESTS, s);
		}
		else
		{
			//activatePersInGame(id, ID_TT);
			// Send new player params to all players
		}
	}
	else	// Client
	{
		g_nidServer = nid;	// Network Server ID
		//g_id = id;
		//memcpy(&g_msg[2], &id, 2);
	}
}


//--------------------------------------------------------------------------------------
// DISCONNECT FROM THE NETWORK
//--------------------------------------------------------------------------------------
void CALLBACK onDisconnect(DWORD acc, byte nReason)	// NOTE: if authorization and chat has been added, then the function will use the account (DWORD acc)
{
	if (g_bServer)	// If server
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
// CONTACT OF BULLETS EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onBullet(WORD idBullet, byte typeObject, WORD idObject, float x, float y, float z)
{
	wchar_t	sMsg[96];
	swprintf_s(sMsg, 96, L"Contact bullet:  %.02f  %.02f  %.02f  %i  %i  %i", x, y, z, idBullet, typeObject, idObject);	// Contact bullet
	addMessage(sMsg);

	if (typeObject == OBJ_PERS)
	{
		if (!g_pPhys.getPersHealth(idObject))	// If a peson is killed, then remove it
		{
		}
	}
}