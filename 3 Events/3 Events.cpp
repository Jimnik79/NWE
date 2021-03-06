//--------------------------------------------------------------------------------------
// File: 3 Events.cpp
// Description: Sample of events handling for keyboard, mouse, application and timer.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with events from:
// - keyboard, using callback function onKeyboard() to keyboard event handling;
// - mouse, using callback function onMouse() to mouse event handling;
// - application message, using callback function onMsgProc() to application message event handling;
// - timer, using callback function onTimer() to timer event handling;
// - instantaneous and every frame event processing.

// Setting any event (except for the timer) occurs in 2 stages:
// 1. Setting the type of events to entry point to the program: setCallbackKeyboard(), setCallbackMouse(), setCallbackMsgProc(), setCallbackFrameMove().
// 2. Adding event-handling code in the callback functions: onKeyboard(), onMouse(), onMsgProc(), onFrameMove().

// Setting timer events occurs in 2 stages (created only for rare events with a large period of time - about second or more):
// 1. Adding timer to initializing. Returns the ID of the timer (for killing, if necessary), starting with 0.
// 2. Adding event-handling code in the callback function LPCALLBACKTIMER type (it called even if the application is minimized).

// NOTE:
// - For frequent events (a few ms) better to create a timer in its separate thread.


#include "..\Engine\NetWars.h"	// Engine

#include <stdio.h>	// swprintf_s


CEngine	g_pAppl;	// Base Engine class
CText	g_pText;	// Text class

float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background
int		g_iTimer = 0, g_iTimer2 = 0;	// Timer counter
wchar_t	g_sPressKey[80];	// Instantaneous key event
wchar_t	g_sPressKey2[80];	// Every frame key event
wchar_t	g_sPressMouse[80];	// Instantaneous mouse event
wchar_t	g_sPressMouse2[80];	// Every frame mouse event
wchar_t	g_sPosMouse[80];	// Instantaneous mouse position
wchar_t	g_sPosMouse2[80];	// Every frame mouse position
wchar_t	g_sEventAppl[80];	// Applecation event
wchar_t	g_sEventTimer1[80];	// Event of Timer 1
wchar_t	g_sEventTimer2[80];	// Event of Timer 2

int	m_iMouseWheel = 0;	// Mouse Wheel (Z axis)

#define	ID_FONT_20		0
#define	ID_FONT_16		1

#define	ID_TIMER_1SEC	0
#define	ID_TIMER_5SEC	1


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	g_pAppl.setCallbackInitialize();		// Set callback for Initialize
	g_pAppl.setCallbackKeyboard();			// Set callback for Keyboard event
	g_pAppl.setCallbackMouse();				// Set callback for Mouse event
	g_pAppl.setCallbackMsgProc();			// Set callback for Application event
	g_pAppl.setCallbackDeviceCreated();		// Set callback for Create Device
	g_pAppl.setCallbackSwapChainResized();	// Set callback for Resize
	g_pAppl.setCallbackFrameMove();			// Set callback for Frame Move
	g_pAppl.setCallbackFrameRender();		// Set callback for Render Frame

	g_pText.setHModule(g_pAppl.getHModule());	// Set NetWars.dll

	// Run Engine
	return g_pAppl.run(L"3 Events", 1024, 600);
}


//--------------------------------------------------------------------------------------
// TIMERS EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onTimer1sec(UINT)
{
	g_iTimer++;
	swprintf_s(g_sEventTimer1, 80, L"Timer1 counter: %i", g_iTimer);
	if (g_iTimer >= 1000)
		g_pAppl.killTimer(ID_TIMER_1SEC);
}

void CALLBACK onTimer5sec(UINT)
{
	g_iTimer2++;
	swprintf_s(g_sEventTimer2, 80, L"Timer2 counter: %i", g_iTimer2);
	if (g_iTimer2 >= 500)
		g_pAppl.killTimer(ID_TIMER_5SEC);
}


//--------------------------------------------------------------------------------------
// INITIALIZING
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	// Adding fonts
	g_pAppl.addFont(L"Arial", 20, FW_MEDIUM);	// Return ID_FONT_20 = 0, adding font
	g_pAppl.addFont(L"Times new Roman", 16, FW_MEDIUM);	// Return ID_FONT_16 = 1, adding font

	// Adding timer (created only for rare events with a large period of time - about second or more)
	g_pAppl.addTimer(onTimer1sec, 1.0f);	// Add 1 sec timer with events, return ID_TIMER_1SEC = 0
	g_pAppl.addTimer(onTimer5sec, 5.0f);	// Add 5 sec timer with events, return ID_TIMER_5SEC = 1

	// For texts
	swprintf_s(g_sPressKey, 80, L"Last instantaneous press key:");	// Last press key
	swprintf_s(g_sPressKey2, 80, L"Every frame key:");	// Last press key
	swprintf_s(g_sPressMouse, 80, L"Last instantaneous press mouse:");	// Last press mouse
	swprintf_s(g_sPressMouse2, 80, L"Every frame mouse:");	// Last press mouse
	swprintf_s(g_sPosMouse, 80, L"Last instantaneous mouse position:");	// Last mouse position
	swprintf_s(g_sPosMouse2, 80, L"Every frame mouse position:");	// Last mouse position
	swprintf_s(g_sEventAppl, 80, L"Last application event:");	// Last application event
	swprintf_s(g_sEventTimer1, 80, L"Timer1 counter: 0");	// Number of timer1 event
	swprintf_s(g_sEventTimer2, 80, L"Timer2 counter: 0");	// Number of timer2 event
}


