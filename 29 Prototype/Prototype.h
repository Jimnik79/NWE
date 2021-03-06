//--------------------------------------------------------------------------------------
// File: 20 Prototype.cpp
// Description: Sample prototype of 3D MMOG.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


#include "..\Engine\NetWars.h"	// Engine

#include <stdio.h>	// swprintf_s
#include <time.h>	// struct tm, _time32, _localtime32_s
//#include <math.h>	// sin, cos


CEngine	g_pGame;	// Base Engine class
C2D		g_p2D;		// 2D class
CTex	g_pTex;		// Texture class
CText	g_pText;	// Text class
CSound	g_pSound;	// Sound class
CMedia	g_pMedia;	// Media class
C3D		g_p3D;		// 3D class
CPhys	g_pPhys;	// Physics class
CCam	g_pCam;		// Camera class
CUI		g_pUI;		// UI class
CDB		g_pDB;		// Database class
CNet	g_pNet;		// Network class


#define	ID_TT			0	// TT
#define	ID_UZI			1	// Uzi
#define	ID_AKM			2	// AKM
#define	ID_M16			3	// M16
#define	ID_SNIP			4	// Sniper
#define	ID_NG7			5	// Negev NG7
#define	ID_RPG			6	// RPG
#define	ID_GREN			7	// Grenade
#define	ID_MINE			8	// Mine
#define	ID_MESH_SOLDIER	9	// Animated mesh of Soldier

#define	SOUND_SHOT		0	// Shot
#define	SOUND_RELOAD	1	// Reload weapon

#define	TABLE_AMMO		3	// Ammo, extended player parameters
#define	TABLE_TRADE		4	// Trade

#define	NO_ACCOUNT		-1	// No account
#define	AMMO_SELL		1	// Sell Ammo state
#define	AMMO_BUY		1	// Buy Ammo state



BYTE	g_nBulletsInClip[11] = { 15, 50, 36, 30, 10, 200, 1, 20, 5, 1, 1 };	// Bullets in clip: TT, Uzi, AKM, M16, Sniper, NG7, RPG, Grenades, Mines, AKM Grenades, M16 Grenades
WORD	g_nAmmoMax[11] = { 999, 999, 500, 500, 300, 400, 10, 20, 5, 20, 20 };	// Ammo, max.: TT, Uzi, AKM, M16, Sniper, NG7, RPG, Grenades, Mines, AKM Grenades, M16 Grenades
wchar_t	g_sAmmoName[11][16] = { L"TT", L"Uzi", L"AKM", L"M16", L"Sniper", L"Negev NG7", L"RPG", L"Grenades", L"Mine", L"AKM Grenades", L"M16 Grenades" };	// Ammo names

#pragma pack(push, 1)	// For correct read/write in the database
struct AMMO_DB	// AMMUNITION IN THE DATABASE TABLE
{
	int		acc;	// Account ID
	BYTE	level;	// Level of progress (0-10), the transition to the next level is carried out with a combination of money to: (1 - $100, 2 - $200, 3 - $500, 4 - $1k, 5 - $2k, 6 - $5k, 7 - $10k, 8 - $20k, 9 - $50k, 10 - $100k, WIN - $1M)
	int		nMoney;	// Money/score
	WORD	nTT;	// Bullets for TT, total
	WORD	nUzi;	// Bullets for Uzi, total
	WORD	nAKM;	// Bullets for AKM, total
	WORD	nM16;	// Bullets for M16, total
	WORD	nSnip;	// Bullets for sniper rifle (SR), total
	WORD	nNG7;	// Bullets for machine gun Negev NG7, total
	BYTE	nRPG;	// Grenades for RPG (rocket-propelled grenade), total
	BYTE	nGren;	// Hand-grenades, total
	BYTE	nMine;	// Land mines (at the time, to contact), total
	BYTE	nAKMG;	// AKM grenades, total
	BYTE	nM16G;	// M16 grenades, total
	//WORD	n[6];	// Ammunition, in stock
	//BYTE	i[5];	// Ammunition, total
};

