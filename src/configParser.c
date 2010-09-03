/*
 * =====================================================================================
 *
 *       Filename:  configParser.c
 *
 *    Description:  This file contains the functions that parse the config
 *                  files and populate the various structures needed by the app.
 *
 *                  See hleahtorCommon.h for structure definitions
 *
 *        Version:  1.0
 *        Created:  15/06/10 20:31:26
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

/* a Macro for error printing */
#define hleahtorConfigError(config,location) \
    do{             \
        fprintf(stderr,"Error at : %s\n%s %d\n",location,config_error_text(config),config_error_line(config)); \
    }while(0);      


#include <libconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configParser.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  hleahtorParseMain
 *  Description:  Parses the mail hleahtor.cfg file and sets up the environment for the app.
 *
 *                Args:
 *                - struct hleahtorConfig *hleahtorAppConfig : pointer to structure passed from \
 *                  main to fill up
 * 
 *                returns : int
 *                - 0 if successful
 *                - non zero otherwise
 * =====================================================================================
 */
int 
hleahtorParseMain(struct hleahtorConfig *hleahtorAppConfig)
{
    /* declarations */
    /* a config instance */
    config_t hleahtorConfigHandle;

    /* counter */
    long int i = 0;

    /* dummy string to hold access paths */
    char hleahtorStringPath[50];

    /* initialize libconfig */
    dbg("%s","Declarations done : initializing libconfig");
    config_init(&hleahtorConfigHandle);

    /* read the config file */
    dbg("%s","Config file being read");
    if(config_read_file(&hleahtorConfigHandle, HLEAHTOR_APP_CONFIG_FILE) == CONFIG_FALSE)
    {
        hleahtorConfigError(&hleahtorConfigHandle,"read file");
        return -1;
    }

    /* begin parsing file */
    dbg("%s","Parsing begins");
    if(config_lookup_string(&hleahtorConfigHandle,"application.name",&(hleahtorAppConfig->hleahtorName)) == CONFIG_FALSE)
    {
        hleahtorConfigError(&hleahtorConfigHandle,"application.name");
        return -2;
    }
    dbg("%s","Getting number of comics from cfg");
    if(config_lookup_int(&hleahtorConfigHandle,"application.number",&(hleahtorAppConfig->hleahtorNumberOfComics)) == CONFIG_FALSE)
    {
        hleahtorConfigError(&hleahtorConfigHandle,"application.number");
        return -2;
    }


    /* initialize my comic name array
     * Dont need to provide space to the strings : libconfig takes care of that 
     */
    dbg("%s","Getting comic names");
    (*hleahtorAppConfig).hleahtorListOfComics = calloc(hleahtorAppConfig->hleahtorNumberOfComics, sizeof(char*));
    if(hleahtorAppConfig->hleahtorListOfComics == NULL)
    {
        fprintf(stderr,"Error allocating memory at %d\n",__LINE__);
        return -3;
    }

    for(i = 0; i< hleahtorAppConfig->hleahtorNumberOfComics; i++)
    {
        sprintf(hleahtorStringPath,"application.comics.[%ld]",i);
        if(config_lookup_string(&hleahtorConfigHandle,hleahtorStringPath,&(hleahtorAppConfig->hleahtorListOfComics[i])) == CONFIG_FALSE)
        {
            hleahtorConfigError(&hleahtorConfigHandle,"application.comics");
            return -2;
        }
    }

    /* proxy info */
    hleahtorAppConfig->hleahtorIsUsingProxy = 0;
    hleahtorAppConfig->hleahtorIsProxyAuthenticated = 0;
    dbg("%s","Parsing proxy info");
    if(config_lookup_string(&hleahtorConfigHandle,"application.proxy",&(hleahtorAppConfig->hleahtorProxyURL)) == CONFIG_TRUE)
    {
        dbg("%s","Proxy info found");
        hleahtorAppConfig->hleahtorIsUsingProxy = 1;
        if(config_lookup_string(&hleahtorConfigHandle,"application.proxyPort",&(hleahtorAppConfig->hleahtorProxyPort)) == CONFIG_FALSE)
        {
            dbg("%s","WARNING : No proxy port specfied!");
        }
        if(config_lookup_string(&hleahtorConfigHandle,"application.proxyUsername",&(hleahtorAppConfig->hleahtorProxyUsername)) == CONFIG_FALSE)
        {
            dbg ("%s","No proxy username found. Unauthenticated proxy");
        }
        else
        {
            hleahtorAppConfig->hleahtorIsProxyAuthenticated = 1;
            dbg("%s","Proxy authentication found");
            if(config_lookup_string(&hleahtorConfigHandle,"application.proxyPassword",&(hleahtorAppConfig->hleahtorProxyPassword)) == CONFIG_FALSE)
            {
                dbg("%s","No password found for authenticated proxy. -> weird \n");
            }
        }
    }
    else
    {
        dbg("%s","No proxy info found");
    }

    return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  hleahtorParseComic
 *  Description:  Parses the mail hleahtor.cfg file and sets up the environment for the app.
 *
 *                Args:
 *                - struct hleahtorComicConfig *hleahtorComicConfigArray : pointer to array of the \
 *                  structure passed from main to fill up
 *                - int numberOfComics
 * 
 *                returns : int
 *                - 0 if successful
 *                - non zero otherwise
 * =====================================================================================
 */
int 
hleahtorParseComic(struct hleahtorComicConfig *hleahtorComicConfigArray,long int numberOfComics)
{
    /* declarations */
    /* a config instance */
    config_t hleahtorConfigHandle1;

    /* counter */
    long int i = 0;

    /* dummy string to hold access paths */
    char hleahtorStringPath[50] ;

    /* initialize libconfig */
    dbg("%s","Declarations done : initializing libconfig");
    config_init(&hleahtorConfigHandle1);

    /* read the config file */
    dbg("%s","Config file being read");
    if(config_read_file(&hleahtorConfigHandle1,  HLEAHTOR_COMICS_CONFIG_FILE) == CONFIG_FALSE)
    {
        hleahtorConfigError(&hleahtorConfigHandle1,"read file");
        return -1;
    }

    dbg("%s","Getting various fields");
    for( i = 0; i< numberOfComics ; i++)
    {
        sprintf(hleahtorStringPath,"application.comics.[%ld].name",i);
        if(config_lookup_string(&hleahtorConfigHandle1,hleahtorStringPath,&(hleahtorComicConfigArray[i].hleahtorComicName)) == CONFIG_FALSE)
        {
            hleahtorConfigError(&hleahtorConfigHandle1,hleahtorStringPath);
            return -2;
        }
        sprintf(hleahtorStringPath,"application.comics.[%ld].feedURL",i);
        if(config_lookup_string(&hleahtorConfigHandle1,hleahtorStringPath,&(hleahtorComicConfigArray[i].hleahtorComicFeedURL)) == CONFIG_FALSE)
        {
            hleahtorConfigError(&hleahtorConfigHandle1,hleahtorStringPath);
            return -2;
        }
        sprintf(hleahtorStringPath,"application.comics.[%ld].URL",i);
        if(config_lookup_string(&hleahtorConfigHandle1,hleahtorStringPath,&(hleahtorComicConfigArray[i].hleahtorComicURL)) == CONFIG_FALSE)
        {
            hleahtorConfigError(&hleahtorConfigHandle1,hleahtorStringPath);
            return -2;
        }
        sprintf(hleahtorStringPath,"application.comics.[%ld].enabled",i);
        if(config_lookup_int(&hleahtorConfigHandle1,hleahtorStringPath,&(hleahtorComicConfigArray[i].hleahtorComicIsEnabled)) == CONFIG_FALSE)
        {
            hleahtorConfigError(&hleahtorConfigHandle1,hleahtorStringPath);
            return -2;
        }
        sprintf(hleahtorStringPath,"application.comics.[%ld].previousIssue",i);
        if(config_lookup_string(&hleahtorConfigHandle1,hleahtorStringPath,&(hleahtorComicConfigArray[i].hleahtorComicPreviousIssueTimestamp)) == CONFIG_FALSE)
        {
            hleahtorConfigError(&hleahtorConfigHandle1,hleahtorStringPath);
            return -2;
        }
        hleahtorComicConfigArray[i].hleahtorComicUpdateIsAvailable = 0;
    }


    return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  hleahtorUpdateConfig
 *  Description:  Updates the config file. Overwrites the previous issue date with the newly acquired one
 *
 *                Args:
 *                - int hleahtorNumberOfComics
 *                - Struct hleahtorComicConfig *hleahtorComicConfigArray
 *
 *                returns : int
 *                - 0 if successful
 *                - non zero otherwise
 * =====================================================================================
 */
int
hleahtorUpdateConfig(int hleahtorNumberOfComics,struct hleahtorComicConfig *hleahtorComicConfigArray)
{
    /* declarations */
    config_t hleahtorConfigHandle;
    config_setting_t *hleahtorConfigSettingPtr;

    /* counter */
    long int i = 0;
    
    /* dummy string to hold access paths */
    char hleahtorStringPath[50];

    /* initialize libconfig */
    dbg("%s","Declarations done : initializing libconfig");
    config_init(&hleahtorConfigHandle);

    /* read the config file */
    dbg("%s","Config file being read");
    if(config_read_file(&hleahtorConfigHandle, HLEAHTOR_COMICS_CONFIG_FILE) == CONFIG_FALSE)
    {
        hleahtorConfigError(&hleahtorConfigHandle,"read file");
        return -1;
    }

    /* begin updating file */
    for(i = 0; i< hleahtorNumberOfComics; i++)
    {
        memset(hleahtorStringPath,0,50);
        dbg("%s","Config file being updated");
        sprintf(hleahtorStringPath,"application.comics.[%ld].previousIssue",i);

        if((hleahtorConfigSettingPtr = config_lookup(&hleahtorConfigHandle,hleahtorStringPath)) == NULL)
        {
            hleahtorConfigError(&hleahtorConfigHandle,hleahtorStringPath);
            return -2;
        }
        dbg("%s","Setting in progress");
        dbg("value = %s",hleahtorComicConfigArray[i].hleahtorComicNewReceivedTimestamp);
        if(config_setting_set_string(hleahtorConfigSettingPtr,hleahtorComicConfigArray[i].hleahtorComicNewReceivedTimestamp) == CONFIG_FALSE)
        {
            hleahtorConfigError(&hleahtorConfigHandle,"Setting string");
            fprintf(stderr,"Error setting the string\n");
            return -3;
        }
        dbg("%s","Writing the config in progress progress");
        if(config_write_file(&hleahtorConfigHandle,HLEAHTOR_COMICS_CONFIG_FILE) == CONFIG_FALSE)
        {
            hleahtorConfigError(&hleahtorConfigHandle,"Writing file");
            return -5;
        }
    }
                
    config_destroy(&hleahtorConfigHandle);
    return 0;
}

/* end of configParser.c */
