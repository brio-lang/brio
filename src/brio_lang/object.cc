///
/// Brio Lang
/// object.cc
///

#include <brio_lang/object.h>
#include <brio_lang/token.h>
#include <brio_lang/exceptions.h>
#include <brio_lang/util.h>

#include <math.h>


/**
 * Generic error message if an operrator is not supported
 */
Object * invalidOp(Object* lhs, string op, Object* rhs){
    std::cout << lhs->getType() + " does not support '" + op + "' with " + rhs->getType() << std::endl;
    return nullptr;
};


/// ##################################################################
/// Object definition
/// ##################################################################

Object::Object(){};

Object * Object::clone(){return nullptr;};

Object * Object::builtIn(string method_id, Array * params){
    throw NotImplementedError("built in method not implemented");
}

Object * Object::builtIn(string attribute){
    throw NotImplementedError("built in attribute not implemented");
}

Integer * Object::getSize(){
    throw NotImplementedError("size not implemented for " + this->getType());
}

Boolean * Object::equals(Object * o){
    return static_cast<Boolean*>(invalidOp(this, "==", o));
};

Boolean * Object::notEquals(Object * o){
    return static_cast<Boolean*>(invalidOp(this, "!=", o));
};

Object * Object::add(Object * o){
    return invalidOp(this, "add", o);
};

Object * Object::index(Object * o){
    return invalidOp(this, "index", o);
};

Object * Object::subtract(Object* o){
    return invalidOp(this, "subtract", o);
};

Object * Object::multiply(Object* o){
    return invalidOp(this, "multiply", o);
};

Object * Object::divide(Object* o){
    return invalidOp(this, "divide", o);
};

Object * Object::modulus(Object* o){
    return invalidOp(this, "modulus", o);
};

Object * Object::bitOr(Object* o){
    return invalidOp(this, "bitOR", o);
};

Object * Object::bitXor(Object* o){
    return invalidOp(this, "bitXOR", o);
};

Boolean * Object::boolOr(Object* o){
    return static_cast<Boolean*>(invalidOp(this, "boolOR", o));
};

Object * Object::bitAnd(Object* o){
    return invalidOp(this, "bitAND", o);
};

Boolean * Object::boolAnd(Object* o){
    return static_cast<Boolean*>(invalidOp(this, "boolAND", o));
};

Object * Object::exponent(Object* o){
    return invalidOp(this, "exponent", o);
};

Boolean * Object::greaterThan(Object* o){
    return static_cast<Boolean*>(invalidOp(this, ">", o));
};

Integer * Object::bitShiftRight(Object* o){
    return static_cast<Integer*>(invalidOp(this, ">>", o));
};

Boolean * Object::greaterThanOrEqual(Object* o){
    return static_cast<Boolean*>(invalidOp(this, ">=", o));
};

Boolean * Object::lessThan(Object* o){
    return static_cast<Boolean*>(invalidOp(this, "<", o));
};

Integer * Object::bitShiftLeft(Object* o){
    return static_cast<Integer*>(invalidOp(this, "<<", o));
};

Boolean * Object::lessThanOrEqual(Object* o){
    return static_cast<Boolean*>(invalidOp(this, "<=", o));
};

/// ##################################################################
/// String definition
/// ##################################################################

String::String(){};

String::String(Object * obj){
    this->value = obj->toString();
};

String::String(string str_value){
    this->value = str_value;
};

Object * String::builtIn(string method_id, Array * params){
    if (method_id == "size"){
        return this->getSize();
    }
    else{
        throw ValueError("'" + method_id + "' not implemented for " + this->getType());
    }
}

Object * String::builtIn(string attribute){
    throw NotImplementedError("built in attribute not implemented");
}

string String::toString(){
    return this->value;
};

Integer * String::getSize(){
    return new Integer(size(this->value));
}

Object * String::add(Object * obj){
    string result = this->value + obj->toString();
    return new String(result); 
};

Object * String::multiply(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * value = dynamic_cast<Integer *>(obj);
        string result;
        for (int i = 0; i<value->value; i++){
            result += this->value;
        }
        return new String(result); 
    }else{
        throw TypeError("error multiplying String");
    }
};

Boolean * String::boolOr(Object * obj){
    throw NotImplementedError("String boolOr not implemented yet");
};

Boolean * String::boolAnd(Object * obj){
    throw NotImplementedError("String boolAnd not implemented yet");
};

Boolean * String::greaterThan(Object * obj){
    if (dynamic_cast<String *>(obj) != nullptr){
        String * s = dynamic_cast<String *>(obj);
        bool result = size(this->value) > size(s->value);
        return new Boolean(result); 
    }else{
        throw TypeError("error String GT");
    }
};

Boolean * String::greaterThanOrEqual(Object * obj){
    if (dynamic_cast<String *>(obj) != nullptr){
        String * s = dynamic_cast<String *>(obj);
        bool result = size(this->value) >= size(s->value);
        return new Boolean(result); 
    }else{
        throw TypeError("error String GTE");
    }
};

Boolean * String::lessThan(Object * obj){
    if (dynamic_cast<String *>(obj) != nullptr){
        String * s = dynamic_cast<String *>(obj);
        bool result = size(this->value) < size(s->value);
        return new Boolean(result); 
    }else{
        throw TypeError("error String LT");
    }
};

Boolean * String::lessThanOrEqual(Object * obj){
    if (dynamic_cast<String *>(obj) != nullptr){
        String * s = dynamic_cast<String *>(obj);
        bool result = size(this->value) <= size(s->value);
        return new Boolean(result); 
    }else{
        throw TypeError("error String LTE");
    }
};

