#include <iostream>

#include <brio_lang/fcgi.h>
#include <brio_lang/util.h>
#include <brio_lang/exceptions.h>


long STDIN_MAX = 100000000;

void FastCGI::initialize(Interpreter* interpreter){
    this->interpreter = interpreter;
    this->startLoop();
};

string FastCGI::getRequestContent(const FCGX_Request & request) {
    char * content_length_str = FCGX_GetParam("CONTENT_LENGTH",
                                               request.envp);
    unsigned long content_length = STDIN_MAX;

    if (content_length_str) {
        content_length = strtol(content_length_str,
                                &content_length_str,
                                10);

        if (*content_length_str) {
            cerr << "Can't Parse 'CONTENT_LENGTH='"
                 << FCGX_GetParam("CONTENT_LENGTH", request.envp)
                 << "'. Consuming stdin up to " << STDIN_MAX << endl;
        }

        if (content_length > STDIN_MAX) {
            content_length = STDIN_MAX;
        }
    } else {
        // Do not read from stdin if CONTENT_LENGTH is missing
        content_length = 0;
    }
    
    char * content_buffer = new char[content_length];
    cin.read(content_buffer, content_length);
    content_length = cin.gcount();

    string content(content_buffer, content_length);
    delete [] content_buffer;
    return content;
}

Dictionary* FastCGI::buildEnvDictionary(const FCGX_Request & request){
    Dictionary* envDict = new Dictionary();
    envDict->value[new String("GATEWAY_INTERFACE")] = new String(FCGX_GetParam("GATEWAY_INTERFACE", request.envp));
    envDict->value[new String("SERVER_SOFTWARE")] = new String(FCGX_GetParam("SERVER_SOFTWARE", request.envp));
    envDict->value[new String("QUERY_STRING")] = new String(FCGX_GetParam("QUERY_STRING", request.envp));
    envDict->value[new String("REQUEST_METHOD")] = new String(FCGX_GetParam("REQUEST_METHOD", request.envp));
    envDict->value[new String("REQUEST_BODY")] = new String(this->getRequestContent(request));
    envDict->value[new String("CONTENT_TYPE")] = new String(FCGX_GetParam("CONTENT_TYPE", request.envp));
    envDict->value[new String("CONTENT_LENGTH")] = new String(FCGX_GetParam("CONTENT_LENGTH", request.envp));
    envDict->value[new String("SCRIPT_FILENAME")] = new String(FCGX_GetParam("SCRIPT_FILENAME", request.envp));
    envDict->value[new String("SCRIPT_NAME")] = new String(FCGX_GetParam("SCRIPT_NAME", request.envp));
    envDict->value[new String("REQUEST_URI")] = new String(FCGX_GetParam("REQUEST_URI", request.envp));
    envDict->value[new String("DOCUMENT_URI")] = new String(FCGX_GetParam("DOCUMENT_URI", request.envp));
    envDict->value[new String("DOCUMENT_ROOT")] = new String(FCGX_GetParam("DOCUMENT_ROOT", request.envp));
    envDict->value[new String("SERVER_PROTOCOL")] = new String(FCGX_GetParam("SERVER_PROTOCOL", request.envp));
    envDict->value[new String("REMOTE_ADDR")] = new String(FCGX_GetParam("REMOTE_ADDR", request.envp));
    envDict->value[new String("REMOTE_PORT")] = new String(FCGX_GetParam("REMOTE_PORT", request.envp));
    envDict->value[new String("SERVER_ADDR")] = new String(FCGX_GetParam("SERVER_ADDR", request.envp));
    envDict->value[new String("SERVER_PORT")] = new String(FCGX_GetParam("SERVER_PORT", request.envp));
    envDict->value[new String("SERVER_NAME")] = new String(FCGX_GetParam("SERVER_NAME", request.envp));
    return envDict;
};

int FastCGI::startLoop() {

    // Backup the stdio streambufs
    streambuf * cin_streambuf  = cin.rdbuf();
    streambuf * cout_streambuf = cout.rdbuf();
    streambuf * cerr_streambuf = cerr.rdbuf();

    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0) {

        // get fastcgi streambufs
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        // redirect stdin
        cin.rdbuf(&cin_fcgi_streambuf);

        // extract env parameters
        Dictionary * envDict = this->buildEnvDictionary(request);

        // exec main method
        vector<Object*> mainParams;
        mainParams.push_back(envDict);
        Object * output = this->interpreter->main(mainParams);

        // redirect stdio streambufs
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);

        // write output
        cout << Util::escape_raw_string(output->toString());

        // restore stdio streambufs
        cin.rdbuf(cin_streambuf);
        cout.rdbuf(cout_streambuf);
        cerr.rdbuf(cerr_streambuf);
    }

    // restore stdio streambufs
    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

    return 0;
}