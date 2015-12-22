//--------------------------------------------------------------------------------------
// File: 1 Sprites.cpp
// Description: Sample of sprites.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// In this example, we will look at 2 main classes of work with 2D graphics: texture class (CTex) and the class of sprites (C2D)

// This example shows how to work with:
// - textures from file, memory, resource, dynamic and from GUI;
// - static and dynamic sprites, rotation and sprite animation;
// - render setting: only texture, only color, texture with color (for example, for dynamically adjusting the transparency of sprites);
// - setting callback functions to creating device, changing screen size, rendering frame.

// Setting the sprite occurs in 3 stages:
// 1. Adding the texture (and getting its size if necessary). Returns the ID of the texture (and size), starting with 0.
// 2. Adding the sprite through texture coordinates (with texture size), display position and depth. Returns the ID of the sprite, starting with 0.
// 3. Setting sprites and its texteres to display after setRenderSprite() when rendering frame.

// NOTE:
// - Don't forget to connect the hModule of DLL to an instance of other classes for dynamic connection DLL.
// - Don't forget to specify texture size when adding sprite.
// - You can declare a globals variable to identify the added textures, sprites, etc, and you can define their through #define.
// - Update sprite buffer after at least one update sprite and before setRenderSprite().
// - Following texture formats are supported: DDS (recomended), JPG, PNG, BMP, TIFF, GIF, WMP. Last 4 not recomended.
// - Depth of sprites > 0.0f and < 1.0f, rendered less. For overlapping sprites, sprites with lower depth should be rendered after sprites with higher depth. Behind it you will need to keep an eye on their own.
// - To obtain DDS textures can be used NVIDIA Texture Tools for Adobe Photoshop https://developer.nvidia.com/nvidia-texture-tools-adobe-photoshop . Don't forget to set MIP Map Generation = No MIP Map. The best options: 8-8-8-8 ARGB (32 bit), 8-8-8 RGB (24 bit), DXT1, DXT5.
// - All dynamic textures have a copy of map in the RAM for changes.
// - Can use g_pTex.renderUI() for render sprites from the GUI Texture.
// - When set render sprites set linear filter, but it can be changed to another (anisot, point, etc.).
// - More details of the work with dynamic textures, including scaling, cropping and saving to file, there is in Example 8 (2D Editor).

// Used content:
// - Content\Texture\BackGround.dds
// - Content\Texture\Panel.dds
// - Content\Texture\Resource.dds


#include "..\Engine\NetWars.h"	// Engine

#include <stdio.h>		// For FILE, _wfopen_s, fseek, ftell, fread, fclose
#include "Resource.h"	// For IDR_RCDATA1


CEngine	g_pAppl;	// Base Engine class
C2D		g_p2D;		// 2D class
CTex	g_pTex;		// Texture class


#define	ID_TEXTURE_FILE		0	// Id of texture from file
#define	ID_TEXTURE_MEMORY	1	// Id of texture from memory
#define	ID_TEXTURE_RESOURCE	2	// Id of texture from resource
#define	ID_TEXTURE_DYNAMIC	3	// Id of dynamic texture 


UINT	g_iWidth0, g_iHeight0;	// Width and Height of texture from file
UINT	g_iWidth1, g_iHeight1;	// Width and Height of texture from memory
UINT	g_iWidth2, g_iHeight2;	// Width and Height of texture from resource
UINT	g_iWidth3 = 128, g_iHeight3 = 128;	// Width and Height of dynamic texture 

float g_fHW = 512.0f;	// Half width
float g_fHH = 300.0f;	// Half height


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	g_pAppl.setCallbackDeviceCreated();		// Callback functions to creating device
	g_pAppl.setCallbackSwapChainResized();	// Callback functions to changing screen size
	g_pAppl.setCallbackFrameRender();		// Callback functions to rendering frame

	g_p2D.setHModule(g_pAppl.getHModule());	// Connection the hModule of DLL to an instance of other classes for dynamic connection DLL
	g_pTex.setHModule(g_pAppl.getHModule());	// Connection the hModule of DLL to an instance of other classes for dynamic connection DLL

	return g_pAppl.run(L"1 Sprites", 1024, 600);	// Run Engine
}



