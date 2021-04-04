///
/// Brio Lang
/// interpreter.cc
///
/// Tree-Based Interpreter
///

#include <brio_lang/interpreter.h>
#include <brio_lang/visitor.h>
#include <brio_lang/tree.h>
#include <brio_lang/exceptions.h>
#include <brio_lang/object.h>
#include <brio_lang/util.h>
#include <brio_lang/http.h>
#include <brio_lang/parser.h>
#include <brio_lang/lexer.h>
#include <brio_lang/def.h>
#include <brio_lang/ref.h>

#include <string>
#include <iostream>
#include <cmath>


Interpreter::Interpreter(ProgramNode * rootNode, SymbolTable * symTable){
    this->initialize(rootNode, symTable, ".");
};

Interpreter::Interpreter(ProgramNode * rootNode, SymbolTable * symTable, string path){
    this->initialize(rootNode, symTable, path);
};

Interpreter::Interpreter(ProgramNode * rootNode, SymbolTable * symTable, vector<string> args, string path){
    this->initialize(rootNode, symTable, path);
    this->arguments = args;
};

/**
 * Initialize the interpreter by setting pointers to the root node, symbol table,
 * as well as initialize the globals space and set it as current.
 */
void Interpreter::initialize(ProgramNode * rootNode, SymbolTable * symTable, string path){
    root = rootNode;
    table = symTable;
    programPath = path;
    globalScope = &symTable->globals;
    globals = new MemorySpace("globals");
    currentSpace = globals;
};

/**
 * Finds the "main" method and begins execution there.
 */
Object * Interpreter::exec(){
    // invoke import statements
    for (int i=0; i<int(root->children.size()); i++){
        AST * node = root->children[i];
        if (node->getType() == ImportNode::type){
            this->exec(node);
        }
    }

    // invoke class and variable declarations
    for (int i=0; i<int(root->children.size()); i++){
        AST * node = root->children[i];
        if (node->getType() == ClassDeclarationNode::type){
            this->exec(node);
        }
        else if (node->getType() == VarDeclarationNode::type){
            this->exec(node);
        }
    }

    // invoke imported module declarations
    this->execImportedModules(this->root);

    // execute the "main" method
    return this->main();
};

void Interpreter::execImportedModules(AST * node){
    if (node->getType() == ImportedModuleNode::type){
        this->exec(node);
    }
    for (int i=0; i<node->children.size(); i++){
        this->execImportedModules(node->children[i]);
    }
};

/**
 * Visitor dispatch according to AST node type.
 */
Object * Interpreter::exec(AST * node){
    string nodeType = node->getType();

    if (nodeType == ProgramNode::type ){
        this->program(node);
    }
    else if (nodeType == ImportNode::type){
        this->import(node);
    }
    else if (nodeType == ImportedModuleNode::type){
        this->importedModule(node);
    }
    else if (nodeType == BlockNode::type ){
        this->block(node);
    }
    else if (nodeType == VarAssignmentNode::type){
        this->varAssignment(node);
    }
    else if (nodeType == AdditionAssignNode::type){
        this->addAssign(node);
    }
    else if (nodeType == SubtractionAssignNode::type){
        this->subtractAssign(node);
    }
    else if (nodeType == MultiplicationAssignNode::type){
        this->multiplyAssign(node);
    }
    else if (nodeType == DivisionAssignNode::type){
        this->divideAssign(node);
    }
    else if (nodeType == BitOrAssignNode::type){
        this->bitOrAssign(node);
    }
    else if (nodeType == BitAndAssignNode::type){
        this->bitAndAssign(node);
    }
    else if (nodeType == VarDeclarationNode::type){
        this->varDeclaration(node);
    }
    else if (nodeType == MethodCallNode::type){
        return this->methodCall(node);
    }
    else if (nodeType == ClassDeclarationNode::type){
        this->classDeclaration(node);
    }
    else if (nodeType == ClassInstantiationNode::type){
        return this->classInstantiation(node);
    }
    else if (nodeType == AddExpressionNode::type){
        return this->add(node);
    }
    else if (nodeType == SubtractExpressionNode::type){
        return this->subtract(node);
    }
    else if (nodeType == MultiplyExpressionNode::type){
        return this->multiply(node);
    }
    else if (nodeType == DivideExpressionNode::type){
        return this->divide(node);
    }
    else if (nodeType == ParenthesesNode::type){
        return this->parentheses(node);
    }
    else if (nodeType == LiteralIntNode::type){
        return this->literalInt(node);
    }
    else if (nodeType == LiteralStringNode::type){
        return this->literalString(node);
    }
    else if (nodeType == LiteralDecimalNode::type){
        return this->literalDecimal(node);
    }
    else if (nodeType == LiteralBooleanNode::type){
        return this->literalBool(node);
    }
    else if (nodeType == LiteralNoneNode::type){
        return this->literalNone(node);
    }
    else if (nodeType == ReturnNode::type){
        return this->returnStatement(node);
    }
    else if (nodeType == IndexNode::type){
        return this->index(node);
    }
    else if (nodeType == ArrayNode::type){
        return this->array(node);
    }
    else if (nodeType == DictionaryNode::type){
        return this->dictionary(node);
    }
    else if (nodeType == BoolAndNode::type){
        return this->boolAnd(node);
    }
    else if (nodeType == BoolOrNode::type){
        return this->boolOr(node);
    }
    else if (nodeType == BoolEqualsNode::type){
        return this->boolEquals(node);
    }
    else if (nodeType == BoolNotEqualsNode::type){
        return this->boolNotEquals(node);
    }
    else if (nodeType == BoolLessThanNode::type){
        return this->boolLessThan(node);
    }
    else if (nodeType == BoolLessThanOrEqualNode::type){
        return this->boolLessThanOrEqual(node);
    }
    else if (nodeType == BoolGreaterThanNode::type){
        return this->boolGreaterThan(node);
    }
    else if (nodeType == BoolGreaterThanOrEqualNode::type){
        return this->boolGreaterThanOrEqual(node);
    }
    else if (nodeType == BitShiftLeftNode::type){
        return this->bitShiftLeft(node);
    }
    else if (nodeType == BitShiftRightNode::type){
        return this->bitShiftRight(node);
    }
    else if (nodeType == ModulusExpressionNode::type){
        return this->modulus(node);
    }
    else if (nodeType == ExponentExpressionNode::type){
        return this->exponent(node);
    }
    else if (nodeType == BitAndExpressionNode::type){
        return this->bitAnd(node);
    }
    else if (nodeType == BitOrExpressionNode::type){
        return this->bitOr(node);
    }
    else if (nodeType == BitXorExpressionNode::type){
        return this->bitXor(node);
    }
    else if (nodeType == IfStatementNode::type){
        this->ifStatement(node);
    }
    else if (nodeType == TryStatementNode::type){
        this->tryStatement(node);
    }
    else if (nodeType == RaiseStatementNode::type){
        this->raiseStatement(node);
    }
    else if (nodeType == ForStatementNode::type){
        this->forStatement(node);
    }
    else if (nodeType == EachStatementNode::type){
        this->eachStatement(node);
    }
    else if (nodeType == WhileStatementNode::type){
        this->whileStatement(node);
    }
    else if (nodeType == SkipStatementNode::type){
        this->skipStatement(node);
    }
    else if (nodeType == MemberAccessNode::type){
        return this->memberAccess(node);
    }
    else if (nodeType == IdentifierNode::type){
        return this->identifier(node);
    }
    else if (nodeType == MethodDeclarationNode::type){
        // nothing to do
    }
    else{
        throw NotImplementedError("interpreter: unimplemented node: " + nodeType);
    }
    return nullptr;
};

