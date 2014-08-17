#ifndef __CHATFILE_H__
#define __CHATFILE_H__

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "config.h"

/* Returns 1 if the lastCheckedTime is less than the last write to the file */
time_t fileLastModifiedAfter(const char * filename, time_t lastCheckedTime);

/* Returns NULL on err, otherwise a Read Only file descriptor for the chat file */
FILE * getChatFile();

/* Add a message from the user to the chat history, 1 on success 0 on err */
int updateConversation(const char * user, const char * addendum);

/* Initialization of chat, simply ensures the chat file exists */
int chatInit();

#endif