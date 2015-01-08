////////////////////////////////////////////////////////////////////////////////
//
// Sniffy.cpp
//
// Copyright (C) 2000 Bruno 'Beosil' Heidelberger
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	This is the main DLL class. It handles all startup and hooking.
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Sniffy.h"
#include "Hooks.h"
#include "Patch.h"
#include "Logger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



////////////////////////////////////////////////////////////////////////////////
//
//	Windows message map
//
////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSniffyApp, CWinApp)
	//{{AFX_MSG_MAP(CSniffyApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



////////////////////////////////////////////////////////////////////////////////
//
//	The one and only DLL instance
//
////////////////////////////////////////////////////////////////////////////////

CSniffyApp theApp;



////////////////////////////////////////////////////////////////////////////////
//
//	Array of the packet lengths
//
////////////////////////////////////////////////////////////////////////////////

static int g_packetLength[] = {
	/* 0x00 */ 0x68, 0x05, 0x07, 0x00,  0x02,  0x05,  0x05, 0x07, 0x0E, 0x05, 0x0B, 0x10A, 0x00,  0x03, 0x00, 0x3D, 
	/* 0x10 */ 0xD7, 0x00, 0x00, 0x0A,  0x06,  0x09,  0x01, 0x00, 0x00, 0x00, 0x00, 0x25,  0x00,  0x05, 0x04, 0x08, 
	/* 0x20 */ 0x13, 0x08, 0x03, 0x1A,  0x07,  0x14,  0x05, 0x02, 0x05, 0x01, 0x05, 0x02,  0x02,  0x11, 0x0F, 0x0A, 
	/* 0x30 */ 0x05, 0x01, 0x02, 0x02,  0x0A,  0x28D, 0x00, 0x08, 0x07, 0x09, 0x00, 0x00,  0x00,  0x02, 0x25, 0x00, 
	/* 0x40 */ 0xC9, 0x00, 0x00, 0x229, 0x2C9, 0x05,  0x00, 0x0B, 0x49, 0x5D, 0x05, 0x09,  0x00,  0x00, 0x06, 0x02, 
	/* 0x50 */ 0x00, 0x00, 0x00, 0x02,  0x0C,  0x01,  0x0B, 0x6E, 0x6A, 0x00, 0x00, 0x04,  0x02,  0x49, 0x00, 0x31, 
	/* 0x60 */ 0x05, 0x09, 0x0F, 0x0D,  0x01,  0x04,  0x00, 0x15, 0x00, 0x00, 0x03, 0x09,  0x13,  0x03, 0x0E, 0x00, 
	/* 0x70 */ 0x1C, 0x00, 0x05, 0x02,  0x00,  0x23,  0x10, 0x11, 0x00, 0x09, 0x00, 0x02,  0x00,  0x0D, 0x02, 0x00, 
	/* 0x80 */ 0x3E, 0x00, 0x02, 0x27,  0x45,  0x02,  0x00, 0x00, 0x42, 0x00, 0x00, 0x00,  0x0B,  0x00, 0x00, 0x00, 
	/* 0x90 */ 0x13, 0x41, 0x00, 0x63,  0x00,  0x09,  0x00, 0x02, 0x00, 0x1A, 0x00, 0x102, 0x135, 0x33, 0x00, 0x00, 
	/* 0xa0 */ 0x03, 0x09, 0x09, 0x09,  0x95,  0x00,  0x00, 0x04, 0x00, 0x00, 0x05, 0x00,  0x00,  0x00, 0x00, 0x0D, 
	/* 0xb0 */ 0x00, 0x00, 0x00, 0x00,  0x00,  0x40,  0x09, 0x00, 0x00, 0x03, 0x06, 0x09,  0x03,  0x00, 0x00, 0x00, 
	/* 0xc0 */ 0x24, 0x00, 0x00, 0x00,  0x06,  0xCB,  0x01, 0x31, 0x02, 0x06, 0x06, 0x07,  0x00
};



////////////////////////////////////////////////////////////////////////////////
//
//	Original addresses of sniffed functions
//
////////////////////////////////////////////////////////////////////////////////

static unsigned int g_recvAddress = 0;
static unsigned int g_sendAddress = 0;



////////////////////////////////////////////////////////////////////////////////
//
//	This function is called whenever a send or a recv event is triggered
//
//	PARAMETERS:
//		unsigned char *pBuffer	A pointer to the data buffer
//		int source				"0" if server-packet, "1" if client-packet
//
//	RETURNS:
//		-void-
//
////////////////////////////////////////////////////////////////////////////////

void __stdcall SnifferFunction(unsigned char *pBuffer, int source)
{
	unsigned int length;
	length = g_packetLength[*pBuffer];

	if(source == 0)
	{
		if(length == 0) length = *(unsigned short *)&pBuffer[1];
		theLogger.OnServerPacket(pBuffer, length);
	}
	else
	{
		if(length == 0) length = (unsigned int)pBuffer[1] * 256 + (unsigned int)pBuffer[2];
		theLogger.OnClientPacket(pBuffer, length);
	}
}



////////////////////////////////////////////////////////////////////////////////
//
//	Wrapper/Sniffer function replacing the original ones
//
////////////////////////////////////////////////////////////////////////////////

__declspec(naked) void SendWrapperFunction(int, int)
{
	__asm
	{
		push ecx

		push 0x00000001
		push [esp + 0x0c]
		call SnifferFunction
		
		pop ecx

		push [esp + 0x08]
		push [esp + 0x08]
		call g_sendAddress

		ret 8
	}
}

