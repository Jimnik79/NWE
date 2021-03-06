//--------------------------------------------------------------------------------------
// File: 4 GUI.cpp
// Description: Sample of GUI.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with:
// - GUI elements (window, message, editbox, listbox, combobox, button, text, creepline, slider, checkbox, link, radiobutton);
// - GUI events (click, move, select, change, enter, update, edit, rclick, dblclick, rdblclick, copy);
// - setting callback functions to get GUI event.

// Setting the UI control occurs in 3 stages:
// 1. Adding the font to initializing. Returns the ID of the font, starting with 0.
// 2. Adding the UI control with depth and other parameters when creating device. Returns the ID of the UI control, starting with 0.
// 3. Setting the display position of the UI control when screen resized.
// -- All elements that are not prohibited by visualization (visible(false)), will be rendered.
// -- All elements that are not disabled (enable(false)), will be called events.

// NOTE:
// - Depth of UI controls > 0.0f and < 1.0f, rendered less. For overlapping UI controls, UI controls with lower depth should be rendered after UI controls with higher depth. Behind it you will need to keep an eye on their own.
// - When using Unicode text in UI control save this file as Unicode: FILE -> Advanced Save Options... -> Unicode - Codepage 1200.
// - UI control tied to the window control, can be added before adding window control or after.
// - You can use IDs of GUI element defined by "#define" (const IDs) or defined by "UINT" (variable IDs)
// - You can use text class for working with text or use text GUI element. You must decide which way when it is better to be applied.
// - Overlapping elements, such as ComboBox, should be added after the elements which them overlap.
// - Charting support Random and Uniform (values of X correspond to the natural numbers) data types.


// In the next version of SDK, maybe, will be added TabControl, PageControl, Grid (as in Excel), ToolTip, ProgressBar, Chart3D, ScrollBar, FileDialog (Open/Save Dialog).
// In the next version of SDK, maybe, will be described the list of components and details of how to work with each of them (including the description of the work with ToolTip).


#include "..\Engine\NetWars.h"	// Engine

#include <stdio.h>	// swprintf_s


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
	g_pAppl.setCallbackInitialize();		// Set callback for Initialize
	g_pAppl.setCallbackGUI();				// Set callback for GUI event
	g_pAppl.setCallbackDeviceCreated();		// Set callback for Create Device
	g_pAppl.setCallbackSwapChainResized();	// Set callback for Resize
	g_pAppl.setCallbackFrameRender();		// Set callback for Render Frame

	g_pText.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_pUI.setHModule(g_pAppl.getHModule());		// Set NetWars.dll

	// Run Engine
	return g_pAppl.run(L"4 GUI", 1024, 600);
}


//--------------------------------------------------------------------------------------
// INITIALIZING
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	// Adding fonts
	g_pAppl.addFont(L"Arial", 20, FW_MEDIUM);	// Return ID_FONT_20 = 0, adding font for GUI
	g_pAppl.addFont(L"Times new Roman", 16, FW_MEDIUM);	// Return ID_FONT_16 = 1, adding font for GUI and texts

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
	// NOTE: You can use textMsg() instead of three call addItem()
}


