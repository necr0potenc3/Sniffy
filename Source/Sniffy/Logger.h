////////////////////////////////////////////////////////////////////////////////
//
// Logger.h
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
//	This is the logger class. It analyzes all packets from server and client.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "resource.h"

class CLogger
{
protected:
	FILE *m_hFile;

public:
	// Constructor/Destructor
	CLogger();
	~CLogger();

public:
	// Callback functions
	void OnClientPacket(unsigned char *pBuffer, unsigned int length);
	void OnServerPacket(unsigned char *pBuffer, unsigned int length);

	// dump functions
	void Dump(unsigned char *pBuffer, int length);
	void Print(const char *strFormat, ...);
};

extern CLogger theLogger;

#endif
