///
/// Brio Lang
/// brio.cc
///

#include <brio_lang/lexer.h>
#include <brio_lang/parser.h>
#include <brio_lang/tree.h>
#include <brio_lang/exceptions.h>
#include <brio_lang/visitor.h>
#include <brio_lang/dot.h>
#include <brio_lang/symbol.h>
#include <brio_lang/def.h>
#include <brio_lang/ref.h>
#include <brio_lang/interpreter.h>
#include <brio_lang/repl.h>
#include <brio_lang/version.h>
#include <brio_lang/fcgi.h>
#include <brio_lang/util.h>

#include <iostream>
#include <fstream>
#include <string>


/**
 * Iterates through lexer and prints all token names.
 * 
 * @param lex Pointer to the lexer instance
 */
void print_tokens(Lexer * lex){
    Token token = lex->nextToken();
    while (token.getName() != "EOF"){
        std::cout << token.getName() << " (text='" << token.getText() << "', line=" << token.getLineNo() << ")" << std::endl;
        token = lex->nextToken();
    }
    std::cout << std::endl;
}

/**
 * Prints the version to the screen.
 */
void print_version(){
    std::cout << "brio-lang: version " << VERSION << std::endl;
};

/**
 * Prints usage information to the screen.
 */
void print_help(){
    print_version();
    std::cout << "usage: ./brio [-h] [-v] [-i] [-t] [-gv] [-sym] [-ast] FILE.brio" << std::endl;
    std::cout << std::endl;
    std::cout << "optional arguments:" << std::endl;
    
    const int PADDING = 30;

    string args_h = "  -h, --help";
    string desc_h = "Prints the help information";
    std::cout << args_h;
    cout.width(PADDING - args_h.size() + desc_h.size());
    std::cout << desc_h << std::endl;

    string args_v = "  -v, --version";
    string desc_v = "Prints the version";
    std::cout << args_v;
    cout.width(PADDING - args_v.size() + desc_v.size());
    std::cout << desc_v << std::endl;

    string args_i = "  -i";
    string desc_i = "Runs in interactive mode (read-eval-print loop)";
    std::cout << args_i;
    cout.width(PADDING - args_i.size() + desc_i.size());
    std::cout << desc_i << std::endl;

    string args_tokens = "  -t";
    string desc_tokens = "Prints the tokens from the lexer";
    std::cout << args_tokens;
    cout.width(PADDING - args_tokens.size() + desc_tokens.size());
    std::cout << desc_tokens << std::endl;

    string args_gv = "  -gv";
    string desc_gv = "Outputs a DOT file for visualizing the AST";
    std::cout << args_gv;
    cout.width(PADDING - args_gv.size() + desc_gv.size());
    std::cout << desc_gv << std::endl;

    string args_sym = "  -sym";
    string desc_sym = "Prints the symbol table globals";
    std::cout << args_sym;
    cout.width(PADDING - args_sym.size() + desc_sym.size()); 
    std::cout << desc_sym << std::endl;

    string args_ast = "  -ast";
    string desc_ast = "Prints each node type in the AST";
    std::cout << args_ast;
    cout.width(PADDING - args_ast.size() + desc_ast.size());
    std::cout << desc_ast << std::endl;

    string args_fcgi = "  -fcgi";
    string desc_fcgi = "Starts FastCGI listener, must be called from spawn-fcgi";
    std::cout << args_fcgi;
    cout.width(PADDING - args_fcgi.size() + desc_fcgi.size());
    std::cout << desc_fcgi << std::endl;
}

void start_repl(){
    REPL repl;
    repl.start();
}

/**
 * Returns true or false to indicate if a particular option was 
 * specified in the arguments. Note, must be provided before 
 * filename, e.g.  brio [here] path/to/file.brio
 */
bool option_exists(char * argv[], int argc, string option){
    for (int i=0; i<argc; i++){
        string arg = argv[i];
        if (arg == option) return true;

        // determine if file path reached
        if (Util::string_has_ending(arg, ".brio")){
            return false;
        }
    }
    return false;
}

/**
 * Returns the source file path and subsequent arguments.
 */
vector<string> get_file_args(char * argv[], int argc){
    vector<string> args;
    int file_index = 0;
    for (int i=0; i<argc; i++){
        string arg = argv[i];
        if (!file_index && Util::string_has_ending(arg, ".brio")){
            file_index = i;
            args.push_back(arg);
        }
        else if (file_index && i > file_index){
            args.push_back(arg);
        }
    }
    return args;
}

