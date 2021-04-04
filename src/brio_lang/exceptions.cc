///
/// Brio Lang
/// exceptions.cc
///

#include <brio_lang/exceptions.h>
#include <iostream>
#include <string>

Object * ExceptionUtil::castException(string name, void * e){
    if (name == "BaseException"){
        return static_cast<BaseException*>(e);
    }else if (name=="ClassException"){
        return static_cast<ClassException*>(e);
    }else if (name=="Exception"){
        return static_cast<Exception*>(e);
    }else if (name=="LexerError"){
        return static_cast<LexerError*>(e);
    }else if (name=="ParserError"){
        return static_cast<ParserError*>(e);
    }else if (name=="RecognitionException"){
        return static_cast<RecognitionException*>(e);
    }else if (name=="PreviousParseFailedException"){
        return static_cast<PreviousParseFailedException*>(e);
    }else if (name=="MismatchedTokenException"){
        return static_cast<MismatchedTokenException*>(e);
    }else if (name=="SyntaxError"){
        return static_cast<SyntaxError*>(e);
    }else if (name=="ArithmeticError"){
        return static_cast<ArithmeticError*>(e);
    }else if (name=="BufferError"){
        return static_cast<BufferError*>(e);
    }else if (name=="IOError"){
        return static_cast<IOError*>(e);
    }else if (name=="LookupError"){
        return static_cast<LookupError*>(e);
    }else if (name=="AttributeError"){
        return static_cast<AttributeError*>(e);
    }else if (name=="EOFError"){
        return static_cast<EOFError*>(e);
    }else if (name=="ImportError"){
        return static_cast<ImportError*>(e);
    }else if (name=="ModuleNotFoundError"){
        return static_cast<ModuleNotFoundError*>(e);
    }else if (name=="IndexError"){
        return static_cast<IndexError*>(e);
    }else if (name=="KeyError"){
        return static_cast<KeyError*>(e);
    }else if (name=="KeyboardInterrupt"){
        return static_cast<KeyboardInterrupt*>(e);
    }else if (name=="MemoryError"){
        return static_cast<MemoryError*>(e);
    }else if (name=="NameError"){
        return static_cast<NameError*>(e);
    }else if (name=="NotImplementedError"){
        return static_cast<NotImplementedError*>(e);
    }else if (name=="OSError"){
        return static_cast<OSError*>(e);
    }else if (name=="OverflowError"){
        return static_cast<OverflowError*>(e);
    }else if (name=="RecursionError"){
        return static_cast<RecursionError*>(e);
    }else if (name=="RuntimeError"){
        return static_cast<RuntimeError*>(e);
    }else if (name=="SystemError"){
        return static_cast<SystemError*>(e);
    }else if (name=="TypeError"){
        return static_cast<TypeError*>(e);
    }else if (name=="ValueError"){
        return static_cast<ValueError*>(e);
    }else if (name=="ZeroDivisionError"){
        return static_cast<ZeroDivisionError*>(e);
    }else if (name=="FileExistsError"){
        return static_cast<FileExistsError*>(e);
    }else if (name=="FileNotFoundError"){
        return static_cast<FileNotFoundError*>(e);
    }else if (name=="TimeoutError"){
        return static_cast<TimeoutError*>(e);
    }
};

Object * ExceptionUtil::createException(string name, string msg){
    if (name == "BaseException"){
        return new BaseException(msg);
    }else if (name=="ClassException"){
        return new ClassException(msg);
    }else if (name=="Exception"){
        return new Exception(msg);
    }else if (name=="LexerError"){
        return new LexerError(msg);
    }else if (name=="ParserError"){
        return new ParserError(msg);
    }else if (name=="RecognitionException"){
        return new RecognitionException(msg);
    }else if (name=="PreviousParseFailedException"){
        return new PreviousParseFailedException(msg);
    }else if (name=="MismatchedTokenException"){
        return new MismatchedTokenException(msg);
    }else if (name=="SyntaxError"){
        return new SyntaxError(msg);
    }else if (name=="ArithmeticError"){
        return new ArithmeticError(msg);
    }else if (name=="BufferError"){
        return new BufferError(msg);
    }else if (name=="IOError"){
        return new IOError(msg);
    }else if (name=="LookupError"){
        return new LookupError(msg);
    }else if (name=="AttributeError"){
        return new AttributeError(msg);
    }else if (name=="EOFError"){
        return new EOFError(msg);
    }else if (name=="ImportError"){
        return new ImportError(msg);
    }else if (name=="ModuleNotFoundError"){
        return new ModuleNotFoundError(msg);
    }else if (name=="IndexError"){
        return new IndexError(msg);
    }else if (name=="KeyError"){
        return new KeyError(msg);
    }else if (name=="KeyboardInterrupt"){
        return new KeyboardInterrupt(msg);
    }else if (name=="MemoryError"){
        return new MemoryError(msg);
    }else if (name=="NameError"){
        return new NameError(msg);
    }else if (name=="NotImplementedError"){
        return new NotImplementedError(msg);
    }else if (name=="OSError"){
        return new OSError(msg);
    }else if (name=="OverflowError"){
        return new OverflowError(msg);
    }else if (name=="RecursionError"){
        return new RecursionError(msg);
    }else if (name=="RuntimeError"){
        return new RuntimeError(msg);
    }else if (name=="SystemError"){
        return new SystemError(msg);
    }else if (name=="TypeError"){
        return new TypeError(msg);
    }else if (name=="ValueError"){
        return new ValueError(msg);
    }else if (name=="ZeroDivisionError"){
        return new ZeroDivisionError(msg);
    }else if (name=="FileExistsError"){
        return new FileExistsError(msg);
    }else if (name=="FileNotFoundError"){
        return new FileNotFoundError(msg);
    }else if (name=="TimeoutError"){
        return new TimeoutError(msg);
    }
};

