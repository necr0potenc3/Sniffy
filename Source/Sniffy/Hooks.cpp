////////////////////////////////////////////////////////////////////////////////
//
// Hooks.cpp
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
//	These are the socket hook functions
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Hooks.h"
#include "Logger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



////////////////////////////////////////////////////////////////////////////////
//
//	The socket hook functions
//
////////////////////////////////////////////////////////////////////////////////

int WINAPI Hook_closesocket(SOCKET s)
{
//	theLogger.Print(">> closesocket()\r\n");

	return closesocket(s);
}

int WINAPI Hook_connect(SOCKET s, const struct sockaddr FAR *name, int namelen)
{
//	theLogger.Print(">> connect()\r\n");

	return connect(s, name, namelen);
}

static unsigned int g_e = 0;

int WINAPI Hook_recv(SOCKET s, char FAR *buf, int len, int flags)
{
	int err = recv(s, buf, len, flags);

//	theLogger.Print(">> recv(%x)\r\n", err);
//	theLogger.Dump((unsigned char *)buf, err);

	return err;
}

int WINAPI Hook_send(SOCKET s, /*const*/ char FAR *buf, int len, int flags)
{
//	theLogger.Print(">> send(%x)\r\n", len);
//	theLogger.Dump((unsigned char *)buf, len);

	return send(s, buf, len, flags);
}
