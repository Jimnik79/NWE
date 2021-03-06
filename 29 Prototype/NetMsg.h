//--------------------------------------------------------------------------------------
// File: Game.h
// Description: Class of Prototype game.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// 100 money for the initialization of the transaction + 10% for closed the transaction, that not to abuse the work with transactions, the transaction hangs on the server no more than 60 minutes.


#define	MSG_ENTERTRADE	50	// Enter the Trade
#define	MSG_PLAYERAMMO	51	// Player Ammo and existing commercial offerings/transactions (their own and others)
#define	MSG_ENTERGAME	52	// Enter the Game
#define	MSG_EVERYFRAME	53	// Every frame message
#define	MSG_ACTION		54	// Every frame action on the client

#define	MSG_SELLAMMO	60	// Request to the server for sale of ammunition
#define	MSG_BUYAMMO		61	// Request to the server for buy of ammunition
#define	MSG_AMMOVALID	62	// Response to the request for the sale/buy of ammunition (0 - error, 1 - confirmation of the creation, 2 - the conclusion of the transaction, 3 - the abolition of the transaction, 4 - confirmation of acceptance, 5 - end of time)
#define	MSG_BIDCANCEL	63	// Cancel the transaction, bail to return
#define	MSG_MAKEDEAL	64	// Accept offer

#define	MSG_NEWBID		65	// Add the transaction to all those in the trade
#define	MSG_DELBID		66	// Delete the transaction to all those in the trade


void sendToClient_PlayerAmmo(WORD id);

//--------------------------------------------------------------------------------------
// SELL AMMO
//--------------------------------------------------------------------------------------
void sendToServer_SellAmmo(BYTE iType, WORD num, UINT money)
{
	g_msg[0] = MSG_SELLAMMO;	// g_msg[1] = 0;	// Sell Ammo
	g_msg[4] = iType;
	memcpy(&g_msg[5], &num, 2);
	memcpy(&g_msg[7], &money, 4);
	g_pNet.msg(g_nidServer, g_msg, 11);
}

void msgSellAmmo(BYTE *msg)	// Sell Ammo
{
	WORD id;
	memcpy(&id, &msg[2], 2);
	if (id < MAX_PLR)	// Additional verification
	{
		UINT acc = g_Plr[id].acc;
		TRADE_DB trade = { g_idTrade, g_Plr[id].acc, _time32(NULL), UINT(NO_ACCOUNT), 0, AMMO_SELL, msg[4] };
		memcpy(&trade.num, &msg[5], 2);
		memcpy(&trade.nMoney, &msg[7], 4);
		g_pDB.add(TABLE_TRADE, (BYTE*)&trade);

		////// Check similarly counter offer of buy, if yes, then auto-deal

		wchar_t	s[96], s2[16];// , s3[16];
		swprintf_s(s, 96, L"%d: %d, %d, 0, 0, SELL, %d, %d, $%d", g_idTrade, acc, trade.tOpen, msg[4], trade.num, trade.nMoney);
		g_pUI.addItem(ID_LIST_TRADE, s, g_idTrade);	// Add trade item to the trade list
		swprintf_s(s, 16, L"%d", trade.num);
		swprintf_s(s2, 16, L"$%d", trade.nMoney);
		g_pUI.addItem3(ID_LIST_TRANSACT, g_sAmmoName[trade.iType], s, s2, g_idTrade);

		g_tradeSell[g_iSell].id = trade.id;
		g_tradeSell[g_iSell].accOpen = acc;
		g_tradeSell[g_iSell].tOpen = trade.tOpen;
		g_tradeSell[g_iSell].iType = msg[4];
		g_tradeSell[g_iSell].num = trade.num;
		g_tradeSell[g_iSell].nMoney = trade.nMoney;
		g_iSell++;
		g_nSell++;
	}
}


//--------------------------------------------------------------------------------------
// BUY AMMO
//--------------------------------------------------------------------------------------
void sendToServer_BuyAmmo(BYTE iType, WORD num, UINT money)
{
	g_msg[0] = MSG_BUYAMMO;	// g_msg[1] = 0;	// Buy Ammo
	g_msg[4] = iType;
	memcpy(&g_msg[5], &num, 2);
	memcpy(&g_msg[7], &money, 4);
	g_pNet.msg(g_nidServer, g_msg, 11);
}