Object * String::clone(){
    return new String(this);
};

Object * String::index(Object * obj){
    if (static_cast<Integer*>(obj) != nullptr){
        Integer * index = static_cast<Integer*>(obj);
        if (index->value >= this->value.size()){
            throw IndexError("cannot access element " + std::to_string(index->value) + ", max index is " + std::to_string(this->value.size()-1));
        }else if (index->value < 0){
            throw IndexError("cannot access index less than 0");
        }
        string chr_str;
        chr_str.push_back(this->value[index->value]);
        return new String(chr_str);
    }else{
        throw TypeError("cannot access array index with this type");
    }
};

Boolean * String::equals(Object * obj){
    if (dynamic_cast<String *>(obj) != nullptr){
        String * i = dynamic_cast<String *>(obj);
        bool result = this->value == i->value;
        return new Boolean(result); 
    }else{
        throw TypeError("error with String equals");
    }
};

Boolean * String::notEquals(Object * obj){
    if (dynamic_cast<String *>(obj) != nullptr){
        String * i = dynamic_cast<String *>(obj);
        bool result = this->value != i->value;
        return new Boolean(result); 
    }else{
        throw TypeError("error with String notEquals");
    }
};

/// ##################################################################
/// Integer definition
/// ##################################################################

Integer::Integer(){};

Integer::Integer(Object * obj){
    if (dynamic_cast<String*>(obj) != nullptr){
        String * ptr = dynamic_cast<String*>(obj);
        this->value = std::stoi(ptr->value);
    }else if (dynamic_cast<Integer*>(obj) != nullptr){
        Integer * ptr = dynamic_cast<Integer*>(obj);
        this->value = ptr->value;
    }else if (dynamic_cast<Decimal*>(obj) != nullptr){
        Decimal * ptr = dynamic_cast<Decimal*>(obj);
        this->value = ptr->value; // float to int? fix this
    }else if (dynamic_cast<Boolean*>(obj) != nullptr){
        Boolean * ptr = dynamic_cast<Boolean*>(obj);
        if (ptr->toString() == Literals::TRUE) this->value = 1;
        else this->value = 0;
    }else{
        throw TypeError("cannot cast object to Integer");
    }
};

Integer::Integer(int int_value){
    this->value = int_value;
};

Integer::Integer(string str_value){
    this->value = std::stoi(str_value);
};

Object * Integer::builtIn(string attribute){
    throw NotImplementedError("built in attribute not implemented");
}

Object * Integer::builtIn(string method_id, Array * params){
    throw NotImplementedError("built in method not implemented");
}

string Integer::toString(){
    return std::to_string(this->value);
};

Object * Integer::add(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        int result = this->value + i->value;
        return new Integer(result); 
    }
    else if(dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        float result = this->value + i->value;
        return new Decimal(result);
    }
    else{
        throw TypeError("error adding Integer");
    }
};

Object * Integer::subtract(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        int result = this->value - i->value;
        return new Integer(result); 
    }
    else if(dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        float result = this->value - i->value;
        return new Decimal(result);
    }
    else{
        throw TypeError("error subtracting Integer");
    }
};

Object * Integer::multiply(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        int result = this->value * i->value;
        return new Integer(result); 
    }else if (dynamic_cast<String *>(obj) != nullptr){
        String * value = dynamic_cast<String *>(obj);
        string result;
        for (int i = 0; i<this->value; i++){
            result += value->value;
        }
        return new String(result); 
    }
    else if(dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        float result = this->value * i->value;
        return new Decimal(result);
    }
    else{
        throw TypeError("error multiplying Integer");
    }
};

Object * Integer::divide(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        float result = float(this->value) / float(i->value);
        if (ceil(result) == result){
            return new Integer(result);             
        }
        else{
            return new Decimal(result); 
        }
    }
    else if(dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        float result = this->value / i->value;
        return new Decimal(result);
    }
    else{
        throw TypeError("error dividing Integer");
    }
};

Object * Integer::exponent(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        float result = powf(this->value, i->value);
        if (ceil(result) == result){
            return new Integer(result);             
        }
        else{
            return new Decimal(result); 
        }
    }
    else if(dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        float result = pow(this->value, i->value);
        return new Decimal(result);
    }
    else{
        throw TypeError("error with Integer exponent");
    }
};

Object * Integer::modulus(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        int result = this->value % i->value;
        return new Integer(result); 
    }
    else if(dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        float result = fmod(this->value, i->value);
        return new Decimal(result);
    }
    else{
        throw TypeError("error with Integer modulus");
    }
};

Object * Integer::bitOr(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        int result = this->value | i->value;
        return new Integer(result); 
    }else{
        throw TypeError("error with Integer bitOr");
    }
};

Object * Integer::bitXor(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        int result = this->value ^ i->value;
        return new Integer(result); 
    }else{
        throw TypeError("error with Integer bitXor");
    }
};

Boolean * Integer::boolOr(Object * obj){
    throw TypeError("cannot boolOr Integer");
};

Boolean * Integer::boolAnd(Object * obj){
    throw TypeError("cannot boolAnd Integer");
};

Object * Integer::bitAnd(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        int result = this->value & i->value;
        return new Integer(result); 
    }else{
        throw TypeError("error with Integer bitAnd");
    }
};

Boolean * Integer::greaterThan(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        bool result = this->value > i->value;
        return new Boolean(result); 
    }
    else if(dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        bool result = this->value > i->value;
        return new Boolean(result);
    }
    else{
        throw TypeError("error with Integer GT");
    }
};

