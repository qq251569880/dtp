/*
* Copyright (c) 2011
* All rights reserved.
*
* Filename:
* Author:
* Date:
* Description:
*
*/
#include "log.h"

int LOGERR(int err_no, char *fmt,...)
{
    va_list args;
    va_start(args,fmt);
    char logbuf[10240] ={0};
    vsprintf(logbuf,fmt,args);
    if (is_tofile)
    {
        char fbuf[10240] = {0};
        sprintf(fbuf,"%d:%s",err_no,logbuf);
        //finish it in future
    }
    if(!is_background)
    {
        printf("%d:%s",err_no,logbuf);
    }
    va_end(args);
    return 0;
}

int LOGINFO(char *fmt,...)
{
    va_list args;
    va_start(args,fmt);
    vprintf(fmt,args);
    va_end(args);
    return 0;
}
