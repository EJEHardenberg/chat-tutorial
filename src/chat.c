#include "config.h"
#include "chatfile.h"
#include "load_qdecoder.h"

/* Don't send msg with newline or "'s! */
static void printSuccess(int updated, char * msg){
    printf("{\"success\": %s, \"message\" : \"%s\"}", updated ? "true" : "false", msg);
}

int main(void){
    chatInit();
#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0) {
#endif
    qentry_t *req = qcgireq_parse(NULL, Q_CGI_POST);
    char * user = NULL; 
    char * msg = NULL;
    qcgires_setcontenttype(req, "application/json");

    user = req->getstr(req, "u", true);
    if(user == NULL){
        /* They did not send us a proper request. */
        printSuccess(0, "Invalid Request");
        goto end;
    }
    /* Limit the user name length */
    int i = 0;
    int maxlength = 21;
    for (i = 0; i < maxlength && user[i] != '\0'; ++i)
        ;
    if(i == maxlength){
        printSuccess(0, "Username too long");
        free(user);
        goto end;
    }

    msg = req->getstr(req, "m", true);
    if(msg == NULL){
        printSuccess(0, "Invalid Request");
        free(user);
        goto end;
    }

    int updated = updateConversation(user, msg);
    printSuccess(updated, updated ? "Message has been sent" : "Message failed to send please try again");
    
    free(user);
    free(msg);
    // De-allocate memories
    end:
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}