void msgBuyAmmo(BYTE *msg)	// Buy Ammo
{
	WORD id;
	memcpy(&id, &msg[2], 2);
	if (id < MAX_PLR)	// Additional verification
	{
		UINT acc = g_Plr[id].acc;
		TRADE_DB trade = { g_idTrade, g_Plr[id].acc, _time32(NULL), UINT(NO_ACCOUNT), 0, AMMO_BUY, msg[4] };
		memcpy(&trade.num, &msg[5], 2);
		memcpy(&trade.nMoney, &msg[7], 4);
		g_pDB.add(TABLE_TRADE, (BYTE*)&trade);

		////// Check similarly counter offer of sell, if yes, then auto-deal

		wchar_t	s[96], s2[16];// , s3[16];
		swprintf_s(s, 96, L"%d: %d, %d, 0, 0, BUY, %d, %d, $%d", g_idTrade, acc, trade.tOpen, msg[4], trade.num, trade.nMoney);
		g_pUI.addItem(ID_LIST_TRADE, s, g_idTrade);	// Add trade item to the trade list
		swprintf_s(s, 16, L"%d", trade.num);
		swprintf_s(s2, 16, L"$%d", trade.nMoney);
		g_pUI.addItem3(ID_LIST_TRANSACT, g_sAmmoName[trade.iType], s, s2, g_idTrade);

		g_tradeBuy[g_iBuy].id = trade.id;
		g_tradeBuy[g_iBuy].accOpen = acc;
		g_tradeBuy[g_iBuy].tOpen = trade.tOpen;
		g_tradeBuy[g_iBuy].iType = msg[4];
		g_tradeBuy[g_iBuy].num = trade.num;
		g_tradeBuy[g_iBuy].nMoney = trade.nMoney;
		g_iBuy++;
		g_nBuy++;
	}
}


//--------------------------------------------------------------------------------------
// AMMO VALID
//--------------------------------------------------------------------------------------
void sendToServer_AmmoValid(WORD id, UINT idTrade, BYTE iValid)
{
	BYTE msgAV[7];
	msgAV[0] = MSG_BUYAMMO;	// g_msg[1] = 0;	// Buy Ammo
	msgAV[1] = 0;
	memcpy(&msgAV[2], &idTrade, 4);
	msgAV[6] = iValid;
	g_pNet.msg(g_Plr[id].nid, msgAV, 7);
}

void msgAmmoValid(BYTE *msg)	// Ammo Valid
{
	UINT idTrade;
	memcpy(&idTrade, &msg[2], 4);

	if (msg[6] == 1)	// iValid
	{
		//swprintf_s(s, 16, L"SELL %d", pTrade[i].num);
		//swprintf_s(s2, 16, L"$%d", pTrade[i].nMoney);
		//g_pUI.addItem3(ID_LIST_MYTRANSACT, g_sAmmoName[pTrade[i].iType], s, s2, pTrade[i].id);
	}
}






//--------------------------------------------------------------------------------------
// ENTER THE TRADE
//--------------------------------------------------------------------------------------
void sendToServer_EnterTrade()
{
	g_msg[0] = MSG_ENTERTRADE;	// g_msg[1] = 0;	// Enter the Trade
	g_pNet.msg(g_nidServer, g_msg, 4);
}

void msgEnterTrade(BYTE *msg)	// Enter the Trade
{
	WORD id;
	memcpy(&id, &msg[2], 2);
	if (id < MAX_PLR)	// Additional verification
	{
		g_Plr[id].bInGame = false;	// Came out of the game to the trade
		for (int i = 0; i < 6; i++)	// Clear clip
		{
			g_Plr[id].ammo.n[i] += g_Plr[id].ammo.i[i];	// Bullets, in stock
			g_Plr[id].ammo.i[i] = 0;	// Bullets, in clip
		}
		sendToClient_PlayerAmmo(id);
	}
}