struct TRADE_DB	// TRADE IN THE DATABASE TABLE
{
	int			id;			// ID of bargain/transaction
	UINT		accOpen;	// Account opened the deal/transaction
	__time32_t	tOpen;		// Opening time
	UINT		accClose;	// Account closed the deal/transaction (-1 - the transaction is open, canceled or not took place in the allotted time)
	__time32_t	tClose;		// Closing time (0 - still open)
	BYTE		iState;		// Status of bargain/transaction (1 - open the deal/transaction to sell weapons, 2 - open the deal/transaction to buy weapons, 3.4 - not held for sale/buy 5.6 - held for sale/buy)
	BYTE		iType;		// Type of ammunition (0 - TT, 1 - Uzi, 2 - AKM, 3 - M16, 4 - Sniper, 5 - NG7, 6 - RPG, 7 - Grenades, 8 - Mines, 9 - AKMG, 10 - M16G)
	WORD		num;		// The amount of ammunition
	UINT		nMoney;		// Money
};
#pragma pack(pop)	// For correct read/write in the database

int	g_idTrade = 0;	// ID of bargain/transaction

struct AMMO	// AMMUNITION
{
	BYTE	level;	// Level of progress (0-10), the transition to the next level is carried out with a combination of money to: (1 - $100, 2 - $200, 3 - $500, 4 - $1k, 5 - $2k, 6 - $5k, 7 - $10k, 8 - $20k, 9 - $50k, 10 - $100k, WIN - $1M)
	UINT	nMoney;	// Money/score
	BYTE	nHealth;// Health, percent
	WORD	n[11];	// Ammunition, in stock or total
	BYTE	i[6];	// Ammunition, in clip
};
AMMO	g_Ammo = { 0, 10, 100, 100, 200, 100, 100, 200, 100, 2, 2, 2, 2, 2, 15, 50, 36, 30, 50, 10 };	// Player Ammunition on the client
float	g_tShot = 0.0f;	// Time of permission for next shot
float	g_time = 0.0f;	// Current time
float	g_tNewShot[7] = { 0.3f, 0.1f, 0.1f, 0.07f, 0.4f, 0.06f };	// Time for new shot: TT, Uzi, AKM, M16, Sniper, NG7, RPG
float	g_tDelta = 0.01f;
float	g_tReload = 3.0f;	// While reloading weapons
float	g_tReloadEnd = 0.0f;	// End time for reloading the clip (established after the beginning of reloading the clip)

#define	MAX_PLR		5000	// The maximum number of players in the network, if it's more, then must be sent a message that the server is overloaded
#define	MAX_TEST	10		// The maximum number of players in the network FOR TESTS, if it's more, then must be sent a message that the server is overloaded

struct TRADING	// TRADE IN THE MEMORY ON THE SERVER
{
	int			id;			// ID of bargain/transaction
	UINT		accOpen;	// Account opened the deal/transaction
	__time32_t	tOpen;		// Opening time
	//BYTE		iState;		// Status of bargain/transaction (1 - open the deal/transaction to sell weapons, 2 - open the deal/transaction to buy weapons, 3.4 - not held for sale/buy 5.6 - held for sale/buy)
	BYTE		iType;		// Type of ammunition (0 - TT, 1 - Uzi, 2 - AKM, 3 - M16, 4 - Sniper, 5 - NG7, 6 - RPG, 7 - Grenades, 8 - Mines, 9 - AKMG, 10 - M16G)
	WORD		num;		// The amount of ammunition
	UINT		nMoney;		// Money
};
TRADING	g_tradeSell[5000];
TRADING	g_tradeBuy[5000];
UINT	g_iSell = 0;
UINT	g_nSell = 0;
UINT	g_iBuy= 0;
UINT	g_nBuy = 0;

CRITICAL_SECTION m_csTrade;	// Critical section for trading data
#define	TRADE_LOCK()	EnterCriticalSection(&m_csTrade);
#define	TRADE_UNLOCK()	LeaveCriticalSection(&m_csTrade);
//InitializeCriticalSection(&m_csTrade);	// Initialize critical section for trading data
//DeleteCriticalSection(&m_csTrade);	// Delete critical section for trading data


struct PLAYER_SERVER	// FULL PLAYER PARAMETERS ON THE SERVER
{
	//WORD	id;			// Player's ID
	UINT	acc;		// Player's ACCOUNT
	bool	bActive;	// Flag of active player (the player on the network: in the trade or in the game)
	UINT	nid;		// Network ID
	bool	bInGame;	// true - run the world, false - trade panel or even offline
	bool	bFire;		// Flag of shooting at a given time
	BYTE	weapon;		// Weapon type
	float	x, y, z, a;	// Player Position in the Game World
	float	fTime;		// The time of last entering the game

	AMMO	ammo;		// Player Ammunition
};
PLAYER_SERVER	g_Plr[MAX_PLR];

float g_nxFire, g_nyFire, g_nzFire;	// The normal direction of the shot

