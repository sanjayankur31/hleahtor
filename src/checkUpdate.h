/* Copyright 2010 Ankur Sinha 
* Author: Ankur Sinha <sanjay DOT ankur AT gmail DOT com> 
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* File : checkUpdate.h
* 
* Synopsis: header file for checkUpdate.c
*
* Date : Tue, 22 Jun 2010
*/

#include "hleahtorCommon.h"
/* debugs */
extern int hleahtorDebug;

/* for the call back */
struct MemoryStruct {
    char *memory;
    size_t size;
};

/* structure for easy date comparison */
struct hleahtorDate{
    int hleahtorDay;
    char hleahtorMonthAlpha[5];
    int hleahtorMonthInt;
    int hleahtorYear;
    int hleahtorHours;
    int hleahtorMinutes;
    int hleahtorSeconds;
};

int
hleahtorUpdateConfig(int hleahtorNumberOfComics,struct hleahtorComicConfig *hleahtorComicConfigArray);

/* end of checkUpdate.h */