//--------------------------------------------------------------------------------------
// GUI EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onGUI(UINT idEvent, int idControl)
{
	switch (idEvent)
	{
	case EVENT_CLICK:	// Clicked button Event: Window (exit), Message, Button
		switch (idControl)
		{
		case ID_WINDOW:	// Close message (now not working)
			return;
		case ID_MESSAGE:	// Close message
			g_pUI.visible(ID_MESSAGE, false);
			g_pUI.text(ID_STATIC, L"Closed message");
			return;
		case ID_BUTTON:		// Open message
			if (g_pUI.getVisible(ID_MESSAGE))
				g_pUI.text(ID_STATIC, L"Clicked button");
			else
			{
				g_pUI.visible(ID_MESSAGE, true);
				g_pUI.text(ID_STATIC, L"Clicked button, opened message");
			}
			return;
		case ID_BUTTON2:	// Open message
			if (g_pUI.getVisible(ID_MESSAGE))
				g_pUI.text(ID_STATIC, L"Clicked button2");
			else
			{
				g_pUI.visible(ID_MESSAGE, true);
				g_pUI.text(ID_STATIC, L"Clicked button2, opened message");
			}
			return;
		}
		break;

	case EVENT_MOVE:	// Moved or selected window or message: Window, Message
		switch (idControl)
		{
		case ID_WINDOW:		// Window moved or selected
			g_pUI.text(ID_STATIC, L"Moved window");
			return;
		case ID_MESSAGE:	// Message moved or selected
			g_pUI.text(ID_STATIC, L"Moved message");
			return;
		}
		break;

	case EVENT_SELECT:	// Selection item Event: ListBox (is fired off when the selection changes in a single selection list box), ComboBox
		switch (idControl)
		{
		case ID_LISTBOX:
			g_pUI.text(ID_STATIC, g_pUI.getSelText(ID_LISTBOX));
			return;
		case ID_LISTBOX2:
			g_pUI.text(ID_STATIC, g_pUI.getSelText(ID_LISTBOX2));
			return;
		case ID_COMBOBOX:
			g_pUI.text(ID_STATIC, g_pUI.getSelText(ID_COMBOBOX));
			return;
		case ID_COMBOBOX2:
			g_pUI.text(ID_STATIC, g_pUI.getSelText(ID_COMBOBOX2));
			return;
		}
		break;

	case EVENT_CHANGE:	// Changed Event: RadioButton, CheckBox, Slider
		switch (idControl)
		{
		case ID_SLIDER:
			g_pUI.text(ID_STATIC, L"Changed slider");
			return;
		case ID_CHECKBOX:
			g_pUI.text(ID_STATIC, g_pUI.getChecked(ID_CHECKBOX) ? L"CheckBox checked" : L"CheckBox checked off");
			return;
		case ID_CHECKBOX2:
			g_pUI.text(ID_STATIC, g_pUI.getChecked(ID_CHECKBOX2) ? L"CheckBox2 checked" : L"CheckBox2 checked off");
			return;
		case ID_LINK:
			g_pAppl.mustWindowed();	// Windowed mode
			ShellExecuteW(NULL, L"open", L"http://www.wmsg6.narod.ru", NULL, NULL, SW_SHOW);	// Open in the browser
			g_pUI.text(ID_STATIC, L"World Mind Sports Games");
			return;
		case ID_RADIOBUTTON1:
			g_pUI.text(ID_STATIC, L"Changed language to English");
			return;
		case ID_RADIOBUTTON2:
			g_pUI.text(ID_STATIC, L"Changed language to Russian");
			return;
		case ID_RADIOBUTTON3:
			g_pUI.text(ID_STATIC, L"Changed language to German");
			return;
		case ID_RADIOBUTTON4:
			g_pUI.text(ID_STATIC, L"Changed language to Chinese");
			return;
		case ID_RADIOBUTTON5:
			g_pUI.text(ID_STATIC, L"Changed language to Arabic");
			return;
		}
		break;

	case EVENT_ENTER:	// Pressed enter Event: EditBox
		switch (idControl)
		{
		case ID_EDITBOX:
			g_pUI.text(ID_EDITBOX, L"Was pressed 'Enter'");
			return;
		case ID_EDITBOX2:
			g_pUI.text(ID_EDITBOX2, L"Was pressed 'Enter'");
			return;
		}
		break;

	case EVENT_UPDATE:	// Update data Event: Slider
		g_pUI.text(ID_STATIC, L"Slider updated");
		break;

	case EVENT_EDIT:	// Edit Event: EditBox (is sent when the editbox content changes due to user input)
		switch (idControl)
		{
		case ID_EDITBOX:
			g_pUI.text(ID_STATIC, L"Edited text in the EditBox");
			return;
		case ID_EDITBOX2:
			g_pUI.text(ID_STATIC, L"Edited text in the EditBox2");
			return;
		}
		break;

	case EVENT_RCLICK:	// Right clicked mouse Event: ListBox, ComboBox
		switch (idControl)
		{
		case ID_LISTBOX:
			g_pUI.text(ID_STATIC, L"Right click mouse in the ListBox");
			return;
		case ID_LISTBOX2:
			g_pUI.text(ID_STATIC, L"Right click mouse in the ListBox2");
			return;
		case ID_COMBOBOX:
			g_pUI.text(ID_STATIC, L"Right click mouse in the ComboBox");
			return;
		case ID_COMBOBOX2:
			g_pUI.text(ID_STATIC, L"Right click mouse in the ComboBox2");
			return;
		}
		break;

	case EVENT_DBLCLK:	// Double clicked mouse Event: ListBox
		switch (idControl)
		{
		case ID_LISTBOX:
			g_pUI.text(ID_STATIC, L"Double left click mouse in the ListBox");
			return;
		case ID_LISTBOX2:
			g_pUI.text(ID_STATIC, L"Double left click mouse in the ListBox2");
			return;
		}
		break;

	case EVENT_RDBLCLK:	// Right double clicked mouse Event: ListBox, ComboBox
		switch (idControl)
		{
		case ID_LISTBOX:
			g_pUI.text(ID_STATIC, L"Double right click mouse in the ListBox");
			return;
		case ID_LISTBOX2:
			g_pUI.text(ID_STATIC, L"Double right click mouse in the ListBox2");
			return;
		case ID_COMBOBOX:
			g_pUI.text(ID_STATIC, L"Double right click mouse in the ComboBox");
			return;
		case ID_COMBOBOX2:
			g_pUI.text(ID_STATIC, L"Double right click mouse in the ComboBox2");
			return;
		}
		break;

	case EVENT_COPY:	// Copyed to windows buffer Event (Ctrl+X, Ctrl+C, Ctrl+Ins, Ctrl+Del, Shift+Del): EditBox, ListBox, ComboBox
		if (OpenClipboard(NULL))
		{
			HANDLE handle = GetClipboardData(CF_UNICODETEXT);
			if (handle)
			{
				// Convert the ANSI string to Unicode, then insert to our buffer.
				WCHAR* pwszText = (WCHAR*)GlobalLock(handle);
				if (pwszText)
				{
					wchar_t s[MAX_PATH];
					swprintf_s(s, MAX_PATH, L"Copy to Windows buffer: %s", pwszText);
					g_pUI.text(ID_STATIC, s);
					GlobalUnlock(handle);
				}
			}
			CloseClipboard();
		}
		break;

	case EVENT_SCROLL:	// Scrolling
		if (idControl == ID_SCROLL)
		{
			wchar_t s[96];
			bool bVisible = !g_pUI.getScrollPos(ID_SCROLL);
			g_pUI.visible(ID_EDITBOX, bVisible);
			g_pUI.visible(ID_EDITBOX2, bVisible);
			g_pUI.visible(ID_LISTBOX, bVisible);
			g_pUI.visible(ID_LISTBOX2, bVisible);
			g_pUI.visible(ID_COMBOBOX, bVisible);
			g_pUI.visible(ID_COMBOBOX2, bVisible);
			g_pUI.visible(ID_BUTTON, bVisible);
			g_pUI.visible(ID_BUTTON2, bVisible);
			g_pUI.visible(ID_SLIDER, bVisible);
			g_pUI.visible(ID_CHECKBOX, bVisible);
			g_pUI.visible(ID_CHECKBOX2, bVisible);
			g_pUI.visible(ID_LINK, bVisible);
			g_pUI.visible(ID_RADIOBUTTON1, bVisible);
			g_pUI.visible(ID_RADIOBUTTON2, bVisible);
			g_pUI.visible(ID_RADIOBUTTON3, bVisible);
			g_pUI.visible(ID_RADIOBUTTON4, bVisible);
			g_pUI.visible(ID_RADIOBUTTON5, bVisible);
			g_pUI.visible(ID_RADIOBUTTON6, bVisible);
			g_pUI.visible(ID_RADIOBUTTON7, bVisible);			
			swprintf_s(s, 96, L"Scrolling pos = %d", g_pUI.getScrollPos(ID_SCROLL));
			g_pUI.text(ID_STATIC, s);
			g_pUI.getScrollPos(ID_SCROLL);
		}
		break;
	}

	// This can be used where the UI control have only one event
	switch (idControl)
	{
	case ID_RADIOBUTTON6:	// 
		g_pUI.text(ID_STATIC, L"Changed gender to male");
		return;
	case ID_RADIOBUTTON7:	// 
		g_pUI.text(ID_STATIC, L"Changed gender to female");
		return;
	}
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.2f);	// Return 0, add space for fps
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.2f);	// Return 1, add space for timer
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int w, int h)
{
	int hw = w >> 1;
	int hh = h >> 1;

	g_pText.pos(0, 5, 2);	// FPS
	g_pText.pos(1, 105, 2);	// Time

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