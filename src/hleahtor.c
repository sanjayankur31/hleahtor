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
* File : hleahtor.c
*
* Synopsis:
* This file contains the function "main". The main function 
* is the callee for most of the other methods. 
*
* Date : Tue, Jun 15 2010.
* 
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#include "hleahtor.h"

int hleahtorDebug = 0;

/* main:
 * main function for the hleahtor app which invokes its 
 * other parts
 *
 * args:
 * - int argc : number of arguments on command line
 *
 * - char **argv : an array that stores the arguments
 *
 * returns:
 * - int
 * - 0 on successful functioning
 * - non zero on error
 */
int 
main(int argc, char **argv){
    /* Declarations */

    /* optstring : string to pass to getopt(3) */
    const char *hleahtor_optstring = "d::h::v::l::";

    /* holds the option returned by getopt */
    char hleahtor_option = 0;

    /* structure for comic configration */
    struct hleahtorConfig hleahtorAppConfig;

    /* structure array for comics */
    struct hleahtorComicConfig *hleahtorComicConfigArray;

    /* misc */
    int i = 0;

    /* environment */
    char *hleahtorUserHomeDir = NULL;
    FILE *hleahtorAppFile, *hleahtorSysDirFile;
    char ch;
    int hleahtorRetVal = 0;

    /* Parse command line args.
     * This is only for debugging purposes.
     * -d : print debug messages
     * -h : print usage
     * -v : print version and exit
     */
    while((hleahtor_option = getopt(argc,argv,hleahtor_optstring)) != -1)
    {
        switch (hleahtor_option)
        {
            case 'd' : 
                hleahtorDebug = 1;
                break;
            case 'h' :
                printf("Usage : hleahtor [-d] [-l]  [-h] [-v]\n" \
                        "-d : print debug messages\n" \
                        "-l : print list of comics defined by config file\n" \
                        "-h : print this message and exit\n" \
                        "-v : print version and exit\n" \
                        "Ankur Sinha <ankursinha AT fedoraproject DOT org>\n");
            case 'v' :
                printf("hleahtor Version %d.%d.%d\n",HLEAHTOR_VERSION_MAJOR,HLEAHTOR_VERSION_MINOR,HLEAHTOR_VERSION_REVISION);
                return 0;
            case 'l' :
                printf("List of comics:\n");
                /* parse hleahtor.cfg */
                if(hleahtorParseMain(&hleahtorAppConfig))
                {
                    fprintf(stderr,"Error parsing hleahtor.cfg. Please use -d flag to run in debug mode.");
                    return -1;
                }
                /* memory allocation to array */
                hleahtorComicConfigArray = malloc((sizeof(struct hleahtorComicConfig )) * hleahtorAppConfig.hleahtorNumberOfComics);
                if(hleahtorComicConfigArray == NULL)
                {
                    fprintf(stderr,"Error allocating memory to ComicArray.\n");
                    return -2;
                }
                if(hleahtorParseComic(hleahtorComicConfigArray,hleahtorAppConfig.hleahtorNumberOfComics))
                {
                    fprintf(stderr,"Error parsing comics. Please check comics.cfg\n");
                    return -1;
                }
                for(i = 0; i< hleahtorAppConfig.hleahtorNumberOfComics; i++)
                    printf("[%i] %s -> %s\n" , i+1, hleahtorComicConfigArray[i].hleahtorComicName,hleahtorComicConfigArray[i].hleahtorComicURL);

                free(hleahtorComicConfigArray);
                return 0;
        }
    };

    /* set up application environment */
    if((hleahtorUserHomeDir = getenv("HOME")) == NULL)
    {
       fprintf(stderr,"Couldnt get value of $HOME. Exiting\n");
       return -99;
    }
    if(chdir(hleahtorUserHomeDir) == -1)
    {
        fprintf(stderr,"Error changing directory to ~/.\n%s\nExiting\n",strerror(errno));
        return -98;
    }
    /* create working directory for hleahtor if it doenst already exist 
     * TODO
     * An EEXIST does not necessarily mean that the DIRECTORY exists, could be a file too.
     * This might never happen, but someone fooling around (like me ;)) might manage to create this situation
     */
    dbg("%s","Creating ~/.hleahtor");
    if((hleahtorRetVal = mkdir(HLEAHTOR_USER_CONFIG_DIR,0700)) == -1)
    {
        if(errno != EEXIST)
        {
            fprintf(stderr,"Error creating directory.\n%s\nExiting\n",strerror(errno));
            return -97;
        }
    }
    chdir(HLEAHTOR_USER_CONFIG_DIR);
    dbg("%s","Directory changed to ~/.hleahtor");

    /* check existence of config files else create them
     * copy contents of files from the sysconfdir to user specific local dir
     */
    /* hleahtor conf file */
    if((hleahtorAppFile = fopen(HLEAHTOR_APP_CONFIG_FILE,"r")) == NULL)
    {
        dbg("%s","Hleahtor conf file not found. Creating..");
        /* create file */
        hleahtorAppFile = fopen(HLEAHTOR_APP_CONFIG_FILE,"w");
        hleahtorSysDirFile = fopen(DATADIR "/" HLEAHTOR_NAME "/" HLEAHTOR_APP_CONFIG_FILE,"r");
        dbg("Reading file %s",DATADIR "/" HLEAHTOR_NAME "/" HLEAHTOR_APP_CONFIG_FILE);
        if(hleahtorAppFile == NULL)
        {
            fprintf(stderr,"Error creating config files\n");
            return -96;
        }
        if(hleahtorSysDirFile == NULL)
        {
            fprintf(stderr,"Error opening sys config files\n");
            return -96;
        }
        /* copy the file */
        dbg("%s","Writing new hleahtor conf file");
        hleahtorRetVal = 0;
        while(!feof(hleahtorSysDirFile)) 
        {
            hleahtorRetVal++;
            ch = fgetc(hleahtorSysDirFile);
            if(ferror(hleahtorSysDirFile)) 
            {
                printf("Error reading source file.\n");
                fclose(hleahtorAppFile);
                fclose(hleahtorSysDirFile);
                return -95;
            }
            if(!feof(hleahtorSysDirFile)) 
                fputc(ch, hleahtorAppFile);
            if(ferror(hleahtorAppFile)) 
            {
                printf("Error writing destination file.\n");
                fclose(hleahtorAppFile);
                fclose(hleahtorSysDirFile);
                return -95;
            }
        } 
        dbg("Characters written : %d",hleahtorRetVal);
        fclose(hleahtorAppFile);
        fclose(hleahtorSysDirFile);
    }
    /* comics configuration file */
    if((hleahtorAppFile = fopen(HLEAHTOR_COMICS_CONFIG_FILE,"r")) == NULL)
    {
        dbg("%s","Comics conf file not found. Creating..");
        /* create file */
        hleahtorAppFile = fopen(HLEAHTOR_COMICS_CONFIG_FILE,"w");
        hleahtorSysDirFile = fopen(DATADIR "/" HLEAHTOR_NAME "/" HLEAHTOR_COMICS_CONFIG_FILE,"r");
        if(hleahtorAppFile == NULL)
        {
            fprintf(stderr,"Error creating comic config files\n");
            return -96;
        }
        if(hleahtorSysDirFile == NULL)
        {
            fprintf(stderr,"Error opening sys comic config files\n");
            return -96;
        }
        /* copy the file
         * TODO : make this another function? */
        dbg("%s","Writing new comics conf file");
        hleahtorRetVal = 0;
        while(!feof(hleahtorSysDirFile)) 
        {
            ch = fgetc(hleahtorSysDirFile);
            if(ferror(hleahtorSysDirFile)) 
            {
                printf("Error reading source file.\n");
                fclose(hleahtorAppFile);
                fclose(hleahtorSysDirFile);
                return -95;
            }
            if(!feof(hleahtorSysDirFile)) 
                fputc(ch, hleahtorAppFile);
            if(ferror(hleahtorAppFile)) 
            {
                printf("Error writing destination file.\n");
                fclose(hleahtorAppFile);
                fclose(hleahtorSysDirFile);
                return -95;
            }
        } 
        fclose(hleahtorAppFile);
        fclose(hleahtorSysDirFile);
        dbg("Characters written : %d",hleahtorRetVal);
    }
    dbg("%s","Environment set up");

    /* init curl */
    if(curl_global_init(CURL_GLOBAL_ALL))
    {
        fprintf(stderr,"Error initing curl.Exiting\n");
        curl_global_cleanup();
        return -1;
    }

    /* parse hleahtor.cfg */
    if(hleahtorParseMain(&hleahtorAppConfig))
    {
        fprintf(stderr,"Error parsing hleahtor.cfg. Please use -d flag to run in debug mode.");
        return -1;
    }

    /* debug messages */
    dbg("Number of comics mentioned in hleahtor.cfg = %ld",hleahtorAppConfig.hleahtorNumberOfComics);
    if(hleahtorAppConfig.hleahtorIsUsingProxy == 1)
    {
        dbg("ProxyURL = %s",hleahtorAppConfig.hleahtorProxyURL);
        dbg("ProxyPort = %s",hleahtorAppConfig.hleahtorProxyPort);
        dbg("ProxyUsername = %s",hleahtorAppConfig.hleahtorProxyUsername);
        dbg("ProxyPassword = %s",hleahtorAppConfig.hleahtorProxyPassword);
    }

    /* memory allocation to array */
    hleahtorComicConfigArray = malloc((sizeof(struct hleahtorComicConfig )) * hleahtorAppConfig.hleahtorNumberOfComics);
    if(hleahtorComicConfigArray == NULL)
    {
        fprintf(stderr,"Error allocating memory to ComicArray.\n");
        return -2;
    }
    dbg("%s","Memory allocated to hleahtorComicConfigArray");

    /* parsing individual comic info now */
    dbg("%s","Parsing comic info now");
    if(hleahtorParseComic(hleahtorComicConfigArray,hleahtorAppConfig.hleahtorNumberOfComics))
    {
        fprintf(stderr,"Error parsing comics. Please check comics.cfg\n");
        return -1;
    }

    if(hleahtorDebug)
    {
        for(i = 0; i< hleahtorAppConfig.hleahtorNumberOfComics; i++)
        {
            dbg("Comic name : %s",hleahtorComicConfigArray[i].hleahtorComicName);
            dbg("Comic feed url : %s ",hleahtorComicConfigArray[i].hleahtorComicFeedURL);
            dbg("Comic url : %s ",hleahtorComicConfigArray[i].hleahtorComicURL);
            dbg("Comic is enabled : %ld ",hleahtorComicConfigArray[i].hleahtorComicIsEnabled);
        }
    }

    while(1)
    {
        /* check for update */
        dbg("%s","Checking for updates");
        if(hleahtorCheckUpdate(hleahtorAppConfig,hleahtorComicConfigArray))
        {
            fprintf(stderr,"Error checking for updates. Please run in debug mode\n");
            return -1;
        }
        
        if(hleahtorNotify(hleahtorComicConfigArray,hleahtorAppConfig.hleahtorNumberOfComics))
        {
            fprintf(stderr,"Error sending notifications. Please run in debug mode\n");
            return -1;
        }

        sleep(24*60*60);
    }

    /* clean up my mess */
    free(hleahtorComicConfigArray);

    /* close curl */
    curl_global_cleanup();
    return 0;
}
                        
/* end of hleahtor.c */