struct PLAYER_CLIENT	// FULL PLAYER PARAMETERS ON THE SERVER
{
	wchar_t	sName[16];	// Player Name
	bool	bActive;	// Flag of active player (the player on the network: in the trade or in the game)
	bool	bInGame;	// true - run the world, false - trade panel or even offline
	bool	bFire;		// Flag of shooting at a given time
	BYTE	weapon;		// Weapon type
	//float	x, y, z, a;	// Player Position in the Game World (in physics subsystem, does not need to duplicate)
};
PLAYER_CLIENT	g_plr1[MAX_PLR];

BYTE	g_weapon = ID_TT;	// Weapons of players
BYTE	g_damage[6] = { 15, 10, 15, 20, 40, 20 };	// Damage from 1 bullet: TT, Uzi, AKM, M16, Sniper rifle, Negev NG7

bool	g_bServer = false;	// Flag of Server initialization
bool	g_bClient = false;	// Flag of Client initialization

bool	g_bInitPerses = false;


#include "Game.h"	// Class of Map
#include "..\Shared\Map.h"	// Class of Map
//#include "Mesh.h"	// Class of Mesh

CMap	g_pMap;		// Map class


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
#define	ID_WINDOW_ADMIN		40	// ADMIN PANEL
#define	ID_STATIC_PLAYERS	41	// List of players
#define	ID_LIST_PLAYERS		42	// List of players
#define	ID_STATIC_CHAT		43	// Chat with ...
#define	ID_LIST_CHAT		44	// List of chat
#define	ID_STATIC_MESSAGE	45	// Message:
#define	ID_EDIT_MESSAGE		46	// Edit message
#define	ID_STATIC_GUESTS	47	// Number of guests (not registered clients)
#define	ID_BUTTON_TRADE		48	// Trade on the Server
#define	ID_LIST_IP			49	// List of IP-addresses
#define	ID_BUTTON_EXIT		50	// Exit
#define	ID_MESSAGE_WAIT		51	// Wait
#define	ID_MESSAGE_ERROR	52	// Message with text of error
#define	ID_LIST_ACC			53	// List of accounts
#define	ID_LIST_REG			54	// List of registrations
#define	ID_LIST_AMMO		55	// List of ammo
#define	ID_LIST_TRADE		56	// List of trade

#define	ID_WINDOW_TRADE		57	// TRADE PANEL (trade ammunition)
#define	ID_LIST_MYTRANSACT	58	// List of my open transactions (max 5 buy + 5 sales)
#define	ID_LIST_WEAPON		59	// List of weapons (type, number/amount)
#define	ID_EDIT_NUMBER		60	// Number of ammunitions (ammo)
#define	ID_EDIT_MONEY		61	// Money for ammo
#define	ID_BUTTON_SELL		62	// Create transaction - Sell ammo
#define	ID_BUTTON_BUY		63	// Create transaction - Buy ammo
#define	ID_BUTTON_CANCELT	64	// Cancel transaction
#define	ID_CHECKBOX_BUY		65	// Buy ammo
#define	ID_LIST_TRANSACT	66	// List of open transactions 
#define	ID_BUTTON_MAKEDEAL	67	// Make a deal
#define	ID_BUTTON_PLAY		68	// Continue to play

#define	ID_STATIC_MYTRANS	69	// List of my open transactions
#define	ID_STATIC_WEAPON	70	// List of weapons
#define	ID_STATIC_NUMBER	71	// Number of ammo
#define	ID_STATIC_MONEY		72	// Money for ammo
#define	ID_STATIC_TRANS		73	// List of open transactions


// After the formation of a bid/transaction for buy or sale of ammunition, given the number of ammunitions/money is blocked until the transaction is closed (completed or canceled)
// Create a bid/transaction to sale ammunition (which, number, total cost).
// Create a bid/transaction to buy ammunition (which, number, total cost).

#define	ID_FONT_15	0
#define	ID_FONT_30	1

#define	GROUP_GENDER	0

#define	ID_TEXTURE_GROUND	0	// Grass texture
#define	ID_TEXTURE_WALL		1	// Wall texture
#define	ID_TEXTURE_PERS		2	// Dynamic texture of Perses on the Map
#define	ID_TEXTURE_SIGHT	3	// Sight texture
#define	ID_TEXTURE_MAP		4	// Dynamic texture of Map

#define	ID_VERTEXBUFFER_GROUND	0	// Ground vertex buffer
#define	ID_INDEXBUFFER_GROUND	1	// Ground index buffer
#define	ID_VERTEXBUFFER_WALLS	2	// Walls vertex buffer
#define	ID_INDEXBUFFER_WALLS	3	// Walls index buffer




