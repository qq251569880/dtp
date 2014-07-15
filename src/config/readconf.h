/*
* Copyright (c) 2011
* All rights reserved.
*
* Filename:         readconf.h
* Author:           Carl
* Date:             2013/12/17
* Description:      Read config file like
-----------------------------------------
[area1]
key11 = value11;
key12 = value12;
[area2]
key21 = value21;
key22 = value22;
-----------------------------------------
or
-----------------------------------------
key1=value1
key2=value2
key3=value3
key4=value4
-----------------------------------------
*
*/

#ifndef __FILE_READ_CONF_H__
#define __FILE_READ_CONF_H__

#include <stdio.h>
#include "common.h"

typedef struct CItem
{
    char item_name[100];
    char item_value[1024];
    CItem *next;
    CItem(const char *name,const char *value)
    {
        strcpy(item_name,name);
        strcpy(item_value,value);
        next = NULL;
    }
}CItem;

typedef struct CArea 
{
    char area_name[100];
    CArea *next;
    CItem *item;
    CArea(const char *name)
    {
        strcpy(area_name,name);
        next = NULL;
        item = NULL;
    }
}CArea;

class CConfig_conf
{
public:
    CConfig_conf();

    ~CConfig_conf();

    CConfig_conf(char *filename);
    int parse_conf(char *filename);
    int delete_conf();
    char* get_value(char *area_name,char *item_name);
    char* get_value(char *item_name);

    void print();
protected:
    int InitConf();
    int AddArea(CArea *area);
    int AddItem(char *area_name,char *item);

    bool   GetAreaName(char*   line, char *strarea);
    bool   GetKeyValue(char* line, char *key,char *value);
    
private:
    CArea *m_conf_content;
    int m_error_line;
    char   chSectionBMark   =   '['; //area start
    char   chSectionEMark   =   ']'; //area end
    char   chRecordEMark   =   ';';   //record   end-mark
    char   chCommentMark   =   '#';  //comment

};


#endif
