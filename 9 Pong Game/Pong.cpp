//--------------------------------------------------------------------------------------
// File: Pong.cpp
// Description: Class of Pingpong.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


#include "..\Engine\NetWars.h"	// For registry
#include "Pong.h"	// Class of Pingpong

#include <stdio.h>	// swprintf_s
#include <math.h>	// atan, sin, cos


//--------------------------------------------------------------------------------------
// CONSTRUCTOR
//--------------------------------------------------------------------------------------
CPong::CPong()
{
	m_iPointsToWin = 21;	// Points to Win the match
	m_iWinsToFinal = 3;	// Wins to the Final
	m_nScore = 0;	// Score in the current game
	m_iSpeed = 3;	// Start Speed of Ball
	m_bSide = true;	// Side for single game: true - Red, false - Blue

	m_fSpeedComp = 180.0f;	// Speed of Comp Racquet
	m_fSpeedPlayer = 600.0f;	// Speed of Player Racquet
	m_fYRedRacquet = 300.0f;	// Position of Red Racquet
	m_fYBlueRacquet = 300.0f;	// Position of Blue Racquet
	m_fTorsionRacquet = 0.00001f;	// Torsion for the Racquet
	m_fYCalc = 300.0f;	// Estimated arrival position for the ball
	m_fSpeedStart = 500.0f;	// Start Speed of Ball
	m_fSpeedBall = 500.0f;	// Current Speed of Ball
	m_fAngleBall = 0.0f;	// 0 - right, pi/2 - down
	m_fXBall = 22.0f;	// X position of ball
	m_fYBall = 540.0f;	// Y position of ball
	m_fXMove = 400.0f;	// Speed ​​of moving the X-axis, pixels per second
	m_fYMove = -300.0f;	// Speed ​​of moving the Y-axis, pixels per second

	m_b1Up = false;	// Flag going up for the player 1
	m_b1Dn = false;	// Flag going down for the player 1
	m_b2Up = false;	// Flag going up for the player 2
	m_b2Dn = false;	// Flag going down for the player 2
	m_bAIRed = true;	// flag to use artificial intelligence (AI) for Red
	m_bAIBlue = true;	// flag to use AI for Blue
	m_iPointsRed = 0;	// Red points in the current match
	m_iPointsBlue = 0;	// Blue points in the current match
	m_iWinsRed = 0;		// Matches won for Red
	m_iWinsBlue = 0;	// Matches won for Blue

	m_bGameOver = false;	// Flag of Game Over
	m_bPause = false;	// Flag of pause in the game
}


//--------------------------------------------------------------------------------------
// START NEW GAME
//--------------------------------------------------------------------------------------
void CPong::NewGame()
{
	ServeRed();	// First serve is Red
	YCalc();	// Calculate the trajectory of the ball
	m_iPointsRed = 0;
	m_iPointsBlue = 0;
	m_iWinsRed = 0;
	m_iWinsBlue = 0;
	m_nScore = 0;	// Score in the current game
	m_sPoints[0] = L'\0';	// Points in the current game
	m_fYRedRacquet = m_fYBlueRacquet = 300.0f;	// Position of racquets
}


//--------------------------------------------------------------------------------------
// START SINGLE GAME
//--------------------------------------------------------------------------------------
void CPong::startSingle(BOOL bSide, LPCWSTR sName, float fSpeed)
{
	m_bSide = m_bAIBlue = bSide;	// Side: true - Red, false - Blue
	m_bAIRed = !bSide;
	swprintf_s(m_sName, 16, sName);
	m_fSpeedBall = m_fSpeedStart = fSpeed;
	NewGame();
}


//--------------------------------------------------------------------------------------
// START 2 PLAYERS GAME
//--------------------------------------------------------------------------------------
void CPong::start2Players(float fSpeed)	// Settings for 2 players game
{
	m_bAIRed = m_bAIBlue = false;	// No AI
	m_fSpeedBall = m_fSpeedStart = fSpeed;
	NewGame();
}


//--------------------------------------------------------------------------------------
// START DEMO PLAY
//--------------------------------------------------------------------------------------
void CPong::startDemo()
{
	m_bAIRed = m_bAIBlue = true;	// AI for 2 players
	m_fSpeedBall = m_fSpeedStart = 500.0f;
	NewGame();
}


//--------------------------------------------------------------------------------------
// AI FOR RED
//--------------------------------------------------------------------------------------
void CPong::AIRed(float time)
{
	float fYCalc = m_fXMove > 0.0f ? 300.0f : m_fYCalc;	// Departs / arrives

	if (m_fYRedRacquet < fYCalc)	// Moving up
	{
		m_fYRedRacquet += time * m_fSpeedComp;	// The new position of the racquet
		if (m_fYRedRacquet > fYCalc)	// Movement completed
			m_fYRedRacquet = fYCalc;
	}
	else	// Moving down
	{
		m_fYRedRacquet -= time * m_fSpeedComp;	// The new position of the racquet
		if (m_fYRedRacquet < fYCalc)	// Movement completed
			m_fYRedRacquet = fYCalc;
	}
}


