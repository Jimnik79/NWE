//--------------------------------------------------------------------------------------
// File: 9 Pong Game.cpp
// Description: Sample of 2D Game: Pong.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to create 2D game:
// - state machine, using AI, registry;
// - 2D graphics: sprites, texts and UI;
// - input and time control;
// - sound and media playback.

// Gaming state machine contains 5 states:
// 1. MENU - when the program starts, after exit (exit button click or escape key) or 2 players game over.
// 2. SINGLE - after "Single Game" button click
// 3. TWOPLAYERS - after "2 Players Game" button click
// 4. RECORDS - after Records button click or single game over.
// 5. HELP - after Help button click or F1 key in the menu.

// The whole gameplay is calculated in the class CPong.
// Playing field - 1000*500 pixels.
// 2 racquets (red and blue) - 100 pixels height.
// Ball - 20 pixels in diameter.

// Keys:
// - Mouse - select options;
// - WE - up/down for RED player;
// - ↑↓ - up/down for BLUE player.

// Used content:
// - Content\Sound\Click.wav
// - Content\Sound\Pong.wav
// - Content\Video\Galaxy.avi
// - Content\Texture\Pong.dds

// In the next version of SDK, maybe, will be added language/audio/video settings and short rules to help in the next version of engine.


#include "..\Engine\NetWars.h"	// Engine

#include <stdio.h>	// swprintf_s
#include <stdlib.h>	// srand
#include <time.h>	// time

#include "Pong.h"	// Pingpong class


enum state	// State machine
{
	MENU,
	SINGLE,
	TWOPLAYERS,
	RECORDS,
	HELP,
};
enum state g_eState = MENU;	// State machine

CEngine	g_pGame;	// Base Engine class
CTex	g_pTex;		// Texture class
C2D		g_p2D;		// 2D class
CText	g_pText;	// Text class
CUI		g_pUI;		// UI class
CSound	g_pSound;	// Sound class
CMedia	g_pMedia;	// Media class
CPong	g_pPong;	// Pingpong class

#define	ID_WINDOW_MENU		0
#define	ID_STATIC_NAME		1
#define	ID_EDITBOX_NAME		2
#define	ID_STATIC_SPEED		3
#define	ID_COMBOBOX_SPEED	4
#define	ID_RADIO_SIDE_RED	5
#define	ID_RADIO_SIDE_BLUE	6
#define	ID_CHECK_FULLSCREEN	7
#define	ID_BUTTON_SINGLE	8
#define	ID_BUTTON_2PLRS		9
#define	ID_BUTTON_RECORDS	10
#define	ID_BUTTON_HELP		11
#define	ID_BUTTON_EXIT		12
#define	ID_WINDOW_RECORDS	13
#define	ID_STATIC_RECORDS	14
#define	ID_LISTBOX_RECORDS	15
#define	ID_WINDOW_HELP		16

#define	ID_FONT16	0
#define	ID_FONT20	1
#define	ID_FONT48	2

#define	ID_SOUND1	0
#define	ID_SOUND2	1

#define	ID_MEDIA	0

#define	ID_TEXTURE	0

#define	GROUP_COLOR	0

int g_iW = 1024, g_iH = 600;

wchar_t	g_sKey[] = L"NetWars\\Samples\\8 Pong Game";	// Key for registry


// Write to registry
void WriteRegistry()
{
	HKEY hKey;	// The key for work with the registry
	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, g_sKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL))	// Create the key for work with the registry
	{
		g_pGame.RegBoolWrite(hKey, L"Side", (BOOL)g_pUI.getChecked(ID_RADIO_SIDE_RED));	// Side
		g_pGame.RegStrWrite(hKey, L"Name", g_pUI.getText(ID_EDITBOX_NAME));	// Name
		g_pGame.RegIntWrite(hKey, L"Speed", g_pUI.getSelIndex(ID_COMBOBOX_SPEED));	// Speed
		g_pGame.RegBoolWrite(hKey, L"FullScreen", (BOOL)g_pUI.getChecked(ID_CHECK_FULLSCREEN));	// Full screen
		RegCloseKey(hKey);
	}
}