void Interpreter::importedModule(AST * node){
    // add imported module symbol to global space
    ImportedModuleSymbol * import_sym = dynamic_cast<ImportedModuleSymbol*>(node->children[0]->symbol);
    ImportedInstance * import_inst = new ImportedInstance(import_sym);
    currentSpace->put(import_sym->getSymbolName(), import_inst);

    // set current space as import_inst
    MemorySpace * saveSpace = currentSpace;
    currentSpace = import_inst;

    // execute block
    this->exec(node->children[2]);
    currentSpace = saveSpace;
};

/**
 * Returns a relative path to the designated import file created from 
 * the provided node. Node can be type IdentifierNode, or type 
 * MemberAccessNode.
 */
string Interpreter::getImportRelativePath(AST * node){
    if (node->getType() == MemberAccessNode::type){
        string parts;
        for (int i=0; i<node->children.size(); i++){
            parts += node->children[i]->getNodeText();
            if (i+1 < node->children.size()){
                parts += "/";
            }
        }
        return parts + ".brio";
    }
    else if (node->getType() == IdentifierNode::type){
        return node->getNodeText() + ".brio";
    }
    else{
        throw ImportError("invalid module path");
    }
};

/**
 * Returns the path to the program based on context. If this is called within
 * an ImportedModuleNode, the path to the imported module is returned. Otherwise,
 * the path to the main program is returned. 
 */
string Interpreter::getProgramPath(AST * node){
    if (node->hasAncestor(ImportedModuleNode::type)){
        AST * importedModule = node->getAncestor(ImportedModuleNode::type);
        return importedModule->children[1]->getNodeText();
    }
    return this->programPath;
};

/**
 * Finds the target import file using BRIOPATH, and if not found looks at
 * the relative directory to the running brio program. Throws an ImportError 
 * if the file cannot be found.
 */
string Interpreter::findImportFile(AST * node){
    // determine BRIOPATH from environment variable
    if(getenv("BRIOPATH") == nullptr){
        throw ImportError("BRIOPATH environment variable not found");
    }
    string briopath = getenv("BRIOPATH");
    string importpath = this->getImportRelativePath(node);

    // look in BRIOPATH first
    string filename = briopath + "/library/" + importpath;
    if (Util::file_exists(filename)){
        return filename;
    }

    // look in script relative directory next
    string relativepath = this->getProgramPath(node);
    string::size_type pos = relativepath.find_last_of('/');
    relativepath = relativepath.substr(0, pos);
    filename = relativepath + "/" + importpath;
    if (Util::file_exists(filename)){
        return filename;
    }
    
    // import was not found
    throw ImportError("cannot find module '" + importpath + "'");
    return nullptr;
};

void Interpreter::import(AST * node){
    // extract import identifier and token
    AST * targetId = node->children[0];
    Token targetToken = targetId->getToken();
    if (targetId->getType() == MemberAccessNode::type){
        targetToken = targetId->children[size(targetId->children)-1]->getToken();
    }

    // find module, parse it, and create AST
    string filename = this->findImportFile(targetId);
    string code = Util::read_file(filename);
    BrioLexer importLexer(code);
    BrioParser importParser(&importLexer);
    ProgramNode importTree = importParser.program();
    OperatorPrecedenceVisitor opVisitor;
    opVisitor.visit(&importTree);

    // prepare the ImportedModuleNode to introduce to main AST
    ImportedModuleNode * importedModule = new ImportedModuleNode(targetToken);
    IdentifierNode * importedId = new IdentifierNode(targetToken);
    LiteralStringNode * importedPath = new LiteralStringNode(Token(T_LITERAL_STRING, filename, targetToken.getLineNo(), targetToken.getOffset(), targetToken.getIndex()));
    BlockNode * importedBlock = new BlockNode(Token(T_DELIMITER, "{", targetToken.getLineNo(), targetToken.getOffset(), targetToken.getIndex()));
    for (int i=0; i<importTree.children.size(); i++){
        importedBlock->addChild(importTree.children[i]);
    }
    importedModule->addChild(importedId);
    importedModule->addChild(importedPath);
    importedModule->addChild(importedBlock);
    node->replace(importedModule);

    // re-run the def/ref process so new elements are in symbol table and scope
    Def def(this->table);
    def.downUp(this->root);
    Ref ref(this->table);
    ref.downUp(this->root);
    this->exec(importedModule);
};

/** Return scope holding id's value; current method space, imported module space, or globals. */
MemorySpace * Interpreter::getSpaceWithSymbol(AST * node){
    if (stack.size() > 0 && stack.peek()->get(node->getNodeText()) != nullptr){
        return stack.peek();
    }
    
    if (node->hasAncestor(ImportedModuleNode::type)){
        AST * importParent = node->getAncestor(ImportedModuleNode::type);
        ImportedInstance * importSpace = static_cast<ImportedInstance*>(globals->get(importParent->getNodeText()));
        if (importSpace->get(node->getNodeText()) != nullptr){
            return importSpace;
        }
    }

    if (globals->get(node->getNodeText()) != nullptr){ 
        return globals;
    }
    return nullptr;
};

void Interpreter::program(AST * node){
    this->block(node);
};

void Interpreter::block(AST * node){
    for (int i=0; i<node->children.size(); i++){
        AST * stmt = node->children[i];
        this->exec(stmt);
    }
};

/**
 * Temporary function to enable "size()" built-in method.
 * This will go away.
 */
Integer * Interpreter::getSize(AST * node){
    if (int(node->children.size()) != 1){
        throw ValueError("size() takes 1 argument");
    }

    Object * val = this->exec(node->children[0]);
    return val->getSize();
};

/**
 * Temporary function to enable "input()" built-in method.
 * This will go away.
 */
Object * Interpreter::input(AST * node){
    String * strInput = new String();

    if (int(node->children.size()) > 1){
        throw ValueError("input() takes zero or 1 argument");
    }

    if (int(node->children.size()) == 1){
        std::cout << node->children[0]->getNodeText();
    }

    getline(cin, strInput->value);
    return strInput;
};

/**
 * Temporary function to enable "boolean()" built-in method.
 * This will go away.
 */
Boolean * Interpreter::boolean(AST * node){
    Object * result = this->exec(node);
    return new Boolean(result);
};

/**
 * Temporary function to enable "string()" built-in method.
 * This will go away.
 */
String * Interpreter::str(AST * node){
    Object * result = this->exec(node);
    return new String(result);
};

/**
 * Temporary function to enable "integer()" built-in method.
 * This will go away.
 */
Integer * Interpreter::integer(AST * node){
    Object * result = this->exec(node);
    return new Integer(result);
};

/**
 * Temporary function to enable "decimal()" built-in method.
 * This will go away.
 */