//--------------------------------------------------------------------------------------
// KEYBOARD EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onKeyboard(UINT idKey, bool bKeyDown)
{
	wchar_t	sKey[8];

	if (bKeyDown)
	{
		switch (idKey)	// http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
		{
		case VK_LBUTTON:	swprintf_s(sKey, 8, L"LMouse");	break;	// 0x01 Left mouse button
		case VK_RBUTTON:	swprintf_s(sKey, 8, L"RMouse");	break;	// 0x02 Right mouse button
		case VK_CANCEL:		swprintf_s(sKey, 8, L"Cancel");	break;	// 0x03 Control - break processing
		case VK_MBUTTON:	swprintf_s(sKey, 8, L"MMouse");	break;	// 0x04 Middle mouse button (three - button mouse)
		case VK_XBUTTON1:	swprintf_s(sKey, 8, L"X1Mouse");break;	// 0x05 X1 mouse button
		case VK_XBUTTON2:	swprintf_s(sKey, 8, L"X2Mouse");break;	// 0x06 X2 mouse button
		// 0x07	Undefined
		case VK_BACK:		swprintf_s(sKey, 8, L"Back");	break;	// 0x08 BACKSPACE key
		case VK_TAB:		swprintf_s(sKey, 8, L"Tab");	break;	// 0x09 TAB key
		// 0x0A, 0x0B Reserved
		case VK_CLEAR:		swprintf_s(sKey, 8, L"Clear");	break;	// 0x0C CLEAR key
		case VK_RETURN:		swprintf_s(sKey, 8, L"Enter");	break;	// 0x0D ENTER key
		// 0x0E, 0x0F Undefined
		case VK_SHIFT:		swprintf_s(sKey, 8, L"Shift");	break;	// 0x10 SHIFT key
		case VK_CONTROL:	swprintf_s(sKey, 8, L"Ctrl");	break;	// 0x11 CTRL key
		case VK_MENU:		swprintf_s(sKey, 8, L"Alt");	break;	// 0x12 ALT key
		case VK_PAUSE:		swprintf_s(sKey, 8, L"Pause");	break;	// 0x13 PAUSE key
		case VK_CAPITAL:	swprintf_s(sKey, 8, L"Caps");	break;	// 0x14 CAPS LOCK key
		case VK_KANA:		swprintf_s(sKey, 8, L"Kana");	break;	// 0x15 IME Kana mode
		//case VK_HANGUEL:	swprintf_s(sKey, 8, L"Hanguel");break;	// 0x15 IME Hanguel mode(maintained for compatibility; use VK_HANGUL)
		//case VK_HANGUL:		swprintf_s(sKey, 8, L"Hangul");	break;	// 0x15 IME Hangul mode
		// 0x16	Undefined
		case VK_JUNJA:		swprintf_s(sKey, 8, L"Junja");	break;	// 0x17 IME Junja mode
		case VK_FINAL:		swprintf_s(sKey, 8, L"Final");	break;	// 0x18 IME final mode
		case VK_HANJA:		swprintf_s(sKey, 8, L"Hanja");	break;	// 0x19 IME Hanja mode
		//case VK_KANJI:		swprintf_s(sKey, 8, L"Kanji");	break;	// 0x19 IME Kanji mode
		// 0x1A	Undefined
		case VK_ESCAPE:		swprintf_s(sKey, 8, L"Esc");	break;	// 0x1B ESC key
		case VK_CONVERT:	swprintf_s(sKey, 8, L"Convert");break;	// 0x1C IME convert
		case VK_NONCONVERT:	swprintf_s(sKey, 8, L"NonConv");break;	// 0x1D IME nonconvert
		case VK_ACCEPT:		swprintf_s(sKey, 8, L"Accept");	break;	// 0x1E IME accept
		case VK_MODECHANGE:	swprintf_s(sKey, 8, L"Mode");	break;	// 0x1F IME mode change request
		case VK_SPACE:		swprintf_s(sKey, 8, L"Space");	break;	// 0x20 SPACEBAR
		case VK_PRIOR:		swprintf_s(sKey, 8, L"PageUp");	break;	// 0x21 PAGE UP key
		case VK_NEXT:		swprintf_s(sKey, 8, L"PageDn");	break;	// 0x22 PAGE DOWN key
		case VK_END:		swprintf_s(sKey, 8, L"End");	break;	// 0x23 END key
		case VK_HOME:		swprintf_s(sKey, 8, L"Home");	break;	// 0x24 HOME key
		case VK_LEFT:		swprintf_s(sKey, 8, L"Left");	break;	// 0x25 LEFT ARROW key
		case VK_UP:			swprintf_s(sKey, 8, L"Up");		break;	// 0x26 UP ARROW key
		case VK_RIGHT:		swprintf_s(sKey, 8, L"Right");	break;	// 0x27 RIGHT ARROW key
		case VK_DOWN:		swprintf_s(sKey, 8, L"Down");	break;	// 0x28 DOWN ARROW key
		case VK_SELECT:		swprintf_s(sKey, 8, L"Select");	break;	// 0x29 SELECT key
		case VK_PRINT:		swprintf_s(sKey, 8, L"Print");	break;	// 0x2A PRINT key
		case VK_EXECUTE:	swprintf_s(sKey, 8, L"Execute");break;	// 0x2B EXECUTE key
		case VK_SNAPSHOT:	swprintf_s(sKey, 8, L"PrtScr"); break;	// 0x2C PRINT SCREEN key
		case VK_INSERT:		swprintf_s(sKey, 8, L"Ins");	break;	// 0x2D INS key
		case VK_DELETE:		swprintf_s(sKey, 8, L"Del");	break;	// 0x2E DEL key
		case VK_HELP:		swprintf_s(sKey, 8, L"Help");	break;	// 0x2F HELP key
		case 0x30:			swprintf_s(sKey, 8, L"0");		break;	// 0x30 0 key
		case 0x31:			swprintf_s(sKey, 8, L"1");		break;	// 0x31 1 key
		case 0x32:			swprintf_s(sKey, 8, L"2");		break;	// 0x32 2 key
		case 0x33:			swprintf_s(sKey, 8, L"3");		break;	// 0x33 3 key
		case 0x34:			swprintf_s(sKey, 8, L"4");		break;	// 0x34 4 key
		case 0x35:			swprintf_s(sKey, 8, L"5");		break;	// 0x35 5 key
		case 0x36:			swprintf_s(sKey, 8, L"6");		break;	// 0x36 6 key
		case 0x37:			swprintf_s(sKey, 8, L"7");		break;	// 0x37 7 key
		case 0x38:			swprintf_s(sKey, 8, L"8");		break;	// 0x38 8 key
		case 0x39:			swprintf_s(sKey, 8, L"9");		break;	// 0x39 9 key
		// 0x3A	Undefined
		case 0x41:			swprintf_s(sKey, 8, L"A");		break;	// 0x41 A key
		case 0x42:			swprintf_s(sKey, 8, L"B");		break;	// 0x42 B key
		case 0x43:			swprintf_s(sKey, 8, L"C");		break;	// 0x43 C key
		case 0x44:			swprintf_s(sKey, 8, L"D");		break;	// 0x44 D key
		case 0x45:			swprintf_s(sKey, 8, L"E");		break;	// 0x45 E key
		case 0x46:			swprintf_s(sKey, 8, L"F");		break;	// 0x46 F key
		case 0x47:			swprintf_s(sKey, 8, L"G");		break;	// 0x47 G key
		case 0x48:			swprintf_s(sKey, 8, L"H");		break;	// 0x48 H key
		case 0x49:			swprintf_s(sKey, 8, L"I");		break;	// 0x49 I key
		case 0x4A:			swprintf_s(sKey, 8, L"J");		break;	// 0x4A J key
		case 0x4B:			swprintf_s(sKey, 8, L"K");		break;	// 0x4B K key
		case 0x4C:			swprintf_s(sKey, 8, L"L");		break;	// 0x4C L key
		case 0x4D:			swprintf_s(sKey, 8, L"M");		break;	// 0x4D M key
		case 0x4E:			swprintf_s(sKey, 8, L"N");		break;	// 0x4E N key
		case 0x4F:			swprintf_s(sKey, 8, L"O");		break;	// 0x4F O key
		case 0x50:			swprintf_s(sKey, 8, L"P");		break;	// 0x50 P key
		case 0x51:			swprintf_s(sKey, 8, L"Q");		break;	// 0x51 Q key
		case 0x52:			swprintf_s(sKey, 8, L"R");		break;	// 0x52 R key
		case 0x53:			swprintf_s(sKey, 8, L"S");		break;	// 0x53 S key
		case 0x54:			swprintf_s(sKey, 8, L"T");		break;	// 0x54 T key
		case 0x55:			swprintf_s(sKey, 8, L"U");		break;	// 0x55 U key
		case 0x56:			swprintf_s(sKey, 8, L"V");		break;	// 0x56 V key
		case 0x57:			swprintf_s(sKey, 8, L"W");		break;	// 0x57 W key
		case 0x58:			swprintf_s(sKey, 8, L"X");		break;	// 0x58 X key
		case 0x59:			swprintf_s(sKey, 8, L"Y");		break;	// 0x59 Y key
		case 0x5A:			swprintf_s(sKey, 8, L"Z");		break;	// 0x5A Z key
		case VK_LWIN:		swprintf_s(sKey, 8, L"LWin");	break;	// 0x5B Left Windows key (Natural keyboard)
		case VK_RWIN:		swprintf_s(sKey, 8, L"RWin");	break;	// 0x5C Right Windows key (Natural keyboard)
		case VK_APPS:		swprintf_s(sKey, 8, L"Apps");	break;	// 0x5D Applications key (Natural keyboard)
		// 0x5E	Reserved
		case VK_SLEEP:		swprintf_s(sKey, 8, L"Sleep");	break;	// 0x5F Computer Sleep key
		case VK_NUMPAD0:	swprintf_s(sKey, 8, L"Num0");	break;	// 0x60 Numeric keypad 0 key
		case VK_NUMPAD1:	swprintf_s(sKey, 8, L"Num1");	break;	// 0x61 Numeric keypad 1 key
		case VK_NUMPAD2:	swprintf_s(sKey, 8, L"Num2");	break;	// 0x62 Numeric keypad 2 key
		case VK_NUMPAD3:	swprintf_s(sKey, 8, L"Num3");	break;	// 0x63 Numeric keypad 3 key
		case VK_NUMPAD4:	swprintf_s(sKey, 8, L"Num4");	break;	// 0x64 Numeric keypad 4 key
		case VK_NUMPAD5:	swprintf_s(sKey, 8, L"Num5");	break;	// 0x65 Numeric keypad 5 key
		case VK_NUMPAD6:	swprintf_s(sKey, 8, L"Num6");	break;	// 0x66 Numeric keypad 6 key
		case VK_NUMPAD7:	swprintf_s(sKey, 8, L"Num7");	break;	// 0x67 Numeric keypad 7 key
		case VK_NUMPAD8:	swprintf_s(sKey, 8, L"Num8");	break;	// 0x68 Numeric keypad 8 key
		case VK_NUMPAD9:	swprintf_s(sKey, 8, L"Num9");	break;	// 0x69 Numeric keypad 9 key
		case VK_MULTIPLY:	swprintf_s(sKey, 8, L"Num*");	break;	// 0x6A Multiply key
		case VK_ADD:		swprintf_s(sKey, 8, L"Num+");	break;	// 0x6B Add key
		case VK_SEPARATOR:	swprintf_s(sKey, 8, L"Separ");	break;	// 0x6C Separator key
		case VK_SUBTRACT:	swprintf_s(sKey, 8, L"Num-");	break;	// 0x6D Subtract key
		case VK_DECIMAL:	swprintf_s(sKey, 8, L"Num,");	break;	// 0x6E Decimal key
		case VK_DIVIDE:		swprintf_s(sKey, 8, L"Num/");	break;	// 0x6F Divide key
		case VK_F1:			swprintf_s(sKey, 8, L"F1");		break;	// 0x70 F1 key
		case VK_F2:			swprintf_s(sKey, 8, L"F2");		break;	// 0x71 F2 key
		case VK_F3:			swprintf_s(sKey, 8, L"F3");		break;	// 0x72 F3 key
		case VK_F4:			swprintf_s(sKey, 8, L"F4");		break;	// 0x73 F4 key
		case VK_F5:			swprintf_s(sKey, 8, L"F5");		break;	// 0x74 F5 key
		case VK_F6:			swprintf_s(sKey, 8, L"F6");		break;	// 0x75 F6 key
		case VK_F7:			swprintf_s(sKey, 8, L"F7");		break;	// 0x76 F7 key
		case VK_F8:			swprintf_s(sKey, 8, L"F8");		break;	// 0x77 F8 key
		case VK_F9:			swprintf_s(sKey, 8, L"F9");		break;	// 0x78 F9 key
		case VK_F10:		swprintf_s(sKey, 8, L"F10");	break;	// 0x79 F10 key
		case VK_F11:		swprintf_s(sKey, 8, L"F11");	break;	// 0x7A F11 key
		case VK_F12:		swprintf_s(sKey, 8, L"F12");	break;	// 0x7B F12 key
		case VK_F13:		swprintf_s(sKey, 8, L"F13");	break;	// 0x7C F13 key
		case VK_F14:		swprintf_s(sKey, 8, L"F14");	break;	// 0x7D F14 key
		case VK_F15:		swprintf_s(sKey, 8, L"F15"); 	break;	// 0x7E F15 key
		case VK_F16:		swprintf_s(sKey, 8, L"F16"); 	break;	// 0x7F F16 key
		case VK_F17:		swprintf_s(sKey, 8, L"F17"); 	break;	// 0x80 F17 key
		case VK_F18:		swprintf_s(sKey, 8, L"F18"); 	break;	// 0x81 F18 key
		case VK_F19:		swprintf_s(sKey, 8, L"F19"); 	break;	// 0x82 F19 key
		case VK_F20:		swprintf_s(sKey, 8, L"F20"); 	break;	// 0x83 F20 key
		case VK_F21:		swprintf_s(sKey, 8, L"F21"); 	break;	// 0x84 F21 key
		case VK_F22:		swprintf_s(sKey, 8, L"F22"); 	break;	// 0x85 F22 key
		case VK_F23:		swprintf_s(sKey, 8, L"F23"); 	break;	// 0x86 F23 key
		case VK_F24:		swprintf_s(sKey, 8, L"F24"); 	break;	// 0x87 F24 key
		// 0x88 - 0x8F Unassigned
		case VK_NUMLOCK:	swprintf_s(sKey, 8, L"Num"); 	break;	// 0x90 NUM LOCK key
		case VK_SCROLL:		swprintf_s(sKey, 8, L"Scroll"); break;	// 0x91 SCROLL LOCK key
		// 0x92 - 0x96 OEM specific
		// 0x97 - 0x9F Unassigned
		case VK_LSHIFT:		swprintf_s(sKey, 8, L"LShift");	break;	// 0xA0 Left SHIFT key
		case VK_RSHIFT:		swprintf_s(sKey, 8, L"RShift");	break;	// 0xA1 Right SHIFT key
		case VK_LCONTROL:	swprintf_s(sKey, 8, L"LCtrl");	break;	// 0xA2 Left CONTROL key
		case VK_RCONTROL:	swprintf_s(sKey, 8, L"RCtrl");	break;	// 0xA3 Right CONTROL key
		case VK_LMENU:		swprintf_s(sKey, 8, L"LAlt");	break;	// 0xA4 Left MENU key
		case VK_RMENU:		swprintf_s(sKey, 8, L"RAlt");	break;	// 0xA5 Right MENU key
		case VK_BROWSER_BACK:		swprintf_s(sKey, 8, L"BrBack");	break;	// 0xA6 Browser Back key
		case VK_BROWSER_FORWARD:	swprintf_s(sKey, 8, L"BrForw");	break;	// 0xA7 Browser Forward key
		case VK_BROWSER_REFRESH:	swprintf_s(sKey, 8, L"BrRefr");	break;	// 0xA8 Browser Refresh key
		case VK_BROWSER_STOP:		swprintf_s(sKey, 8, L"BrStop");	break;	// 0xA9 Browser Stop key
		case VK_BROWSER_SEARCH:		swprintf_s(sKey, 8, L"BrSear");	break;	// 0xAA Browser Search key
		case VK_BROWSER_FAVORITES:	swprintf_s(sKey, 8, L"BrFavor");break;	// 0xAB Browser Favorites key
		case VK_BROWSER_HOME:		swprintf_s(sKey, 8, L"BrHome");	break;	// 0xAC Browser Start and Home key
		case VK_VOLUME_MUTE:		swprintf_s(sKey, 8, L"VolMute");break;	// 0xAD Volume Mute key
		case VK_VOLUME_DOWN:		swprintf_s(sKey, 8, L"VolDn");	break;	// 0xAE Volume Down key
		case VK_VOLUME_UP:			swprintf_s(sKey, 8, L"VolUp");	break;	// 0xAF Volume Up key
		case VK_MEDIA_NEXT_TRACK:	swprintf_s(sKey, 8, L"Next");	break;	// 0xB0 Next Track key
		case VK_MEDIA_PREV_TRACK:	swprintf_s(sKey, 8, L"Prev");	break;	// 0xB1 Previous Track key
		case VK_MEDIA_STOP:			swprintf_s(sKey, 8, L"Stop");	break;	// 0xB2 Stop Media key
		case VK_MEDIA_PLAY_PAUSE:	swprintf_s(sKey, 8, L"Pause");	break;	// 0xB3 Play / Pause Media key
		case VK_LAUNCH_MAIL:		swprintf_s(sKey, 8, L"Mail");	break;	// 0xB4 Start Mail key
		case VK_LAUNCH_MEDIA_SELECT:swprintf_s(sKey, 8, L"Media");	break;	// 0xB5 Select Media key
		case VK_LAUNCH_APP1:		swprintf_s(sKey, 8, L"App1");	break;	// 0xB6 Start Application 1 key
		case VK_LAUNCH_APP2:		swprintf_s(sKey, 8, L"App2");	break;	// 0xB7 Start Application 2 key
		// 0xB8, 0xB9 Reserved
		case VK_OEM_1:		swprintf_s(sKey, 8, L";:");	break;	// 0xBA Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
		case VK_OEM_PLUS:	swprintf_s(sKey, 8, L"=+");	break;	// 0xBB For any country / region, the '+' key
		case VK_OEM_COMMA:	swprintf_s(sKey, 8, L",<");	break;	// 0xBC For any country / region, the ',' key
		case VK_OEM_MINUS:	swprintf_s(sKey, 8, L"-_");	break;	// 0xBD For any country / region, the '-' key
		case VK_OEM_PERIOD:	swprintf_s(sKey, 8, L".>");	break;	// 0xBE For any country / region, the '.' key
		case VK_OEM_2:		swprintf_s(sKey, 8, L"/?");		break;	// 0xBF Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
		case VK_OEM_3:		swprintf_s(sKey, 8, L"~`");		break;	// 0xC0 Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
		// 0xC1 - 0xD7 Reserved
		// 0xD8 - 0xDA Unassigned
		case VK_OEM_4:		swprintf_s(sKey, 8, L"[{");		break;	// 0xDB Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
		case VK_OEM_5:		swprintf_s(sKey, 8, L"\\|");	break;	// 0xDC Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
		case VK_OEM_6:		swprintf_s(sKey, 8, L"]}");		break;	// 0xDD Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
		case VK_OEM_7:		swprintf_s(sKey, 8, L"\"'");	break;	// 0xDE Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
		case VK_OEM_8:		swprintf_s(sKey, 8, L"OEM8");	break;	// 0xDF Used for miscellaneous characters; it can vary by keyboard.
		// 0xE0 Reserved
		// 0xE1 OEM specific
		case VK_OEM_102:	swprintf_s(sKey, 8, L"RT102");	break;	// 0xE2 Either the angle bracket key or the backslash key on the RT 102 - key keyboard
		// 0xE3, 0xE4 OEM specific
		case VK_PROCESSKEY:	swprintf_s(sKey, 8, L"Process");break;	// 0xE5 IME PROCESS key
		// 0xE6 OEM specific
		case VK_PACKET:		swprintf_s(sKey, 8, L"Packet");	break;	// 0xE7 Used to pass Unicode characters as if they were keystrokes.The VK_PACKET key is the low word of a 32 - bit Virtual Key value used for non - keyboard input methods.For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
		// 0xE8 Unassigned
		// 0xE9 - 0xF5 OEM specific
		case VK_ATTN:		swprintf_s(sKey, 8, L"Attn");	break;	// 0xF6 Attn key
		case VK_CRSEL:		swprintf_s(sKey, 8, L"CrSel");	break;	// 0xF7 CrSel key
		case VK_EXSEL:		swprintf_s(sKey, 8, L"ExSel");	break;	// 0xF8 ExSel key
		case VK_EREOF:		swprintf_s(sKey, 8, L"ErEOF");	break;	// 0xF9 Erase EOF key
		case VK_PLAY:		swprintf_s(sKey, 8, L"Play");	break;	// 0xFA Play key
		case VK_ZOOM:		swprintf_s(sKey, 8, L"Zoom");	break;	// 0xFB Zoom key
		case VK_NONAME:		swprintf_s(sKey, 8, L"NoName");	break;	// 0xFC Reserved
		case VK_PA1:		swprintf_s(sKey, 8, L"PA1");	break;	// 0xFD PA1 key
		case VK_OEM_CLEAR:	swprintf_s(sKey, 8, L"Clear");	break;	// 0xFE Clear key
		case 0xFF:			swprintf_s(sKey, 8, L"ResFF");	break;	// 0xFF Reserved
		}

		if (idKey == 0x5A)	// 0x5A Z key
		{
			if (GetKeyState(VK_CONTROL) < 0)	// Control + Z
				swprintf_s(sKey, 8, L"Ctrl+Z");
			if (GetKeyState(VK_SHIFT) < 0)	// Shift + Z
				swprintf_s(sKey, 8, L"Shift+Z");
			if (GetKeyState(VK_MENU) < 0)	// Alt + Z
				swprintf_s(sKey, 8, L"Alt+Z");
			if (GetKeyState(VK_LWIN) < 0 || GetKeyState(VK_RWIN) < 0)	// Win + Z
				swprintf_s(sKey, 8, L"Win+Z");
		}

		swprintf_s(g_sPressKey, 80, L"Last press key: %s", sKey);	// Last press key
	}
}