//--------------------------------------------------------------------------------------
// AI FOR BLUE
//--------------------------------------------------------------------------------------
void CPong::AIBlue(float time)	// AI for Blue
{
	float fYCalc = m_fXMove < 0.0f ? 300.0f : m_fYCalc;	// Departs / arrives

	if (m_fYBlueRacquet < fYCalc)	// Moving up
	{
		m_fYBlueRacquet += time * m_fSpeedComp;	// The new position of the racquet
		if (m_fYBlueRacquet > fYCalc)	// Movement completed
			m_fYBlueRacquet = fYCalc;
	}
	else	// Moving down
	{
		m_fYBlueRacquet -= time * m_fSpeedComp;	// The new position of the racquet
		if (m_fYBlueRacquet < fYCalc)	// Movement completed
			m_fYBlueRacquet = fYCalc;
	}
}


//--------------------------------------------------------------------------------------
// CALCULATE THE TRAJECTORY (ARRIVAL POSITION) FOR THE BALL
//--------------------------------------------------------------------------------------
void CPong::YCalc()
{
	if (m_fXMove > 0.0f)
	{
		float delta = m_fYMove*(1012.0f - m_fXBall) / m_fXMove;
		delta -= 960.0f * int(delta / 960.0f);

		m_fYCalc = m_fYBall + delta;
		if (m_fYCalc > 540.0f)
		{
			m_fYCalc = 1080.0f - m_fYCalc;
			if (m_fYCalc < 60.0f)
				m_fYCalc = 120.0f - m_fYCalc;
		}
		if (m_fYCalc < 60.0f)
		{
			m_fYCalc = 120.0f - m_fYCalc;
			if (m_fYCalc > 540.0f)
				m_fYCalc = 1080.0f - m_fYCalc;
		}
	}
	else
	{
		float delta = m_fYMove*(12.0f - m_fXBall) / m_fXMove;
		delta -= 960.0f * int(delta / 960.0f);

		m_fYCalc = m_fYBall + delta;
		if (m_fYCalc > 540.0f)
		{
			m_fYCalc = 1080.0f - m_fYCalc;
			if (m_fYCalc < 60.0f)
				m_fYCalc = 120.0f - m_fYCalc;
		}
		if (m_fYCalc < 60.0f)
		{
			m_fYCalc = 120.0f - m_fYCalc;
			if (m_fYCalc > 540.0f)
				m_fYCalc = 1080.0f - m_fYCalc;
		}
	}
}


//--------------------------------------------------------------------------------------
// SERVE FOR RED
//--------------------------------------------------------------------------------------
void CPong::ServeRed()
{
	m_fSpeedBall = m_fSpeedStart;
	m_fXBall = 22.0f;
	m_fYBall = 540.0f;

	if (m_fYRedRacquet < 300.0f)
		m_fAngleBall = atanf(480.0f / ((300.0f - m_fYRedRacquet) / 210.0f * 490.0f - 980.0f));
	else
		m_fAngleBall = atanf(((m_fYRedRacquet - 300.0f) / 210.0f * 240.0f - 480.0f) / 980.0f);

	m_fXMove = m_fSpeedBall * cosf(m_fAngleBall);
	m_fYMove = m_fSpeedBall * sinf(m_fAngleBall);
}


//--------------------------------------------------------------------------------------
// SERVE FOR BLUE
//--------------------------------------------------------------------------------------
void CPong::ServeBlue()
{
	m_fSpeedBall = m_fSpeedStart;
	m_fXBall = 1002.0f;
	m_fYBall = 60.0f;

	if (m_fYBlueRacquet < 300.0f)
		m_fAngleBall = PI - atanf((480.0f - (300.0f - m_fYBlueRacquet) / 210.0f * 240.0f) / 980.0f);
	else
		m_fAngleBall = PI - atanf(480.0f / (980.0f - (m_fYBlueRacquet - 300.0f) / 210.0f * 490.0f));

	m_fXMove = m_fSpeedBall * cosf(m_fAngleBall);
	m_fYMove = m_fSpeedBall * sinf(m_fAngleBall);
}


