////////////////////////////////////////////////////////////////////////////////
//
// Logger.cpp
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

#include "StdAfx.h"
#include "Logger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



////////////////////////////////////////////////////////////////////////////////
//
//	The one and only logger instance
//
////////////////////////////////////////////////////////////////////////////////

CLogger theLogger;



////////////////////////////////////////////////////////////////////////////////
//
//	Constructor/Destructor
//
////////////////////////////////////////////////////////////////////////////////

CLogger::CLogger()
{
	m_hFile = fopen("SniffyLog.txt", "wb");
}

CLogger::~CLogger()
{
	if(m_hFile) fclose(m_hFile);
}



////////////////////////////////////////////////////////////////////////////////
//
//	Callback functions
//
////////////////////////////////////////////////////////////////////////////////

void CLogger::OnClientPacket(unsigned char *pBuffer, unsigned int length)
{
	fprintf(m_hFile, "-------------------------------- client --------------------------------\r\n");
	Dump(pBuffer, length);
}

void CLogger::OnServerPacket(unsigned char *pBuffer, unsigned int length)
{
	fprintf(m_hFile, "-------------------------------- server --------------------------------\r\n");
	Dump(pBuffer, length);
}



////////////////////////////////////////////////////////////////////////////////
//
//	Dump functions
//
////////////////////////////////////////////////////////////////////////////////

void CLogger::Dump(unsigned char *pBuffer, int length)
{
	if(length > 4096) length = 4096;

	for(int actLine = 0; actLine < (length / 16) + 1; actLine++)
	{
		fprintf(m_hFile, "%04x: ", actLine * 16);

		for(int actRow = 0; actRow < 16; actRow++)
		{
			if(actLine * 16 + actRow < length) fprintf(m_hFile, "%02x ", (unsigned int)((unsigned char)pBuffer[actLine * 16 + actRow]));
			else fprintf(m_hFile, "-- ");
		}

		fprintf(m_hFile, ": ");

		for(actRow = 0; actRow < 16; actRow++)
		{
			if(actLine * 16 + actRow < length) fprintf(m_hFile, "%c", isprint(pBuffer[actLine * 16 + actRow]) ? pBuffer[actLine * 16 + actRow] : '.');
		}

		fprintf(m_hFile, "\r\n");
	}

	fprintf(m_hFile, "\r\n");

	fflush(m_hFile);
}

void CLogger::Print(const char *strFormat, ...)
{
  va_list arg;
  va_start(arg, strFormat);

  vfprintf(m_hFile, strFormat, arg);
}
