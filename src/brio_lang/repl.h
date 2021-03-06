#ifndef BRIOLANG_SRC_REPL_H_
#define BRIOLANG_SRC_REPL_H_

#include <brio_lang/interpreter.h>

#include <string>

using namespace std;

class REPL {
    public:
        REPL();
        void start();
        void eval(string input);
        void printHelp();
        void printLicense();
        void printCredits();
        void printCopyright();
    private:
        void printHeader();
        string getDateTime();
        string getOSVersion();
        Interpreter * interpreter;
    
};


#endif  // BRIOLANG_SRC_REPL_H_