//--------------------------------------------------------------------------------------
// File: 8 2D Editor.cpp
// Description: Sample of work with 2D editing.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with 3D editing:
// - opening and saving texture: DDS (recomended), JPG, PNG, BMP, TIFF, GIF, WMP (last 4 not recomended);
// - resizing/scaling the texture(it is recommended that the width and height to power of two, even better if the width is equal to the height);
// - zooming (Ctrl+, Ctrl-);
// - choice of colors in the color palette;
// - drawing and filling;
// - adding text with the specified font and dimension;
// - selecting, cut (Ctrl+X), copy (Ctrl+Y), paste (Ctrl+V) and change (Ctrl+B);
// - Restoring the previous map of texture (Ctrl+Z).


// NOTE:
// - The size of the texture should not be more than 8192 in width or height.
// - The source code for get the list of fonts, registered in Windows, can use before initializing fonts to check the font availability and for choose fonts.
// - Textures, that are not multiples of 2 degrees, displayed blurring, so the process using right texture, and at the end save to the need texture.
// - Do not forget to try option 1 and option 2 of copy to clipboard ("2D Editor.h", function copy())
// - Due to the fact that different 2D graphics editors work differently with clipboard in terms of transparency, even from a single vendor, such as Paint in incorrect copying MVS2013,
//       copying between this program and Visual Studio 2013 - you can include transparency, to copy from Paint or Photoshop - you need to turn off transparency.


#include "..\Engine\NetWars.h"	// Engine

#include "2D Editor.h"	// Declaring defines, variables and constants. Auxiliary functions.

#include <stdio.h>	// swprintf_s, swscanf_s


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	g_pAppl.setCallbackInitialize();		// Set callback for Initialize
	g_pAppl.setCallbackDeinitialize();		// Set callback for Deinitialize
	g_pAppl.setCallbackKeyboard();			// Set callback for Keyboard event
	g_pAppl.setCallbackMouse();				// Set callback for Mouse event
	g_pAppl.setCallbackGUI();				// Set callback for GUI event
	g_pAppl.setCallbackDeviceCreated();		// Callback functions to creating device
	g_pAppl.setCallbackSwapChainResized();	// Callback functions to changing screen size
	g_pAppl.setCallbackFrameRender();		// Callback functions to rendering frame

	g_pTex.setHModule(g_pAppl.getHModule());	// Connection the hModule of DLL to an instance of other classes for dynamic connection DLL
	g_p2D.setHModule(g_pAppl.getHModule());		// Connection the hModule of DLL to an instance of other classes for dynamic connection DLL
	g_pText.setHModule(g_pAppl.getHModule());	// Connection the hModule of DLL to an instance of other classes for dynamic connection DLL
	g_pUI.setHModule(g_pAppl.getHModule());		// Connection the hModule of DLL to an instance of other classes for dynamic connection DLL

	return g_pAppl.run(L"8 2D Editor", 1024, 600);	// Run Engine
}