// State machine - change state
void ChangeState(state newState)
{
	g_eState = newState;	// New state

	if (g_eState == SINGLE)	// Set side for single game
		g_pPong.startSingle(g_pUI.getChecked(ID_RADIO_SIDE_RED), g_pUI.getText(ID_EDITBOX_NAME), (float)g_pUI.getSelectedData(ID_COMBOBOX_SPEED));	// true - Red, false - Blue
	else if (g_eState == TWOPLAYERS)
		g_pPong.start2Players((float)g_pUI.getSelectedData(ID_COMBOBOX_SPEED));	// 
	else
		g_pPong.startDemo();	// 

	if ((g_eState != MENU && g_pUI.getChecked(ID_CHECK_FULLSCREEN) && g_pGame.isWindowed()) || (g_eState == MENU && !g_pGame.isWindowed()))
		g_pGame.toggleFullScreen();

	g_pUI.visible(ID_WINDOW_MENU, g_eState == MENU);
	g_pUI.visible(ID_WINDOW_RECORDS, g_eState == RECORDS);
	g_pUI.visible(ID_WINDOW_HELP, g_eState == HELP);
}


// Exit event (Exit button or Escape key)
void Exit()
{
	if (g_eState == MENU)	// If in Menu
		g_pGame.destroy();	// Exit Game
	else					// Else
		ChangeState(MENU);	// Exit to Menu
}


// Load Records from the text file
void LoadRecords()	// Load Records from text file
{
	FILE* pFile;
	if (NULL == _wfopen_s(&pFile, L"Records.txt", L"r, ccs=UNICODE"))	// Open a text file for reading
	{
		wchar_t s[40], s1[4], sName[16], sPoint[40];
		int i = 0, nScore;
		while (NULL != fgetws(s, 40, pFile))
		{
			swscanf_s(s, L"%d	%[^\n]", &nScore, &sName, 16);	// Score, name
			fgetws(s, 40, pFile);	// Point with "\n"
			swscanf_s(s, L"%[^\n]", &sPoint, 40);	// Point without "\n"
			swprintf_s(s1, 4, L"%i", ++i);
			g_pUI.addItem3(ID_LISTBOX_RECORDS, s1, sName, sPoint, nScore);	// Id, name, points, score (shadow)
		}
		fclose(pFile);	// Close file
	}
}


// Save Records to the text file
void SaveRecords()
{
	FILE* pFile;
	if (NULL == _wfopen_s(&pFile, L"Records.txt", L"w, ccs=UNICODE"))	// Open a text file for writing
	{
		for (int i = 0; i < g_pUI.getSize(ID_LISTBOX_RECORDS); i++)	// All records
		{
			fwprintf(pFile, L"%d	%s\n", g_pUI.getData(ID_LISTBOX_RECORDS, i), g_pUI.getIndexText2(ID_LISTBOX_RECORDS, i));	// Score, name
			fwprintf(pFile, L"%s\n", g_pUI.getIndexText3(ID_LISTBOX_RECORDS, i));	// Points
		}
		fclose(pFile);	// Close file
	}
}


