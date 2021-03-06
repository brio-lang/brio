#ifndef BRIOLANG_SRC_HTTP_H_
#define BRIOLANG_SRC_HTTP_H_

#include <string>
#include <curl/curl.h>

using namespace std;

class HttpClient{
    public:
        HttpClient();
        ~HttpClient();
        string get(string url);
        string get(string url, string header);
        string post(string url, string user, string pass, string fields);
    private:
        CURL * curl;
};

#endif  // BRIOLANG_SRC_HTTP_H_