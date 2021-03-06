#ifndef BRIOLANG_SRC_FCGI_H_
#define BRIOLANG_SRC_FCGI_H_

#include "fcgio.h"
#include <brio_lang/interpreter.h>
#include <brio_lang/object.h>

class FastCGI{
    public:
        void initialize(Interpreter*);
        string getRequestContent(const FCGX_Request &request);
        Dictionary* buildEnvDictionary(const FCGX_Request &request);
        int startLoop();
    private:
        Interpreter* interpreter;
};

#endif  // BRIOLANG_SRC_FCGI_H_