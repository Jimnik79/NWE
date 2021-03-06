//--------------------------------------------------------------------------------------
// File: 5 GUI Skin.cpp
// Description: Sample of GUI skin.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with GUI Skin.

// Setting the GUI Skin occurs in 2 stages:
// 1. Setting the name of UI texture file to entry point to the program, using function setTextureUI();
// 2. Setting texture coordinates for UI controls to initialize.

// Used content:
// - Content\Texture\NetWarsUI.dds

// Comment out the first version of GUI coordinates, and uncomment the second.


#include "..\Engine\NetWars.h"	// Engine


CEngine	g_pAppl;	// Base Engine class
CText	g_pText;	// Text class
CUI		g_pUI;		// UI class

float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background

#define	ID_CREEPLINE	0
#define	ID_CHART		1
#define	ID_TREEVIEW		2
#define	ID_WINDOW		3
#define	ID_EDITBOX		4
#define	ID_EDITBOX2		5
#define	ID_LISTBOX		6
#define	ID_LISTBOX2		7
#define	ID_COMBOBOX		8
#define	ID_COMBOBOX2	9
#define	ID_BUTTON		10
#define	ID_BUTTON2		11
#define	ID_SLIDER		12
#define	ID_CHECKBOX		13
#define	ID_CHECKBOX2	14
#define	ID_LINK			15
#define	ID_RADIOBUTTON1	16
#define	ID_RADIOBUTTON2	17
#define	ID_RADIOBUTTON3	18
#define	ID_RADIOBUTTON4	19
#define	ID_RADIOBUTTON5	20
#define	ID_RADIOBUTTON6	21
#define	ID_RADIOBUTTON7	22
#define	ID_STATIC		23
#define	ID_SCROLL		24
#define	ID_MESSAGE		25

#define	ID_FONT_20		0
#define	ID_FONT_16		1

#define	GROUP_LANGUAGE	0
#define	GROUP_GENDER	1

#define	ID_SERIES_RED	0	// Red line on the chart
#define	ID_SERIES_GREEN	1	// Green line on the chart
#define	ID_SERIES_BLUE	2	// Blue line on the chart


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	// Set UI File

	//g_pAppl.addLOG(sFolder);	// Add logging

	g_pAppl.setCallbackInitialize();		// Set callback for Initialize
	g_pAppl.setCallbackGUI();				// Set callback for GUI event
	g_pAppl.setCallbackDeviceCreated();		// Set callback for Create Device
	g_pAppl.setCallbackSwapChainResized();	// Set callback for Resize
	g_pAppl.setCallbackFrameRender();		// Set callback for Render Frame

	g_pText.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_pUI.setHModule(g_pAppl.getHModule());		// Set NetWars.dll

	g_pUI.setNameTextureUI(L"..\\Content\\Texture\\NetWarsUI.dds");	// Set the name of UI texture file

	// Run Engine
	return g_pAppl.run(L"5 GUI Skin", 1024, 600);
}


