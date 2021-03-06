//--------------------------------------------------------------------------------------
// File: BJ.h
// Description: Class of BlackJack.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


#include <windows.h>


#define	MAX_PLAYERS	5				// Number of players
#define	ALL			MAX_PLAYERS + 1	// Number of players + Dealer
#define	MAX_CARDS	5				// Maximum cards in the hand (20, if there is no rule 5 cards)
#define	LAST_CARD	MAX_CARDS - 1	// Last card in the hand
#define	MIN_MONEY	45				// Minimum of money (split + 2 double + insurance)


class CBlackJack
{
protected:
	byte	m_nDecks;		// Number of decks, max. 8
	byte	m_iDeck[416];	// 8 decks * 52 cards
	WORD	m_idCard;		// ID of current card
	WORD	m_minCards;		// Minimum cards in the deck before generate new deal

	bool	m_bActive[ALL];				// Flag of activity
	bool	m_bNewDeal[ALL];			// Flag of ready to new deal
	UINT	m_nid[ALL];					// Network ID
	byte	m_iCard[ALL][MAX_CARDS];	// Cards, first hand
	byte	m_iCard2[ALL][MAX_CARDS];	// Cards, second hand
	byte	m_nCard[ALL];				// Number of cards, first hand
	byte	m_nCard2[ALL];				// Number of cards, second hand
	byte	m_iSum[ALL];				// Sum, first hand
	byte	m_iSum2[ALL];				// Sum, second hand
	int		m_iMoney[ALL];				// Money
	byte	m_iBid[ALL];				// Bid in current game, first hand
	byte	m_iBid2[ALL];				// Bid in current game, second hand
	byte	m_iHand[ALL];				// Hand, which plays, 0 - play dealer
	byte	m_iInsurance[ALL];			// Insurance

	float	m_fX[ALL][MAX_CARDS];
	float	m_fY[ALL][MAX_CARDS];
	float	m_fZ[ALL][MAX_CARDS];
	float	m_fX2[ALL][MAX_CARDS];
	float	m_fY2[ALL][MAX_CARDS];
	float	m_fZ2[ALL][MAX_CARDS];

	CRITICAL_SECTION m_cs;	// Critical section
#define	LOCK()		EnterCriticalSection(&m_cs);
#define	UNLOCK()	LeaveCriticalSection(&m_cs);

public:
	CBlackJack();	// Constructor
	~CBlackJack();	// Destructor

	void Start(byte nDeck);	//
	void GenerateNewDeal();	//
	
	void Sum(int idPlayer);	// Calc sum
	void DeleteCards();		// Delete cards from hands
	void DealerCalc();		// Dealer Calc
	void Dealer();			// Dealer Play
	void GetDealerCards(BYTE* msg);	// Get dealer cards

	byte CardPoint(byte iCard)	{ return iCard % 13 < 9 ? 2 + (iCard % 13) : (iCard % 13 < 12 ? 10 : 1); }

	void AddCard(int idPlayer, BYTE* msg);	// Add card to current player
	BYTE Stop(int idPlayer);		// Stop
	void SetCard(BYTE idPlayer, BYTE iCard, BYTE iCard2);	// Set card to current player
	
	void SetHit(BYTE idPlayer, BYTE iCard, BYTE iCard2);	// Set hit to current player
	void SetStand(BYTE idPlayer, BYTE iCard2);				// Set stand to current player
	void SetDouble(BYTE idPlayer, BYTE iCard, BYTE iCard2);	// Set double to current player
	void SetSplit(BYTE idPlayer, BYTE iCard, BYTE iCard2);	// Set split to current player
	void SetInsurance(BYTE idPlayer);						// Set insurance to current player
	void SetSurrender(BYTE idPlayer);						// Set surrender to current player
	void SetAddMoney(BYTE idPlayer)	{ m_iMoney[idPlayer] += 100; }	// Set money +100 to current player
	void SetDealerCards(BYTE* msg);	// Set dealer cards