//--------------------------------------------------------------------------------------
// START NEW MATCH
//--------------------------------------------------------------------------------------
void CPong::NewMatch()
{
	m_nScore += m_bSide ? m_iPointsRed - m_iPointsBlue : m_iPointsBlue - m_iPointsRed;	// Calc the current score
	swprintf_s(m_sPoints, 40, L"%s%d:%d, ", m_sPoints, m_bSide ? m_iPointsRed : m_iPointsBlue, m_bSide ? m_iPointsBlue : m_iPointsRed);	// Record points
	m_iPointsRed = 0;
	m_iPointsBlue = 0;
	if ((m_iWinsRed + m_iWinsBlue) % 2)
		ServeBlue();
	else
		ServeRed();
}


//--------------------------------------------------------------------------------------
// GAME OVER
//--------------------------------------------------------------------------------------
void CPong::GameOver()
{
	m_nScore += m_bSide ? m_iPointsRed - m_iPointsBlue : m_iPointsBlue - m_iPointsRed;	// Calc the final score
	swprintf_s(m_sPoints, 40, L"%s%d:%d", m_sPoints, m_bSide ? m_iPointsRed : m_iPointsBlue, m_bSide ? m_iPointsBlue : m_iPointsRed);	// Record final points
	m_bGameOver = true;	// Game Over
}


