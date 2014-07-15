/*
* Copyright (c) 2011
* All rights reserved.
*
* Filename:         readconf.cpp
* Author:           Carl
* Date:             2013/12/17
* Description:      reference the head file readconf.h
*
*/

#include "readconf.h"
#include "log/log.h"

static inline bool   istab(int   c)
{ 
    return (c == '\t'); 
}
static inline char*   strltrim(char* str)
{
    while(isspace(*str)   ||   istab(*str))
    {
        ++str;
    }
    return   str;
}

static   inline   char*   strrtrim(char*   str)
{
    int   len   =   strlen(str)   -   1;
    while(isspace(str[len])   ||   istab(str[len]))
    {
        str[len--]   =   '\0';
    }
    return   str;
}

CConfig_conf::CConfig_conf()
{
    InitConf();
}

CConfig_conf::~CConfig_conf()
{
    delete_conf();
}

CConfig_conf::CConfig_conf(char *filename)
{
    InitConf();
    parse_conf(filename);
}

int CConfig_conf::parse_conf(char *filename)
{

    if (m_conf_content != NULL)
    {
        delete_conf();
        InitConf();
    }

    using   std::string;
    std::ifstream   in(filename);

    if(!in.is_open())   return   errno;

    char   line[1024];
    char*   pline;
    bool   bInArea   =   false;
    bool   bHaveArea   =   false;
    char   strArea[100] = {0};
    char   strKey[100] = {0};
    char   strValue[1024] = {0};
    m_error_line   =   0;
    while(!in.eof())
    {
        in.getline(line,   sizeof(line));
        pline   =   line;
        ++m_error_line;
        pline   =   strltrim(pline);
        if(pline[0]   ==   '\0')  
            continue;   //white   line,   skip
        if(pline[0]   ==   chCommentMark)  
            continue;   //comment   line,   skip
        if(m_conf_content != NULL)
        {
            //is   new-section   begin?
            if(pline[0]   ==   chSectionBMark)
            {
                if (GetAreaName(pline,   strArea))
                {
                    if (bHaveArea)
                    {
                        CArea *area = new CArea(strArea);
                        AddArea(area);
                    }
                    else
                    {
                        LOGERR(0,"the previous area must have a name in [] at line %d",m_error_line);
                        return -1;
                    }
                }
                else
                {
                    LOGERR(0,"config syntax error");
                    return -1;
                }
            }
            else if( GetKeyValue(pline, strKey,strValue) )
            {
                CItem *item = new CItem(strKey,strValue);
                AddItem(strArea,item);
            }
            else
            {
                in.close();
                LOGERR(0,"config syntax error at line %d",m_error_line);
                return -1;
            }
        }
        else
        {
            if (GetAreaName(pline,   strArea))
            {
                CArea *area = new CArea(strArea);
                AddArea(area);
                bHaveArea = true;
            }
            else if( GetKeyValue(pline, strKey,strValue) )
            {
                CItem *item = new CItem(strKey,strValue);

                sprintf(strArea,"ROOT");
                CArea *area = new CArea(strArea);
                AddArea(area);

                AddItem(strArea,item);
                bHaveArea = false;
            }
            else
            {
                in.close();
                LOGERR(0,"config syntax error at line %d",m_error_line);
                return -1;
            }
        }
    }

    in.close();
    return   0;
}
int CConfig_conf::delete_conf()
{   

    while( m_conf_content!= NULL)
    {

        CArea *p_area = m_conf_content;
        m_conf_content = p_area->next;
        while (p_area->item !=NULL)
        {
            CItem *p_item = p_area->item;
            p_area->item = p_area->item->next;
            p_item->next = NULL;
            delete p_item;
            p_item = NULL;
        }
        p_area->next = NULL;
        delete p_area;
        p_area =NULL;
    }
    return 0;
}
char* CConfig_conf::get_value(char *area_name,char *item_name)
{
    CArea *p_area = m_conf_content;
    while(p_area != NULL)
    {
        if(strcmp(area_name,p_area->area_name) == 0)
        {
            CItem *p_item = p_area->item;
            while(p_item != NULL)
            {
                if (strcmp(item_name,p_item->item_name) == 0)
                {
                    return p_item->item_value;
                }
                p_item = p_item->next
            }
            return NULL;
        }
        p_area = p_area->next;
    }
    return NULL;

}
char* CConfig_conf::get_value(char *item_name)
{
    char *root_area = "ROOT";
    return get_value(root_area,item_name);
}

int CConfig_conf::InitConf()
{
    if(m_conf_content != NULL)
        delete_conf();
    return 0;
}
int CConfig_conf::AddArea(CArea *area)
{
    CArea *p_area = m_conf_content;
    while (p_area != NULL)
    {
        p_area = p_area->next;
    }
    p_area->next = area;
    return 0;
}
int CConfig_conf::AddItem(char *area_name,char *item)
{
    CArea *p_area = m_conf_content;
    while(p_area != NULL)
    {
        if(strcmp(area_name,p_area->area_name) == 0)
        {
            CItem *p_item = p_area->item;
            while(p_item != NULL)
            {
                p_item = p_item->next;
            }
            p_item->next = item;
            return 0;
        }
        p_area = p_area->next;
    }
    LOGERR(0,"can't find area[%s]",area_name);
    return -1;
}

bool  CConfig_conf::GetAreaName(char*   line, char *strarea)
{
    char*   tmp;
    if(line[0]   ==   chSectionBMark)
    {
        if((tmp   =   strchr(++line,   chSectionEMark))   !=   NULL)
        {
            *tmp   =   '\0';
            strcpy(strarea  , line);
            return   true;
        }
    }

    return   false;
}
bool   CConfig_conf::GetKeyValue(char* line, char *key,char *value)
{
    char*   tmp;
    if((tmp   =   strrchr(line,   chRecordEMark))   !=   NULL)
    {
        *tmp   =   '\0';   //ignore   content   after   ';'(the   RecordEMark)
        if((tmp   =   strchr(line,   '='))   !=   NULL)
        {
            *tmp++   =   '\0';
            tmp   =   strltrim(tmp);
            tmp   =   strrtrim(tmp);
            line   =   strrtrim(line);

            strcpy(key,line);
            strcpy(value ,tmp);
            return   true;
        }
    }
    return   false;
}
void CConfig_conf::print()
{
    CArea *p_area = m_conf_content;
    while(p_area != NULL)
    {
        LOGINFO("[%s]\n",p_area->area_name);
        CItem *p_item = p_area->item;
        while(p_item != NULL)
        {
            LOGINFO("\t%s = %s;\n",p_item->item_name,p_item->item_value);
            p_item = p_item->next
        }
        p_area = p_area->next;
    }
}