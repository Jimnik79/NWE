//--------------------------------------------------------------------------------------
// File: NetWars.h
// Description: Library for communicating with the engine (dynamic-link library NetWars.dll).
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// The library contains:
// - callback function which can be setted;
// - base macros;
// - base engine class (is required for each game/application);
// - class for work with textures;
// - class for work with 2D (sprites);
// - class for work with text;
// - class for work with graphical user interface;
// - class for work with sound (*.wav files);
// - class for work with media (*.mp3, *.avi, other files), including rendering to D3D texture;
// - class for work with 3D (meshes, keyframe animation, HLSL 5.0);
// - class for work with camera;
// - class for work with physics (static and dynamic objects, personages, speed, acceleration, free fall, collision detection and handling, shooting, explosions);
// - class for network (server, host, client);
// - class for work with MS SQL Server.


// NOTE:
// This library is used exclusively for communication with the engine (dynamic-link library NetWars.dll)
// This library is not intended for the study and for changes
// You can move Engine Files (DLL, H, DDS) to any other folder in its sole discretion, not forgetting to change the path to the DLL and H files in your project/solution.


#include <windows.h>

#pragma warning( disable: 4005 )	// Turn off the warning about overriding macros defined in <windows.h>
#include "d3d11.h"
#pragma warning( default: 4005 )

#include "D3DX10Math.h"	// D3DXMATRIX
#include "D3DX11tex.h"	// D3DX11_IMAGE_FILE_FORMAT


// Initialize
void CALLBACK onInitialize();
typedef void(CALLBACK *LPCALLBACKINITIALIZE)();

// Deinitialize
void CALLBACK onDeinitialize();
typedef void(CALLBACK *LPCALLBACKDEINITIALIZE)();

// Updates to the scene
void CALLBACK onFrameMove(double fTime, float fElapsedTime);
typedef void (CALLBACK *LPCALLBACKFRAMEMOVE)(double fTime, float fElapsedTime);

// Handles the GUI events
void CALLBACK onGUI(UINT idEvent, int idControl);
typedef void(CALLBACK *LPCALLBACKGUI)(UINT idEvent, int idControl);

// Handle key presses
void CALLBACK onKeyboard(UINT idKey, bool bKeyDown);	//, bool bAltDown
typedef void (CALLBACK *LPCALLBACKKEYBOARD)(UINT idKey, bool bKeyDown);

// Handle mouse
void CALLBACK onMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos);
typedef void (CALLBACK *LPCALLBACKMOUSE)(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos);

// Handle messages to the application
LRESULT CALLBACK onMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);
typedef LRESULT(CALLBACK *LPCALLBACKMSGPROC)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);

// Timer
//void CALLBACK onTimer(UINT idEvent);
typedef void (CALLBACK *LPCALLBACKTIMER)(UINT idEvent);

// Create resources that do not depend on the back buffer
void CALLBACK onDeviceCreated();
typedef void(CALLBACK *LPCALLBACKDEVICECREATED)();

// Create resources that depend on the back buffer
void CALLBACK onSwapChainResized(int w, int h);
typedef void(CALLBACK *LPCALLBACKSWAPCHAINRESIZED)(int w, int h);