//--------------------------------------------------------------------------------------
// FRAME
//--------------------------------------------------------------------------------------
bool CPong::frame(float fElapsedTime)
{
	if (m_bPause)	// Pause in the game
		return false;

	bool bSound = false;	// Flag of sound

	if (m_bAIRed)	AIRed(fElapsedTime);	// AI for Red if necessary
	if (m_bAIBlue)	AIBlue(fElapsedTime);	// AI for Blue if necessary

	// New racquets position for players
	if (m_b1Up && !m_b1Dn)	m_fYRedRacquet -= m_fSpeedPlayer * fElapsedTime;	if (m_fYRedRacquet < 90.0f) m_fYRedRacquet = 90.0f;
	if (m_b1Dn && !m_b1Up)	m_fYRedRacquet += m_fSpeedPlayer * fElapsedTime;	if (m_fYRedRacquet > 510.0f) m_fYRedRacquet = 510.0f;
	if (m_b2Up && !m_b2Dn)	m_fYBlueRacquet -= m_fSpeedPlayer * fElapsedTime;	if (m_fYBlueRacquet < 90.0f) m_fYBlueRacquet = 90.0f;
	if (m_b2Dn && !m_b2Up)	m_fYBlueRacquet += m_fSpeedPlayer * fElapsedTime;	if (m_fYBlueRacquet > 510.0f) m_fYBlueRacquet = 510.0f;

	// New ball position
	float fXBall = m_fXBall;
	float fYBall = m_fYBall;
	m_fXBall += m_fXMove * fElapsedTime;
	m_fYBall += m_fYMove * fElapsedTime;

	if (m_fYBall < 60.0f)	// UP border
	{
		bSound = true;
		m_fAngleBall = -m_fAngleBall;
		m_fYBall = 120.0f - m_fYBall;
		m_fYMove = -m_fYMove;

	}
	if (m_fYBall > 540.0f)	// DOWN border
	{
		bSound = true;
		m_fAngleBall = -m_fAngleBall;
		m_fYBall = 1080.0f - m_fYBall;
		m_fYMove = -m_fYMove;
	}
	if (fXBall >= 22.0f && m_fXBall < 22.0f)	// RED side
	{
		float fY = fYBall + (m_fYBall - fYBall)*(fXBall - 22.0f) / (fXBall - m_fXBall);
		if (fY < m_fYRedRacquet + 50.0f && fY > m_fYRedRacquet - 50.0f)
		{
			bSound = true;
			m_fSpeedBall *= 1.2f;
			m_fAngleBall = m_fAngleBall > 0.0f ? PI - m_fAngleBall : -PI - m_fAngleBall;
			if (m_bAIRed)	// COMP
			{
				int i = rand() % 3;
				switch (i)
				{
				case 0:	// UP Torsion
					m_fAngleBall -= m_fTorsionRacquet * m_fSpeedComp;	// Add Torsion
					if (m_fAngleBall < -0.9f * PI2)
						m_fAngleBall = -0.9f * PI2;
					break;
				case 1:	// DOWN Torsion
					m_fAngleBall -= m_fTorsionRacquet * m_fSpeedComp;	// Add Torsion
					if (m_fAngleBall > 0.9f * PI2)
						m_fAngleBall = 0.9f * PI2;
					break;
				}
			}
			else	// PLAYER
			{
				if (m_b1Up && !m_b1Dn)	// UP Torsion
				{
					m_fAngleBall -= m_fTorsionRacquet * m_fSpeedPlayer;	// Add Torsion
					if (m_fAngleBall < -0.9f * PI2)
						m_fAngleBall = -0.9f * PI2;
				}
				else if (m_b1Dn && !m_b1Up)	// DOWN Torsion
				{
					m_fAngleBall += m_fTorsionRacquet * m_fSpeedPlayer;	// Add Torsion
					if (m_fAngleBall > 0.9f * PI2)
						m_fAngleBall = 0.9f * PI2;
				}
			}
			m_fXBall = (m_fXBall - 22.0f) / m_fXMove;
			m_fXMove = m_fSpeedBall * cosf(m_fAngleBall);
			m_fYMove = m_fSpeedBall * sinf(m_fAngleBall);
			m_fXBall = 22.0f + m_fXBall * m_fXMove;

			YCalc();
		}
	}
	if (fXBall <= 1002.0f && m_fXBall > 1002.0f)	// BLUE side
	{
		float fY = fYBall + (m_fYBall - fYBall)*(1002.0f - fXBall) / (m_fXBall - fXBall);
		if (fY < m_fYBlueRacquet + 50.0f && fY > m_fYBlueRacquet - 50.0f)
		{
			bSound = true;
			m_fSpeedBall *= 1.2f;
			m_fAngleBall = m_fAngleBall > 0.0f ? PI - m_fAngleBall : -PI - m_fAngleBall;
			if (m_bAIBlue)	// COMP
			{
				int i = rand() % 3;
				switch (i)
				{
				case 0:	// UP Torsion
					m_fAngleBall += m_fTorsionRacquet * m_fSpeedComp;	// Add Torsion
					if (m_fAngleBall > -1.1f * PI2 && m_fAngleBall < 0.0f)
						m_fAngleBall = -1.1f * PI2;
					else if (m_fAngleBall > PI)
						m_fAngleBall -= _2PI;
					break;
				case 1:	// DOWN Torsion
					m_fAngleBall -= m_fTorsionRacquet * m_fSpeedComp;	// Add Torsion
					if (m_fAngleBall < 1.1f * PI2 && m_fAngleBall > 0.0f)
						m_fAngleBall = 1.1f * PI2;
					else if (m_fAngleBall < -PI)
						m_fAngleBall += _2PI;
					break;
				}
			}
			else	// PLAYER
			{
				if (m_b2Up && !m_b2Dn)	// UP Torsion
				{
					m_fAngleBall += m_fTorsionRacquet * m_fSpeedPlayer;	// Add Torsion
					if (m_fAngleBall > -1.1f * PI2 && m_fAngleBall < 0.0f)
						m_fAngleBall = -1.1f * PI2;
					else if (m_fAngleBall > PI)
						m_fAngleBall -= _2PI;
				}
				else if (m_b2Dn && !m_b2Up)	// DOWN Torsion
				{
					m_fAngleBall -= m_fTorsionRacquet * m_fSpeedPlayer;	// Add Torsion
					if (m_fAngleBall < 1.1f * PI2 && m_fAngleBall > 0.0f)
						m_fAngleBall = 1.1f * PI2;
					else if (m_fAngleBall < -PI)
						m_fAngleBall += _2PI;
				}
			}
			m_fXBall = (m_fXBall - 1002.0f) / m_fXMove;
			m_fXMove = m_fSpeedBall * cosf(m_fAngleBall);
			m_fYMove = m_fSpeedBall * sinf(m_fAngleBall);
			m_fXBall = 1002.0f + m_fXBall * m_fXMove;

			YCalc();
		}
	}

	if (m_fXBall < -10.0f || m_fXBall > 1034.0f)	// 1 point
	{
		if (m_bAIRed && m_bAIBlue)	// If demo play
		{
			if (m_fXBall > 1034.0f)	// Point for Red
				ServeRed();
			else	// Point for Blue
				ServeBlue();
		}
		else	// If game with player(s)
		{
			if (m_fXBall > 1034.0f)	// Point for Red
				m_iPointsRed++;
			else	// Point for Blue
				m_iPointsBlue++;

			if (m_iPointsRed < m_iPointsToWin && m_iPointsBlue < m_iPointsToWin)	// Game
			{
				if (((m_iPointsRed + m_iPointsBlue) / 5 + m_iWinsRed + m_iWinsBlue) % 2)
					ServeBlue();
				else
					ServeRed();
			}
			else if (m_iPointsRed > m_iPointsBlue + 1)	// Red won the match 
				
			{
				if (++m_iWinsRed == m_iWinsToFinal)
					GameOver();
				else
					NewMatch();
			}
			else if (m_iPointsBlue > m_iPointsRed + 1)	// Blue won the match 
			{
				if (++m_iWinsBlue == m_iWinsToFinal)
					GameOver();
				else
					NewMatch();
			}
			else if ((m_iPointsRed + m_iPointsBlue + m_iWinsRed + m_iWinsBlue) % 2)	// Draw, submission by one
				ServeBlue();
			else
				ServeRed();
		}
		YCalc();
	}

	return bSound;	// Return flag of sound
}