//--------------------------------------------------------------------------------------
// MOUSE EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos)
{
	if (bLeftButtonDown)
		swprintf_s(g_sPressMouse, 80, L"Last instantaneous press mouse: LMouse");
	if (bRightButtonDown)
		swprintf_s(g_sPressMouse, 80, L"Last instantaneous press mouse: RMouse");
	if (bMiddleButtonDown)
		swprintf_s(g_sPressMouse, 80, L"Last instantaneous press mouse: MMouse");
	if (bSideButton1Down)
		swprintf_s(g_sPressMouse, 80, L"Last instantaneous press mouse: X1Mouse");
	if (bSideButton2Down)
		swprintf_s(g_sPressMouse, 80, L"Last instantaneous press mouse: X2Mouse");
	if (nMouseWheelDelta)
		swprintf_s(g_sPressMouse, 80, L"Last instantaneous press mouse: Delta - %i", nMouseWheelDelta);

	swprintf_s(g_sPosMouse, 80, L"Last instantaneous mouse position: %i, %i", xPos, yPos);	// Last mouse position
}


//--------------------------------------------------------------------------------------
// APPLICATION MESSAGE EVENT HANDLING
//--------------------------------------------------------------------------------------
LRESULT CALLBACK onMsgProc(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam, bool*)
{
	switch (uMsg)
	{
	case WM_PAINT:
		//swprintf_s(g_sEventAppl, 80, L"Last application event: WM_PAINT");
		break;

	case WM_SIZE:
		if (SIZE_MINIMIZED == wParam)
		{
			swprintf_s(g_sEventAppl, 80, L"Last application event: SIZE_MINIMIZED");
		}
		else
		{
			RECT rcCurrentClient;
			GetClientRect(g_pAppl.getHWND(), &rcCurrentClient);
			if (rcCurrentClient.top == 0 && rcCurrentClient.bottom == 0)
			{
				// Rapidly clicking the task bar to minimize and restore a window can cause a WM_SIZE message with SIZE_RESTORED when the window has actually become minimized due to rapid change so just ignore this message
				swprintf_s(g_sEventAppl, 80, L"Last application event: SIZE_0");
			}
			else if (SIZE_MAXIMIZED == wParam)
			{
				swprintf_s(g_sEventAppl, 80, L"Last application event: SIZE_MAXIMIZED");
			}
			else if (SIZE_RESTORED == wParam)
			{
				swprintf_s(g_sEventAppl, 80, L"Last application event: SIZE_RESTORED");
			}
		}
		break;

	case WM_GETMINMAXINFO:
		swprintf_s(g_sEventAppl, 80, L"Last application event: WM_GETMINMAXINFO");
		break;

	case WM_ENTERSIZEMOVE:
		swprintf_s(g_sEventAppl, 80, L"Last application event: WM_ENTERSIZEMOVE");
		break;

	case WM_EXITSIZEMOVE:
		swprintf_s(g_sEventAppl, 80, L"Last application event: WM_EXITSIZEMOVE");
		break;

	case WM_MOUSEMOVE:
		//swprintf_s(g_sEventAppl, 80, L"Last application event: WM_MOUSEMOVE");
		break;

	case WM_SETCURSOR:
		//swprintf_s(g_sEventAppl, 80, L"Last application event: WM_SETCURSOR");
		break;

	case WM_ACTIVATEAPP:
		if (wParam == TRUE && !g_pAppl.isActive())	// Handle only if previously not active 
		{
			swprintf_s(g_sEventAppl, 80, L"Last application event: WM_ACTIVATE");
		}
		else if (wParam == FALSE && g_pAppl.isActive())	// Handle only if previously active 
		{
			swprintf_s(g_sEventAppl, 80, L"Last application event: WM_NOTACTIVATE");
		}
		break;

	case WM_ENTERMENULOOP:
		swprintf_s(g_sEventAppl, 80, L"Last application event: WM_ENTERMENULOOP");
		break;

	case WM_EXITMENULOOP:
		swprintf_s(g_sEventAppl, 80, L"Last application event: WM_EXITMENULOOP");
		break;

	case WM_MENUCHAR:
		swprintf_s(g_sEventAppl, 80, L"Last application event: WM_MENUCHAR");
		break;

	case WM_NCHITTEST:
		//swprintf_s(g_sEventAppl, 80, L"Last application event: WM_NCHITTEST");
		break;

	case WM_POWERBROADCAST:
		switch (wParam)
		{
#ifndef PBT_APMQUERYSUSPEND
#define PBT_APMQUERYSUSPEND 0x0000
#endif
		case PBT_APMQUERYSUSPEND:	// At this point, the app should save any data for open network connections, files, etc., and prepare to go into a suspended mode.  The app can use the MsgProc callback to handle this if desired.
			swprintf_s(g_sEventAppl, 80, L"Last application event: PBT_APMQUERYSUSPEND");
			return true;

#ifndef PBT_APMRESUMESUSPEND
#define PBT_APMRESUMESUSPEND 0x0007
#endif
		case PBT_APMRESUMESUSPEND:	// At this point, the app should recover any data, network connections, files, etc., and resume running from when the app was suspended. The app can use the MsgProc callback to handle this if desired.
			swprintf_s(g_sEventAppl, 80, L"Last application event: PBT_APMRESUMESUSPEND");
			return true;
		}
		break;

	case WM_SYSCOMMAND:	// Prevent moving/sizing in full screen mode
		switch ((wParam & 0xFFF0))
		{
		case SC_MOVE:
			swprintf_s(g_sEventAppl, 80, L"Last application event: SC_MOVE");
			break;
		case SC_SIZE:
			swprintf_s(g_sEventAppl, 80, L"Last application event: SC_SIZE");
			break;
		case SC_MAXIMIZE:
			swprintf_s(g_sEventAppl, 80, L"Last application event: SC_MAXIMIZE");
			break;
		case SC_KEYMENU:
			swprintf_s(g_sEventAppl, 80, L"Last application event: SC_KEYMENU");
			break;
		}
		break;

	case WM_SYSKEYDOWN:
	{
		switch (wParam)
		{
		case VK_RETURN:
			swprintf_s(g_sEventAppl, 80, L"Last application event: VK_RETURN");
			if (lParam & 1 << 29)	// If change fullscreen/windowed mode
			{
				swprintf_s(g_sEventAppl, 80, L"Last application event: ALT + ENTER");
			}
		}
		break;
	}

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			swprintf_s(g_sEventAppl, 80, L"Last application event: VK_ESCAPE");
			break;
		case VK_PAUSE:
			swprintf_s(g_sEventAppl, 80, L"Last application event: VK_PAUSE");
			break;
		}
		break;
	}

	case WM_CLOSE:
		swprintf_s(g_sEventAppl, 80, L"Last application event: WM_CLOSE");
		return 0;

	case WM_ERASEBKGND:
		swprintf_s(g_sEventAppl, 80, L"Last application event: WM_ERASEBKGND");
		return 1;

	case WM_DESTROY:
		swprintf_s(g_sEventAppl, 80, L"Last application event: WM_DESTROY");
		break;
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Add space for fps
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Add space for timer

	g_pText.addSpace(ID_FONT_20, TEXT_SHADOW, 512, DT_LEFT, 0.5f);	// Instantaneous key event
	g_pText.addSpace(ID_FONT_20, TEXT_SHADOW, 512, DT_LEFT, 0.5f);	// Every frame key event
	g_pText.addSpace(ID_FONT_20, TEXT_SHADOW, 512, DT_LEFT, 0.5f);	// Instantaneous mouse event
	g_pText.addSpace(ID_FONT_20, TEXT_SHADOW, 512, DT_LEFT, 0.5f);	// Every frame mouse event
	g_pText.addSpace(ID_FONT_20, TEXT_SHADOW, 512, DT_LEFT, 0.5f);	// Instantaneous mouse position
	g_pText.addSpace(ID_FONT_20, TEXT_SHADOW, 512, DT_LEFT, 0.5f);	// Every frame mouse position
	g_pText.addSpace(ID_FONT_20, TEXT_SHADOW, 512, DT_LEFT, 0.5f);	// Applecation event
	g_pText.addSpace(ID_FONT_20, TEXT_SHADOW, 512, DT_LEFT, 0.5f);	// Event of Timer 1
	g_pText.addSpace(ID_FONT_20, TEXT_SHADOW, 512, DT_LEFT, 0.5f);	// Event of Timer 2
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int, int)
{
	g_pText.pos(0, 5, 2);	// FPS
	g_pText.pos(1, 105, 2);	// Time

	g_pText.pos(2, 5, 50);	// Instantaneous key event
	g_pText.pos(3, 5, 80);	// Every frame key event
	g_pText.pos(4, 5, 110);	// Instantaneous mouse event
	g_pText.pos(5, 5, 140);	// Every frame mouse event
	g_pText.pos(6, 5, 170);	// Instantaneous mouse position
	g_pText.pos(7, 5, 200);	// Every frame mouse position
	g_pText.pos(8, 5, 230);	// Applecation event
	g_pText.pos(9, 5, 260);	// Event of Timer 1
	g_pText.pos(10, 5, 290);// Event of Timer 2
}


