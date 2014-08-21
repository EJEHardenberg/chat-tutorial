#include "config.h"
#include "chatfile.h"
#include "load_qdecoder.h"

int main(void){
#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0) {
#endif
    qentry_t *req = qcgireq_parse(NULL, Q_CGI_GET);
    qcgires_setcontenttype(req, "text/plain");

    FILE * fp =  getChatFile();
    if(fp == NULL){
    	printf("%s\n", "Could not retrieve chat history. Please try again later");
    	goto end;
    }

    int cOrEOF;
    char c;
    while( (cOrEOF = fgetc(fp)) != EOF){
        c = (char)cOrEOF;
        printf("%c", c);
    }
    fclose(fp);

    end:
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}