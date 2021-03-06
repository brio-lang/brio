///
/// Brio Lang
/// repl.cc
///
/// Read Eval Print Loop
///

#include <brio_lang/repl.h>
#include <brio_lang/version.h>
#include <brio_lang/interpreter.h>
#include <brio_lang/lexer.h>
#include <brio_lang/parser.h>
#include <brio_lang/def.h>
#include <brio_lang/ref.h>
#include <brio_lang/symbol.h>
#include <brio_lang/util.h>
#include <brio_lang/exceptions.h>
#include <brio_lang/visitor.h>

#include <brio_lang/linenoise.h>

#include <sys/utsname.h>
#include <iostream>
#include <string>
#include <ctime>


using namespace std;


REPL::REPL(){
    SymbolTable * table = new SymbolTable();
    interpreter = new Interpreter(nullptr, table);
};

/**
 * Starts the Brio Lang REPL (read-eval-print-loop). Will run on a 
 * continuous loop until Ctrl+D or "exit()" is provided.
 */
void REPL::start(){
    this->printHeader();

    while (true) {
        std::string line;
        auto quit = linenoise::Readline(">>> ", line);

        if (quit) {
            break;
        }

        linenoise::AddHistory(line.c_str());
        this->eval(std::string(line));
    }

    std::exit(0);
};

void REPL::eval(string input){
    // interpret the statement
    if (input == "exit"){
        std::cout << "Use exit() or Ctrl-D (i.e. EOF) to exit" << "\n";
    }
    else if (input == "exit()" || input == "exit();"){
        std::exit(0);
    }
    else if (input == "help"){
        this->printHelp();
    }
    else if (input == "copyright"){
        this->printCopyright();
    }
    else if (input == "license"){
        this->printLicense();
    }
    else {
        try{
            // interpret statement
            BrioLexer lex(input);
            BrioParser parse(&lex);
            ProgramNode stmt = parse.program();

            // enforce operator precedence
            OperatorPrecedenceVisitor opVisitor;
            opVisitor.visit(&stmt);

            // update symbol talbe
            Def def(interpreter->table);
            def.downUp(&stmt);
            Ref ref(interpreter->table);
            ref.downUp(&stmt);

            // execute statement
            interpreter->root = &stmt;
            Object * result = interpreter->exec(stmt.children[0]);
            if (result != nullptr){
                std::cout << result->toString() << std::endl;
            }
        }catch(const std::exception &e){
            std::cout << e.what() << "\n";
        }
    }
};

void REPL::printHelp(){
    std::cout << "Welcome to Brio Lang " << VERSION << "! Please visit https://www.brio-lang.org to learn more." << std::endl;
};

void REPL::printLicense(){
    std::cout << Util::read_file("./LICENSE") << std::endl;
};

void REPL::printCopyright(){
    std::cout << "Brio Lang is copyrighted free software by Trent Seed (c) 2021. All rights reserved." << std::endl;
    std::cout << "Brio Lang includes packages written by third parties, which carry their own copyright" << std::endl;
    std::cout << "and license terms. Please review the COPYRIGHT file for the complete list." << std::endl;
};

/**
 * Returns the OS version of the machine.
 */
string REPL::getOSVersion(){
    struct utsname name;
	if(uname(&name)) exit(-1);
	return name.release;
}

/**
 * Returns a formated string of the current date and time.
 * Example:  "May 30 2019, 09:28:50"
 */
string REPL::getDateTime(){
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%b %d %Y, %H:%M:%S", timeinfo);
    return std::string(buffer);
};

/**
 * Outputs the header contents which include the brio lang version,
 * compiler version, date time, as well as some example commands.
 */
void REPL::printHeader(){
    std::cout << "Brio Lang " << VERSION << " (" << this->getDateTime() << ")" << std::endl;
    std::cout << "[" << COMPILER << "] on " << this->getOSVersion() << std::endl;
    std::cout << "Type \"help\", \"license\", or \"copyright\" for more information." << std::endl;
};