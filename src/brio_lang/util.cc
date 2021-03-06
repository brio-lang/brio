///
/// Brio Lang
/// util.cc
///
/// Shared Utilities
///


#include <brio_lang/util.h>

#include <fstream>
#include <string>

/**
 * Reads a file and returns the contents as a string.
 * 
 * @param file The input file path
 * @return string containing contents of the file
 */
string Util::read_file(string file){
    string sourcecode;
    string line;
    ifstream myfile;
    myfile.open(file);
    if (myfile.is_open()){
        while (getline(myfile, line)){
            sourcecode += line + "\n";
        }
        myfile.close();
    }
    else {
        throw IOError("unable to open input file: " + file); 
    }
    return sourcecode;
}

/**
 * Writes a file with the provided data.
 * 
 * @param file The output file path
 * @param data The output file contents
 */
void Util::write_file(string file, string data){
    ofstream myfile;
    myfile.open(file);
    if (myfile.is_open()){
        myfile << data;
    }
    else {
        throw IOError("unable to open output file: " + file); 
    }
    myfile.close();
}

/**
 * Returns true or false if the provided file exists.
 */
bool Util::file_exists(string path){
    ifstream f(path.c_str());
    return f.good();
};

/**
 * Determines if a string ends with another string value.
 * https://stackoverflow.com/a/874160
 * 
 * @param fullString The string to verify if contains a particular ending
 * @param ending The ending string value to verify 
 * @returns boolean to indicate true or false if string ends with value
 */
bool Util::string_has_ending(string const &fullString, string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(
            fullString.length() - ending.length(), ending.length(), ending
        ));
    } else {
        return false;
    }
}

/**
 * Escapes control characters so they are included when printing to the screen.
 */
string Util::to_raw_string(string const& in)
{
    string ret;
    int i = 0;
    while (i < in.size()){
        char index = in[i];
        if (index == '\n'){
            ret += "\\n";
            i += 1;
        }
        else if (index == '\t'){
            ret += "\\t";
            i += 1;
        }
        else if (index == '\v'){
            ret += "\\v";
            i += 1;
        }
        else if (index == '\b'){
            ret += "\\b";
            i += 1;
        }
        else if (index == '\r'){
            ret += "\\r";
            i += 1;
        }
        else if (index == '\f'){
            ret += "\\f";
            i += 1;
        }
        else if (index == '\a'){
            ret += "\\a";
            i += 1;
        }
        else if (index == '\\'){
            ret += "\\\\";
            i += 1;
        }
        else if (index == '\0'){
            ret += "\\0";
            i += 1;
        }
        else{
            ret += index;
            i += 1;
        }
    }
    return ret;
}

/**
 * Takes a raw string and escapes special characters.
 */
string Util::escape_raw_string(string input){
    string newString;
    int index = 0;
    while (index < input.size()){
        if (input[index] == '\\' && index+1 < input.size()){
            char nextChar = input[index+1];
            if (nextChar == 'n'){
                newString += '\n';
                index += 2;
            }
            else if (nextChar == 't'){
                newString += '\t';
                index += 2;
            }
            else if (nextChar == 'v'){
                newString += '\v';
                index += 2;
            }
            else if (nextChar == 'b'){
                newString += '\b';
                index += 2;
            }
            else if (nextChar == 'r'){
                newString += '\r';
                index += 2;
            }
            else if (nextChar == 'f'){
                newString += '\f';
                index += 2;
            }
            else if (nextChar == 'a'){
                newString += '\a';
                index += 2;
            }
            else if (nextChar == '\\'){
                newString += '\\';
                index += 2;
            }
            else if (nextChar == '0'){
                newString += '\0';
                index += 2;
            }
        }else{
            newString += input[index];
            index += 1;
        }
    }
    return newString;
}