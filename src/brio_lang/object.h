#ifndef BRIOLANG_SRC_OBJECT_H_
#define BRIOLANG_SRC_OBJECT_H_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <brio_lang/symbol.h>

using namespace std;

class Boolean;  // forward reference
class Integer;  // forward reference
class Array;  // forward referene

class Object {
    public:
        Object();
        virtual string toString(){return "Object";};
        virtual string getType(){return "Object";};
        virtual Integer * getSize();
        virtual Object * index(Object *);
        virtual Object * clone();
        virtual Boolean * equals(Object *);
        virtual Boolean * notEquals(Object *);
        virtual Object * add(Object *);
        virtual Object * subtract(Object *);
        virtual Object * multiply(Object *);
        virtual Object * divide(Object *);
        virtual Object * modulus(Object *);
        virtual Object * bitOr(Object *);
        virtual Object * bitXor(Object *);
        virtual Boolean * boolOr(Object *);
        virtual Object * bitAnd(Object *);
        virtual Boolean * boolAnd(Object *);
        virtual Object * exponent(Object *);
        virtual Boolean * greaterThan(Object *);
        virtual Integer * bitShiftRight(Object *);
        virtual Boolean * greaterThanOrEqual(Object *);
        virtual Boolean * lessThan(Object *);
        virtual Integer * bitShiftLeft(Object *);
        virtual Boolean * lessThanOrEqual(Object *);
        virtual Object * builtIn(string attribute);
        virtual Object * builtIn(string method_id, Array * params);
    private:
};

class String : public Object {
    public:
        String();
        String(Object * obj);
        String(string str_value);
        string getType(){return "String";};
        Integer * getSize();
        Boolean * equals(Object *);
        Boolean * notEquals(Object *);
        Object * add(Object *);
        Object * multiply(Object *);
        Boolean * boolOr(Object *);
        Boolean * boolAnd(Object *);
        Boolean * greaterThan(Object *);
        Boolean * greaterThanOrEqual(Object *);
        Boolean * lessThan(Object *);
        Boolean * lessThanOrEqual(Object *);
        Object * index(Object *);
        Object * clone();
        Object * builtIn(string method_id, Array * params);
        Object * builtIn(string attribute);
        string toString();
        string value;
    private:
};

class Integer : public Object {
    public:
        Integer();
        Integer(Object * obj);
        Integer(int int_value);
        Integer(string str_value);
        string getType(){return "Integer";};
        Boolean * equals(Object *);
        Boolean * notEquals(Object *);
        Object * add(Object *);
        Object * subtract(Object *);
        Object * multiply(Object *);
        Object * divide(Object *);
        Object * modulus(Object *);
        Object * bitOr(Object *);
        Object * bitXor(Object *);
        Boolean * boolOr(Object *);
        Object * bitAnd(Object *);
        Boolean * boolAnd(Object *);
        Object * exponent(Object *);
        Boolean * greaterThan(Object *);
        Integer * bitShiftRight(Object *);
        Boolean * greaterThanOrEqual(Object *);
        Boolean * lessThan(Object *);
        Integer * bitShiftLeft(Object *);
        Boolean * lessThanOrEqual(Object *);
        Object * clone();
        Object * builtIn(string method_id, Array * params);
        Object * builtIn(string attribute);
        string toString();
        int value;
    private:
};

class Decimal : public Object {
    public:
        Decimal();
        Decimal(Object * obj);
        Decimal(float float_value);
        Decimal(string str_value);
        string getType(){return "Decimal";};
        Boolean * equals(Object *);
        Boolean * notEquals(Object *);
        Object * add(Object *);
        Object * subtract(Object *);
        Object * multiply(Object *);
        Object * divide(Object *);
        Object * modulus(Object *);
        Boolean * boolOr(Object *);
        Boolean * boolAnd(Object *);
        Object * exponent(Object *);
        Boolean * greaterThan(Object *);
        Boolean * greaterThanOrEqual(Object *);
        Boolean * lessThan(Object *);
        Boolean * lessThanOrEqual(Object *);
        Object * clone();
        Object * builtIn(string method_id, Array * params);
        Object * builtIn(string attribute);
        string toString();
        float value;
    private:
};