BaseException::BaseException(){};

BaseException::BaseException(string msg){
    this->message = msg;
};

BaseException::BaseException(string msg, int lineno){
    this->message = "line " + to_string(lineno) + ": " + msg;
    this->lineno = lineno;
};

string BaseException::getName(){
    return this->getType();
};

Object * BaseException::builtIn(string method_id, Array * params){
    throw ValueError("method '" + method_id + "' not found in " + this->getType());
};

Object * BaseException::builtIn(string attribute){
    if (attribute == "message"){
        return new String(this->toString());
    }
    throw ValueError(this->getType() + " has no member '" + attribute + "'");
};

string BaseException::toString(){
    if (this->message == "") return this->getName();
    return this->getName() + ": " + this->getMessage();
};

string BaseException::getMessage(){
    return this->message;
};

ClassException::ClassException(string name) : BaseException() { this->name = name; };
ClassException::ClassException(string name, string message) : BaseException(message) { this->name = name; };
ClassException::ClassException(string name, string message, int lineno) : BaseException(message, lineno) { this->name = name; };
string ClassException::toString(){
    return this->getName() + ": " + this->getMessage();
};
string ClassException::getName(){
    return this->name;
};

Exception::Exception() : BaseException() {};
Exception::Exception(string message) : BaseException(message) {};
Exception::Exception(string message, int lineno) : BaseException(message, lineno) {};

SyntaxError::SyntaxError() : BaseException() {};
SyntaxError::SyntaxError(string message) : BaseException(message) {};
SyntaxError::SyntaxError(string message, int lineno) : BaseException(message, lineno) {};

RecognitionException::RecognitionException() : BaseException() {};
RecognitionException::RecognitionException(string message) : BaseException(message) {};
RecognitionException::RecognitionException(string message, int lineno) : BaseException(message, lineno) {};

PreviousParseFailedException::PreviousParseFailedException() : RecognitionException() {};
PreviousParseFailedException::PreviousParseFailedException(string message) : RecognitionException(message) {};
PreviousParseFailedException::PreviousParseFailedException(string message, int lineno) : RecognitionException(message, lineno) {};

MismatchedTokenException::MismatchedTokenException() : RecognitionException() {};
MismatchedTokenException::MismatchedTokenException(string message) : RecognitionException(message) {};
MismatchedTokenException::MismatchedTokenException(string message, int lineno) : RecognitionException(message, lineno) {};

LexerError::LexerError() : BaseException() {};
LexerError::LexerError(string message) : BaseException(message) {};
LexerError::LexerError(string message, int lineno) : BaseException(message, lineno) {};

ParserError::ParserError() : BaseException() {};
ParserError::ParserError(string message) : BaseException(message) {};
ParserError::ParserError(string message, int lineno) : BaseException(message, lineno) {};

ArithmeticError::ArithmeticError() : BaseException() {};
ArithmeticError::ArithmeticError(string message) : BaseException(message) {};
ArithmeticError::ArithmeticError(string message, int lineno) : BaseException(message, lineno) {};

BufferError::BufferError() : BaseException() {};
BufferError::BufferError(string message) : BaseException(message) {};
BufferError::BufferError(string message, int lineno) : BaseException(message, lineno) {};

IOError::IOError() : BaseException() {};
IOError::IOError(string message) : BaseException(message) {};
IOError::IOError(string message, int lineno) : BaseException(message, lineno) {};