//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background

UINT	g_nidServer = 0;// Network Server ID
WORD	g_id = 0;		// Player ID
int		g_acc = 0;		// Player Account: -2 - an incorrect login, -3 - an incorrect password, -4 - the player has already been activated, -5 - ended billable time

BYTE g_msg[65536];
//BYTE	g_data[1000];	// Message type (2 bytes); Id of player (0 - server message)(2 bytes) 
bool	g_bMsgError = false;	// Flag of message with text of error
UINT	g_nGuests = 0;	// Number of guests (not registered clients)
wchar_t	g_sName[16];	// The player name

float	g_x = 500.0f;
float	g_y = 0.0f;
float	g_z = 500.0f;
float	g_a = 0.0f;


enum state	// State machine
{
	MENU,	// Menu
	REG,	// Registration
	WAIT,	// Menu
	SERVER,	// Server Menu (contains chat)
	TRADE,	// Trading on the client game (contains chat)
	TRADES,	// Trading on the server game (contains chat)
	GAME,	// Online client game
	GAMES,	// Online server game
};
enum state g_eState = MENU;	// State machine

//enum weapon	// Choose weapon
//{
//	TT,		// TT pistol
//	AKM,	// Kalashnikov rifle
//};
//enum weapon g_eWeapon = TT;	// default weapon = TT pistol

float g_scale = 2.0f;	// Model scale (person height = 2.0f + 0.1f * level)

bool g_bLMouse0 = false;	// Was left-click mouse on the last frame

int	g_width = 1280;	// The width of the window area
int	g_height = 720;	// The height of the window area

wchar_t	g_sPosition[96];
wchar_t	g_sBulletBeg[96];
wchar_t	g_sBulletEnd[96];
wchar_t	g_sPers[5][96];
wchar_t	g_sMoney[12];		// Money/score, total
wchar_t	g_sHealth[8];		// Health, percent
wchar_t	g_sNGrenades[4];	// Grenades, total
wchar_t	g_sNMines[4];		// Mines, total
wchar_t	g_sNBulletsAll[8];	// Ammunition, total
wchar_t	g_sNBulletsClip[4];	// Bullets in the clip

wchar_t	g_sAmmo[7][8];	// 
//wchar_t	g_sNRifleGrenades[4];	// Rifle grenades, total


int	g_iMessage = 0;
wchar_t	g_sMessages[10][96] = { L"" };	// Current messages

void addMessage(LPCWSTR sMsg)
//void addMessage(const char *format[, argument] ...)
{
	memcpy(g_sMessages[g_iMessage++], sMsg, (wcslen(sMsg) + 1) << 1);
	if (g_iMessage > 9)
		g_iMessage = 0;
}

wchar_t	g_sPlayers[10][256] = { L"" };	// Players parameters




//--------------------------------------------------------------------------------------
// HELPS FUNCTION
//--------------------------------------------------------------------------------------

void ChangeState(state eState)
{
	g_eState = eState;	// New state

	g_pUI.visible(ID_WINDOW_MENU, eState == MENU);		// START CHAT
	g_pUI.visible(ID_WINDOW_REG, eState == REG);		// REGISTRATION
	g_pUI.visible(ID_MESSAGE_WAIT, eState == WAIT);		// WAIT
	g_pUI.visible(ID_WINDOW_ADMIN, eState == SERVER);	// ADMIN PANEL
	g_pUI.visible(ID_MESSAGE_ERROR, g_bMsgError);		// ERROR
	g_pUI.visible(ID_LIST_IP, eState == SERVER);		// List of IP-addresses
	g_pUI.visible(ID_LIST_ACC, eState == SERVER);		// List of accounts
	g_pUI.visible(ID_LIST_REG, eState == SERVER);		// List of registrations
	g_pUI.visible(ID_LIST_AMMO, eState == SERVER);		// List of ammo
	g_pUI.visible(ID_LIST_TRADE, eState == SERVER);		// List of trade
	g_pUI.visible(ID_BUTTON_EXIT, eState < GAME);		// Exit
	g_pUI.visible(ID_WINDOW_TRADE, eState == TRADE || eState == TRADES); 	// TRADE PANEL
}


void EnterTheGame()	// Prepare to enter the game
{
	//g_Ammo.nTT = 100;	// Bullets for TT, total
	//g_Ammo.iTT = 15;	// Bullets for TT, in the clip
}