//--------------------------------------------------------------------------------------
// PLAYER'S AMMO
//--------------------------------------------------------------------------------------
void sendToClient_PlayerAmmo(WORD id)
{
	BYTE msgPA[1000];
	msgPA[0] = MSG_PLAYERAMMO;	// Player's ammo
	msgPA[1] = 0;
	memcpy(&msgPA[2], &g_Plr[id].ammo.nMoney, 4);	// Money/score, in all
	msgPA[6] = g_Plr[id].ammo.nHealth;	// Health, percent
	memcpy(&msgPA[7], g_Plr[id].ammo.n, 12);	// Ammunition of this type of weapon in hand or total
	msgPA[19] = (BYTE)g_Plr[id].ammo.n[6];
	msgPA[20] = (BYTE)g_Plr[id].ammo.n[7];
	msgPA[21] = (BYTE)g_Plr[id].ammo.n[8];
	msgPA[22] = (BYTE)g_Plr[id].ammo.n[9];
	msgPA[23] = (BYTE)g_Plr[id].ammo.n[10];
	memcpy(&msgPA[24], g_Plr[id].ammo.i, 6);	// Bullets in the clip for all 6 weapons

	// List of Player public offerings

	// List of foreign public offerings

	g_pNet.msg(g_Plr[id].nid, msgPA, 30);
}

void msgPlayerAmmo(BYTE *msg)	// Enter the Game
{
	memcpy(&g_Ammo.nMoney, &msg[2], 4);	// Money/score, in all
	g_Ammo.nHealth = msg[6];	// Health, percent
	memcpy(g_Ammo.n, &msg[7], 12);	// Ammunition of this type of weapon in hand or total
	g_Ammo.n[6] = msg[19];
	g_Ammo.n[7] = msg[20];
	g_Ammo.n[8] = msg[21];
	g_Ammo.n[9] = msg[22];
	g_Ammo.n[10] = msg[23];
	memcpy(g_Ammo.i, &msg[24], 6);	// Bullets in the clip for all 6 weapons

	// List of Player public offerings

	// List of foreign public offerings

	// Fill lists
	/*g_pUI.deleteAllItems(ID_LIST_WEAPON);
	wchar_t	s[8], sMax[12];
	for (int i = 0; i < 11; i++)
	{
		swprintf_s(s, 8, L"%d", g_Ammo.n[i]);
		swprintf_s(sMax, 12, L"%d /%d ", g_nAmmoMax[i], g_nBulletsInClip[i]);
		g_pUI.addItem3(ID_LIST_WEAPON, g_sAmmoName[i], s, sMax, i);
	}*/
	wchar_t	s[8];
	for (int i = 0; i < 11; i++)
	{
		swprintf_s(s, 8, L"%d", g_Ammo.n[i]);
		g_pUI.setIndexText2(ID_LIST_WEAPON, i, s);
	}
}


//--------------------------------------------------------------------------------------
// ENTER THE GAME
//--------------------------------------------------------------------------------------
void sendToServer_EnterGame()
{
	g_msg[0] = MSG_ENTERGAME;	// g_msg[1] = 0;	// Enter the Game
	g_pNet.msg(g_nidServer, g_msg, 4);
}

void msgEnterGame(BYTE *msg)	// Enter the Game
{
	WORD id;
	memcpy(&id, &msg[2], 2);
	if (id < MAX_PLR)	// Additional verification
	{
		g_Plr[id].bInGame = true;
		g_Plr[id].ammo.nHealth = 100;	// 100% health
		for (int i = 0; i < 6; i++)	// Full clip
		{
			g_Plr[id].ammo.i[i] = g_nBulletsInClip[i] < g_Plr[id].ammo.n[i] ? g_nBulletsInClip[i] : (BYTE)g_Plr[id].ammo.n[i];	// Bullets, in clip
			g_Plr[id].ammo.n[i] -= g_Plr[id].ammo.i[i];	// Bullets, in stock
		}
		activatePersInGame(id, ID_TT);
	}
}









wchar_t	g_sMsgTitle[32];
//wchar_t	g_sMessage[192];