//--------------------------------------------------------------------------------------
// INITIALIZING
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	// Adding fonts
	g_pAppl.addFont(L"Arial", 16, FW_MEDIUM);	// Return ID_FONT_16 = 0, adding font for GUI and texts

	// Adding UI controls
	g_pUI.addEditBox(-1, ID_FONT_16, L"0", 35, 0.5f);	// Red color degree, return ID_EDITBOX_R = 0
	g_pUI.setInt(ID_EDITBOX_R, 255);
	g_pUI.addEditBox(-1, ID_FONT_16, L"0", 35, 0.5f);	// Green color degree, return ID_EDITBOX_G = 1
	g_pUI.setInt(ID_EDITBOX_G, 255);
	g_pUI.addEditBox(-1, ID_FONT_16, L"0", 35, 0.5f);	// Blue color degree, return ID_EDITBOX_B = 2
	g_pUI.setInt(ID_EDITBOX_B, 255);
	g_pUI.addEditBox(-1, ID_FONT_16, L"255", 35, 0.5f);	// Alpha degree, rlpha degree, return ID_EDITBOX_A = 3
	g_pUI.setInt(ID_EDITBOX_A, 255);
	g_pUI.addEditBox(-1, ID_FONT_16, L"512", 42, 0.5f);	// Texture widht, return ID_EDITBOX_W = 4
	g_pUI.setInt(ID_EDITBOX_W, 8192);
	g_pUI.addEditBox(-1, ID_FONT_16, L"512", 42, 0.5f);	// Texture height, return ID_EDITBOX_H = 5
	g_pUI.setInt(ID_EDITBOX_H, 8192);
	g_pUI.addEditBox(-1, ID_FONT_16, L"30", 35, 0.5f);	// Font height, return ID_EDITBOX_FH = 6
	g_pUI.setInt(ID_EDITBOX_FH, 254);					// 256 with stroke
	g_pUI.addEditBox(-1, ID_FONT_16, L"Text", 250, 0.5f);	// Text for insert into the texture, return ID_EDITBOX_TEXT = 7
	// It can be set a limit on the length of the text
	g_pUI.addButton(-1, ID_FONT_16, L"Create", 100, 1, 0.5f);	// Create new texture, return ID_BUTTON_CREATE = 8
	g_pUI.addButton(-1, ID_FONT_16, L"Scale", 100, 1, 0.5f);	// Scale or crop the texture to the desired width and height, return ID_BUTTON_OPEN = 9
	g_pUI.addButton(-1, ID_FONT_16, L"Open", 100, 1, 0.5f);		// Open texture from the file, return ID_BUTTON_SAVE = 10
	g_pUI.addButton(-1, ID_FONT_16, L"Save As", 100, 1, 0.5f);	// Save texture to the file, return ID_BUTTON_PEN = 11
	g_pUI.addButton(-1, ID_FONT_16, L"Fill", 100, 1, 0.5f);		// Draw a filled, return ID_BUTTON_FILL = 12
	g_pUI.addButton(-1, ID_FONT_16, L"Pen", 100, 1, 0.5f);		// Draw pen, return ID_BUTTON_SCALE = 13
	g_pUI.addButton(-1, ID_FONT_16, L"Text", 100, 1, 0.5f);		// Insert text into the texture, return ID_BUTTON_TEXT = 14
	g_pUI.addButton(-1, ID_FONT_16, L"Select", 100, 1, 0.5f);	// Select a piece of texture, return ID_BUTTON_SELECT = 15
	g_pUI.addComboBox(-1, ID_FONT_16, 210, 0.5f);	// Fonts for text, return ID_COMBOBOX_FONT = 16
	g_pUI.addComboBox(-1, ID_FONT_16, 112, 0.5f);	// Type of scale or crop, return ID_COMBOBOX_TYPE = 17
	g_pUI.addItem(ID_COMBOBOX_TYPE, L"Ratably", 0);
	g_pUI.addItem(ID_COMBOBOX_TYPE, L"Up-left", 1);
	g_pUI.addItem(ID_COMBOBOX_TYPE, L"Up", 2);
	g_pUI.addItem(ID_COMBOBOX_TYPE, L"Up-right", 3);
	g_pUI.addItem(ID_COMBOBOX_TYPE, L"Left", 4);
	g_pUI.addItem(ID_COMBOBOX_TYPE, L"Center", 5);
	g_pUI.addItem(ID_COMBOBOX_TYPE, L"Right", 6);
	g_pUI.addItem(ID_COMBOBOX_TYPE, L"Down-left", 7);
	g_pUI.addItem(ID_COMBOBOX_TYPE, L"Down", 8);
	g_pUI.addItem(ID_COMBOBOX_TYPE, L"Down-right", 9);
	g_pUI.addCheckBox(-1, ID_FONT_16, L"Transparency", COLOR_BLACK, true, 0.5f);	// Transparency support in the clipboard, Return ID_CHECKBOX_ALPHA = 18

	// Get the list of fonts, which can be used
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetDC(hwnd);
	LOGFONT  lf;
	lf.lfFaceName[0] = '\0';
	lf.lfCharSet = DEFAULT_CHARSET;	// All fonts
	EnumFontFamiliesEx(hdc, &lf, (FONTENUMPROC)EnumFontFamExProc, (LPARAM)0, (DWORD)0);
	ReleaseDC(hwnd, hdc);
}


//--------------------------------------------------------------------------------------
// DEINITIALIZING
//--------------------------------------------------------------------------------------
void CALLBACK onDeinitialize()
{
	SAFE_DELETE_ARRAY(g_mapBorder);
	SAFE_DELETE_ARRAY(g_mapPalette);
	SAFE_DELETE_ARRAY(g_mapWork);
	SAFE_DELETE_ARRAY(g_mapLast);
}