//--------------------------------------------------------------------------------------
// INITIALIZING
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	// Adding fonts
	g_pAppl.addFont(L"Arial", 20, FW_MEDIUM);	// Return ID_FONT_20 = 0, adding font for GUI
	g_pAppl.addFont(L"Times new Roman", 16, FW_MEDIUM);	// Return ID_FONT_16 = 1, adding font for GUI and texts

	// BEGIN. First version of GUI coordinates
	RECT rcEdit;	 rcEdit.left =  8;	 rcEdit.right = 246;	 rcEdit.top =  66;	 rcEdit.bottom = 105;	// EditBox
	RECT rcText;	 rcText.left = 20;	 rcText.right = 234;	 rcText.top =  78;	 rcText.bottom =  93;	// EditBox, text
	RECT rcCaret;	rcCaret.left = 40;	rcCaret.right =  41;	rcCaret.top = 121;	rcCaret.bottom = 122;	// EditBox, caret
	RECT rcCombo;	rcCombo.left = 98;	rcCombo.right = 151;	rcCombo.top = 189;	rcCombo.bottom = 238;	// ComboBox, button
	RECT rcSel;		  rcSel.left = 18;	  rcSel.right = 238;	  rcSel.top = 166;	  rcSel.bottom = 183;	// Select
	g_pUI.setSkinEdit(&rcEdit, &rcText, &rcCaret, &rcCombo, &rcSel, 6);

	RECT rcBar;		  rcBar.left = 198;	  rcBar.right = 216;	  rcBar.top = 194;	  rcBar.bottom = 243;	// ScrollBar
	RECT rcTrack;	rcTrack.left = 211;	rcTrack.right = 226;	rcTrack.top = 218;	rcTrack.bottom = 223;	// ScrollBar, track
	RECT rcThumb;	rcThumb.left = 221;	rcThumb.right = 237;	rcThumb.top = 192;	rcThumb.bottom = 234;	// ScrollBar, thumb
	RECT rcCen;		  rcCen.left = 202;	  rcCen.right = 224;	  rcCen.top = 202;	  rcCen.bottom = 224;	// ScrollBar, thumb, center
	g_pUI.setSkinBar(&rcBar, &rcTrack, &rcThumb, &rcCen);

	RECT rcLine;	 rcLine.left = 1;	 rcLine.right =  93;	 rcLine.top = 187;	 rcLine.bottom = 228;	// Slider
					rcThumb.left = 151;	rcThumb.right = 192;	rcThumb.top = 193;	rcThumb.bottom = 234;	// Slider, thumb
	g_pUI.setSkinSlider(&rcLine, &rcThumb);

	RECT rcCheck;	 rcCheck.left =  0;	   rcCheck.right = 27;	 rcCheck.top = 108;	 rcCheck.bottom = 135;	// CheckBox, background
	RECT rcCheck1;	rcCheck1.left = 27;	  rcCheck1.right = 54;	rcCheck1.top = 108;	rcCheck1.bottom = 135;	// CheckBox, check
	RECT rcCheck2;	rcCheck1.left = 54;	  rcCheck1.right = 81;	rcCheck1.top = 108;	rcCheck1.bottom = 135;	// CheckBox, X
	RECT rcRadio;	 rcRadio.left = 81;	  rcRadio.right = 108;	 rcRadio.top = 108;	 rcRadio.bottom = 135;	// Radiobutton, background
	RECT rcRadio1;	rcRadio1.left = 108; rcRadio1.right = 135;	rcRadio1.top = 108;	rcRadio1.bottom = 135;	// Radiobutton, dot
	RECT rcTree;	  rcTree.left = 135;   rcTree.right = 162;	  rcTree.top = 108;	  rcTree.bottom = 135;	// TreeView, branches closed
	RECT rcTree1;	 rcTree1.left = 162;  rcTree1.right = 189;	 rcTree1.top = 108;	 rcTree1.bottom = 135;	// TreeView, branches opened
	g_pUI.setSkinCheckRadioTree(&rcCheck, &rcCheck1, &rcCheck2, &rcRadio, &rcRadio1, &rcTree, &rcTree1);

	RECT rcButton[24];
	for (int i = 0; i<6; i++)
	{
	SetRect(&rcButton[2 * i], 42 * i, 0, 42 * i + 41, 32);
	SetRect(&rcButton[2 * i + 1], 42 * i + 16, 0, 42 * i + 17, 32);
	}
	for (int i = 0; i<6; i++)
	{
	SetRect(&rcButton[12 + 2 * i], 42 * i, 33, 42 * i + 41, 65);
	SetRect(&rcButton[13 + 2 * i], 42 * i + 16, 33, 42 * i + 17, 65);
	}
	g_pUI.setSkinButton(rcButton);

	RECT rcWindow[18];
	SetRect(&rcWindow[0],   0,  87, 34, 119);	// Window, up-left
	SetRect(&rcWindow[1],  34,  87, 35, 119);	// Window, up
	SetRect(&rcWindow[2],  34,  87, 78, 119);	// Window, up-right
	SetRect(&rcWindow[3],   0, 119, 34, 214);	// Window, left
	SetRect(&rcWindow[4],  34, 119, 78, 214);	// Window, right
	SetRect(&rcWindow[5],   0, 214, 34, 256);	// Window, down-left
	SetRect(&rcWindow[6],  34, 214, 35, 256);	// Window, down
	SetRect(&rcWindow[7],  34, 214, 78, 256);	// Window, down-right
	SetRect(&rcWindow[8],  80, 151, 82, 153);	// Window, title, up
	SetRect(&rcWindow[9],  80, 153, 82, 151);	// Window, title, down
	SetRect(&rcWindow[10], 92, 151, 90, 153);	// Window, title, left
	SetRect(&rcWindow[11], 90, 151, 92, 153);	// Window, title, right
	SetRect(&rcWindow[12], 85, 151, 87, 153);	// Window, title, up-left
	SetRect(&rcWindow[13], 87, 151, 85, 153);	// Window, title, up-right
	SetRect(&rcWindow[14], 85, 153, 87, 151);	// Window, title, down-left
	SetRect(&rcWindow[15], 87, 153, 85, 151);	// Window, title, down-right
	SetRect(&rcWindow[16], 95, 151, 97, 153);	// Window, title, centre
	SetRect(&rcWindow[17], 34, 150, 35, 151);	// Window, space
	g_pUI.setSkinWindow(rcWindow);
	// END. First version of GUI coordinates

	/*
	// BEGIN. Second version of GUI coordinates
	RECT rcEdit;	 rcEdit.left = 8;	 rcEdit.right = 246;	 rcEdit.top = 16;	 rcEdit.bottom = 55;	// EditBox
	RECT rcText;	 rcText.left = 20;	 rcText.right = 234;	 rcText.top = 28;	 rcText.bottom = 43;	// EditBox, text
	RECT rcCaret;	rcCaret.left = 40;	rcCaret.right =  41;	rcCaret.top = 121;	rcCaret.bottom = 122;	// EditBox, caret
	RECT rcCombo;	rcCombo.left = 98;	rcCombo.right = 151;	rcCombo.top = 39;	rcCombo.bottom = 88;	// ComboBox, button
	RECT rcSel;		  rcSel.left = 18;	  rcSel.right = 238;	  rcSel.top = 16;	  rcSel.bottom = 33;	// Select
	g_pUI.setSkinEdit(&rcEdit, &rcText, &rcCaret, &rcCombo, &rcSel, 6);

	RECT rcBar;		  rcBar.left = 198;	  rcBar.right = 216;	  rcBar.top = 94;	  rcBar.bottom = 143;	// ScrollBar
	RECT rcTrack;	rcTrack.left = 211;	rcTrack.right = 226;	rcTrack.top = 118;	rcTrack.bottom = 123;	// ScrollBar, track
	RECT rcThumb;	rcThumb.left = 221;	rcThumb.right = 237;	rcThumb.top = 92;	rcThumb.bottom = 134;	// ScrollBar, thumb
	RECT rcCen;		  rcCen.left = 202;	  rcCen.right = 224;	  rcCen.top = 102;	  rcCen.bottom = 124;	// ScrollBar, thumb, center
	g_pUI.setSkinBar(&rcBar, &rcTrack, &rcThumb, &rcCen);

	RECT rcLine;	 rcLine.left = 1;	 rcLine.right =  93;	 rcLine.top = 37;	 rcLine.bottom = 78;	// Slider
					rcThumb.left = 151;	rcThumb.right = 192;	rcThumb.top = 43;	rcThumb.bottom = 84;	// Slider, thumb
	g_pUI.setSkinSlider(&rcLine, &rcThumb);

	RECT rcCheck;	 rcCheck.left =  0;	 rcCheck.right = 27;	 rcCheck.top = 8;	 rcCheck.bottom = 35;	// CheckBox, background
	RECT rcCheck1;	rcCheck1.left = 27;	rcCheck1.right = 54;	rcCheck1.top = 8;	rcCheck1.bottom = 35;	// CheckBox, check
	RECT rcRadio;	 rcRadio.left = 54;	 rcRadio.right = 81;	 rcRadio.top = 8;	 rcRadio.bottom = 35;	// Radiobutton, background
	RECT rcRadio1;	rcRadio1.left = 81;	rcRadio1.right = 108;	rcRadio1.top = 8;	rcRadio1.bottom = 35;	// Radiobutton, dot
	g_pUI.setSkinCheckRadio(&rcCheck, &rcCheck1, &rcRadio, &rcRadio1);

	RECT rcButton[24];
	for (int i = 0; i<6; i++)
	{
		SetRect(&rcButton[2 * i], 42 * i, 0, 42 * i + 41, 32);
		SetRect(&rcButton[2 * i + 1], 42 * i + 16, 0, 42 * i + 17, 32);
	}
	for (int i = 0; i<6; i++)
	{
		SetRect(&rcButton[12 + 2 * i], 42 * i, 33, 42 * i + 41, 65);
		SetRect(&rcButton[13 + 2 * i], 42 * i + 16, 33, 42 * i + 17, 65);
	}
	g_pUI.setSkinButton(rcButton);

	RECT rcWindow[18];
	SetRect(&rcWindow[0],   0,  87, 34, 119);	// Window, up-left
	SetRect(&rcWindow[1],  34,  87, 35, 119);	// Window, up
	SetRect(&rcWindow[2],  34,  87, 78, 119);	// Window, up-right
	SetRect(&rcWindow[3],   0, 119, 34, 214);	// Window, left
	SetRect(&rcWindow[4],  34, 119, 78, 214);	// Window, right
	SetRect(&rcWindow[5],   0, 214, 34, 256);	// Window, down-left
	SetRect(&rcWindow[6],  34, 214, 35, 256);	// Window, down
	SetRect(&rcWindow[7],  34, 214, 78, 256);	// Window, down-right
	SetRect(&rcWindow[8],  80, 151, 82, 153);	// Window, title, up
	SetRect(&rcWindow[9],  80, 153, 82, 151);	// Window, title, down
	SetRect(&rcWindow[10], 92, 151, 90, 153);	// Window, title, left
	SetRect(&rcWindow[11], 90, 151, 92, 153);	// Window, title, right
	SetRect(&rcWindow[12], 85, 151, 87, 153);	// Window, title, up-left
	SetRect(&rcWindow[13], 87, 151, 85, 153);	// Window, title, up-right
	SetRect(&rcWindow[14], 85, 153, 87, 151);	// Window, title, down-left
	SetRect(&rcWindow[15], 87, 153, 85, 151);	// Window, title, down-right
	SetRect(&rcWindow[16], 95, 151, 97, 153);	// Window, title, centre
	SetRect(&rcWindow[17], 34, 150, 35, 151);	// Window, space
	g_pUI.setSkinWindow(rcWindow);
	// END. Second version of GUI coordinates
	*/
	// Adding UI controls
	// Adding UI controls
	g_pUI.addCreepLine(-1, ID_FONT_20, L"Creep Line", 600, COLOR_RED, 0.9f);	// Return ID_CREEPLINE = 0

	g_pUI.addChart(-1, ID_FONT_16, L"Chart", 640, 480, 0.9f);	// Return ID_CHART = 1
	g_pUI.setXText(ID_CHART, L"X parametrs");	// Set the title text on the X axis
	g_pUI.setYText(ID_CHART, L"Y parametrs");	// Set the title text on the Y axis
	g_pUI.setBackGroundColor(ID_CHART, 0xff80c0ff);
	g_pUI.setBorderColor(ID_CHART, 0xffff00ff);
	g_pUI.setBarLinesColor(ID_CHART, 0xffff8080);
	g_pUI.setTitleColor(ID_CHART, 0xff0000ff);
	g_pUI.setXTextColor(ID_CHART, 0xff00ff00);
	g_pUI.setYTextColor(ID_CHART, 0xffff0000);
	g_pUI.setYTextColor(ID_CHART, 0xffff0000);
	g_pUI.addSeries(ID_CHART, COLOR_RED, 0, 0);	// Return ID_SERIES_RED = 0
	for (int i = 1; i < 102; i++)
		g_pUI.addPoint(ID_CHART, ID_SERIES_RED, float(i));
	g_pUI.addSeries(ID_CHART, COLOR_GREEN, 256, 100);	// Return ID_SERIES_GREEN = 1
	for (int i = 1; i < 151; i++)
		g_pUI.addPoint(ID_CHART, ID_SERIES_GREEN, float(100 - i));
	g_pUI.addSeries(ID_CHART, COLOR_BLUE, 256, 20);	// Return ID_SERIES_BLUE = 2
	for (float f = 0.2f; f < 25.1f; f += 0.2f)
		g_pUI.addPoint(ID_CHART, ID_SERIES_BLUE, float(20.0f + 50.0f*sin(f)));

	g_pUI.addTreeView(-1, ID_FONT_16, 250, 10, 0.9f);	// Return ID_TREEVIEW = 2
	g_pUI.addItem(ID_TREEVIEW, L"Node 0", 0);	// nodes leaves
	g_pUI.addItem(ID_TREEVIEW, L"Node 1sfsafsafsadfasdfsdafsadfsdaf", 1);	// 
	g_pUI.addItem(ID_TREEVIEW, L"Node 2", 2);	// 
	g_pUI.addItem(ID_TREEVIEW, L"Node 3", 3);	// 
	g_pUI.addItem(ID_TREEVIEW, L"Node 4dfgsdgfsfsfdsdfafsasafsdfasf", 3);	// 
	g_pUI.addItem(ID_TREEVIEW, L"Node 5", 3);	// 
	g_pUI.addItem(ID_TREEVIEW, L"Node 6", 6);	// 
	g_pUI.addItem(ID_TREEVIEW, L"Node 7", 7);	// 
	g_pUI.addItem(ID_TREEVIEW, L"Node 8", 8);	// 
	g_pUI.addItem(ID_TREEVIEW, L"Node 9", 9);	// 
	g_pUI.addItem(ID_TREEVIEW, L"Node 10", 10);	// 

	//g_pUI.addButton(-1, ID_FONT_16, L"Chart", 640, 1, 0.9f);
	g_pUI.addWindow(ID_FONT_20, L"SAMPLE OF GUI", 700, 450, COLOR_ORANGE, true, 0.8f);	// Return ID_WINDOW = 3
	g_pUI.addEditBox(ID_WINDOW, ID_FONT_20, L"EditBox", 150, 0.8f);	// Return ID_EDITBOX = 4
	g_pUI.addEditBox(ID_WINDOW, ID_FONT_16, L"EditBox2", 150, 0.8f);	// Return ID_EDITBOX = 5

	g_pUI.addListBox(ID_WINDOW, ID_FONT_20, 150, 5, 0.8f);	// Return ID_LISTBOX = 6
	g_pUI.addItem(ID_LISTBOX, L"0. LB string 0", 0);
	g_pUI.addItem(ID_LISTBOX, L"2. LB string 2", 2);
	g_pUI.insItem(ID_LISTBOX, 1, L"1. LB string 1", 1);
	g_pUI.addItem(ID_LISTBOX, L"3. LB string 3", 3);
	g_pUI.addItem(ID_LISTBOX, L"4. LB string 4", 4);
	g_pUI.addItem(ID_LISTBOX, L"5. LB string 5", 5);
	g_pUI.addItem(ID_LISTBOX, L"6. LB string 6", 5);
	g_pUI.selItem(ID_LISTBOX, 4);
	g_pUI.addListBox(ID_WINDOW, ID_FONT_16, 150, 6, 0.8f);	// Return ID_LISTBOX = 7
	g_pUI.addItem(ID_LISTBOX2, L"0. LB2 string 0", 0);
	g_pUI.addItem(ID_LISTBOX2, L"2. LB2 string 2", 2);
	g_pUI.insItem(ID_LISTBOX2, 1, L"1. LB2 string 1", 1);
	g_pUI.addItem(ID_LISTBOX2, L"3. LB2 string 3", 3);
	g_pUI.addItem(ID_LISTBOX2, L"4. LB2 string 4", 4);
	g_pUI.addItem(ID_LISTBOX2, L"5. LB2 string 5", 5);
	g_pUI.addItem(ID_LISTBOX2, L"6. LB2 string 6", 5);
	g_pUI.selItem(ID_LISTBOX2, 4);

	g_pUI.addComboBox(ID_WINDOW, ID_FONT_20, 150, 0.8f);	// Return ID_COMBOBOX = 8
	g_pUI.addItem(ID_COMBOBOX, L"0. CB string 0", 0);
	g_pUI.addItem(ID_COMBOBOX, L"1. CB string 1", 1);
	g_pUI.addItem(ID_COMBOBOX, L"3. CB string 3", 3);
	g_pUI.insItem(ID_COMBOBOX, 2, L"2. CB string 2", 2);
	g_pUI.addItem(ID_COMBOBOX, L"4. CB string 4", 4);
	g_pUI.addItem(ID_COMBOBOX, L"5. CB string 5", 5);
	g_pUI.addItem(ID_COMBOBOX, L"6. CB string 6", 6);
	g_pUI.addItem(ID_COMBOBOX, L"7. CB string 7", 6);
	g_pUI.addComboBox(ID_WINDOW, ID_FONT_16, 150, 0.8f);	// Return ID_COMBOBOX = 9
	g_pUI.addItem(ID_COMBOBOX2, L"0. CB2 string 0", 0);
	g_pUI.addItem(ID_COMBOBOX2, L"1. CB2 string 1", 1);
	g_pUI.addItem(ID_COMBOBOX2, L"3. CB2 string 3", 3);
	g_pUI.insItem(ID_COMBOBOX2, 2, L"2. CB2 string 2", 2);
	g_pUI.addItem(ID_COMBOBOX2, L"4. CB2 string 4", 4);
	g_pUI.addItem(ID_COMBOBOX2, L"5. CB2 string 5", 5);
	g_pUI.addItem(ID_COMBOBOX2, L"6. CB2 string 6", 6);
	g_pUI.addItem(ID_COMBOBOX2, L"7. CB2 string 7", 6);

	g_pUI.addButton(ID_WINDOW, ID_FONT_20, L"Button", 150, 1, 0.8f);	// Return ID_BUTTON = 10
	g_pUI.addButton(ID_WINDOW, ID_FONT_16, L"Button2", 150, 1, 0.8f);	// Return ID_BUTTON = 11
	g_pUI.addSlider(ID_WINDOW, 200, 20, 0, 200, 100, 0.8f);	// Return ID_SLIDER = 12
	g_pUI.addCheckBox(ID_WINDOW, ID_FONT_20, L"CheckBox", COLOR_GOLD, true, 0.8f);	// Return ID_CHECKBOX = 13
	g_pUI.setCross(ID_CHECKBOX, true);	// X for CheckBox
	g_pUI.setConstColor(ID_CHECKBOX, true);	// Const color ot the text
	g_pUI.addCheckBox(ID_WINDOW, ID_FONT_16, L"CheckBox2", COLOR_GOLD, false, 0.8f);	// Return ID_CHECKBOX2 = 14
	g_pUI.addLink(ID_WINDOW, ID_FONT_20, L"World Mind Sports Games", COLOR_LINK, 0.8f);	// Return ID_LINK = 15
	g_pUI.setStateColor(ID_LINK, HYPER_FOCUS, COLOR_GOLD);
	g_pUI.setStateColor(ID_LINK, HYPER_MOUSEOVER, COLOR_GOLD);
	g_pUI.setStateColor(ID_LINK, HYPER_PRESSED, COLOR_GOLD);

	g_pUI.addRadioButton(ID_WINDOW, ID_FONT_20, GROUP_LANGUAGE, L"English", COLOR_GOLD, 0.8f);	// Return ID_RADIOBUTTON1 = 16, English, First - TRUE
	g_pUI.addRadioButton(ID_WINDOW, ID_FONT_20, GROUP_LANGUAGE, L"Русский", COLOR_GOLD, 0.8f);	// Return ID_RADIOBUTTON2 = 17, Russian, Other - FALSE
	g_pUI.addRadioButton(ID_WINDOW, ID_FONT_20, GROUP_LANGUAGE, L"Deutsch", COLOR_GOLD, 0.8f);	// Return ID_RADIOBUTTON3 = 18, German
	g_pUI.addRadioButton(ID_WINDOW, ID_FONT_20, GROUP_LANGUAGE, L"中文", COLOR_GOLD, 0.8f);		// Return ID_RADIOBUTTON4 = 19, Chinese
	g_pUI.addRadioButton(ID_WINDOW, ID_FONT_20, GROUP_LANGUAGE, L"العربية", COLOR_GOLD, 0.8f);// Return ID_RADIOBUTTON5 = 20, Arabic
	g_pUI.addRadioButton(ID_WINDOW, ID_FONT_16, 1, L"Male", COLOR_GOLD, 0.8f);	// Return ID_RADIOBUTTON6 = 21
	g_pUI.addRadioButton(ID_WINDOW, ID_FONT_16, 1, L"Female", COLOR_GOLD, 0.8f);	// Return ID_RADIOBUTTON7 = 22
	g_pUI.addStatic(ID_WINDOW, ID_FONT_20, L"Static text.", DT_LEFT, COLOR_GOLD, 0.8f);	// Return ID_STATIC = 23
	g_pUI.addScrollBar(ID_WINDOW, 1, 2, 16, 270, 0.7f);	// Return ID_SCROLL = 24

	g_pUI.addMessage(ID_FONT_20, L"Click OK to close message.", L"OK", 500, 210, COLOR_GOLD, true, 0.7f);	// Return ID_MESSAGE = 25
	g_pUI.addItem(ID_MESSAGE, L"Click OK to string 2.", 0);	// NULL parameter (int data) has not taken
	g_pUI.addItem(ID_MESSAGE, L"Click OK to string 3.", 0);
	g_pUI.addItem(ID_MESSAGE, L"Click OK to string 4.", 0);
}


