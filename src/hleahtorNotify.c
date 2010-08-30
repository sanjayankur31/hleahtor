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
* File : hleahtorNotify.c
* 
* Synopsis:
* This file contains functions using the libnotify lib to send the required
* notifications
*
* Date : Sun Jun 27 2010
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <libnotify/notification.h>

#include "hleahtorNotify.h"

/* hleahtorNotify:
 * pops up a notification bubble
 *
 * args:
 * - struct hleahtorComicConfig *hleahtorComicNotify :
 *   Structure containing comic info
 *
 * - int hleahtorNumberOfComics 
 *
 * returns:
 * - int
 * - 0 on success
 * - non zero on failure
 */
int 
hleahtorNotify(struct hleahtorComicConfig *hleahtorComicNotify, int hleahtorNumberOfComics)
{
    /* declarations */
    NotifyNotification *hleahtorNotification;
    int i = 0;
    int hleahtorStringLen = 0;

    /* dummy char array to hold the summary */
    char hleahtorUpdateNotificationContent[1024];

    /* init notify */
    if(notify_init(HLEAHTOR_NAME) != TRUE)
    {
        fprintf(stderr,"Error initing notify.Exiting\n");

        return -1;
    }

    for(i = 0; i< hleahtorNumberOfComics; i++)
    {
        if(hleahtorComicNotify[i].hleahtorComicUpdateIsAvailable)
        {
            sprintf(hleahtorUpdateNotificationContent+hleahtorStringLen,"<b>%s</b> : <a href='%s'>%s </a>\n",hleahtorComicNotify[i].hleahtorComicName, hleahtorComicNotify[i].hleahtorComicURL,hleahtorComicNotify[i].hleahtorComicURL);
            hleahtorStringLen += (strlen(hleahtorComicNotify[i].hleahtorComicName) + 2*strlen(hleahtorComicNotify[i].hleahtorComicURL) + 27);
        }
    }
    hleahtorNotification = notify_notification_new("hleahtor : Updates are available!!",
            hleahtorUpdateNotificationContent,
            NULL,
            NULL);
    
    notify_notification_set_timeout(hleahtorNotification,5000);
    
    if(hleahtorStringLen)
    {
        if (!notify_notification_show (hleahtorNotification, NULL)) 
        {
                fprintf (stderr, "failed to send notification\n");
                return -1;
        }
    }

    g_object_unref (G_OBJECT (hleahtorNotification));
    return 0;
}

/* end of hleahtorNotify.c */
