//--------------------------------------------------------------------------------------
// File: 8 2D Editor.cpp
// Description: Declaring defines, variables and constants. Auxiliary functions..
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


CEngine	g_pAppl;	// Base Engine class
CTex	g_pTex;		// Texture class
C2D		g_p2D;		// 2D class
CText	g_pText;	// Text class
CUI		g_pUI;		// UI class

float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background

#define	ID_EDITBOX_R		0	// Red color degree
#define	ID_EDITBOX_G		1	// Green color degree
#define	ID_EDITBOX_B		2	// Blue color degree
#define	ID_EDITBOX_A		3	// Alpha degree
#define	ID_EDITBOX_W		4	// Texture widht
#define	ID_EDITBOX_H		5	// Texture height
#define	ID_EDITBOX_FH		6	// Font height
#define	ID_EDITBOX_TEXT		7	// Text for insert into the texture
#define	ID_BUTTON_CREATE	8	// Create new texture
#define	ID_BUTTON_SCALE		9	// Scale or crop the texture to the desired width and height
#define	ID_BUTTON_OPEN		10	// Open texture from the file
#define	ID_BUTTON_SAVE		11	// Save texture to the file
#define	ID_BUTTON_FILL		12	// Draw a filled
#define	ID_BUTTON_PEN		13	// Draw pen
#define	ID_BUTTON_TEXT		14	// Insert text into the texture
#define	ID_BUTTON_SELECT	15	// Select a piece of texture
#define	ID_COMBOBOX_FONT	16	// Fonts for text
#define	ID_COMBOBOX_TYPE	17	// Type of scale or crop
#define	ID_CHECKBOX_ALPHA	18	// Transparency support in the clipboard

#define	ID_FONT_16			0	// Work font: Arial, 16

#define	ID_TEXTURE_BACK		0	// Background, similar Photoshop background
#define	ID_TEXTURE_WORK		1	// Work texture
#define	ID_TEXTURE_BORDER	2	// Border and selection elements
#define	ID_TEXTURE_PALETTE	3	// The color palette


inline int mod(int x)	{ return x < 0 ? -x : x; }	// Module of number
inline void setMinMax(int* a, int* b) { if (*a > *b) { int c = *a; *a = *b; *b = c; } }	// Set minimum and maxumum

enum state	// State machine
{
	PEN,	// Draw pen
	FILL,	// Draw a filled
	TEXT,	// Insert text into the texture
	SELECT,	// Select a piece of texture
};
enum state g_eState = SELECT;	// State machine

UINT	g_iWidth = 512, g_iHeight = 512;	// Width and height of work texture
float	g_fZoom = 1.0f;	// Zoom
float	g_fW = 1024.0f, g_fH = 600.0f;	// Width and height of work winndow
float	g_fWV = 1.0f, g_fHV = 1.0f, g_fWVbeg = 0.0f, g_fHVbeg = 0.0f;	// The degree of texture visible (0.0f to 1.0f), 1.0f - fully visible
int		g_x = 0, g_y = 0;	// The initial positions of work texture in pixels
float	g_fLeft, g_fRight, g_fUp, g_fDown;	// Borders of work texture in work window, in pixels
int		g_iLeft, g_iRight, g_iUp, g_iDown;	// Borders of work texture in work window, in pixels
BYTE	g_iR = 0, g_iG = 0, g_iB = 0, g_iA = 255;	// RGBA color
UINT	g_color = 0xff000000;	// RGBA color

UINT*	g_mapLast = NULL;	// Texture map to memorize the last changes
UINT*	g_mapWork = NULL;	// A copy of the work texture map
UINT*	g_mapPalette = NULL;// Map of texture palette
UINT*	g_mapBorder = NULL;	// Texture map of border and select elements
UINT*	g_mapBack = NULL;	// Texture map of background

RECT	g_rcBuffer;			// Borders of texture taken to buffer
bool	g_bLMouse = false;	// Flag of pressing the left mouse button in the last mouse event
bool	g_bRMouse = false;	// Flag of pressing the left mouse button in the last mouse event
int		g_xLast, g_yLast;	// X and Y position of the mouse in the last mouse event while holding down the left mouse button
int		g_xMove, g_yMove;	// The last frame coordinates when moving

