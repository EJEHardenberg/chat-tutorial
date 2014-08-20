#include "config.h"
#include "chatfile.h"
#include "load_qdecoder.h"

int main(void){
#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0) {
#endif
    qentry_t *req = qcgireq_parse(NULL, Q_CGI_GET);
    qcgires_setcontenttype(req, "application/JSON");

    int initialized = chatInit();

    printf("{ \"heartbeat\" : %ld, \"initialized\" : %s }", time(0), initialized ? "true" : "false");

    // De-allocate memories
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}