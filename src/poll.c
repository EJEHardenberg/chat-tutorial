#include "config.h"
#include "chatfile.h"
#include "load_qdecoder.h"

static void printUpdated(int updated){
    printf("{\"updated\": %s}", updated ? "true" : "false");
}

int main(void){
    chatInit();
#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0) {
#endif
    qentry_t *req = qcgireq_parse(NULL, Q_CGI_GET);
    char * sentTime = NULL; 
    long long intermediateTime = 0L;
    time_t parsedTime = 0;
    qcgires_setcontenttype(req, "application/json");

    sentTime = req->getstr(req, "date", true);
    if(sentTime == NULL){
        /* They did not send us a proper request. */
        printUpdated(0);
        goto end;
    }

    int scanned = sscanf(sentTime, "%lld", &intermediateTime);
    if(scanned != 1){
        /* Incorrect format likely since we couldn't parse it out */
        printUpdated(0);
        free(sentTime);
        goto end;
    }

    parsedTime = (time_t)intermediateTime;

    int updated = fileLastModifiedAfter(DATA_FILE, parsedTime);
    printUpdated(updated);
    
    free(sentTime);
    // De-allocate memories
    end:
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}