//--------------------------------------------------------------------------------------
// KEYBOARD EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onKeyboard(UINT idKey, bool bKeyDown)
{
	if (bKeyDown)
	{
		switch (idKey) {
		case VK_LEFT:		// 0x25 LEFT ARROW key
		case VK_NUMPAD4:	// 0x64 Numeric keypad 4 key
			if (g_fWV < 1.0f && g_fWVbeg > 0.0f)
			{
				g_x -= int(16.0f / g_fZoom);
				updateTexture();
			}
			break;

		case VK_UP:			// 0x26 UP ARROW key
		case VK_NUMPAD8:	// 0x68 Numeric keypad 8 key
			if (g_fHV < 1.0f && g_fHVbeg > 0.0f)
			{
				g_y -= int(16.0f / g_fZoom);
				updateTexture();
			}
			break;

		case VK_RIGHT:		// 0x27 RIGHT ARROW key
		case VK_NUMPAD6:	// 0x66 Numeric keypad 6 key
			if (g_fWV < 1.0f && g_fWVbeg + g_fWV < 1.0f)
			{
				g_x += int(16.0f / g_fZoom);
				updateTexture();
			}
			break;

		case VK_DOWN:		// 0x28 DOWN ARROW key
		case VK_NUMPAD2:	// 0x62 Numeric keypad 2 key
			if (g_fHV < 1.0f && g_fHVbeg + g_fHV < 1.0f)
			{
				g_y += int(16.0f / g_fZoom);
				updateTexture();
			}
			break;
		}

		if (GetKeyState(VK_CONTROL) < 0)	// Control + ...
		{
			switch (idKey) {
			case 0x58:	// 0x58 X key
				cut();
				break;
			case 0x43:	// 0x43 C key
				copy();
				break;
			case 0x56:	// 0x56 V key
				paste();
				break;
			case 0x42:	// 0x42 B key	// Not standart function
				change();
				break;

			case 0x5A:	// 0x5A Z key
				restoreMap();
				break;

			case VK_INSERT:	// 0x2D INS key
				copy();
				break;
			case VK_DELETE:	// 0x2E DEL key
				cut();
				break;

			case VK_ADD:		// 0x6B Add key (Num+)
			case VK_OEM_PLUS:	// 0xBB For any country / region, the '+' key
				g_fZoom *= 2.0f;
				if (g_fZoom > 16.0f)
					g_fZoom = 16.0f;
				else
				{
					g_x <<= 1;
					g_y <<= 1;
				}
				updateBorder();	// Update border, background and work texture in the sprite buffer
				if (g_bSelect || g_bCreateSelect)	// If there is selecting element
					updateSelect();	// Update selecting element in the sprite buffer
				break;

			case VK_SUBTRACT:	// 0x6D Subtract key (Num-)
			case VK_OEM_MINUS:	// 0xBD For any country / region, the '-' key
				g_fZoom /= 2.0f;
				if (g_fZoom < 0.0625f)
					g_fZoom = 0.0625f;
				else
				{
					g_x >>= 1;
					g_y >>= 1;
					if (int(g_iWidth*g_fZoom - g_fRight + g_fLeft) < g_x)
						g_x = int(g_iWidth*g_fZoom - g_fRight + g_fLeft) > 0 ? int(g_iWidth*g_fZoom - g_fRight + g_fLeft) : 0;
					if (int(g_iHeight*g_fZoom) - g_fDown + g_fUp < g_y)
						g_y = int(g_iHeight*g_fZoom - g_fDown + g_fUp) > 0 ? int(g_iHeight*g_fZoom - g_fDown + g_fUp) : 0;
				}
				updateBorder();	// Update border, background and work texture in the sprite buffer
				if (g_bSelect || g_bCreateSelect)	// If there is selecting element
					updateSelect();	// Update selecting element in the sprite buffer
				break;
			}
		}

		if (GetKeyState(VK_SHIFT) < 0)	// Shift + ...
		{
			switch (idKey) {
			case VK_INSERT:	// 0x2D INS key
				paste();
				break;
			case VK_DELETE:	// 0x2E DEL key	// Not standart function
				change();
				break;
			}
		}
	}
}