// Create resources with DirectX that depend on the back buffer
void CALLBACK onSwapChainResizedDX(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
typedef void(CALLBACK *LPCALLBACKSWAPCHAINRESIZEDDX)(ID3D11Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);

// Render the scene
void CALLBACK onFrameRender(double fTime, float fElapsedTime);
typedef void (CALLBACK *LPCALLBACKFRAMERENDER)(double fTime, float fElapsedTime);

// Render the scene with DirectX
void CALLBACK onFrameRenderDX(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dContext, double fTime, float fElapsedTime);
typedef void (CALLBACK *LPCALLBACKFRAMERENDERDX)(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime);

// Release resources that depend on the back buffer
void CALLBACK onSwapChainReleasing();
typedef void (CALLBACK *LPCALLBACKSWAPCHAINRELEASING)();

// Release resources that do not depend on the back buffer
void CALLBACK onDeviceDestroyed();
typedef void (CALLBACK *LPCALLBACKDEVICEDESTROYED)();

// Find a new D3D11 device
bool CALLBACK onDeviceRemoved();
typedef bool (CALLBACK *LPCALLBACKDEVICEREMOVED)();

// Connect to the network (network ID, ID, type of authorization error: 
// 0 - no error, 1 - no this login, 2 - password error, 3 - already in the network, 4 - playtime is over, 5 - account banned, 6 - incorrect game version, 7+ - unknown error)
void CALLBACK onConnect(DWORD nid, WORD id, byte iAuthError);	
typedef void (CALLBACK *LPCALLBACKCONNECTPLAYER)(DWORD nid, WORD id, byte iAuthError);

// Disconnect from the network (Network ID, the reason of disconnect: 0 - DESTROY_PLAYER event, 1 - no connect)
// NOTE: the function uses the network ID (DWORD nid), but if authorization and chat has been added, then the function will use the account (DWORD acc)
void CALLBACK onDisconnect(DWORD nid, byte nReason);	
typedef void (CALLBACK *LPCALLBACKDISCONNECTPLAYER)(DWORD nid, byte nReason);

// Get the network message
void CALLBACK onNetMsg(BYTE *msg);
typedef void (CALLBACK *LPCALLBACKNETMSG)(BYTE *msg);


// Reject any D3D11 devices that aren't acceptable by returning false
//bool CALLBACK isDeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo, DXGI_FORMAT BackBufferFormat, bool bWindowed);
//typedef bool    (CALLBACK *LPCALLBACKDEVICEACCEPTABLE)(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo, DXGI_FORMAT BackBufferFormat, bool bWindowed);

// Right before creating D3D11 device, to modify the device settings as needed
//bool CALLBACK onDeviceModify(DeviceSettings* pDeviceSettings);
//typedef bool (CALLBACK *LPCALLBACKDEVICEMODIFY)(DeviceSettings* pDeviceSettings);


//--------------------------------------------------------------------------------------
// BASE MACROS
//--------------------------------------------------------------------------------------
#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)	{ hr = (x); if( FAILED(hr) ) MessageBox(NULL, L"VError", L"VError", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY); }	//{ DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#ifndef VR
#define VR(x)	{ hr = (x); if( FAILED(hr) ) { MessageBox(NULL, L"VRError", L"VRError", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY); retuen E_FAIL; } }	//{ return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#else
#ifndef V
#define V(x)	{ hr = (x); }
#endif
#ifndef VR
#define VR(x)	{ hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	{ if (p) { delete (p); (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] (p); (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)	{ if (p) { (p)->Release(); (p)=NULL; } }
#endif

#define PI2		1.570796f	// PI / 2
#define PI		3.141593f	// PI
#define _3PI2	4.712389f	// 3 * PI / 2
#define _2PI	6.283185f	// 2 * PI


//--------------------------------------------------------------------------------------
// BASE ENGINE CLASS
//--------------------------------------------------------------------------------------
#define	FILTER_ANISOT	0	// Anisotropic Filter
#define	FILTER_LINEAR	1	// Linear Filter
#define	FILTER_POINT	2	// Point Filter
#define	FILTER_POINTCOM	3	// Point Comparsion Filter

class CEngine
{
public:
	CEngine()	// Constructor
	{
// WARNING: here you can set the desired folders for the engine/DLL
#ifdef _WIN64
		m_bX64 = true;	// 64 bit build
//#if defined(DEBUG) || defined(_DEBUG)
//		m_hLib = LoadLibrary(L"..\\Engine\\NetWarsD.dll");		// DLL for Debug 64 bit compile
//#else
		m_hLib = LoadLibrary(L"..\\Engine\\NetWars.dll");		// DLL for Release (Profile) 64 bit compile
//#endif
#else
		m_bX64 = false;	// 32 bit build
//#if defined(DEBUG) || defined(_DEBUG)
//		m_hLib = LoadLibrary(L"..\\Engine\\NetWarsD_x86.dll");	// DLL for 32 bit compile
//#else
		m_hLib = LoadLibrary(L"..\\Engine\\NetWars_x86.dll");	// DLL for Release (Profile) 32 bit compile
//#endif
#endif
	}

	~CEngine()	// Destructor
	{
		if (m_hLib)
			FreeLibrary(m_hLib);
	}

	ID3D11Device*(*getD3D)();	// Get Direct3D Device
	ID3D11DeviceContext*(*getContext)();	// Get Context Device
	D3D_FEATURE_LEVEL(*getFeatureLevel)();	// Get Feature Level
	IDXGISwapChain*(*getDXGISwapChain)();	// Get DXGI Swap Chain
	ID3D11RenderTargetView*(*getRenderTargetView)();	// Get Render Target View
	ID3D11DepthStencilView*(*getDepthStencilView)();	// Get Depth Stencil View
	const DXGI_SURFACE_DESC*(*getDXGIBackBufferSurfaceDesc)();	// Get DXGI Back Buffer Surface Desc
	/// Need to add getting the current and the absolute time

	HINSTANCE(*getHInstance)();	// Get handle to Instance
	HWND(*getHWND)();	// Get handle to Window
	bool(*isWindowed)();	// Get flag of Windowed mode (true - windowed mode, false - full screen mode)
	bool(*isActive)();	// Get flag of application window activity (true - focus in, false - focus out)
	HRESULT(*toggleFullScreen)();	// Toggle between full screen and windowed
	HRESULT(*mustWindowed)();	// Exit from full screen mode to windowed mode
	void(*activeSettings)(bool bActive);	// Video settings (true - activate, false - close)
	void(*destroy)();	// Destroy the engine
	//void(*setMinScreenSize)(UINT width, UINT height);	// Setting minimum screen size
	///void rememberCurrentRenderState;
	///void setRememberRenderState;
	void(*setPauseMiliseconds)(UINT ms);	// Set the time of the interframe pause for inactivity of the application window
	
	UINT(*addTimer)(LPCALLBACKTIMER pCallbackTimer, float fTimeoutInSecs);	// Add the timer (callback function, call period in seconds)
	void(*killTimer)(UINT idTimer);	// Kill the timer

	void(*setRenderFilter)(BYTE filter);	// Set render filter (sampler state: anisot = 0, linear = 1, point = 2, etc.)

	// For CText and CUI
	UINT(*addFont)(LPCWSTR sFont, LONG height, LONG weight);	// Add the font (font name, font height, font weight)
	UINT(*addFontEx)(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline,	// Add the font with extended settings https://msdn.microsoft.com/en-us/library/windows/desktop/dd183499(v=vs.85).aspx
		DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCWSTR strFaceName);
	void(*setFont)(UINT idFont, LPCWSTR sFont, LONG height, LONG weight);	// Change the font, set the new settings (font name, font height, font weight)
	void(*setFontEx)(UINT idFont, int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline,	// Change the font, set the new extended settings https://msdn.microsoft.com/en-us/library/windows/desktop/dd183499(v=vs.85).aspx
		DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCWSTR strFaceName);

	void(*clearScreen)(const FLOAT *cClear);	// Clear the screen, the first function of render
	void(*setRenderSprites)(BYTE iType, UINT idTexture);	// Set the engine for render sprites or GUI (type: RENDER_ONLY_TEXTURE = 0, RENDER_ONLY_COLOR = 1, RENDER_TEXTURE_MULTI_COLOR = 2, RENDER_TEXTURE_PLUS_COLOR = 3)
	void(*setRenderTexts)();	// Set the engine for render texts, use only after setRenderSprites or immediately after render sprites or UI

	void(*clearDepthStencilView)();	// Clear DepthStencil buffer

	// Input devices
	int(*getMouseX)();			// Return the X position of the mouse
	int(*getMouseY)();			// Return the Y position of the mouse
	short(*getMouseWheel)();	// Return the Z changes (wheel) of the mouse
	bool(*getLMouseClick)();	// Get the flag of left mouse button pressed at the time of the last frame
	bool(*getLMouseUnClick)();	// Get the flag of left mouse button pressed over at the time of the last frame
	bool(*getLMousePress)();	// Get the flag of left mouse button pressed at the moment
	bool(*getRMouseClick)();	// Get the flag of right mouse button pressed at the time of the last frame
	bool(*getRMouseUnClick)();	// Get the flag of right mouse button pressed over at the time of the last frame
	bool(*getRMousePress)();	// Get the flag of right mouse button pressed at the moment
	bool(*getMMouseClick)();	// Get the flag of middle mouse button pressed at the time of the last frame
	bool(*getMMouseUnClick)();	// Get the flag of middle mouse button pressed over at the time of the last frame
	bool(*getMMousePress)();	// Get the flag of middle mouse button pressed at the moment
	bool(*getX1MouseClick)();	// Get the flag of X1 mouse button pressed at the time of the last frame
	bool(*getX1MouseUnClick)();	// Get the flag of X1 mouse button pressed over at the time of the last frame
	bool(*getX1MousePress)();	// Get the flag of X1 mouse button pressed at the moment
	bool(*getX2MouseClick)();	// Get the flag of X2 mouse button pressed at the time of the last frame
	bool(*getX2MouseUnClick)();	// Get the flag of X2 mouse button pressed over at the time of the last frame
	bool(*getX2MousePress)();	// Get the flag of X2 mouse button pressed at the moment

	bool(*isKeyDown)(BYTE vKey);	// Pass a virtual-key code, ex. VK_F1, 'A', VK_RETURN, VK_LSHIFT, etc
	bool(*wasKeyPressed)(BYTE vKey);	// Like isKeyDown() but return true only if the key was just pressed
	bool(*wasKeyUnPressed)(BYTE vKey);	// Like isKeyDown() but return true only if the key was just unpressed

	bool(*encodeFile)(wchar_t* sIn, wchar_t* sOut);	// Encode the file (name of In and Out)
	bool(*decodeFile)(wchar_t* sIn, wchar_t* sOut);	// Decode the file (name of In and Out)

	int run(LPCWSTR sTitle, int iWidth, int iHeight)	// Run Engine with Init and Destroy
	{
		if (m_hLib)
		{
			(FARPROC &)getD3D = GetProcAddress(m_hLib, "GetD3D11Device");	// Get Direct3D Device
			(FARPROC &)getContext = GetProcAddress(m_hLib, "GetD3D11DeviceContext");	// Get Context Device
			(FARPROC &)getFeatureLevel = GetProcAddress(m_hLib, "GetD3D11DeviceFeatureLevel");	// Get Feature Level
			(FARPROC &)getDXGISwapChain = GetProcAddress(m_hLib, "GetDXGISwapChain");	// Get DXGI Swap Chain
			(FARPROC &)getRenderTargetView = GetProcAddress(m_hLib, "GetD3D11RenderTargetView");	// Get Render Target View
			(FARPROC &)getDepthStencilView = GetProcAddress(m_hLib, "GetD3D11DepthStencilView");	// Get Depth Stencil View
			(FARPROC &)getDXGIBackBufferSurfaceDesc = GetProcAddress(m_hLib, "GetDXGIBackBufferSurfaceDesc");	// Get DXGI Back Buffer Surface Desc


			(FARPROC &)getHInstance = GetProcAddress(m_hLib, "getHInstance");	// Get handle to Instance
			(FARPROC &)getHWND = GetProcAddress(m_hLib, "getHWND");	// Get handle to Window
			(FARPROC &)isWindowed = GetProcAddress(m_hLib, "isWindowed");	// Get flag of Windowed mode (true - windowed mode, false - full screen mode)
			(FARPROC &)isActive = GetProcAddress(m_hLib, "isActive");	// Get flag of application window activity (true - focus in, false - focus out)
			(FARPROC &)toggleFullScreen = GetProcAddress(m_hLib, "toggleFullScreen");	// Toggle between full screen and windowed
			(FARPROC &)mustWindowed = GetProcAddress(m_hLib, "mustWindowed");	// Exit from full screen mode to windowed mode
			(FARPROC &)activeSettings = GetProcAddress(m_hLib, "activeSettings");	// Video settings (true - activate, false - close)
			(FARPROC &)destroy = GetProcAddress(m_hLib, "destroy");	// Destroy the engine
			//(FARPROC &)setMinScreenSize = GetProcAddress(m_hLib, "setMinScreenSize");	// Setting minimum screen size
			(FARPROC &)setPauseMiliseconds = GetProcAddress(m_hLib, "setPauseMiliseconds");	// Set the time of the interframe pause for inactivity of the application window

			(FARPROC &)addTimer = GetProcAddress(m_hLib, "addTimer");	// Add the timer
			(FARPROC &)killTimer = GetProcAddress(m_hLib, "killTimer");	// Kill the timer

			(FARPROC &)setRenderFilter = GetProcAddress(m_hLib, "setRenderFilter");	// Set render filter

			(FARPROC &)addFont = GetProcAddress(m_hLib, "addFont");	// Add the font
			(FARPROC &)addFontEx = GetProcAddress(m_hLib, "addFontEx");	// Add the font with extended settings
			(FARPROC &)setFont = GetProcAddress(m_hLib, "setFont");	// Change the font
			(FARPROC &)setFontEx = GetProcAddress(m_hLib, "setFontEx");	// Change the font, set the new extended settings

			(FARPROC &)clearScreen = GetProcAddress(m_hLib, "clearScreen");	// Clear the screen, the first function of render
			(FARPROC &)setRenderSprites = GetProcAddress(m_hLib, "setRenderSprites");	// Set the engine for render sprites or GUI
			(FARPROC &)setRenderTexts = GetProcAddress(m_hLib, "setRenderSpriteText");	// Set the engine for render texts

			(FARPROC &)clearDepthStencilView = GetProcAddress(m_hLib, "clearDepthStencilView");

			// Input devices
			(FARPROC &)getMouseX = GetProcAddress(m_hLib, "getMouseX");					// Return the X position of the mouse
			(FARPROC &)getMouseY = GetProcAddress(m_hLib, "getMouseY");					// Return the Y position of the mouse
			(FARPROC &)getMouseWheel = GetProcAddress(m_hLib, "getMouseWheel");			// Return the Z changes (wheel) of the mouse
			(FARPROC &)getLMouseClick = GetProcAddress(m_hLib, "getLMouseClick");		// Get the flag of left mouse button pressed at the time of the last frame
			(FARPROC &)getLMouseUnClick = GetProcAddress(m_hLib, "getLMouseUnClick");	// Get the flag of left mouse button pressed over at the time of the last frame
			(FARPROC &)getLMousePress = GetProcAddress(m_hLib, "getLMousePress");		// Get the flag of left mouse button pressed at the moment
			(FARPROC &)getRMouseClick = GetProcAddress(m_hLib, "getRMouseClick");		// Get the flag of right mouse button pressed at the time of the last frame
			(FARPROC &)getRMouseUnClick = GetProcAddress(m_hLib, "getRMouseUnClick");	// Get the flag of right mouse button pressed over at the time of the last frame
			(FARPROC &)getRMousePress = GetProcAddress(m_hLib, "getRMousePress");		// Get the flag of right mouse button pressed at the moment
			(FARPROC &)getMMouseClick = GetProcAddress(m_hLib, "getMMouseClick");		// Get the flag of middle mouse button pressed at the time of the last frame
			(FARPROC &)getMMouseUnClick = GetProcAddress(m_hLib, "getMMouseUnClick");	// Get the flag of middle mouse button pressed over at the time of the last frame
			(FARPROC &)getMMousePress = GetProcAddress(m_hLib, "getMMousePress");		// Get the flag of middle mouse button pressed at the moment
			(FARPROC &)getX1MouseClick = GetProcAddress(m_hLib, "getX1MouseClick");		// Get the flag of X1 mouse button pressed at the time of the last frame
			(FARPROC &)getX1MouseUnClick = GetProcAddress(m_hLib, "getX1MouseUnClick");	// Get the flag of X1 mouse button pressed over at the time of the last frame
			(FARPROC &)getX1MousePress = GetProcAddress(m_hLib, "getX1MousePress");		// Get the flag of X1 mouse button pressed at the moment
			(FARPROC &)getX2MouseClick = GetProcAddress(m_hLib, "getX2MouseClick");		// Get the flag of X2 mouse button pressed at the time of the last frame
			(FARPROC &)getX2MouseUnClick = GetProcAddress(m_hLib, "getX2MouseUnClick");	// Get the flag of X2 mouse button pressed over at the time of the last frame
			(FARPROC &)getX2MousePress = GetProcAddress(m_hLib, "getX2MousePress");		// Get the flag of X2 mouse button pressed at the moment

			(FARPROC &)isKeyDown = GetProcAddress(m_hLib, "isKeyDown");					// Pass a virtual-key code, ex. VK_F1, 'A', VK_RETURN, VK_LSHIFT, etc
			(FARPROC &)wasKeyPressed = GetProcAddress(m_hLib, "wasKeyPressed");			// Like isKeyDown() but return true only if the key was just pressed
			(FARPROC &)wasKeyUnPressed = GetProcAddress(m_hLib, "wasKeyUnPressed");		// Like isKeyDown() but return true only if the key was just unpressed

			(FARPROC &)encodeFile = GetProcAddress(m_hLib, "encodeFile");	// Encode the file
			(FARPROC &)decodeFile = GetProcAddress(m_hLib, "decodeFile");	// Decode the file

			// Run the engine
			int(*pEngineRun)(LPCWSTR sTitle, int iWidth, int iHeight);
			(FARPROC &)pEngineRun = GetProcAddress(m_hLib, "engineRun");
			return pEngineRun(sTitle, iWidth, iHeight);
		}
		else
		{
			MessageBox(NULL, m_bX64 ? L"Check the file ..\\Engine\\NetWars.dll" : L"Check the file ..\\Engine\\NetWars_x86.dll", L"Error of load Engine DLL", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
			return 1;
		}
	}

	int runEx(LPCWSTR sTitle, int iWidth, int iHeight, LPWSTR arg)	// Run Engine with Init and Destroy (extended - with argumements)
	{
		void(*pSetArg)(LPWSTR arg);
		(FARPROC &)pSetArg = GetProcAddress(m_hLib, "setArg");
		pSetArg(arg);
		return run(sTitle, iWidth, iHeight);
	}

	// Setting LOG
	void setLOG(wchar_t* sFolder, bool bUnicode)
	{
		void(*pSetLOG)(wchar_t* sFolder, bool bUnicode);
		(FARPROC &)pSetLOG = GetProcAddress(m_hLib, "setLOG");
		pSetLOG(sFolder, bUnicode);
	}

	// Setting registry folder, remember last audio and video settings and other
	void setRegistryFolder(HKEY hKey, LPCWSTR lpSubKey)
	{
		void(*pSetRegistryFolder)(HKEY hKey, LPCWSTR lpSubKey);
		(FARPROC &)pSetRegistryFolder = GetProcAddress(m_hLib, "setRegistryFolder");
		pSetRegistryFolder(hKey, lpSubKey);
	}

	// Setting minimum screen size
	void setMinScreenSize(UINT width, UINT height)
	{
		void(*pMinScreenSize)(UINT width, UINT height);
		(FARPROC &)pMinScreenSize = GetProcAddress(m_hLib, "setMinScreenSize");
		pMinScreenSize(width, height);
	}

	// Set parent HWND from main window
	void setParentHWND(HWND hWndParent)
	{
		void(*pSetParentHWND)(HWND hWndParent);
		(FARPROC &)pSetParentHWND = GetProcAddress(m_hLib, "setParentHWND");
		pSetParentHWND(hWndParent);
	}

	// Video setting
	void setVideo(UINT level, UINT lod, UINT effects, UINT sync, UINT count, UINT quality)
	{
		void(*pSetVideo)(UINT level, UINT lod, UINT effects, UINT sync, UINT count, UINT quality);
		(FARPROC &)pSetVideo = GetProcAddress(m_hLib, "setVideo");
		pSetVideo(level, lod, effects, sync, count, quality);
	}

	// Video setting, full screen
	//void setVideoFullScreen(bool bFullScreen, UINT width, UINT height, UINT rateN, UINT rateD)
	void setVideoFullScreen(bool bFullScreen, UINT width, UINT height, UINT rate)
	{
		void(*pSetVideoFullScreen)(bool bFullScreen, UINT width, UINT height, UINT rate);
		(FARPROC &)pSetVideoFullScreen = GetProcAddress(m_hLib, "setVideoFullScreen");
		pSetVideoFullScreen(bFullScreen, width, height, rate);
	}


	// Setting the callback function to initialize the application
	void setCallbackInitialize()
	{
		void(*pInitialize)(LPCALLBACKINITIALIZE pCallback);
		(FARPROC &)pInitialize = GetProcAddress(m_hLib, "SetCallbackInitialize");
		pInitialize(onInitialize);
	}

	// Setting the callback function to deinitialize the application
	void setCallbackDeinitialize()
	{
		void(*pDeinitialize)(LPCALLBACKDEINITIALIZE pCallback);
		(FARPROC &)pDeinitialize = GetProcAddress(m_hLib, "SetCallbackDeinitialize");
		pDeinitialize(onDeinitialize);
	}

	// Setting callbacks for GUI events
	void setCallbackGUI()
	{
		void(*pGUI)(LPCALLBACKGUI pCallback);
		(FARPROC &)pGUI = GetProcAddress(m_hLib, "SetCallbackGUI");
		pGUI(onGUI);
	}

	// Setting callbacks for keyboard events
	void setCallbackKeyboard()
	{
		void(*pKeyboard)(LPCALLBACKKEYBOARD pCallback);
		(FARPROC &)pKeyboard = GetProcAddress(m_hLib, "SetCallbackKeyboard");
		pKeyboard(onKeyboard);
	}

	// Setting callbacks for mouse events
	void setCallbackMouse()
	{
		void(*pMouse)(LPCALLBACKMOUSE pCallback);
		(FARPROC &)pMouse = GetProcAddress(m_hLib, "SetCallbackMouse");
		pMouse(onMouse);
	}

	// Setting callbacks for application events
	void setCallbackMsgProc()
	{
		void(*pMsgProc)(LPCALLBACKMSGPROC pCallback);
		(FARPROC &)pMsgProc = GetProcAddress(m_hLib, "SetCallbackMsgProc");
		pMsgProc(onMsgProc);
	}

	// Setting the callback functions to create Direct3D11 device 
	void setCallbackDeviceCreated()
	{
		void(*pDeviceCreated)(LPCALLBACKDEVICECREATED pCallback);
		(FARPROC &)pDeviceCreated = GetProcAddress(m_hLib, "SetCallbackDeviceCreated");
		pDeviceCreated(onDeviceCreated);
	}

	// Setting the callback function to resize the output scene
	void setCallbackSwapChainResized()
	{
		void(*pSwapChainResized)(LPCALLBACKSWAPCHAINRESIZED pCallback);
		(FARPROC &)pSwapChainResized = GetProcAddress(m_hLib, "SetCallbackSwapChainResized");
		pSwapChainResized(onSwapChainResized);
	}

	// Setting the callback function to resize the output scene (with DirectX devices)
	void setCallbackSwapChainResizedDX()
	{
		void(*pSwapChainResizedDX)(LPCALLBACKSWAPCHAINRESIZEDDX pCallback);
		(FARPROC &)pSwapChainResizedDX = GetProcAddress(m_hLib, "SetCallbackSwapChainResizedDX");
		pSwapChainResizedDX(onSwapChainResizedDX);
	}

	// Setting the callback function to handle the events of the current frame
	void setCallbackFrameMove()
	{
		void(*pFrameMove)(LPCALLBACKFRAMEMOVE pCallback);
		(FARPROC &)pFrameMove = GetProcAddress(m_hLib, "SetCallbackFrameMove");
		pFrameMove(onFrameMove);
	}

	// Setting the callback to render the current frame
	void setCallbackFrameRender()
	{
		void(*pFrameRender)(LPCALLBACKFRAMERENDER pCallback);
		(FARPROC &)pFrameRender = GetProcAddress(m_hLib, "SetCallbackFrameRender");
		pFrameRender(onFrameRender);
	}

	// Setting the callback to render the current frame (with DirectX devices)
	void setCallbackFrameRenderDX()
	{
		void(*pFrameRenderDX)(LPCALLBACKFRAMERENDERDX pCallback);
		(FARPROC &)pFrameRenderDX = GetProcAddress(m_hLib, "SetCallbackFrameRenderDX");
		pFrameRenderDX(onFrameRenderDX);
	}

	// Setting the callback function to release before resizing the output scene
	void setCallbackReleasingSwapChain()
	{
		void(*pSwapChainReleasing)(LPCALLBACKSWAPCHAINRELEASING pCallback);
		(FARPROC &)pSwapChainReleasing = GetProcAddress(m_hLib, "SetCallbackSwapChainReleasing");
		pSwapChainReleasing(onSwapChainReleasing);
	}

	// Setting the callback function to destroy Direct3D11 device
	void setCallbackDeviceDestroyed()
	{
		void(*pDeviceDestroyed)(LPCALLBACKDEVICEDESTROYED pCallback);
		(FARPROC &)pDeviceDestroyed = GetProcAddress(m_hLib, "SetCallbackDeviceDestroyed");
		pDeviceDestroyed(onDeviceDestroyed);
	}

	// Setting the callback function to remove Direct3D11 device 
	void setCallbackDeviceRemoved()
	{
		void(*pDeviceRemoved)(LPCALLBACKDEVICEREMOVED pCallback);
		(FARPROC &)pDeviceRemoved = GetProcAddress(m_hLib, "SetCallbackDeviceRemoved");
		pDeviceRemoved(onDeviceRemoved);
	}

	//-----------------------------------------------------------------------------
	// READ BOOLEAN FROM THE REGISTER ON KEY
	//-----------------------------------------------------------------------------
	HRESULT RegBoolRead(HKEY hKey, WCHAR* strRegName, BOOL* pbDest, BOOL bDefault)
	{
		DWORD dwType;
		DWORD dwLength = sizeof(BOOL);
		if (RegQueryValueEx(hKey, strRegName, 0, &dwType, (BYTE*)pbDest, &dwLength) != ERROR_SUCCESS)
		{
			*pbDest = bDefault;
			if (dwType != REG_DWORD)
				return E_FAIL;
			return S_OK;
		}
		return S_OK;
	}

	//-----------------------------------------------------------------------------
	// WRITE BOOLEAN TO THE REGISTER ON KEY
	//-----------------------------------------------------------------------------
	HRESULT RegBoolWrite(HKEY hKey, WCHAR* strRegName, BOOL bValue)
	{
		if (RegSetValueEx(hKey, strRegName, 0, REG_DWORD, (BYTE*)&bValue, sizeof(BOOL)) != ERROR_SUCCESS)
			return E_FAIL;
		return S_OK;
	}


	//-----------------------------------------------------------------------------
	// READ DWORD FROM THE REGISTER ON KEY
	//-----------------------------------------------------------------------------
	HRESULT RegIntRead(HKEY hKey, WCHAR* strRegName, DWORD* pdwDest, DWORD dwDefault)
	{
		DWORD dwType;
		DWORD dwLength = sizeof(DWORD);
		if (RegQueryValueEx(hKey, strRegName, 0, &dwType, (BYTE*)pdwDest, &dwLength) != ERROR_SUCCESS)
		{
			*pdwDest = dwDefault;
			if (dwType != REG_DWORD)
				return E_FAIL;
			return S_OK;
		}
		return S_OK;
	}


	//-----------------------------------------------------------------------------
	// WRITE DWORD TO THE REGISTER ON KEY
	//-----------------------------------------------------------------------------
	HRESULT RegIntWrite(HKEY hKey, WCHAR* strRegName, DWORD dwValue)
	{
		if (RegSetValueEx(hKey, strRegName, 0, REG_DWORD, (BYTE*)&dwValue, sizeof(DWORD)) != ERROR_SUCCESS)
			return E_FAIL;
		return S_OK;
	}


	//-----------------------------------------------------------------------------
	// READ STRING FROM THE REGISTER ON KEY. Be careful not to pass in sizeof(strDest) on UNICODE builds.
	//-----------------------------------------------------------------------------
	HRESULT RegStrRead(HKEY hKey, WCHAR* strRegName, WCHAR* strDest, DWORD cchDest, WCHAR* strDefault)
	{
		DWORD dwType;
		DWORD cbDest = cchDest * sizeof(WCHAR);
		if (RegQueryValueEx(hKey, strRegName, 0, &dwType, (BYTE*)strDest, &cbDest) != ERROR_SUCCESS)
		{
			strDest = strDefault;
			if (dwType != REG_SZ)
				return E_FAIL;
			return S_OK;
		}
		return S_OK;
	}


	//-----------------------------------------------------------------------------
	// WRITE STRING TO THE REGISTER ON KEY
	//-----------------------------------------------------------------------------
	HRESULT RegStrWrite(HKEY hKey, WCHAR* strRegName, const WCHAR* strValue)
	{
		if (strValue == NULL)	return E_INVALIDARG;
		DWORD cbValue = ((DWORD)wcslen(strValue) + 1) * sizeof(WCHAR);
		if (RegSetValueEx(hKey, strRegName, 0, REG_SZ, (BYTE*)strValue, cbValue) != ERROR_SUCCESS)
			return E_FAIL;
		return S_OK;
	}

	HMODULE getHModule()	{ return m_hLib; }	// Get NetWars.dll

protected:

	HMODULE	m_hLib;	// NetWars.dll
	BOOL	m_bX64;
	ID3D11RenderTargetView* m_pRenderTargetView;
};


//--------------------------------------------------------------------------------------
// TEXTURE CLASS
//--------------------------------------------------------------------------------------
#define	IFF_BMP		0
#define	IFF_JPG		1
#define	IFF_PNG		3
#define	IFF_DDS		4
#define	IFF_TIFF	10
#define	IFF_GIF		11
#define	IFF_WMP		12
//#define	IFF_FORCE_DWORD	0x7fffffff

class CTex
{
public:
	//CTex()	{}	// Constructor
	//~CTex()	{}	// Destructor

	UINT(*add)(LPCWSTR sFile);	// Add the texture from the File
	UINT(*addFromResource)(LPCWSTR sResource);	// Add the texture from Resource
	UINT(*addFromMemory)(LPCVOID pData, SIZE_T size);	// Add the texture from Memory
	UINT(*addDynamic)(UINT width, UINT height);	// Add a blank of texture with support of dynamic changes
	UINT(*addWithDynamic)(LPCWSTR sFile);	// Add the texture from the File with support of dynamic changes
	UINT(*addFromResourceWithDynamic)(LPCWSTR sResource);	// Add the texture from Resource with support of dynamic changes
	UINT(*addFromMemoryWithDynamic)(LPCVOID pData, SIZE_T size);	// Add the texture from Memory with support of dynamic changes
	bool(*update)(UINT idTexture, LPCWSTR sFile);	// Update the texture from the File	// When using the update texture should always make sure that when you restart the D3D device these textures also be opened correctly
	bool(*updateFromResource)(UINT idTexture, LPCWSTR sResource);	// Update the texture from Resource
	bool(*updateFromMemory)(UINT idTexture, LPCVOID pData, SIZE_T size);	// Update the texture from Memory
	bool(*updateDynamic)(UINT idTexture, UINT width, UINT height);	// Update the texture from a blank of texture with support of dynamic changes
	bool(*updateWithDynamic)(UINT idTexture, LPCWSTR sFile);	// Update the texture from the File with support of dynamic changes
	bool(*updateFromResourceWithDynamic)(UINT idTexture, LPCWSTR sResource);	// Update the texture from Resource with support of dynamic changes
	bool(*updateFromMemoryWithDynamic)(UINT idTexture, LPCVOID pData, SIZE_T size);	// Update the texture from Memory with support of dynamic changes
	////// Be sure to add a feature/function loadToMemoryFromFile()

	UINT(*getN)();	// Get number of textures
	bool(*getSize)(UINT idTexture, UINT* iWidth, UINT* iHeight);	// Get the size of the texture
	bool(*getMap)(UINT idTexture, UINT* textureMap);	// Get a pointer to the copy of texture map (bitmap)
	bool(*setMap)(UINT idTexture, UINT* textureMap);	// Set the texture map (bitmap) from the bitmap pointer 
	bool(*setPixel)(UINT idTexture, UINT pixel, UINT color);	// Set the color of pixel to the copy of the texture map (bitmap)
	bool(*updateMap)(UINT idTexture);	// Update the texture from the copy of texture map (bitmap)

	bool(*scale)(UINT idTexture, byte type, UINT newWidth, UINT newHeight);	// Scale existing texture (both dynamic and conventional)
	bool(*addText)(UINT idTexture, UINT x, UINT y, LPCWSTR sText, LPCWSTR sFont, UINT fontHeight, UINT fontWeight, byte type, UINT color);	// Add the text to a texture (X,U position, text, font with height and weight, the type of text (normal, with shadow, with stroke), color)
	bool(*copyPart)(UINT idTexture, RECT* rect, UINT* data);	// Copy bitmap to the texture map as adding a piece of texture
	bool(*changePart)(UINT idTexture, RECT* rect, UINT* data);	// Copy bitmap to the texture map as changing a piece of texture

	bool(*saveToFile)(UINT idTexture, LPCWSTR sFile, UINT format);	// Save texture to the file (file name, file format)
	//UINT(*saveToMemory)(UINT idTexture, byte* bytes, UINT format);	// Save texture to memory (bitmap, file format);  return number of bytes;  byte* = unsigned char*
	//bool(*saveToFile)(UINT idTexture, LPCWSTR sFile, D3DX11_IMAGE_FILE_FORMAT format);	// Save texture to the file (file name, file format);  typedef enum D3DX11_IMAGE_FILE_FORMAT { D3DX11_IFF_BMP = 0, D3DX11_IFF_JPG = 1, D3DX11_IFF_PNG = 3, D3DX11_IFF_DDS = 4, D3DX11_IFF_TIFF = 10, D3DX11_IFF_GIF = 11, D3DX11_IFF_WMP = 12, D3DX11_IFF_FORCE_DWORD = 0x7fffffff }
	UINT(*saveToMemory)(UINT idTexture, byte* bytes, D3DX11_IMAGE_FILE_FORMAT format);	// Save texture to memory (bitmap, file format);  return number of bytes;  byte* = unsigned char*;  typedef enum D3DX11_IMAGE_FILE_FORMAT { D3DX11_IFF_BMP = 0, D3DX11_IFF_JPG = 1, D3DX11_IFF_PNG = 3, D3DX11_IFF_DDS = 4, D3DX11_IFF_TIFF = 10, D3DX11_IFF_GIF = 11, D3DX11_IFF_WMP = 12, D3DX11_IFF_FORCE_DWORD = 0x7fffffff }
	void(*render)(UINT idTexture);	// Set the rendering texture
	void(*renderUI)();	// Set the rendering texture from GUI

	// Work with Clipboard
	bool(*copyToClipboard)(UINT idDynamicTexture, RECT* rcSelect, bool bTransparency);	// Copy the selection rect to the clipboard texture
	bool(*copyMapToClipboard)(UINT* textureMap, UINT iWidth, UINT iHeight, bool bTransparency);	// Copy the texture map (bitmap) to the clipboard texture
	bool(*pasteFromClipboard)(UINT idDynamicTexture, RECT* rcSelect, bool bTransparency);	// Paste an image from the clipboard texture to the rect of texture map;  false - error reading clipboard;  not yet support non 32-bit image files
	bool(*changeFromClipboard)(UINT idDynamicTexture, RECT* rcSelect, bool bTransparency);	// Change an image from the clipboard texture to the rect of texture map;  false - error reading clipboard;  not yet support non 32-bit image files
	UINT(*getClipboardSize)();	// Gets the texture size in pixels only if it's the clipboard texture (0 - free buffer bitmap)
	UINT(*pasteMapFromClipboard)(UINT* textureMap);	// Paste an image from the clipboard texture to the bitmap;  return texture width in pixels (0 - error reading clipboard)

	void setHModule(HMODULE hLib)	// Set NetWars.dll
	{
		m_hLib = hLib;

		(FARPROC &)add = GetProcAddress(m_hLib, "addTexture");	// Add the texture from the File
		(FARPROC &)addFromResource = GetProcAddress(m_hLib, "addTextureFromResource");	// Add the texture from Resource
		(FARPROC &)addFromMemory = GetProcAddress(m_hLib, "addTextureFromMemory");	// Add the texture from Memory
		(FARPROC &)addDynamic = GetProcAddress(m_hLib, "addTextureDynamic");	// Add a blank of texture with support of dynamic changes
		(FARPROC &)addWithDynamic = GetProcAddress(m_hLib, "addTextureWithDynamic");	// Add the texture from the File with support of dynamic changes
		(FARPROC &)addFromResourceWithDynamic = GetProcAddress(m_hLib, "addTextureFromResourceWithDynamic");	// Add the texture from Resource with support of dynamic changes
		(FARPROC &)addFromMemoryWithDynamic = GetProcAddress(m_hLib, "addTextureFromMemoryWithDynamic");	// Add the texture from Memory with support of dynamic changes
		(FARPROC &)update = GetProcAddress(m_hLib, "updateTexture");	// Update the texture from the File
		(FARPROC &)updateFromResource = GetProcAddress(m_hLib, "updateTextureFromResource");	// Update the texture from Resource
		(FARPROC &)updateFromMemory = GetProcAddress(m_hLib, "updateTextureFromMemory");	// Update the texture from Memory
		(FARPROC &)updateDynamic = GetProcAddress(m_hLib, "updateTextureDynamic");	// Update the texture from a blank of texture with support of dynamic changes
		(FARPROC &)updateWithDynamic = GetProcAddress(m_hLib, "updateTextureWithDynamic");	// Update the texture from the File with support of dynamic changes
		(FARPROC &)updateFromResourceWithDynamic = GetProcAddress(m_hLib, "updateTextureFromResourceWithDynamic");	// Update the texture from Resource with support of dynamic changes
		(FARPROC &)updateFromMemoryWithDynamic = GetProcAddress(m_hLib, "updateTextureFromMemoryWithDynamic");	// Update the texture from Memory with support of dynamic changes

		(FARPROC &)getN = GetProcAddress(m_hLib, "getNTextures");	// Get number of textures
		(FARPROC &)getSize = GetProcAddress(m_hLib, "getTextureSize");	// Get the size of the texture
		(FARPROC &)getMap = GetProcAddress(m_hLib, "getTextureMap");	// Get a pointer to the copy of texture map (bitmap)
		(FARPROC &)setMap = GetProcAddress(m_hLib, "setTextureMap");	// Set the texture map (bitmap) from the bitmap pointer 
		(FARPROC &)setPixel = GetProcAddress(m_hLib, "setTexturePixel");	// Set the color of pixel to the copy of the texture map (bitmap)
		(FARPROC &)updateMap = GetProcAddress(m_hLib, "updateTextureMap");	// Update the texture from the copy of texture map (bitmap)
		(FARPROC &)scale = GetProcAddress(m_hLib, "scaleTexture");	// Scale existing texture (both dynamic and conventional)
		(FARPROC &)addText = GetProcAddress(m_hLib, "addTextToTexture");	// Add the text to a texture (X,U position, text, font with height and weight, the type of text (normal, with shadow, with stroke), color)
		(FARPROC &)copyPart = GetProcAddress(m_hLib, "copyTexturePart");	// Copy bitmap to the texture map as adding a piece of texture
		(FARPROC &)changePart = GetProcAddress(m_hLib, "changeTexturePart");	// Copy bitmap to the texture map as changing a piece of texture


		(FARPROC &)saveToFile = GetProcAddress(m_hLib, "saveTextureToFile");	// Save texture to the file
		(FARPROC &)saveToMemory = GetProcAddress(m_hLib, "saveTextureToMemory");	// Save texture to memory
		(FARPROC &)render = GetProcAddress(m_hLib, "setTexture");	// Set the rendering texture
		(FARPROC &)renderUI = GetProcAddress(m_hLib, "setTextureUI");	// Set the rendering texture from GUI

		// Work with the Clipboard
		(FARPROC &)copyToClipboard = GetProcAddress(m_hLib, "copyToClipboard");	// Copy the selection rect to the clipboard texture
		(FARPROC &)copyMapToClipboard = GetProcAddress(m_hLib, "copyMapToClipboard");	// Copy the texture map (bitmap) to the clipboard texture
		(FARPROC &)pasteFromClipboard = GetProcAddress(m_hLib, "pasteFromClipboard");;	// Paste an image from the clipboard texture to the rect of texture map
		(FARPROC &)changeFromClipboard = GetProcAddress(m_hLib, "changeFromClipboard");	// Change an image from the clipboard texture to the rect of texture map
		(FARPROC &)getClipboardSize = GetProcAddress(m_hLib, "getClipboardSize");	// Gets the texture size in pixels only if it's the clipboard texture (0 - free buffer bitmap)
		(FARPROC &)pasteMapFromClipboard = GetProcAddress(m_hLib, "pasteMapFromClipboard");	// Paste an image from the clipboard texture to the bitmap;  return texture width in pixels (0 - error reading clipboard)
	}

protected:

	HMODULE	m_hLib;	// NetWars.dll
};


//--------------------------------------------------------------------------------------
// 2D CLASS
//--------------------------------------------------------------------------------------
#define	RENDER_ONLY_TEXTURE			0	// Only texture
#define	RENDER_ONLY_COLOR			1	// Only color
#define	RENDER_TEXTURE_MULTI_COLOR	2	// Texture multiplied by color
#define	RENDER_TEXTURE_PLUS_COLOR	3	// Texture plus color

class C2D
{
public:
	//C2D()	{}	// Constructor
	//~C2D()	{}	// Destructor
	
	void(*setType)(BYTE iType);	// Set the type of sprite rendering: RENDER_ONLY_TEXTURE = 0, RENDER_ONLY_COLOR = 1, RENDER_TEXTURE_MULTI_COLOR = 2, RENDER_TEXTURE_PLUS_COLOR = 3
	void(*setColor)(const float *color);	// Set the color for sprite rendering with color: RENDER_ONLY_COLOR = 1, RENDER_TEXTURE_MULTI_COLOR = 2, RENDER_TEXTURE_PLUS_COLOR = 3
	void(*setTextureSize)(UINT iWidth, UINT iHeight);	// Set the texture size in pixels for correctly addition sprites
	UINT(*add)(int iXb, int iXe, int iYb, int iYe, float fXb, float fXe, float fYb, float fYe, float fD);	// Add the sprite (Begin and End X,Y texture coordinates of the sprite in pixels from 0, 0 to texture size; Begin and End X,Y coordinates display sprite from 0.0f, 0.0f to width, height; depth)
	UINT(*addEx)(int iXb, int iXe, int iYb, int iYe, int iTX, int iTY, float fXb, float fXe, float fYb, float fYe, float fD);	// Add the sprite (Begin and End X,Y texture coordinates of the sprite in pixels from 0, 0 to texture size; X,Y texture size in pixels; Begin and End X,Y coordinates display sprite from 0.0f, 0.0f to width, height; depth)
	UINT(*addF)(float tXb, float tXe, float tYb, float tYe, float fXb, float fXe, float fYb, float fYe, float fD);	// Add the sprite (Begin and End X,Y texture coordinates of the sprite from 0.0f, 0.0f to 1.0f, 1.0f; Begin and End X,Y coordinates display sprite from 0.0f, 0.0f to width, height; depth)
	UINT(*addEmpty)(UINT n);	// Add empty sprites for future updates
	bool(*update)(UINT idSprite, int iXb, int iXe, int iYb, int iYe, float fXb, float fXe, float fYb, float fYe, float fD);	// Update the sprite (Begin and End X,Y texture coordinates of the sprite in pixels from 0, 0 to texture size; Begin and End X,Y coordinates display sprite from 0.0f, 0.0f to width, height; depth)
	bool(*updateEx)(UINT idSprite, int iXb, int iXe, int iYb, int iYe, float fTX, float fTY, float fXb, float fXe, float fYb, float fYe, float fD);	// Update the sprite (Begin and End X,Y texture coordinates of the sprite in pixels from 0, 0 to texture size; X,Y texture size in pixels; Begin and End X,Y coordinates display sprite from 0.0f, 0.0f to width, height; depth)
	bool(*updateF)(UINT idSprite, float tXb, float tXe, float tYb, float tYe, float fXb, float fXe, float fYb, float fYe, float fD);	// Update the sprite (Begin and End X,Y texture coordinates of the sprite from 0.0f, 0.0f to 1.0f, 1.0f; Begin and End X,Y coordinates display sprite from 0.0f, 0.0f to width, height; depth)
	bool(*pos4)(UINT idSprite, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);	// Update only sprite position (X,Y-coordinates for 4 points of sprite) 
	void(*resetBuffer)();	// Reset the sprite buffer (if there is a change of buffer size / adding or deleting sprite)
	void(*updateBuffer)();	// Update the sprite buffer (if there is no changes of buffer size)
	void(*draw)(UINT idStart, UINT nCount);	// Draw the sprite (Id of initial sprite, number of sprites for rendering)

	void setHModule(HMODULE hLib)	// Set NetWars.dll
	{
		m_hLib = hLib;

		(FARPROC &)setColor = GetProcAddress(hLib, "setRenderSpriteColor");	// Set the type of sprite rendering
		(FARPROC &)setType = GetProcAddress(hLib, "setRenderSpriteType");	// Set the color for sprite rendering with color
		(FARPROC &)setTextureSize = GetProcAddress(hLib, "setTextureSize");	// Set the texture size in pixels for correctly addition sprites
		(FARPROC &)add = GetProcAddress(hLib, "addSprite");	// Add the sprite
		(FARPROC &)addEx = GetProcAddress(hLib, "addSpriteEx");	// Add the sprite (extended)
		(FARPROC &)addF = GetProcAddress(hLib, "addSpriteF");	// Add the sprite (with 0.0f, 0.0f - 1.0f, 1.0f texture coordinates)
		(FARPROC &)addEmpty = GetProcAddress(hLib, "addSpriteEmpty");	// Add empty sprites for future updates
		(FARPROC &)update = GetProcAddress(hLib, "updateSprite");	// Update the sprite
		(FARPROC &)updateEx = GetProcAddress(hLib, "updateSpriteEx");	// Update the sprite (extended)
		(FARPROC &)updateF = GetProcAddress(hLib, "updateSpriteF");	// Update the sprite (with 0.0f, 0.0f - 1.0f, 1.0f texture coordinates)
		(FARPROC &)pos4 = GetProcAddress(hLib, "posSprite4");	// Update only sprite position
		(FARPROC &)resetBuffer = GetProcAddress(hLib, "resetSpriteBuffer");	// Reset the sprite buffer
		(FARPROC &)updateBuffer = GetProcAddress(hLib, "updateSpriteBuffer");	// Update the sprite buffer
		(FARPROC &)draw = GetProcAddress(hLib, "drawSprite");	// Draw the sprite
	}

protected:

	HMODULE	m_hLib;	// NetWars.dll
};


//--------------------------------------------------------------------------------------
// TEXT CLASS
//--------------------------------------------------------------------------------------
#define	TEXT_NORMAL	0	// Normal text
#define	TEXT_SHADOW	1	// Text with shadow
#define	TEXT_STROKE	2	// Text with stroke

#define	COLOR_ALPHA	0x00000000	// Transparent color
#define	COLOR_WHITE	0xffffffff	// White color
#define	COLOR_BLACK	0xff000000	// Black color
#define	COLOR_GREY	0xffcccccc	// Grey color
#define	COLOR_RED	0xff0000ff	// Red color
#define	COLOR_GREEN	0xff00ff00	// Green color
#define	COLOR_BLUE	0xffff0000	// Blue color
#define	COLOR_LBLUE	0xffff8080	// Light blue color
#define	COLOR_GOLD	0xff00e0ff	// Gold color
#define	COLOR_LINK	0xffff8c5a	// Hyperlink color
#define	COLOR_ORANGE 0xff0080ff	// Orange color
#define	COLOR_YELLOW 0xff00ffff	// Yellow color

class CText
{
public:
	//CText()	{}	// Constructor
	//~CText()	{}	// Destructor

	UINT(*add)(UINT idFont, LPCWSTR str, UINT format, UINT color, float fDepth);	// Add text
	UINT(*addShadow)(UINT idFont, LPCWSTR str, UINT format, UINT color, float fDepth);	// Add text with shadow
	UINT(*addStroke)(UINT idFont, LPCWSTR str, UINT format, UINT color, UINT colorStroke, float fDepth);	// Add text with stroke
	UINT(*addEx)(UINT idFont, LPCWSTR str, UINT format, UINT color, float fDepth, int width, int spacing, int indent);	// Add formatted text
	UINT(*addSpace)(UINT idFont, UINT type, int len, UINT format, float fDepth);	// Add space for dynamic text
	//UINT(*add3D)()	// Add text to be displayed in 3D, such as the name of the player in the MMO
	bool(*pos)(UINT idText, int iX, int iY);	// Set X,Y text position
	bool(*posA)(UINT idText, int iX, int iY, float fAlpha);	// Set X,Y text position with angle clockwise rotation
	bool(*posEx)(UINT idText, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);	// Set extended text position (X,Y coordinates of 4 points)
	void(*draw)(UINT idStart, UINT nCount);	// Draw text (Id of initial sprite of text, number of text sprites for rendering)
	void(*dynamic)(UINT idFont, UINT idDynamic, BYTE type, BYTE format, UINT color, LPCWSTR str);	// Dynamic changes of text
	void(*dynamicEx)(UINT idFont, UINT idDynamic, BYTE type, BYTE dwFormat, UINT color, LPCWSTR str, int iX, int iY);	// Dynamic changes of text with changes text height, use updateBuffer() atfter
	void(*fps)(UINT idFont, UINT idDynamic, UINT format, UINT color);	// Dynamic changes of FPS text
	void(*timer)(UINT idFont, UINT idDynamic, UINT format, UINT color);	// Dynamic changes of Timer text
	float(*getSizeX)(UINT idText);	// Text size on the X axis
	float(*getSizeY)(UINT idText);	// Text size on the Y axis
	void(*resetBuffer)();	// Reset text sprite buffer (if there is a change of buffer size / adding or deleting sprite of text)
	void(*updateBuffer)();	// Update text sprite buffer (if there is no changes of buffer size)

	void setHModule(HMODULE hLib)	// Set NetWars.dll
	{
		m_hLib = hLib;

		(FARPROC &)add = GetProcAddress(hLib, "addText");	// Add text
		(FARPROC &)addShadow = GetProcAddress(hLib, "addTextShadow");	// Add text with shadow
		(FARPROC &)addStroke = GetProcAddress(hLib, "addTextStroke");	// Add text with stroke
		(FARPROC &)addEx = GetProcAddress(hLib, "addTextEx");	// Add formatted text
		(FARPROC &)addSpace = GetProcAddress(hLib, "addTextSpace");	// Add space for dynamic text
		(FARPROC &)pos = GetProcAddress(hLib, "posText");	// Set X,Y text position
		(FARPROC &)posA = GetProcAddress(hLib, "posTextA");	// Set X,Y text position with angle clockwise rotation
		(FARPROC &)posEx = GetProcAddress(hLib, "posTextEx");	// Set extended text position (X,Y coordinates of 4 points)
		(FARPROC &)draw = GetProcAddress(hLib, "drawText");	// Draw text (Id of initial sprite of text, number of text sprites for rendering)
		(FARPROC &)dynamic = GetProcAddress(hLib, "dynamic");	// Dynamic changes of text
		(FARPROC &)dynamicEx = GetProcAddress(hLib, "dynamicEx");	// Dynamic changes of text with changes text height, use updateBuffer() atfter
		(FARPROC &)fps = GetProcAddress(hLib, "fps");	// Dynamic changes of FPS text
		(FARPROC &)timer = GetProcAddress(hLib, "timer");	// Dynamic changes of Timer text
		(FARPROC &)getSizeX = GetProcAddress(hLib, "getTextSizeX");	// Text size on the X axis
		(FARPROC &)getSizeY = GetProcAddress(hLib, "getTextSizeY");	// Text size on the Y axis
		(FARPROC &)resetBuffer = GetProcAddress(hLib, "resetTextBuffer");	// Reset text sprite buffer
		(FARPROC &)updateBuffer = GetProcAddress(hLib, "updateTextBuffer");	// Update text sprite buffer
	}

protected:

	HMODULE	m_hLib;	// NetWars.dll
};





//--------------------------------------------------------------------------------------
// GUI CLASS
//--------------------------------------------------------------------------------------
#define	EVENT_CLICK		0	// Clicked button Event: Window (exit), Message, Button
#define	EVENT_MOVE		1	// Moved or Selection control Event: Window, Message
#define	EVENT_SELECT	2	// Selection item Event: ListBox (is fired off when the selection changes in a single selection list box), ComboBox
#define	EVENT_CHANGE	3	// Changed Event: RadioButton, CheckBox, Slider
#define	EVENT_ENTER		4	// Pressed enter Event: EditBox
#define	EVENT_UPDATE	5	// Update data Event: Slider
#define	EVENT_EDIT		6	// Edit Event: EditBox (is sent when the editbox content changes due to user input)
#define EVENT_RCLICK	7	// Right clicked mouse Event: ListBox, ComboBox
#define	EVENT_DBLCLK	8	// Double clicked mouse Event: ListBox
#define	EVENT_RDBLCLK	9	// Right double clicked mouse Event: ListBox, ComboBox
#define	EVENT_COPY		10	// Copyed to windows buffer Event: ListBox, ComboBox
#define	EVENT_SCROLL	11	// Scrolling
#define	EVENT_CHART		12	// Change scale in the chart
#define	EVENT_TOOLTIP	13	// Visual tooltip

#define	STATE_NORMAL	0	// default 0xffffffff
#define	STATE_DISABLED	1	// default 0xffffffff
#define	STATE_HIDDEN	2	// default 0xffffffff
#define	STATE_FOCUS		3	// default 0xffffffff
#define	STATE_MOUSEOVER	4	// default 0xffffffff
#define	STATE_PRESSED	5	// default 0xffffffff
#define	HYPER_FOCUS		6	// default 0xffa06010 Color for hyperlinks
#define	HYPER_MOUSEOVER	7	// default 0xff0080ff Color for hyperlinks
#define	HYPER_PRESSED	8	// default 0xff800080 Color for hyperlinks

#define	BUTTON_YELLOW	0	// Yellow button
#define	BUTTON_GREEN	1	// Green button
#define	BUTTON_RED		2	// Red button
#define	BUTTON_ORANGE	3	// Orange button

/// In ont place - purpose functions
/// In another - type of supported components

class CUI
{
public:
	//CUI()	{}	// Constructor
	//~CUI()	{}	// Destructor

	UINT(*addWindow)(UINT idFont, LPCWSTR str, int width, int height, UINT color, bool bMoved, float fDepth);	// Add Window: font, title text, width, height, title text color, permission flag move, depth
	UINT(*addMessage)(UINT idFont, LPCWSTR str, LPCWSTR strButton, int width, int height, UINT color, bool bMoved, float fDepth);	// Add Message: font, string text, button text, width, height, string text color, permission flag move, depth
	UINT(*addStatic)(int idParentWindow, UINT idFont, LPCWSTR str, UINT format, UINT color, float fDepth);	// Add Static Text String: id of parent window (-1 - without parent), font id, string text, display format, string text color, depth
	UINT(*addEditBox)(int idParentWindow, UINT idFont, LPCWSTR str, int width, float fDepth);	// Add EditBox: id of parent window (-1 - without parent), font id, string text, width, depth
	UINT(*addListBox)(int idParentWindow, UINT idFont, int width, UINT nStr, float fDepth);	// Add ListBox: id of parent window (-1 - without parent), font id, width, number of strings in the page, depth
	UINT(*addComboBox)(int idParentWindow, UINT idFont, int width, float fDepth);	// Add ComboBox: id of parent window (-1 - without parent), font id, width, depth
	UINT(*addScrollBar)(int idParentWindow, UINT nStr, UINT nStrTotal, int width, int height, float fDepth);	// Add ScrollBar: id of parent window (-1 - without parent), number of strings in the page, number of strings (total), width, height, depth 
	UINT(*addButton)(int idParentWindow, UINT idFont, LPCWSTR str, int width, int color, float fDepth);	// Add Button: id of parent window (-1 - without parent), font id, string text, width, button color, depth
	UINT(*addCreepLine)(int idParentWindow, UINT idFont, LPCWSTR str, int width, UINT color, float fDepth);	// Add CreepLine: id of parent window (-1 - without parent), font id, string text, width, string text color, depth
	UINT(*addSlider)(int idParentWindow, int width, int height, int min, int max, int value, float fDepth);	// Add Slider (without text/font): id of parent window (-1 - without parent), width, height, minimum value, maximum value, current value, depth
	UINT(*addCheckBox)(int idParentWindow, UINT idFont, LPCWSTR str, UINT color, bool bChecked, float fDepth);	// Add CheckBox: id of parent window (-1 - without parent), font id, string text, string text color, flag of checking/pressing, depth
	UINT(*addLink)(int idParentWindow, UINT idFont, LPCWSTR str, UINT color, float fDepth);	// Add Hyperlink: id of parent window (-1 - without parent), font id, string text, string text color, depth
	UINT(*addRadioButton)(int idParentWindow, UINT idFont, int iGroup, LPCWSTR str, UINT color, float fDepth);	// Add RadioButton: id of parent window (-1 - without parent), font id, group id, string text, string text color, depth
	UINT(*addChart)(int idParentWindow, UINT idFont, LPCWSTR str, int width, int height, float fDepth);	// Add Chart: id of parent window (-1 - without parent), font id, title text, width, height, depth
	UINT(*addTreeView)(int idParentWindow, UINT idFont, int width, UINT nStr, float fDepth);	// Add TreeView: id of parent window (-1 - without parent), font id, width, number of strings in the page, depth
	UINT(*addGrid)(int idParentWindow, UINT idFont, LPCWSTR str, int width, int height, float fDepth);	// Add Grid: id of parent window (-1 - without parent), font id, title text, width, height, depth
	UINT(*addTabControl)(int idParentWindow, UINT idFont, LPCWSTR str, int width, int height, float fDepth);	// Add TabControl: id of parent window (-1 - without parent), font id, title text, width, height, depth 
	UINT(*addFileDialog)(UINT idFont, LPCWSTR str, int width, int height, float fDepth);	// Add FileDialog: font id, title text, width, height, depth

	void(*render)();	// Render

	bool(*setParentWindow)(UINT idUI, int idParentWindow);	// Set parent window (all except Window, Message, FileDialog)
	void(*setDepth)(UINT idUI, float fDepth);	// Set depth (all elements)
	bool(*setStateColor)(UINT idUI, UINT iState, UINT color);	// Set the color for the GUI control state (all elements)
	void(*text)(UINT idUI, LPCWSTR str);	// Set text (all except ListBox, ComboBox, ScrollBar, Slider)
	void(*textMsg)(UINT idUI, LPCWSTR str1, LPCWSTR str2, LPCWSTR str3, LPCWSTR str4);	// Set text for the message (only Message)
	void(*addItem)(UINT idUI, LPCWSTR str, int data);	// Add item (ListBox, ComboBox, Tree, Message): data - it's additional numbers stored in the item
	void(*insItem)(UINT idUI, int index, LPCWSTR str, int data);	// Insert item using index (ListBox, ComboBox, Tree): data - it's additional numbers stored in the item
	void(*updateItem)(UINT idUI, int index, LPCWSTR str, int data);	// Update item using index (ListBox, ComboBox, Tree): data - it's additional numbers stored in the item
	void(*selItem)(UINT idUI, int index);	// Select item using index (ListBox, ComboBox, Tree)
	void(*pos)(UINT idUI, int x, int y);	// Set position (all controls)
	void(*posEx)(UINT idUI, int x, int y, int w, int h);	// Set position and size (all controls)
	void(*visible)(UINT idUI, bool bVisible);	// Set visible (all controls)
	bool(*getVisible)(UINT idUI);	// Get visible (all controls)
	void(*enable)(UINT idUI, bool bEnable);	// Set enable (all controls)
	bool(*getEnable)(UINT idUI);	// Get enable (all controls)
	void(*setMaxSymbols)(UINT idUI, UINT nMaxSymbols);	// Set a maximum symbols (EditBox)
	void(*setMaxPixels)(UINT idUI, UINT nMaxPixels);	// Set a maximum width of the string in pixels (EditBox)
	void(*setHide)(UINT idUI, bool bHide);	// Set hide text, asterisks instead of text (EditBox)
	void(*setIP)(UINT idUI, bool bIP);	// Set IP text, allow only input IP-address (EditBox)
	void(*setInt)(UINT idUI, UINT nMax);	// Set Integer text, allow only input Integer (EditBox): the maximum Integer
	void(*setFloat)(UINT idUI, UINT nDot, UINT nMax);	// Set Float text, allow only input Float (EditBox): the number of digits after the decimal point, the maximum allowed value
	bool(*checkMail)(LPCWSTR sMail);	// Check the mail address
	int(*getSelIndex)(UINT idUI);	// Get selected index (ListBox, ComboBox, Tree)
	int(*getSelectedData)(UINT idUI);	// Get selected data - additional numbers stored in the item (ListBox, ComboBox, Tree)
	int(*getData)(UINT idUI, UINT index);	// Get data using index - additional numbers stored in the item (ListBox, ComboBox, Tree)

	////// Add the return of the number from string, as a whole and fractional

	void(*set3Column)(UINT idUI, int width1, int width2);	// Set 3 column (ListBox)
	void(*addItem3)(UINT idUI, LPCWSTR str1, LPCWSTR str2, LPCWSTR str3, int data);	// Add item (ListBox): data - it's additional numbers stored in the item
	void(*insertItem3)(UINT idUI, int index, LPCWSTR str1, LPCWSTR str2, LPCWSTR str3, int data);	// Insert item using index (ListBox): data - it's additional numbers stored in the item
	void(*updateItem3)(UINT idUI, int index, LPCWSTR str1, LPCWSTR str2, LPCWSTR str3, int data);	// Update item using index (ListBox): data - it's additional numbers stored in the item
	bool(*deleteAllItems)(UINT idUI);	// Delete all items (ListBox, ComboBox, Tree)
	void(*deleteItem)(UINT idUI, int index);	// Delete item using index (ListBox, ComboBox, Tree)
	int(*getSize)(UINT idUI);	// Get size (ListBox, ComboBox, Tree): -1 - error
	LPCWSTR(*getText)(UINT idUI);	// Get text (EditBox)
	LPCWSTR(*getIndexText)(UINT idUI, int index);	// Get text using index (ListBox, ComboBox, Tree)
	LPCWSTR(*getIndexText2)(UINT idUI, int index);	// Get text from second column using index (ListBox)
	LPCWSTR(*getIndexText3)(UINT idUI, int index);	// Get text from third column using index (ListBox)
	LPCWSTR(*getIndexText4)(UINT idUI, int index);	// Get text from fourth column using index (ListBox)
	LPCWSTR(*getSelText)(UINT idUI);	// Get selected text (ListBox, ComboBox, Tree, EditBox)
	LPCWSTR(*getSelText2)(UINT idUI);	// Get selected text from second column (ListBox)
	LPCWSTR(*getSelText3)(UINT idUI);	// Get selected text from third column (ListBox)
	LPCWSTR(*getSelText4)(UINT idUI);	// Get selected text from fourth column (ListBox)
	void(*setIndexText)(UINT idUI, int index, LPCWSTR str);	// Set text using index (ListBox, ComboBox, Tree)
	void(*setIndexText2)(UINT idUI, int index, LPCWSTR str);	// Set text from second column using index (ListBox)
	void(*setIndexText3)(UINT idUI, int index, LPCWSTR str);	// Set text from third column using index (ListBox)
	void(*setIndexText4)(UINT idUI, int index, LPCWSTR str);	// Set text from fourth column using index (ListBox)
	void(*setChecked)(UINT idUI, bool bChecked);	// Set checked (CheckBox, RadioButton)
	bool(*getChecked)(UINT idUI);	// Get checked (CheckBox, RadioButton)
	void(*setCross)(UINT idUI, bool bCross);	// Establish a "cross" instead of "tick" (CheckBox)
	void(*setConstColor)(UINT idUI, bool bConstColor);	// Flag of permanence for text color (CheckBox, RadioButton)
	UINT(*getScrollPos)(UINT idUI);	// Get the position of ScrollBar, id of the first visible line in the list (ScrollBar, ListBox, ComboBox, Tree)
	bool(*setScrollSize)(UINT idUI, UINT size);	// Set ScrollBar track size (ScrollBar)
	bool(*setScrollPageSize)(UINT idUI, UINT size);	// Set ScrollBar page size (ScrollBar)
	bool(*setMaxTextWidth)(UINT idUI, UINT maxWidth);	// Set the maximum text width in pixels, 0 - unlimited (Static, Button, CheckBox, RadioButton)
	
	//void(*clear)(UINT idUI);

	UINT(*addSeries)(UINT idUI, UINT color, UINT nMemoryValue, float fValue);	// Add Uniform (X: 0, 1, 2, 3, ..., N) series/graph to the Chart (Chart)
	UINT(*addSeriesEx)(UINT idUI, UINT color, UINT nMemoryValue, float fXValue, float fYValue);	// Add Random (set X value) series/graph to the Chart (Chart)
	UINT(*addPoint)(UINT idUI, UINT idSeries, float fValue);	// Add point to the series (Chart)
	bool(*setPoint)(UINT idUI, UINT idSeries, UINT idValue, float fValue);	// Set new value in the point (Chart)

	bool(*setBackGround)(UINT idUI, bool background);	// Set a flag of displaying background, default = true (Chart, ListBox, EditBox)
	bool(*setBorder)(UINT idUI, bool bBorder);	// Set a flag of displaying the boundaries of graphs and numerical values, default = true (Chart)
	bool(*setBarLines)(UINT idUI, bool bBarLines);	// Set a flag of displaying bar lines, default = true (Chart)
	bool(*setSelectEnable)(UINT idUI, bool bSelectEnable);	// Set a flag of enabling selection, default = true (Chart)
	bool(*setVerticalLine)(UINT idUI, bool bVerticalLine);	// Set a flag of displaying vertical line, default = true (Chart)
	bool(*setVerticalText)(UINT idUI, bool bVerticalText);	// Set a flag of displaying text for vertical line, default = true (Chart)
	bool(*setBackGroundColor)(UINT idUI, UINT color);	// Set background color, default = COLOR_WHITE (Chart)
	bool(*setBorderColor)(UINT idUI, UINT color);	// Set border color, default = COLOR_BLACK (Chart)
	bool(*setBarLinesColor)(UINT idUI, UINT color);	// Set bar lines color, default = COLOR_GREY 0xffb3b3b3 (Chart)
	bool(*setSelectColor)(UINT idUI, UINT color);	// Set color of selection, default = COLOR_BLACK (Chart)
	bool(*setVerticalColor)(UINT idUI, UINT color);	// Set color of vertical line and its text, default = COLOR_GREEN (Chart)
	bool(*setTitleColor)(UINT idUI, UINT color);	// Set title color, default = COLOR_BLACK (Chart)
	bool(*setXTextColor)(UINT idUI, UINT color);	// Set color for X axis, default = COLOR_BLACK (Chart)
	bool(*setYTextColor)(UINT idUI, UINT color);	// Set color for Y axis, default = COLOR_BLACK (Chart)
	bool(*setXText)(UINT idUI, LPCWSTR sXText);	// Set text for X axis (Chart)
	bool(*setYText)(UINT idUI, LPCWSTR sYText);	// Set text for Y axis (Chart)

	void setHModule(HMODULE hLib)	// Set NetWars.dll
	{
		m_hLib = hLib;

		(FARPROC &)addWindow = GetProcAddress(hLib, "addWindow");	// Add Window
		(FARPROC &)addMessage = GetProcAddress(hLib, "addMessage");	// Add Message
		(FARPROC &)addStatic = GetProcAddress(hLib, "addStatic");	// Add Static Text String
		(FARPROC &)addEditBox = GetProcAddress(hLib, "addEditBox");	// Add EditBox
		(FARPROC &)addListBox = GetProcAddress(hLib, "addListBox");	// Add ListBox
		(FARPROC &)addComboBox = GetProcAddress(hLib, "addComboBox");	// Add ComboBox
		(FARPROC &)addScrollBar = GetProcAddress(hLib, "addScrollBar");	// Add ScrollBar
		(FARPROC &)addButton = GetProcAddress(hLib, "addButton");	// Add Button
		(FARPROC &)addCreepLine = GetProcAddress(hLib, "addCreepLine");	// Add CreepLine
		(FARPROC &)addSlider = GetProcAddress(hLib, "addSlider");	// Add Slider
		(FARPROC &)addCheckBox = GetProcAddress(hLib, "addCheckBox");	// Add CheckBox
		(FARPROC &)addLink = GetProcAddress(hLib, "addLink");	// Add Hyperlink
		(FARPROC &)addRadioButton = GetProcAddress(hLib, "addRadioButton");	// Add RadioButton
		(FARPROC &)addChart = GetProcAddress(hLib, "addChart");	// Add Chart
		(FARPROC &)addTreeView = GetProcAddress(hLib, "addTreeView");	// Add TreeView
		(FARPROC &)addGrid = GetProcAddress(hLib, "addGrid");	// Add Grid
		(FARPROC &)addTabControl = GetProcAddress(hLib, "addTabControl");	// Add TabControl
		(FARPROC &)addFileDialog = GetProcAddress(hLib, "addFileDialog");	// Add FileDialog

		(FARPROC &)render = GetProcAddress(hLib, "RenderUI");	// Render

		(FARPROC &)setParentWindow = GetProcAddress(hLib, "setParentWindow");	// Set parent window
		(FARPROC &)setDepth = GetProcAddress(hLib, "setDepth");	// Set depth
		(FARPROC &)setStateColor = GetProcAddress(hLib, "setStateColor");	// Set the color for the GUI control state
		(FARPROC &)text = GetProcAddress(hLib, "text");	// Set text 
		(FARPROC &)textMsg = GetProcAddress(hLib, "textMsg");	// Set text for the message
		(FARPROC &)addItem = GetProcAddress(hLib, "addString");	// Add item
		(FARPROC &)insItem = GetProcAddress(hLib, "insString");	// Insert item using index
		(FARPROC &)updateItem = GetProcAddress(hLib, "updateString");	// Update item using index
		(FARPROC &)selItem = GetProcAddress(hLib, "selString");	// Select item using index
		(FARPROC &)pos = GetProcAddress(hLib, "pos");	// Set position
		(FARPROC &)posEx = GetProcAddress(hLib, "posEx");	// Set position and size
		(FARPROC &)visible = GetProcAddress(hLib, "visible");	// Set visible
		(FARPROC &)getVisible = GetProcAddress(hLib, "getVisible");	// Get visible
		(FARPROC &)enable = GetProcAddress(hLib, "enable");	// Set enable
		(FARPROC &)getEnable = GetProcAddress(hLib, "getEnable");	// Get enable
		(FARPROC &)setMaxSymbols = GetProcAddress(hLib, "setMaxSymbols");	// Set a maximum symbols
		(FARPROC &)setMaxPixels = GetProcAddress(hLib, "setMaxPixels");	// Set a maximum width of the string in pixels
		(FARPROC &)setHide = GetProcAddress(hLib, "setHide");	// Set hide text, asterisks instead of text
		(FARPROC &)setIP = GetProcAddress(hLib, "setIP");	// Set IP text, allow only input IP-address
		(FARPROC &)setInt = GetProcAddress(hLib, "setInt");	// Set Integer text, allow only input Integer
		(FARPROC &)setFloat = GetProcAddress(hLib, "setFloat");	// Set Float text, allow only input Float
		(FARPROC &)checkMail = GetProcAddress(hLib, "checkMail");	// Check the mail address
		(FARPROC &)getSelIndex = GetProcAddress(hLib, "getSelIndex");	// Get selected index
		(FARPROC &)getSelectedData = GetProcAddress(hLib, "getSelectedData");	// Get selected data - additional numbers stored in the item
		(FARPROC &)getData = GetProcAddress(hLib, "getData");	// Get data using index - additional numbers stored in the item

		(FARPROC &)set3Column = GetProcAddress(hLib, "set3Column");	// Set 3 column
		(FARPROC &)addItem3 = GetProcAddress(hLib, "addItem3");	// Add item
		(FARPROC &)insertItem3 = GetProcAddress(hLib, "insertItem3");	// Insert item using index
		(FARPROC &)updateItem3 = GetProcAddress(hLib, "updateItem3");	// Update item using index
		(FARPROC &)deleteAllItems = GetProcAddress(hLib, "deleteAllItems");	// Delete all items
		(FARPROC &)deleteItem = GetProcAddress(hLib, "deleteItem");	// Delete item using index
		(FARPROC &)getSize = GetProcAddress(hLib, "getSize");	// Get size
		(FARPROC &)getText = GetProcAddress(hLib, "getText");	// Get text
		(FARPROC &)getIndexText = GetProcAddress(hLib, "getIndexText");	// Get text using index
		(FARPROC &)getIndexText2 = GetProcAddress(hLib, "getIndexText2");	// Get text from second column using index
		(FARPROC &)getIndexText3 = GetProcAddress(hLib, "getIndexText3");	// Get text from third column using index
		(FARPROC &)getIndexText4 = GetProcAddress(hLib, "getIndexText4");	// Get text from fourth column using index
		(FARPROC &)getSelText = GetProcAddress(hLib, "getSelText");	// Get selected text
		(FARPROC &)getSelText2 = GetProcAddress(hLib, "getSelText2");	// Get selected text from second column
		(FARPROC &)getSelText3 = GetProcAddress(hLib, "getSelText3");	// Get selected text from third column
		(FARPROC &)getSelText4 = GetProcAddress(hLib, "getSelText4");	// Get selected text from fourth column
		(FARPROC &)setIndexText = GetProcAddress(hLib, "setIndexText");	// Set text using index
		(FARPROC &)setIndexText2 = GetProcAddress(hLib, "setIndexText2");	// Set text from second column using index
		(FARPROC &)setIndexText3 = GetProcAddress(hLib, "setIndexText3");	// Set text from third column using index
		(FARPROC &)setIndexText4 = GetProcAddress(hLib, "setIndexText4");	// Set text from fourth column using index
		(FARPROC &)setChecked = GetProcAddress(hLib, "setChecked");	// Set checked
		(FARPROC &)getChecked = GetProcAddress(hLib, "getChecked");	// Get checked
		(FARPROC &)setCross = GetProcAddress(hLib, "setCross");	// Establish a "cross" instead of "tick"
		(FARPROC &)setConstColor = GetProcAddress(hLib, "setConstColor");	// Flag of permanence for text color
		(FARPROC &)getScrollPos = GetProcAddress(hLib, "getScrollPos");	// Get the position of ScrollBar, id of the first visible line in the list
		(FARPROC &)setScrollSize = GetProcAddress(hLib, "setScrollSize");	// Set ScrollBar track size
		(FARPROC &)setScrollPageSize = GetProcAddress(hLib, "setScrollPageSize");	// Set ScrollBar page size (ScrollBar)
		(FARPROC &)setMaxTextWidth = GetProcAddress(hLib, "setMaxTextWidth");	// Set the maximum text width in pixels, 0 - unlimited 
		//(FARPROC &)clear = GetProcAddress(hLib, "clear");

		(FARPROC &)addSeries = GetProcAddress(hLib, "addSeriesUniform");	// Add Uniform (X: 0, 1, 2, 3, ..., N) series/graph to the Chart
		(FARPROC &)addSeriesEx = GetProcAddress(hLib, "addSeriesRandom");	// Add Random (set X value) series/graph to the Chart
		(FARPROC &)addPoint = GetProcAddress(hLib, "addPoint");	// Add point to the series
		(FARPROC &)setPoint = GetProcAddress(hLib, "setPoint");	// Set new value in the point

		(FARPROC &)setBackGround = GetProcAddress(hLib, "setBackGround");	// Set a flag of displaying background, default = true
		(FARPROC &)setBorder = GetProcAddress(hLib, "setBorder");	// Set a flag of displaying the boundaries of graphs and numerical values, default = true
		(FARPROC &)setBarLines = GetProcAddress(hLib, "setBarLines");	// Set a flag of displaying bar lines, default = true
		(FARPROC &)setSelectEnable = GetProcAddress(hLib, "setSelectEnable");	// Set a flag of enabling selection, default = true
		(FARPROC &)setVerticalLine = GetProcAddress(hLib, "setVerticalLine");	// Set a flag of displaying vertical line, default = true
		(FARPROC &)setVerticalText = GetProcAddress(hLib, "setVerticalText");	// Set a flag of displaying text for vertical line, default = true
		(FARPROC &)setBackGroundColor = GetProcAddress(hLib, "setBackGroundColor");	// Set background color, default = COLOR_WHITE
		(FARPROC &)setBorderColor = GetProcAddress(hLib, "setBorderColor");	// Set border color, default = COLOR_BLACK
		(FARPROC &)setBarLinesColor = GetProcAddress(hLib, "setBarLinesColor");	// Set bar lines color, default = COLOR_GREY 0xffb3b3b3
		(FARPROC &)setSelectColor = GetProcAddress(hLib, "setSelectColor");	// Set color of selection, default = COLOR_BLACK
		(FARPROC &)setVerticalColor = GetProcAddress(hLib, "setVerticalColor");	// Set color of vertical line and its text, default = COLOR_GREEN
		(FARPROC &)setTitleColor = GetProcAddress(hLib, "setTitleColor");	// Set title color, default = COLOR_BLACK
		(FARPROC &)setXTextColor = GetProcAddress(hLib, "setXTextColor");	// Set color for X axis, default = COLOR_BLACK
		(FARPROC &)setYTextColor = GetProcAddress(hLib, "setYTextColor");	// Set color for Y axis, default = COLOR_BLACK
		(FARPROC &)setXText = GetProcAddress(hLib, "setXText");	// Set text for X axis
		(FARPROC &)setYText = GetProcAddress(hLib, "setYText");	// Set text for Y axis (Chart)
	}

	void setNameTextureUI(wchar_t* sName)
	{
		void(*pUI)(wchar_t* s);
		(FARPROC &)pUI = GetProcAddress(m_hLib, "setNameTextureUI");
		pUI(sName);
	}

	void setSkinEdit(RECT* rc, RECT* rcText, RECT* rcCaret, RECT* rcCombo, RECT* rcSel, int nBorder)
	{
		void(*pSkinEdit)(RECT* rc, RECT* rcText, RECT* rcCaret, RECT* rcCombo, RECT* rcSel, int nBorder);
		(FARPROC &)pSkinEdit = GetProcAddress(m_hLib, "setSkinEdit");
		pSkinEdit(rc, rcText, rcCaret, rcCombo, rcSel, nBorder);
	}

	void setSkinBar(RECT* rc, RECT* rcTrack, RECT* rcThumb, RECT* rcCen)
	{
		void(*pSkinBar)(RECT* rc, RECT* rcTrack, RECT* rcThumb, RECT* rcCen);
		(FARPROC &)pSkinBar = GetProcAddress(m_hLib, "setSkinBar");
		pSkinBar(rc, rcTrack, rcThumb, rcCen);
	}

	void setSkinSlider(RECT* rcLine, RECT* rcThumb)
	{
		void(*pSkinBar)(RECT* rcLine, RECT* rcThumb);
		(FARPROC &)pSkinBar = GetProcAddress(m_hLib, "setSkinSlider");
		pSkinBar(rcLine, rcThumb);
	}

	void setSkinCheckRadioTree(RECT* rcCheck, RECT* rcCheck1, RECT* rcCheck2, RECT* rcRadio, RECT* rcRadio1, RECT* rcTree, RECT* rcTree1)
	{
		void(*pSkinCheckRadioTree)(RECT* rcCheck, RECT* rcCheck1, RECT* rcCheck2, RECT* rcRadio, RECT* rcRadio1, RECT* rcTree, RECT* rcTree1);
		(FARPROC &)pSkinCheckRadioTree = GetProcAddress(m_hLib, "setSkinCheckRadioTree");
		pSkinCheckRadioTree(rcCheck, rcCheck1, rcCheck2, rcRadio, rcRadio1, rcTree, rcTree1);
	}

	void setSkinButton(RECT* rcButton)
	{
		void(*pSkinButton)(RECT* rcButton);
		(FARPROC &)pSkinButton = GetProcAddress(m_hLib, "setSkinButton");
		pSkinButton(rcButton);
	}

	void setSkinWindow(RECT* rcWindow)
	{
		void(*pSkinWindow)(RECT* rcWindow);
		(FARPROC &)pSkinWindow = GetProcAddress(m_hLib, "setSkinWindow");
		pSkinWindow(rcWindow);
	}

protected:

	HMODULE	m_hLib;	// NetWars.dll
};


//--------------------------------------------------------------------------------------
// SOUND CLASS
//--------------------------------------------------------------------------------------
class CSound
{
public:
	//CSound()	{}	// Constructor
	//~CSound()	{}	// Destructor

	UINT(*add)(LPWSTR sSound);	// Add Sound (the path to the audio file)
	UINT(*addEx)(LPWSTR sSound, BYTE nThreads);	// Add Sound with the maximum number of threads for simultaneous playback of this sound, 1 or 0 - single-threaded playback
	void(*play)(DWORD idSound);	// Play Sound
	void(*stop)(DWORD idSound);	// Stop Sound
	void(*addToPlay)(DWORD idSound, DWORD idSoundAdd);	// Add sound to the end of playback (playable sound, added sound)
	void(*playWithReset)(DWORD idSound);	// Play Sound with reset sound device
	void(*volume)(float fVolume);	// Set volume from 0.0f to 1.0f (default = 0.5f)

	void setHModule(HMODULE hLib)	// Set NetWars.dll
	{
		m_hLib = hLib;

		(FARPROC &)add = GetProcAddress(hLib, "addSound");	// Add Sound
		(FARPROC &)addEx = GetProcAddress(hLib, "addSoundEx");	// Add Sound with the maximum number of threads for simultaneous playback of this sound
		(FARPROC &)play = GetProcAddress(hLib, "playSound");	// Play Sound
		(FARPROC &)stop = GetProcAddress(hLib, "stopSound");	// Stop Sound
		(FARPROC &)addToPlay = GetProcAddress(hLib, "addSoundToPlay");	// Add sound to the end of playback
		(FARPROC &)playWithReset = GetProcAddress(hLib, "playSoundWithReset");	// Play Sound with reset sound device
		(FARPROC &)volume = GetProcAddress(hLib, "setVolumeSound");	// Set volume from 0.0f to 1.0f
	}

protected:

	HMODULE	m_hLib;	// NetWars.dll
};


//--------------------------------------------------------------------------------------
// MEDIA CLASS
//--------------------------------------------------------------------------------------
class CMedia
{
public:
	//CMedia()	{}	// Constructor
	//~CMedia()	{}	// Destructor

	void(*init)();	// Media initialization
	void(*draw)();	// Render the video stream
	bool(*check)(LPCWSTR sFile);	// Check, is this the media file
	UINT(*add)(LPCWSTR sFile);	// Add media (the path to the media file)
	void(*sprite)(float left, float right, float top, float bottom, float depth);	// Media sprite position (rect and depth)
	void(*stretch)(bool stretch);	// Flag of stretching render the video to full sprite
	void(*prepare)(int index);	// Prepare for playback
	void(*run)();	// Start playback
	void(*pause)();	// Pause playback
	void(*pos)(int iPlay);	// Set the playback position
	void(*volume)(float fVolume);	// Set the playback volume
	int(*getPosition)(LPWSTR sPos);	// Get the playback position
	int(*getDuration)(LPWSTR sDur);	// Get the duration of the media

	void setHModule(HMODULE hLib)	// Set NetWars.dll
	{
		m_hLib = hLib;

		(FARPROC &)init = GetProcAddress(hLib, "initMedia");	// Media initialization
		(FARPROC &)draw = GetProcAddress(hLib, "drawMedia");	// Render the video stream
		(FARPROC &)check = GetProcAddress(hLib, "checkMedia");	// Check, is this the media file
		(FARPROC &)add = GetProcAddress(hLib, "addMedia");	// Add media
		(FARPROC &)sprite = GetProcAddress(hLib, "posMedia");	// Media sprite position
		(FARPROC &)stretch = GetProcAddress(hLib, "stretchMedia");	// Flag of stretching render the video to full sprite
		(FARPROC &)prepare = GetProcAddress(hLib, "prepareMedia");	// Prepare for playback
		(FARPROC &)run = GetProcAddress(hLib, "runMedia");	// Start playback
		(FARPROC &)pause = GetProcAddress(hLib, "pauseMedia");	// Pause playback
		(FARPROC &)pos = GetProcAddress(hLib, "SetPositionMedia");	// Set the playback position
		(FARPROC &)volume = GetProcAddress(hLib, "setVolumeMedia");	// Set the playback volume
		(FARPROC &)getPosition = GetProcAddress(hLib, "getMediaPosition");	// Get the playback position
		(FARPROC &)getDuration = GetProcAddress(hLib, "getMediaDuration");	// Get the duration of the media
	}

protected:

	HMODULE	m_hLib;	// NetWars.dll
};


//--------------------------------------------------------------------------------------
// 3D CLASS
//--------------------------------------------------------------------------------------
// XZ - plane XZ, a plane of the world (A=0 - north, =Pi/2 - east, =Pi - south, =-Pi/2 - west
// Y - the height of the world

#define	TOPOLOGY_POINTLIST		1	// Render point list
#define	TOPOLOGY_LINELIST		2	// Render line  list
#define	TOPOLOGY_TRIANGLELIST	4	// Render triangle list

/*typedef enum D3D11_PRIMITIVE_TOPOLOGY {
	D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED = 0,
	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST = 1,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST = 2,
	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP = 3,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = 5,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ = 10,
	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ = 11,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ = 12,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ = 13,
	D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST = 33,
	D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST = 34,
	D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST = 35,
	D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST = 36,
	D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST = 37,
	D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST = 38,
	D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST = 39,
	D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST = 40,
	D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST = 41,
	D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST = 42,
	D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST = 43,
	D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST = 44,
	D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST = 45,
	D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST = 46,
	D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST = 47,
	D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST = 48,
	D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST = 49,
	D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST = 50,
	D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST = 51,
	D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST = 52,
	D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST = 53,
	D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST = 54,
	D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST = 55,
	D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST = 56,
	D3D11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST = 57,
	D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST = 58,
	D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST = 59,
	D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST = 60,
	D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST = 61,
	D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST = 62,
	D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST = 63,
	D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST = 64
} D3D11_PRIMITIVE_TOPOLOGY;*/

class C3D
{
public:
	//C3D()	{}	// Constructor
	//~C3D()	{}	// Destructor

	void(*init)(float R, float A, float B, float Rmin, float Rmax, float Bmin, float Bmax);	// Initialize work with 3D standard camera (the distance from the camera to the center of the observation, angle of camera direction in the plane XZ, angle of camera direction in the plane XY or ZY, minimum and maximum distance, minimum and maximum angle in the plane XY or ZY)
	//void(*setCamera)(byte iType);	// Set type of camera
	//void(*setPosition)(float x, float y, float z);	// Set position of camera
	void(*setRender3D)();	// Set the context device for rendering 3D
	UINT(*addMesh)(LPCWSTR sMesh);	// Add Mesh
	void(*renderMesh)(UINT idMesh, float x, float y, float z);	// Rendering the mesh without texture (using preset texture)
	void(*renderMeshTex)(UINT idMesh, float x, float y, float z);	// Rendering the mesh with texture
	void(*renderMeshTexA)(UINT idMesh, float x, float y, float z, float a);	// Rendering the mesh with texture and rotation in the plane XZ (world plane)
	void(*renderMeshSubset)(UINT idMesh, UINT idSubset, float x, float y, float z);	// Rendering the subset of the mesh
	void(*renderMeshSubsetRotateX)(UINT idMesh, UINT idSubset, float x, float y, float z, float a);	// Rendering the subset of the mesh and rotation in the plane XZ (world plane)
	void(*renderMeshSubsetInit)(UINT idMesh);	// Initialization of rendering subsets of the mesh (necessary before rendering the subset of the mesh)
	UINT(*addMeshAnim)(LPWSTR sMesh, LPWSTR sAnim);	// Add Mesh with Animation
	void(*renderMeshAnim)(UINT idMesh, float x, float y, float z, float a);	// Render Animated Mesh with Position and rotation in the plane XZ (world plane)
	void(*renderMeshAnimScale)(UINT idMesh, float x, float y, float z, float a, float s);	// Render Mesh with Position, rotation in the plane XZ (world plane) and scaling
	////// ADD rotation, scaling, SETTING WORLD MATRIX
	// ADD rendering with preset of world matrix
	// void(*renderMesh)(UINT idMesh, D3DXMATRIX mWorld);	// Render Mesh with preset of world matrix

	UINT(*addVertex)(void* pResourceData, UINT nBytes);	// Add vertex buffer
	void(*updateVertex)(UINT idVertex, void* pResourceData, UINT nBytes);	// Update vertex buffer
	//void(*resetVertex)(UINT idVertex, void* pResourceData);	// Reset vertex buffer, update without resizing
	void(*renderVertex)(UINT idVertex, float x, float y, float z);	// Render vertex buffer in XYZ position
	UINT(*addIndex)(void* pResourceData, UINT nBytes);	// Add WORD type index buffer (2 bytes per index, <= 65535 indexes)
	UINT(*addIndex4)(void* pResourceData, UINT nBytes);	// Add DWORD type index buffer (4 bytes per index, > 65535 indexes)
	void(*updateIndex)(UINT idVertex, void* pResourceData, UINT nBytes);	// Update WORD type index buffer (2 bytes per index, <= 65535 indexes)
	void(*updateIndex4)(UINT idVertex, void* pResourceData, UINT nBytes);	// Update DWORD type index buffer (4 bytes per index, > 65535 indexes)
	//void(*resetIndex)(UINT idVertex, void* pResourceData, UINT nBytes);	// Reset WORD type index buffer, update without resizing (2 bytes per index, <= 65535 indexes)
	//void(*resetIndex4)(UINT idVertex, void* pResourceData, UINT nBytes);	// Reset DWORD type index buffer, update without resizing (4 bytes per index, > 65535 indexes)
	void(*renderIndex)(UINT idVertex, UINT idIndex, float x, float y, float z);	// Render WORD type index buffer in XYZ position
	void(*renderIndex4)(UINT idVertex, UINT idIndex, float x, float y, float z);	// Render DWORD type index buffer in XYZ position
	//void(*setPrimitiveTopology)(D3D11_PRIMITIVE_TOPOLOGY iTopology);	// Set primitive topology: D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	void(*setPrimitiveTopology)(UINT iTopology);	// Set primitive topology: TOPOLOGY_POINTLIST, TOPOLOGY_LINELIST, TOPOLOGY_TRIANGLELIST

	// Section of Mathematical Functions
	void(*getScreenCoordFrom3DPoint)(float fX, float fY, float fZ, int* iX, int* iY);	// Get the screen coordinates of a point in world;  [in] fX, fY, fZ - XYZ position;  [out] iX, iY - screen coordinates
	void(*getRayDirectionFromScreenPoint)(int iX, int iY, float* fX, float* fY, float* fZ);	// Get ray direction in the world using screen coordinates;  [in] iX, iY - screen coordinates;  [out] fX, fY, fZ - ray direction
	bool(*get3DCoordFromScreenPoint)(float fX0, float fY0, float fZ0, float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2, int iX, int iY, float* fX, float* fY, float* fZ);	// Get the coordinates of the intersection of the plane and the observation beam;  [in] fX0, fY0, fZ0, fX0, fY0, fZ0, fX0, fY0, fZ0 - the three points of intersection of the plane;  [in] iX, iY - the screen coordinates for the observation beam;  [out] fX, fY, fZ - the coordinates of the intersection
	//bool(*get3DCoordFromScreenPoint)(D3DXVECTOR3* v, int iX, int iY, float* fX, float* fY, float* fZ);	// Get the coordinates of the intersection of the plane and the observation beam;  [in] v[3] - the three points of intersection of the plane;  [in] iX, iY - the screen coordinates for the observation beam;  [out] fX, fY, fZ - the coordinates of the intersection
	void(*getPerpPlane)(D3DXVECTOR3* v, D3DXVECTOR3* vNor, float dist);	// Get the perpendicular plane to the center of the observation and normal to the perpendicular plane;  [out] *v - the three points of the perpendicular plane;  [out] *vNor - normal to the perpendicular plane;  [in] dist - the distance to the perpendicular plane
	bool(*get3DCoordForPerpPlane)(int iX, int iY, float dist, float* fX, float* fY, float* fZ, D3DXVECTOR3* vNor);	// Get the perpendicular plane to the observation point;  [in] iX, iY - the screen coordinates for the observation beam;  [in] dist - the distance to the perpendicular plane [out] *fX, *fY, *fZ - the three points of the perpendicular plane;  [out] *vNor - normal to the perpendicular plane
	bool(*pick)(float fX0, float fY0, float fZ0, float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2, int iX, int iY);	// Pick, return the intersection of the triangle and the observation beam;  [in] fX0, fY0, fZ0, fX1, fY1, fZ1, fX2, fY2, fZ2 - three points of the triangle;  [in] iX, iY - the screen coordinates for the observation beam
	int(*pickN)(int N, WORD* indexes, float* fX, float* fY, float* fZ, int iX, int iY);	// Pick, return the index of first intersection of the triangle and the observation beam;  -1 - non intersection;  [in] N - number of triangles;  [in] *indexes - indexes of triangles;  [in] *fX, *fY, *fZ - coordinates of the triangle points;  [in] iX, iY - the screen coordinates for the observation beam

	void setHModule(HMODULE hLib)	// Set NetWars.dll
	{
		m_hLib = hLib;

		(FARPROC &)init = GetProcAddress(hLib, "init3D");	// Initialize work with 3D standard camera
		//(FARPROC &)setCamera = GetProcAddress(hLib, "setCameraType");	// Set type of camera
		//(FARPROC &)setPosition = GetProcAddress(hLib, "setCameraPosition");	// Set position of camera
		(FARPROC &)setRender3D = GetProcAddress(hLib, "setRender3D");
		(FARPROC &)addMesh = GetProcAddress(hLib, "addMesh");	// Add Mesh
		(FARPROC &)renderMesh = GetProcAddress(hLib, "renderMesh");	// Rendering the mesh without texture
		(FARPROC &)renderMeshTex = GetProcAddress(hLib, "renderMeshTex");	// Rendering the mesh with texture
		(FARPROC &)renderMeshTexA = GetProcAddress(hLib, "renderMeshTexA");	// Rendering the mesh with texture and rotation in the plane XZ
		(FARPROC &)renderMeshSubset = GetProcAddress(hLib, "renderMeshSubset");	// Rendering the subset of the mesh
		(FARPROC &)renderMeshSubsetRotateX = GetProcAddress(hLib, "renderMeshSubsetRotateX");	// Rendering the subset of the mesh and rotation in the plane XZ
		(FARPROC &)renderMeshSubsetInit = GetProcAddress(hLib, "renderMeshSubsetInit");	// Initialization of rendering subsets of the mesh
		(FARPROC &)addMeshAnim = GetProcAddress(hLib, "addMeshAnim");	// Add Mesh with Animation
		(FARPROC &)renderMeshAnim = GetProcAddress(hLib, "renderMeshAnim");	// Render Mesh with Position and Rotation
		(FARPROC &)renderMeshAnimScale = GetProcAddress(hLib, "renderMeshAnimScale");	// Render Mesh with Position, Rotation and Scale

		(FARPROC &)addVertex = GetProcAddress(hLib, "addVertex");		// Add a vertex buffer with 3D mesh
		//(FARPROC &)resetVertex = GetProcAddress(hLib, "resetVertex");	// Reset vertex buffer
		(FARPROC &)updateVertex = GetProcAddress(hLib, "updateVertex");	// Update vertex buffer
		(FARPROC &)renderVertex = GetProcAddress(hLib, "renderVertex");	// Render 3D mesh from buffer
		(FARPROC &)addIndex = GetProcAddress(hLib, "addIndex");			// Add an index buffer for 3D mesh (2 bytes per index, NOT more than 65536 vertexes)
		(FARPROC &)addIndex4 = GetProcAddress(hLib, "addIndex4");		// Add an index buffer for 3D mesh (4 bytes per index, more than 65536 vertexes)
		//(FARPROC &)resetIndex = GetProcAddress(hLib, "resetIndex");	// Reset index buffer
		//(FARPROC &)resetIndex4 = GetProcAddress(hLib, "resetIndex4");	// Reset index buffer
		(FARPROC &)updateIndex = GetProcAddress(hLib, "updateIndex");	// Update index buffer
		(FARPROC &)updateIndex4 = GetProcAddress(hLib, "updateIndex4");	// Update index buffer
		(FARPROC &)renderIndex = GetProcAddress(hLib, "renderIndex");	// Render 3D mesh from buffer
		(FARPROC &)renderIndex4 = GetProcAddress(hLib, "renderIndex4");	// Render 3D mesh from buffer
		(FARPROC &)setPrimitiveTopology = GetProcAddress(hLib, "setPrimitiveTopology");	// Set primitive topology

		// Section of Mathematical Functions
		(FARPROC &)getScreenCoordFrom3DPoint = GetProcAddress(hLib, "getScreenCoordFrom3DPoint");	// Get the screen coordinates of a point in world
		(FARPROC &)getRayDirectionFromScreenPoint = GetProcAddress(hLib, "getRayDirectionFromScreenPoint");	// Get ray direction in the world using screen coordinates
		(FARPROC &)get3DCoordFromScreenPoint = GetProcAddress(hLib, "get3DCoordFromScreenPoint");	// Get the coordinates of the intersection of the plane and the observation beam
		(FARPROC &)getPerpPlane = GetProcAddress(hLib, "getPerpPlane");	// Get the perpendicular plane to the center of the observation and normal to the perpendicular plane
		(FARPROC &)get3DCoordForPerpPlane = GetProcAddress(hLib, "get3DCoordForPerpPlane");	// Get the perpendicular plane to the observation point
		(FARPROC &)pick = GetProcAddress(hLib, "pick");	// Pick, return the intersection of the triangle and the observation beam
		(FARPROC &)pickN = GetProcAddress(hLib, "pickN");	// Pick, return the index of first intersection of the triangle and the observation beam
	}

protected:

	HMODULE	m_hLib;	// NetWars.dll
};


//--------------------------------------------------------------------------------------
// CAMERA CLASS
//--------------------------------------------------------------------------------------
#define	CAMERA_MANUAL		0	// Default camera;  Processing change of camera - hand-held camera (self-installation vectors: position of eye, viewing direction and orientation of view)
#define	CAMERA_SPHERE		1	// The sphere camera for for viewing and editing 3D models
#define	CAMERA_CIRCULAR		2	// The circular camera for card and board games
#define	CAMERA_MAPEDITOR	3	// The map editor camera for 3D WYSIWYG (What You See Is What You Get) map editing, or 3D strategy games
#define	CAMERA_FIRSTPERSON	4	// The camera for first person games (shooter, role-playing, race, etc.)
#define	CAMERA_THIRDPERSON	5	// The camera for third person games (shooter, role-playing, race, etc.)

////// Add function to work with view matrix and projection matrix, vectors of camera position, positions of observation and rotation camera

class CCam
{
public:
	//CCam()	{}	// Constructor
	//~CCam()	{}	// Destructor

	void(*setType)(byte iType);	// Set type of camera
	void(*setPosition)(float x, float y, float z);	// Set position of camera

	void(*getMatrixViewProj)(D3DXMATRIX* mVP);	// Get Matrix View Projection
	void(*getMatrixView)(D3DXMATRIX* mV);	// Get Matrix View
	void(*getMatrixProj)(D3DXMATRIX* mP);	// Get Matrix Projection
	void(*getEye)(D3DXVECTOR3* vEye);	// Get eye/camera position
	void(*getAt)(D3DXVECTOR3* vAt);	// Get eye/camera direction
	void(*getUp)(D3DXVECTOR3* vUp);	// Get vector indicating the top
	//setEye	D3DXVECTOR3	g_Eye;	// Set eye/camera position
	//setAt	D3DXVECTOR3	g_At;		// Set eye/camera direction
	//setUp	D3DXVECTOR3	g_Up;		// Set vector indicating the top
	//getViewInverse				// Get Matrix View Inverse

	void setHModule(HMODULE hLib)	// Set NetWars.dll
	{
		m_hLib = hLib;

		(FARPROC &)setType = GetProcAddress(hLib, "setCameraType");	// Set type of camera
		(FARPROC &)setPosition = GetProcAddress(hLib, "setCameraPosition");	// Set position of camera

		(FARPROC &)getMatrixViewProj = GetProcAddress(m_hLib, "getMatrixViewProj");	// Get Matrix View Projection
		(FARPROC &)getMatrixView = GetProcAddress(m_hLib, "getMatrixView");	// Get Matrix View
		(FARPROC &)getMatrixProj = GetProcAddress(m_hLib, "getMatrixProj");	// Get Matrix Projection
		(FARPROC &)getEye = GetProcAddress(m_hLib, "getEye");	// Get eye/camera position
		(FARPROC &)getAt = GetProcAddress(m_hLib, "getAt");	// Get eye/camera direction
		(FARPROC &)getUp = GetProcAddress(m_hLib, "getUp");	// Get vector indicating the top
	}

protected:

	HMODULE	m_hLib;	// NetWars.dll
};


//--------------------------------------------------------------------------------------
// PHYSICS CLASS
//--------------------------------------------------------------------------------------
#define	OBJ_MAP		0	// Type of map/world border object (id: 0 - left/xMin, 1 - right/xMax, 2 - groung/down/yMin, 3 - sky/up/yMax, 4 - back/zMin, 5 - forward/zMax)
#define	OBJ_QUAD	1	// Type of quad object
#define	OBJ_PERS	2	// Type of personage object

// Contact of Bullets
void CALLBACK onBullet(WORD idBullet, byte typeObject, WORD idObject, float x, float y, float z);
typedef void (CALLBACK *LPCALLBACKBULLET)(WORD idBullet, byte typeObject, WORD idObject, float x, float y, float z);

class CPhys
{
public:
	//CPhys()	{}	// Constructor
	//~CPhys()	{}	// Destructor

	UINT(*addQuad)(float x1, float z1, float x2, float z2, float x3, float z3, float x4, float z4, float y1, float y2);	// Add 4-angle static object;  [in] x1, z1, x2, z2, x3, z3, x4, z4 - 4 XZ-coordinates;  [in] y1, y2 - bottom and top
	UINT(*addStatic)(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);	// Add a static object (building, wall, outdoor TV, etc.);  [in] xmin, xmax, ymin, ymax, zmin, zmax - the object boundaries (AABB)
	UINT(*addDynamic)(float x, float y, float z, float r, float nx, float ny, float nz, float speed, float accel);	// Add a dynamic object (car, grenade before the explosion);  [in] x, y, z - world position;  [in] r - radius;  [in] nx, ny, nz - norrmal direction vector;  [in] speed, accel - speed, acceleration
	UINT(*addPers)(float x, float y, float z, float r, float h, byte health);	// Add person (including other players and computer bots);  [in] x, y, z - world position;  [in] r, h - radius, height;  [in] health - health;  direction vector, speed, acceleration, etc. set later
	UINT(*addBullet)(float x, float y, float z, float nx, float ny, float nz, byte damage);	// Add bullet;  [in] x, y, z - world position;  [in] nx, ny, nz - direction vector with speed;  [in] damage - damage
	//UINT(*addGrenade)();	// Add explosion of grenade;  [in] x, y, z - position;   the damage, radius of damage, time of explosion

	bool(*setIDOwnPers)(WORD idPers);	// Set id of own person (usually - zero)
	WORD(*getIDOwnPers)();	// Get id of own person (usually - zero)
	WORD(*getNPers)();	// Get number of person
	WORD(*getNAPers)();	// Get number of active person
	bool(*setPersActive)(WORD idPers, bool bActive);	// Set person activity
	bool(*getPersActive)(WORD idPers);	// Get person activity
	float(*getPersX)(WORD idPers);	// Get the X-coordinate of the person
	float(*getPersY)(WORD idPers);	// Get the Y-coordinate of the person
	float(*getPersZ)(WORD idPers);	// Get the Z-coordinate of the person
	float(*getPersNA)(WORD idPers);	// Get the person rotation angle in the XZ plane (direction of sight)
	float(*getPersSA)(WORD idPers);	// Get the person movement angle in the XZ plane (direction of movement)
	bool(*getPersMotionNormal)(WORD idPers, float *sx, float *sy, float *sz);	// Get XYZ-normal of person movement
	bool(*getPersViewNormal)(WORD idPers, float *nx, float *ny, float *nz);	// Get XYZ-normal of person sight
	float(*getPersSX)(WORD idPers);	// Get the speed of the person on the X axis
	float(*getPersSY)(WORD idPers);	// Get the speed of the person on the Y axis
	float(*getPersSZ)(WORD idPers);	// Get the speed of the person on the Z axis
	float(*getPersNX)(WORD idPers);	// Get X-normal of person sight
	float(*getPersNY)(WORD idPers);	// Get Y-normal of person sight
	float(*getPersNZ)(WORD idPers);	// Get Z-normal of person sight

	bool(*setPersHealth)(WORD idPers, byte health);	// Set the percent of person health
	bool(*addPersHealth)(WORD idPers, byte health);	// Add the percent of person health
	byte(*getPersHealth)(WORD idPers);	// Get the percent of person health

	bool(*setBulletSpeed)(UINT idBullet, float speed);	// Set bullet speed
	bool(*addBulletTime)(UINT idBullet, float time);	// Add time for bullet

	void(*objectProcessing)();	// Processing interaction of all objects
	bool(*setCameraAfterObjectProcessing)();	// Adjust the position of the camera, using the processing results of the physical engine

	bool(*getBulletDirectionForThirdPersonCamera)(float x, float y, float z, float* nx, float* ny, float* nz);	// Get the bullet direction for third party camera;  [in] x, y, z - the person position;  [in/out] *nx, *ny, *nz - the normal of camera direction / normal of bullet direction

	bool(*setPersFrameMove)(WORD idPers, float nx, float ny, float nz, float na, float sx, float sy, float sz, float sa, float speed, float accel, float fTime);	// Set every frame peron parameters;  [in] nx, ny, nz, na - XYZ-normal of sight with direction;  [in] sx, sy, sz, sa - XYZ-normal of person movement with direction;  [in] speed, accel - speed, acceleration;  [in] fTime - elapsed time (frame time)
	bool(*setPersPosFromServer)(WORD idPers, float x, float y, float z, float na);	// Set person positon, using data from server (correction of person positon);  [in] x, y, z, na - world position with direction of sight
	bool(*setPersSpeedType)(WORD idPers, BYTE iSpeed);	// Set the type of speed
	BYTE(*getPersSpeedType)(WORD idPers);	// Get the type of speed

	void setHModule(HMODULE hLib)	// Set NetWars.dll
	{
		m_hLib = hLib;

		(FARPROC &)addQuad = GetProcAddress(hLib, "addObjectQuad");	// Add 4-angle static object
		(FARPROC &)addStatic = GetProcAddress(hLib, "addObjectStatic");	// Add a static object 
		(FARPROC &)addDynamic = GetProcAddress(hLib, "addObjectDynamic");	// Add a dynamic object
		(FARPROC &)addPers = GetProcAddress(hLib, "addObjectPers");	// Add person
		(FARPROC &)addBullet = GetProcAddress(hLib, "addObjectBullet");	// Add bullet
		//(FARPROC &)addGrenade = GetProcAddress(hLib, "addObjectGrenade");	// Add explosion of grenade

		(FARPROC &)setIDOwnPers = GetProcAddress(hLib, "setIDOwnPers");	// Set id of own person
		(FARPROC &)getIDOwnPers = GetProcAddress(hLib, "getIDOwnPers");	// Get id of own person

		(FARPROC &)getNPers = GetProcAddress(hLib, "getNPers");	// Get number of person
		(FARPROC &)getNAPers = GetProcAddress(hLib, "getNAPers");	// Get number of active person

		(FARPROC &)setPersActive = GetProcAddress(hLib, "setPersActive");	// Set person activity
		(FARPROC &)getPersActive = GetProcAddress(hLib, "getPersActive");	// Get person activity
		(FARPROC &)getPersX = GetProcAddress(hLib, "getPersX");	// Get the X-coordinate of the person
		(FARPROC &)getPersY = GetProcAddress(hLib, "getPersY");	// Get the Y-coordinate of the person
		(FARPROC &)getPersZ = GetProcAddress(hLib, "getPersZ");	// Get the Z-coordinate of the person
		(FARPROC &)getPersNA = GetProcAddress(hLib, "getPersNA");	// Get the person rotation angle in the XZ plane
		(FARPROC &)getPersSA = GetProcAddress(hLib, "getPersSA");	// Get the person movement angle in the XZ plane
		(FARPROC &)getPersMotionNormal = GetProcAddress(hLib, "getPersMotionNormal");	// Get XYZ-normal of person movement
		(FARPROC &)getPersViewNormal = GetProcAddress(hLib, "getPersViewNormal");	// Get XYZ-normal of person sight
		(FARPROC &)getPersSX = GetProcAddress(hLib, "getPersSX");	// Get the speed of the person on the X axis
		(FARPROC &)getPersSY = GetProcAddress(hLib, "getPersSY");	// Get the speed of the person on the Y axis
		(FARPROC &)getPersSZ = GetProcAddress(hLib, "getPersSZ");	// Get the speed of the person on the Z axis
		(FARPROC &)getPersNX = GetProcAddress(hLib, "getPersNX");	// Get X-normal of person sight
		(FARPROC &)getPersNY = GetProcAddress(hLib, "getPersNY");	// Get Y-normal of person sight
		(FARPROC &)getPersNZ = GetProcAddress(hLib, "getPersNZ");	// Get Z-normal of person sight

		(FARPROC &)setPersHealth = GetProcAddress(hLib, "setPersHealth");	// Set the percent of person health
		(FARPROC &)addPersHealth = GetProcAddress(hLib, "addPersHealth");	// Add the percent of person health
		(FARPROC &)getPersHealth = GetProcAddress(hLib, "getPersHealth");	// Get the percent of person health

		(FARPROC &)setBulletSpeed = GetProcAddress(hLib, "setBulletSpeed");	// Set bullet speed
		(FARPROC &)addBulletTime = GetProcAddress(hLib, "addBulletTime");	// Add time for bullet
		(FARPROC &)objectProcessing = GetProcAddress(hLib, "objectProcessing");	// Processing interaction of all objects
		(FARPROC &)setCameraAfterObjectProcessing = GetProcAddress(hLib, "setCameraAfterObjectProcessing");	// Adjust the position of the camera, using the processing results of the physical engine

		(FARPROC &)getBulletDirectionForThirdPersonCamera = GetProcAddress(hLib, "getBulletDirectionForThirdPersonCamera");	// Get the bullet direction for third party camera

		(FARPROC &)setPersFrameMove = GetProcAddress(hLib, "setPersFrameMove");	// Set every frame peron parameters
		(FARPROC &)setPersPosFromServer = GetProcAddress(hLib, "setPersPosFromServer");	// Set person positon, using data from server
		(FARPROC &)setPersSpeedType = GetProcAddress(hLib, "setPersSpeedType");	// Set the type of speed
		(FARPROC &)getPersSpeedType = GetProcAddress(hLib, "getPersSpeedType");	// Get the type of speed
	}

	// Setting the callback function to initialize the application
	void setCallbackBullet()
	{
		void(*pBullet)(LPCALLBACKBULLET pCallback);
		(FARPROC &)pBullet = GetProcAddress(m_hLib, "SetCallbackBullet");
		pBullet(onBullet);
	}

protected:

	HMODULE	m_hLib;	// NetWars.dll
};


//--------------------------------------------------------------------------------------
// NETWORK CLASS
//--------------------------------------------------------------------------------------
#define	MSG_AUTHORIZE	2	// From server to client. The answer to the authorization
#define MSG_INFOPLAYER	7	// From server to client. Information about player.
#define MSG_INFOPLAYERS	8	// From server to client. Information about players. Message size depends on the number of players
#define	MSG_REGISTRATE	11	// From server to client. The answer to the registration
#define MSG_CHATSER		16	// From client to server. Chat message
#define MSG_CHAT		17	// From server to client. Chat message
#define MSG_CHAT_VALID	18	// From server to client. Chat message valid with server time
#define MSG_CHATBLOCK	19	// From server to client. Chat messages. Message size depends on the number of chat messages


class CNet
{
public:
	//CNet()	{}	// Constructor
	//~CNet()	{}	// Destructor

	void(*setVersion)(WORD nVersion);	// Set the version for comparison versions of client and server (by default it's 43, the engine version 0.44b)
	UINT(*startHost)(WORD port, WORD maxPlayers);	// Start Host (port number, maximum clients/players)
	UINT(*startServer)(WORD port);	// Start MMO Server
	void(*stopConnect)();	// Stop connect to the Server/Host
	void(*setLoginAlways)(bool bLoginAlways);	// Flag of always login on the server (even if the incorrect version or if any invalid login is automatically created new player with the name of Player"ID", ID - from 1)
	LPCWSTR(*getIP)(UINT iIP);	// Return IP-address
	void(*connectToHost)(LPCWSTR ip, WORD port, LPCWSTR name);	// Connect to the Host
	void(*connectToServer)(LPCWSTR ip, WORD port, LPCWSTR name, LPCWSTR pass);	// Connect to the Server
	UINT(*getNID)();	// Get Network ID
	UINT(*getNIDServer)();	// Get Network ID of Server
	void(*msg)(UINT nid, BYTE* msg, DWORD size);	// Network message (nid = 0 - to all with loop back)
	void(*msgToAll)(BYTE* msg, DWORD size);	// Network message to all (no loop back)

	void(*guestContent)(bool bAllow);	// Set flag to allow guest connections

	// Functions for the protocol to work with accounts and chat
	UINT(*addAccountAndChat)(LPCWSTR sName, LPCWSTR sServer, LPCWSTR sLogin, LPCWSTR sPassword);	// Add work with account and chat on the server (DB tables and protocol), Connect to SQL Server (with Accounts and Chat), Activate the server database, Return number of accounts in the DataBase
	void(*setAccountAndChat)();	// Set work with account and chat on the client (only protocol)
	void(*sendRegistrToServer)(LPCWSTR sName, LPCWSTR sPass, LPCWSTR sMail, LPCWSTR sSkype, LPCWSTR sCountry, LPCWSTR sCity, BYTE nAgeSex, LPCWSTR sMyself);	// Send registration to the server
	void(*sendChatMsgToServer)(UINT accRecipient, LPCWSTR sMsg);	// Send chat message from the client to the server
	__time32_t(*sendChatMsgToClient)(UINT accRecipient, LPCWSTR sMsg);	// Send chat message from the server to the client
	void(*sendGetContentToServer)(UINT param);	// Send a request to download content
	void(*sendGetUpdateToServer)(WORD version, UINT param, bool x64);	// Send a request to download update

	// Functions for transfer content
	// The requirement to the content: the size of each file should not exceed 4 GB
	// Do not forget to take into account that the maximum length of the path and file name can not be more than MAXPATH (260 or 256).
	// Should not contain exit to an external folder \.\ or \..\ 
	void(*setContentFolderAndFile)(LPCWSTR sFolder, LPCWSTR sFile);	// Set content folder and file
	bool(*setMinMaxThreads)(BYTE nThreadMin, BYTE nThreadMax);	// Set the minimum and maximum of threads for downloading content, return the flag of the true installation
	UINT(*addFile)(LPCWSTR sFile, UINT param);	// Add file to list of content for distribution	// Specify a file only within the directory (no "..")
	void(*prepareToSendContent)();	// Add work with content on the server, prepare to send content
	void(*prepareToReceiveContent)();	// Add work with content on the client, prepare to receive content

	//UINT(*addFileUpdate)(LPCWSTR sFile);	// exe and NetWars.dll don't need added (only for server)
	//void(*prepareToSendUpdate)(WORD version);	// Add work with update on the server, prepare to send update files (only for server)

	//setPathEngine();	// Set the path to DLL libraries of engine

	void setHModule(HMODULE hLib)	// Set NetWars.dll
	{
		m_hLib = hLib;

		(FARPROC &)setVersion = GetProcAddress(hLib, "setVersion");	// Set the version for comparison versions of client and server
		(FARPROC &)startHost = GetProcAddress(hLib, "startHost");	// Start Host
		(FARPROC &)startServer = GetProcAddress(hLib, "startServer");	// Start MMO Server
		(FARPROC &)stopConnect = GetProcAddress(hLib, "stopConnect");	// Stop connect to the Server/Host
		(FARPROC &)setLoginAlways = GetProcAddress(hLib, "setLoginAlways");	// Flag of always login on the server
		(FARPROC &)getIP = GetProcAddress(hLib, "getIP");	// Return IP-address
		(FARPROC &)connectToHost = GetProcAddress(hLib, "connectToHost");	// Connect to the Host
		(FARPROC &)connectToServer = GetProcAddress(hLib, "connectToServer");	// Connect to the Server
		(FARPROC &)getNID = GetProcAddress(hLib, "getNID");	// Get Network ID
		(FARPROC &)getNIDServer = GetProcAddress(hLib, "getNIDServer");	// Get Network ID of Server
		(FARPROC &)msg = GetProcAddress(hLib, "msg");	// Network message (nid=0 - to all with loop back)
		(FARPROC &)msgToAll = GetProcAddress(hLib, "msgToAll");	// Network message to all (no loop back)
		(FARPROC &)guestContent = GetProcAddress(hLib, "guestContent");	// Set flag to allow guest connections

		// Functions for the protocol to work with accounts and chat
		(FARPROC &)addAccountAndChat = GetProcAddress(hLib, "addAccountAndChatEx");	// Add work with account and chat on the server, Connect to SQL Server, Activate the server database, Return number of accounts in the DataBase
		(FARPROC &)setAccountAndChat = GetProcAddress(hLib, "setAccountAndChat");	// Set work with account and chat on the client
		(FARPROC &)sendRegistrToServer = GetProcAddress(hLib, "sendRegistrToServer");	// Send registration to the server
		(FARPROC &)sendChatMsgToServer = GetProcAddress(hLib, "sendChatMsgToServer");	// Send chat message from the client to the server
		(FARPROC &)sendChatMsgToClient = GetProcAddress(hLib, "sendChatMsgToClient");	// Send chat message from the server to the client
		(FARPROC &)sendGetContentToServer = GetProcAddress(hLib, "sendGetContentToServer");	// Send a request to download content

		// Functions for transfer content
		(FARPROC &)setContentFolderAndFile = GetProcAddress(hLib, "setContentFolderAndFile");	// Set content folder and file
		(FARPROC &)setContentFolderAndFile = GetProcAddress(hLib, "setMinMaxThreads");	// Set the minimum and maximum of threads for downloading content, return the flag of the true installation
		(FARPROC &)addFile = GetProcAddress(hLib, "addFileContent");	// Add file to list of content for distribution
		(FARPROC &)prepareToSendContent = GetProcAddress(hLib, "prepareToSendContent");	// Add work with content on the server, prepare to send content
		(FARPROC &)prepareToReceiveContent = GetProcAddress(hLib, "prepareToReceiveContent");	// Add work with content on the client, prepare to receive content

		//(FARPROC &)addFileUpdate = GetProcAddress(hLib, "addFileUpdate");	// Add file to list of content for distribution
	}

	// Set callback function for connect the player to the server (host) or connect the server (host) to the player
	void setCallbackConnect()
	{
		void(*pConnect)(LPCALLBACKCONNECTPLAYER pCallback);
		(FARPROC &)pConnect = GetProcAddress(m_hLib, "SetCallbackConnectPlayer");
		pConnect(onConnect);
	}

	// Set callback function for disconnect the player from the server (host) or disconnect the server (host) from the player
	void setCallbackDisconnect()
	{
		void(*pDisconnect)(LPCALLBACKDISCONNECTPLAYER pCallback);
		(FARPROC &)pDisconnect = GetProcAddress(m_hLib, "SetCallbackDisconnectPlayer");
		pDisconnect(onDisconnect);
	}

	// Set callback function for get the network message
	void setCallbackNetMsg()
	{
		void(*pNetMsg)(LPCALLBACKNETMSG pCallback);
		(FARPROC &)pNetMsg = GetProcAddress(m_hLib, "SetCallbackNetMsg");
		pNetMsg(onNetMsg);
	}

protected:

	HMODULE	m_hLib;	// NetWars.dll
};


//--------------------------------------------------------------------------------------
// DATABASE CLASS
//--------------------------------------------------------------------------------------
#define	DB_INT		0	// int, __time32_t: from –2^31 (–2 147 483 648) to 2^31 - 1 (2 147 483 647)
#define	DB_WCHAR	1	// wchar_t[]
#define	DB_SYSTIME	2	// SYSTEMTIME (16 byte)
#define	DB_INT64	3	// __int64: from - 2^63 (-9 223 372 036 854 775 808) to 2^63 - 1 (9 223 372 036 854 775 807)
#define	DB_SHORT	4	// short: from 2^15 (-32 768) to 2^15 - 1 (32 767)
#define	DB_BYTE		5	// byte: from 0 to 255
#define	DB_FLOAT	6	// float: -3,40E+38 — -1,18E-38, 0 and 1,18E-38 — 3,40E+38
#define	DB_DOUBLE	7	// double: -1,79E+308 — -2,23E-308, 0 and 2, 23E-308 — 1,79E+308

#define	TABLE_ACCOUNT	0	// Account
#define	TABLE_REGISTR	1	// Registration
#define	TABLE_MESSAGE	2	// Message

#pragma pack(push,1)
struct ACCOUNT
{
	int acc;
	wchar_t sName[16];
	wchar_t sPass[16];
	float	fRate;
	__time32_t	tRest;
	__time32_t	tBan;
};
struct REGISTR
{
	int acc;
	wchar_t sMail[64];
	wchar_t sSkype[64];
	wchar_t sCountry[64];
	wchar_t sCity[32];
	byte	nAgeSex;
	wchar_t sMyself[96];
};
struct MESSAGE
{
	int id;
	int accSender;
	int accRecipient;
	__time32_t	time;
	wchar_t str[128];
};
#pragma pack(pop)

class CDB
{
public:
	//CDB()	{}	// Constructor
	//~CDB()	{}	// Destructor
	
	UINT(*addField)(UINT iType, LPCWSTR sName, UINT size);	// Add field
	UINT(*addTable)(LPCWSTR sTable, UINT nKey);				// Add table
	UINT(*connect)(LPCWSTR sName, LPCWSTR sServer, LPCWSTR sLogin, LPCWSTR sPassword);	// Connect to SQL Server
	void(*disconnect)();	// Disconnect
	UINT(*getSize)(UINT idTable);	// Get table size (new ID)
	UINT(*getDataLast)(UINT idTable, BYTE* data, UINT num);	// Get last data (last num items)
	void(*getData)(UINT idTable, BYTE *pData);	// Get data (all items)
	bool(*getItem)(UINT idTable, BYTE* pData);	// Get item (item id inside pData)
	void(*add)(UINT idTable, BYTE* pData);		// Add item
	//void(*insert)(UINT idTable, BYTE* pData);	// Insert item (item id inside pData)
	void(*update)(UINT idTable, BYTE* pData);	// Update item
	void(*remove)(UINT idTable, int ID);		// Remove item

	// Functions for work accounts and chat tables (DB) and cashe (need add Account And Chat)
	void(*addAccountAndChat)();	// Add fields and tables for work with accounts and chat (must be used prior to the addition of other tables)
	void(*activateAccountAndChat)(UINT nAcc);	// Activate accounts and chat on the database server
	bool(*createAdmin)(LPCWSTR sName, LPCWSTR sPass, LPCWSTR sMail, LPCWSTR sSkype, LPCWSTR sCountry, LPCWSTR sCity, byte nAgeSex, LPCWSTR sMyself);	// Create Admin
	bool(*getAccount)(WORD id, ACCOUNT* acc, REGISTR* reg);	// Get account
	LPCWSTR(*getName)(UINT acc);	// Get account name from cashe, using the ACC
	LPCWSTR(*getNameID)(WORD id);	// Get account name from cashe, using the ID

	void setHModule(HMODULE hLib)	// Set NetWars.dll
	{
		m_hLib = hLib;

		(FARPROC &)addField = GetProcAddress(hLib, "addField");	// Add field
		(FARPROC &)addTable = GetProcAddress(hLib, "addTable");	// Add table
		
		(FARPROC &)connect = GetProcAddress(hLib, "connectToSQLServer");	// Connect to SQL Server
		(FARPROC &)disconnect = GetProcAddress(hLib, "disconnectSQL");	// Disconnect
		(FARPROC &)getSize = GetProcAddress(hLib, "getSizeSQL");	// Get table size
		(FARPROC &)getDataLast = GetProcAddress(hLib, "getDataLastSQL");	// Get last data
		(FARPROC &)getData = GetProcAddress(hLib, "getDataSQL");	// Get data
		(FARPROC &)getItem = GetProcAddress(hLib, "getItemSQL");	// Get item
		(FARPROC &)add = GetProcAddress(hLib, "addSQL");	// Add item
		//(FARPROC &)insert = GetProcAddress(hLib, "insertSQL");	// Insert item
		(FARPROC &)update = GetProcAddress(hLib, "updateSQL");	// Update item
		(FARPROC &)remove = GetProcAddress(hLib, "deleteSQL");	// Remove item

		// Functions for work accounts and chat tables (DB) and cashe (need add Account And Chat)
		(FARPROC &)addAccountAndChat = GetProcAddress(hLib, "addAccountAndChat");	// Add fields and tables for work with accounts and chat
		(FARPROC &)activateAccountAndChat = GetProcAddress(hLib, "activateAccountAndChat");	// Activate accounts and chat on the database server
		(FARPROC &)createAdmin = GetProcAddress(hLib, "createAdmin");	// Create Admin
		(FARPROC &)getAccount = GetProcAddress(hLib, "getAccount");	// Get item
		(FARPROC &)getName = GetProcAddress(hLib, "getName");	// Get the account name from cashe, using the ACC
		(FARPROC &)getNameID = GetProcAddress(hLib, "getNameID");	// Get the account name from cashe, using the ID
	}

protected:

	HMODULE	m_hLib;	// NetWars.dll
};


//--------------------------------------------------------------------------------------
// LANGUAGE CLASS
//--------------------------------------------------------------------------------------
class CLang
{
public:
	//C3D()	{}	// Constructor
	//~C3D()	{}	// Destructor
	
	void setHModule(HMODULE hLib)	// Set NetWars.dll
	{
		m_hLib = hLib;
	}

protected:

	HMODULE	m_hLib;	// NetWars.dll
};