class Boolean : public Object {
    public:
        Boolean();
        Boolean(Object * obj);
        Boolean(bool bool_value);
        Boolean(string str_value);
        string getType(){return "Boolean";};
        Boolean * equals(Object *);
        Boolean * notEquals(Object *);
        Object * add(Object *);
        Object * subtract(Object *);
        Object * multiply(Object *);
        Object * divide(Object *);
        Object * modulus(Object *);
        Object * bitOr(Object *);
        Object * bitXor(Object *);
        Boolean * boolOr(Object *);
        Object * bitAnd(Object *);
        Boolean * boolAnd(Object *);
        Object * exponent(Object *);
        Boolean * greaterThan(Object *);
        Integer * bitShiftRight(Object *);
        Boolean * greaterThanOrEqual(Object *);
        Boolean * lessThan(Object *);
        Integer * bitShiftLeft(Object *);
        Boolean * lessThanOrEqual(Object *);
        Object * clone();
        Object * builtIn(string method_id, Array * params);
        Object * builtIn(string attribute);
        string toString();
        bool value;
    private:
};

class Array : public Object {
    public:
        Array();
        Array(Object * obj);
        Array(vector<Object*>);
        string getType(){return "Array";};
        Integer * getSize();
        Boolean * equals(Object *);
        Boolean * notEquals(Object *);
        Object * add(Object *);
        Object * subtract(Object *);
        Object * multiply(Object *);
        Object * divide(Object *);
        Object * modulus(Object *);
        Object * bitOr(Object *);
        Object * bitXor(Object *);
        Boolean * boolOr(Object *);
        Object * bitAnd(Object *);
        Boolean * boolAnd(Object *);
        Object * exponent(Object *);
        Boolean * greaterThan(Object *);
        Integer * bitShiftRight(Object *);
        Boolean * greaterThanOrEqual(Object *);
        Boolean * lessThan(Object *);
        Integer * bitShiftLeft(Object *);
        Boolean * lessThanOrEqual(Object *);
        Object * index(Object *);
        Object * clone();
        void push(Array *);
        Object * pop(Array *);
        Object * builtIn(string method_id, Array * params);
        Object * builtIn(string attribute);
        string toString();
        vector<Object*> value;
    private:
};

class Dictionary : public Object {
    public:
        Dictionary();
        Dictionary(Object * obj);
        string getType(){return "Dictionary";};
        Integer * getSize();
        Boolean * equals(Object *);
        Boolean * notEquals(Object *);
        Object * add(Object *);
        Object * subtract(Object *);
        Boolean * boolOr(Object *);
        Boolean * boolAnd(Object *);
        Boolean * greaterThan(Object *);
        Boolean * greaterThanOrEqual(Object *);
        Boolean * lessThan(Object *);
        Boolean * lessThanOrEqual(Object *);
        Object * index(Object *);
        Object * clone();
        Object * pop(Array *);
        Object * builtIn(string method_id, Array * params);
        Object * builtIn(string attribute);
        string toString();
        map<Object*, Object*> value;
    private:
};

class None : public Object {
    public:
        None();
        string getType(){return "None";};
        Boolean * equals(Object *);
        Boolean * notEquals(Object *);
        Boolean * boolOr(Object *);
        Boolean * boolAnd(Object *);
        Object * clone();
        Object * builtIn(string method_id, Array * params);
        Object * builtIn(string attribute);
        string toString();
        bool value;
    private:
};

class File : public Object {
    public:
        File(string filePath);
        File(string filePath, string fileMode);
        File(string filePath, string fileMode, bool universalNewLines);
        string getType(){return "File";};
        Boolean * equals(Object *);
        Boolean * notEquals(Object *);
        Boolean * boolOr(Object *);
        Boolean * boolAnd(Object *);
        void open();
        Object * clone();
        Object * builtIn(string method_id, Array * params);
        Object * builtIn(string attribute);
        string toString();
        String * read();
        void write(Array * params);
        void writeline(Array * params);
        void writelines(Array * params);
        Integer * tell();
        void flush();
        void seek(Array * params);
        Array * readlines(Array * params);
        String * readline(Array * params);
        void close();
    private:
        bool universalNewLines;
        string filePath;
        string fileMode;
        string encoding;
        fstream fstreamFile;
        void processFileMode(string);
        string processNewLines(string);
        ios_base::openmode mode;
        bool modeWrite;
        bool modeRead;
        bool modeAppend;
};

class Socket : public Object {
    public:
        Socket();
        string getType(){return "Socket";};
        Boolean * equals(Object *);
        Boolean * notEquals(Object *);
        Boolean * boolOr(Object *);
        Boolean * boolAnd(Object *);
        Object * clone();
        Object * builtIn(string method_id, Array * params);
        Object * builtIn(string attribute);
        string toString();

        // void accept();
        // void bind(string address);
        // void close();
        // void connect();
        // int fileno();
        // string getPeerName();
        // string getSocketName();
        // void listen();
        // void receive();
        // void send(string data);
        // string read();
        // void setTimeout();
        // void getTimeout();

        string family;
        string type;
        string protocol;
    private:
};

#endif  // BRIOLANG_SRC_OBJECT_H_