Integer * Integer::bitShiftRight(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        int result = this->value >> i->value;
        return new Integer(result); 
    }else{
        throw TypeError("error with Integer bitshiftright");
    }
};

Boolean * Integer::greaterThanOrEqual(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        bool result = this->value >= i->value;
        return new Boolean(result); 
    }
    else if(dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        bool result = this->value >= i->value;
        return new Boolean(result);
    }
    else{
        throw TypeError("error with Integer GTE");
    }
};

Boolean * Integer::lessThan(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        bool result = this->value < i->value;
        return new Boolean(result); 
    }
    else if(dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        bool result = this->value < i->value;
        return new Boolean(result);
    }
    else{
        throw TypeError("error with Integer LT");
    }
};

Integer * Integer::bitShiftLeft(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        int result = this->value << i->value;
        return new Integer(result); 
    }else{
        throw TypeError("error with Integer bitshiftleft");
    }
};

Boolean * Integer::lessThanOrEqual(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        bool result = this->value <= i->value;
        return new Boolean(result);
    }
    else if(dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        bool result = this->value <= i->value;
        return new Boolean(result);
    }
    else{
        throw TypeError("error with Integer LTE");
    }
};

Object * Integer::clone(){
    return new Integer(this);
};

Boolean * Integer::equals(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        bool result = this->value == i->value;
        return new Boolean(result); 
    }
    else if(dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        bool result = this->value == i->value;
        return new Boolean(result);
    }
    else{
        throw TypeError("error with Integer equals");
    }
};

Boolean * Integer::notEquals(Object * obj){
    if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        bool result = this->value != i->value;
        return new Boolean(result); 
    }
    else if(dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        bool result = this->value != i->value;
        return new Boolean(result);
    }
    else{
        throw TypeError("error with Integer notEquals");
    }
};

/// ##################################################################
/// Decimal definition
/// ##################################################################

Decimal::Decimal(){};

Decimal::Decimal(Object * obj){
    if (dynamic_cast<String*>(obj) != nullptr){
        String * ptr = dynamic_cast<String*>(obj);
        this->value = std::stof(ptr->value);
    }else if (dynamic_cast<Integer*>(obj) != nullptr){
        Integer * ptr = dynamic_cast<Integer*>(obj);
        this->value = ptr->value;
    }else if (dynamic_cast<Decimal*>(obj) != nullptr){
        Decimal * ptr = dynamic_cast<Decimal*>(obj);
        this->value = ptr->value;
    }else if (dynamic_cast<Boolean*>(obj) != nullptr){
        Boolean * ptr = dynamic_cast<Boolean*>(obj);
        if (ptr->toString() == Literals::TRUE) this->value = 1.0;
        else this->value = 0.0;
    }else{
        throw TypeError("cannot cast object to Decimal");
    }
};

Decimal::Decimal(float float_value){
    this->value = float_value;
};

Decimal::Decimal(string str_value){
    this->value = std::stof(str_value);
};

Object * Decimal::builtIn(string attribute){
    throw NotImplementedError("built in attribute not implemented");
}

Object * Decimal::builtIn(string method_id, Array * params){
    throw NotImplementedError("built in method not implemented");
}

string Decimal::toString(){
    string str = std::to_string(this->value);
    for (int i=str.size()-1; i>0; i--){
        if (str[i-1] == '.') break;
        if (str[i] == '0') str.pop_back();
        else break;
    };
    return str;
};

Object * Decimal::add(Object * obj){
    if (dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * d = dynamic_cast<Decimal *>(obj);
        float result = this->value + d->value;
        return new Decimal(result); 
    }
    else if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * d = dynamic_cast<Integer *>(obj);
        float result = this->value + d->value;
        return new Decimal(result); 
    }else{
        throw TypeError("error adding Decimal");
    }
};

Object * Decimal::subtract(Object * obj){
    if (dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * d = dynamic_cast<Decimal *>(obj);
        float result = this->value - d->value;
        return new Decimal(result); 
    }
    else if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * d = dynamic_cast<Integer *>(obj);
        float result = this->value - d->value;
        return new Decimal(result); 
    }
    else{
        throw TypeError("error subtracting Decimal");
    }
};

Object * Decimal::multiply(Object * obj){
    if (dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * d = dynamic_cast<Decimal *>(obj);
        float result = this->value * d->value;
        return new Decimal(result); 
    }
    else if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * d = dynamic_cast<Integer *>(obj);
        float result = this->value * d->value;
        return new Decimal(result); 
    }
    else{
        throw TypeError("error multiplying Decimal");
    }
};

Object * Decimal::divide(Object * obj){
    if (dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * d = dynamic_cast<Decimal *>(obj);
        float result = this->value / d->value;
        return new Decimal(result); 
    }
    else if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * d = dynamic_cast<Integer *>(obj);
        float result = this->value / d->value;
        return new Decimal(result); 
    }
    else{
        throw TypeError("error dividing Decimal");
    }
};

Object * Decimal::exponent(Object * obj){
    if (dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * d = dynamic_cast<Decimal *>(obj);
        float result = pow(this->value, d->value);
        return new Decimal(result); 
    }
    else if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * d = dynamic_cast<Integer *>(obj);
        float result = pow(this->value, d->value);
        return new Decimal(result); 
    }
    else{
        throw TypeError("error exponent Decimal");
    }
};