Decimal * Interpreter::decimal(AST * node){
    Object * result = this->exec(node);
    return new Decimal(result);
};

/**
 * Built-in function to retrieve an environment variable.
 */
Object * Interpreter::getEnv(AST * node){
    if (node->children.size() == 0){
        return new None();
    }
    if (getenv(node->children[0]->getNodeText().c_str()) != nullptr){
        string value = getenv(node->children[0]->getNodeText().c_str());
        return new String(value);
    }
    return new None();
}

/**
 * Temporary function to enable "print()" built-in method.
 * This will go away.
 */
void Interpreter::print(AST * node){
    Object * result = this->exec(node->children[0]);
    if (result == nullptr) {
        std::cout << std::endl;
        return;
    }

    // if second argument to print function, use as terminating character
    if (node->children.size() == 2){
        std::cout << result->toString() << this->exec(node->children[1])->toString();
    }else{
        std::cout << result->toString() << std::endl;
    }
}

/**
 * Temporary function to enable "range()" built-in method.
 * This will go away.
 */
Object * Interpreter::range(AST * node){
    Object * result = this->exec(node->children[0]);

    if (static_cast<Integer*>(result) == nullptr){
        throw ValueError("range accepts an Integer value only");
    }

    Integer * intResult = static_cast<Integer*>(result);

    Array * rng = new Array();
    for (int i = 0; i < intResult->value; i++){
        rng->value.push_back(new Integer(i));
    }
    return rng;
}

/**
 * Temporary function to enable "arg()" built-in method.
 * This will go away.
 */
String * Interpreter::arg(AST * node){
    Object * obj = this->exec(node);
    if (dynamic_cast<Integer*>(obj) != nullptr){
        Integer * index = dynamic_cast<Integer*>(obj);
        if (index->value > this->arguments.size()-1){
            throw IndexError("provided index is greater than size");
        }else{
            return new String(this->arguments[index->value]);
        }
    }else{
        throw ValueError("arg only accepts an integer index");
    }
}

/**
 * Temporary function to enable "open()" built-in method.
 * This will go away.
 */
Object * Interpreter::open(AST * node){
    if (node->children.size() == 1){
        AST * astFilePath = node->children[0];
        String * filePath = static_cast<String*>(this->exec(astFilePath));
        return new File(filePath->toString());
    }
    else if (node->children.size() == 2){
        AST * astFilePath = node->children[0];
        AST * astFileMode = node->children[1];
        String * filePath = static_cast<String*>(this->exec(astFilePath));
        String * fileMode = static_cast<String*>(this->exec(astFileMode));
        return new File(filePath->toString(), fileMode->toString());
    }
    else if (node->children.size() == 3){
        AST * astFilePath = node->children[0];
        AST * astFileMode = node->children[1];
        AST * astNewLine = node->children[2];
        String * filePath = static_cast<String*>(this->exec(astFilePath));
        String * fileMode = static_cast<String*>(this->exec(astFileMode));
        Boolean * newline = static_cast<Boolean*>(this->exec(astNewLine));
        return new File(filePath->value, fileMode->value, newline->value);
    }
    else{
        throw ValueError("open() takes 1, 2, or 3 arguments");
    }
}

/**
 * Temporary function to enable "close()" built-in method.
 * This will go away.
 */
Object * Interpreter::close(AST * node){
    Object * obj = this->exec(node);
    if (dynamic_cast<File*>(obj) != nullptr){
        File * file = dynamic_cast<File*>(obj);
        file->close();
    }
    else{
        throw NotImplementedError("'close' not implemented for " + obj->getType());
    }
    return nullptr;
}

/**
 * Temporary function to enable "exit()" built-in method.
 * This will go away.
 */
void Interpreter::exit(){
    std::exit(0);
}

/**
 * Temporary function to enable "exit(Integer)" built-in method.
 * This will go away.
 */
void Interpreter::exit(AST * node){
    Object * exitCode = this->exec(node);
    if (static_cast<Integer*>(exitCode) != nullptr){
        Integer * exitCodeInt = static_cast<Integer*>(exitCode);
        std::exit(exitCodeInt->value);
    }else{
        throw ValueError("invalid exit status code");
        std::exit(2);
    }
}

/**
 * Temporary function to enable basic HTTP GETs for early development.
 * This will go away.
 */
Object * Interpreter::httpGet(AST * node){
    if (node->getChildCount() == 1){
        AST * url = node->children[0];
        Object * strUrl = this->exec(url);
        HttpClient client;
        string response = client.get(strUrl->toString());
        return new String(response);
    }else if (node->getChildCount() == 2){
        AST * url = node->children[0];
        AST * header = node->children[1];
        Object * strUrl = this->exec(url);
        Object * strHeader = this->exec(header);
        HttpClient client;
        string response = client.get(
            strUrl->toString(),
            strHeader->toString()    
        );
        return new String(response);
    }
    return nullptr;
}

/**
 * Temporary function to enable basic HTTP POSTs for early development.
 * This will go away.
 */
Object * Interpreter::httpPost(AST * node){
    AST * url = node->children[0];
    AST * user = node->children[1];
    AST * pass = node->children[2];
    AST * fields = node->children[3];

    Object * strUrl = this->exec(url);
    Object * strUser = this->exec(user);
    Object * strPass = this->exec(pass);
    Object * strFields = this->exec(fields);

    HttpClient client;
    string response = client.post(
        strUrl->toString(), strUser->toString(), 
        strPass->toString(), strFields->toString()
    );
    return new String(response);
}

/**
 * Load an object
 */
Object * Interpreter::load(AST * node){
    if (node->getType() == MemberAccessNode::type){
        return this->memberLoad(node);
    }

    MemorySpace * space = this->getSpaceWithSymbol(node);
    if (space != nullptr) return space->get(node->getNodeText());
    throw NameError("no such variable " + node->getNodeText());
    return nullptr;
}

/**
 * Load an object from a class instance or object
 * E.g. @board[0][1]  z["key"].foo  x[0]  y["foo"]  @board.foo.bar
 */
Object * Interpreter::memberLoad(AST * node){
    AST * a = node->children[0];
    Object * obj = this->load(a);

    AST * attributeNode = node->children[node->getChildCount()-1]; 
    string attribute = attributeNode->getNodeText();

    // load subsequent members
    for (int i=1; i<int(node->children.size()); i++){
        AST * memberNode = node->children[i];

        if (memberNode->getType() == IdentifierNode::type){
            ClassInstance * class_inst = static_cast<ClassInstance*>(obj);
            if (class_inst == nullptr){
                throw AttributeError(obj->toString() + " is not a class instance");
                return nullptr;
            }
            if (class_inst->get(memberNode->getNodeText()) == nullptr){
                throw AttributeError(class_inst->toString() + " has no member '" + memberNode->getNodeText() + "'");
                return nullptr;
            }
            obj = class_inst->get(memberNode->getNodeText());
        }
        else if (memberNode->getType() == MemberAccessNode::type){
            obj = obj->index(this->exec(memberNode));
        }
        else if (memberNode->getType() == IndexNode::type){
            obj = obj->index(this->exec(memberNode));
        }
        else{
            throw AttributeError("Invalid member load expression");
        }

        // if not final member, check if built-in type
        // TODO: fix me:  @data = @a[@b]
        // if (attributeNode != memberNode){
        //     if (this->isBuiltInType(obj)) return this->builtInTypeAttribute(obj, attribute);
        // }
    }

    return obj;
};