void CALLBACK onGUI(UINT, int idControl)
{
	switch (idControl)
	{
	case ID_LINK:	// World Mind Sports Games
		g_pAppl.mustWindowed();	// Windowed mode
		ShellExecuteW(NULL, L"open", L"http://www.wmsg6.narod.ru", NULL, NULL, SW_SHOW);	// Open in the browser
		break;
	}
}

//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 0, add space for fps
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 1, add space for timer
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int w, int h)
{
	int hw = w >> 1;
	int hh = h >> 1;

	g_pText.pos(0, 5, 2);
	g_pText.pos(1, 105, 2);

	g_pUI.pos(ID_CREEPLINE, hw - 300, 0);
	g_pUI.pos(ID_CHART, 20, 20);
	g_pUI.pos(ID_TREEVIEW, 700, 20);
	g_pUI.pos(ID_WINDOW, 0, hh - 280);
	g_pUI.pos(ID_EDITBOX, 50, hh - 190);
	g_pUI.pos(ID_EDITBOX2, 220, hh - 190);
	g_pUI.pos(ID_LISTBOX, 50, hh - 150);
	g_pUI.pos(ID_LISTBOX2, 220, hh - 150);
	g_pUI.pos(ID_COMBOBOX, 50, hh - 30);
	g_pUI.pos(ID_COMBOBOX2, 220, hh - 30);
	g_pUI.pos(ID_BUTTON, 50, hh + 10);
	g_pUI.pos(ID_BUTTON2, 220, hh + 10);
	g_pUI.pos(ID_SLIDER, 390, hh - 190);
	g_pUI.pos(ID_CHECKBOX, 390, hh - 160);
	g_pUI.pos(ID_CHECKBOX2, 390, hh - 130);
	g_pUI.pos(ID_LINK, 390, hh - 100);
	g_pUI.pos(ID_RADIOBUTTON1, 390, hh - 70);
	g_pUI.pos(ID_RADIOBUTTON2, 390, hh - 48);
	g_pUI.pos(ID_RADIOBUTTON3, 390, hh - 26);
	g_pUI.pos(ID_RADIOBUTTON4, 390, hh - 4);
	g_pUI.pos(ID_RADIOBUTTON5, 390, hh + 18);
	g_pUI.pos(ID_RADIOBUTTON6, 390, hh + 42);
	g_pUI.pos(ID_RADIOBUTTON7, 390, hh + 60);
	g_pUI.pos(ID_STATIC, 200, hh + 100);
	g_pUI.pos(ID_SCROLL, 625, hh - 190);
	g_pUI.pos(ID_MESSAGE, 400, hh + 90);
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double, float)
{
	// Update dynamic texts
	g_pText.fps(ID_FONT_16, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT_16, 1, DT_LEFT, COLOR_ORANGE);	// timer

	// Render
	g_pAppl.clearScreen(g_cClear);
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pUI.render();
	g_pAppl.setRenderTexts();
	g_pText.draw(0, 2);	// Render fps and timer
}