void msgAuthorizeServer(BYTE *msg)	// Authorization of player on the server
{
	WORD id;
	int acc;
	wchar_t	s[32];
	memcpy(&id, msg + 2, 2);	// Player ID
	memcpy(&acc, msg + 4, 4);	// Player Account
	// Server as well known server version 

	if (acc > 0)	// If authorized
	{
		swprintf_s(s, 32, L"%d %s", acc, g_pDB.getName(acc));	// Account and name of authorized player
		g_pUI.insItem(ID_LIST_PLAYERS, id, s, acc);	// Insert item into the players list
		AMMO_DB ammo = { acc };
		g_pDB.getItem(TABLE_AMMO, (BYTE*)&ammo);	// Get from database ammunition just authorized player
		// Insert an ammunition to Player parameters
		g_Plr[id].bActive = true;
		g_Plr[id].ammo.nHealth = 100;	// Health, percent
		g_Plr[id].ammo.level = ammo.level;	// Level of progress
		g_Plr[id].ammo.nMoney = ammo.nMoney;	// Money/score
		g_Plr[id].ammo.n[0] = ammo.nTT;		// Bullets for TT, in stock
		g_Plr[id].ammo.n[1] = ammo.nUzi;	// Bullets for Uzi, in stock
		g_Plr[id].ammo.n[2] = ammo.nAKM;	// Bullets for AKM, in stock
		g_Plr[id].ammo.n[3] = ammo.nM16;	// Bullets for M16, in stock
		g_Plr[id].ammo.n[4] = ammo.nSnip;	// Bullets for SR, in stock
		g_Plr[id].ammo.n[5] = ammo.nNG7;	// Bullets for NG7, total
		g_Plr[id].ammo.n[6] = ammo.nRPG;	// Grenades for RPG (rocket-propelled grenade), total
		g_Plr[id].ammo.n[7] = ammo.nGren;	// Hand-grenades, total
		g_Plr[id].ammo.n[8] = ammo.nMine;	// Land mines (at the time, to contact), total
		g_Plr[id].ammo.n[9] = ammo.nAKMG;	// AKM grenades, total
		g_Plr[id].ammo.n[10] = ammo.nM16G;	// M16 grenades, total

		sendToClient_PlayerAmmo(id);	// Inform the client/player of his ammunition
	}
	else	// If not authorized (guest)
	{
		swprintf_s(s, 32, L"Guests: %d", ++g_nGuests);
		g_pUI.text(ID_STATIC_GUESTS, s);
	}
}


