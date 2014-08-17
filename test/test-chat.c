#include "chatfile.h"
#include <assert.h>
#include <unistd.h>

int main(){

	/* Test init */
	int success = chatInit();
	assert(success == 1);

	/* Test adding to conversation */
	success = updateConversation("test", "I AM GROOT");
	assert(success == 1);

	sleep(1);
	/* Test modified time */
	assert(fileLastModifiedAfter(DATA_FILE, time(0)) == 0 );

	/* Test that reading converation has what was written */
	FILE * fp = getChatFile();
	assert(fp != NULL);

	char buffer[512];
	bzero(buffer, sizeof(buffer));
	fgets(buffer, sizeof(buffer), fp);
	fclose(fp);

	assert( strstr(buffer, "I AM GROOT") != NULL);
	
	return 0;
}