//--------------------------------------------------------------------------------------
// UPDATES TO THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameMove(double, float)	// time, fTime
{
	swprintf_s(g_sPosMouse2, 80, L"Every frame mouse position: %i, %i", g_pAppl.getMouseX(), g_pAppl.getMouseY());

	m_iMouseWheel += g_pAppl.getMouseWheel();
		
	swprintf_s(g_sPressMouse2, 80, L"Every frame mouse: %s, %s, %s, %i, %s, %s",
		g_pAppl.getLMouseClick() ? L"Lclick" : (g_pAppl.getLMouseUnClick() ? L"Lunclick" : (g_pAppl.getLMousePress() ? L"Lpress" : L"Lnon")),
		g_pAppl.getRMouseClick() ? L"Rclick" : (g_pAppl.getRMouseUnClick() ? L"Runclick" : (g_pAppl.getRMousePress() ? L"Rpress" : L"Rnon")),
		g_pAppl.getMMouseClick() ? L"Mclick" : (g_pAppl.getMMouseUnClick() ? L"Munclick" : (g_pAppl.getMMousePress() ? L"Mpress" : L"Mnon")), m_iMouseWheel,
		g_pAppl.getX1MouseClick() ? L"X1click" : (g_pAppl.getX1MouseUnClick() ? L"X1unclick" : (g_pAppl.getX1MousePress() ? L"X1press" : L"X1non")),
		g_pAppl.getX2MouseClick() ? L"X2click" : (g_pAppl.getX2MouseUnClick() ? L"X2unclick" : (g_pAppl.getX2MousePress() ? L"X2press" : L"X2non")));

	// You can also use wasKeyPressed() or wasKeyUnPressed() instead isKeyDown() to determine pressing or unpressing the buttons for the last frame
	swprintf_s(g_sPressKey2, 80, L"Every frame key:");
	if (g_pAppl.isKeyDown(VK_CANCEL))	swprintf_s(g_sPressKey2, 80, L"%s Cancel", g_sPressKey2);	// 0x03 Control - break processing
	if (g_pAppl.isKeyDown(VK_BACK))		swprintf_s(g_sPressKey2, 80, L"%s Back", g_sPressKey2);		// 0x08 BACKSPACE key
	if (g_pAppl.isKeyDown(VK_TAB))		swprintf_s(g_sPressKey2, 80, L"%s Tab", g_sPressKey2);		// 0x09 TAB key
	if (g_pAppl.isKeyDown(VK_CLEAR))	swprintf_s(g_sPressKey2, 80, L"%s Clear", g_sPressKey2);	// 0x0C CLEAR key
	if (g_pAppl.isKeyDown(VK_RETURN))	swprintf_s(g_sPressKey2, 80, L"%s Enter", g_sPressKey2);	// 0x0D ENTER key
	//if (g_pAppl.isKeyDown(VK_SHIFT))	swprintf_s(g_sPressKey2, 80, L"%s Shift", g_sPressKey2);	// 0x10 SHIFT key
	//if (g_pAppl.isKeyDown(VK_CONTROL))	swprintf_s(g_sPressKey2, 80, L"%s Ctrl", g_sPressKey2);		// 0x11 CTRL key
	//if (g_pAppl.isKeyDown(VK_MENU))		swprintf_s(g_sPressKey2, 80, L"%s Alt", g_sPressKey2);		// 0x12 ALT key
	if (g_pAppl.isKeyDown(VK_PAUSE))	swprintf_s(g_sPressKey2, 80, L"%s Pause", g_sPressKey2);	// 0x13 PAUSE key
	if (g_pAppl.isKeyDown(VK_CAPITAL))	swprintf_s(g_sPressKey2, 80, L"%s Caps", g_sPressKey2);		// 0x14 CAPS LOCK key
	if (g_pAppl.isKeyDown(VK_KANA))		swprintf_s(g_sPressKey2, 80, L"%s Kana", g_sPressKey2);		// 0x15 IME Kana mode
	if (g_pAppl.isKeyDown(VK_JUNJA))	swprintf_s(g_sPressKey2, 80, L"%s Junja", g_sPressKey2);	// 0x17 IME Junja mode
	if (g_pAppl.isKeyDown(VK_FINAL))	swprintf_s(g_sPressKey2, 80, L"%s Final", g_sPressKey2);	// 0x18 IME final mode
	if (g_pAppl.isKeyDown(VK_HANJA))	swprintf_s(g_sPressKey2, 80, L"%s Hanja", g_sPressKey2);	// 0x19 IME Hanja mode
	if (g_pAppl.isKeyDown(VK_ESCAPE))	swprintf_s(g_sPressKey2, 80, L"%s Esc", g_sPressKey2);		// 0x1B ESC key
	if (g_pAppl.isKeyDown(VK_CONVERT))	swprintf_s(g_sPressKey2, 80, L"%s Convert", g_sPressKey2);	// 0x1C IME convert
	if (g_pAppl.isKeyDown(VK_NONCONVERT))	swprintf_s(g_sPressKey2, 80, L"%s NonConv", g_sPressKey2);	// 0x1D IME nonconvert
	if (g_pAppl.isKeyDown(VK_ACCEPT))	swprintf_s(g_sPressKey2, 80, L"%s Accept", g_sPressKey2);	// 0x1E IME accept
	if (g_pAppl.isKeyDown(VK_MODECHANGE))	swprintf_s(g_sPressKey2, 80, L"%s Mode", g_sPressKey2);	// 0x1F IME mode change request
	if (g_pAppl.isKeyDown(VK_SPACE))	swprintf_s(g_sPressKey2, 80, L"%s Space", g_sPressKey2);	// 0x20 SPACEBAR
	if (g_pAppl.isKeyDown(VK_PRIOR))	swprintf_s(g_sPressKey2, 80, L"%s PageUp", g_sPressKey2);	// 0x21 PAGE UP key
	if (g_pAppl.isKeyDown(VK_NEXT))		swprintf_s(g_sPressKey2, 80, L"%s PageDn", g_sPressKey2);	// 0x22 PAGE DOWN key
	if (g_pAppl.isKeyDown(VK_END))		swprintf_s(g_sPressKey2, 80, L"%s End", g_sPressKey2);		// 0x23 END key
	if (g_pAppl.isKeyDown(VK_HOME))		swprintf_s(g_sPressKey2, 80, L"%s Home", g_sPressKey2);		// 0x24 HOME key
	if (g_pAppl.isKeyDown(VK_LEFT))		swprintf_s(g_sPressKey2, 80, L"%s Left", g_sPressKey2);		// 0x25 LEFT ARROW key
	if (g_pAppl.isKeyDown(VK_UP))		swprintf_s(g_sPressKey2, 80, L"%s Up", g_sPressKey2);		// 0x26 UP ARROW key
	if (g_pAppl.isKeyDown(VK_RIGHT))	swprintf_s(g_sPressKey2, 80, L"%s Right", g_sPressKey2);	// 0x27 RIGHT ARROW key
	if (g_pAppl.isKeyDown(VK_DOWN))		swprintf_s(g_sPressKey2, 80, L"%s Down", g_sPressKey2);		// 0x28 DOWN ARROW key
	if (g_pAppl.isKeyDown(VK_SELECT))	swprintf_s(g_sPressKey2, 80, L"%s Select", g_sPressKey2);	// 0x29 SELECT key
	if (g_pAppl.isKeyDown(VK_PRINT))	swprintf_s(g_sPressKey2, 80, L"%s Print", g_sPressKey2);	// 0x2A PRINT key
	if (g_pAppl.isKeyDown(VK_EXECUTE))	swprintf_s(g_sPressKey2, 80, L"%s Execute", g_sPressKey2);	// 0x2B EXECUTE key
	if (g_pAppl.isKeyDown(VK_SNAPSHOT))	swprintf_s(g_sPressKey2, 80, L"%s PrtScr", g_sPressKey2);	// 0x2C PRINT SCREEN key
	if (g_pAppl.isKeyDown(VK_INSERT))	swprintf_s(g_sPressKey2, 80, L"%s Ins", g_sPressKey2);		// 0x2D INS key
	if (g_pAppl.isKeyDown(VK_DELETE))	swprintf_s(g_sPressKey2, 80, L"%s Del", g_sPressKey2);		// 0x2E DEL key
	if (g_pAppl.isKeyDown(VK_HELP))		swprintf_s(g_sPressKey2, 80, L"%s Help", g_sPressKey2);		// 0x2F HELP key
	if (g_pAppl.isKeyDown(VK_LWIN))		swprintf_s(g_sPressKey2, 80, L"%s LWin", g_sPressKey2);		// 0x5B Left Windows key (Natural keyboard)
	if (g_pAppl.isKeyDown(VK_RWIN))		swprintf_s(g_sPressKey2, 80, L"%s RWin", g_sPressKey2);		// 0x5C Right Windows key (Natural keyboard)
	if (g_pAppl.isKeyDown(VK_APPS))		swprintf_s(g_sPressKey2, 80, L"%s Apps", g_sPressKey2);		// 0x5D Applications key (Natural keyboard)
	if (g_pAppl.isKeyDown(VK_SLEEP))	swprintf_s(g_sPressKey2, 80, L"%s Sleep", g_sPressKey2);	// 0x5F Computer Sleep key
	if (g_pAppl.isKeyDown(VK_MULTIPLY))	swprintf_s(g_sPressKey2, 80, L"%s Num*", g_sPressKey2);		// 0x6A Multiply key
	if (g_pAppl.isKeyDown(VK_ADD))		swprintf_s(g_sPressKey2, 80, L"%s Num+", g_sPressKey2);		// 0x6B Add key
	if (g_pAppl.isKeyDown(VK_SEPARATOR))swprintf_s(g_sPressKey2, 80, L"%s Separ", g_sPressKey2);	// 0x6C Separator key
	if (g_pAppl.isKeyDown(VK_SUBTRACT))	swprintf_s(g_sPressKey2, 80, L"%s Num-", g_sPressKey2);		// 0x6D Subtract key
	if (g_pAppl.isKeyDown(VK_DECIMAL))	swprintf_s(g_sPressKey2, 80, L"%s Num,", g_sPressKey2);		// 0x6E Decimal key
	if (g_pAppl.isKeyDown(VK_DIVIDE))	swprintf_s(g_sPressKey2, 80, L"%s Num/", g_sPressKey2);		// 0x6F Divide key
	if (g_pAppl.isKeyDown(VK_NUMLOCK))	swprintf_s(g_sPressKey2, 80, L"%s Num", g_sPressKey2);		// 0x90 NUM LOCK key
	if (g_pAppl.isKeyDown(VK_SCROLL))	swprintf_s(g_sPressKey2, 80, L"%s Scroll", g_sPressKey2);	// 0x91 SCROLL LOCK key
	if (g_pAppl.isKeyDown(VK_LSHIFT))	swprintf_s(g_sPressKey2, 80, L"%s LShift", g_sPressKey2);	// 0xA0 Left SHIFT key
	if (g_pAppl.isKeyDown(VK_RSHIFT))	swprintf_s(g_sPressKey2, 80, L"%s RShift", g_sPressKey2);	// 0xA1 Right SHIFT key
	if (g_pAppl.isKeyDown(VK_LCONTROL))	swprintf_s(g_sPressKey2, 80, L"%s LCtrl", g_sPressKey2);	// 0xA2 Left CONTROL key
	if (g_pAppl.isKeyDown(VK_RCONTROL))	swprintf_s(g_sPressKey2, 80, L"%s RCtrl", g_sPressKey2);	// 0xA3 Right CONTROL key
	if (g_pAppl.isKeyDown(VK_LMENU))	swprintf_s(g_sPressKey2, 80, L"%s LAlt", g_sPressKey2);		// 0xA4 Left MENU key
	if (g_pAppl.isKeyDown(VK_RMENU))	swprintf_s(g_sPressKey2, 80, L"%s RAlt", g_sPressKey2);		// 0xA5 Right MENU key
	if (g_pAppl.isKeyDown(VK_BROWSER_BACK))	swprintf_s(g_sPressKey2, 80, L"%s BrBack", g_sPressKey2);	// 0xA6 Browser Back key
	if (g_pAppl.isKeyDown(VK_BROWSER_FORWARD))	swprintf_s(g_sPressKey2, 80, L"%s BrForw", g_sPressKey2);	// 0xA7 Browser Forward key
	if (g_pAppl.isKeyDown(VK_BROWSER_REFRESH))	swprintf_s(g_sPressKey2, 80, L"%s BrRefr", g_sPressKey2);	// 0xA8 Browser Refresh key
	if (g_pAppl.isKeyDown(VK_BROWSER_STOP))	swprintf_s(g_sPressKey2, 80, L"%s BrStop", g_sPressKey2);	// 0xA9 Browser Stop key
	if (g_pAppl.isKeyDown(VK_BROWSER_SEARCH))	swprintf_s(g_sPressKey2, 80, L"%s BrSear", g_sPressKey2);	// 0xAA Browser Search key
	if (g_pAppl.isKeyDown(VK_BROWSER_FAVORITES))	swprintf_s(g_sPressKey2, 80, L"%s BrFavor", g_sPressKey2);	// 0xAB Browser Favorites key
	if (g_pAppl.isKeyDown(VK_BROWSER_HOME))	swprintf_s(g_sPressKey2, 80, L"%s BrHome", g_sPressKey2);	// 0xAC Browser Start and Home key
	if (g_pAppl.isKeyDown(VK_VOLUME_MUTE))	swprintf_s(g_sPressKey2, 80, L"%s VolMute", g_sPressKey2);	// 0xAD Volume Mute key
	if (g_pAppl.isKeyDown(VK_VOLUME_DOWN))	swprintf_s(g_sPressKey2, 80, L"%s VolDn", g_sPressKey2);	// 0xAE Volume Down key
	if (g_pAppl.isKeyDown(VK_VOLUME_UP))	swprintf_s(g_sPressKey2, 80, L"%s VolUp", g_sPressKey2);	// 0xAF Volume Up key
	if (g_pAppl.isKeyDown(VK_MEDIA_NEXT_TRACK))	swprintf_s(g_sPressKey2, 80, L"%s Next", g_sPressKey2);	// 0xB0 Next Track key
	if (g_pAppl.isKeyDown(VK_MEDIA_PREV_TRACK))	swprintf_s(g_sPressKey2, 80, L"%s Prev", g_sPressKey2);	// 0xB1 Previous Track key
	if (g_pAppl.isKeyDown(VK_MEDIA_STOP))	swprintf_s(g_sPressKey2, 80, L"%s Stop", g_sPressKey2);	// 0xB2 Stop Media key
	if (g_pAppl.isKeyDown(VK_MEDIA_PLAY_PAUSE))	swprintf_s(g_sPressKey2, 80, L"%s Pause", g_sPressKey2);	// 0xB3 Play / Pause Media key
	if (g_pAppl.isKeyDown(VK_LAUNCH_MAIL))	swprintf_s(g_sPressKey2, 80, L"%s Mail", g_sPressKey2);	// 0xB4 Start Mail key
	if (g_pAppl.isKeyDown(VK_LAUNCH_MEDIA_SELECT))	swprintf_s(g_sPressKey2, 80, L"%s Media", g_sPressKey2);	// 0xB5 Select Media key
	if (g_pAppl.isKeyDown(VK_LAUNCH_APP1))	swprintf_s(g_sPressKey2, 80, L"%s App1", g_sPressKey2);	// 0xB6 Start Application 1 key
	if (g_pAppl.isKeyDown(VK_LAUNCH_APP2))	swprintf_s(g_sPressKey2, 80, L"%s App2", g_sPressKey2);	// 0xB7 Start Application 2 key
	if (g_pAppl.isKeyDown(VK_OEM_1))	swprintf_s(g_sPressKey2, 80, L"%s ;:", g_sPressKey2);		// 0xBA Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
	if (g_pAppl.isKeyDown(VK_OEM_PLUS))	swprintf_s(g_sPressKey2, 80, L"%s =+", g_sPressKey2);		// 0xBB For any country / region, the '+' key
	if (g_pAppl.isKeyDown(VK_OEM_COMMA))	swprintf_s(g_sPressKey2, 80, L"%s ,<", g_sPressKey2);	// 0xBC For any country / region, the ',' key
	if (g_pAppl.isKeyDown(VK_OEM_MINUS))	swprintf_s(g_sPressKey2, 80, L"%s -_", g_sPressKey2);	// 0xBD For any country / region, the '-' key
	if (g_pAppl.isKeyDown(VK_OEM_PERIOD))	swprintf_s(g_sPressKey2, 80, L"%s .>", g_sPressKey2);	// 0xBE For any country / region, the '.' key
	if (g_pAppl.isKeyDown(VK_OEM_2))	swprintf_s(g_sPressKey2, 80, L"%s /?", g_sPressKey2);		// 0xBF Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
	if (g_pAppl.isKeyDown(VK_OEM_3))	swprintf_s(g_sPressKey2, 80, L"%s ~`", g_sPressKey2);		// 0xC0 Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
	if (g_pAppl.isKeyDown(VK_OEM_4))	swprintf_s(g_sPressKey2, 80, L"%s [{", g_sPressKey2);		// 0xDB Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
	if (g_pAppl.isKeyDown(VK_OEM_5))	swprintf_s(g_sPressKey2, 80, L"%s \\|", g_sPressKey2);		// 0xDC Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
	if (g_pAppl.isKeyDown(VK_OEM_6))	swprintf_s(g_sPressKey2, 80, L"%s ]}", g_sPressKey2);		// 0xDD Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
	if (g_pAppl.isKeyDown(VK_OEM_7))	swprintf_s(g_sPressKey2, 80, L"%s \"'", g_sPressKey2);		// 0xDE Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
	if (g_pAppl.isKeyDown(VK_OEM_8))	swprintf_s(g_sPressKey2, 80, L"%s OEM8", g_sPressKey2);		// 0xDF Used for miscellaneous characters; it can vary by keyboard.
	if (g_pAppl.isKeyDown(VK_OEM_102))	swprintf_s(g_sPressKey2, 80, L"%s RT102", g_sPressKey2);	// 0xE2 Either the angle bracket key or the backslash key on the RT 102 - key keyboard
	if (g_pAppl.isKeyDown(VK_PROCESSKEY))	swprintf_s(g_sPressKey2, 80, L"%s Process", g_sPressKey2);	// 0xE5 IME PROCESS key
	if (g_pAppl.isKeyDown(VK_PACKET))	swprintf_s(g_sPressKey2, 80, L"%s Packet", g_sPressKey2);	// 0xE7 Used to pass Unicode characters as if they were keystrokes.The VK_PACKET key is the low word of a 32 - bit Virtual Key value used for non - keyboard input methods.For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
	if (g_pAppl.isKeyDown(VK_ATTN))		swprintf_s(g_sPressKey2, 80, L"%s Attn", g_sPressKey2);		// 0xF6 Attn key
	if (g_pAppl.isKeyDown(VK_CRSEL))	swprintf_s(g_sPressKey2, 80, L"%s CrSel", g_sPressKey2);	// 0xF7 CrSel key
	if (g_pAppl.isKeyDown(VK_EXSEL))	swprintf_s(g_sPressKey2, 80, L"%s ExSel", g_sPressKey2);	// 0xF8 ExSel key
	if (g_pAppl.isKeyDown(VK_EREOF))	swprintf_s(g_sPressKey2, 80, L"%s ErEOF", g_sPressKey2);	// 0xF9 Erase EOF key
	if (g_pAppl.isKeyDown(VK_PLAY))		swprintf_s(g_sPressKey2, 80, L"%s Play", g_sPressKey2);		// 0xFA Play key
	if (g_pAppl.isKeyDown(VK_ZOOM))		swprintf_s(g_sPressKey2, 80, L"%s Zoom", g_sPressKey2);		// 0xFB Zoom key
	if (g_pAppl.isKeyDown(VK_NONAME))	swprintf_s(g_sPressKey2, 80, L"%s NoName", g_sPressKey2);	// 0xFC Reserved
	if (g_pAppl.isKeyDown(VK_PA1))		swprintf_s(g_sPressKey2, 80, L"%s PA1", g_sPressKey2);		// 0xFD PA1 key
	if (g_pAppl.isKeyDown(VK_OEM_CLEAR))swprintf_s(g_sPressKey2, 80, L"%s Clear", g_sPressKey2);	// 0xFE Clear key
	if (g_pAppl.isKeyDown(0xFF))		swprintf_s(g_sPressKey2, 80, L"%s ResFF", g_sPressKey2);	// 0xFF Reserved

	for (BYTE i = 0x30; i < 0x3A; i++)
		if (g_pAppl.isKeyDown(i))
			swprintf_s(g_sPressKey2, 80, L"%s %c", g_sPressKey2, i);	// 0x30 0 key - 0x39 9 key
	for (BYTE i = 0x41; i < 0x5B; i++)
		if (g_pAppl.isKeyDown(i))
			swprintf_s(g_sPressKey2, 80, L"%s %c", g_sPressKey2, i);	// 0x41 A key - 0x5A Z key
	for (BYTE i = 0x60; i < 0x6A; i++)
		if (g_pAppl.isKeyDown(i))
			swprintf_s(g_sPressKey2, 80, L"%s Num%i", g_sPressKey2, i - 0x60);	// 0x60 Numeric keypad 0 key - 0x69 Numeric keypad 9 key
	for (BYTE i = 0x70; i < 0x88; i++)
		if (g_pAppl.isKeyDown(i))
			swprintf_s(g_sPressKey2, 80, L"%s F%i", g_sPressKey2, i - 0x6F);	// 0x70 F1 key - 0x87 F24 key
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double, float)
{
	// Update dynamic texts
	g_pText.fps(ID_FONT_16, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT_16, 1, DT_LEFT, COLOR_ORANGE);	// timer
	g_pText.dynamic(ID_FONT_20, 2, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sPressKey);	// Instantaneous key event
	g_pText.dynamic(ID_FONT_20, 3, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sPressKey2);	// Every frame key event
	g_pText.dynamic(ID_FONT_20, 4, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sPressMouse);	// Instantaneous mouse event
	g_pText.dynamic(ID_FONT_20, 5, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sPressMouse2);	// Every frame mouse event
	g_pText.dynamic(ID_FONT_20, 6, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sPosMouse);	// Instantaneous mouse position
	g_pText.dynamic(ID_FONT_20, 7, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sPosMouse2);	// Every frame mouse position
	g_pText.dynamic(ID_FONT_20, 8, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sEventAppl);	// Last application event
	g_pText.dynamic(ID_FONT_20, 9, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sEventTimer1);	// Number of timer1 event
	g_pText.dynamic(ID_FONT_20, 10, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sEventTimer2);// Number of timer2 event

	// Render
	g_pAppl.clearScreen(g_cClear);
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pAppl.setRenderTexts();
	g_pText.draw(0, 11);	// Render fps, timer, key, mouse, events
}