/**
 * Assign an object to a class instance or object
 * E.g. @board[0][1] = 1  z["key"].foo = 2  x[0] = 3  @board.foo.bar = 4
 */
void Interpreter::memberAssign(AST * lhs, Object * value){
    AST * a = lhs->children[0];
    AST * z = lhs->children[int(lhs->children.size()) - 1];
    Object * obj = this->load(a);
    
    // load subsequent members
    for (int i=1; i<int(lhs->children.size()) - 1; i++){
        AST * member = lhs->children[i];

        if (member->getType() == IdentifierNode::type){
            ClassInstance * class_inst = static_cast<ClassInstance*>(obj);
            if (class_inst == nullptr){
                throw AttributeError(obj->toString() + " is not a class instance");
            }
            if (class_inst->get(member->getNodeText()) == nullptr){
                throw AttributeError(class_inst->toString() + " has no member '" + member->getNodeText() + "'");
            }
            obj = class_inst->get(member->getNodeText());
        }
        else if (member->getType() == MemberAccessNode::type){
            obj = obj->index(this->exec(member));
        }
        else if (member->getType() == IndexNode::type){
            obj = obj->index(this->exec(member));
        }
        else if (member->getType() == LiteralIntNode::type){
            obj = obj->index(this->exec(member));
        }
        else{
            throw AttributeError("Invalid member load expression");
        }
    }

    // assign last member/index value
    if (dynamic_cast<ClassInstance*>(obj) != nullptr){
        ClassInstance * classInstObj = static_cast<ClassInstance*>(obj); 
        classInstObj->put(z->getNodeText(), value);
    }
    else if (dynamic_cast<Array*>(obj) != nullptr){
        Array * arrayObj = dynamic_cast<Array*>(obj);
        Object * lastIndexObject = this->exec(z);

        if (dynamic_cast<Integer*>(lastIndexObject) == nullptr){
            throw IndexError("array index must be integer, not '" + lastIndexObject->toString() + "'");
        }
        
        // make sure index is valid
        int lastIndexInt = dynamic_cast<Integer*>(lastIndexObject)->value;
        if (lastIndexInt < 0) { 
            throw IndexError("array index cannot be less than 0"); 
        }
        else if (lastIndexInt > int(arrayObj->value.size()) - 1){
            throw IndexError("index cannot be larger than size of array");
        }
        
        arrayObj->value[lastIndexInt] = value;
    }
    else if (dynamic_cast<Dictionary*>(obj) != nullptr){
        Dictionary * dictObj = dynamic_cast<Dictionary*>(obj);
        Object * lastIndexObject = this->exec(z);

        // overwrite key if already exists, otherwise add new
        map<Object*, Object *>::iterator it;
        for (it = dictObj->value.begin(); it != dictObj->value.end(); it++ ){
            Object * key = it->first;
            if (key->toString() == lastIndexObject->toString()
                && key->getType() == lastIndexObject->getType()){
                dictObj->value[key] = value;
                return;
            }
        }

        // key not found, insert new value
        dictObj->value[lastIndexObject] = value;
    }
    else{
        throw RuntimeError("invalid member assignment");
    }
};

void Interpreter::varDeclaration(AST * node){
    AST * lhs = node->children[0];
    string id = lhs->getNodeText();

    // if node only has one child, assume 'none' value for rhs
    Object * value = nullptr;
    if (node->getChildCount() > 1){
        AST * rhs = node->children[1];  
        value = this->exec(rhs);
    }else{
        value = new None();
    }

    currentSpace->put(id, value);
};

void Interpreter::varAssignment(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];
    Object * value = this->exec(rhs);

    if (lhs->getType() == MemberAccessNode::type){
        this->memberAssign(lhs, value);
        return;
    }

    MemorySpace * space = this->getSpaceWithSymbol(lhs);
    if (space == nullptr) space = currentSpace;
    space->put(lhs->getNodeText(), value);
};

Object * Interpreter::main(){
    vector<Object*> emptyParams;
    return this->main(emptyParams);
}

Object * Interpreter::main(vector<Object*> params){
    // check for presense of main method
    const string MAIN = "main";
    Symbol * main_method = table->globals.resolve(MAIN);
    if (main_method == NULL){
        throw LookupError("program must contain a 'main' method");
    }

    MethodSymbol * method_sym = static_cast<MethodSymbol*>(main_method);

    // prepare memory space
    if (method_sym == nullptr){
        throw NameError("method '" + MAIN + "' not found");
    }
    MethodSpace * methSpace = new MethodSpace(method_sym);
    MemorySpace * saveSpace = currentSpace;
    currentSpace = methSpace;

    // verify arguments
    AST * methDeclr = method_sym->def_node->parent;
    AST * methParams = methDeclr->children[2];
    if (methParams->getChildCount() > params.size()){
        throw TypeError("too few parameters provided for '" + MAIN + "'");
    }else if (methParams->getChildCount() < params.size()){
        throw TypeError("too many parameters provided for '" + MAIN + "'");
    }

    // define arguments
    for (int i=0; i<methParams->getChildCount(); i++){
        AST * formalName = methParams->children[i];
        Object * providedValue = params[i];

        methSpace->put(formalName->getNodeText(), providedValue);
    }

    // execute method
    Object * result = nullptr;
    BlockNode * methBlock = dynamic_cast<BlockNode*>(method_sym->def_node->getSibling(3));
    this->stack.push(methSpace);
    try{
        this->exec(methBlock);
    }catch (const ReturnValue rv){
        result = rv.value;
    }
    this->stack.pop();
    currentSpace = saveSpace;
    return result;
};

String * Interpreter::traceback(AST * node){
    // TODO: determine file name and module name
    string traceback = "Traceback (most recent call last):\n";

    for (int i=0; i<this->stack.methodSpace.size(); i++){
        MethodSpace * space = this->stack.methodSpace[i];
        string file = "main.brio";
        string line = std::to_string(space->def_symbol->def_node->getLineNo());
        string module = "module";

        traceback += "  File <" + file + ">, line " + line + ", in <" + module + ">\n";
        traceback += "    " + space->getName() + "()\n";
    } 
    
    traceback += this->exec(node)->toString();
    return new String(traceback);
};

/**
 * Invoked when a BuiltInMethod is called.
 */
