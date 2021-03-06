///
/// Brio Lang
/// http.cc
///

#include <brio_lang/http.h>
#include <brio_lang/exceptions.h>


HttpClient::HttpClient(){
    curl_global_init(CURL_GLOBAL_DEFAULT);
    this->curl = curl_easy_init();
};

HttpClient::~HttpClient(){
    curl_easy_cleanup(this->curl);
    curl_global_cleanup();
};

size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size*nmemb;
    try
    {
        s->append((char*)contents, newLength);
    }
    catch(std::bad_alloc &e)
    {
        //handle memory problem
        return 0;
    }
    return newLength;
}

/** Perform a basic HTTP GET method via libcurl 
 *  Reference: https://curl.haxx.se/libcurl/c/https.html
 *  Reference: https://stackoverflow.com/a/36401787
 **/
string HttpClient::get(string url){
    CURLcode resCode;
    string resString = "";
    const char * charUrl = url.c_str();

    if(this->curl) {
        curl_easy_setopt(curl, CURLOPT_URL, charUrl);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resString);
        curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);

        /* Perform the request, res will get the return code */ 
        resCode = curl_easy_perform(curl);
        /* Check for errors */ 
        if(resCode != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(resCode));
    }

    return resString;
};

/** Perform a basic HTTP GET method via libcurl 
 *  Reference: https://curl.haxx.se/libcurl/c/https.html
 *  Reference: https://stackoverflow.com/a/36401787
 **/
string HttpClient::get(string url, string header){
    CURLcode resCode;
    string resString = "";
    const char * charUrl = url.c_str();

    if(this->curl) {
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, header.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        curl_easy_setopt(curl, CURLOPT_URL, charUrl);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resString);
        curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);

        /* Perform the request, res will get the return code */ 
        resCode = curl_easy_perform(curl);
        /* Check for errors */ 
        if(resCode != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(resCode));
    }

    return resString;
};

/** Perform a basic HTTP POST method via libcurl */
string HttpClient::post(string url, string user, string pass, string fields){
    CURLcode resCode;
    string resString = "";
    const char * charUrl = url.c_str();

    // authorization
    curl_easy_setopt(curl, CURLOPT_USERNAME, user.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, pass.c_str());

    if(this->curl) {
        curl_easy_setopt(curl, CURLOPT_URL, charUrl);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resString);
        curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());  // "name=daniel&project=curl"

        /* Perform the request, res will get the return code */ 
        resCode = curl_easy_perform(curl);
        /* Check for errors */ 
        if(resCode != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(resCode));
    }

    return resString;
};