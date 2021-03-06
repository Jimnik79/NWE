//--------------------------------------------------------------------------------------
// File: 2 Text.cpp
// Description: Sample of text.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with:
// - Windows fonts;
// - Unicode texts (regular/usual, with shadow, with stroke) with supporting alignment (left, center, right) and dynamic changes;
// - setting callback functions to creating initializing.

// Setting the text element occurs in 4 stages:
// 1. Adding the font to initializing. Returns the ID of the font, starting with 0.
// 2. Adding the text element with depth when creating device. Returns the ID of the text element, starting with 0.
// 3. Setting the display position of the text element when screen resized.
// 4. Setting text elements to display after setRenderSprite() and setRenderSpriteText() when rendering frame.

// Setting the FPS and TIMER text element occurs in 4 stages:
// 1. Adding the font to initializing. Returns the ID of the font, starting with 0.
// 2. Adding space for FPS and TIMER text element with depth when creating device. Returns the ID of the text element, starting with 0.
// 3. Setting the display position of FPS and TIMER text element when screen resized.
// 4. Setting FPS and TIMER text elements to display first when rendering frame.

// NOTE:
// - When memory allocation for the text, textures size made by powers of two.
// - Don't forget to add extra spaces in the formatted text with hieroglyphs.
// - Don't forget to call setRenderSprite() and setRenderSpriteText() before rendering the text.
// - Depth of text elements > 0.0f and < 1.0f, rendered less. For overlapping text elements, text elements with lower depth should be rendered after text elements with higher depth. Behind it you will need to keep an eye on their own.
// - The text with stroke contains the single degree of transparency, which is taken from the alpha channel of the main color.
// - When using Unicode text save this file as Unicode: FILE -> Advanced Save Options... -> Unicode - Codepage 1200.


#include "..\Engine\NetWars.h"	// Engine

#include <stdio.h>	// swprintf_s


CEngine	g_pAppl;	// Base Engine class
CText	g_pText;	// Text class

float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background
int		g_iTime = -1;	// Time counter
int		g_iTexts = 0;	// Number of text elements

#define	ID_FONT_20		0
#define	ID_FONT_16		1
#define	ID_FONT_30		2
#define	ID_FONT_18		3
#define	ID_FONT_24		4
#define	ID_FONT_28		5


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	g_pAppl.setCallbackInitialize();		// Setting callback functions to initializing
	g_pAppl.setCallbackDeviceCreated();		// Callback functions to creating device
	g_pAppl.setCallbackSwapChainResized();	// Callback functions to changing screen size
	g_pAppl.setCallbackFrameRender();		// Callback functions to rendering frame

	g_pText.setHModule(g_pAppl.getHModule());	// Connection the hModule of DLL to an instance of other classes for dynamic connection DLL

	return g_pAppl.run(L"2 Text", 1024, 600);	// Run Engine
}