LookupError::LookupError() : BaseException() {};
LookupError::LookupError(string message) : BaseException(message) {};
LookupError::LookupError(string message, int lineno) : BaseException(message, lineno) {};

AttributeError::AttributeError() : Exception() {};
AttributeError::AttributeError(string message) : Exception(message) {};
AttributeError::AttributeError(string message, int lineno) : Exception(message, lineno) {};

EOFError::EOFError() : Exception() {};
EOFError::EOFError(string message) : Exception(message) {};
EOFError::EOFError(string message, int lineno) : Exception(message, lineno) {};

ImportError::ImportError() : Exception() {};
ImportError::ImportError(string message) : Exception(message) {};
ImportError::ImportError(string message, int lineno) : Exception(message, lineno) {};

ModuleNotFoundError::ModuleNotFoundError() : ImportError() {};
ModuleNotFoundError::ModuleNotFoundError(string message) : ImportError(message) {};
ModuleNotFoundError::ModuleNotFoundError(string message, int lineno) : ImportError(message, lineno) {};

IndexError::IndexError() : LookupError() {};
IndexError::IndexError(string message) : LookupError(message) {};
IndexError::IndexError(string message, int lineno) : LookupError(message, lineno) {};

KeyError::KeyError() : LookupError() {};
KeyError::KeyError(string message) : LookupError(message) {};
KeyError::KeyError(string message, int lineno) : LookupError(message, lineno) {};

KeyboardInterrupt::KeyboardInterrupt() : BaseException() {};
KeyboardInterrupt::KeyboardInterrupt(string message) : BaseException(message) {};
KeyboardInterrupt::KeyboardInterrupt(string message, int lineno) : BaseException(message, lineno) {};

MemoryError::MemoryError() : Exception() {};
MemoryError::MemoryError(string message) : Exception(message) {};
MemoryError::MemoryError(string message, int lineno) : Exception(message, lineno) {};

NameError::NameError() : Exception() {};
NameError::NameError(string message) : Exception(message) {};
NameError::NameError(string message, int lineno) : Exception(message, lineno) {};

NotImplementedError::NotImplementedError() : Exception() {};
NotImplementedError::NotImplementedError(string message) : Exception(message) {};
NotImplementedError::NotImplementedError(string message, int lineno) : Exception(message, lineno) {};

OSError::OSError() : Exception() {};
OSError::OSError(string message) : Exception(message) {};
OSError::OSError(string message, int lineno) : Exception(message, lineno) {};

OverflowError::OverflowError() : Exception() {};
OverflowError::OverflowError(string message) : Exception(message) {};
OverflowError::OverflowError(string message, int lineno) : Exception(message, lineno) {};

RecursionError::RecursionError() : Exception() {};
RecursionError::RecursionError(string message) : Exception(message) {};
RecursionError::RecursionError(string message, int lineno) : Exception(message, lineno) {};

RuntimeError::RuntimeError() : Exception() {};
RuntimeError::RuntimeError(string message) : Exception(message) {};
RuntimeError::RuntimeError(string message, int lineno) : Exception(message, lineno) {};

SystemError::SystemError() : Exception() {};
SystemError::SystemError(string message) : Exception(message) {};
SystemError::SystemError(string message, int lineno) : Exception(message, lineno) {};

TypeError::TypeError() : Exception() {};
TypeError::TypeError(string message) : Exception(message) {};
TypeError::TypeError(string message, int lineno) : Exception(message, lineno) {};

ValueError::ValueError() : Exception() {};
ValueError::ValueError(string message) : Exception(message) {};
ValueError::ValueError(string message, int lineno) : Exception(message, lineno) {};

ZeroDivisionError::ZeroDivisionError() : ArithmeticError() {};
ZeroDivisionError::ZeroDivisionError(string message) : ArithmeticError(message) {};
ZeroDivisionError::ZeroDivisionError(string message, int lineno) : ArithmeticError(message, lineno) {};

FileExistsError::FileExistsError() : Exception() {};
FileExistsError::FileExistsError(string message) : Exception(message) {};
FileExistsError::FileExistsError(string message, int lineno) : Exception(message, lineno) {};

FileNotFoundError::FileNotFoundError() : Exception() {};
FileNotFoundError::FileNotFoundError(string message) : Exception(message) {};
FileNotFoundError::FileNotFoundError(string message, int lineno) : Exception(message, lineno) {};

TimeoutError::TimeoutError() : Exception() {};
TimeoutError::TimeoutError(string message) : Exception(message) {};
TimeoutError::TimeoutError(string message, int lineno) : Exception(message, lineno) {};