Object * Interpreter::builtInMethodCall(AST * node){
    string method_id = node->children[0]->getNodeText();
    AST * params = node->children[1];

    if (method_id == "arg"){
        if (params->getChildCount() == 1){
            return this->arg(params->children[0]);
        }
        else{
            throw ValueError("arg expects a single argument");
        }
    }
    else if (method_id == "traceback"){
        if (params->getChildCount() == 1){
            return this->traceback(params->children[0]);
        }
        else{
            throw ValueError("traceback expects a single argument");
        }
    }
    else if (method_id == "boolean"){
        if (params->getChildCount() == 1){
            return this->boolean(params->children[0]);
        }
        else{
            throw ValueError("boolean expects a single argument");
        }
    }
    else if (method_id == "string"){
        if (params->getChildCount() == 1){
            return this->str(params->children[0]);
        }
        else{
            throw ValueError("string expects a single argument");
        }
    }
    else if (method_id == "integer"){
        if (params->getChildCount() == 1){
            return this->integer(params->children[0]);
        }
        else{
            throw ValueError("integer expects a single argument");
        }
    }
    else if (method_id == "decimal"){
        if (params->getChildCount() == 1){
            return this->decimal(params->children[0]);
        }
        else{
            throw ValueError("decimal expects a single argument");
        }
    }
    else if (method_id == "size"){
        return this->getSize(params);
    }
    else if (method_id == "input"){
        return this->input(params);
    }
    else if (method_id == "print"){
        this->print(params);
    }
    else if (method_id == "getEnv"){
        return this->getEnv(params);
    }
    else if (method_id == "range"){
        return this->range(params);
    }
    else if (method_id == "open"){
        return this->open(params);
    }
    else if (method_id == "close"){ 
        if (params->children.size() == 1){
            this->close(params->children[0]);
        }else{
            throw TypeError("'close' takes exactly 1 argument");
        }
    }
    else if (method_id == "exit"){
        if (params->children.size() == 0){
            this->exit();
        }else if (params->children.size() == 1){
            this->exit(params->children[0]);
        }else{
            throw TypeError("'exit' takes exactly 1 argument");
        }
    }
    else if (method_id == "httpGet"){
        return this->httpGet(params);
    }
    else if (method_id == "httpPost"){
        return this->httpPost(params);
    }
    return nullptr;
}

/**
 * Determines if a provided MethodCallNode is for a built-in method.
 */
bool Interpreter::isBuiltInMethodCall(AST * node){
    if (node->children[0]->getType() == IdentifierNode::type){
        string method_id = node->children[0]->getNodeText();
        return (
            this->table->globals.resolve(method_id) != nullptr &&
            this->table->globals.resolve(method_id)->getSymbolType() == "BuiltInMethodSymbol"
        );
    }
    return false;
}

bool Interpreter::isBuiltInType(Object * obj){
    Symbol * sym = this->table->globals.resolve(obj->getType());
    return bool(dynamic_cast<BuiltInTypeSymbol*>(sym) != nullptr);
};

Object * Interpreter::builtInTypeAttribute(Object * obj, string attribute){
    return obj->builtIn(attribute);
};

Object * Interpreter::builtInTypeMethodCall(Object * obj, string method_id, AST * params){
    Array * paramsArray = new Array();

    // populate array with exec'd params
    for (int i=0; i<params->children.size(); i++){
        paramsArray->value.push_back(this->exec(params->children[i]));
    }

    return obj->builtIn(method_id, paramsArray);
};

Object * Interpreter::methodCall(AST * node){
    // determine if built-in method
    if (this->isBuiltInMethodCall(node)){
        return this->builtInMethodCall(node);
    }

    // standard method call handler
    MethodSymbol * method_sym = nullptr;
    ClassInstance * class_inst = nullptr;
    Object * obj = nullptr;

    // determine method identifier name
    AST * firstId = node->children[0];
    string method_id;
    if (firstId->getType() == MemberAccessNode::type){
        method_id = firstId->children[firstId->getChildCount()-1]->getNodeText();
    }else{
        method_id = firstId->getNodeText();
    }

    if (firstId->getType() == MemberAccessNode::type){
        AST * a = firstId->children[0];
        AST * z = firstId->children[firstId->children.size() - 1];

        // load class inst for left-most child (e.g. "a" in "a.b.foo()")
        obj = this->load(a);
        if (this->isBuiltInType(obj)) return this->builtInTypeMethodCall(obj, method_id, node->children[1]);

        // for remaining siblings, load left to right
        for (int i=1; i<firstId->children.size()-1; i++){
            AST * member = firstId->children[i];
            if (member->getType() == IdentifierNode::type){
                obj = dynamic_cast<ClassInstance*>(obj)->get(member->getNodeText());
            }
            else if (member->getType() == IndexNode::type){
                obj = obj->index(this->exec(member));
            }
            else if (member->getType() == MemberAccessNode::type){
                obj = obj->index(this->exec(member));
            }
            else{
                throw RuntimeError("invalid method call member access");
            }

            // check if method call for built-in type
            if (this->isBuiltInType(obj)) return this->builtInTypeMethodCall(obj, method_id, node->children[1]);
        }

        // obj must be a class instance or imported instance
        if (dynamic_cast<ClassInstance*>(obj) == nullptr
           && dynamic_cast<ImportedInstance*>(obj) == nullptr){
            throw RuntimeError("invalid method call, not a class instance");
        }

        if (dynamic_cast<ClassInstance*>(obj) != nullptr){
            class_inst = dynamic_cast<ClassInstance*>(obj);
            // load from super
            if (a->getNodeText() == Literals::SUPER){
                method_sym = static_cast<MethodSymbol*>(class_inst->symbol->superClass->resolveMember(z->getNodeText()));
            }else{
                method_sym = static_cast<MethodSymbol*>(class_inst->symbol->resolveMember(z->getNodeText()));
            }
        }else if (dynamic_cast<ImportedInstance*>(obj) != nullptr){
            ImportedInstance * import_inst = dynamic_cast<ImportedInstance*>(obj);
            method_sym = static_cast<MethodSymbol*>(import_inst->symbol->resolveMember(z->getNodeText()));
        }
        else{
            throw RuntimeError("invalid member access");
        }
    }else{
        method_sym = static_cast<MethodSymbol*>(firstId->scope->resolve(firstId->getNodeText()));
    }

    // prepare memory space
    if (method_sym == nullptr){
        throw NameError("method '" + method_id + "' not found in " + this->currentSpace->toString());
    }
    MethodSpace * methSpace = new MethodSpace(method_sym);
    MemorySpace * saveSpace = currentSpace;
    currentSpace = methSpace;

    // add class instance and super to method space
    if (class_inst != nullptr){
        methSpace->put(Literals::THIS, class_inst);

        if(class_inst->symbol->superClass != nullptr){
            methSpace->put(Literals::SUPER, class_inst);
        }
    }

    // verify arguments
    AST * methDeclr = method_sym->def_node->parent;
    AST * methParams = methDeclr->children[2];
    AST * providedParams = node->children[1];
    if (methParams->getChildCount() > providedParams->getChildCount()){
        throw TypeError("too few parameters provided for '" + method_id + "'");
    }else if (methParams->getChildCount() < providedParams->getChildCount()){
        throw TypeError("too many parameters provided for '" + method_id + "'");
    }

    // define arguments
    for (int i=0; i<methParams->getChildCount(); i++){
        AST * formalName = methParams->children[i];
        AST * providedValue = providedParams->children[i];
        Object * providedExec = this->exec(providedValue);

        methSpace->put(formalName->getNodeText(), providedExec);
    }

    // execute method
    Object * result = nullptr;
    BlockNode * methBlock = dynamic_cast<BlockNode*>(method_sym->def_node->getSibling(3));
    this->stack.push(methSpace);
    try{
        this->exec(methBlock);
    }catch (const ReturnValue rv){
        result = rv.value;
    }
    this->stack.pop();
    currentSpace = saveSpace;
    return result;
};