Object * Decimal::modulus(Object * obj){
    if (dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * d = dynamic_cast<Decimal *>(obj);
        float result = fmod(this->value, d->value);
        return new Decimal(result); 
    }
    else if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * d = dynamic_cast<Integer *>(obj);
        float result = fmod(this->value, d->value);
        return new Decimal(result); 
    }
    else{
        throw TypeError("error modulus Decimal");
    }
};

Boolean * Decimal::boolOr(Object * obj){
    throw TypeError("cannot boolOr Decimal");
};

Boolean * Decimal::boolAnd(Object * obj){
    throw TypeError("cannot boolAnd Decimal");
};

Boolean * Decimal::greaterThan(Object * obj){
    if (dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        bool result = this->value > i->value;
        return new Boolean(result);
    }
    else if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        bool result = this->value > i->value;
        return new Boolean(result); 
    }
    else{
        throw TypeError("error with Decimal GT");
    }
};

Boolean * Decimal::greaterThanOrEqual(Object * obj){
    if (dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        bool result = this->value >= i->value;
        return new Boolean(result);
    }
    else if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        bool result = this->value >= i->value;
        return new Boolean(result); 
    }
    else{
        throw TypeError("error with Decimal GTE");
    }
};

Boolean * Decimal::lessThan(Object * obj){
    if (dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        bool result = this->value < i->value;
        return new Boolean(result);
    }
    else if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        bool result = this->value < i->value;
        return new Boolean(result); 
    }
    else{
        throw TypeError("error with Decimal LT");
    }
};

Boolean * Decimal::lessThanOrEqual(Object * obj){
    if (dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        bool result = this->value <= i->value;
        return new Boolean(result);
    }
    else if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        bool result = this->value <= i->value;
        return new Boolean(result); 
    }
    else{
        throw TypeError("error with Decimal LTE");
    }
};

Object * Decimal::clone(){
    return new Decimal(this);
};

Boolean * Decimal::equals(Object * obj){
    if (dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        bool result = this->value == i->value;
        return new Boolean(result); 
    }
    else if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        bool result = this->value == i->value;
        return new Boolean(result); 
    }
    else{
        throw TypeError("error with Decimal equals");
    }
};

Boolean * Decimal::notEquals(Object * obj){
    if (dynamic_cast<Decimal *>(obj) != nullptr){
        Decimal * i = dynamic_cast<Decimal *>(obj);
        bool result = this->value != i->value;
        return new Boolean(result); 
    }
    else if (dynamic_cast<Integer *>(obj) != nullptr){
        Integer * i = dynamic_cast<Integer *>(obj);
        bool result = this->value != i->value;
        return new Boolean(result); 
    }
    else{
        throw TypeError("error with Decimal notEquals");
    }
};

/// ##################################################################
/// Boolean definition
/// ##################################################################

Boolean::Boolean(){};

Boolean::Boolean(Object * obj){
    if (dynamic_cast<String*>(obj) != nullptr){
        String * ptr = dynamic_cast<String*>(obj);
        this->value = size(ptr->value) > 0;
    }else if (dynamic_cast<Integer*>(obj) != nullptr){
        Integer * ptr = dynamic_cast<Integer*>(obj);
        this->value = ptr->value > 0;
    }else if (dynamic_cast<Decimal*>(obj) != nullptr){
        Decimal * ptr = dynamic_cast<Decimal*>(obj);
        this->value = ptr->value > 0.0;
    }else if (dynamic_cast<Boolean*>(obj) != nullptr){
        Boolean * ptr = dynamic_cast<Boolean*>(obj);
        this->value = ptr->value;
    }else if (dynamic_cast<None*>(obj) != nullptr){
        None * ptr = dynamic_cast<None*>(obj);
        this->value = false;
    }else{
        throw TypeError("cannot cast object to Boolean");
    }
};

Boolean::Boolean(bool bool_value){
    this->value = bool_value;
};

Boolean::Boolean(string str_value){
    if (str_value == Literals::TRUE){
        this->value = true;
    }else{
        this->value = false;
    }
};

Object * Boolean::builtIn(string method_id, Array * params){
    throw NotImplementedError("built in method not implemented");
}

Object * Boolean::builtIn(string attribute){
    throw NotImplementedError("built in attribute not implemented");
}

string Boolean::toString(){ 
    if(this->value) return Literals::TRUE;
    return Literals::FALSE;
};

Object * Boolean::add(Object * obj){
    throw TypeError("cannot add Boolean values");
};

Object * Boolean::subtract(Object * obj){
    throw TypeError("cannot subtract Boolean values");
};

Object * Boolean::multiply(Object * obj){
    throw TypeError("cannot multiply Boolean values");
};

Object * Boolean::divide(Object * obj){
    throw TypeError("cannot divide Boolean values");
};

Object * Boolean::exponent(Object * obj){
    throw TypeError("cannot exponent Boolean values");
};

Object * Boolean::modulus(Object * obj){
    throw TypeError("cannot modulus Boolean values");
};

Object * Boolean::bitOr(Object * obj){
    throw TypeError("cannot bitOr Boolean values");
};

Object * Boolean::bitXor(Object * obj){
    throw TypeError("cannot bitXor Boolean values");
};

Boolean * Boolean::boolOr(Object * obj){
    if (dynamic_cast<Boolean *>(obj) != nullptr){
        Boolean * b = dynamic_cast<Boolean *>(obj);
        bool result = this->value || b->value;
        return new Boolean(result); 
    }else{
        throw TypeError("error with Boolean OR");
    }
};

Object * Boolean::bitAnd(Object * obj){
    throw TypeError("cannot bitAnd Boolean values");
};