//--------------------------------------------------------------------------------------
// MOUSE EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onMouse(bool bLeftButtonDown, bool bRightButtonDown, bool, bool, bool, int, int xPos, int yPos)
{
	if (bLeftButtonDown)	// Click on the color palette
	{
		if (xPos > -1 && xPos < 256 && yPos > 74 && yPos < 331)	// Click on the color palette
		{
			g_iR = BYTE(xPos);
			g_iG = BYTE(yPos - 75);
			wchar_t	s[4];
			swprintf_s(s, 4, L"%i", g_iR);
			g_pUI.text(ID_EDITBOX_R, s);
			swprintf_s(s, 4, L"%i", g_iG);
			g_pUI.text(ID_EDITBOX_G, s);
			changePalette();
		}

		if (xPos >= g_iLeft && xPos < g_iRight && yPos >= g_iUp && yPos < g_iDown)	// Click on the texture
		{
			int x = int((xPos - g_iLeft) / g_fZoom);
			int y = int((yPos - g_iUp) / g_fZoom);

			switch (g_eState) {
			case PEN:	// Drawing pen
				if (!g_bLMouse)	// If left mouse button pressed just happened,
					rememberMap();	// Remember map of the previous state

				g_mapWork[g_iWidth * (y + g_y) + x + g_x] = g_color;
				if (g_bLMouse)	// If continuous drawing
				{
					g_bWorkSpace = true;	// Flag of mouse clicking on the work texture
					if (g_xLast == x)
					{
						if(g_yLast < y)
							for (int j = g_yLast + 1; j < y; j++)
								g_mapWork[g_iWidth * (j + g_y) + x + g_x] = g_color;
						else
							for (int j = y + 1; j < g_yLast; j++)
								g_mapWork[g_iWidth * (j + g_y) + x + g_x] = g_color;
					}
					else if (g_yLast == y)
					{
						if (g_xLast < x)
							for (int i = g_xLast + 1; i < x; i++)
								g_mapWork[g_iWidth * (y + g_y) + i + g_x] = g_color;
						else
							for (int i = x + 1; i < g_xLast; i++)
								g_mapWork[g_iWidth * (y + g_y) + i + g_x] = g_color;
					}
					else // if (g_xLast != x && g_yLast != y)
					{
						int sum = 5 * (mod(x - g_xLast) + mod(y - g_yLast));
						float dx = float(x - g_xLast) / sum;
						float dy = float(y - g_yLast) / sum;
						float fx = float(g_xLast) + 0.5f;
						float fy = float(g_yLast) + 0.5f;
						int ix = x;
						int iy = y;
						for (int i = 0; i < sum; i++)
							if (int(i*dx + fx) != ix || int(i*dy + fy) != iy)
							{
								ix = int(i*dx + fx);
								iy = int(i*dy + fy);
								g_mapWork[g_iWidth * (iy + g_y) + ix + g_x] = g_color;
							}
					}
				}
				g_pTex.setMap(ID_TEXTURE_WORK, g_mapWork);
				g_xLast = x;
				g_yLast = y;
				break;

			case FILL:	// Drawing filled
				fill(x + g_x, y + g_y);
				g_pTex.setMap(ID_TEXTURE_WORK, g_mapWork);
				break;

			case TEXT:	// Drawing Text
				if (!g_bLMouse)
				{
					rememberMap();	// Remember map of the previous state
					UINT fontHeight;
					swscanf_s(g_pUI.getText(ID_EDITBOX_FH), L"%d", &fontHeight);
					g_pTex.addText(ID_TEXTURE_WORK, x + g_x, y + g_y, g_pUI.getText(ID_EDITBOX_TEXT), g_pUI.getSelText(ID_COMBOBOX_FONT), fontHeight, 500, TEXT_NORMAL, g_color);	// TEXT_SHADOW
					g_pTex.getMap(ID_TEXTURE_WORK, g_mapWork);
				}
				break;

			case SELECT:	// Select a piece of texture
				if (g_bSelect)	// If inside of the selected area, then move the selected area
				{
					x = xPos - g_iLeft;
					y = yPos - g_iUp;
					setMinMax(&g_xBegT, &g_xEndT);
					setMinMax(&g_yBegT, &g_yEndT);
					if (g_bMoveSelect)
					{
						// Move the selected area (if it was)
						if (xPos >= g_iLeft && xPos < g_iRight && yPos >= g_iUp && yPos < g_iDown && (x != g_xMove || y != g_yMove) && 
							(x - g_xMove >= (int)g_fZoom || g_xMove - x >= (int)g_fZoom || y - g_yMove >= (int)g_fZoom || g_yMove - y >= (int)g_fZoom))
						{
							g_xBegT += int((x - g_xMove) / g_fZoom);
							g_xEndT += int((x - g_xMove) / g_fZoom);
							g_yBegT += int((y - g_yMove) / g_fZoom);
							g_yEndT += int((y - g_yMove) / g_fZoom);
							g_xMove += int(int((x - g_xMove) / g_fZoom) * g_fZoom);
							g_yMove += int(int((y - g_yMove) / g_fZoom) * g_fZoom);
							updateSelect();	// Update selecting element in the sprite buffer
						}
					}
					else if (x >= g_xBeg && x <= g_xEnd && y >= g_yBeg && y <= g_yEnd)	// Activate to move the selected block
					{
						g_xMove = x;
						g_yMove = y;
						g_bMoveSelect = true;
					}
					else
					{
						g_bSelect = false;
						g_bMoveSelect = false;
					}
				}

				if (!g_bSelect)	// If the area is not selected, to start selection
				{
					x = xPos - g_iLeft;
					y = yPos - g_iUp;
					if (!g_bLMouse)
					{
						// The coordinates (begin and end) to selection area of texture
						g_xBegT = g_xEndT = int((x + g_x) / g_fZoom);
						g_yBegT = g_yEndT = int((y + g_y) / g_fZoom);
						g_xLast = x;	// Use to visual the shear only more than 3 pixels
						g_yLast = y;
						updateSelect();	// Update selecting element in the sprite buffer
					}
					else
					{
						// The coordinates (only end) to selection area of texture
						g_xEndT = int((x + g_x) / g_fZoom);
						g_yEndT = int((y + g_y) / g_fZoom);
						if (x > g_xLast + 3 || x < g_xLast - 3 || y > g_yLast + 3 || y < g_yLast - 3)	// If changes along one axis is greater than 3 pixels
							g_bCreateSelect = true;	// 
						
						////// CAN ADD CHECK FOR NEED UPDATE
						updateSelect();	// Update selecting element in the sprite buffer
					}
				}
				break;
			}
			g_bLMouse = true;
		}
		else if (g_eState == PEN && g_bLMouse)	// Out of texture while drawing
		{
			int x = int((xPos - g_iLeft) / g_fZoom);
			int y = int((yPos - g_iUp) / g_fZoom);

			// Find the nearest border
			if (x < 0)
				x = 0;
			if (xPos >= g_iRight)
				x = int((g_iRight - g_iLeft) / g_fZoom) - 1;
			if (y < 0)
				y = 0;
			if (yPos >= g_iDown)
				y = int((g_iDown - g_iUp) / g_fZoom) - 1;

			if (g_bWorkSpace)
			{
				g_bWorkSpace = false;

				g_mapWork[g_iWidth * (y + g_y) + x + g_x] = g_color;

				if (g_xLast == x)
				{
					if (g_yLast < y)
						for (int j = g_yLast + 1; j < y; j++)
							g_mapWork[g_iWidth * (j + g_y) + x + g_x] = g_color;
					else
						for (int j = y + 1; j < g_yLast; j++)
							g_mapWork[g_iWidth * (j + g_y) + x + g_x] = g_color;
				}
				else if (g_yLast == y)
				{
					if (g_xLast < x)
						for (int i = g_xLast + 1; i < x; i++)
							g_mapWork[g_iWidth * (y + g_y) + i + g_x] = g_color;
					else
						for (int i = x + 1; i < g_xLast; i++)
							g_mapWork[g_iWidth * (y + g_y) + i + g_x] = g_color;
				}
				else // if (g_xLast != x && g_yLast != y)
				{
					int sum = 5 * (mod(x - g_xLast) + mod(y - g_yLast));
					float dx = float(x - g_xLast) / sum;
					float dy = float(y - g_yLast) / sum;
					float fx = float(g_xLast) + 0.5f;
					float fy = float(g_yLast) + 0.5f;
					int ix = x;
					int iy = y;
					for (int i = 0; i < sum; i++)
						if (int(i*dx + fx) != ix || int(i*dy + fy) != iy)
						{
							ix = int(i*dx + fx);
							iy = int(i*dy + fy);
							g_mapWork[g_iWidth * (iy + g_y) + ix + g_x] = g_color;
						}
				}
				g_pTex.setMap(ID_TEXTURE_WORK, g_mapWork);
			}
			g_xLast = x;
			g_yLast = y;
		}
	}
	else
	{
		g_bLMouse = false;
		g_bMoveSelect = false;
		if (g_eState == SELECT && g_bCreateSelect)	// If was created selection element
		{
			g_bCreateSelect = false;	// Creation completed
			g_bSelect = true;	// It created
		}
	}

	if (bRightButtonDown)	// Click on the color palette
	{
		g_bLMouse = false;
		
		int x = xPos - g_iLeft;
		int y = yPos - g_iUp;

		if (g_bMoveTexture)	// Moving texture
		{
			bool bUpdate = false;
			if (g_xMove - x >= int(ceil(g_fZoom)))	// Moving left
			{
				g_x += int((g_xMove - x) / g_fZoom);
				if (g_x > int(g_iWidth) - int(ceil((g_iRight - g_iLeft) / g_fZoom)))
					g_x = int(g_iWidth) - int(ceil((g_iRight - g_iLeft) / g_fZoom));
				bUpdate = true;
			}
			if (x - g_xMove >= int(ceil(g_fZoom)))	// Moving right
			{
				g_x += int((g_xMove - x) / g_fZoom);
				if (g_x < 0)
					g_x = 0;
				bUpdate = true;
			}
			if (y - g_yMove >= int(ceil(g_fZoom)))	// Moving up
			{
				g_y += int((g_yMove - y) / g_fZoom);
				if (g_y < 0)
					g_y = 0;
				bUpdate = true;
			}
			if (g_yMove - y >= int(ceil(g_fZoom)))	// Moving down
			{
				g_y += int((g_yMove - y) / g_fZoom);
				if (g_y > int(g_iHeight) - int(ceil((g_iDown - g_iUp) / g_fZoom)))
					g_y = int(g_iHeight) - int(ceil((g_iDown - g_iUp) / g_fZoom));
				bUpdate = true;
			}
			if (bUpdate)
			{
				if (g_bSelect)
					updateSelect();	// Update selecting element in the sprite buffer
				g_xMove = x;
				g_yMove = y;
				updateTexture();
			}
		}
		else if (!g_bMoveSelect && x >= 0 && x < g_iRight - g_iLeft && y >= 0 && y < g_iDown - g_iUp)
		{
			g_xMove = x;
			g_yMove = y;
			g_bMoveTexture = true;
		}
		else
			g_bMoveTexture = false;

		g_bRMouse = true;
	}
	else	// No moving texture
	{
		g_bRMouse = false;
		g_bMoveTexture = false;
	}
}