	void Hit(int idPlayer, BYTE* msg);	// Hit
	BYTE Stand(int idPlayer);		// Stand
	void Double(int idPlayer, BYTE* msg);	// Double bid
	void Split(int idPlayer, BYTE* msg);	// Split
	void Insurance(int idPlayer);	// Insurance
	void Surrender(int idPlayer);	// Surrender
	void NewDeal(int idPlayer);		// New deal
	void AddMoney(int idPlayer);	// Money +100

	int ConnectPlayer(UINT nid);	// Connect player
	int DisconnectPlayer(UINT nid);	// Disconnect player

	bool GetHit(int id)			{ return m_iHand[id] ? true : false; }	// Return the flag of hit	// && g_Plr[g_iPlayer].iSum < 21 && g_Plr[g_iPlayer].iCard[4] == 255
	bool GetStand(int id)		{ return m_iHand[id] ? true : false; }	// Return the flag of stand	// && g_Plr[g_iPlayer].iSum < 21 && g_Plr[g_iPlayer].iCard[4] == 255
	bool GetDouble(int id)		{ return (m_iHand[id] == 1 && m_iCard[id][2] == 255) || (m_iHand[id] == 2 && m_iCard2[id][2] == 255); }	// Return the flag of double
	bool GetSplit(int id)		{ return m_iHand[id] && m_iCard[id][2] == 255 && m_iCard2[id][0] == 255 && CardPoint(m_iCard[id][0]) == CardPoint(m_iCard[id][1]); }	// Return the flag of split
	bool GetInsurance(int id)	{ return m_iHand[id] && m_iCard[id][2] == 255 && m_iCard2[id][0] == 255 && m_iCard[0][0] % 13 == 12 && !m_iInsurance[id]; }	// Return the flag of insurance
	bool GetSurrender(int id)	{ return m_iHand[id] && m_iCard[id][2] == 255 && m_iCard2[id][0] == 255; }	// Return the flag of surrender
	bool GetNewDeal(int id)		{ return !m_iHand[id] && m_iMoney[id] >= MIN_MONEY; }	// Return the flag of new deal
	bool GetAddMoney(int id)	{ return !m_iHand[id] && m_iMoney[id] < MIN_MONEY; }	// Return the flag of add money

	byte GetHand(int idPlayer)	{ return m_iHand[idPlayer]; }
	int GetSum(int idPlayer)	{ return m_iSum[idPlayer]; }
	int GetSum2(int idPlayer)	{ return m_iSum2[idPlayer]; }
	bool GetActive(int idPlayer){ return m_bActive[idPlayer]; }
	int GetMoney(int idPlayer)	{ return m_iMoney[idPlayer]; }
	byte GetBid(int idPlayer)	{ return m_iBid[idPlayer]; }
	byte GetBid2(int idPlayer)	{ return m_iBid2[idPlayer]; }
	byte GetInsur(int idPlayer)	{ return m_iInsurance[idPlayer]; }
	byte GetCard(int idPlayer, int iCard)	{ return m_iCard[idPlayer][iCard]; }
	byte GetCard2(int idPlayer, int iCard)	{ return m_iCard2[idPlayer][iCard]; }
	byte GetNCard(int idPlayer)	{ return m_nCard[idPlayer]; }
	byte GetNCard2(int idPlayer)	{ return m_nCard2[idPlayer]; }

	float GetPosX(int i, int j)	{ return m_fX[i][j]; }
	float GetPosY(int i, int j)	{ return m_fY[i][j]; }
	float GetPosZ(int i, int j)	{ return m_fZ[i][j]; }
	float GetPosX2(int i, int j)	{ return m_fX2[i][j]; }
	float GetPosY2(int i, int j)	{ return m_fY2[i][j]; }
	float GetPosZ2(int i, int j)	{ return m_fZ2[i][j]; }

	UINT GetNID(int id)	{ return m_nid[id]; }	// Return Network ID

	void lock() { LOCK(); }
	void unlock() { UNLOCK(); }

	UINT MsgConnectPlayer(BYTE *msg, UINT nid);	// Message with current game, return bytes of data

	BYTE ParserConnectPlayer(BYTE *msg);	// Parser for current game, return player ID

	UINT MsgNewDeal(BYTE *msg);			// Message with new deal, return bytes of data
	void ParserNewDeal(BYTE *msg);		// Parser for new deal
};