Boolean * Boolean::boolAnd(Object * obj){
    if (dynamic_cast<Boolean *>(obj) != nullptr){
        Boolean * b = dynamic_cast<Boolean *>(obj);
        bool result = this->value && b->value;
        return new Boolean(result); 
    }else{
        throw TypeError("error with Boolean AND");
    }
};

Boolean * Boolean::greaterThan(Object * obj){
    Integer intA = Integer(this);
    Integer intB = Integer(obj);
    bool result = intA.value > intB.value;
    return new Boolean(result);
};

Integer * Boolean::bitShiftRight(Object * obj){
    throw TypeError("cannot >> Boolean values");
};

Boolean * Boolean::greaterThanOrEqual(Object * obj){
    Integer intA = Integer(this);
    Integer intB = Integer(obj);
    bool result = intA.value >= intB.value;
    return new Boolean(result);
};

Boolean * Boolean::lessThan(Object * obj){
    Integer intA = Integer(this);
    Integer intB = Integer(obj);
    bool result = intA.value < intB.value;
    return new Boolean(result);
};

Integer * Boolean::bitShiftLeft(Object * obj){
    throw TypeError("cannot << Boolean values");
};

Boolean * Boolean::lessThanOrEqual(Object * obj){
    Integer intA = Integer(this);
    Integer intB = Integer(obj);
    bool result = intA.value <= intB.value;
    return new Boolean(result);
};

Object * Boolean::clone(){
    return new Boolean(this);
};

Boolean * Boolean::equals(Object * obj){
    if (dynamic_cast<Boolean *>(obj) != nullptr){
        Boolean * i = dynamic_cast<Boolean *>(obj);
        bool result = this->value == i->value;
        return new Boolean(result); 
    }else{
        throw TypeError("error with Boolean equals");
    }
};

Boolean * Boolean::notEquals(Object * obj){
    if (dynamic_cast<Boolean *>(obj) != nullptr){
        Boolean * i = dynamic_cast<Boolean *>(obj);
        bool result = this->value != i->value;
        return new Boolean(result); 
    }else{
        throw TypeError("error with Boolean notEquals");
    }
};

/// ##################################################################
/// Array definition
/// ##################################################################

Array::Array(){};

Array::Array(Object * obj){
    throw NotImplementedError("array instantiation not implemented yet.");
};

Array::Array(vector<Object*> elements){
    this->value = elements;
};

Object * Array::builtIn(string attribute){
    throw NotImplementedError("built in attribute not implemented");
}

Object * Array::builtIn(string method_id, Array * params){
    if (method_id == "size"){
        return this->getSize();
    }
    else if (method_id == "push"){
        this->push(params);
    }
    else if (method_id == "pop"){
        return this->pop(params);
    }
    else{
        throw ValueError("'" + method_id + "' not implemented for " + this->getType());
    }
}

Object * Array::pop(Array * params){
    int popIndex = this->value.size() - 1;
    
    if (params->value.size() > 1){
        throw ValueError("Array pop() accepts zero or one arguments.");
    }
    
    if (params->value.size() == 1){
        Integer * paramIndex = dynamic_cast<Integer*>(params->value[0]);
        if (paramIndex == nullptr){
            throw TypeError("Array pop() accepts an integer value only.");
        }
        popIndex = paramIndex->value;
    }

    if (popIndex > this->value.size()-1){
        throw ValueError("Cannot pop index greater than Array size.");
    }
    else if (popIndex < 0){
        throw ValueError("Cannot pop index less than Array size.");
    }

    Object * returnValue = this->value[popIndex];
    this->value.erase(this->value.begin() + popIndex);
    return returnValue;
}

void Array::push(Array * params){
    if (params->value.size() != 1){
        throw ValueError("Array push() accepts a single argument.");
    }
    Object * paramValue = params->value[0];
    this->value.push_back(paramValue);
}

string Array::toString(){
    string output = "["; 
    for (int i=0; i<this->value.size(); i++){
        Object * element = this->value[i];

        // include quotes if a string
        string elementStr = Util::to_raw_string(element->toString());
        if (dynamic_cast<String*>(element) != nullptr){
            output += "\"" + elementStr + "\"";            
        }else{
            output += elementStr;
        }
        if ((i+1) < this->value.size()) output += ", ";
    }
    output.append("]");
    return output;
};

Integer * Array::getSize(){
    return new Integer(size(this->value));
}

Object * Array::add(Object * obj){
    throw TypeError("cannot add Array values");
};

Object * Array::subtract(Object * obj){
    throw TypeError("cannot subtract Array values");
};

Object * Array::multiply(Object * obj){
    throw TypeError("cannot multiply Array values");
};

Object * Array::divide(Object * obj){
    throw TypeError("cannot divide Array values");
};

Object * Array::exponent(Object * obj){
    throw TypeError("cannot exponent Array values");
};

Object * Array::modulus(Object * obj){
    throw TypeError("cannot modulus Array values");
};

Object * Array::bitOr(Object * obj){
    throw TypeError("cannot bitOr Array values");
};

Object * Array::bitXor(Object * obj){
    throw TypeError("cannot bitXor Array values");
};

Boolean * Array::boolOr(Object * obj){
    throw NotImplementedError("Array bool or not implemented yet.");
};

Object * Array::bitAnd(Object * obj){
    throw TypeError("cannot bitAnd Array values");
};

Boolean * Array::boolAnd(Object * obj){
    throw NotImplementedError("Array bool and implemented yet.");
};

Boolean * Array::greaterThan(Object * obj){
    throw TypeError("cannot > Array values");
};