//--------------------------------------------------------------------------------------
// GUI EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onGUI(UINT, int idControl)
{
	switch (idControl)
	{
	case ID_BUTTON_CREATE:	// Create new texture, release current texture
		g_x = g_y = 0;
		g_eState = SELECT;
		swscanf_s(g_pUI.getText(ID_EDITBOX_W), L"%d", &g_iWidth);
		swscanf_s(g_pUI.getText(ID_EDITBOX_H), L"%d", &g_iHeight);
		g_pTex.updateDynamic(ID_TEXTURE_WORK, g_iWidth, g_iHeight);
		updateTextureMap();
		updateBorder();	// Update border, background and work texture in the sprite buffer
		break;

	case ID_BUTTON_SCALE:	// Scale or crop the texture to the desired width and height
		g_x = g_y = 0;
		g_eState = SELECT;
		swscanf_s(g_pUI.getText(ID_EDITBOX_W), L"%d", &g_iWidth);
		swscanf_s(g_pUI.getText(ID_EDITBOX_H), L"%d", &g_iHeight);
		g_pTex.scale(ID_TEXTURE_WORK, (byte)g_pUI.getSelIndex(ID_COMBOBOX_TYPE), g_iWidth, g_iHeight);
		updateTextureMap();
		updateBorder();	// Update border, background and work texture in the sprite buffer
		break;

	case ID_BUTTON_OPEN: {	// Open texture from the file
		g_eState = SELECT;
		BSTR sFileName = openFileDialog();
		if (sFileName[0])
		{
			g_x = g_y = 0;
			g_pTex.updateWithDynamic(ID_TEXTURE_WORK, sFileName);
			g_pTex.getSize(ID_TEXTURE_WORK, &g_iWidth, &g_iHeight);
			updateTextureMap();
			updateBorder();	// Update border, background and work texture in the sprite buffer
			wchar_t	s[8];
			swprintf_s(s, 8, L"%d", g_iWidth);
			g_pUI.text(ID_EDITBOX_W, s);
			swprintf_s(s, 8, L"%d", g_iHeight);
			g_pUI.text(ID_EDITBOX_H, s);
		}
	}	break;

	case ID_BUTTON_SAVE: {	// Save texture to the file
		g_eState = SELECT;
		wchar_t sFileName[MAX_PATH] = L"";
		D3DX11_IMAGE_FILE_FORMAT format = saveFileDialog(sFileName);
		if (sFileName[0])
			g_pTex.saveToFile(ID_TEXTURE_WORK, sFileName, format);	// Save texture to file
	}	break;

	case ID_BUTTON_PEN:		g_eState = PEN;		break;	// Drawing pen
	case ID_BUTTON_FILL:	g_eState = FILL;	break;	// Filling
	case ID_BUTTON_TEXT:	g_eState = TEXT;	break;	// Drawing text
	case ID_BUTTON_SELECT:	g_eState = SELECT;	break;	// Selection area

	case ID_EDITBOX_R: {	// EVENT_EDIT & EVENT_ENTER
		int i;
		swscanf_s(g_pUI.getText(ID_EDITBOX_R), L"%i", &i);
		g_iR = (BYTE)i;
	}	break;
	case ID_EDITBOX_G: {	// EVENT_EDIT & EVENT_ENTER
		int i;
		swscanf_s(g_pUI.getText(ID_EDITBOX_G), L"%i", &i);
		g_iG = (BYTE)i;
	}	break;
	case ID_EDITBOX_B: {	// EVENT_EDIT & EVENT_ENTER
		int i;
		swscanf_s(g_pUI.getText(ID_EDITBOX_B), L"%i", &i);
		g_iB = (BYTE)i;
		changePalette();
	}	break;
	case ID_EDITBOX_A: {	// EVENT_EDIT & EVENT_ENTER
		int i;
		swscanf_s(g_pUI.getText(ID_EDITBOX_A), L"%i", &i);
		g_iA = (BYTE)i;
		changePalette();
	}	break;
	}
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.2f);	// Return 0, add space for fps
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.2f);	// Return 1, add space for timer

	//g_pText.addShadow(ID_FONT_16, L"Ctrl-, Ctrl+ for Zoom = 1.0", DT_LEFT, COLOR_ORANGE, 0.5f);	// Zoom, return 2
	g_pText.addSpace(ID_FONT_16, TEXT_SHADOW, 250, DT_LEFT, 0.5f);	// Zoom, return 2
	g_pText.addShadow(ID_FONT_16, L"CtrlX,C,V,B for Clipboard", DT_LEFT, COLOR_ORANGE, 0.5f);	// Scroll, return 3
	g_pText.addShadow(ID_FONT_16, L"↑↓→← for scroll, CtrlZ for restore", DT_LEFT, COLOR_ORANGE, 0.5f);	// Move, return 4
	g_pText.addShadow(ID_FONT_16, L"R:", DT_RIGHT, COLOR_ORANGE, 0.5f);	// Red color, return 5
	g_pText.addShadow(ID_FONT_16, L"G:", DT_RIGHT, COLOR_ORANGE, 0.5f);	// Green color, return 6
	g_pText.addShadow(ID_FONT_16, L"B:", DT_RIGHT, COLOR_ORANGE, 0.5f);	// Blue color, return 7
	g_pText.addShadow(ID_FONT_16, L"A:", DT_RIGHT, COLOR_ORANGE, 0.5f);	// Alpha, return 8
	g_pText.addShadow(ID_FONT_16, L"W:", DT_RIGHT, COLOR_ORANGE, 0.5f);	// Width, return 9
	g_pText.addShadow(ID_FONT_16, L"H:", DT_RIGHT, COLOR_ORANGE, 0.5f);	// Height, return 10
	
	// Background, similar Photoshop background
	g_pTex.addDynamic(16, 16);	// Return ID_TEXTURE_BACK = 0
	SAFE_DELETE_ARRAY(g_mapBack);
	g_mapBack = new UINT[256];	// 16 * 16
	for (int i = 0; i < 256; i++)
		g_mapBack[i] = (i % 16 < 8 == i / 16 < 8) ? COLOR_WHITE : COLOR_GREY;
	g_pTex.setMap(ID_TEXTURE_BACK, g_mapBack);	// Set texture map of background

	// Work texture
	g_pTex.addDynamic(g_iWidth, g_iHeight);	// Return ID_TEXTURE_WORK = 1
	updateTextureMap();

	// Border elements
	g_pTex.addDynamic(8, 8);	// Return ID_TEXTURE_BORDER = 2
	SAFE_DELETE_ARRAY(g_mapBorder);
	g_mapBorder = new UINT[64];	// 8 * 8
	for (int i = 0; i < 64; i++)
		g_mapBorder[i] = ((i % 8 && i>3) || i>31) ? COLOR_ALPHA : COLOR_BLACK;
	g_pTex.setMap(ID_TEXTURE_BORDER, g_mapBorder);

	// The color palette 
	g_pTex.addDynamic(256, 256);	// Return ID_TEXTURE_PALETTE = 3
	SAFE_DELETE_ARRAY(g_mapPalette);
	g_mapPalette = new UINT[65536];	// 256 * 256
	changePalette();
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int w, int h)
{
	g_fW = (float)w;
	g_fH = (float)h;

	// Texts
	g_pText.pos(0, 5, 2);		// FPS
	g_pText.pos(1, 105, 2);		// Time
	g_pText.pos(2, 3, 20);		// Zooom
	g_pText.pos(3, 3, 38);		// ClipBoard
	g_pText.pos(4, 3, 56);		// Scroll
	g_pText.pos(5, 23, 338);	// Red color
	g_pText.pos(6, 88, 338);	// Green color
	g_pText.pos(7, 153, 338);	// Blue color
	g_pText.pos(8, 218, 338);	// Blue color
	g_pText.pos(9, 23, 374);	// Width
	g_pText.pos(10, 88, 374);	// Height

	// GUI
	g_pUI.pos(ID_EDITBOX_R, 25, 332);		// Red color degree
	g_pUI.pos(ID_EDITBOX_G, 90, 332);		// Green color degree
	g_pUI.pos(ID_EDITBOX_B, 155, 332);		// Blue color degree
	g_pUI.pos(ID_EDITBOX_A, 220, 332);		// Alpha degree
	g_pUI.pos(ID_EDITBOX_W, 25, 368);		// Texture widht
	g_pUI.pos(ID_EDITBOX_H, 90, 368);		// Texture height
	g_pUI.pos(ID_EDITBOX_FH, 218, 400);		// Font height
	g_pUI.pos(ID_EDITBOX_TEXT, 3, 432);		// Text for insert into the texture
	g_pUI.pos(ID_BUTTON_CREATE, 18, 464);	// Create new texture
	g_pUI.pos(ID_BUTTON_SCALE, 136, 464);	// Scale or crop the texture to the desired width and height
	g_pUI.pos(ID_BUTTON_OPEN, 18, 496);		// Open texture from the file
	g_pUI.pos(ID_BUTTON_SAVE, 136, 496);	// Save texture to the file
	g_pUI.pos(ID_BUTTON_FILL, 18, 528);		// Draw a filled
	g_pUI.pos(ID_BUTTON_PEN, 136, 528);		// Draw pen
	g_pUI.pos(ID_BUTTON_TEXT, 18, 560);		// Insert text into the texture
	g_pUI.pos(ID_BUTTON_SELECT, 136, 560);	// Select a piece of texture
	g_pUI.pos(ID_COMBOBOX_FONT, 3, 400);	// Fonts for text
	g_pUI.pos(ID_COMBOBOX_TYPE, 144, 368);	// Type of scale
	g_pUI.pos(ID_CHECKBOX_ALPHA, 155, 38);	// Transparency support in the clipboard

	// Sprites
	calcBorder();
	g_p2D.addF(0.0f, (g_fRight - g_fLeft) / 16.0f, 0.0f, (g_fDown - g_fUp) / 16.0f, g_fLeft, g_fRight, g_fUp, g_fDown, 0.9f);	// Return 0, Background
	g_p2D.addF(0.0f, g_fWV, 0.0f, g_fHV, g_fLeft, g_fRight, g_fUp, g_fDown, 0.8f);	// Return 1, texture
	g_p2D.addF(0.25f, 0.25f, 0.06f, 0.06f, g_fLeft - 4.0f, g_fRight + 4.0f, g_fUp - 4.0f, g_fUp, 0.7f);	// Return 2, Border element, up
	g_p2D.addF(0.25f, 0.25f, 0.06f, 0.06f, g_fLeft - 4.0f, g_fRight + 4.0f, g_fDown, g_fDown + 4.0f, 0.7f);	// Return 3, Border element, down
	g_p2D.addF(0.06f, 0.06f, 0.25f, 0.25f, g_fLeft - 4.0f, g_fLeft, g_fUp, g_fDown, 0.7f);	// Return 4, Border element, left
	g_p2D.addF(0.06f, 0.06f, 0.25f, 0.25f, g_fRight, g_fRight + 4.0f, g_fUp, g_fDown, 0.7f);	// Return 5, Border element, right
	g_p2D.addF(0.0f, 1.0f, 0.0f, 1.0f, 100.0f, 108.0f, 100.0f, 108.0f, 0.7f);	// Return 6, Selection element, up
	g_p2D.addF(0.0f, 1.0f, 0.0f, 1.0f, 100.0f, 108.0f, 100.0f, 108.0f, 0.7f);	// Return 7, Selection element, down
	g_p2D.addF(0.0f, 1.0f, 0.0f, 1.0f, 100.0f, 108.0f, 100.0f, 108.0f, 0.7f);	// Return 8, Selection element, left
	g_p2D.addF(0.0f, 1.0f, 0.0f, 1.0f, 100.0f, 108.0f, 100.0f, 108.0f, 0.7f);	// Return 9, Selection element, right
	g_p2D.addF(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 256.0f, 75.0f, 331.0f, 0.8f);	// Return 10, the color palette
	if (g_bSelect || g_bCreateSelect)	// If there is selecting element
		updateSelect();	// Update selecting element in the sprite buffer
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double, float)
{
	// Update dynamic texts
	wchar_t s[64];
	swprintf_s(s, 64, L"Ctrl-, Ctrl+ for Zoom = %f", g_fZoom);
	g_pText.dynamic(ID_FONT_16, 2, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, s);
	g_pText.fps(ID_FONT_16, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT_16, 1, DT_LEFT, COLOR_ORANGE);	// timer

	// Start render, Background
	g_pAppl.clearScreen(g_cClear);	// If there is background then don't need to clear screen
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, ID_TEXTURE_BACK);
	g_p2D.draw(0, 1);	// Background

	// Work texture
	g_pTex.render(ID_TEXTURE_WORK);
	g_p2D.draw(1, 1);	// Work texture

	// texture border
	g_pTex.render(ID_TEXTURE_BORDER);
	g_p2D.draw(2, 4);	// Border
	if (g_bSelect || g_bCreateSelect)	// If there is selecting element
		g_p2D.draw(6, 4);	// Selecting element

	// The color palette
	g_pTex.render(ID_TEXTURE_PALETTE);
	g_p2D.draw(10, 1);	// The color palette

	// GUI
	g_pUI.render();

	// Render fps, timer and other texts
	g_pAppl.setRenderTexts();
	g_pText.draw(0, 11);	// Texts
}