int		g_xBeg = 0, g_yBeg = 0, g_xEnd = 0, g_yEnd = 0;	// The coordinates of selection area (in window pixels)
int		g_xBegT = 0, g_yBegT = 0, g_xEndT = 0, g_yEndT = 0;	// The coordinates of selection area (in texture pixels)

bool	g_bCreateSelect = false;// Flag of creating borders for selection area
bool	g_bSelect = false;		// Flag of completing creating borders for selection area
bool	g_bMoveSelect = false;	// Flag of capture to move the selected area
bool	g_bMoveTexture = false;	// Flag of capture to move the texture entire
bool	g_bWorkSpace = false;	// Flag of mouse clicking on the work texture


//--------------------------------------------------------------------------------------
// GET A LIST OF FONTS
//--------------------------------------------------------------------------------------
int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX*, DWORD, LPARAM)
{
	if (!g_pUI.getSize(ID_COMBOBOX_FONT) || wcscmp(g_pUI.getIndexText(ID_COMBOBOX_FONT, g_pUI.getSize(ID_COMBOBOX_FONT) - 1), lpelfe->elfLogFont.lfFaceName))
		g_pUI.addItem(ID_COMBOBOX_FONT, lpelfe->elfLogFont.lfFaceName, 0);
	return true;
}


//--------------------------------------------------------------------------------------
// OPEN FILE DIALOG FOR TEXTURES
//--------------------------------------------------------------------------------------
BSTR openFileDialog()
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	wchar_t szBuffer[MAX_PATH];
	szBuffer[0] = NULL;
	static const wchar_t szFilter[] = L"Textures (.DDS, .BMP, .PNG, .JPG, .TIFF, .GIF, .WMP)\0*.DDS;*.BMP;*.PNG;*.JPG;*.TIFF;*.GIF;*.WMP\0" \
		L"All files (*.*)\0*.*\0\0";
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = g_pAppl.getHWND();
	ofn.hInstance = NULL;
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.lpstrFile = szBuffer;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = L"Title";
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = L"DefExt";
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;

	return GetOpenFileName(&ofn) ? SysAllocString(szBuffer) : SysAllocString(L"");	// If selected a file, then return its name, else return an empty string
}


//--------------------------------------------------------------------------------------
// SAVE FILE DIALOG FOR TEXTURES
//--------------------------------------------------------------------------------------
D3DX11_IMAGE_FILE_FORMAT saveFileDialog(wchar_t* szBuffer)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	static const wchar_t szFilter[] = L"D3D/DDS (*.DDS)\0*.dds\0" \
		L"PNG (*.PNG)\0*.png\0" \
		L"JPEG (*.JPG)\0*.jpg\0" \
		L"BMP (*.BMP)\0*.bmp\0" \
		L"TIFF (*.TIFF)\0*.tiff\0" \
		L"WMP (*.WMP)\0*.wmp\0\0";
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = g_pAppl.getHWND();
	ofn.hInstance = NULL;
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.lpstrFile = szBuffer;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = L"Title";
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = L"DefExt";
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;

	GetSaveFileName(&ofn);

	switch (ofn.nFilterIndex) {
	case 1: return D3DX11_IFF_DDS;
	case 2: return D3DX11_IFF_PNG;
	case 3: return D3DX11_IFF_JPG;
	case 4: return D3DX11_IFF_BMP;
	case 5: return D3DX11_IFF_TIFF;
	default: return D3DX11_IFF_WMP;
	}
}

//--------------------------------------------------------------------------------------
// REMEMBER TEXTURE MAP OF THE PREVIOUS STATE
//--------------------------------------------------------------------------------------
void rememberMap()
{
	memcpy(g_mapLast, g_mapWork, g_iWidth * g_iHeight << 2);	// Remember texture map
}


//--------------------------------------------------------------------------------------
// RESTORE THE PREVIOUS MAP
//--------------------------------------------------------------------------------------
void restoreMap()
{
	UINT* mapBuffer = new UINT[g_iWidth * g_iHeight];
	memcpy(mapBuffer, g_mapWork, g_iWidth * g_iHeight << 2);
	memcpy(g_mapWork, g_mapLast, g_iWidth * g_iHeight << 2);
	memcpy(g_mapLast, mapBuffer, g_iWidth * g_iHeight << 2);
	SAFE_DELETE_ARRAY(mapBuffer);
	g_pTex.setMap(ID_TEXTURE_WORK, g_mapWork);	// Set texture map to the engine
}