Integer * Array::bitShiftRight(Object * obj){
    throw TypeError("cannot >> Array values");
};

Boolean * Array::greaterThanOrEqual(Object * obj){
    throw TypeError("cannot >= Array values");
};

Boolean * Array::lessThan(Object * obj){
    throw TypeError("cannot < Array values");
};

Integer * Array::bitShiftLeft(Object * obj){
    throw TypeError("cannot << Array values");
};

Boolean * Array::lessThanOrEqual(Object * obj){
    throw TypeError("cannot <= Array values");
};

Object * Array::clone(){
    return new Array(this);
};

Object * Array::index(Object * obj){
    if (static_cast<Integer*>(obj) != nullptr){
        Integer * index = static_cast<Integer*>(obj);
        if (index->value >= this->value.size()){
            throw IndexError("cannot access element " + std::to_string(index->value) + ", max index is " + std::to_string(this->value.size()-1));
        }else if (index->value < 0){
            throw IndexError("cannot access index less than 0");
        }
        return this->value[index->value];
    }else{
        throw TypeError("cannot access array index with this type");
    }
};

Boolean * Array::equals(Object * obj){
    throw NotImplementedError("Array equals not implemented yet.");
};

Boolean * Array::notEquals(Object * obj){
    throw NotImplementedError("Array not equals not implemented yet.");
};

// /// ##################################################################
// /// Dictionary definition
// /// ##################################################################

Dictionary::Dictionary(){};

Dictionary::Dictionary(Object * obj){
    if (dynamic_cast<Dictionary*>(obj) != nullptr){
        Dictionary * ptr = dynamic_cast<Dictionary*>(obj);
        this->value = ptr->value;
    }else{
        throw TypeError("cannot cast object to Dictionary");
    }
};

Integer * Dictionary::getSize(){
    return new Integer(size(this->value));
}

Object * Dictionary::builtIn(string attribute){
    throw NotImplementedError("built in attribute not implemented");
}

Object * Dictionary::builtIn(string method_id, Array * params){
    if (method_id == "size"){
        return this->getSize();
    }
    else if (method_id == "pop"){
        return this->pop(params);
    }
    else{
        throw ValueError("'" + method_id + "' not implemented for " + this->getType());
    }
}

Boolean * Dictionary::equals(Object *){
    throw NotImplementedError("dict equals not implemented yet");
};

Boolean * Dictionary::notEquals(Object *){
    throw NotImplementedError("dict not equals not implemented yet");
};

Object * Dictionary::add(Object * obj){
    if (dynamic_cast<Dictionary*>(obj) != nullptr){
        Dictionary * dictUnion = dynamic_cast<Dictionary*>(this->clone());
        Dictionary * rhs = dynamic_cast<Dictionary*>(obj);

        for(map<Object*,Object*>::iterator it = rhs->value.begin(); it != rhs->value.end(); ++it) {
            Object* key = it->first;
            Object* val = it->second;
            dictUnion->value[key] = val;
        }

        return dictUnion;

    }else{
        throw TypeError("can only add a dict with another dict");
    }
};

Object * Dictionary::pop(Array * params){
    if (params->value.size() != 1){
        throw ValueError("Dictionary pop() accepts one argument.");
    }
    
    Object * param = params->value[0];
    Integer * paramIndexInt = dynamic_cast<Integer*>(param);
    String * paramIndexStr = dynamic_cast<String*>(param);
    if (paramIndexInt == nullptr && paramIndexStr == nullptr){
        throw TypeError("Array pop() accepts an integer or string value only.");
    }

    for(map<Object*,Object*>::iterator it = this->value.begin(); it != this->value.end(); ++it) {
        Object* key = it->first;
        Object* val = it->second;

        if (key->toString() == param->toString() && key->getType() == param->getType()){
            this->value.erase(key);
            return val;
        }
    }

    throw IndexError("Key '" + param->toString() + "' not found");
}

Object * Dictionary::subtract(Object *){
    throw NotImplementedError("dict subtract not implemented yet");
};

Boolean * Dictionary::boolOr(Object *){
    throw NotImplementedError("dict boolOR not implemented yet");
};

Boolean * Dictionary::boolAnd(Object *){
    throw NotImplementedError("dict boolAND not implemented yet");
};

Boolean * Dictionary::greaterThan(Object *){
    throw NotImplementedError("dict > not implemented yet");
};

Boolean * Dictionary::greaterThanOrEqual(Object *){
    throw NotImplementedError("dict >= not implemented yet");
};

Boolean * Dictionary::lessThan(Object *){
    throw NotImplementedError("dict < not implemented yet");
};

Boolean * Dictionary::lessThanOrEqual(Object *){
    throw NotImplementedError("dict <= not implemented yet");
};

Object * Dictionary::index(Object * obj){
    for(map<Object*,Object*>::iterator it = this->value.begin(); it != this->value.end(); ++it) {
        Object* key = it->first;
        Object* val = it->second;
        if (key->toString() == obj->toString() && key->getType() == obj->getType()){
            return val;
        }
    }
    throw IndexError("Key '" + obj->toString() + "' not found");
};

Object * Dictionary::clone(){
    return new Dictionary(this);
};

string Dictionary::toString(){
    string dict = "{";
    
    for(map<Object*,Object*>::iterator it = this->value.begin(); it != this->value.end(); ++it) {
        Object* key = it->first;
        Object* val = it->second;

        if (dict.size() > 1) dict += ", ";

        if (dynamic_cast<String*>(key) != nullptr){
            dict += "\"" + key->toString() + "\": ";
        }else{
            dict += key->toString() + ": ";
        }

        if (dynamic_cast<String*>(val) != nullptr){
            dict += "\"" + val->toString() + "\"";
        }else{
            dict += val->toString();
        }
    }

    dict += "}";
    return dict;
};

