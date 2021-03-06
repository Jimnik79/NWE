//--------------------------------------------------------------------------------------
// File: BJ.cpp
// Description: Class of BlackJack.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


#include <windows.h>
#include <stdlib.h>	// srand
#include <time.h>	// time
#include <memory.h>	// memcpy

#include "BJ.h"		// Class of BlackJack


//--------------------------------------------------------------------------------------
// CONSTRUCTOR
//--------------------------------------------------------------------------------------
CBlackJack::CBlackJack()
{
	m_nDecks = 1;		// Number of decks, max. 8
	m_idCard = 51;		// ID of current card
	m_minCards = 50;	// Minimum cards in the deck before generate new deal

	//  4  2  1  3  5
	for (int j = 0; j < MAX_CARDS; j++)
	{
		m_fX[0][j] = 0.2f*j - 0.4f;
		m_fY[0][j] = 0.003f*j;
		m_fZ[0][j] = -0.2f;

		m_fX[1][j] = 0.12f*j - 0.24f;
		m_fY[1][j] = 0.003f*j;
		m_fZ[1][j] = -2.4f + 0.1f*j;
		m_fX2[1][j] = 0.12f*j - 0.24f;
		m_fY2[1][j] = 0.003f*j;
		m_fZ2[1][j] = -1.4f + 0.1f*j;

		m_fX[2][j] = 0.12f*j - 1.24f;
		m_fY[2][j] = 0.003f*j;
		m_fZ[2][j] = -2.4f + 0.1f*j;
		m_fX2[2][j] = 0.12f*j - 1.24f;
		m_fY2[2][j] = 0.003f*j;
		m_fZ2[2][j] = -1.4f + 0.1f*j;

		m_fX[3][j] = 0.12f*j - 0.76f;
		m_fY[3][j] = 0.003f*j;
		m_fZ[3][j] = -2.4f + 0.1f*j;
		m_fX2[3][j] = 0.12f*j - 0.76f;
		m_fY2[3][j] = 0.003f*j;
		m_fZ2[3][j] = -1.4f + 0.1f*j;

		m_fX[4][j] = 0.12f*j - 2.24f;
		m_fY[4][j] = 0.003f*j;
		m_fZ[4][j] = -2.4f + 0.1f*j;
		m_fX2[4][j] = 0.12f*j - 2.24f;
		m_fY2[4][j] = 0.003f*j;
		m_fZ2[4][j] = -1.4f + 0.1f*j;

		m_fX[5][j] = 0.12f*j - 1.76f;
		m_fY[5][j] = 0.003f*j;
		m_fZ[5][j] = -2.4f + 0.1f*j;
		m_fX2[5][j] = 0.12f*j - 1.76f;
		m_fY2[5][j] = 0.003f*j;
		m_fZ2[5][j] = -1.4f + 0.1f*j;
	}

	DeleteCards();
	InitializeCriticalSection(&m_cs);
}


//--------------------------------------------------------------------------------------
// DESTRUCTOR
//--------------------------------------------------------------------------------------
CBlackJack::~CBlackJack()
{
	DeleteCriticalSection(&m_cs);
}


