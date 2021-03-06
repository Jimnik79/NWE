//--------------------------------------------------------------------------------------
// File: Pong.h
// Description: Class of Pingpong.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


#include <windows.h>


class CPong
{
protected:

	int		m_iPointsToWin;	// Points to Win the match (default = 21)
	int		m_iWinsToFinal;	// Wins to the Final (default = 3)
	wchar_t	m_sName[16];	// Player name
	int		m_nScore;	// Score in the current game
	wchar_t	m_sPoints[40];	// Points in the current game
	DWORD	m_iSpeed;	// Start Speed of Ball
	BOOL	m_bSide;	// Side for single game: true - Red, false - Blue

	float	m_fSpeedComp;	// Speed of Comp Racquet
	float	m_fSpeedPlayer;	// Speed of Player Racquet
	float	m_fYRedRacquet;	// Position of Red Racquet
	float	m_fYBlueRacquet;	// Position of Blue Racquet
	float	m_fTorsionRacquet;	// Torsion for the Racquet
	float	m_fYCalc;	// Estimated arrival position for the ball
	float	m_fSpeedStart;	// Start Speed of Ball
	float	m_fSpeedBall;	// Current Speed of Ball
	float	m_fAngleBall;	// 0 - right, pi/2 - down
	float	m_fXBall;	// X position of ball
	float	m_fYBall;	// Y position of ball
	float	m_fXMove;	// Speed ​​of moving the X-axis, pixels per second
	float	m_fYMove;	// Speed ​​of moving the Y-axis, pixels per second

	bool	m_b1Up;	// Flag going up for the player 1
	bool	m_b1Dn;	// Flag going down for the player 1
	bool	m_b2Up;	// Flag going up for the player 2
	bool	m_b2Dn;	// Flag going down for the player 2
	BOOL	m_bAIRed;	// flag to use artificial intelligence (AI) for Red
	BOOL	m_bAIBlue;	// flag to use AI for Blue
	int		m_iPointsRed;	// Red points in the current match
	int		m_iPointsBlue;	// Blue points in the current match
	int		m_iWinsRed;		// Matches won for Red
	int		m_iWinsBlue;	// Matches won for Blue

	bool	m_bGameOver;	// Flag of Game Over
	bool	m_bPause;	// Flag of pause in the game

public:
	CPong();	// Constructor

	void	NewGame();	// Start new game
	void	startSingle(BOOL bSide, LPCWSTR sName, float fSpeed);	// Start single game
	void	start2Players(float fSpeed);	// Start 2 players game
	void	startDemo();	// Start demo play
	void	AIRed(float time);	// AI for Red
	void	AIBlue(float time);	// AI for Blue
	void	YCalc();	// Calculate the trajectory (arrival position) for the ball
	void	ServeRed();	// Serve for Red
	void	ServeBlue();// Serve for Blue
	void	NewMatch();	// Start new match
	void	GameOver();	// Game Over
	bool	frame(float fElapsedTime);	// Frame, return flag of sound

	BOOL	getSide()	{ return m_bSide; }	// Get side for single game: true - Red, false - Blue
	void	setSpeed(DWORD iSpeed)	{ m_iSpeed = iSpeed; }	// Set start speed
	DWORD	getSpeed()	{ return m_iSpeed; }	// Get start speed
	void	setPointsToWin(UINT iPointsToWin)	{ m_iPointsToWin = iPointsToWin; }	// Points to the Win
	void	setWinsToFinal(UINT iWinsToFinal)	{ m_iWinsToFinal = iWinsToFinal; }	// Wins to the Final
	int		getScore()	{ return m_nScore; }	// Return score
	LPCWSTR	getPoints()	{ return m_sPoints; }	// Return the results of matches
	void	set1Up(bool bKeyDown)	{ m_b1Up = bKeyDown; }	// Player 1 up
	void	set1Dn(bool bKeyDown)	{ m_b1Dn = bKeyDown; }	// Player 1 down
	void	set2Up(bool bKeyDown)	{ m_b2Up = bKeyDown; }	// Player 2 up
	void	set2Dn(bool bKeyDown)	{ m_b2Dn = bKeyDown; }	// Player 2 down
	int		getPointsRed()	{ return m_iPointsRed; }	// Return Red points in the current match
	int		getPointsBlue()	{ return m_iPointsBlue; }	// Return Blue points in the current match
	int		getWinsRed()	{ return m_iWinsRed; }	// Return matches won for Red
	int		getWinsBlue()	{ return m_iWinsBlue; }	// Return matches won for Blue
	float	getYRedRacquet()	{ return m_fYRedRacquet; }	// Position of Red Racquet
	float	getYBlueRacquet()	{ return m_fYBlueRacquet; }	// Position of Blue Racquet
	float	getXBall()	{ return m_fXBall; }	// Return X position of ball
	float	getYBall()	{ return m_fYBall; }	// Return Y position of ball
	bool	gameover()	{ if (m_bGameOver) { m_bGameOver = false; return true; } return false; }	// Get flag of game over
	void	pause()	{ m_bPause = !m_bPause; }	// Flag of pause
};