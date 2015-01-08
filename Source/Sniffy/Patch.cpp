////////////////////////////////////////////////////////////////////////////////
//
// Patch.cpp
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
// This class handles the whole patching information.
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Patch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



////////////////////////////////////////////////////////////////////////////////
//
//	Construct an empty patch instance
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		-none-
//
////////////////////////////////////////////////////////////////////////////////

CPatch::CPatch()
{
	m_strName = 0;
	m_strTarget = 0;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Destruct instance
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		-none-
//
////////////////////////////////////////////////////////////////////////////////

CPatch::~CPatch()
{
	Clear();
}



////////////////////////////////////////////////////////////////////////////////
//
//	Clear all information and free memory allocations
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		-none-
//
////////////////////////////////////////////////////////////////////////////////

void CPatch::Clear()
{
	delete [] m_strName;
	delete [] m_strTarget;

	m_sendAddress = 0;
	m_recvAddress = 0;

	while(!m_recvList.IsEmpty())
	{
		TElement *pElement = m_recvList.RemoveHead();

		delete pElement;
	}

	while(!m_sendList.IsEmpty())
	{
		TElement *pElement = m_sendList.RemoveHead();

		delete pElement;
	}
}



////////////////////////////////////////////////////////////////////////////////
//
//	Create patch information from a configuration line
//
//	PARAMETERS:
//		char *strLine		Configuration line
//
//	RETURNS:
//		bool				"true" if successful, "false" if not
//
////////////////////////////////////////////////////////////////////////////////

bool CPatch::Create(char *strLine)
{
	Clear();

	char *pToken = strtok(strLine, ",\n\r");

	while(pToken != 0)
	{
		pToken += strspn(pToken, " \t");

		bool bSend;

		if(!strncmp(pToken, "send", 4)) bSend = true;
		else if(!strncmp(pToken, "recv", 4)) bSend = false;
		else return false;

		pToken += 4;
		pToken += strspn(pToken, " \t");

		unsigned int address;
		if(sscanf(pToken, "%x", &address) != 1) return false;

		if(bSend) m_sendAddress = address;
		else m_recvAddress = address;

		if((pToken = strchr(pToken, '=')) == 0) continue;
		pToken++;

		pToken += strspn(pToken, " \t");

		while(*pToken != 0)
		{
			unsigned int value;
			if(sscanf(pToken, "%x", &value) != 1) return false;

			TElement *pElement = new TElement;
			pElement->address = value;

			if(bSend) m_sendList.AddTail(pElement);
			else m_recvList.AddTail(pElement);

			pToken += strspn(pToken, "01234567890abcdefABCDEF");
			pToken += strspn(pToken, " \t");
		}

		pToken = strtok(0, ",\n\r");
	}

	return true;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Get name of patch
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		char *		Name of patch
//
////////////////////////////////////////////////////////////////////////////////

char *CPatch::GetName()
{
	return m_strName;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Get address of original receive function
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		unsigned int	Address of function
//
////////////////////////////////////////////////////////////////////////////////

unsigned int CPatch::GetRecvAddress()
{
	return m_recvAddress;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Get a reference to the receive element list
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		CPatch::TElementList&	Reference to list
//
////////////////////////////////////////////////////////////////////////////////

CPatch::TElementList& CPatch::GetRecvList()
{
	return m_recvList;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Get address of original send function
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		unsigned int	Address of function
//
////////////////////////////////////////////////////////////////////////////////

unsigned int CPatch::GetSendAddress()
{
	return m_sendAddress;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Get a reference to the send element list
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		CPatch::TModuleList&	Reference to list
//
////////////////////////////////////////////////////////////////////////////////

CPatch::TElementList& CPatch::GetSendList()
{
	return m_sendList;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Get filename of target
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		char *		Fileame of target
//
////////////////////////////////////////////////////////////////////////////////

char *CPatch::GetTarget()
{
	return m_strTarget;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Load patch information for a given checksum/length pair
//
//	PARAMETERS:
//		const char *strConfigFilename	Filename of configuration file
//		unsigned int checksum			Checksum of target
//		unsigned int length				Length of target
//
//	RETURNS:
//		bool							"true" if successful, "false" if not
//
////////////////////////////////////////////////////////////////////////////////

bool CPatch::LoadPatch(const char *strConfigFilename, unsigned int checksum, unsigned int length)
{
	FILE *hFile;
	if((hFile = fopen(strConfigFilename, "rb")) == 0) return false;

	while(true)
	{
		char buffer[PATCH_LINE_MAX];
		fgets(buffer, PATCH_LINE_MAX, hFile);

		if(feof(hFile)) break;

		char *pToken = &buffer[strspn(buffer, " \t\n\r")];

		if(*pToken++ != '\"') continue;

		char *strName = pToken;

		if((pToken = strchr(pToken, '\"')) == 0) continue;

		*pToken++ = 0;

		unsigned int currentChecksum;
		unsigned int currentLength;

		if(sscanf(pToken, "%x %x", &currentChecksum, &currentLength) != 2) continue;

		if((currentChecksum != checksum) || (currentLength != length)) continue;

		if((pToken = strchr(pToken, ':')) == 0) continue;

		if(!Create(pToken + 1)) continue;

		m_strName = new char[strlen(strName) + 1];
		strcpy(m_strName, strName);

		fclose(hFile);

		return true;
	}

	fclose(hFile);

	return false;
}