void msgAuthorizeClient(BYTE *msg)	// Report player about authorization
{
	wchar_t	s[32];
	memcpy(&g_id, &msg[2], 2);		// Player ID
	memcpy(&g_acc, &msg[4], 4);		// Player Account
	initPerses();
	g_pPhys.setIDOwnPers(g_id);
	memcpy(&g_msg[2], &g_id, 2);	// Player/Guests ID
	g_msg[1] = 0;	// If only single-byte message identifiers are used
	//memcpy(&g_version, msg + 8, 2);	// Application version
	if (g_acc > 0)	// If authorized
	{
		swprintf_s(g_sMsgTitle, 32, L"To ALL: ");	// To Recipient:
		swprintf_s(s, 32, L"%d %s", g_acc, g_pUI.getText(ID_EDIT_LOGIN));	// Account and name of authorized player
		//g_pUI.addItem(ID_LIST_PLAYERS, s, g_acc);	// Insert item into the players list
		//g_pUI.selItem(ID_LIST_PLAYERS, 0);	// Select own account for chat with all
		g_bClient = true;	// Flag of Client initialization
		ChangeState(TRADE);	// Trading on the client game (contains chat)
		//g_pUI.deleteAllItems(ID_LIST_WEAPON);
		//sendToServer_EnterTrade();
	}
	else
	{
		if (g_sName[0])	// If entered the name
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


void msgRegistrateServer(BYTE *msg)	// Player registration on the server
{
	WORD id;
	DWORD acc;
	memcpy(&id, msg + 2, 2);	// Player ID
	memcpy(&acc, msg + 4, 4);	// Player Account

	AMMO_DB ammo = { (int)acc, 0, 10, 100, 200, 100, 100, 200, 100, 2, 2, 2, 2, 2 };
	g_pDB.add(TABLE_AMMO, (BYTE*)&ammo);	// Add to database ammunition just registered player

	if (id < MAX_PLR)
	{
		wchar_t	s[256];
		ACCOUNT	Acc = { (int)acc };	// Account item
		REGISTR	Reg = { (int)acc };	// Registrations item
		//Acc.acc = Reg.acc = acc;
		g_pDB.getAccount(id, &Acc, &Reg);
		swprintf_s(s, 256, L"%d %s", Acc.acc, Acc.sName);
		g_pUI.insItem(ID_LIST_PLAYERS, id, s, acc);	// Added to players list
		swprintf_s(s, 256, L"%d %s %s", Acc.acc, Acc.sName, Acc.sPass);
		g_pUI.addItem(ID_LIST_ACC, s, acc);	// Added to account list
		swprintf_s(s, 256, L"%d %s %s %s %s %d %s %s", Reg.acc, Reg.sMail, Reg.sSkype, Reg.sCountry, Reg.sCity, Reg.nAgeSex % 128, Reg.nAgeSex > 127 ? L"Male" : L"Female", Reg.sMyself);
		g_pUI.addItem(ID_LIST_REG, s, acc);	// Added to registration list
		swprintf_s(s, 162, L"%d: %d %d %d %d %d %d %d %d %d %d %d %d %d", ammo.acc, ammo.level, ammo.nMoney,
			ammo.nTT, ammo.nUzi, ammo.nAKM, ammo.nM16, ammo.nSnip, ammo.nNG7, ammo.nRPG, ammo.nGren, ammo.nMine, ammo.nAKMG, ammo.nM16G);
		g_pUI.addItem(ID_LIST_AMMO, s, acc);	// Added to registration list
		swprintf_s(s, 16, L"Guests: %d", --g_nGuests);	// Take away one guest
		g_pUI.text(ID_STATIC_GUESTS, s);

		// Insert an ammunition to Player parameters
		g_Plr[id].bActive = true;
		g_Plr[id].ammo.nHealth = 100;	// Health, percent
		g_Plr[id].ammo.level = ammo.level;	// Level of progress
		g_Plr[id].ammo.nMoney = ammo.nMoney;	// Money/score
		g_Plr[id].ammo.n[0] = ammo.nTT;		// Bullets for TT, in stock
		g_Plr[id].ammo.n[1] = ammo.nUzi;	// Bullets for Uzi, in stock
		g_Plr[id].ammo.n[2] = ammo.nAKM;	// Bullets for AKM, in stock
		g_Plr[id].ammo.n[3] = ammo.nM16;	// Bullets for M16, in stock
		g_Plr[id].ammo.n[4] = ammo.nSnip;	// Bullets for SR, in stock
		g_Plr[id].ammo.n[5] = ammo.nNG7;	// Bullets for NG7, total
		g_Plr[id].ammo.n[6] = ammo.nRPG;	// Grenades for RPG (rocket-propelled grenade), total
		g_Plr[id].ammo.n[7] = ammo.nGren;	// Hand-grenades, total
		g_Plr[id].ammo.n[8] = ammo.nMine;	// Land mines (at the time, to contact), total
		g_Plr[id].ammo.n[9] = ammo.nAKMG;	// AKM grenades, total
		g_Plr[id].ammo.n[10] = ammo.nM16G;	// M16 grenades, total

		sendToClient_PlayerAmmo(id);	// Inform the client/player of his ammunition
	}
}


void msgRegistrateClient(BYTE *msg)	// Report player about registration
{
	memcpy(&g_id, msg + 2, 2);		// Player ID
	memcpy(&g_acc, msg + 4, 4);		// Player Account
	initPerses();
	g_pPhys.setIDOwnPers(g_id);
	memcpy(&g_msg[2], &g_id, 2);	// Player/Guests ID
	g_msg[1] = 0;	// If only single-byte message identifiers are used
	//memcpy(&g_version, msg + 8, 2);	// Application version
	if (g_id < MAX_PLR)	// If authorized
	{
		swprintf_s(g_sMsgTitle, 32, L"To ALL: ");	// To Recipient:
		swprintf_s(g_sName, 16, g_pUI.getText(ID_EDIT_NAME));
		wchar_t	s[32];
		swprintf_s(s, 32, L"%d %s", g_acc, g_pUI.getText(ID_EDIT_NAME));	// Account and name of authorized player
		g_pUI.addItem(ID_LIST_PLAYERS, s, g_acc);	// Insert item into the players list
		g_pUI.selItem(ID_LIST_PLAYERS, 0);	// Select own account for chat with all
		g_bClient = true;	// Flag of Client initialization
		ChangeState(TRADE);	// Trading on the client game (contains chat)
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


void msgInfoPlayer(BYTE *msg)	// Other player params (only on the client)
{
	DWORD acc;
	wchar_t	sName[16], s[32];
	memcpy(&acc, msg + 2, 4);	// Player Account
	memcpy(&sName, msg + 6, 32);	// Player Name
	swprintf_s(s, 32, L"%d %s", acc, sName);
	g_pUI.addItem(ID_LIST_PLAYERS, s, acc);	// Added to players list
}


void msgInfoPlayers(BYTE *msg)	// Other players params (only on the client)
{
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
}


void msgChatBlock(BYTE *msg)	// Chat block
{
	BYTE iChat = msg[2];	// Number of chat messages
	if (iChat)	// If received chat messages
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
			memcpy(&t, &msg[iData], 4);	iData += 4;	// Time of receiving messages on the server
			_localtime32_s(&ts, &t);	// 
			iType = msg[iData++];	// Message type
			if (iType != 2)	// If don't send all
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
}



//--------------------------------------------------------------------------------------
// SEND EVERY FRAME MESSAGE FROM SERVER TO CLIENTS
//--------------------------------------------------------------------------------------
void sendToClients_EveryFrame()
{
	BYTE msgEF[1000];
	msgEF[0] = MSG_EVERYFRAME;	// Every frame message
	msgEF[1] = 0;

	WORD	num = 0;
	for (WORD id = 0; id < MAX_TEST; id++)
		if (g_Plr[id].bInGame)
		{
			num++;
		}
	memcpy(&msgEF[2], &num, 2);

	DWORD n = 4;
	for (WORD id = 0; id < MAX_TEST; id++)
		if (g_pPhys.getPersActive(id)) //g_Plr[id].bInGame)
		{
			memcpy(&msgEF[n], &id, 2);	n += 2;
			float x = g_pPhys.getPersX(id);		memcpy(&msgEF[n], &x, 4);	n += 4;
			float y = g_pPhys.getPersY(id);		memcpy(&msgEF[n], &y, 4);	n += 4;
			float z = g_pPhys.getPersZ(id);		memcpy(&msgEF[n], &z, 4);	n += 4;
			float na = g_pPhys.getPersNA(id);	memcpy(&msgEF[n], &na, 4);	n += 4;
			float sa = g_pPhys.getPersSA(id);	memcpy(&msgEF[n], &sa, 4);	n += 4;
			msgEF[n++] = g_pPhys.getPersSpeedType(id);	// g_Plr[id].weapon;
			msgEF[n++] = g_Plr[id].bFire;
		}

	g_pNet.msgToAll(msgEF, n);
}

void msgEveryFrame(BYTE *msg)	// Every Frame message
{
	WORD	num;
	memcpy(&num, &msg[2], 2);	// Number of personage in the Game

	WORD id = 0;
	for (; id < MAX_TEST; id++)
		g_Plr[id].bInGame = false;

	DWORD n = 4;
	for (WORD i = 0; i < num; i++)
	{
		memcpy(&id, &msg[n], 2);	n += 2;
		float x;	memcpy(&x, &msg[n], 4);	n += 4;
		float y;	memcpy(&y, &msg[n], 4);	n += 4;
		float z;	memcpy(&z, &msg[n], 4);	n += 4;
		float na;	memcpy(&na, &msg[n], 4);	n += 4;
		float sa;	memcpy(&sa, &msg[n], 4);	n += 4;
		BYTE iSpeed = msg[n++];
		g_Plr[id].bFire = msg[n++] ? true : false;
		// Add the vector direction of fire

		if (id != g_id)	// Obtained a correction movement is not own person
		{
			g_Plr[id].bInGame = true;
			float speed = 1.0f;
			switch (iSpeed) {
			case 0:	speed = 0.0f;	break;	// Stands on the site
			case 1:	speed = 3.6f;	break;	// Walk Speed
			case 2:	speed = 7.0f;	break;	// Run Speed
			}
			g_pPhys.setPersActive(id, true);
			g_pPhys.setPersPosFromServer(id, x, y, z, na);
			g_pPhys.setPersSpeedType(id, iSpeed);	// Set the type of speed
		}
	}
}


//--------------------------------------------------------------------------------------
// EVERY FRAME ACTION
//--------------------------------------------------------------------------------------
void sendToServer_Action(float fTime)	// Action
{
	g_msg[0] = MSG_ACTION;	// g_msg[1] = 0;	// Client player action

	//BYTE	speed = 1;	// BYTE: 0 - stands, 1 - goes, 2 - run, 3 - squats, 4 - goes squatting, 5 - lying motionless, 6 - crawling
	float	a = g_pPhys.getPersNA(g_id);
	memcpy(&g_msg[4], &a, 4);
	a = g_pPhys.getPersSA(g_id);
	memcpy(&g_msg[8], &a, 4);
	memcpy(&g_msg[12], &fTime, 4);	// Time of the last frame
	g_msg[16] = g_pPhys.getPersSpeedType(g_id);	// BYTE: 0 - stands, 1 - goes, 2 - run, 3 - squats, 4 - goes squatting, 5 - lying motionless, 6 - crawling
	g_msg[17] = g_plr1[g_id].bFire;
	if (g_msg[17])
	{
		memcpy(&g_msg[18], &g_nxFire, 4);
		memcpy(&g_msg[22], &g_nyFire, 4);
		memcpy(&g_msg[26], &g_nzFire, 4);
	}
		// Add the vector direction of fire

	g_pNet.msg(g_nidServer, g_msg, g_msg[17] ? 30 : 18);
}

void msgAction(BYTE *msg)	// Getting the messages on the server with player actions
{
	WORD id;
	memcpy(&id, &msg[2], 2);

	float	fTime;
	float	na, sa;
	memcpy(&na, &msg[4], 4);
	memcpy(&sa, &msg[8], 4);
	memcpy(&fTime, &msg[12], 4);
	BYTE iSpeed = msg[16];	// BYTE: 0 - stands, 1 - goes, 2 - run, 3 - squats, 4 - goes squatting, 5 - lying motionless, 6 - crawling
	g_Plr[id].bFire = msg[17] ? true : false;
	// Add the vector direction of fire
	if (msg[17])
	{
		//g_Plr[id].weapon = msg[17];
		if (id)	// If not Admin then add bullet to physics subsystem on the server
			g_pPhys.addBullet(g_Plr[id].x + g_nxFire, g_Plr[id].y + g_nyFire, g_Plr[id].z + g_nzFire, g_nxFire, g_nyFire, g_nzFire, g_damage[g_Plr[id].weapon]);

		if (g_Plr[id].weapon > 5)	// If heavy weapons
			g_Plr[id].ammo.n[g_Plr[id].weapon]--;
		else	// If firearms
		{
			if (!g_Plr[id].ammo.i[g_Plr[id].weapon])	// If finished bullets in the clip, then reload it
			{
				if (g_Plr[id].ammo.n[g_Plr[id].weapon] < g_nBulletsInClip[g_weapon])	// If bullets remains less than the capacity of the clip
				{
					g_Plr[id].ammo.i[g_Plr[id].weapon] = (BYTE)g_Plr[id].ammo.n[g_Plr[id].weapon];
					g_Plr[id].ammo.n[g_Plr[id].weapon] = 0;
				}
				else	// If there is enough bullets to completely fill the clip
				{
					g_Plr[id].ammo.i[g_Plr[id].weapon] = g_nBulletsInClip[g_weapon];
					g_Plr[id].ammo.n[g_Plr[id].weapon] -= g_nBulletsInClip[g_weapon];
				}
			}

			if (g_Plr[id].ammo.i[g_Plr[id].weapon])		// If there were bullets in the clip,
				g_Plr[id].ammo.i[g_Plr[id].weapon]--;	// Then shoot out of the clip
		}
	}

	float speed;
	switch (iSpeed)	{
	case 0:	speed = 0.0f;	break;	// 0 Stands on the site
	case 1:	speed = 3.6f;	break;	// 1 Walk Speed
	default:speed = 7.0f;	break;	// 2 Run Speed
	}
	float MB = 0.0f;
	g_pPhys.setPersFrameMove(id, sinf(na)*cosf(MB), sinf(MB), cosf(na)*cosf(MB), na, sinf(sa), 0.0f, cosf(sa), sa, speed, 0.0f, fTime);
	g_pPhys.setPersSpeedType(id, iSpeed);	// Set the type of speed

	////// PROCESS CREATION OF BULLETS

	////// DO LIST IN respective arrays
}


//--------------------------------------------------------------------------------------
// NETWORK MESSAGE EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onNetMsg(BYTE *msg)
{
	WORD iMsg;
	memcpy(&iMsg, msg, 2);	// ID message

	if (g_bServer)
	{

	}

	if (g_bClient)
	{

	}

	switch (iMsg) {
	case MSG_AUTHORIZE:	// Authorize (valid)
		if (g_eState == SERVER)	// Player logged on to the server
			msgAuthorizeServer(msg);
		else	// Validation
			msgAuthorizeClient(msg);
		break;

	case MSG_REGISTRATE:	// Registrate (valid)
		if (g_eState == SERVER)	// Player logged on to the server
			msgRegistrateServer(msg);
		else	// Validation
			msgRegistrateClient(msg);
		break;

	case MSG_INFOPLAYER: {	// Other player params (only on the client)
		DWORD acc;
		wchar_t	sName[16], s[32];
		memcpy(&acc, msg + 2, 4);	// Player Account
		memcpy(&sName, msg + 6, 32);	// Player Name
		swprintf_s(s, 32, L"%d %s", acc, sName);
		g_pUI.addItem(ID_LIST_PLAYERS, s, acc);	// Added to players list
	}	break;

	case MSG_INFOPLAYERS:
		msgInfoPlayers(msg);	// Other players params (only on the client)
		break;

	case MSG_CHATSER: {	// Chat message from the client to the server
		WORD idSender;
		DWORD accRecipient;
		wchar_t	sMsg[128], sMsg2[162];
		memcpy(&idSender, &msg[2], 2);	// Sender ID
		memcpy(&accRecipient, &msg[4], 4);	// Recipient ACC
		BYTE len = msg[8];
		memcpy(sMsg, &msg[9], len);	// Message string
		sMsg[len >> 1] = L'\0';
		swprintf_s(sMsg2, 162, L"From %s to %s: %s", g_pDB.getNameID(idSender), accRecipient == -1 ? L"ALL" : g_pDB.getName(accRecipient), sMsg);
		g_pUI.addItem(ID_LIST_CHAT, sMsg2, 0);	// 
	}	break;

	case MSG_CHAT: {	// Chat message from/through the server to the client
		DWORD accSender;
		wchar_t	sMsg[128], sMsg2[162];
		__time32_t	t;
		struct	tm	ts;
		memcpy(&accSender, &msg[2], 4);	// Sender ACC
		memcpy(&t, &msg[6], 4);	_localtime32_s(&ts, &t);	// 	// Server time
		bool bPersonal = msg[10] % 2 ? true : false;
		BYTE len = bPersonal ? --msg[10] : msg[10];
		memcpy(sMsg, &msg[11], len);	// Message string
		sMsg[len >> 1] = L'\0';

		////// Find Sender
		swprintf_s(sMsg2, 162, L"%02i:%02i:%02i  from %i to %s: %s", ts.tm_hour, ts.tm_min, ts.tm_sec, accSender, bPersonal ? L"ME" : L"ALL", sMsg);
		g_pUI.addItem(ID_LIST_CHAT, sMsg2, 0);	// 
	}	break;

	case MSG_CHAT_VALID: {	// Chat message valid
		__time32_t	t;
		struct	tm	ts;
		//wchar_t	s[192];
		memcpy(&t, &msg[2], 4);	_localtime32_s(&ts, &t);	// 	// Server time
		//swprintf_s(s, 192, L"%02i:%02i:%02i  %s", ts.tm_hour, ts.tm_min, ts.tm_sec, g_sMessage);
		//g_pUI.addItem(ID_LIST_CHAT, s, 0);	// 
	}	break;

	case MSG_CHATBLOCK:
		msgChatBlock(msg);	// Chat block
		break;

	case MSG_ENTERTRADE:	// Enter the Trade
		msgEnterTrade(msg);	// Enter the Trade
		break;

	case MSG_PLAYERAMMO:	// Player's Ammo
		msgPlayerAmmo(msg);	// Player's Ammo
		break;

	case MSG_ENTERGAME:		// Enter the Game
		msgEnterGame(msg);	// Enter the Game
		break;

	case MSG_EVERYFRAME:	// Every Frame message
		msgEveryFrame(msg);	// Every Frame message
		break;

	case MSG_ACTION:	// Every frame action on the client
		msgAction(msg);	// Action
		break;


	case MSG_SELLAMMO:	// Request to the server for sale of ammunition
		msgSellAmmo(msg);	// Request to the server for sale of ammunition
		break;

	case MSG_BUYAMMO:	// Request to the server for buy of ammunition
		msgBuyAmmo(msg);	// Request to the server for buy of ammunition
		break;

	case MSG_AMMOVALID:	// Response to the request for the sale/buy of ammunition (0 - error, 1 - confirmation of the creation, 2 - the conclusion of the transaction, 3 - the abolition of the transaction, 4 - confirmation of acceptance, 5 - end of time)
		msgAmmoValid(msg);	// Response to the request for the sale/buy of ammunition (0 - error, 1 - confirmation of the creation, 2 - the conclusion of the transaction, 3 - the abolition of the transaction, 4 - confirmation of acceptance, 5 - end of time)
		break;

	case MSG_BIDCANCEL:	// Cancel the transaction, bail to return
		//msgBidCancel(msg);	// Cancel the transaction, bail to return
		break;

	case MSG_MAKEDEAL:	// Accept offer
		//msgMakeDeal(msg);	// Accept offer
		break;
	}
}