// End of game. Changes to the records if necessary
void GameOver()
{
	if (g_eState == SINGLE)
	{
		wchar_t s[4];
		bool bNeedAdd = true;	// Flag to add the record in the end of records
		for (int i = 0; i < g_pUI.getSize(ID_LISTBOX_RECORDS); i++)	// All records
		{
			if (g_pPong.getScore() > g_pUI.getData(ID_LISTBOX_RECORDS, i))	// Find a place to insert the record
			{
				bNeedAdd = false;
				swprintf_s(s, 4, L"%i", i + 1);	// Id of the record
				g_pUI.insertItem3(ID_LISTBOX_RECORDS, i, s, g_pUI.getText(ID_EDITBOX_NAME), g_pPong.getPoints(), g_pPong.getScore());	// Insert the record
				for (int j = i + 1; j < g_pUI.getSize(ID_LISTBOX_RECORDS); j++)	// Move ids
				{
					swprintf_s(s, 4, L"%i", j + 1);
					g_pUI.setIndexText(ID_LISTBOX_RECORDS, j, s);
				}
				if (g_pUI.getSize(ID_LISTBOX_RECORDS) > 20)	// Remove unnecessary records
					g_pUI.deleteItem(ID_LISTBOX_RECORDS, 20); 
				SaveRecords();	// Save records to the text file
				break;
			}
		}

		if (bNeedAdd && g_pUI.getSize(ID_LISTBOX_RECORDS) < 20)	// Add the record in the end of records
		{
			swprintf_s(s, 4, L"%i", g_pUI.getSize(ID_LISTBOX_RECORDS) + 1);	// Id of the record
			g_pUI.addItem3(ID_LISTBOX_RECORDS, s, g_pUI.getText(ID_EDITBOX_NAME), g_pPong.getPoints(), g_pPong.getScore());	// Add the record
			SaveRecords();	// Save records to the text file
		}

		ChangeState(RECORDS);	// Show records
	}
	else	// if (g_eState == TWOPLAYERS)
		ChangeState(MENU);	// Skip to menu
}


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
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

	// Run Engine
	return g_pGame.run(L"9 Pong Game", 1024, 600);
}


//--------------------------------------------------------------------------------------
// GUI EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onGUI(UINT idEvent, int idControl)
{
	// Sound for click button
	if (idEvent == EVENT_CLICK)
		g_pSound.play(ID_SOUND1);

	//  Button event
	switch (idControl)
	{
	case ID_BUTTON_SINGLE:	// Single game
		WriteRegistry();
		ChangeState(SINGLE);
		break;

	case ID_BUTTON_2PLRS:	// 2 players game
		WriteRegistry();
		ChangeState(TWOPLAYERS);
		break;

	case ID_BUTTON_RECORDS:
		ChangeState(RECORDS);
		break;

	case ID_BUTTON_HELP:
		ChangeState(HELP);
		break;

	case ID_BUTTON_EXIT:
		Exit();
		break;
	}
}


