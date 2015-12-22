//--------------------------------------------------------------------------------------
// File: 0 Clear Screen.cpp
// Description: Sample of the clear screen.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example demonstrates the simplest call of the engine (only clear screen) and use optional settings (fullscreen mode).

// NOTE:
// - To compile in the Visual Studio 2013 or 2010, set Project -> Properties -> Configuration Properties -> General -> Platform Toolset = Visual Studio 2013 (v120) or Visual Studio 2012 (v110) or Visual Studio 2010 (v100) respectively.
// - It's recommended to have installed DirectX SDK v.43 (june 2010). Files: release (D3DCompiler_43.dll, D3DX9_43.dll, d3dx10_43.dll, d3dx11_43.dll), debug (D3dx9d_43.dll, D3DX10d_43.dll, D3DX11d_43.dll). If not installed then 64 bit DirectX DLL library are located in "Debug DLL" and "Bin" folders; 32 bit - in "Debug DLL 32" and "Bin32". You may copy this files in "Windows\System32" and "Windows\SysWOW64" folders or in each folder with example.
// - Registry folders: HKEY_CLASSES_ROOT, HKEY_CURRENT_CONFIG, HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, HKEY_PERFORMANCE_DATA, HKEY_USERS
// - Press ALT + ENTER to switch from fullscreen to windowed and back.


#include "..\Engine\NetWars.h"	// Engine


CEngine	g_pAppl;	// Base Engine class


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	// Optional settings
	g_pAppl.setLOG(L"LOG", true);	// LOG, Unicode text file
	g_pAppl.setRegistryFolder(HKEY_CURRENT_USER, L"NetWars\\Samples\\0 Clear Screen");	// Registry folder, remember last audio and video settings and other
	g_pAppl.setVideo(0, 0, 0, 1, 4, 0);	// Video setting
	g_pAppl.setVideoFullScreen(true, 0, 0, 0);	// Video setting: full screen, width, height, refresh rate
	//g_pAppl.setAudio();	// Audio setting
	g_pAppl.setMinScreenSize(1024, 600);	// Minimum screen size (default: 1024, 600)

	return g_pAppl.run(L"0 Clear Screen", 1024, 600);	// Run Engine
}
