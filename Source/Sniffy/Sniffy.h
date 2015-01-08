////////////////////////////////////////////////////////////////////////////////
//
// Sniffy.h
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

#ifndef _SNIFFY_H_
#define _SNIFFY_H_

#include "resource.h"

#define SNIFFY_INFO	"Sniffy 1.2"

// Error codes
#define SNIFFY_ERROR_NONE				0
#define SNIFFY_ERROR_NOPATCH			1
#define SNIFFY_ERROR_BADEXE				2
#define SNIFFY_ERROR_PROTECT			3

class CSniffyApp : public CWinApp
{
public:
	int Init(unsigned int checksum, unsigned int length);

	//{{AFX_VIRTUAL(CSniffyApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSniffyApp)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

// Get error text ( exported function )
extern char *GetErrorText(int error);

// Get info text ( exported function )
extern char *GetInfoText();

// Start target process and hook it ( exported function )
extern int Install(unsigned int checksum, unsigned int length);

//{{AFX_INSERT_LOCATION}}

#endif