//--------------------------------------------------------------------------------------
// START
//--------------------------------------------------------------------------------------
void CBlackJack::Start(byte nDeck)
{
	LOCK();

	for (int i = 2; i < ALL; i++)	// All players
		m_bActive[i] = false;	// Are deacivated

	m_nDecks = nDeck;
	m_bActive[1] = true;
	m_iMoney[1] = 100;
	m_nCard2[0] = 0;
	srand((unsigned)time(NULL));	// Initialize the random number generator
	GenerateNewDeal();
	NewDeal(1);

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// GENERATE NEW DEAL
//--------------------------------------------------------------------------------------
void CBlackJack::GenerateNewDeal()
{
	for (byte i = 0; i < m_nDecks; i++)	// All decks
		for (byte j = 0; j < 52; j++)	// All cards
			m_iDeck[52 * i + j] = j;

	int a;	// Random Card of the remaining
	byte b;	// Variable to replace
	for (int i = 52 * m_nDecks - 1; i; i--)
	{
		a = rand() % (i + 1);	// Random Card of the remaining
		b = m_iDeck[i];			// Interchanged with the last Card
		m_iDeck[i] = m_iDeck[a];
		m_iDeck[a] = b;
	}

	m_idCard = 52 * m_nDecks - 1;		// ID of current card
	m_minCards = 40 + 10 * m_nDecks;	// Minimum cards in the deck before generate new deal
}


//--------------------------------------------------------------------------------------
// NEW DEAL
//--------------------------------------------------------------------------------------
void CBlackJack::NewDeal(int idPlayer)	// New deal
{
	m_bNewDeal[idPlayer] = true;	// Flag of ready to new deal

	for (int i = 1; i < ALL; i++)
		if (m_bActive[i] && !m_bNewDeal[i])
			return;

	DeleteCards();

	if (m_idCard < m_minCards)
		GenerateNewDeal();

	for (int i = 1; i < ALL; i++)
		if (m_bActive[i])
		{
			m_iCard[i][0] = m_iDeck[m_idCard--];
			m_iCard[i][1] = m_iDeck[m_idCard--];
			m_iMoney[i] -= m_iBid[i] = 10;
			m_iHand[i] = 1;
			m_nCard[i] = 2;
			Sum(i);
			m_bNewDeal[i] = false;	// Not ready to new deal
		}

	m_nCard[0] = 1;
	m_iCard[0][0] = m_iDeck[m_idCard--];
	Sum(0);	// Dealer Sum
	m_iCard[0][1] = m_iDeck[m_idCard--];
	m_iHand[0] = 0;

	Dealer();	// If all player have BlackJack
}


//--------------------------------------------------------------------------------------
// CALC SUM
//--------------------------------------------------------------------------------------
void CBlackJack::Sum(int idPlayer)
{
	bool bAce = false;	// Flag of Ace, false
	m_iSum[idPlayer] = 0;	// Player Sum, first hand
	for (int j = 0; j < m_nCard[idPlayer]; j++)	// First Hand
	{
		m_iSum[idPlayer] += CardPoint(m_iCard[idPlayer][j]);
		if (m_iCard[idPlayer][j] % 13 == 12)
			bAce = true;	// Flag of Ace
	}
	if (bAce && m_iSum[idPlayer] < 12)
		m_iSum[idPlayer] += 10;

	if (idPlayer)	// Second hand only for player
	{
		bAce = false;	// Flag of Ace, false
		m_iSum2[idPlayer] = 0;	// Player Sum, second hand
		for (int j = 0; j < m_nCard2[idPlayer]; j++)	// First Hand
		{
			m_iSum2[idPlayer] += CardPoint(m_iCard2[idPlayer][j]);
			if (m_iCard2[idPlayer][j] % 13 == 12)
				bAce = true;	// Flag of Ace
		}
		if (bAce && m_iSum2[idPlayer] < 12)
			m_iSum2[idPlayer] += 10;

		if (m_iSum[idPlayer] > 20 || m_nCard[idPlayer]>4)
			m_iHand[idPlayer] = m_nCard2[idPlayer] ? 2 : 0;	// Deal to second hand or end
		if (m_iSum[idPlayer] > 21)
			m_iBid[idPlayer] = 0;

		if (m_iSum2[idPlayer] > 20 || m_nCard2[idPlayer]>4)
			m_iHand[idPlayer] = 0;	// Deal end
		if (m_iSum2[idPlayer] > 21)
			m_iBid2[idPlayer] = 0;
	}
}


//--------------------------------------------------------------------------------------
// DELETE CARDS FROM HANDS
//--------------------------------------------------------------------------------------
void CBlackJack::DeleteCards()
{
	for (int i = 0; i < ALL; i++)	// All players + dealer
		for (int j = 0; j < MAX_CARDS; j++)	// All cards
		{
			m_iSum[i] = 0;
			m_iSum2[i] = 0;
			m_iCard[i][j] = 255;
			m_iCard2[i][j] = 255;
			m_nCard[i] = 0;
			m_nCard2[i] = 0;
			m_iBid[i] = 0;
			m_iBid2[i] = 0;
			m_iInsurance[i] = 0;
		}
}


//--------------------------------------------------------------------------------------
// DEALER CALC
//--------------------------------------------------------------------------------------
void CBlackJack::DealerCalc()
{
	// Dealer final sum
	if (m_iSum[0] > 21)	// If plunk
		m_iSum[0] = 0;
	else if (m_iSum[0] == 21 && m_nCard[0] == 2)	// If BlackJack
		m_iSum[0] = 23;
	else if (m_nCard[0] == 5)
		m_iSum[0] = 22;

	for (int i = 1; i < ALL; i++)	// All players
	{
		if (m_bActive[i])
		{
			if (m_iBid[i])	// FIRST HAND
			{
				//if (m_iSum[i] > 21)	// If plunk in first hand
				//	m_iSum[i] = 0;
				if (m_iSum[i] == 21 && m_nCard[i] == 2)	// If BlackJack in first hand
					m_iSum[i] = 23;
				if (m_nCard[i] == 5)
					m_iSum[i] = 22;
				if (m_iSum[i] > m_iSum[0])	// If player win
				{
					m_iMoney[i] += m_iBid[i] << 1;
					if (m_iSum[i] == 23)	// If player win with BlackJack
						m_iMoney[i] += m_iBid[i] >> 1;	// 3:2
				}
				else if (m_iSum[i] == m_iSum[0])	// 
					m_iMoney[i] += m_iBid[i];
				m_iBid[i] = 0;
			}
			if (m_iBid2[i])	// SECOND HAND
			{
				//if (m_iSum2[i] > 21)	// If plunk in second hand
				//	m_iSum2[i] = 0;
				if (m_iSum2[i] == 21 && m_nCard2[i] == 2)	// If BlackJack in second hand
					m_iSum2[i] = 23;
				if (m_nCard2[i] == 5)	// If 5 card in second hand
					m_iSum2[i] = 22;
				if (m_iSum2[i] > m_iSum[0])	// If player win
				{
					m_iMoney[i] += m_iBid2[i] << 1;
					if (m_iSum2[i] == 23)	// If player win with BlackJack
						m_iMoney[i] += m_iBid2[i] >> 1;	// 3:2
				}
				else if (m_iSum2[i] == m_iSum[0])	// 
					m_iMoney[i] += m_iBid2[i];
				m_iBid2[i] = 0;
			}
			if (m_iInsurance[i])
			{
				if (m_iCard[0][2] == 255 && m_iSum[0] == 23)
					m_iMoney[i] += m_iInsurance[i] + (m_iInsurance[i] << 1);
				m_iInsurance[i] = 0;
			}
		}
	}
}


//--------------------------------------------------------------------------------------
// DEALER PLAY
//--------------------------------------------------------------------------------------
void CBlackJack::Dealer()
{
	for (int i = 1; i < ALL; i++)	// All players
		if (m_bActive[i] && m_iHand[i])
			return;

	if (m_iSum[1] == 21)
		m_iSum[1] = m_iSum[1];

	// Check only BlackJack
	bool bWithoutBJ = false;
	for (int i = 1; i < ALL; i++)	// All players
		if (m_bActive[i])
		{
			if (m_nCard[i] > 2 || m_iSum[i] < 21)	{ bWithoutBJ = true; break; }	// First hand without BlackJack
			if (m_nCard2[i] > 2 || (m_nCard2[i] && m_iSum2[i] < 21))	{ bWithoutBJ = true; break; }	// Second hand without BlackJack
		}
	
	for (int i = 1; i < ALL; i++)	// All players
		if (m_bActive[i] && (m_iBid[i] || m_iBid2[i]))
		{
			m_iHand[0] = 1;	// Dealer Play
			if (bWithoutBJ || m_iSum[0] > 9)	// If there is a hand without BlackJack
			{
				m_nCard[0] = 2;	// Dealer open second card
				Sum(0);	// Dealer sum
			}
			if (bWithoutBJ)	// If there is a hand without BlackJack
				while (m_iSum[0] < 17)
				{
					m_iCard[0][m_nCard[0]++] = m_iDeck[m_idCard--];	// Add card to the dealer
					Sum(0);
				}

			DealerCalc();

			return;
		}
}


//--------------------------------------------------------------------------------------
// GET DEALER CARDS
//--------------------------------------------------------------------------------------
void CBlackJack::GetDealerCards(BYTE* msg)
{
	LOCK();

	for (BYTE j = 1; j < m_nCard[0]; j++)
		msg[j + 1] = m_iCard[0][j];
	for (BYTE j = m_nCard[0] + 1; j < 4; j++)
		msg[j + 1] = 255;

	UNLOCK();
}



/// Check whether all players to play 
/// IF EVERYTHING TO MAKE A GAME DEALER

//--------------------------------------------------------------------------------------
// ADD CARD TO CURRENT PLAYER
//--------------------------------------------------------------------------------------
void CBlackJack::AddCard(int idPlayer, BYTE* msg)
{
	/// Consider the option of 2 cards 2 HANDS
	if (m_iHand[idPlayer] == 1)
	{
		for (int j = 1; j < MAX_CARDS; j++)
			if (m_iCard[idPlayer][j] == 255)
			{
				msg[2] = m_iCard[idPlayer][m_nCard[idPlayer]++] = m_iDeck[m_idCard--];	// Add card to the player
				break;
			}
		Sum(idPlayer);
		if (m_iHand[idPlayer] == 2)	// If go to second hand
		{
			msg[3] = m_iCard2[idPlayer][m_nCard2[idPlayer]++] = m_iDeck[m_idCard--];	// Add card to second hand
			Sum(idPlayer);
		}
	}
	else
	{
		for (int j = 1; j < MAX_CARDS; j++)
			if (m_iCard2[idPlayer][j] == 255)
			{
				msg[2] = m_iCard2[idPlayer][m_nCard2[idPlayer]++] = m_iDeck[m_idCard--];	// Add card to the player
				break;
			}
		Sum(idPlayer);
	}
}

//--------------------------------------------------------------------------------------
// STOP
//--------------------------------------------------------------------------------------
BYTE CBlackJack::Stop(int idPlayer)
{
	BYTE iCard2 = 255;

	if (m_iHand[idPlayer] == 1 && m_nCard2[idPlayer])	// If go to second hand
	{
		m_iHand[idPlayer] = 2;	// Deal to second hand
		iCard2 = m_iCard2[idPlayer][m_nCard2[idPlayer]++] = m_iDeck[m_idCard--];	// Add card to second hand
		Sum(idPlayer);
	}
	else
		m_iHand[idPlayer] = 0;	// Deal end

	return iCard2;
}


//--------------------------------------------------------------------------------------
// SET CARD TO CURRENT PLAYER
//--------------------------------------------------------------------------------------
void CBlackJack::SetCard(BYTE idPlayer, BYTE iCard, BYTE iCard2)
{
	if (m_iHand[idPlayer] == 1)
	{
		for (int j = 1; j < MAX_CARDS; j++)
			if (m_iCard[idPlayer][j] == 255)
			{
				m_iCard[idPlayer][m_nCard[idPlayer]++] = iCard;	// Set card to the player
				m_idCard--;
				break;
			}
		Sum(idPlayer);
		if (m_iHand[idPlayer] == 2)
		{
			for (int j = 1; j < MAX_CARDS; j++)
				if (m_iCard2[idPlayer][j] == 255)
				{
					m_iCard2[idPlayer][m_nCard2[idPlayer]++] = iCard2;	//Set card to the player
					m_idCard--;
					break;
				}
			Sum(idPlayer);
		}
	}
	else if (m_iHand[idPlayer] == 2)
	{
		for (int j = 1; j < MAX_CARDS; j++)
			if (m_iCard2[idPlayer][j] == 255)
			{
				m_iCard2[idPlayer][m_nCard2[idPlayer]++] = iCard;	//Set card to the player
				m_idCard--;
				break;
			}
		Sum(idPlayer);
	}
}


//--------------------------------------------------------------------------------------
// SET HIT TO CURRENT PLAYER
//--------------------------------------------------------------------------------------
void CBlackJack::SetHit(BYTE idPlayer, BYTE iCard, BYTE iCard2)
{
	LOCK();

	SetCard(idPlayer, iCard, iCard2);

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// SET STAND TO CURRENT PLAYER
//--------------------------------------------------------------------------------------
void CBlackJack::SetStand(BYTE idPlayer, BYTE iCard2)
{
	LOCK();

	if (m_iHand[idPlayer] == 1 && m_iCard2[idPlayer][0] < 52)	// If go to second hand
	{
		m_iHand[idPlayer] = 2;	// Deal to second hand
		m_iCard2[idPlayer][m_nCard2[idPlayer]++] = iCard2;// Add card to second hand
		m_idCard--;	
		Sum(idPlayer);
	}
	else
	{
		m_iHand[idPlayer] = 0;	// Deal end
	}

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// SET DOUBLE TO CURRENT PLAYER
//--------------------------------------------------------------------------------------
void CBlackJack::SetDouble(BYTE idPlayer, BYTE iCard, BYTE iCard2)
{
	LOCK();

	if (m_iHand[idPlayer] == 1)	// Double bid on the first hand
	{
		m_iMoney[idPlayer] -= m_iBid[idPlayer];
		m_iBid[idPlayer] <<= 1;
	}
	else if (m_iHand[idPlayer] == 2)	// Double bid the second hand
	{
		m_iMoney[idPlayer] -= m_iBid2[idPlayer];
		m_iBid2[idPlayer] <<= 1;
	}
	SetCard(idPlayer, iCard, iCard2);	// Set card
	if (m_iHand[idPlayer] == 1 && m_iCard2[idPlayer][0] < 52)	// If go to second hand
	{
		m_iHand[idPlayer] = 2;	// Deal to second hand
		m_iCard2[idPlayer][m_nCard2[idPlayer]++] = iCard2;	// Add card to second hand
		m_idCard--;
		Sum(idPlayer);
	}
	else
	{
		m_iHand[idPlayer] = 0;	// Deal end
	}

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// SET SPLIT TO CURRENT PLAYER
//--------------------------------------------------------------------------------------
void CBlackJack::SetSplit(BYTE idPlayer, BYTE iCard, BYTE iCard2)
{
	LOCK();

	m_iMoney[idPlayer] -= m_iBid2[idPlayer] = m_iBid[idPlayer];	// Split bid
	m_iCard2[idPlayer][m_nCard2[idPlayer]++] = m_iCard[idPlayer][--m_nCard[idPlayer]];
	SetCard(idPlayer, iCard, iCard2);	// Set card

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// SET INSURANCE TO CURRENT PLAYER
//--------------------------------------------------------------------------------------
void CBlackJack::SetInsurance(BYTE idPlayer)
{
	LOCK();

	m_iMoney[idPlayer] -= m_iInsurance[idPlayer] = m_iBid[idPlayer] >> 1;	// Insurance
	m_iHand[idPlayer] = 0;

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// SET SURRENDER TO CURRENT PLAYER
//--------------------------------------------------------------------------------------
void CBlackJack::SetSurrender(BYTE idPlayer)
{
	LOCK();

	m_iMoney[idPlayer] += m_iBid[idPlayer] >> 1;	// Back half of bid
	m_iBid[idPlayer] = 0;
	m_iHand[idPlayer] = 0;
	m_nCard[idPlayer] = 0;

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// SET DEALER CARDS
//--------------------------------------------------------------------------------------
void CBlackJack::SetDealerCards(BYTE* msg)
{
	LOCK();

	for (int i = 1; i < 5; i++)
		if (msg[i] < 52)
			m_iCard[0][m_nCard[0]++] = msg[i];
	Sum(0);
	DealerCalc();

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// HIT
//--------------------------------------------------------------------------------------
void CBlackJack::Hit(int idPlayer, BYTE* msg)	// Add card to current player
{
	LOCK();

	AddCard(idPlayer, msg);	// Add card to current player
	//if (!m_iHand[idPlayer])
	Dealer();

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// STAND
//--------------------------------------------------------------------------------------
BYTE CBlackJack::Stand(int idPlayer)	// Stand
{
	LOCK();

	BYTE iCard2 = Stop(idPlayer);
	Dealer();

	UNLOCK();

	return iCard2;
}


//--------------------------------------------------------------------------------------
// DOUBLE BID
//--------------------------------------------------------------------------------------
void CBlackJack::Double(int idPlayer, BYTE* msg)
{
	LOCK();

	if (m_iHand[idPlayer] == 1)	// Double bid on the first hand
	{
		m_iMoney[idPlayer] -= m_iBid[idPlayer];
		m_iBid[idPlayer] <<= 1;
	}
	else if (m_iHand[idPlayer] == 2)	// Double bid the second hand
	{
		m_iMoney[idPlayer] -= m_iBid2[idPlayer];
		m_iBid2[idPlayer] <<= 1;
	}
	AddCard(idPlayer, msg);		// Add card to current player
	msg[3] = Stop(idPlayer);

	Dealer();

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// SPLIT
//--------------------------------------------------------------------------------------
void CBlackJack::Split(int idPlayer, BYTE* msg)
{
	LOCK();

	m_iMoney[idPlayer] -= m_iBid2[idPlayer] = m_iBid[idPlayer];	// Split bid
	m_iCard2[idPlayer][m_nCard2[idPlayer]++] = m_iCard[idPlayer][1];	// Split
	msg[2] = m_iCard[idPlayer][1] = m_iDeck[m_idCard--];	// Add card to first hand
	Sum(idPlayer);

	if (m_iHand[idPlayer] == 2)	// If BlackJack
	{
		msg[3] = m_iCard2[idPlayer][1] = m_iDeck[m_idCard--];	// Add card to second hand
		Sum(idPlayer);
		//if (!m_iHand[idPlayer])	// If BlackJack
		//	Dealer();
	}
	Dealer();

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// INSURANCE
//--------------------------------------------------------------------------------------
void CBlackJack::Insurance(int idPlayer)	// Insurance
{
	LOCK();

	m_iMoney[idPlayer] -= m_iInsurance[idPlayer] = m_iBid[idPlayer] >> 1;	// Insurance
	m_iHand[idPlayer] = 0;
	Dealer();

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// SURRENDER
//--------------------------------------------------------------------------------------
void CBlackJack::Surrender(int idPlayer)
{
	LOCK();

	m_iMoney[idPlayer] += m_iBid[idPlayer] >> 1;	// Money
	m_iHand[idPlayer] = 0;
	m_iBid[idPlayer] = 0;

	//if (m_iMoney[idPlayer] >= MIN_MONEY)
	//	NewDeal(idPlayer);	// New deal
	//else
	//	m_iHand[idPlayer] = 0;

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// NEW DEAL
//--------------------------------------------------------------------------------------
/*void CBlackJack::NreDeal(int idPlayer)
{
	LOCK();

	UNLOCK();
}*/


//--------------------------------------------------------------------------------------
// ADD MONEY
//--------------------------------------------------------------------------------------
void CBlackJack::AddMoney(int idPlayer)	// Money +100
{
	LOCK();

	m_iMoney[idPlayer] += 100;	// Money
	//NewDeal(idPlayer);	// New deal

	UNLOCK();
}


//--------------------------------------------------------------------------------------
// CONNECT PLAYER
//--------------------------------------------------------------------------------------
int CBlackJack::ConnectPlayer(UINT nid)
{
	LOCK();

	for (int i = 1; i < ALL; i++)
	{
		if (!m_bActive[i])
		{
			m_bActive[i] = true;
			m_nid[i] = nid;
			m_iMoney[i] = 0;

			UNLOCK();
			return i;
		}
	}

	UNLOCK();
	return 0;
}


//--------------------------------------------------------------------------------------
// DISCONNECT PLAYER
//--------------------------------------------------------------------------------------
int CBlackJack::DisconnectPlayer(UINT nid)
{
	LOCK();

	for (int i = 1; i < ALL; i++)
	{
		if (m_nid[i] == nid)
		{
			m_bActive[i] = false;
			m_nid[i] = 0;
			m_iSum[i] = 0;

			UNLOCK();
			return i;
		}
	}

	UNLOCK();
	return 0;
}


//--------------------------------------------------------------------------------------
// MESSAGE WITH CURRENT GAME
//--------------------------------------------------------------------------------------
UINT CBlackJack::MsgConnectPlayer(BYTE *msg, UINT nid)
{
	LOCK();

	for (BYTE i = 2; i < ALL; i++)
		if (!m_bActive[i])	// If there is free place
		{
			// Common
			msg[0] = 10;	// Message type
			msg[1] = i - 2 + 4 * m_bActive[3] + 8 * m_bActive[4] + 16 * m_bActive[5];	// Player ID + active players
			msg[2] = m_nDecks;	// Number of decks, max. 8
			memcpy(&msg[3], &m_idCard, 2);	// ID of current card

			// Dealer
			msg[5] = m_nCard[0];
			for (byte j = 0; j < m_nCard[0]; j++)
				msg[6 + j] = m_iCard[0][j];

			// Players
			DWORD iData = 6 + m_nCard[0];
			for (byte n = 1; n < ALL; n++)
				if (m_bActive[n])
				{
					memcpy(&msg[iData], &m_nid[n], 4);		iData += 4;
					memcpy(&msg[iData], &m_iMoney[n], 4);	iData += 4;
					msg[iData++] = m_iBid[n];
					msg[iData++] = m_iBid2[n];
					msg[iData++] = m_iInsurance[n];
					msg[iData++] = m_nCard[n];
					for (byte j = 0; j < m_nCard[n]; j++)
						msg[iData++] = m_iCard[n][j];
					if (m_iBid2[n])
					{
						msg[iData++] = m_nCard2[n];
						for (byte j = 0; j < m_nCard2[n]; j++)
							msg[iData++] = m_iCard2[n][j];
					}
				}

			m_bActive[i] = true;
			m_bNewDeal[i] = true;
			m_nid[i] = nid;
			m_iMoney[i] = 100;
			m_iBid[i] = 0;
			m_iBid2[i] = 0;
			m_iInsurance[i] = 0;
			m_iHand[i] = 0;
			m_nCard[i] = 0;
			m_nCard2[i] = 0;
			m_iSum[i] = 0;
			m_iSum2[i] = 0;

			UNLOCK();
			return iData;
		}

	UNLOCK();
	return 0;	// No free place
}


//--------------------------------------------------------------------------------------
// PARSER FOR CURRENT GAME
//--------------------------------------------------------------------------------------
BYTE CBlackJack::ParserConnectPlayer(BYTE *msg)
{
	LOCK();
	
	// Common
	m_bActive[5] = msg[1] / 16 ? true : false;	msg[1] -= 16 * m_bActive[5];	// Active players
	m_bActive[4] = msg[1] / 8 ? true : false;	msg[1] -= 8 * m_bActive[4];
	m_bActive[3] = msg[1] / 4 ? true : false;	msg[1] -= 4 * m_bActive[3];
	m_bActive[2] = msg[1] ? true : false;
	m_bActive[1] = true;
	m_nDecks = msg[2];	// Number of decks, max. 8
	memcpy(&m_idCard, &msg[3], 2);	// ID of current card

	// Dealer
	m_nCard[0] = msg[5];
	for (byte j = 0; j < m_nCard[0]; j++)
		m_iCard[0][j] = msg[6 + j];
	Sum(0);
	m_iHand[0] = m_nCard[0] == 1;

	// Players
	UINT iData = 6 + m_nCard[0];
	for (byte i = 1; i < ALL; i++)
		if (m_bActive[i])
		{
			memcpy(&m_nid[i], &msg[iData], 4);		iData += 4;
			memcpy(&m_iMoney[i], &msg[iData], 4);	iData += 4;
			m_iBid[i] = msg[iData++];
			m_iBid2[i] = msg[iData++];
			m_iInsurance[i] = msg[iData++];
			m_nCard[i] = msg[iData++];
			for (byte j = 0; j < m_nCard[i]; j++)
				m_iCard[i][j] = msg[iData++];
			if (m_iBid2[i])
			{
				m_nCard2[i] = msg[iData++];
				for (byte j = 0; j < m_nCard2[i]; j++)
					m_iCard2[i][j] = msg[iData++];
			}
			else
				m_nCard2[i] = 0;
			Sum(i);
		}

	byte id = msg[1] + 2;
	m_bActive[id] = true;
	m_bNewDeal[id] = true;
	m_iMoney[id] = 100;
	m_iBid[id] = 0;
	m_iBid2[id] = 0;
	m_iInsurance[id] = 0;
	m_iHand[id] = 0;
	m_nCard[id] = 0;
	m_nCard2[id] = 0;
	m_iSum[id] = 0;
	m_iSum2[id] = 0;

	UNLOCK();

	return id;
}


//--------------------------------------------------------------------------------------
// MESSAGE WITH NEW DEAL
//--------------------------------------------------------------------------------------
UINT CBlackJack::MsgNewDeal(BYTE *msg)
{
	UINT iData = 1;
	msg[0] = 19;	// New deal message

	LOCK();

	msg[1] = m_iCard[0][0];	// Dealer card
	for (int i = 1; i < ALL; i++)	// All active players
		if (m_bActive[i])
		{
			msg[iData++] = m_iBid[i];		// Bid
			msg[iData++] = m_iCard[i][0];	// First card
			msg[iData++] = m_iCard[i][1];	// Second card
		}

	UNLOCK();

	return iData;
}


//--------------------------------------------------------------------------------------
// PARSER FOR NEW DEAL
//--------------------------------------------------------------------------------------
void CBlackJack::ParserNewDeal(BYTE *msg)
{
	UINT iData = 2;

	LOCK();

	m_iCard[0][0] = msg[1];	// Dealer card
	m_idCard--;
	m_iHand[0] = 0;
	m_nCard[0] = 1;
	Sum(0);	// Dealer Sum

	for (int i = 1; i < ALL; i++)	// All active players
		if (m_bActive[i])
		{
			m_iBid[i] = msg[iData++];		// Bid
			msg[iData++] = m_iCard[i][0];	// First card
			msg[iData++] = m_iCard[i][1];	// Second card
			m_idCard -= 2;
			m_iMoney[i] -= m_iBid[i];
			m_iHand[i] = 1;
			m_nCard[i] = 2;
			Sum(i);
		}

	UNLOCK();
}