//--------------------------------------------------------------------------------------
// INITIALIZING
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	// Adding fonts
	g_pAppl.addFont(L"Arial", 20, FW_MEDIUM);	// Return ID_FONT_20 = 0 - Add font for text
	g_pAppl.addFont(L"Times New Roman", 16, FW_MEDIUM);	// Return ID_FONT_16 = 1 - Add font for text
	g_pAppl.addFontEx(30, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Calibri");	// Return ID_FONT_30 = 2
	g_pAppl.addFontEx(18, 0, 0, 0, FW_NORMAL, true, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");		// Return ID_FONT_18 = 3
	g_pAppl.addFontEx(24, 0, 0, 0, FW_NORMAL, false, true, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Courier New");// Return ID_FONT_24 = 4
	g_pAppl.addFontEx(28, 0, 0, 0, FW_NORMAL, false, false, true, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Verdana");	// Return ID_FONT_28 = 5

	// To use if you need to change the font during the program.
	// After changing the font, you must restart the Direct3D device.
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_iTime = -1;	// For changing current time in the first frame after restart the Direct3D device

	// Adding text elements
	g_pText.addSpace(ID_FONT_30, TEXT_NORMAL, 100, DT_LEFT, 0.1f);	// Return 0 - add space for FPS
	g_pText.addSpace(ID_FONT_30, TEXT_SHADOW, 100, DT_LEFT, 0.1f);	// Return 1 - add space for timer
	g_pText.addSpace(ID_FONT_30, TEXT_STROKE, 100, DT_LEFT, 0.1f);	// Return 2 - current time after run application
	g_pText.add(ID_FONT_16, L"Hello World!", DT_LEFT, COLOR_GOLD, 0.9f);	// Return 3 - Add static text with ARGB color
	g_pText.add(ID_FONT_16, L"Привет Мир!", DT_LEFT, COLOR_GOLD, 0.9f);	// Return 4 - Russian text
	g_pText.add(ID_FONT_16, L"Hallo Welt!", DT_LEFT, COLOR_GOLD, 0.9f);	// Return 5 - German text
	g_pText.add(ID_FONT_16, L"你好世界！", DT_CENTER, COLOR_GOLD, 0.9f);		// Return 6 - Chinese text
	g_pText.add(ID_FONT_16, L"!مرحبا العالم", DT_RIGHT, COLOR_GOLD, 0.9f);	// Return 7 - Arabic text
	g_pText.add(ID_FONT_30, L"Click Alt+Enter to FullScreen mode!", DT_CENTER, COLOR_BLUE, 0.9f);	// Return 8
	g_pText.addShadow(ID_FONT_30, L"Translucent text with shadow (overlapped).", DT_CENTER, 0x800080ff, 0.8f);	// Return 9 - Add translucent text with shadow
	g_pText.addShadow(ID_FONT_30, L"Text with shadow", DT_CENTER, COLOR_BLUE, 0.8f);	// Return 10 - Add text with shadow
	g_pText.addStroke(ID_FONT_30, L"Translucent text with stroke (overlapped).", DT_CENTER, 0x800080ff, COLOR_BLACK, 0.8f);	// Return 11 - Add translucent text with stroke
	g_pText.addStroke(ID_FONT_30, L"Text with stroke.", DT_CENTER, COLOR_ORANGE, COLOR_BLACK, 0.8f);	// Return 12 - Add text with stroke
	g_pText.addEx(ID_FONT_20, L"English formatted text, shifted to left. This is a demonstration of a multi-formatted text with the specified width, indented, formatted and spaced line.", DT_LEFT, COLOR_GOLD, 0.7f, 350, 1, 30);	// Return 13 - English formatted text
	g_pText.addEx(ID_FONT_20, L"Русский форматированный текст, выровненный по центру. Демонстрация многостраничного форматированного текста с заданными шириной, отступом, форматом и междустрочным интервалом.", DT_CENTER, COLOR_GOLD, 0.7f, 350, 1, 30);	// Return 14 - Russian formatted text
	g_pText.addEx(ID_FONT_20, L"Deutsch formatierten Text, nach rechts verschoben. Dies ist eine Demonstration eines mehrformatierten Text mit der angegebenen Breite eingerückt, formatiert und beabstandet Linie.", DT_RIGHT, COLOR_GOLD, 0.7f, 350, 1, 30);	// Return 15 - German formatted text
	g_pText.addEx(ID_FONT_20, L"中国的格式化文本， 在中心对齐。 这是一个多格 式文本的示范用指定的宽度， 缩进， 格式化和间 隔线。", DT_CENTER, COLOR_GOLD, 0.7f, 350, 1, 30);	// Return 16 - Chinese formatted text
	g_pText.addEx(ID_FONT_20, L"نص منسق ،العربية المنحازة في .المركز هذا هو مظاهرة من نص منسق متعددة مع محدد ،عرض مسافة ،البادئة تنسيق وتباعد .خط", DT_CENTER, COLOR_GOLD, 0.7f, -350, 1, 30);	// Return 17 - Arabic formatted text
	g_pText.addStroke(ID_FONT_30, L"Rotating text", DT_CENTER, COLOR_ORANGE, COLOR_BLACK, 0.5f);	// Return 18 - Rotating text
	g_pText.addStroke(ID_FONT_30, L"Changing text", DT_CENTER, COLOR_ORANGE, COLOR_BLACK, 0.5f);	// Return 19 - Changing text
	g_pText.addSpace(ID_FONT_30, TEXT_STROKE, 120, DT_LEFT, 0.5f);	// Return 20 - Alpha text
	g_pText.add(ID_FONT_18, L"Italic text", DT_LEFT, COLOR_GOLD, 0.4f);	// Return 21 - Italic text
	g_pText.addStroke(ID_FONT_24, L"UnderLine text", DT_LEFT, COLOR_ORANGE, COLOR_BLACK, 0.4f);	// Return 22 - UnderLine text
	g_iTexts = g_pText.addStroke(ID_FONT_28, L"StrikeOut text", DT_LEFT, COLOR_ORANGE, COLOR_BLACK, 0.4f) + 1;	// Return 23 - StrikeOut text
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int w, int h)
{
	g_pText.pos(0, 5, 2);		// fps
	g_pText.pos(1, 105, 2);		// timer
	g_pText.pos(2, 205, 2);		// Current time after run application
	g_pText.pos(3, 100, 100);	// Hello World!
	g_pText.pos(4, 100, 120);	// Привет Мир!
	g_pText.pos(5, 100, 140);	// Hallo Welt!
	g_pText.pos(6, 100, 160);	// 你好世界！
	g_pText.pos(7, 100, 180);	// !مرحبا العالم
	g_pText.pos(8, 220, 200);	// Click Alt+Enter to FullScreen mode!
	g_pText.pos(9, 280, 123);	// Hello World 2!
	g_pText.pos(10, w - 200, h - 50);	// Hello World 3!
	g_pText.pos(11, 280, 163);	// Hello World 0123456789!
	g_pText.pos(12, 220, h - 50);	// Hello World 0123456789!
	g_pText.pos(13, 400, 10);	// English formatted text
	g_pText.pos(14, 400, 100);	// Russian formatted text
	g_pText.pos(15, 400, 210);	// German formatted text
	g_pText.pos(16, 400, 320);	// Chinese formatted text
	g_pText.pos(17, 400, 390);	// Arabic formatted text
	g_pText.pos(18, 200, 400);	// Rotating text
	g_pText.pos(19, 400, 400);	// Changing text
	g_pText.pos(20, 650, 400);	// Alpha text
	g_pText.pos(21, 100, 230);	// Italic text
	g_pText.pos(22, 100, 250);	// UnderLine text
	g_pText.pos(23, 100, 275);	// StrikeOut text
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double fTime, float)
{
	// Change sprites of text
	float f = float(fTime) / 2.0f;
	g_pText.posA(18, 200, 400, f);	// Rotating text rotates clockwise
	f = f - floorf(f) + 0.75f;
	if (f > 1.25f)
		f = 2.5f - f;
	float sx = g_pText.getSizeX(19), sy = g_pText.getSizeY(19);	// Size of changing text
	g_pText.posEx(19, 200.0f+f*200.0f, 400.0f + f*sy, 300.0f+f*100.0f, 350.0f+f*50.0f, 400.0f + f*sx, 320.0f + f*80.0f, 370.0f + f*30.0f + f*sx, 380.0f + f*20.0f + f*sy);	// Changing text
	g_pText.updateBuffer();

	// Update dynamic texts
	g_pText.fps(ID_FONT_30, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT_30, 1, DT_LEFT, COLOR_ORANGE);	// timer

	if (g_iTime != (int)fTime)
	{
		wchar_t	sTime[12];
		g_iTime = (int)fTime;
		swprintf_s(sTime, 12, L"%02i:%02i:%02i", g_iTime / 3600, (g_iTime / 60) % 60, g_iTime % 60);
		g_pText.dynamic(ID_FONT_30, 2, TEXT_STROKE, DT_LEFT, COLOR_ORANGE, sTime);	// Current time
	}
	BYTE i = BYTE(255.0f*(fTime - (int)fTime));
	g_pText.dynamic(ID_FONT_30, 20, TEXT_STROKE, DT_LEFT, (i << 24) + ((255 - i) << 8) + i, L"Alpha text");	// Alpha text

	// Render
	g_pAppl.clearScreen(g_cClear);
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pAppl.setRenderTexts();
	g_pText.draw(0, g_iTexts);	// Rendering texts
}