//--------------------------------------------------------------------------------------
// KEYBOARD EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onKeyboard(UINT idKey, bool bKeyDown)
{
	if (bKeyDown)
	{
		switch (idKey)
		{
		case VK_RETURN:	// Enter - media playback
			g_pMedia.prepare(ID_MEDIA);
			g_pMedia.run();
			break;	// 

		case VK_PAUSE:	// Pause in the game
			if (g_eState == SINGLE || g_eState == TWOPLAYERS)
				g_pPong.pause();
			break;	// 

		case VK_F1:	// Help
			if (g_eState == MENU)
				ChangeState(HELP);
			break;

		case VK_ESCAPE:	// Game exit
			Exit();
			break;
		}
	}

	switch (idKey)
	{
	case 0x57:	if ((g_eState == SINGLE && g_pUI.getChecked(ID_RADIO_SIDE_RED)) || g_eState == TWOPLAYERS)	g_pPong.set1Up(bKeyDown);	break;	// 0x57 W key
	case 0x53:	if ((g_eState == SINGLE && g_pUI.getChecked(ID_RADIO_SIDE_RED)) || g_eState == TWOPLAYERS)	g_pPong.set1Dn(bKeyDown);	break;	// 0x53 S key
	case VK_UP:		if ((g_eState == SINGLE && g_pUI.getChecked(ID_RADIO_SIDE_BLUE)) || g_eState == TWOPLAYERS)	g_pPong.set2Up(bKeyDown);	break;	// 0x26 UP ARROW key
	case VK_DOWN:	if ((g_eState == SINGLE && g_pUI.getChecked(ID_RADIO_SIDE_BLUE)) || g_eState == TWOPLAYERS)	g_pPong.set2Dn(bKeyDown);	break;	// 0x28 DOWN ARROW key
	}
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

	// Adding sound and media files
	g_pSound.add(L"..\\Content\\Sound\\Click.wav");	// Return 0, add sound of click
	g_pSound.add(L"..\\Content\\Sound\\Pong.wav");	// Return 1, add sound of ping pong
	g_pMedia.add(L"..\\Content\\Video\\Galaxy.avi");	// Return 0, add video clip

	// Media initialization
	g_pMedia.init();
	g_pMedia.stretch(true);	// Stretching render the video to full sprite

	// Read registry
	HKEY hKey;	// The key for work with the registry
	wchar_t	sName[16] = L"Name";
	DWORD	iSpeed = 3;
	BOOL	bSide = true, bFullScreen = false;
	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, g_sKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL))	// Create the key for work with the registry
	{
		g_pGame.RegBoolRead(hKey, L"Side", &bSide, bSide);		// Side
		g_pGame.RegStrRead(hKey, L"Name", sName, 16, sName);	// Name
		g_pGame.RegIntRead(hKey, L"Speed", &iSpeed, iSpeed);	// Speed
		g_pGame.RegBoolRead(hKey, L"FullScreen", &bFullScreen, bFullScreen);	// Full screen
		RegCloseKey(hKey);
	}

	// Adding UI controls
	g_pUI.addWindow(ID_FONT20, L"MENU", 320, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_MENU = 0

	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT20, L"Name:", DT_RIGHT, COLOR_GOLD, 0.4f);	// Return ID_STATIC_NAME = 1
	g_pUI.addEditBox(ID_WINDOW_MENU, ID_FONT20, sName, 132, 0.4f);	// Return ID_EDITBOX_NAME = 2
	g_pUI.addStatic(ID_WINDOW_MENU, ID_FONT20, L"Speed:", DT_RIGHT, COLOR_GOLD, 0.4f);	// Return ID_STATIC_SPEED = 3
	g_pUI.addComboBox(ID_WINDOW_MENU, ID_FONT20, 132, 0.4f);	// Return ID_COMBOBOX_SPEED = 4
	g_pUI.addItem(ID_COMBOBOX_SPEED, L"100 pix/s", 100);
	g_pUI.addItem(ID_COMBOBOX_SPEED, L"200 pix/s", 200);
	g_pUI.addItem(ID_COMBOBOX_SPEED, L"300 pix/s", 300);
	g_pUI.addItem(ID_COMBOBOX_SPEED, L"500 pix/s", 500);
	g_pUI.addItem(ID_COMBOBOX_SPEED, L"1000 pix/s", 1000);
	g_pUI.addItem(ID_COMBOBOX_SPEED, L"2000 pix/s", 2000);
	g_pUI.addItem(ID_COMBOBOX_SPEED, L"3000 pix/s", 3000);
	g_pUI.selItem(ID_COMBOBOX_SPEED, iSpeed);

	g_pUI.addRadioButton(ID_WINDOW_MENU, ID_FONT20, GROUP_COLOR, L"RED", COLOR_GOLD, 0.4f);	// Return ID_RADIO_SIDE_RED = 5
	g_pUI.addRadioButton(ID_WINDOW_MENU, ID_FONT20, GROUP_COLOR, L"BLUE", COLOR_GOLD, 0.4f);	// Return ID_RADIO_SIDE_BLUE = 6
	if (!bSide)	// If play for Blue
		g_pUI.setChecked(ID_RADIO_SIDE_BLUE, true);

	g_pUI.addCheckBox(ID_WINDOW_MENU, ID_FONT20, L"FullScreen Mode", COLOR_GOLD, bFullScreen ? true : false, 0.4f);	// Return ID_CHECK_FULLSCREEN = 7
	g_pUI.addButton(ID_WINDOW_MENU, ID_FONT20, L"Single Game", 200, 1, 0.4f);	// Return ID_BUTTON_SINGLE = 8
	g_pUI.addButton(ID_WINDOW_MENU, ID_FONT20, L"2 Players Game", 200, 3, 0.4f);	// Return ID_BUTTON_2PLRS = 9
	g_pUI.addButton(ID_WINDOW_MENU, ID_FONT20, L"Records", 200, 0, 0.4f);	// Return ID_BUTTON_RECORDS = 10
	g_pUI.addButton(ID_WINDOW_MENU, ID_FONT20, L"Help", 200, 2, 0.4f);	// Return ID_BUTTON_HELP = 11
	g_pUI.addButton(-1, ID_FONT20, L"Exit", 100, 2, 0.4f);	// Return ID_BUTTON_EXIT = 12

	g_pUI.addWindow(ID_FONT20, L"RECORDS", 560, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_RECORDS = 13
	g_pUI.addStatic(ID_WINDOW_RECORDS, ID_FONT20, L"#    Name                Points", DT_LEFT, COLOR_GOLD, 0.4f);	// Return ID_STATIC_RECORDS = 14
	g_pUI.addListBox(ID_WINDOW_RECORDS, ID_FONT20, 450, 14, 0.4f);	// Return ID_LISTBOX_RECORDS = 15
	g_pUI.set3Column(ID_LISTBOX_RECORDS, 30, 120);

	g_pUI.addWindow(ID_FONT20, L"HELP", 560, 450, COLOR_ORANGE, false, 0.5f);	// Return ID_WINDOW_HELP = 16

	// Initialize the random number generator
	srand((unsigned)time(NULL));

	// Load Records from text file
	LoadRecords();

	// State: MENU
	ChangeState(MENU);
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	// Create texts
	g_pText.addSpace(ID_FONT16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Add space for fps
	g_pText.addSpace(ID_FONT16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Add space for timer
	g_pText.addStroke(ID_FONT48, L"PONG", DT_CENTER, COLOR_ORANGE, COLOR_BLACK, 0.5f);
	g_pText.addShadow(ID_FONT20, L"Version: 0.43β", DT_LEFT, COLOR_WHITE, 0.5f);
	g_pText.addShadow(ID_FONT20, L"Copyright © 2015 Jimnik.All rights reserved.", DT_CENTER, COLOR_WHITE, 0.5f);
	g_pText.addSpace(ID_FONT48, TEXT_STROKE, 60, DT_CENTER, 0.5f);	// Red points
	g_pText.addStroke(ID_FONT48, L":", DT_CENTER, COLOR_ORANGE, COLOR_BLACK, 0.5f);
	g_pText.addSpace(ID_FONT48, TEXT_STROKE, 60, DT_CENTER, 0.5f);	// Blue points
	g_pText.addSpace(ID_FONT48, TEXT_STROKE, 30, DT_CENTER, 0.5f);	// Red wins
	g_pText.addStroke(ID_FONT48, L":", DT_CENTER, COLOR_ORANGE, COLOR_BLACK, 0.5f);
	g_pText.addSpace(ID_FONT48, TEXT_STROKE, 30, DT_CENTER, 0.5f);	// Blue wins
	g_pText.addEx(ID_FONT16, L"Pong is a two-dimensional sports game that simulates table tennis. The player controls an in-game paddle by moving it vertically across the left side of the screen, and can compete against either a computer controlled opponent or another player controlling a second paddle on the opposing side. Players use the paddles to hit a ball back and forth. The aim is for a player to earn more points than the opponent; points are earned when one fails to return the ball to the other.",
		DT_CENTER, COLOR_GOLD, 0.4f, 450, 0, 30);
	g_pText.add(ID_FONT16, L"W, S is the control key for player 1.", DT_LEFT, COLOR_GOLD, 0.4f);
	g_pText.add(ID_FONT16, L"Up, Down - for player 2.", DT_LEFT, COLOR_GOLD, 0.4f);
	g_pText.add(ID_FONT16, L"Enter - for playback of videoclip on the background.", DT_LEFT, COLOR_GOLD, 0.4f);
	g_pText.add(ID_FONT16, L"Pause - for pause in the game.", DT_LEFT, COLOR_GOLD, 0.4f);
	g_pText.add(ID_FONT16, L"This game is the part of NetWars Engine SDK.", DT_LEFT, COLOR_GOLD, 0.4f);

	// Load Texture from file
	g_pTex.add(L"..\\Content\\Texture\\Pong.dds");	// Return ID_TEXTURE = 0
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int w, int h)
{
	int hw = w >> 1;
	int hh = h >> 1;
	g_iW = w;
	g_iH = h;

	// Texts position
	g_pText.pos(0, 5, 2);				// Time
	g_pText.pos(1, 105, 2);				// FPS
	g_pText.pos(2, hw, -4);				// Pong
	g_pText.pos(3, 15, h - 34);			// Version
	g_pText.pos(4, hw, h - 25);			// Rights
	g_pText.pos(5, hw - 35, -4);		// Red points
	g_pText.pos(6, hw, -8);				// :
	g_pText.pos(7, hw + 35, -4);		// Blue points
	g_pText.pos(8, hw - 25, h - 44);	// Red points
	g_pText.pos(9, hw, h - 48);			// :
	g_pText.pos(10, hw + 25, h - 44);	// Blue points
	g_pText.pos(11, hw - 220, hh - 140);// Help 1
	g_pText.pos(12, hw - 190, hh - 26);	// Help 2
	g_pText.pos(13, hw - 190, hh - 8);	// Help 3
	g_pText.pos(14, hw - 190, hh + 10);	// Help 4
	g_pText.pos(15, hw - 190, hh + 28);	// Help 5
	g_pText.pos(16, hw - 190, hh + 150);// Help 6

	// Sprites position
	g_p2D.setTextureSize(1024, 1024);
	g_p2D.add(0, 1024, 0, 600, 0.0f, (float)w, 0.0f, (float)h, 0.9f);	// BackGround
	g_p2D.add(63, 121, 952, 962, 0.0f, (float)w, 40.0f*h / 600.0f, 50.0f*h / 600.0f, 0.8f);	// Border Up
	g_p2D.add(63, 121, 952, 962, 0.0f, (float)w, 550.0f*h / 600.0f, 560.0f*h / 600.0f, 0.8f);	// Border Down
	g_p2D.add(62, 72, 940, 950, 2.0f*w / 1024.0f, 12.0f*w / 1024.0f, 50.0f*h / 600.0f, 550.0f*h / 600.0f, 0.8f);	// Line for Red
	g_p2D.add(62, 72, 940, 950, 1012.0f*w / 1024.0f, 1022.0f*w / 1024.0f, 50.0f*h / 600.0f, 550.0f*h / 600.0f, 0.8f);	// Line for Blue
	g_p2D.add(0, 30, 783, 1023, 2.0f*w / 1024.0f, 12.0f*w / 1024.0f, (g_pPong.getYRedRacquet() - 40.0f)*h / 600.0f, (g_pPong.getYRedRacquet() + 40.0f)*h / 600.0f, 0.7f);	// Red Racquet
	g_p2D.add(31, 61, 783, 1023, 1012.0f*w / 1024.0f, 1022.0f*w / 1024.0f, (g_pPong.getYBlueRacquet() - 40.0f)*h / 600.0f, (g_pPong.getYBlueRacquet() + 40.0f)*h / 600.0f, 0.7f);	// Blue Racquet
	g_p2D.add(62, 122, 963, 1023, (g_pPong.getXBall() - 10.0f)*w / 1024.0f, (g_pPong.getXBall() + 10.0f)*w / 1024.0f, (g_pPong.getYBall() - 10.0f)*h / 600.0f, (g_pPong.getYBall() + 10.0f)*h / 600.0f, 0.6f);	// Ball

	// UI position
	g_pUI.pos(ID_WINDOW_MENU, hw - 155, hh - 225);
	g_pUI.pos(ID_STATIC_NAME, hw - 45, hh - 124);
	g_pUI.pos(ID_EDITBOX_NAME, hw - 40, hh - 130);
	g_pUI.pos(ID_STATIC_SPEED, hw - 45, hh - 84);
	g_pUI.pos(ID_COMBOBOX_SPEED, hw - 40, hh - 90);
	g_pUI.pos(ID_RADIO_SIDE_RED, hw - 85, hh - 50);
	g_pUI.pos(ID_RADIO_SIDE_BLUE, hw + 15, hh - 50);
	g_pUI.pos(ID_CHECK_FULLSCREEN, hw - 85, hh - 20);
	g_pUI.pos(ID_BUTTON_SINGLE, hw - 100, hh + 10);
	g_pUI.pos(ID_BUTTON_2PLRS, hw - 100, hh + 50);
	g_pUI.pos(ID_BUTTON_RECORDS, hw - 100, hh + 90);
	g_pUI.pos(ID_BUTTON_HELP, hw - 100, hh + 130);
	g_pUI.pos(ID_BUTTON_EXIT, w - 104, h - 36);
	g_pUI.pos(ID_WINDOW_RECORDS, hw - 270, hh - 225);
	g_pUI.pos(ID_STATIC_RECORDS, hw - 212, hh - 143);
	g_pUI.pos(ID_LISTBOX_RECORDS, hw - 220, hh - 120);
	g_pUI.pos(ID_WINDOW_HELP, hw - 270, hh - 225);

	// Media sprite position
	g_pMedia.sprite(0.0f, (float)w, 0.0f, (float)h, 0.9f);
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double, float fElapsedTime)
{
	// Frame of game
	if (g_pPong.frame(fElapsedTime) && (g_eState == SINGLE || g_eState == TWOPLAYERS))
		g_pSound.play(ID_SOUND2);
	if (g_pPong.gameover())
		GameOver();

	// Update texts
	g_pText.fps(ID_FONT16, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT16, 1, DT_LEFT, COLOR_ORANGE);	// timer
	if (g_eState == SINGLE || g_eState == TWOPLAYERS)
	{
		wchar_t s[4];
		swprintf_s(s, 4, L"%i", g_pPong.getPointsRed());	g_pText.dynamic(ID_FONT48, 5, TEXT_STROKE, DT_CENTER, COLOR_RED, s);
		swprintf_s(s, 4, L"%i", g_pPong.getPointsBlue());	g_pText.dynamic(ID_FONT48, 7, TEXT_STROKE, DT_CENTER, COLOR_BLUE, s);
		swprintf_s(s, 4, L"%i", g_pPong.getWinsRed());		g_pText.dynamic(ID_FONT48, 8, TEXT_STROKE, DT_CENTER, COLOR_RED, s);
		swprintf_s(s, 4, L"%i", g_pPong.getWinsBlue());		g_pText.dynamic(ID_FONT48, 10, TEXT_STROKE, DT_CENTER, COLOR_BLUE, s);
	}

	// Update sprite buffer
	g_p2D.setTextureSize(1024, 1024);
	g_p2D.update(5, 0, 30, 783, 1023, 2.0f*g_iW / 1024.0f, 12.0f*g_iW / 1024.0f, (g_pPong.getYRedRacquet() - 40.0f)*g_iH / 600.0f, (g_pPong.getYRedRacquet() + 40.0f)*g_iH / 600.0f, 0.7f);	// Red Racquet
	g_p2D.update(6, 31, 61, 783, 1023, 1012.0f*g_iW / 1024.0f, 1022.0f*g_iW / 1024.0f, (g_pPong.getYBlueRacquet() - 40.0f)*g_iH / 600.0f, (g_pPong.getYBlueRacquet() + 40.0f)*g_iH / 600.0f, 0.7f);	// Blue Racquet
	g_p2D.update(7, 62, 122, 963, 1023, (g_pPong.getXBall() - 10.0f)*g_iW / 1024.0f, (g_pPong.getXBall() + 10.0f)*g_iW / 1024.0f, (g_pPong.getYBall() - 10.0f)*g_iH / 600.0f, (g_pPong.getYBall() + 10.0f)*g_iH / 600.0f, 0.6f);	// Ball
	g_p2D.updateBuffer();
	
	// Render
	//g_pGame.clearScreen(g_cClear);
	g_pGame.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_p2D.draw(0, 8);
	g_pMedia.draw();
	g_pUI.render();
	g_pGame.setRenderTexts();
	g_pText.draw(0, 2);	// Render fps and timer

	switch (g_eState)	// State machine
	{
	case SINGLE:
	case TWOPLAYERS:g_pText.draw(5, 6);	break;	// Render points and wins
	default:		g_pText.draw(2, 3); break;	// Render title, version and copyright
	}
	if (g_eState == HELP)
		g_pText.draw(11, 6);
}