/**
 * Entry point for the Brio Lang Interpreter.
 * 
 * @param argc The integer count of arguments
 * @param argv An array of string arguments
 */
int main(int argc, char * argv[]){

    bool OUTPUT_TOKENS = false;      // if enabled, prints the tokens to the screen
    bool OUTPUT_DOT_GV = false;      // if enabled, creates a .gv file for visualization
    bool OUTPUT_SYM_TABLE = false;   // if enabled, prints the sym table globals
    bool OUTPUT_AST_NODES = false;   // if enabled, visits each ast node and prints the type
    bool OUTPUT_FAST_CGI = false;    // if enabled, initialize Fast CGI 

    // check the number of parameters
    if (argc < 2) {
        start_repl();
        return 1;
    }

    // check for compiler options
    if (option_exists(argv, argc, "-h") || 
        option_exists(argv, argc, "--help")){
        print_help();    
        std::exit(0);
    }
    if (option_exists(argv, argc, "-v") || 
        option_exists(argv, argc, "--version")){
        print_version();
        std::exit(0);
    }
    if (option_exists(argv, argc, "-i")){
        start_repl();
    }
    if (option_exists(argv, argc, "-t")){
        OUTPUT_TOKENS = true;
    }
    if (option_exists(argv, argc, "-gv")){
        OUTPUT_DOT_GV = true;
    }
    if (option_exists(argv, argc, "-sym")){
        OUTPUT_SYM_TABLE = true;
    }
    if (option_exists(argv, argc, "-ast")){
        OUTPUT_AST_NODES = true;
    }
    if (option_exists(argv, argc, "-fcgi")){
        OUTPUT_FAST_CGI = true;
    }

    // get file path and optional args
    vector<string> file_args = get_file_args(argv, argc);
    if (file_args.size() == 0){
        std::cout << "invalid file extension, must be .brio" << std::endl;
        std::exit(2);
    }
    string file_path = file_args[0];
    if (!Util::string_has_ending(file_path, ".brio")){
        std::cout << "invalid file extension, must be .brio" << std::endl;
        std::exit(2);
    }

    // read source code
    string code = Util::read_file(file_path);
    BrioLexer lexer(code);

    if (OUTPUT_TOKENS){
        std::cout << std::endl << "Tokens:" << std::endl;
        print_tokens(&lexer);
        lexer.reset();
    }

    // parse tokens and generate AST
    BrioParser parser(&lexer);
    ProgramNode tree = parser.program();

    // enforce operator precedence
    OperatorPrecedenceVisitor opVisitor;
    opVisitor.visit(&tree);

    if (OUTPUT_AST_NODES){
        // run a node type print visitor
        std::cout << std::endl << "AST Nodes:" << std::endl;
        IndependentPrintVisitor visitor;
        visitor.visit(&tree);
        std::cout << std::endl;
    }

    if (OUTPUT_DOT_GV){
        // generate DOT representation of AST
        DotGraphVisitor dot_visitor;
        string dot = dot_visitor.generate(&tree);

        // write DOT file with ".gv" extension
        string output_path = argv[argc-1];
        output_path = output_path.substr(0, output_path.size()-5);
        output_path.append(".gv");
        Util::write_file(output_path, dot);
        std::cout << std::endl << "Outputing DOT visualization:" << std::endl;
        std::cout << "-->  " << output_path << std::endl << std::endl;
    }

    // create symbol table and define symbols
    SymbolTable symbol_table;
    Def def(&symbol_table);
    def.downUp(&tree);

    if (OUTPUT_SYM_TABLE){
        std::cout << std::endl << "Symbol Table:" << std::endl;
        std::cout << symbol_table.toString() << std::endl;
    }

    // resolve symbols and compute expression types
    Ref ref(&symbol_table);
    ref.downUp(&tree);

    // execute the tree-based interpreter
    Interpreter interpreter(&tree, &symbol_table, file_args, file_path);

    if (OUTPUT_FAST_CGI){
        FastCGI fcgi;
        fcgi.initialize(&interpreter);
        return 0;
    }

    // execute interpreter
    interpreter.exec();

    return 0;
};