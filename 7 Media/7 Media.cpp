//--------------------------------------------------------------------------------------
// File: 7 Media.cpp
// Description: Sample of Media: Sound, Voice, Music, Video.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with:
// - sound files (*.wav);
// - media files, including video.

// Sound reproduction occurs in 2 stages:
// 1. Adding the sound file to initializing or other place. Returns the ID of the sound file, starting with 0.
// 2. Playback.

// Media reproduction occurs in 3 stages:
// 1. Adding the media file to initializing or other place. Returns the ID of the sound file, starting with 0.
// 2. Prepare the media file for playback.
// 3. Playback with pause and setting position.

// NOTE:
// - Don't forget to set the volume for sound and media.
// - Use stretching to full sprite for video files as needed.
// - To play sound must be registered XAUDIO2, ie installed DirectX v.43 (june 2010) or later. DirectX SDK v.43 (june 2010) for debugging.

// Used content:
// - Content\Sound\Click.wav
// - Content\Sound\Pong.wav
// - Content\Video\Teleport.avi
// - Content\Video\Galaxy.avi

// In the next version of SDK, maybe, will be added Sound3D in the next version of engine.


#include "..\Engine\NetWars.h"	// Engine

#include <stdio.h>	// swprintf_s


CEngine	g_pAppl;	// Base Engine class
CText	g_pText;	// Text class
CSound	g_pSound;	// Sound class
CMedia	g_pMedia;	// Media class

float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background
float	g_fVolume = 0.5f;	// Sound and media volume
wchar_t	g_sVolume[64];	// String for sound and media volume
wchar_t	g_sPosition[16];
wchar_t	g_sDuration[16] = L"00:00.000";
int		g_iDuration = 0;	// In milliseconds

#define	ID_FONT_16	0

#define	ID_SOUND	0
#define	ID_SOUND2	1

