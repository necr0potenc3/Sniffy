////////////////////////////////////////////////////////////////////////////////
//
// Patch.h
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

#ifndef _PATCH_H_
#define _PATCH_H_

#define PATCH_LINE_MAX		4096
#define PATCH_ELEMENT_MAX	4096

class CPatch
{
public:
	typedef struct
	{
		unsigned int address;
	} TElement;

	typedef CList<TElement *, TElement *> TElementList;

protected:
	char *m_strName;
	char *m_strTarget;

	unsigned int m_sendAddress;
	unsigned int m_recvAddress;

	TElementList m_sendList;
	TElementList m_recvList;

public:
	// Construct an empty patch instance
	CPatch();

	// Destruct instance
	virtual ~CPatch();

	// Clear all information and free memory allocations
	void Clear();

	// Get name of patch
	char *GetName();

	// Get address of original receive function
	unsigned int GetRecvAddress();

	// Get a reference to the receive element list
	TElementList& GetRecvList();

	// Get address of original send function
	unsigned int GetSendAddress();

	// Get a reference to the send element list
	TElementList& GetSendList();

	// Get filename of target
	char *GetTarget();

	// Load patch information for a given checksum/length pair
	bool LoadPatch(const char *strConfigFilename, unsigned int checksum, unsigned int length);

protected:
	// Create patch information from a configuration line
	bool Create(char *strInit);
};

#endif