//--------------------------------------------------------------------------------------
// CHANGE PALETTE OF COLORS
//--------------------------------------------------------------------------------------
void changePalette()
{
	UINT color = (UINT(g_iA) << 24) + (UINT(g_iB) << 16);	// BA color for texture palette
	g_color = color + (UINT(g_iG) << 8) + g_iR;	// RGBA color
	for (int i = 0; i < 65536; i++)
		g_mapPalette[i] = color++;
	g_pTex.setMap(ID_TEXTURE_PALETTE, g_mapPalette);	// Set palette to the palette texture
}


//--------------------------------------------------------------------------------------
// FILLING, RETURN THE NUMBER OF FILLING PIXELS
//--------------------------------------------------------------------------------------
int fill(UINT x, UINT y)
{
	int p = g_iWidth * y + x;
	if (g_mapWork[p] == g_color)	// Note: exit, if that point is already filled to the fill color
		return 0;

	rememberMap();	// Remember texture map of the previous state

	UINT colorFill = g_mapWork[p];
	g_mapWork[p] = g_color;	// Paint over only fill color
	int* np = new int[g_iWidth * g_iHeight];	// New position
	int count = 0;	// Counter of filling pixels
	int i = 0;
	while (i >= -1)	// while (true)
	{
		if (y && (g_mapWork[p - g_iWidth] == colorFill))	// Check Up
		{
			np[count] = p - g_iWidth;
			g_mapWork[np[count++]] = g_color;	// Paint over only fill color
		}
		if ((x < g_iWidth - 1) && (g_mapWork[p + 1] == colorFill))	// Check Right
		{
			np[count] = p + 1;
			g_mapWork[np[count++]] = g_color;	// Paint over only fill color
		}
		if ((y < g_iHeight - 1) && (g_mapWork[p + g_iWidth] == colorFill))	// Check Down
		{
			np[count] = p + g_iWidth;
			g_mapWork[np[count++]] = g_color;	// Paint over only fill color
		}
		if (x && (g_mapWork[p - 1] == colorFill))	// Check Left
		{
			np[count] = p - 1;
			g_mapWork[np[count++]] = g_color;	// Paint over only fill color
		}
		if (i == count)	// If all points are passed
			break;
		p = np[i++];
		x = p % g_iWidth;
		y = p / g_iWidth;
	}
	delete[] np;

	return count;
}


//--------------------------------------------------------------------------------------
// CALCULATE BORDER OF WORK TEXTURE
//--------------------------------------------------------------------------------------
void calcBorder()
{
	g_iLeft = int(g_fW + 256.0f - g_iWidth * g_fZoom) / 2;
	if (g_iLeft < 260)
		g_iLeft = 260;
	g_fLeft = float(g_iLeft);
	g_iRight = g_iLeft + int(g_iWidth * g_fZoom);
	if (g_iRight > int(g_fW) - 4)
		g_iRight = int(g_fW) - 4;
	g_fRight = float(g_iRight);

	g_iUp = int(g_fH - g_iHeight * g_fZoom) / 2;
	if (g_iUp < 4)
		g_iUp = 4;
	g_fUp = float(g_iUp);
	g_iDown = g_iUp + int(g_iHeight * g_fZoom);
	if (g_iDown > int(g_fH) - 4)
		g_iDown = int(g_fH) - 4;
	g_fDown = float(g_iDown);

	g_fWV = (g_fW - 264.0f) / g_iWidth / g_fZoom;
	if (g_fWV > 1.0f)
		g_fWV = 1.0f;
	g_fHV = (g_fH - 8.0f) / g_iHeight / g_fZoom;
	if (g_fHV > 1.0f)
		g_fHV = 1.0f;
}


//--------------------------------------------------------------------------------------
// UPDATE WORK TEXTURE IN THE SPRITE BUFFER
//--------------------------------------------------------------------------------------
void updateTexture()
{
	// It works only if g_fWV < 1.0f or g_fHV < 1.0f
	g_fWVbeg = (float)g_x / g_iWidth;
	g_fHVbeg = (float)g_y / g_iHeight;
	if (g_fWVbeg > 1.0f - g_fWV)
		g_fWVbeg = 1.0f - g_fWV;
	if (g_fHVbeg > 1.0f - g_fHV)
		g_fHVbeg = 1.0f - g_fHV;
	g_p2D.updateF(1, g_fWVbeg, g_fWVbeg + g_fWV, g_fHVbeg, g_fHVbeg + g_fHV, g_fLeft, g_fRight, g_fUp, g_fDown, 0.8f);	// Texture
	g_p2D.updateBuffer();	// Update sprite buffer after all sprite updates
}