//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	// Load Texture from file
	g_pTex.add(L"..\\Content\\Texture\\BackGround.dds");	// Return ID_TEXTURE_FILE = 0
	g_pTex.getSize(ID_TEXTURE_FILE, &g_iWidth0, &g_iHeight0);	// Texture size

	// Load Texture from memory
	FILE* pFile;
	if (NULL == _wfopen_s(&pFile, L"..\\Content\\Texture\\Panel.dds", L"rb"))	// Open a binary file for reading
	{
		fseek(pFile, 0, SEEK_END);		// Move the pointer to the end of the file
		SIZE_T size = ftell(pFile);		// Get the file size
		fseek(pFile, 0, SEEK_SET);		// Move the pointer to the beginning of the file
		BYTE *pData = new BYTE[size];	// Allocate memory
		fread(pData, 1, size, pFile);	// Copy file to memory
		fclose(pFile);					// Cllose file

		// Load Texture from memory
		g_pTex.addFromMemory(pData, size);	// Return ID_TEXTURE_MEMORY = 1
		g_pTex.getSize(ID_TEXTURE_MEMORY, &g_iWidth1, &g_iHeight1);	// Texture size

		delete[] pData;
	}

	// Load Texture from resource
	g_pTex.addFromResource(MAKEINTRESOURCE(IDR_RCDATA1));	// Return ID_TEXTURE_RESOURCE = 2
	g_pTex.getSize(ID_TEXTURE_RESOURCE, &g_iWidth2, &g_iHeight2);	// Texture size

	// Create Dynamic Texture: 128 * 128
	g_pTex.addDynamic(g_iWidth3, g_iHeight3);	// Return ID_TEXTURE_DYNAMIC = 3
	RECT rect;
	SetRect(&rect, 10, 20, 110, 120);

	// Through DWORD
	UINT* data = new UINT[10000];
	for (int i = 0; i < 10000; i++)
		*data++ = 0xff0080ff;
	data -= 10000;
	// Through BYTE
	//BYTE* data = new BYTE[40000];
	//for (int i = 0; i < 10000; i++)
	//{
	//	*data++ = 255;
	//	*data++ = 0;
	//	*data++ = 0;
	//	*data++ = 255;
	//}
	//data -= 40000;

	//g_pAppl.updateTexture(ID_TEXTURE_DYNAMIC, &rect, data);
	g_pTex.copyPart(ID_TEXTURE_DYNAMIC, &rect, data);	// Copy a piece of texture in texture, as addition
	g_pTex.changePart(ID_TEXTURE_DYNAMIC, &rect, data);	// Copy a piece of texture in texture, as substitute

	delete[] data;
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int w, int h)
{
	g_fHW = float(w >> 1);
	g_fHH = float(h >> 1);

	// Background sprite
	g_p2D.addF(0.0f / g_iWidth0, 1024.0f / g_iWidth0, 0.0f / g_iHeight0, 600.0f / g_iHeight0, 0.0f, (float)w, 0.0f, (float)h, 0.9f);	// Return 0

	// Timer
	g_p2D.addF(0.0f, 106.0f / g_iWidth0, 630.0f / g_iHeight0, 736.0f / g_iHeight0, g_fHW - 53.0f, g_fHW + 53.0f, g_fHH - 53.0f, g_fHH + 53.0f, 0.6f);	// Return 1, Timer
	g_p2D.addF(0.0f, 56.0f / g_iWidth0, 738.0f / g_iHeight0, 752.0f / g_iHeight0, g_fHW - 28.0f, g_fHW + 28.0f, g_fHH + 28.0f, g_fHH + 42.0f, 0.5f);	// Return 2, Timer line
	for (int i = 0; i < 17; i++)
		g_p2D.addF(60.0f * i / g_iWidth0, 60.0f * (i + 1) / g_iWidth0, 601.0f / g_iHeight0, 629.0f / g_iHeight0, g_fHW - 30.0f, g_fHW + 30.0f, g_fHH - 29.0f, g_fHH - 1.0f, 0.5f);	// Return 3 - 19, Timer indicator

	// Panel sprites
	g_p2D.setTextureSize(g_iWidth1, g_iHeight1);	// For the correct calculation of texture coordinates of sprites
	g_p2D.add( 0, 22,  0, 30,   0.0f,  22.0f, 30.0f,    60.0f, 0.8f);	// Return 20, up, begin
	g_p2D.add(21, 22,  0, 30,  22.0f, 192.0f, 30.0f,    60.0f, 0.8f);	// Return 21, up, center
	g_p2D.add(21, 49,  0, 30, 192.0f, 220.0f, 30.0f,    60.0f, 0.8f);	// Return 22, up, end
	g_p2D.add( 0, 22, 30, 88,   0.0f,  22.0f, 60.0f, (float)h, 0.8f);	// Return 23, basis, begin
	g_p2D.add(21, 22, 30, 88,  22.0f, 192.0f, 60.0f, (float)h, 0.8f);	// Return 24, basis, center
	g_p2D.add(21, 49, 30, 88, 192.0f, 220.0f, 60.0f, (float)h, 0.8f);	// Return 25, basis, end

	// Button sprites
	g_p2D.setTextureSize(g_iWidth2, g_iHeight2);	// For the correct calculation of texture coordinates of sprites
	g_p2D.add(  0,  16, 0, 32,   8.0f,  24.0f, 40.0f, 72.0f, 0.7f);	// Return 26, button 1, begin
	g_p2D.add( 16,  17, 0, 32,  24.0f, 184.0f, 40.0f, 72.0f, 0.7f);	// Return 27, button 1, center
	g_p2D.add( 17,  41, 0, 32, 184.0f, 208.0f, 40.0f, 72.0f, 0.7f);	// Return 28, button 1, end
	g_p2D.add( 42,  58, 0, 32,   8.0f,  24.0f, 40.0f, 72.0f, 0.7f);	// Return 29, button 2, begin
	g_p2D.add( 58,  59, 0, 32,  24.0f, 184.0f, 40.0f, 72.0f, 0.7f);	// Return 30, button 2, center
	g_p2D.add( 59,  83, 0, 32, 184.0f, 208.0f, 40.0f, 72.0f, 0.7f);	// Return 31, button 2, end
	g_p2D.add( 84, 100, 0, 32,   8.0f,  24.0f, 40.0f, 72.0f, 0.7f);	// Return 32, button 3, begin
	g_p2D.add(100, 101, 0, 32,  24.0f, 184.0f, 40.0f, 72.0f, 0.7f);	// Return 33, button 3, center
	g_p2D.add(101, 125, 0, 32, 184.0f, 208.0f, 40.0f, 72.0f, 0.7f);	// Return 34, button 3, end

	g_p2D.setTextureSize(256, 256);	// 256 * 256 - UI texture size
	g_p2D.add(  0,  16, 33, 65,   8.0f,  24.0f, 168.0f, 200.0f, 0.7f);	// Return 35, button 1, begin
	g_p2D.add( 16,  17, 33, 65,  24.0f, 184.0f, 168.0f, 200.0f, 0.7f);	// Return 36, button 1, center
	g_p2D.add( 17,  41, 33, 65, 184.0f, 208.0f, 168.0f, 200.0f, 0.7f);	// Return 37, button 1, end
	g_p2D.add( 42,  58, 33, 65,   8.0f,  24.0f, 168.0f, 200.0f, 0.7f);	// Return 38, button 2, begin
	g_p2D.add( 58,  59, 33, 65,  24.0f, 184.0f, 168.0f, 200.0f, 0.7f);	// Return 39, button 2, center
	g_p2D.add( 59,  83, 33, 65, 184.0f, 208.0f, 168.0f, 200.0f, 0.7f);	// Return 40, button 2, end
	g_p2D.add( 84, 100, 33, 65,   8.0f,  24.0f, 168.0f, 200.0f, 0.7f);	// Return 41, button 3, begin
	g_p2D.add(100, 101, 33, 65,  24.0f, 184.0f, 168.0f, 200.0f, 0.7f);	// Return 42, button 3, center
	g_p2D.add(101, 125, 33, 65, 184.0f, 208.0f, 168.0f, 200.0f, 0.7f);	// Return 43, button 3, end

	// Red square sprite
	g_p2D.addEx(10, 110, 20, 120, g_iWidth3, g_iHeight3, 300.0f, 400.0f, 300.0f, 400.0f, 0.6f);	// Return 44, Orange square
	g_p2D.addEx(10, 110, 20, 120, g_iWidth3, g_iHeight3, 8.0f, 208.0f, 79.0f, 99.0f, 0.6f);	// Return 45, Orange progress
	g_p2D.addEmpty(2);	// Return 47, add 2 empty sprites (46 and 47) for future udates
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double time, float)
{
	//float cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background
	//g_pAppl.clearScreen(cClear);	// If there is background then don't need to clear screen

	float fTime = float(time);
	// Update sprites (supported update any sprites)
	g_p2D.pos4(44, 300.0f - 50.0f*sinf(fTime), 400.0f + 50.0f*cosf(fTime), 300.0f - 50.0f*sinf(fTime + PI2), 400.0f + 50.0f*cosf(fTime + PI2),
		300.0f - 50.0f*sinf(fTime + PI), 400.0f + 50.0f*cosf(fTime + PI), 300.0f - 50.0f*sinf(fTime + _3PI2), 400.0f + 50.0f*cosf(fTime + _3PI2));	// Orange square, rotation
	fTime -= 4 * int(fTime / 4.0f);
	if (fTime > 2.0f)
		fTime = 4.0f - fTime;
	g_p2D.updateF(2, 0.0f, 28.0f * fTime / g_iWidth0, 738.0f / g_iHeight0, 752.0f / g_iHeight0, g_fHW - 28.0f, g_fHW - 28.0f * (1.0f - fTime), g_fHH + 28.0f, g_fHH + 42.0f, 0.5f);	// Timer line
	g_p2D.updateEx(45, 30, 40, 30, 40, 128.0f, 128.0f, 8.0f, 8.0f + 100.0f*fTime, 80.0f, 100.0f, 0.6f);	// Orange progress
	g_p2D.updateF(46, 30 / 128.0f, 40 / 128.0f, 30 / 128.0f, 40 / 128.0f, 208.0f - 100.0f*fTime, 208.0f, 110.0f, 130.0f, 0.6f);	// Orange progress 2
	g_p2D.setTextureSize(g_iWidth3, g_iHeight3);	// For the correct calculation of texture coordinates of sprites
	g_p2D.update(47, 30, 40, 30, 40, 108.0f - 50.0f*fTime, 108.0f + 50.0f*fTime, 140.0f, 160.0f, 0.6f);	// Orange progress 3
	g_p2D.updateBuffer();	// Update sprite buffer after all sprite updates and before setRenderSprite()

	// Render background + timer
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, ID_TEXTURE_FILE);
	//g_pAppl.setRenderFilter(FILTER_ANISOT);	// Set Anisotropic Filter, if necessary
	g_p2D.draw(0, 3);	// Background + timer
	if (fTime > 0.15f && fTime < 1.85f)
		g_p2D.draw(3 + int(10.0f * fTime - 1.5f), 1);	// Timer indicator

	// Render panel
	g_pAppl.setRenderSprites(RENDER_TEXTURE_MULTI_COLOR, ID_TEXTURE_MEMORY);
	float	alpha[4] = { 1.0f, 1.0f, 1.0f, fTime / 2.0f };	// Dynamically adjusting the transparency of sprites
	g_p2D.setColor(alpha);
	g_p2D.draw(20, 6);

	// Render button from resourse
	g_pAppl.setRenderSprites(RENDER_TEXTURE_PLUS_COLOR, ID_TEXTURE_RESOURCE);
	float	add[4] = { 0.0f, 0.0f, fTime / 4.0f, 0.0f };	// Adding some blue
	g_p2D.setColor(add);
	g_p2D.draw(26 + 3 * int(fTime + 0.5f), 3);

	// Render button from UI texture
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pTex.renderUI();
	g_p2D.draw(35 + 3 * int(fTime + 0.5f), 3);

	// Render orange square with rotation
	g_pTex.render(ID_TEXTURE_DYNAMIC);
	g_p2D.draw(44, 1);

	// Render with change size and color
	g_pAppl.setRenderSprites(RENDER_ONLY_COLOR, 0);	// Only color, excluding texture
	float	color[4] = { 1.0f, fTime / 2.0f, 0.0f, 1.0f };
	g_p2D.setColor(color);
	g_p2D.draw(45, 3);
}