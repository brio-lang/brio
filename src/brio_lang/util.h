#ifndef BRIOLANG_SRC_UTIL_H_
#define BRIOLANG_SRC_UTIL_H_

#include <brio_lang/exceptions.h>

#include <string>

using namespace std;

class Util {
    public:
        static void write_file(string file, string data);
        static string read_file(string path);
        static bool string_has_ending(string const &fullString, string const &ending);
        static string escape_raw_string(string input);
        static string to_raw_string(string const& in);
        static bool file_exists(string path);
    private:
};

#endif  // BRIOLANG_SRC_UTIL_H_