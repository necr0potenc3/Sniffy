o--------------------------------------------------------------------o
|                                                                    |
|                             S/N/I/F/F/Y                            |
|                                                                    |
|                             Version 1.2                            |
|                           (30. Jan. 2001)                          |
|                                                                    |
|        Copyright (C) 2000, 2001 Bruno 'Beosil' Heidelberger        |
|                                                                    |
o--------------------------------------------------------------------o



o License o----------------------------------------------------------o

   This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

   You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA



o What is it? o------------------------------------------------------o

   This Ignition plugin provides a mechanism to sniff the data-stream
between a server and a client of Ultime Online. The data is fetched
before encryption or after decryption and is given to the
corresponding callback function in the CLogger class. The default
behaviour (dump of the data to the file "SnifferLog.txt") can be
changed by modifying the CLogger class implementation.



o Setup o------------------------------------------------------------o

   IMPORTANT: This plugin needs Ignition 1.2 or higher to work. Make
sure that your Ignition.cfg and Sniffy.cfg file are up-to-date with
your client version.

   Following steps are necessary to set Sniffy up:

     - Put "Sniffy.dll" and "Sniffy.cfg" into the Ignition directory

     - Add the following line in the Ignition.cfg file:

         !module "Sniffy.dll"



o Website o----------------------------------------------------------o

   You can find the official website of Ignition/Sniffy with updates
and source at:

   http://www.beosil.com



o Author o-----------------------------------------------------------o

   Bruno 'Beosil' Heidelberger (beosil@swileys.com)



o History o----------------------------------------------------------o

   Version 1.2 (30. Jan. 2001)

     - UO:3D support added
     - Updated packet lengths
     - Fixed a dangling file handle

   Version 1.1 (9. Jan. 2001)

     - Fixed some version-dependent code

   Version 1.0 (22. Dec. 2000)

     - Initial release



o--------------------------------------------------------------------o
