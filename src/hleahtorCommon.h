/*
 * =====================================================================================
 *
 *       Filename:  hleahtorCommon.h
 *
 *    Description:  A common header files holding headers used in all source files etc.
 *
 *        Version:  1.0
 *        Created:  22/06/10 20:32:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ankur Sinha (FranciscoD), sanjay DOT ankur AT gmail DOT com
 *        Company:  
 *
 * =====================================================================================
 */

/* Copyright 2010 Ankur Sinha 
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
 */

#define dbg(format,message ) 										\
	do {												\
		if(hleahtorDebug) 										\
			printf("[DEBUG] %s: (%d) : "format"\n", __func__ , __LINE__ , message);	\
	}while(0);

/* structure to hold vals for the application */
struct hleahtorConfig
{
    const char *hleahtorName;
    long int hleahtorNumberOfComics;
    int hleahtorIsUsingProxy;
    const char **hleahtorListOfComics;
    const char *hleahtorProxyURL;
    const char *hleahtorProxyPort;
    int hleahtorIsProxyAuthenticated;
    const char *hleahtorProxyUsername;
    const char *hleahtorProxyPassword;
};

/* structure to hold info per comic */
struct hleahtorComicConfig
{
    const char *hleahtorComicName;
    const char *hleahtorComicURL;
    const char *hleahtorComicFeedURL;
    long int hleahtorComicIsEnabled;
    const char *hleahtorComicPreviousIssueTimestamp;
    const char *hleahtorComicNewReceivedTimestamp;
    int hleahtorComicUpdateIsAvailable;

};

/* version macros */
#define HLEAHTOR_VERSION_MAJOR          0
#define HLEAHTOR_VERSION_MINOR          2
#define HLEAHTOR_VERSION_REVISION       2
#define HLEAHTOR_NAME               "hleahtor"

#define HLEAHTOR_USER_CONFIG_DIR    ".hleahtor/"
#define HLEAHTOR_APP_CONFIG_FILE    "hleahtor.cfg"
#define HLEAHTOR_COMICS_CONFIG_FILE "comics.cfg"

/* end of hleahtorCommon.h */
