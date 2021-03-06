#ifndef BRIOLANG_SRC_EXCEPTIONS_H_
#define BRIOLANG_SRC_EXCEPTIONS_H_

#include <brio_lang/object.h>
#include <iostream>
#include <string>

using namespace std;


class ExceptionUtil{
    public:
        static Object * castException(string name, void * e);
        static Object * createException(string name, string msg); 
};

class BaseException : public std::exception, public Object{
    public:
        BaseException();
        BaseException(string message);
        BaseException(string message, int lineno);
        const char * what () const throw (){
            return const_cast<char*>(this->message.c_str());
        };
        virtual string getType(){return "BaseException";};
        virtual Object * builtIn(string method_id, Array * params);
        virtual Object * builtIn(string attribute);
        virtual string toString();
        virtual string getName();
        string getMessage();
    private:
        string message;
        int lineno;
};

class ClassException : public BaseException{
    public:
        ClassException(string name);
        ClassException(string name, string message);
        ClassException(string name, string message, int lineno);
        string getType(){return "ClassException"; };
        string getName();
        string toString();
    private:
        string name;
};

class Exception : public BaseException{
    public:
        Exception();
        Exception(string message);
        Exception(string message, int lineno);
        string getType(){return "Exception";};
};

class LexerError : public BaseException{
    public:
        LexerError();
        LexerError(string message);
        LexerError(string message, int lineno);
        string getType(){return "LexerError";};
};

class ParserError : public BaseException{
    public:
        ParserError();
        ParserError(string message);
        ParserError(string message, int lineno);
        string getType(){return "ParserError";};
};

class RecognitionException : public BaseException{
    public:
        RecognitionException();
        RecognitionException(string message);
        RecognitionException(string message, int lineno);
        string getType(){return "RecognitionException";};
};

class MismatchedTokenException : public RecognitionException{
    public:
        MismatchedTokenException();
        MismatchedTokenException(string message);
        MismatchedTokenException(string message, int lineno);
        string getType(){return "MismatchedTokenException";};
};

class SyntaxError : public BaseException{
    public:
        SyntaxError();
        SyntaxError(string message);
        SyntaxError(string message, int lineno);
        string getType(){return "SyntaxError";};
};

class ArithmeticError : public BaseException{
    public:
        ArithmeticError();
        ArithmeticError(string message);
        ArithmeticError(string message, int lineno);
        string getType(){return "ArithmeticError";};
};

class BufferError : public BaseException{
    public:
        BufferError();
        BufferError(string message);
        BufferError(string message, int lineno);
        string getType(){return "BufferError";};
};

class IOError : public BaseException{
    public:
        IOError();
        IOError(string message);
        IOError(string message, int lineno);
        string getType(){return "IOError";};
};

class LookupError : public BaseException{
    public:
        LookupError();
        LookupError(string message);
        LookupError(string message, int lineno);
        string getType(){return "LookupError";};
};

class AttributeError : public Exception{
    public:
        AttributeError();
        AttributeError(string message);
        AttributeError(string message, int lineno);
        string getType(){return "AttributeError";};
};

class EOFError : public Exception{
    public:
        EOFError();
        EOFError(string message);
        EOFError(string message, int lineno);
        string getType(){return "EOFError";};
};

class ImportError : public Exception{
    public:
        ImportError();
        ImportError(string message);
        ImportError(string message, int lineno);
        string getType(){return "ImportError";};
};

class ModuleNotFoundError : public ImportError{
    public:
        ModuleNotFoundError();
        ModuleNotFoundError(string message);
        ModuleNotFoundError(string message, int lineno);
        string getType(){return "ModuleNotFoundError";};
};

class IndexError : public LookupError{
    public:
        IndexError();
        IndexError(string message);
        IndexError(string message, int lineno);
        string getType(){return "IndexError";};
};

class KeyError : public LookupError{
    public:
        KeyError();
        KeyError(string message);
        KeyError(string message, int lineno);
        string getType(){return "KeyError";};
};

class KeyboardInterrupt : public BaseException{
    public:
        KeyboardInterrupt();
        KeyboardInterrupt(string message);
        KeyboardInterrupt(string message, int lineno);
        string getType(){return "KeyboardInterrupt";};
};

class MemoryError : public Exception{
    public:
        MemoryError();
        MemoryError(string message);
        MemoryError(string message, int lineno);
        string getType(){return "MemoryError";};
};

class NameError : public Exception{
    public:
        NameError();
        NameError(string message);
        NameError(string message, int lineno);
        string getType(){return "NameError";};
};

class NotImplementedError : public Exception{
    public:
        NotImplementedError();
        NotImplementedError(string message);
        NotImplementedError(string message, int lineno);
        string getType(){return "NotImplementedError";};
};

class OSError : public Exception{
    public:
        OSError();
        OSError(string message);
        OSError(string message, int lineno);
        string getType(){return "OSError";};
};

class OverflowError : public Exception{
    public:
        OverflowError();
        OverflowError(string message);
        OverflowError(string message, int lineno);
        string getType(){return "OverflowError";};
};

class RecursionError : public Exception{
    public:
        RecursionError();
        RecursionError(string message);
        RecursionError(string message, int lineno);
        string getType(){return "RecursionError";};
};

class RuntimeError : public Exception{
    public:
        RuntimeError();
        RuntimeError(string message);
        RuntimeError(string message, int lineno);
        string getType(){return "RuntimeError";};
};

class SystemError : public Exception{
    public:
        SystemError();
        SystemError(string message);
        SystemError(string message, int lineno);
        string getType(){return "SystemError";};
};

class TypeError : public Exception{
    public:
        TypeError();
        TypeError(string message);
        TypeError(string message, int lineno);
        string getType(){return "TypeError";};
};

class ValueError : public Exception{
    public:
        ValueError();
        ValueError(string message);
        ValueError(string message, int lineno);
        string getType(){return "ValueError";};
};

class ZeroDivisionError : public ArithmeticError{
    public:
        ZeroDivisionError();
        ZeroDivisionError(string message);
        ZeroDivisionError(string message, int lineno);
        string getType(){return "ZeroDivisionError";};
};

class FileExistsError : public Exception{
    public:
        FileExistsError();
        FileExistsError(string message);
        FileExistsError(string message, int lineno);
        string getType(){return "FileExistsError";};
};

class FileNotFoundError : public Exception{
    public:
        FileNotFoundError();
        FileNotFoundError(string message);
        FileNotFoundError(string message, int lineno);
        string getType(){return "FileNotFoundError";};
};

class TimeoutError : public Exception{
    public:
        TimeoutError();
        TimeoutError(string message);
        TimeoutError(string message, int lineno);
        string getType(){return "TimeoutError";};
};

#endif  // BRIOLANG_SRC_EXCEPTIONS_H_