Object * Interpreter::index(AST * node){
    return this->exec(node->children[0]);
}

void Interpreter::classDeclaration(AST * node){
    // add class symbol to global space
    ClassSymbol * class_sym = dynamic_cast<ClassSymbol*>(node->children[0]->symbol);
    ClassInstance * class_inst = new ClassInstance(class_sym);
    currentSpace->put(class_sym->getSymbolName(), class_inst);

    // set current space as class_inst
    MemorySpace * saveSpace = currentSpace;
    currentSpace = class_inst;

    // execute class block
    this->exec(node->children[3]);
    currentSpace = saveSpace;
};

ClassInstance * Interpreter::classInstantiation(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];
    string className;
    ClassSymbol * class_sym;
    ClassInstance * class_instance;
    Object * obj = nullptr;

    if (lhs->getType() == MemberAccessNode::type){
        className = lhs->children[int(lhs->children.size()) - 1]->getNodeText();
    }
    else if (lhs->getType() == IdentifierNode::type){
        className = lhs->getNodeText();
    }
    else{
        throw RuntimeError("cannot initiatize class for " + lhs->getType());
    }

    // TODO: consolidate with methodCall logic
    if (lhs->getType() == MemberAccessNode::type){
        AST * a = lhs->children[0];
        AST * z = lhs->children[lhs->children.size() - 1];
        obj = this->load(a);

        // for remaining siblings, load left to right
        for (int i=1; i<lhs->children.size()-1; i++){
            AST * member = lhs->children[i];
            if (member->getType() == IdentifierNode::type){
                obj = dynamic_cast<ClassInstance*>(obj)->get(member->getNodeText());
            }
            else if (member->getType() == IndexNode::type){
                obj = obj->index(this->exec(member));
            }
            else if (member->getType() == MemberAccessNode::type){
                obj = obj->index(this->exec(member));
            }
            else{
                throw RuntimeError("invalid method call member access");
            }
        }

        // obj must be imported instance
        if (dynamic_cast<ImportedInstance*>(obj) != nullptr){
            ImportedInstance * import_inst = dynamic_cast<ImportedInstance*>(obj);
            class_sym = static_cast<ClassSymbol*>(import_inst->symbol->resolveMember(z->getNodeText()));
            class_instance = static_cast<ClassInstance*>(import_inst->get(className)->clone());
        }
        else{
            throw RuntimeError("invalid class instatiation access");
        }
    }else{
        class_sym = static_cast<ClassSymbol*>(lhs->scope->resolve(className));
        if (class_sym != nullptr){
            class_instance = static_cast<ClassInstance*>(this->load(lhs)->clone());
        }
    }

    if (class_sym == nullptr){
        throw RuntimeError("invalid ClassSymbol '" + className + "'");
    }
 
    // look for init method
    MethodSymbol * initSymbol = static_cast<MethodSymbol*>(class_instance->symbol->resolveMember("init"));

    // TODO: consolidate following code with methodCall
    if (initSymbol != nullptr){

        // verify arguments
        AST * methDeclr = initSymbol->def_node->parent;
        AST * methParams = methDeclr->children[2];
        AST * providedParams = node->children[1];
        if (methParams->getChildCount() > providedParams->getChildCount()){
            throw TypeError("too few parameters provided for 'init'");
        }else if (methParams->getChildCount() < providedParams->getChildCount()){
            throw TypeError("too many parameters provided for 'init");
        }

        // prepare memory space
        MethodSpace * methSpace = new MethodSpace(static_cast<MethodSymbol*>(initSymbol));
        MemorySpace * saveSpace = currentSpace;
        currentSpace = methSpace;

        methSpace->put(Literals::THIS, class_instance);
        if (class_instance->symbol->superClass != nullptr){
            methSpace->put(Literals::SUPER, class_instance);
        }

        // define arguments
        for (int i=0; i<methParams->getChildCount(); i++){
            AST * formalName = methParams->children[i];
            AST * providedValue = providedParams->children[i];
            Object * providedExec = this->exec(providedValue);

            methSpace->put(formalName->getNodeText(), providedExec);
        }

        // execute method
        Object * result = nullptr;
        BlockNode * methBlock = dynamic_cast<BlockNode*>(initSymbol->def_node->getSibling(3));
        this->stack.push(methSpace);
        try{
            this->exec(methBlock);
        }catch (const ReturnValue rv){
            result = rv.value;
        }
        this->stack.pop();
        currentSpace = saveSpace;
    }

    return class_instance;
};

Object * Interpreter::identifier(AST * node){
    return this->load(node);
};

Object * Interpreter::memberAccess(AST * node){
    return this->load(node);
};

Object * Interpreter::add(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return value1->add(value2);
};

Object * Interpreter::subtract(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return value1->subtract(value2);
};

Object * Interpreter::multiply(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return value1->multiply(value2);
};

Object * Interpreter::divide(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    if (static_cast<Integer*>(value2) != nullptr){
        Integer * i = static_cast<Integer*>(value2);
        if (i->value == 0){
            throw ZeroDivisionError("division by zero", rhs->getLineNo());
        }
    }
    else if (static_cast<Decimal*>(value2) != nullptr){
        Decimal * i = static_cast<Decimal*>(value2);
        if (i->value == 0.0){
            throw ZeroDivisionError("decimal division by zero", rhs->getLineNo());
        }
    }

    return value1->divide(value2);
};

void Interpreter::addAssign(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];
    Object * value = this->exec(rhs);

    if (lhs->getType() == MemberAccessNode::type){
        Object * curValue = this->memberLoad(lhs);
        this->memberAssign(lhs, curValue->add(value));
        return;
    }

    MemorySpace * space = this->getSpaceWithSymbol(lhs);
    if (space == nullptr) space = currentSpace;
    
    Object * curValue = space->get(lhs->getNodeText());
    space->put(lhs->getNodeText(), curValue->add(value));
};

void Interpreter::subtractAssign(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];
    Object * value = this->exec(rhs);

    if (lhs->getType() == MemberAccessNode::type){
        Object * curValue = this->memberLoad(lhs);
        this->memberAssign(lhs, curValue->subtract(value));
        return;
    }

    MemorySpace * space = this->getSpaceWithSymbol(lhs);
    if (space == nullptr) space = currentSpace;
    
    Object * curValue = space->get(lhs->getNodeText());
    space->put(lhs->getNodeText(), curValue->subtract(value));
};

void Interpreter::multiplyAssign(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];
    Object * value = this->exec(rhs);

    if (lhs->getType() == MemberAccessNode::type){
        Object * curValue = this->memberLoad(lhs);
        this->memberAssign(lhs, curValue->multiply(value));
        return;
    }

    MemorySpace * space = this->getSpaceWithSymbol(lhs);
    if (space == nullptr) space = currentSpace;
    
    Object * curValue = space->get(lhs->getNodeText());
    space->put(lhs->getNodeText(), curValue->multiply(value));
};

void Interpreter::divideAssign(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];
    Object * value = this->exec(rhs);

    if (lhs->getType() == MemberAccessNode::type){
        Object * curValue = this->memberLoad(lhs);
        this->memberAssign(lhs, curValue->divide(value));
        return;
    }

    MemorySpace * space = this->getSpaceWithSymbol(lhs);
    if (space == nullptr) space = currentSpace;
    
    Object * curValue = space->get(lhs->getNodeText());
    space->put(lhs->getNodeText(), curValue->divide(value));
};