//--------------------------------------------------------------------------------------
// UPDATE BORDER, BACKGROUND AND WORK TEXTURE IN THE SPRITE BUFFER
//--------------------------------------------------------------------------------------
void updateBorder()
{
	calcBorder();	// Calculate border of work texture

	g_p2D.updateF(0, 0.0f, (g_fRight - g_fLeft) / 16.0f, 0.0f, (g_fDown - g_fUp) / 16.0f, g_fLeft, g_fRight, g_fUp, g_fDown, 0.9f);	// Background
	g_p2D.updateF(2, 0.25f, 0.25f, 0.06f, 0.06f, g_fLeft - 4.0f, g_fRight + 4.0f, g_fUp - 4.0f, g_fUp,          0.7f);	// Border, up
	g_p2D.updateF(3, 0.25f, 0.25f, 0.06f, 0.06f, g_fLeft - 4.0f, g_fRight + 4.0f, g_fDown,      g_fDown + 4.0f, 0.7f);	// Border, down
	g_p2D.updateF(4, 0.06f, 0.06f, 0.25f, 0.25f, g_fLeft - 4.0f, g_fLeft,         g_fUp,        g_fDown,        0.7f);	// Border, left
	g_p2D.updateF(5, 0.06f, 0.06f, 0.25f, 0.25f, g_fRight,       g_fRight + 4.0f, g_fUp,        g_fDown,        0.7f);	// Border, right

	updateTexture();	// Update work texture
}


//--------------------------------------------------------------------------------------
// UPDATE SELECTING ELEMENT IN THE SPRITE BUFFER
//--------------------------------------------------------------------------------------
void updateSelect()
{
	// Using the texture coordinates of selection area to calculate new window coordinates of selection area
	g_xBeg = int(g_xBegT * g_fZoom) - g_x;
	g_xEnd = int(g_xEndT * g_fZoom) - g_x;
	g_yBeg = int(g_yBegT * g_fZoom) - g_y;
	g_yEnd = int(g_yEndT * g_fZoom) - g_y;

	setMinMax(&g_xBeg, &g_xEnd);
	setMinMax(&g_yBeg, &g_yEnd);

	g_xEnd += int(ceil(g_fZoom)) - 1;
	g_yEnd += int(ceil(g_fZoom)) - 1;

	// Adjustment for zoom
	float xBeg = floorf(g_xBeg / g_fZoom) * g_fZoom + g_fLeft;
	float yBeg = floorf(g_yBeg / g_fZoom) * g_fZoom + g_fUp;
	float xEnd = ceilf(g_xEnd / g_fZoom) * g_fZoom + g_fLeft;
	float yEnd = ceilf(g_yEnd / g_fZoom) * g_fZoom + g_fUp;

	// Adjustment for border
	if (xBeg < g_fLeft)
		xBeg = g_fLeft;
	if (yBeg < g_fUp)
		yBeg = g_fUp;
	if (xEnd > g_fRight)
		xEnd = g_fRight;
	if (yEnd > g_fDown)
		yEnd = g_fDown;

	g_p2D.updateF(6, 0.0f, (xEnd - xBeg + 2.0f) / 8.0f, 0.0f, 0.125f, xBeg - 1.0f, xEnd + 1.0f, yBeg - 1.0f, yBeg, 0.6f);	// Selection element, up
	g_p2D.updateF(7, (xEnd - xBeg + 2.0f) / 8.0f, 0.0f, 0.0f, 0.125f, xBeg - 1.0f, xEnd + 1.0f, yEnd, yEnd + 1.0f, 0.6f);	// Selection element, down
	g_p2D.updateF(8, 0.0f, 0.125f, 0.125f, (yEnd - yBeg - 1.0f) / 8.0f, xBeg - 1.0f, xBeg, yBeg, yEnd, 0.6f);	// Selection element, left
	g_p2D.updateF(9, 0.0f, 0.125f, (yEnd - yBeg - 1.0f) / 8.0f, 0.125f, xEnd, xEnd + 1.0f, yBeg, yEnd, 0.6f);	// Selection element, right
	g_p2D.updateBuffer();	// Update sprite buffer after all sprite updates and before setRenderSprite()
}


