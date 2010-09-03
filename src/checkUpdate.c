/*
 * =====================================================================================
 *
 *       Filename:  checkUpdate.c
 *
 *    Description:  This file contains methods that are used in checking for new updates
 *
 *        Version:  1.0
 *        Created:  03/09/10 20:31:26
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "checkUpdate.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  isMoreRecent
 *  Description:  Compares two dates in the format "Wed, 16 Jun 2010 17:22:43 GMT"
 *
 *                Args :
 *                - char * date1 
 *                - const char * date2 : from the struct -> previous issue stored
 *
 *                returns : int
 *                - 1 if date1 is more recent tha1 date2
 *                - 0 if date2 is more recent than date1
 *
 * =====================================================================================
 */
int
isMoreRecent(char *date1, const char *date2)
{
    /* declarations */
    /* counter */
    int i = 0;

    struct hleahtorDate hleahtorDates[2];

    /* define a const char list of months */
    const char hleahtorMonthList[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    dbg("date1 : %s",date1);
    dbg("date2 : %s",date2);

    hleahtorDates[0].hleahtorDay = hleahtorDates[1].hleahtorDay = 0;
    hleahtorDates[0].hleahtorMonthInt = hleahtorDates[1].hleahtorMonthInt = 0;
    hleahtorDates[0].hleahtorYear = hleahtorDates[1].hleahtorYear = 0;

    /* parse string to fill up date structure */
    /* new date from header */
    sscanf(date1+5,"%d %s %d %d:%d:%d GMT",&(hleahtorDates[0].hleahtorDay),hleahtorDates[0].hleahtorMonthAlpha,&(hleahtorDates[0].hleahtorYear), \
            &(hleahtorDates[0].hleahtorHours),&(hleahtorDates[0].hleahtorMinutes),&(hleahtorDates[0].hleahtorSeconds));

    /* date stored in struct from config file */
    sscanf(date2+5,"%d %s %d %d:%d:%d GMT",&(hleahtorDates[1].hleahtorDay),hleahtorDates[1].hleahtorMonthAlpha,&(hleahtorDates[1].hleahtorYear), \
            &(hleahtorDates[1].hleahtorHours),&(hleahtorDates[1].hleahtorMinutes),&(hleahtorDates[1].hleahtorSeconds));

    /* get month in numerals */
    for (i = 1; i <= 12; i++){
        if(!strcmp(hleahtorMonthList[i-1],hleahtorDates[0].hleahtorMonthAlpha))
        {
            hleahtorDates[0].hleahtorMonthInt = i;
            break;
        }
    }
                
    for (i = 1; i <= 12; i++){
        if(!strcmp(hleahtorMonthList[i-1],hleahtorDates[1].hleahtorMonthAlpha))
        {
            hleahtorDates[1].hleahtorMonthInt = i;
            break;
        }
    }

    /* start comparison */
    if(hleahtorDates[0].hleahtorYear > hleahtorDates[1].hleahtorYear)
        return 1;
    else if(hleahtorDates[0].hleahtorYear < hleahtorDates[1].hleahtorYear)
        return 0;
    else 
    {
        dbg("%s","Equal years");
        if(hleahtorDates[0].hleahtorMonthInt > hleahtorDates[1].hleahtorMonthInt)
            return 1;
        else if(hleahtorDates[0].hleahtorMonthInt < hleahtorDates[1].hleahtorMonthInt)
            return 0;
        else 
        {
            dbg("%s","Equal months");
            if(hleahtorDates[0].hleahtorDay > hleahtorDates[1].hleahtorDay)
                return 1;
            else if(hleahtorDates[0].hleahtorDay < hleahtorDates[1].hleahtorDay)
                return 0;
            else 
            {
                dbg("%s","Equal days");
                if(hleahtorDates[0].hleahtorHours > hleahtorDates[1].hleahtorHours)
                    return 1;
                else if(hleahtorDates[0].hleahtorHours < hleahtorDates[1].hleahtorHours)
                    return 0;
                else 
                {
                    dbg("%s","Equal hours");
                    if(hleahtorDates[0].hleahtorMinutes > hleahtorDates[1].hleahtorMinutes)
                        return 1;
                    else if(hleahtorDates[0].hleahtorMinutes < hleahtorDates[1].hleahtorMinutes)
                        return 0;
                    else 
                    {
                        dbg("%s","Equal minutes");
                        if(hleahtorDates[0].hleahtorSeconds > hleahtorDates[1].hleahtorSeconds)
                            return 1;
                        else if(hleahtorDates[0].hleahtorSeconds < hleahtorDates[1].hleahtorSeconds)
                            return 0;
                    }
                }
            }
        }
    }
    dbg("%s","Equal seconds -> Returning 0");
    return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  hleahtorRealloc
 *  Description:  A customized function to take care of reallocs
 *                Also taken from http://curl.haxx.se/libcurl/c/getinmemory.html
 *
 *                Args:
 *                - void *ptr : pointer to be allocated memory
 *                - size : size of memory to be allocated
 *
 *                returns : static void*
 *                - the pointer
 * =====================================================================================
 */
static void *
hleahtorRealloc(void *ptr, size_t size)
{
    if(ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  hleahtorHeaderCallback
 *  Description:  Callback function that saves my header
 *
 *                Taken from curl example : http://curl.haxx.se/libcurl/c/getinmemory.html 
 *
 *
 *                Args:
 *                - void *ptr, size_t size, size_t nmemb, void *data
 *
 *                returns : size_t
 *                - size of data passed by curl
 * =====================================================================================
 */
size_t
hleahtorHeaderCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)data;
   
    mem->memory = hleahtorRealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) 
    {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  hleahtorIsUpdateAvailable
 *  Description:  Function that parses the chunk
 *
 *                Dates in the format "Wed, 16 Jun 2010 17:22:43 GMT"
 *                  
 *                Args:
 *                - struct MemoryStruct chunk : chunk holding header
 *                - struct hleahtorComicConfig *hleahtorComicConfigArray
 *
 *                returns : int
 *                - 1 if update available
 *                - 0 if not
 *                - something else otherwise
 * =====================================================================================
 */
int   
hleahtorIsUpdateAvailable(struct MemoryStruct chunk, struct hleahtorComicConfig *hleahtorComicConfigSingle)
{
    char hleahtorDummyString[50], *hleahtorTimeStampHeader, hleahtorDayAlpha[5];
    struct hleahtorDate hleahtorDateVar;

    /* get to the last-modified header */
    hleahtorTimeStampHeader = strstr(chunk.memory,"Last-Modified:");
    if(hleahtorTimeStampHeader == NULL)
    {
        fprintf(stderr,"Didn't find the Last-Modified tag!!\n");
        return 0;
    }
    dbg("%s","Found last modified tag");

    sscanf((hleahtorTimeStampHeader + (sizeof(char) * (int)strlen("Last-Modified:"))),"%s %d %s %d %d:%d:%d GMT",hleahtorDayAlpha, \
            &(hleahtorDateVar.hleahtorDay), hleahtorDateVar.hleahtorMonthAlpha, &(hleahtorDateVar.hleahtorYear),\
            &(hleahtorDateVar.hleahtorHours), &(hleahtorDateVar.hleahtorMinutes), &(hleahtorDateVar.hleahtorSeconds));
    hleahtorDayAlpha[3] = '\0';

    sprintf(hleahtorDummyString,"%s, %d %s %d %d:%d:%d GMT",hleahtorDayAlpha, hleahtorDateVar.hleahtorDay, hleahtorDateVar.hleahtorMonthAlpha,\
            hleahtorDateVar.hleahtorYear, hleahtorDateVar.hleahtorHours, hleahtorDateVar.hleahtorMinutes, hleahtorDateVar.hleahtorSeconds);

    dbg("%s","Got timestamp from header");

    dbg("First time stamp: %s",hleahtorComicConfigSingle->hleahtorComicPreviousIssueTimestamp);
    dbg("New time stamp: %s",hleahtorDummyString);

    hleahtorComicConfigSingle->hleahtorComicNewReceivedTimestamp = malloc(sizeof(char) * strlen(hleahtorDummyString));
    strcpy(hleahtorComicConfigSingle->hleahtorComicNewReceivedTimestamp,hleahtorDummyString);
    dbg("value = %s",hleahtorComicConfigSingle->hleahtorComicNewReceivedTimestamp);

    /* if first time, show update as available */
    if(!(strcmp(hleahtorComicConfigSingle->hleahtorComicPreviousIssueTimestamp,"0")))
    {
        dbg("First run for %s",hleahtorComicConfigSingle->hleahtorComicName);
        return 1;
    }


    /* returns 1 if more recent */
    if((isMoreRecent(hleahtorDummyString,hleahtorComicConfigSingle->hleahtorComicPreviousIssueTimestamp)))
    {
        dbg("%s","Update available");
        return 1;
    }    

    return 0;

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  hleahtorCheckUpdate
 *  Description:  This method checks for updates
 *                  
 *                  Args :
 *                  - struct hleahtorConfig hleahtorAppConfig : structure containing info
 *                    parsed from the main config file for app
 *                  - struct hleahtorComicConfig *hleahtorComicConfigArray : an array of 
 *                    the structure containing info parsed from the comic configurations
 *                    file
 *
 *                  returns : int
 *                  - 0 if successful
 *                  - non zero otherwise
 * =====================================================================================
 */
int
hleahtorCheckUpdate(struct hleahtorConfig hleahtorAppConfig, struct hleahtorComicConfig *hleahtorComicConfigArray)
{
    /* declarations */
    /* curl handle */
    CURL *hleahtorCURLHandle;

    /* counter */
    int i = 0;

    /* return value holder for curl_easy_perform */
    int hleahtorCURLReturnValue = 0;

    /* memory for the call back to store in */
    struct MemoryStruct chunk;

    /* initialize my cURL handle */
    hleahtorCURLHandle = curl_easy_init();
    dbg("%s","cURL initialized");

    /* Common options to be set for all comics */
    dbg("%s","cURL general options being set");
/*  curl_easy_setopt(hleahtorCURLHandle,CURLOPT_HEADER,1);*/
    curl_easy_setopt(hleahtorCURLHandle,CURLOPT_NOPROGRESS,1);
    curl_easy_setopt(hleahtorCURLHandle,CURLOPT_NOBODY,1);

    /* callback function */
    dbg("%s","cURL header function being set");
    curl_easy_setopt(hleahtorCURLHandle, CURLOPT_HEADERFUNCTION, hleahtorHeaderCallback);
    curl_easy_setopt(hleahtorCURLHandle, CURLOPT_WRITEHEADER, (void *)&chunk);
    curl_easy_setopt(hleahtorCURLHandle, CURLOPT_USERAGENT, "hleahtor/0.1.1");

    /* set up proxy */
    if(hleahtorAppConfig.hleahtorIsUsingProxy)
    {
        dbg("%s","cURL proxy options being set");
        curl_easy_setopt(hleahtorCURLHandle,CURLOPT_PROXYAUTH,CURLAUTH_ANY);
        curl_easy_setopt(hleahtorCURLHandle,CURLOPT_PROXY,hleahtorAppConfig.hleahtorProxyURL);
        curl_easy_setopt(hleahtorCURLHandle,CURLOPT_PROXYPORT,hleahtorAppConfig.hleahtorProxyPort);

        if(hleahtorAppConfig.hleahtorIsProxyAuthenticated)
        {
            curl_easy_setopt(hleahtorCURLHandle,CURLOPT_PROXYUSERNAME,hleahtorAppConfig.hleahtorProxyUsername);
            curl_easy_setopt(hleahtorCURLHandle,CURLOPT_PROXYPASSWORD,hleahtorAppConfig.hleahtorProxyPassword);
        }
    }

    /* customize for each url and perform required action */
    for(i = 0; i < hleahtorAppConfig.hleahtorNumberOfComics; i++)
    {
        if(hleahtorComicConfigArray[i].hleahtorComicIsEnabled)
        {
            dbg("cURL working for comic %d",i+1);

            /* initialize the chunk that will handle this data */
            chunk.memory=NULL; 
            chunk.size = 0;     

            curl_easy_setopt(hleahtorCURLHandle,CURLOPT_URL,hleahtorComicConfigArray[i].hleahtorComicFeedURL);
            hleahtorCURLReturnValue = curl_easy_perform(hleahtorCURLHandle);

            if(hleahtorCURLReturnValue)
            {
                fprintf(stderr,"[[ERROR]] : (%d) : %s\n",hleahtorCURLReturnValue,curl_easy_strerror(hleahtorCURLReturnValue));
                return -1;
            }

            hleahtorComicConfigArray[i].hleahtorComicUpdateIsAvailable = hleahtorIsUpdateAvailable(chunk,&hleahtorComicConfigArray[i]);
            if(hleahtorComicConfigArray[i].hleahtorComicUpdateIsAvailable)
            {
                dbg("Function returned %d\n",hleahtorComicConfigArray[i].hleahtorComicUpdateIsAvailable);
                dbg("Update available for %s\n",hleahtorComicConfigArray[i].hleahtorComicName);
            }
            else
            {
                dbg("Function returned %d\n",hleahtorComicConfigArray[i].hleahtorComicUpdateIsAvailable);
                dbg("Update not available for %s\n",hleahtorComicConfigArray[i].hleahtorComicName);
            }

            
            /* free after called once for a comic */
            if(chunk.memory)
                free(chunk.memory);
        }
    }
    /* update my config file 
     * replace last issue with this newly acquired date 
     */
    if(hleahtorUpdateConfig(hleahtorAppConfig.hleahtorNumberOfComics,hleahtorComicConfigArray))
    {
        fprintf(stderr,"Error updating config file, please check debug logs by using -d\n");
        return -1;
    }

    /* clean up my mess */
    curl_easy_cleanup(hleahtorCURLHandle);
    return 0;
}

/* end of checkUpdate.c */