Object * Interpreter::parentheses(AST * node){
    if (size(node->children) != 1){
        throw RuntimeError("parentheses must have one child node");
    }
    AST * onlyChild = node->children[0];
    Object * value = this->exec(onlyChild);
    return value;
};

Integer * Interpreter::literalInt(AST * node){
    return new Integer(node->getNodeText());
};

String * Interpreter::literalString(AST * node){
    string data = Util::escape_raw_string(node->getNodeText());
    return new String(data);
};

Decimal * Interpreter::literalDecimal(AST * node){
    return new Decimal(node->getNodeText());
};

Boolean * Interpreter::literalBool(AST * node){
    return new Boolean(node->getNodeText());
};

None * Interpreter::literalNone(AST * node){
    // return new None(node->getNodeText());
    return new None();
};

Object * Interpreter::returnStatement(AST * node){
    this->sharedReturnValue.value = this->exec(node->children[0]);
    throw this->sharedReturnValue;
};

Dictionary * Interpreter::dictionary(AST * node){
    Dictionary * dict = new Dictionary();

    // initialize dict value members
    for(int i=0; i<node->children.size(); i++){
        AST * kvPair = node->children[i];
        AST * key = kvPair->children[0];
        AST * value = kvPair->children[1];

        dict->value[this->exec(key)] = this->exec(value);
    }
    
    return dict;
};

Array * Interpreter::array(AST * node){
    vector<Object*> elements;

    for (int i=0; i<node->getChildCount(); i++){
        AST * arrayElement = node->children[i];
        elements.push_back(this->exec(arrayElement));
    };

    return new Array(elements);
};

Boolean * Interpreter::boolAnd(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return dynamic_cast<Boolean *>(value1->boolAnd(value2));
};

Boolean * Interpreter::boolOr(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return dynamic_cast<Boolean *>(value1->boolOr(value2));
};

Boolean * Interpreter::boolEquals(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return dynamic_cast<Boolean *>(value1->equals(value2));
};

Boolean * Interpreter::boolNotEquals(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return dynamic_cast<Boolean *>(value1->notEquals(value2));
};

Boolean * Interpreter::boolLessThan(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return dynamic_cast<Boolean*>(value1->lessThan(value2));
};

Boolean * Interpreter::boolLessThanOrEqual(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return dynamic_cast<Boolean *>(value1->lessThanOrEqual(value2));
};

Boolean * Interpreter::boolGreaterThan(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return dynamic_cast<Boolean*>(value1->greaterThan(value2));
};

Boolean * Interpreter::boolGreaterThanOrEqual(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return dynamic_cast<Boolean *>(value1->greaterThanOrEqual(value2));
};

Integer * Interpreter::bitShiftLeft(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return dynamic_cast<Integer*>(value1->bitShiftLeft(value2));
};

Integer * Interpreter::bitShiftRight(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return dynamic_cast<Integer*>(value1->bitShiftRight(value2));
};

Object * Interpreter::modulus(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return value1->modulus(value2);
};

Object * Interpreter::exponent(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return value1->exponent(value2);
};

Integer * Interpreter::bitAnd(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return dynamic_cast<Integer*>(value1->bitAnd(value2));
};

Integer * Interpreter::bitOr(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return dynamic_cast<Integer*>(value1->bitOr(value2));
};

void Interpreter::bitOrAssign(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];
    Object * value = this->exec(rhs);

    if (lhs->getType() == MemberAccessNode::type){
        Object * curValue = this->memberLoad(lhs);
        this->memberAssign(lhs, curValue->bitOr(value));
        return;
    }

    MemorySpace * space = this->getSpaceWithSymbol(lhs);
    if (space == nullptr) space = currentSpace;
    
    Object * curValue = space->get(lhs->getNodeText());
    space->put(lhs->getNodeText(), curValue->bitOr(value));
};

void Interpreter::bitAndAssign(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];
    Object * value = this->exec(rhs);

    if (lhs->getType() == MemberAccessNode::type){
        Object * curValue = this->memberLoad(lhs);
        this->memberAssign(lhs, curValue->bitAnd(value));
        return;
    }

    MemorySpace * space = this->getSpaceWithSymbol(lhs);
    if (space == nullptr) space = currentSpace;
    
    Object * curValue = space->get(lhs->getNodeText());
    space->put(lhs->getNodeText(), curValue->bitAnd(value));
};

Integer * Interpreter::bitXor(AST * node){
    AST * lhs = node->children[0];
    AST * rhs = node->children[1];  

    Object * value1 = this->exec(lhs);
    Object * value2 = this->exec(rhs);

    return dynamic_cast<Integer*>(value1->bitXor(value2));
};

