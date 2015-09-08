#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "include/log.h"
#include "include/config.h"

#define CONFIGFILE "/home/root/capture.conf"
/*
static int Write_to_recordfile()
{
  static int fd = 0;
  if(0 == fd)
  {
    char time
    time_t = time();
    
    fd = open()
  }
  }*/
int ConfigSerial(struct serialConf *sc) {
    int ret;
    int i;
    char tmp;

    if(access(CONFIGFILE, F_OK) == -1) {
        LOGE("config file is not exist\n");
        return -1;
    }
    //LOGD("Access config file success\n");
    ret = GetConfigIntValue(CONFIGFILE, "serial", "speed", &i);
    if(ret != SUCCESS) {
        printf("serial speed=%d\n", i);
        return -1;
    }
    sc->speed = i;

    ret = GetConfigIntValue(CONFIGFILE, "serial", "bits", &i);
    if(ret != SUCCESS) {
        printf("serial bits=%d\n", i);
        return -1;
    }
    sc->bits = i;

    ret = GetConfigIntValue(CONFIGFILE, "serial", "fctl", &i);
    if(ret != SUCCESS) {
        printf("serial fctl=%d\n", i);
        return -1;
    }
    sc->fctl = i;

    ret = GetConfigStringValue(CONFIGFILE, "serial", "event", &tmp);
    if(ret != SUCCESS) {
        printf("serial event=%c\n", tmp);
        return -1;
    }
    sc->event = tmp;

    ret = GetConfigIntValue(CONFIGFILE, "serial", "stop", &i);
    if(ret != SUCCESS) {
        printf("serial stop=%d\n", i);
        return -1;
    }
    sc->stop = i;

    return 0; 
}

int GetConfigStringValue(char *pInFileName,char *pInSectionName,char *pInKeyName,char *pOutKeyValue)
{
	FILE *fpConfig;
	char szBuffer[150];
	char *pStr1,*pStr2;
	int iRetCode = 0;
	
	/*
	printf("pInFileName: %s !\n",pInFileName);
	printf("pInSectionName: %s !\n",pInSectionName);
	printf("pInKeyName: %s !\n",pInKeyName);
	*/
	
	memset(szBuffer,0,sizeof(szBuffer));
	if( NULL==( fpConfig=fopen(pInFileName,"r") ) )
		return FILENAME_NOTEXIST;
		
	while( !feof(fpConfig) )
	{
		if( NULL==fgets(szBuffer,150,fpConfig) )
			break;
		pStr1 = szBuffer ;	
		while( (' '==*pStr1) || ('\t'==*pStr1) )
			pStr1++;
		if( '['==*pStr1 )
		{
			pStr1++;
			while( (' '==*pStr1) || ('\t'==*pStr1) )
				pStr1++;
			pStr2 = pStr1;
			while( (']'!=*pStr1) && ('\0'!=*pStr1) )
				pStr1++;
			if( '\0'==*pStr1)	
				continue;
			while( ' '==*(pStr1-1) )
				pStr1--;	
			*pStr1 = '\0';
					
			iRetCode = CompareString(pStr2,pInSectionName);	
			if( !iRetCode )/*检查节名*/
			{
				iRetCode = GetKeyValue(fpConfig,pInKeyName,pOutKeyValue);
				fclose(fpConfig);
				return iRetCode;
			}	
		}					
	}
	
	fclose(fpConfig);
	return SECTIONNAME_NOTEXIST;
	
}	

/*区分大小写*/
int CompareString(char *pInStr1,char *pInStr2)
{
	if( strlen(pInStr1)!=strlen(pInStr2) )
	{
		return STRING_LENNOTEQUAL;
	}	
		
	/*while( toupper(*pInStr1)==toupper(*pInStr2) )*//*#include <ctype.h>*/
	while( *pInStr1==*pInStr2 )
	{
		if( '\0'==*pInStr1 )
			break;	
		pInStr1++;
		pInStr2++;	
	}
	
	if( '\0'==*pInStr1 )
		return STRING_EQUAL;
		
	return STRING_NOTEQUAL;	
	
}

int GetKeyValue(FILE *fpConfig,char *pInKeyName,char *pOutKeyValue)
{
	char szBuffer[150];
	char *pStr1,*pStr2,*pStr3;
	unsigned int uiLen;
	int iRetCode = 0;
	
	memset(szBuffer,0,sizeof(szBuffer));	
	while( !feof(fpConfig) )
	{	
		if( NULL==fgets(szBuffer,150,fpConfig) )
			break;
		pStr1 = szBuffer;	
		while( (' '==*pStr1) || ('\t'==*pStr1) )
			pStr1++;
		if( '#'==*pStr1 )	
			continue;
		if( ('/'==*pStr1)&&('/'==*(pStr1+1)) )	
			continue;	
		if( ('\0'==*pStr1)||(0x0d==*pStr1)||(0x0a==*pStr1) )	
			continue;	
		if( '['==*pStr1 )
		{
			pStr2 = pStr1;
			while( (']'!=*pStr1)&&('\0'!=*pStr1) )
				pStr1++;
			if( ']'==*pStr1 )
				break;
			pStr1 = pStr2; 	
		}	
		pStr2 = pStr1;
		while( ('='!=*pStr1)&&('\0'!=*pStr1) )
			pStr1++;
		if( '\0'==*pStr1 )	
			continue;
		pStr3 = pStr1+1;
		if( pStr2==pStr1 )
			continue;	
		*pStr1 = '\0';
		pStr1--;
		while( (' '==*pStr1)||('\t'==*pStr1) )
		{
			*pStr1 = '\0';
			pStr1--;
		}
		
		iRetCode = CompareString(pStr2,pInKeyName);
		if( !iRetCode )/*检查键名*/
		{
			pStr1 = pStr3;
			while( (' '==*pStr1)||('\t'==*pStr1) )
				pStr1++;
			pStr3 = pStr1;
			while( ('\0'!=*pStr1)&&(0x0d!=*pStr1)&&(0x0a!=*pStr1) )
			{
				if( ('/'==*pStr1)&&('/'==*(pStr1+1)) )
					break;
				pStr1++;	
			}	
			*pStr1 = '\0';
			uiLen = strlen(pStr3);
			memcpy(pOutKeyValue,pStr3,uiLen);
			*(pOutKeyValue+uiLen) = '\0';
		 	return SUCCESS;
		}
	}
	
	return KEYNAME_NOTEXIST;
}

int GetConfigIntValue(char *pInFileName,char *pInSectionName,char *pInKeyName,int *pOutKeyValue)
{
	int iRetCode = 0;
	char szKeyValue[16],*pStr;
	
	memset(szKeyValue,0,sizeof(szKeyValue));
	iRetCode = GetConfigStringValue(pInFileName,pInSectionName,pInKeyName,szKeyValue);
	if( iRetCode )
		return iRetCode;
	pStr	= szKeyValue;
	while( (' '==*pStr)||('\t'==*pStr))
		pStr++;
	if( ('0'==*pStr)&&( ('x'==*(pStr+1))||('X'==*(pStr+1)) ) )	
		sscanf(pStr+2,"%x",pOutKeyValue);
	else
		sscanf(pStr,"%d",pOutKeyValue);
		
	return SUCCESS;	
			
}