__declspec(naked) void ReceiveWrapperFunction(int)
{
	__asm
	{
		push ecx

		push 0x00000000
		push [esp + 0x0c]
		call SnifferFunction

		pop ecx

		push ecx

		push [esp + 0x08]
		call g_recvAddress

		pop ecx

		ret 4
	}
}



////////////////////////////////////////////////////////////////////////////////
//
//	Initialize the DLL, hook and patch target application
//
//	PARAMETERS:
//		unsigned int checksum	Checksum of the patch target
//		unsigned int length		Length of the patch target
//
//	RETURNS:
//		int		SNIFFY_ERROR_NOPATCH:	Invalid patch information
//				SNIFFY_ERROR_PROTECT:	Memory protection
//				SNIFFY_ERROR_BADEXE:	Unknown executable
//				SNIFFY_ERROR_NONE:		Success
//
////////////////////////////////////////////////////////////////////////////////

int CSniffyApp::Init(unsigned int checksum, unsigned int length)
{
	CPatch patch;
	if(!patch.LoadPatch("Sniffy.cfg", checksum, length)) return SNIFFY_ERROR_NOPATCH;

	DWORD oldProtect;

	IMAGE_DOS_HEADER *idh = (IMAGE_DOS_HEADER *)GetModuleHandle("client.exe");
	if(!idh) idh = (IMAGE_DOS_HEADER *)GetModuleHandle("uotd.exe");
	if(!idh) return SNIFFY_ERROR_BADEXE;

	IMAGE_FILE_HEADER *ifh = (IMAGE_FILE_HEADER *)((DWORD)(idh) + idh->e_lfanew + sizeof(DWORD));
	IMAGE_OPTIONAL_HEADER *ioh = (IMAGE_OPTIONAL_HEADER *)((DWORD)(ifh) + sizeof(IMAGE_FILE_HEADER));
	IMAGE_IMPORT_DESCRIPTOR *iid = (IMAGE_IMPORT_DESCRIPTOR *)((DWORD)(idh) + ioh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	VirtualProtect((LPVOID)((DWORD)(idh) + ioh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress), ioh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size, PAGE_READWRITE, &oldProtect);

	while(iid->Name)
	{
		if(!stricmp("wsock32.dll", (char *)((DWORD)(idh) + iid->Name)))
		{
			IMAGE_THUNK_DATA *pThunk = (IMAGE_THUNK_DATA *)((DWORD)(idh) + (DWORD)iid->Characteristics);
			IMAGE_THUNK_DATA *pThunk2 = (IMAGE_THUNK_DATA *)((DWORD)(idh) + (DWORD)iid->FirstThunk);

			while(pThunk->u1.Ordinal)
			{
				switch(pThunk->u1.Ordinal & 0x7fffffff)
				{
					case 3:
						pThunk2->u1.Function = (PDWORD)Hook_closesocket;
						break;
					case 4:
						pThunk2->u1.Function = (PDWORD)Hook_connect;
						break;
					case 16:
						pThunk2->u1.Function = (PDWORD)Hook_recv;
						break;
					case 19:
						pThunk2->u1.Function = (PDWORD)Hook_send;
						break;
				}

				pThunk++;
				pThunk2++;
			}
		}

		iid++;
	}

	g_sendAddress = patch.GetSendAddress();

	CPatch::TElementList& sendList = patch.GetSendList();

	POSITION position;
	position = sendList.GetHeadPosition();

	while(position)
	{
		CPatch::TElement *pElement = sendList.GetNext(position);

		if(VirtualProtect((LPVOID)(pElement->address + 1), 4, PAGE_READWRITE, &oldProtect) == 0) return SNIFFY_ERROR_PROTECT;
		*(unsigned int *)(pElement->address + 1) = (unsigned int)SendWrapperFunction - (pElement->address + 5);
	}

	g_recvAddress = patch.GetRecvAddress();

	CPatch::TElementList& recvList = patch.GetRecvList();

	position = recvList.GetHeadPosition();
	while(position)
	{
		CPatch::TElement *pElement = recvList.GetNext(position);

		if(VirtualProtect((LPVOID)(pElement->address + 1), 4, PAGE_READWRITE, &oldProtect) == 0) return SNIFFY_ERROR_PROTECT;
		*(unsigned int *)(pElement->address + 1) = (unsigned int)ReceiveWrapperFunction - (pElement->address + 5);
	}

	return SNIFFY_ERROR_NONE;
}



////////////////////////////////////////////////////////////////////////////////
//
// Get error text ( exported function )
//
//	PARAMETERS:
//		int error		Error number
//
//	RETURNS:
//		char *			Error description
//
////////////////////////////////////////////////////////////////////////////////

char *GetErrorText(int error)
{
	switch(error)
	{
	case SNIFFY_ERROR_NONE:
		return "No error";
	case SNIFFY_ERROR_NOPATCH:
		return "Invalid patch information";
	case SNIFFY_ERROR_PROTECT:
		return "Memory protection";
	}

	return "Undefined error";
}



////////////////////////////////////////////////////////////////////////////////
//
// Get info text ( exported function )
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		char *			Module description
//
////////////////////////////////////////////////////////////////////////////////

char *GetInfoText()
{
	return SNIFFY_INFO;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Install the DLL ( exported function )
//
//	PARAMETERS:
//		unsigned int checksum	Checksum of the patch target
//		unsigned int length		Length of the patch target
//
//	RETURNS:
//		int		SNIFFY_ERROR_XXX:		Failed to load DLL
//				SNIFFY_ERROR_NONE:		Success
//
////////////////////////////////////////////////////////////////////////////////

int Install(unsigned int checksum, unsigned int length)
{
	return theApp.Init(checksum, length);
}