// /// ##################################################################
// /// None definition
// /// ##################################################################

None::None(){};

Object * None::builtIn(string attribute){
    throw NotImplementedError("built in attribute not implemented");
}

Object * None::builtIn(string method_id, Array * params){
    throw NotImplementedError("built in method not implemented");
}

Boolean * None::equals(Object *){
    throw NotImplementedError("none equals not implemented yet");
};

Boolean * None::notEquals(Object *){
    throw NotImplementedError("none not equals not implemented yet");
};

Boolean * None::boolOr(Object * obj){
    if (dynamic_cast<Boolean *>(obj) != nullptr){
        Boolean * b = dynamic_cast<Boolean *>(obj);
        bool result = false || b->value;
        return new Boolean(result); 
    }else{
        throw TypeError("error with None OR");
    }
};

Boolean * None::boolAnd(Object * obj){
    if (dynamic_cast<Boolean *>(obj) != nullptr){
        Boolean * b = dynamic_cast<Boolean *>(obj);
        bool result = false && b->value;
        return new Boolean(result); 
    }else{
        throw TypeError("error with None AND");
    }
};

Object * None::clone(){
    return new None();
};

string None::toString(){
    return "none";
};

// /// ##################################################################
// /// File definition
// /// ##################################################################

File::File(string filePath){
    this->filePath = filePath;
    this->processFileMode("r");
    this->encoding = "UTF-8";
    this->universalNewLines = true;
    this->open();
}

File::File(string filePath, string fileMode){
    this->filePath = filePath;
    this->processFileMode(fileMode);
    this->encoding = "UTF-8";
    this->universalNewLines = true;
    this->open();
};

File::File(string filePath, string fileMode, bool universalNewLines){
    this->filePath = filePath;
    this->processFileMode(fileMode);
    this->encoding = "UTF-8";
    this->universalNewLines = universalNewLines;
    this->open();
};

void File::open(){
    this->fstreamFile.open(this->filePath, ios::binary | this->mode);
    if (!this->fstreamFile.is_open()){
        throw IOError("unable to open input file: " + this->filePath); 
    }
}

void File::processFileMode(string fileMode){
    this->fileMode = fileMode;
    if (fileMode == "r"){
        this->modeRead = true;
        this->mode = ios::in;
    }
    else if (fileMode == "r+"){
        this->modeRead = true;
        this->modeWrite = true;
        this->mode = ios::in | ios::out;
    }
    else if (fileMode == "a"){
        this->modeAppend = true;
        this->mode = ios::app;
    }
    else if (fileMode == "a+"){
        this->modeAppend = true;
        this->modeRead = true;
        this->mode = ios::app | ios::in;
    }
    else if (fileMode == "w"){
        this->modeWrite = true;
        this->mode = ios::out;
    }
    else if (fileMode == "w+"){
        this->modeWrite = true;
        this->modeRead = true;
        this->mode = ios::out | ios::in;
    }
    else{
        throw ValueError("mode must be 'r', 'r+', 'w', 'w+', 'a', or 'a+', not '" + fileMode + "'");
    }
};

Object * File::builtIn(string attribute){
    if (attribute == "closed"){
        return new Boolean(!this->fstreamFile.is_open());
    }
    else if (attribute == "encoding"){
        return new String(this->encoding);
    }
    else if (attribute == "mode"){
        return new String(this->fileMode);
    }
    else if (attribute == "name"){
        return new String(this->filePath);
    }
    else{
        throw ValueError(this->getType() + " has no member '" + attribute + "'");
    }
}

Object * File::builtIn(string method_id, Array * params){
    if (method_id == "read"){
        return this->read();
    }
    else if (method_id == "readline"){
        return this->readline(params);
    }
    else if (method_id == "readlines"){
        return this->readlines(params);
    }
    else if (method_id == "size"){
        return this->getSize();
    }
    else if (method_id == "write"){
        this->write(params);
    }
    else if (method_id == "writeline"){
        this->writeline(params);
    }
    else if (method_id == "writelines"){
        this->writelines(params);
    }
    else if (method_id == "tell"){
        return this->tell();
    }
    else if (method_id == "flush"){
        this->flush();
    }
    else if (method_id == "seek"){
        this->seek(params);
    }
    else if (method_id == "close"){
        this->close();
    }
    else{
        throw ValueError("method '" + method_id + "' not found in " + this->getType());
    }
}

string File::processNewLines(string data){
    // if universal new lines disabled, return raw data as is
    if (!this->universalNewLines) {
        return data;
    }

    // if universal new lines (the default), convert line endings to '\n'
    string processedData;
    int index = 0;
    while (index < data.size()){
        char c = data[index];
        if (c == '\r' && index + 1 < data.size() && data[index+1] == '\n'){
            processedData += '\n';
            index += 2;
        }
        else if (c == '\r'){
            processedData += '\n';
            index += 1;
        }
        else{
            processedData += c;
            index += 1;
        }
    }
    return processedData;
};

String * File::read(){
    if (!fstreamFile.is_open()) {
        throw ValueError("I/O operation on closed file.");
    }
    if (!this->modeRead){
        throw IOError("File not opened with read permissions.");
    }
    string file_contents { istreambuf_iterator<char>(fstreamFile), istreambuf_iterator<char>() };
    return new String(this->processNewLines(file_contents));
};