//--------------------------------------------------------------------------------------
// COPY TO CLIPBOARD
//--------------------------------------------------------------------------------------
void copy()
{
	setMinMax(&g_xBegT, &g_xEndT);
	setMinMax(&g_yBegT, &g_yEndT);

	// Variant 1
	RECT rcSelect;
	SetRect(&rcSelect, g_xBegT, g_yBegT, g_xEndT + 1, g_yEndT + 1);
	g_pTex.copyToClipboard(ID_TEXTURE_WORK, &rcSelect, g_pUI.getChecked(ID_CHECKBOX_ALPHA));

	/*// Variant 2 (through getting the texture map)
	UINT iWidth = g_xEndT + 1 - g_xBegT;
	UINT iHeight = g_yEndT + 1 - g_yBegT;
	UINT* pBuffer = new UINT[iWidth * iHeight];
	for (int i = g_yBegT; i <= g_yEndT; i++)
	memcpy(&g_pBuffer[(i - g_yBegT)*iWidth], &g_mapWork[i * g_iWidth + g_xBegT], iWidth << 2);
	SetRect(&g_rcBuffer, g_xBegT, g_yBegT, g_xEndT + 1, g_yEndT + 1);
	g_pTex.copyMapToClipboard(g_pBuffer, iWidth, iHeight, g_pUI.getChecked(ID_CHECKBOX_ALPHA));
	SAFE_DELETE_ARRAY(pBuffer);
	*/
}


//--------------------------------------------------------------------------------------
// CUT TO CLIPBOARD
//--------------------------------------------------------------------------------------
void cut()
{
	copy();
	rememberMap();	// Remember map of the previous state
	for (int i = g_yBegT; i <= g_yEndT; i++)
		for (int j = g_xBegT; j <= g_xEndT; j++)
			g_mapWork[i*g_iWidth + j] = 0;	// 0x00000000 in RGBA
	g_pTex.setMap(ID_TEXTURE_WORK, g_mapWork);	// 
}


//--------------------------------------------------------------------------------------
// PASTE FROM CLIPBOARD
//--------------------------------------------------------------------------------------
void paste()
{
	// If the texture is empty, then autodetect sizes of inserted textures, otherwise regular insertion and cutting to embed images if needed
	RECT rcSelect;
	g_bSelect ? SetRect(&rcSelect, g_xBegT, g_yBegT, g_xEndT + 1, g_yEndT + 1) : SetRect(&rcSelect, 0, 0, 1, 1);
	g_pTex.pasteFromClipboard(ID_TEXTURE_WORK, &rcSelect, g_pUI.getChecked(ID_CHECKBOX_ALPHA));
	rememberMap();	// Remember map of the previous state
	g_pTex.getMap(ID_TEXTURE_WORK, g_mapWork);
	g_bSelect = false;
}


//--------------------------------------------------------------------------------------
// CHANGE FROM CLIPBOARD
//--------------------------------------------------------------------------------------
void change()
{
	RECT rcSelect;
	g_bSelect ? SetRect(&rcSelect, g_xBegT, g_yBegT, g_xEndT + 1, g_yEndT + 1) : SetRect(&rcSelect, 0, 0, 1, 1);
	g_pTex.changeFromClipboard(ID_TEXTURE_WORK, &rcSelect, g_pUI.getChecked(ID_CHECKBOX_ALPHA));
	rememberMap();	// Remember map of the previous state
	g_pTex.getMap(ID_TEXTURE_WORK, g_mapWork);
	g_bSelect = false;
}


//--------------------------------------------------------------------------------------
// UPDATE TEXTURE MAP
//--------------------------------------------------------------------------------------
void updateTextureMap()	// Use after resizing texture, if the dimensions were not changed, it is best to use g_pAppl.getTextureMap
{
	SAFE_DELETE_ARRAY(g_mapWork);
	g_mapWork = new UINT[g_iWidth * g_iHeight];
	SAFE_DELETE_ARRAY(g_mapLast);
	g_mapLast = new UINT[g_iWidth * g_iHeight];
	g_pTex.getMap(ID_TEXTURE_WORK, g_mapWork);
	rememberMap();
}