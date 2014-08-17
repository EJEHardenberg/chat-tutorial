#include "chatfile.h"

static int file_exists(const char * filename){
	/* Security Concern: If you check for a file's existence and then open the 
	 * file, between the time of access checking and creation of a file someone
	 * can create a symlink or something and cause your open to fail or open 
	 * something that shouldn't be opened. That being said... I'm not concerned.
	*/
	struct stat buffer;
	return(stat (filename, &buffer) == 0);
}

static int create_chat_file(){	
	int chatFileExists = file_exists(DATA_FILE);
	if(chatFileExists == 0){
		FILE *fp = fopen(DATA_FILE, "w");
		if(!fp){
			chatFileExists = 0;
			fprintf(stderr,"%s %s\n", "Could not create chat file " DATA_FILE " ",  strerror(errno));
		}else{
			chatFileExists = 1;
			fclose(fp);
		}
	}
	return chatFileExists;
}

/* Check if the file was modified after the lastCheckedTime */
time_t fileLastModifiedAfter(const char * filename, time_t lastCheckedTime){
	struct stat buffer;
	stat(filename, &buffer);
	return lastCheckedTime < buffer.st_mtime;
}

FILE * getChatFile(){
	int userFileExists = file_exists(DATA_FILE);
	if(userFileExists == 0) return NULL;

	FILE * fp = fopen(DATA_FILE, "r");
	if(!fp){
		fprintf(stderr, "Could not open chat file " DATA_FILE " for reading: %s", strerror(errno));
	}
	return fp;
}

int updateConversation(const char * user, const char * addendum){
	FILE * fp = fopen(DATA_FILE, "a");
	
	if(!fp){
		fprintf(stderr, "Could not open chat file " DATA_FILE " for updating: %s", strerror(errno));
		return 0;
	}

	time_t t;
	struct tm * tmp;
	t = time(NULL);
	tmp = localtime(&t);
	if(tmp == NULL){
		fprintf(stderr, "Failed to determine local time\n");
		fprintf(fp, "[Unknown Time %s]: %s\n", user ,addendum);
	}else{
		char timeBuffer[30];
		bzero(timeBuffer, sizeof(timeBuffer));
		strftime(timeBuffer, sizeof(timeBuffer), "%F %I:%M", tmp);
		fprintf(fp, "[%s %s]: %s\n", timeBuffer , user ,addendum);
	}
	fflush(fp);
	fclose(fp);
	return 1;
}

int chatInit(){
	return create_chat_file();
}