#define	ID_MEDIA	0
#define	ID_MEDIA2	1


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	g_pAppl.setCallbackInitialize();		// Set callback for Initialize
	g_pAppl.setCallbackKeyboard();			// Set callback for Keyboard event
	g_pAppl.setCallbackDeviceCreated();		// Set callback for Create Device
	g_pAppl.setCallbackSwapChainResized();	// Set callback for Resize
	g_pAppl.setCallbackFrameRender();		// Set callback for Render Frame

	g_pText.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_pSound.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_pMedia.setHModule(g_pAppl.getHModule());	// Set NetWars.dll

	// Run Engine
	return g_pAppl.run(L"7 Media", 1024, 600);
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
		case VK_SPACE:	// Pressing space
			g_pSound.play(ID_SOUND);
			break;

		case VK_MENU:	// Pressing alt
			g_pSound.play(ID_SOUND2);
			break;

		case VK_RETURN:	// Pressing enter
			g_pMedia.stretch(false);	// Render the video without stretch
			g_pMedia.prepare(ID_MEDIA);
			g_iDuration = g_pMedia.getDuration(g_sDuration);
			g_pMedia.run();
			break;

		case VK_SHIFT:	// Pressing shift
			g_pMedia.stretch(true);	// Stretching render the video to full sprite
			g_pMedia.prepare(ID_MEDIA2);
			g_iDuration = g_pMedia.getDuration(g_sDuration);
			g_pMedia.run();
			break;

		case 0x50:	// 0x50 P key
			g_pMedia.pause();
			break;

		case 0x52:	// 0x52 R key
			g_pMedia.run();
			break;

		case 0x30:		// 0x30 0 key
		case VK_NUMPAD0:// 0x60 Numeric keypad 0 key
			g_pMedia.pos(0);
			break;

		case 0x31:		// 0x31 1 key
		case VK_NUMPAD1:// 0x61 Numeric keypad 1 key
			g_pMedia.pos(g_iDuration / 10);	// 10%
			break;

		case 0x32:		// 0x32 2 key
		case VK_NUMPAD2:// 0x62 Numeric keypad 2 key
			g_pMedia.pos(g_iDuration * 2 / 10);	// 20%
			break;

		case 0x33:		// 0x33 3 key
		case VK_NUMPAD3:// 0x63 Numeric keypad 3 key
			g_pMedia.pos(g_iDuration * 3 / 10);	// 30%
			break;

		case 0x34:		// 0x34 4 key
		case VK_NUMPAD4:// 0x64 Numeric keypad 4 key
			g_pMedia.pos(g_iDuration * 4 / 10);	// 40%
			break;

		case 0x35:		// 0x35 5 key
		case VK_NUMPAD5:// 0x65 Numeric keypad 5 key
			g_pMedia.pos(g_iDuration * 5 / 10);	// 50%
			break;

		case 0x36:		// 0x36 6 key
		case VK_NUMPAD6:// 0x66 Numeric keypad 6 key
			g_pMedia.pos(g_iDuration * 6 / 10);	// 60%
			break;

		case 0x37:		// 0x37 7 key
		case VK_NUMPAD7:// 0x67 Numeric keypad 7 key
			g_pMedia.pos(g_iDuration * 7 / 10);	// 70%
			break;

		case 0x38:		// 0x38 8 key
		case VK_NUMPAD8:// 0x68 Numeric keypad 8 key
			g_pMedia.pos(g_iDuration * 8 / 10);	// 80%
			break;

		case 0x39:		// 0x39 9 key
		case VK_NUMPAD9:// 0x69 Numeric keypad 9 key
			g_pMedia.pos(g_iDuration * 9 / 10);	// 90%
			break;

		case VK_ADD:		// 0x6B Add key (Num+)
		case VK_OEM_PLUS:	// 0xBB For any country / region, the '+' key
			g_fVolume += 0.01f;
			if (g_fVolume > 1.0f)
				g_fVolume = 1.0f;
			g_pSound.volume(g_fVolume);
			g_pMedia.volume(g_fVolume);
			break;

		case VK_SUBTRACT:	// 0x6D Subtract key (Num-)
		case VK_OEM_MINUS:	// 0xBD For any country / region, the '-' key
			g_fVolume -= 0.01f;
			if (g_fVolume < 0.0f)
				g_fVolume = 0.0f;
			g_pSound.volume(g_fVolume);
			g_pMedia.volume(g_fVolume);
			break;
		}
	}
}


