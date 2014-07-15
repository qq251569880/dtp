/*
* Copyright (c) 2011
* All rights reserved.
*
* Filename:         log.h
* Author:           Carl
* Date:             2013/12/18
* Description:      the output of log  when the program running,to standard output such as print screen or to file
*
*/
#ifndef __FILE_LOG_H__
#define __FILE_LOG_H__

#include <stdio.h>
#include <stdarg.h>

bool is_tofile;
bool is_background;
char log_filename[1024];
char fifo_filename[1024];
int max_logfile_size;


int LOGERR(int err_no,char *fmt,...);
int LOGINFO(char *fmt,...);


//log to fifo operation

#endif