void Interpreter::tryStatement(AST * node){
    AST * tryBlock = node->children[0];
    try{
        this->exec(tryBlock);
    }
    catch(const AST * raiseNode){
        string exceptionId = raiseNode->children[0]->getNodeText();
        string exceptionMessage;
        if (raiseNode->children.size() == 2){
            if (raiseNode->children[1]->getChildCount() == 1){
                AST * excMsg = raiseNode->children[1]->children[0];
                exceptionMessage = this->exec(excMsg)->toString();
            }
        }
        this->catchStatement(node, exceptionId, exceptionMessage, nullptr);
    }
    catch(TimeoutError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(FileNotFoundError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(FileExistsError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(ZeroDivisionError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(ValueError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(TypeError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(SystemError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(RuntimeError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(RecursionError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(OverflowError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(OSError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(NotImplementedError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(NameError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(MemoryError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(KeyboardInterrupt e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(KeyError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(IndexError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(ModuleNotFoundError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(ImportError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(EOFError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(AttributeError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(LookupError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(IOError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(BufferError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(ArithmeticError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(SyntaxError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(MismatchedTokenException e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(RecognitionException e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(ParserError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(LexerError e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(Exception e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
    catch(BaseException e){
        this->catchStatement(node, e.getType(), e.what(), &e);
    }
};

/**
 * Finds the first generic catch block based on absense of paramslist
 */
AST * Interpreter::getGenericCatch(AST * node){
    for(int i = 1; i<node->children.size(); i++){
        AST * catchItem = node->children[i];
        if (catchItem->children.size() == 1){
            return catchItem;
        }
    }
};

void Interpreter::catchStatement(AST * node, string exceptionId, string exceptionMessage, void * exception){    
    AST * catchNode = nullptr;
    for(int i = 1; i<node->children.size(); i++){
        AST * catchItem = node->children[i];

        // skip if no params list, it's a generic catch block
        if (catchItem->children[0]->getType() != ParamsListNode::type){
            continue; 
        }

        // review parameters to determine if it's the appropriate catch block
        AST * paramsList = catchItem->children[0];
        for (int z = 0; z < paramsList->children.size(); z++){
            AST * paramItem = paramsList->children[z];

            // if 'as' statement
            if (paramItem->getType() == AsNode::type){
                AST * lhs = paramItem->children[0];
                AST * rhs = paramItem->children[1];
            
                if (this->isClassOrInheritsFrom(exceptionId, paramItem->getNodeText())){
                    catchNode = catchItem;
                    string catchNodeId = lhs->getNodeText();
                    string catchNodeAsId = rhs->getNodeText();
                    Symbol * catchNodeIdSym = lhs->scope->resolve(catchNodeId);

                    if (dynamic_cast<BuiltInTypeSymbol*>(catchNodeIdSym) != nullptr){
                        if (exception != nullptr){
                            // cast built-in exception
                            this->currentSpace->put(catchNodeAsId, ExceptionUtil::castException(exceptionId, exception));
                        }else{
                            // create built-in exception
                            this->currentSpace->put(catchNodeAsId, ExceptionUtil::createException(exceptionId, exceptionMessage));
                        }
                    }
                    else if (dynamic_cast<ClassSymbol*>(catchNodeIdSym) != nullptr){
                        ClassException * classExc = new ClassException(catchNodeId, exceptionMessage);
                        this->currentSpace->put(catchNodeAsId, classExc);
                    }
                    else{
                        throw RuntimeError("invalid member in 'as' statement");
                    }
                }
            }
            else{
                // if identifier
                if (this->isClassOrInheritsFrom(exceptionId, paramItem->getNodeText())){
                    catchNode = catchItem;
                }
            }
        }
    }

    // if specific catchnode was found, call it
    if (catchNode != nullptr){
        this->block(catchNode->children[1]);
        return;
    }

    // if generic catch was found, call it, else throw exception
    AST * genericCatch = this->getGenericCatch(node);
    if (genericCatch != nullptr){
        this->block(genericCatch->children[0]);
    }
    else throw Exception();
};

bool Interpreter::isClassOrInheritsFrom(string className, string match){
    Symbol * sym = this->table->globals.resolve(className);
    if (sym == nullptr) return false;

    ClassSymbol * cSym = dynamic_cast<ClassSymbol*>(sym);
    if (cSym == nullptr) return false;
    
    if (match == cSym->getSymbolName()) return true;
    
    // lookup chain of parent classes for match
    ClassSymbol * pSym = cSym->superClass;
    while (pSym != nullptr){
        if (match == pSym->getSymbolName()){
            return true;
        }else{
            pSym = pSym->superClass;
        }
    }
    return false;
}

void Interpreter::raiseStatement(AST * node){
    throw node;
};

void Interpreter::ifStatement(AST * node){
    for (int i=0; i<node->children.size(); i++){
        AST * ifCondBlockNode = node->children[i];

        if (ifCondBlockNode->children.size() == 2) {
            // if or elseif (with condition and block)
            AST * cond = ifCondBlockNode->children[0];
            AST * block = ifCondBlockNode->children[1];

            if (this->exec(cond)->toString() == Literals::TRUE){
                this->exec(block);
                break;
            }
        }
        else{
            // else block (with no condition)
            AST * block = ifCondBlockNode->children[0];
            this->exec(block);
        }
    }
};

void Interpreter::whileStatement(AST * node){
    AST * lhs = node->children[0];  // bool condition
    AST * rhs = node->children[1];  // block of events

    while (this->exec(lhs)->toString() == Literals::TRUE){
        try{
            this->block(rhs);
        }
        catch(const BreakInterrupt){
            break;
        }
    }
};

void Interpreter::skipStatement(AST * node){
    if (node->hasAncestor(WhileStatementNode::type) || 
        node->hasAncestor(ForStatementNode::type)){
            throw BreakInterrupt();
    }
    else{
        throw SyntaxError("syntax error: 'skip' not properly in loop");
    }
};

void Interpreter::forStatement(AST * node){
    AST * varDeclaration = node->children[0];
    AST * condition = node->children[1];
    AST * increment = node->children[2];
    AST * block = node->children[3];

    for(this->exec(varDeclaration); this->exec(condition)->toString() == Literals::TRUE; this->exec(increment)){
        try{
            this->block(block);
        }
        catch(const BreakInterrupt){
            break;
        }
    }
};

void Interpreter::eachStatement(AST * node){
    AST * varDeclaration = node->children[0];
    AST * iterable = node->children[1];
    AST * block = node->children[2];

    string varId = varDeclaration->children[0]->getNodeText();
    this->exec(varDeclaration);
    
    // iterate through each element
    Object * iterObj = this->exec(iterable);

    if (dynamic_cast<Dictionary*>(iterObj) != nullptr){
        // string index
        Dictionary * iterDict = dynamic_cast<Dictionary*>(iterObj);
        for(map<Object*,Object*>::iterator it = iterDict->value.begin(); it != iterDict->value.end(); ++it) {
            Object * key = it->first;

            currentSpace->put(varId, key);
            
            try{
                this->block(block);
            }
            catch(const BreakInterrupt){
                break;
            }
        }
    }else{
        // integer index
        for(int i = 0; i < iterObj->getSize()->value; i++){
            Object * element = iterObj->index(new Integer(i));

            currentSpace->put(varId, element);
            
            try{
                this->block(block);
            }
            catch(const BreakInterrupt){
                break;
            }
        }
    }
};

MethodSpace::MethodSpace(MethodSymbol * symbol) : 
    MemorySpace(symbol->getSymbolName()) { 
        def_symbol = symbol;
    };

Stack::Stack(){};

/** Returns the MemorySpace on the top of the stack. */
MemorySpace * Stack::peek(){
    if (methodSpace.size() == 0){
        return nullptr;
    }
    return methodSpace[methodSpace.size()-1];
};

int Stack::size(){
    return methodSpace.size();
};

void Stack::push(MethodSpace * space){
    methodSpace.push_back(space);
};

void Stack::pop(){
    methodSpace.pop_back();
};

MemorySpace::MemorySpace(string space_name){
    name = space_name;
};

Object * MemorySpace::get(string id){
    return members[id];
};

void MemorySpace::put(string id, Object * value){
    members[id] = value;
};

string MemorySpace::toString(){
    string output = "<MemorySpace: " + this->name + ">\n";
    map<string, Object *>::iterator it;
    for (it = members.begin(); it != members.end(); it++ )
    {
        output += it->first + " - " + it->second->toString() + "\n";
    }
    return output;
};

string MemorySpace::getName(){
    return this->name;
}

ReturnValue::ReturnValue(){};

BreakInterrupt::BreakInterrupt(){};

ClassInstance::ClassInstance(ClassSymbol * sym) : MemorySpace(sym->getScopeName() + " instance"){
    this->symbol = sym;
};

string ClassInstance::toString(){
    return "ClassInstance <" + this->symbol->getSymbolName() + ">";
};

Object * ClassInstance::clone(){
    ClassInstance * copy = new ClassInstance(this->symbol);
    
    // copy class members
    map<string, Object *>::iterator it;
    for (it = members.begin(); it != members.end(); it++){
        copy->members[it->first] = it->second;
    }

    return copy;
}

ImportedInstance::ImportedInstance(ImportedModuleSymbol * sym) : MemorySpace(sym->getScopeName() + " instance"){
    this->symbol = sym;
};

string ImportedInstance::toString(){
    return "ImportInstance <" + this->symbol->getSymbolName() + ">";
};

Object * ImportedInstance::clone(){
    return new ImportedInstance(this->symbol);
}