Array * File::readlines(Array * params){
    if (!fstreamFile.is_open()) {
        throw ValueError("I/O operation on closed file.");
    }
    if (!this->modeRead){
        throw IOError("File not opened with read permissions.");
    }
    Array * lines = new Array();
    string line;
    while (getline(fstreamFile, line)){
        lines->value.push_back(new String(this->processNewLines(line)));
    }
    fstreamFile.clear();
    return lines;
};

String * File::readline(Array * params){
    if (!fstreamFile.is_open()) {
        throw ValueError("I/O operation on closed file.");
    }
    if (!this->modeRead){
        throw IOError("File not opened with read permissions.");
    }
    string line;
    getline(fstreamFile, line);
    return new String(this->processNewLines(line));
};

void File::write(Array * params){
    if (!fstreamFile.is_open()) {
        throw ValueError("I/O operation on closed file.");
    }
    if (!this->modeWrite){
        throw IOError("File not opened with write permissions.");
    }
    if (params->value.size() != 1){
        throw ValueError("'write' takes exactly 1 argument");
    }
    Object * value = params->value[0];
    if (dynamic_cast<String*>(value) == nullptr){
        throw ValueError("'write' argument must be a string");
    }

    string output = this->processNewLines(value->toString());
    this->fstreamFile << value->toString();
};

void File::writeline(Array * params){
    if (!fstreamFile.is_open()) {
        throw ValueError("I/O operation on closed file.");
    }
        if (!this->modeWrite){
        throw IOError("File not opened with write permissions.");
    }
    if (params->value.size() != 1){
        throw ValueError("'writeline' takes exactly 1 argument");
    }
    Object * value = params->value[0];
    if (dynamic_cast<String*>(value) == nullptr){
        throw ValueError("'writeline' argument must be a string");
    }

    String * line = dynamic_cast<String*>(params->value[0]);
    this->fstreamFile << line->toString() << std::endl;
};

void File::writelines(Array * params){
    if (!fstreamFile.is_open()) {
        throw ValueError("I/O operation on closed file.");
    }
        if (!this->modeWrite){
        throw IOError("File not opened with write permissions.");
    }
    if (params->value.size() != 1){
        throw ValueError("'writelines' takes exactly 1 argument");
    }
    Object * value = params->value[0];
    if (dynamic_cast<Array*>(value) == nullptr){
        throw ValueError("'writelines' argument must be an array");
    }

    Array * lines = dynamic_cast<Array*>(params->value[0]);
    for (int i=0; i<lines->value.size(); i++){
        string output = this->processNewLines(lines->value[i]->toString());
        this->fstreamFile << output << std::endl;
    }
};

Integer * File::tell(){
    return new Integer(int(this->fstreamFile.tellg()));
};

void File::flush(){
    fstreamFile.flush();
};

void File::close(){
    fstreamFile.close();
};

void File::seek(Array * params){
    if (params->getSize()->value != 1 || dynamic_cast<Integer*>(params->value[0]) == nullptr){
        throw ValueError("seek() takes a single integer argument");
    }
    Integer * value = dynamic_cast<Integer*>(params->value[0]);
    this->fstreamFile.seekg(value->value);
    this->fstreamFile.seekp(value->value);
};

Boolean * File::equals(Object *){
    throw NotImplementedError("file equals not implemented yet");
};

Boolean * File::notEquals(Object *){
    throw NotImplementedError("file not equals not implemented yet");
};

Boolean * File::boolOr(Object * obj){
    if (dynamic_cast<Boolean *>(obj) != nullptr){
        Boolean * b = dynamic_cast<Boolean *>(obj);
        bool result = false || b->value;
        return new Boolean(result); 
    }else{
        throw TypeError("error with file OR");
    }
};

Boolean * File::boolAnd(Object * obj){
    if (dynamic_cast<Boolean *>(obj) != nullptr){
        Boolean * b = dynamic_cast<Boolean *>(obj);
        bool result = false && b->value;
        return new Boolean(result); 
    }else{
        throw TypeError("error with file AND");
    }
};

Object * File::clone(){
    return new File(this->filePath, this->fileMode);
};

string File::toString(){
    return "<open file '" + this->filePath + "', mode '" + this->fileMode + "'>";
};

// /// ##################################################################
// /// Socket definition
// /// ##################################################################

Socket::Socket(){};

Boolean * Socket::equals(Object *){
    throw NotImplementedError("none equals not implemented yet");
};

Boolean * Socket::notEquals(Object *){
    throw NotImplementedError("none not equals not implemented yet");
};

Object * Socket::builtIn(string attribute){
    throw NotImplementedError("built in attribute not implemented");
}

Object * Socket::builtIn(string method_id, Array * params){
    throw NotImplementedError("built in method not implemented");
}

Boolean * Socket::boolOr(Object * obj){
    if (dynamic_cast<Boolean *>(obj) != nullptr){
        Boolean * b = dynamic_cast<Boolean *>(obj);
        bool result = false || b->value;
        return new Boolean(result); 
    }else{
        throw TypeError("error with None OR");
    }
};

Boolean * Socket::boolAnd(Object * obj){
    if (dynamic_cast<Boolean *>(obj) != nullptr){
        Boolean * b = dynamic_cast<Boolean *>(obj);
        bool result = false && b->value;
        return new Boolean(result); 
    }else{
        throw TypeError("error with None AND");
    }
};

Object * Socket::clone(){
    return new Socket();
};

string Socket::toString(){
    return "Socket name";
};