//--------------------------------------------------------------------------------------
// INITIALIZE
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	g_pAppl.addFont(L"Arial", 16, FW_MEDIUM);	// Return ID_FONT_16 = 0, add font for text

	g_pSound.add(L"..\\Content\\Sound\\Click.wav");	// Return ID_SOUND = 0, add sound
	g_pSound.add(L"..\\Content\\Sound\\Pong.wav");	// Return ID_SOUND2 = 1, add sound
	g_pMedia.add(L"..\\Content\\Video\\Teleport.avi");	//Return ID_MEDIA = 0, add media
	g_pMedia.add(L"..\\Content\\Video\\Galaxy.avi");	//Return ID_MEDIA2 = 1, add media

	g_pMedia.init();	// Media initialization
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 0, add space for fps
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 1, add space for timer
	g_pText.add(ID_FONT_16, L"Sound volume = 0.50", DT_LEFT, COLOR_ORANGE, 0.5f);	// Return 2 - Sound volume
	g_pText.add(ID_FONT_16, L"Media volume = 0.50", DT_LEFT, COLOR_ORANGE, 0.5f);	// Return 3 - Media volume
	g_pText.add(ID_FONT_16, L"Playback position = 00:00.000 / 00:00.000", DT_LEFT, COLOR_ORANGE, 0.5f);	// Return 4 - Playback position
	g_pText.add(ID_FONT_16, L"Press 'space' to play sound of click.", DT_LEFT, COLOR_ORANGE, 0.5f);	// Return 5
	g_pText.add(ID_FONT_16, L"Press 'alt' to play sound of ping pong.", DT_LEFT, COLOR_ORANGE, 0.5f);	// Return 6
	g_pText.add(ID_FONT_16, L"Press 'enter' to play videoclip - Teleport.", DT_LEFT, COLOR_ORANGE, 0.5f);	// Return 7
	g_pText.add(ID_FONT_16, L"Press 'shift' to play videoclip with stretch - Galaxy.", DT_LEFT, COLOR_ORANGE, 0.5f);	// Return 8
	g_pText.add(ID_FONT_16, L"Press 'key R' to run play media after pause.", DT_LEFT, COLOR_ORANGE, 0.5f);	// Return 9
	g_pText.add(ID_FONT_16, L"Press 'key P' to pause during media playback.", DT_LEFT, COLOR_ORANGE, 0.5f);	// Return 10
	g_pText.add(ID_FONT_16, L"Press 'key 0-9' to set position media playback.", DT_LEFT, COLOR_ORANGE, 0.5f);	// Return 11
	g_pText.add(ID_FONT_16, L"Press 'plus' to volume up.", DT_LEFT, COLOR_ORANGE, 0.5f);	// Return 12
	g_pText.add(ID_FONT_16, L"Press 'minus' to volume down.", DT_LEFT, COLOR_ORANGE, 0.5f);	// Return 13
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int w, int h)
{
	g_pText.pos(0, 5, 2);	// fps
	g_pText.pos(1, 105, 2);	// timer
	g_pText.pos(2, 5, 30);	// Sound volume
	g_pText.pos(3, 5, 50);	// Media volume
	g_pText.pos(4, 5, 80);	// Playback position
	g_pText.pos(5, 5, 110);	// Press 'space' to play sound of click.
	g_pText.pos(6, 5, 130);	// Press 'alt' to play sound of ping pong.
	g_pText.pos(7, 5, 150);	// Press 'enter' to play videoclip - Teleport.
	g_pText.pos(8, 5, 170);	// Press 'shift' to play videoclip with stretch - Galaxy.
	g_pText.pos(9, 5, 200);	// Press 'key R' to run play media after pause.
	g_pText.pos(10, 5, 220);// Press 'key P' to pause during media playback.
	g_pText.pos(11, 5, 240);// Press 'key 0-9' to set position media playback.
	g_pText.pos(12, 5, 260);// Press 'plus' to volume up.
	g_pText.pos(13, 5, 280);// Press 'minus' to volume down.

	g_pMedia.sprite(0.0f, (float)w, 0.0f, (float)h, 0.9f);	// Media sprite position
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double, float)
{
	g_pText.fps(ID_FONT_16, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT_16, 1, DT_LEFT, COLOR_ORANGE);	// timer

	swprintf_s(g_sVolume, 64, L"Sound volume = %.02f", g_fVolume);	// Sound volume
	g_pText.dynamic(ID_FONT_16, 2, TEXT_NORMAL, DT_LEFT, COLOR_ORANGE, g_sVolume);	// Sound volume
	swprintf_s(g_sVolume, 64, L"Media volume = %.02f", g_fVolume);	// Media volume
	g_pText.dynamic(ID_FONT_16, 3, TEXT_NORMAL, DT_LEFT, COLOR_ORANGE, g_sVolume);	// Media volume

	g_pMedia.getPosition(g_sPosition);
	swprintf_s(g_sVolume, 64, L"Playback position = %s / %s", g_sPosition, g_sDuration);	// Playback position
	g_pText.dynamic(ID_FONT_16, 4, TEXT_NORMAL, DT_LEFT, COLOR_ORANGE, g_sVolume);	// Playback position

	g_pAppl.clearScreen(g_cClear);
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pMedia.draw();
	g_pAppl.setRenderTexts();
	g_pText.draw(0, 14);	// Render fps, timer and oyher texts
}