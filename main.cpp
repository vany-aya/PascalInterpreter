#include <cassert>
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <map>
#include <tuple>
#include <stdexcept>
#include <string>
#include <memory>
#include <stack>

struct Lexem{
    enum LexemType {
        LexemDelimetr,
        LexemBoleanConst, // логическая константа
        LexemIntegerConst, // целочисленная константа
        LexemStringConst, // строковая константа
        LexemIdentifier,
        Lexeminteger,
        Lexemstring,
        Lexembool,
        Lexemand,
        Lexembegin,
        Lexemdo,
        Lexemelse,
        Lexemend,
        Lexemfor,
        Lexemif,
        Lexemor,
        Lexemprogram,
        Lexemthen,
        Lexemto,
        Lexemuntil,
        Lexemvar,
        Lexemwhile,
        LexemSemicolon, // разделитель точка с запятой
        LexemColon, // разделитель двоеточие
        LexemAssign,
        LexemAdd,
        LexemSub,
        LexemMul,
        LexemDiv,
        LexemComma,
        LexemEquality,
        LexemLeftParanthesis,
        LexemRightParanthesis,
        LexemLeftSquare,
        LexemRightSquare,
        LexemPoint,
        LexemLess,
        LexemGreater,
        LexemPrint,
        LexemRead,
    };
    LexemType type;
    bool boolean_value;
    std::string value;
    int line;
    Lexem(LexemType type_):type(type_){};
    Lexem(){};
};

class Scanner{
    static const std::unordered_map <std::string,Lexem::LexemType> Keywords_Delimetrs;
    std::vector<Lexem> Programm;
    std::vector<std::string> Programm1;
    std::string tmp;
    std::ifstream file;
    int linecounter;
    int poscouner;
    using State = bool(Scanner::*)(char c);
    State state;
    bool Start(char c);
    bool Delimetr_assign(char c);
    bool ConstString(char c);
    bool ConstNumber(char c);
    bool Idenеtifier_KeyWord_ConstBool(char c);
    bool IsValid(char c)const;
    bool IsDelimetr(char c)const;
    bool IsAlpha(char c)const;
    bool IsDigit(char c)const;
    void TakeLexem();
    Lexem::LexemType DefineType(std::string& word)const;
public:
    explicit  Scanner(const char* filename);
    void ScanProgram(const char* filename);
    void Print()const;
    int size()const;
    const Lexem& GetLexem(int lexem_index)const;
};

bool Scanner::IsAlpha(char c) const
{
    return (((c >= 'a') && (c <='z')) || ((c >= 'A') && (c <='Z')));
}

bool Scanner::IsDigit(char c) const
{
    return ((c >= '0') && (c <= '9'));
}

Lexem::LexemType Scanner::DefineType(std::string& word) const
{
    auto tmp = Keywords_Delimetrs.find(word);
    if(tmp != Keywords_Delimetrs.end())
        return tmp->second;
    if((word == "true") || (word == "false"))
        return Lexem::LexemBoleanConst;
    return Lexem::LexemIdentifier;
}

const std::unordered_map <std::string,Lexem::LexemType> Scanner::Keywords_Delimetrs{
        {"print",Lexem::LexemPrint},
        {"read",Lexem::LexemRead},
        {"integer",Lexem::Lexeminteger},
        {"string",Lexem::Lexemstring},
        {"bool",Lexem::Lexembool},
        {"and",Lexem::Lexemand},
        {"begin",Lexem::Lexembegin},
        {"do",Lexem::Lexemdo},
        {"else",Lexem::Lexemelse},
        {"end",Lexem::Lexemend},
        {"for",Lexem::Lexemfor},
        {"if",Lexem::Lexemif},
        {"or",Lexem::Lexemor},
        {"program",Lexem::Lexemprogram},
        {"then",Lexem::Lexemthen},
        {"to",Lexem::Lexemto},
        {"var",Lexem::Lexemvar},
        {"while",Lexem::Lexemwhile},
        {"+",Lexem::LexemAdd},
        {"-",Lexem::LexemSub},
        {"*",Lexem::LexemMul},
        {"/",Lexem::LexemDiv},
        {",",Lexem::LexemComma},
        {":",Lexem::LexemColon},
        {";",Lexem::LexemSemicolon},
        {"=",Lexem::LexemEquality},
        {"(",Lexem::LexemLeftParanthesis},
        {")",Lexem::LexemRightParanthesis},
        {".",Lexem::LexemPoint},
        {"<",Lexem::LexemLess},
        {">",Lexem::LexemGreater},
        {":=",Lexem::LexemAssign},
        {"[",Lexem::LexemLeftSquare},
        {"]",Lexem::LexemRightSquare},
};

Scanner::Scanner(const char* filename)
{
    linecounter = 1;
    poscouner = 1;
    state = nullptr;
    ScanProgram(filename);

}

int Scanner::size() const
{
    return Programm.size();
}

const Lexem& Scanner::GetLexem(int lexem_index) const
{
    return Programm[lexem_index];
}

void Scanner::Print() const
{
    for(int i = 0;i < Programm1.size();i++)
    {
        std:: cout << Programm[i].type << "   " << Programm[i].value << "\n";
    }
}
void Scanner::TakeLexem()
{
    Lexem tmp1;
    if(state == &Scanner::ConstNumber)
        tmp1.type = Lexem::LexemIntegerConst;
    else if(state == &Scanner::ConstString)
        tmp1.type = Lexem::LexemStringConst;
    else
        tmp1.type = DefineType(tmp);
    tmp1.value = tmp;
    tmp1.line = linecounter;
    Programm.push_back(tmp1);
    Programm1.push_back(tmp);
    tmp.clear();
}

bool Scanner::IsDelimetr(char c) const
{
    std::string tmp;
    tmp.push_back(c);
    return( Keywords_Delimetrs.find(tmp) != Keywords_Delimetrs.end());
}

bool Scanner::IsValid(char c)const
{
    return (((c >= 32) && (c <=126)) || (c == 10) || (c == 13) || (c == 9));
}

bool Scanner::Start(char c)
{
    if(IsDelimetr(c)){
        if(c!=':'){
            state = &Scanner::Start;
            TakeLexem();
        }
        else if(c ==':')
        {state = &Scanner::Delimetr_assign;}
        return true;
    }
    if(IsAlpha(c) || (c == '_')){
        state = &Scanner::Idenеtifier_KeyWord_ConstBool;
        return true;
    }
    if(c == '\''){
        state = &Scanner::ConstString;
        return true;
    }
    if(IsDigit(c)){
        state = &Scanner::ConstNumber;
        return true;
    }
    if((c == 10) || (c == 13) || (c == 9) || (c == ' ')){
        if(c == '\n') {
            poscouner = 1;
            linecounter++;
        }
        tmp.clear();
        state = &Scanner::Start;
        return true;
    }
    else return false;
}

bool Scanner::Delimetr_assign(char c)
{
    if(c != '='){
        tmp.pop_back();
        TakeLexem();
        tmp.push_back(c);
        state = &Scanner::Start;
        return (this->*state)(c);
    } else{
        TakeLexem();
        state = &Scanner::Start;
        return true;
    }
}
bool Scanner::Idenеtifier_KeyWord_ConstBool(char c)
{
    if( IsAlpha(c) || (c == '_') || IsDigit(c) ){
        state = &Scanner::Idenеtifier_KeyWord_ConstBool;
        return true;
    }
    else if((c == 10) || (c == 13) || (c == 9) || (c == ' ') || (IsDelimetr(c))){
        tmp.pop_back();
        TakeLexem();
        tmp.push_back(c);
        state = &Scanner::Start;
        return (this->*state)(c);
    } else
        return false;
}
////////////////////////
bool Scanner::ConstNumber(char c)
{
    if(IsDigit(c)){
        state = &Scanner::ConstNumber;
        return true;
    }
    else if((c == 10) || (c == 13) || (c == 9) || (c == ' ') || (IsDelimetr(c)) || file.eof()){
        tmp.pop_back();
        TakeLexem();
        tmp.push_back(c);
        state = &Scanner::Start;
        return (this->*state)(c);
    }
    else
        return false;
}

bool Scanner::ConstString(char c)
{
    if(c == '\''){
        tmp.erase(tmp.begin());
        tmp.erase((tmp.end() - 1));
        TakeLexem();
        state = &Scanner::Start;
        return true;
    }
    else{
        return IsValid(c);
    }
}

void Scanner::ScanProgram(const char* filename)
{
    char c;
    state = &Scanner::Start;
    file.open(filename);
    while(file.get(c))
    {
        ++poscouner;
        tmp.push_back(c);
        bool r = (this->*state)(c);
        if(!r){
          throw std::runtime_error("error in line: " + std::to_string(linecounter) + ", position: " + std::to_string(poscouner));
        }
    }
    tmp.push_back(' ');
    (this->*state)(' ');
    if(state != &Scanner::Start)
        throw std::runtime_error("error in line: " + std::to_string(linecounter) + ", position: " + std::to_string(poscouner));
    file.close();
    for(int i = 0;i < Programm.size();i++)
        std:: cout << Programm[i].value << "    " << Programm[i].type << "\n";
}
///////////////////////////////////////////////////////////////////////////////////

namespace execution {
    using Variable_Type = enum {
        integer, string, boolean, empty
    };
    using Variable_Name = std::string;

    struct Variable_Value {
        Variable_Value() {};

        Variable_Value(int value) : integer_value(value) {}

        Variable_Value(const std::string &value) : string_value(value) {}

        Variable_Value(bool value) : bool_value(value) {}

        int integer_value;
        bool bool_value;
        std::string string_value;
    };

    struct Variable {
        Variable_Value value;
        const Variable_Type type;
        const Variable_Name name;

        Variable(const Variable_Name &name_, Variable_Type type_, std::size_t = 0) : name(name_), type(type_) {};
    };

    class StackValue {
    public:
        StackValue(Variable *variable) : variable_(variable) { type_ = variable->type; };

        StackValue(Variable_Value value, Variable_Type type) : value_(value), variable_(nullptr), type_(type) {};

        StackValue(const std::string &value) {
            type_ = string;
            value_.string_value = value;
            variable_ = nullptr;
        };
        StackValue(int value) {
            type_ = integer;
            value_.integer_value = value;
            variable_ = nullptr;
        };
        StackValue(bool value) {
            type_ = boolean;
            value_.bool_value = value;
            variable_ = nullptr;
        };
        operator int() const {
            CheckIs(integer);
            return Get().integer_value;
        };
        operator std::string() const {
            CheckIs(string);
            return Get().string_value;
        };
        operator bool() const {
            CheckIs(boolean);
            return Get().bool_value;
        };
        ///
        StackValue SetValue(const StackValue &value) {
            if (!variable_) {
                throw std::logic_error("stack value is not variable");
            }
            //  if(variable_->type != value.type_)
            //    throw std::logic_error("bad SetValue!");
            variable_->value = value.Get();
            return StackValue(variable_->value, variable_->type);
        }

        Variable_Type GetType() const {
            return type_;
        }
    private:
        Variable *variable_;
        Variable_Type type_;
        Variable_Value value_;
        const Variable_Value &Get() const {
            return (variable_ != nullptr) ? variable_->value : value_;
        }
        void CheckIs(Variable_Type type) const {
            if (type != type_) {
                throw std::logic_error("error in CheckIs !");
            }
        }
    };

    using OperationIndex = std::size_t;
    struct Context {
        OperationIndex operation_index = 0;
        std::stack<execution::StackValue> stack;
        std::unordered_map<std::string, std::unique_ptr<execution::Variable>> variables;
    };

    struct Operation {
        virtual ~Operation() {}
        virtual void Do(Context &context) const = 0;
    };

    struct GoOperation : Operation {
        GoOperation(OperationIndex index) : index_(index) {}
        void Do(Context &context) const override {
            context.operation_index = index_;
        }
    private:
        const OperationIndex index_;
    };
    ///
    struct IfOperation : GoOperation {
        IfOperation(OperationIndex index) : GoOperation(index) {}
        void Do(Context &context) const final {
            StackValue value = context.stack.top();
            context.stack.pop();
            if (value.GetType() == boolean) {
                if (!static_cast<bool>(value)) {
                    GoOperation::Do(context);
                }
            }
            else if (value.GetType() == integer) {
                    if (!static_cast<int>(value))
                        GoOperation::Do(context);
                } else
                    throw std::runtime_error("bad arguement for OPERATOR_IF()");
        }
    };
    ///
    struct ValueOperation : Operation {
        ValueOperation(StackValue value) : value_(value) {}
        void Do(Context &context) const final {
            context.stack.emplace(value_);
        }
    private:
        const StackValue value_;
    };

    struct VariableOperation : Operation {
        VariableOperation(const Variable_Name &name) : name_(name) {}
        void Do(Context &context) const final {
            context.stack.emplace(context.variables.at(name_).get());
        }
    private:
        const Variable_Name name_;
    };
    ///
    struct AddVariableOperation : Operation {
        AddVariableOperation(const Variable_Name &name, Variable_Type type, std::size_t size = 256) : name_(name),
                                                                                                      type_(type),
                                                                                                      size_(size) {}
        void Do(Context &context) const final {
            // добавляем переменную
            context.variables[name_].reset(new Variable(name_, type_, size_));
        }
    private:
        const Variable_Name name_;
        const Variable_Type type_;
        const std::size_t size_;
    };

    ///
    template<class T1, class T2>
    struct AssignOperation : Operation {
        void Do(Context &context) const final {
            StackValue op2 = context.stack.top();
            context.stack.pop();
            StackValue op1 = context.stack.top();
            context.stack.pop();
            T1 value = static_cast<T2>(op2);
            op1.SetValue(value);
        }
    };

    template<class T>
    struct UnaryMinusOperation : Operation {
        void Do(Context &context) const final {
            context.stack.top() = -static_cast<T>(context.stack.top());
        }
    };
    ///
    template<class T>
    struct PrintOperation : Operation {
        void Do(Context &context) const final {
            const StackValue op1 = context.stack.top();
            context.stack.pop();
            std::cout << static_cast<T>(op1) << std::endl;
        }
    };
    ///
    template<class T>
    struct ReadOperation : Operation {
        void Do(Context &context) const final {
            T tmp;
            std:: cin >> tmp;
            context.stack.emplace(tmp);
        }
    };
    //////////////////////
    struct MathOperation : Operation {
        void Do(Context &context) const final {
            // извлекаем второй операнд
            StackValue op2 = context.stack.top();
            context.stack.pop();
            // извлекаем первый операнд
            StackValue op1 = context.stack.top();
            context.stack.pop();
            // записываем результат операции
            context.stack.emplace(DoMath(op1, op2));
        }

        virtual StackValue DoMath(const StackValue &op1, const StackValue &op2) const = 0;
    };

    template<class T1, class T2>
    struct PlusOperation : MathOperation {
        StackValue DoMath(const StackValue &op1, const StackValue &op2) const final {
            return static_cast<T1>(op1) + static_cast<T2>(op2);
        }
    };

    template<class T1, class T2>
    struct MinusOperation : MathOperation {
        StackValue DoMath(const StackValue &op1, const StackValue &op2) const final {
            return static_cast<T1>(op1) - static_cast<T2>(op2);
        }
    };

    template<class T1, class T2>
    struct MulOperation : MathOperation {
        StackValue DoMath(const StackValue &op1, const StackValue &op2) const final {
            return static_cast<T1>(op1) * static_cast<T2>(op2);
        }
    };

    template<class T1, class T2>
    struct DivOperation : MathOperation {
        StackValue DoMath(const StackValue &op1, const StackValue &op2) const final {
            return static_cast<T1>(op1) / static_cast<T2>(op2);
        }
    };

    template<class T1, class T2>
    struct LessOperation : MathOperation {
        StackValue DoMath(const StackValue &op1, const StackValue &op2) const final {
            return static_cast<T1>(op1) < static_cast<T2>(op2);
        }
    };

    template<class T1, class T2>
    struct GreaterOperation : MathOperation {
        StackValue DoMath(const StackValue &op1, const StackValue &op2) const final {
            return static_cast<T1>(op1) > static_cast<T2>(op2);
        }
    };

    ///
    template<class T1, class T2>
    struct EqualOperation : MathOperation {
        StackValue DoMath(const StackValue &op1, const StackValue &op2) const final {
            return static_cast<T1>(op1) == static_cast<T2>(op2);
        }
    };
    ///
    template<class T1, class T2>
    struct AndOperation : MathOperation {
        StackValue DoMath(const StackValue &op1, const StackValue &op2) const final {
            return (static_cast<T1>(op1) && static_cast<T2>(op2)) ? true : false;
        }
    };
    ///
    template<class T1, class T2>
    struct OrOperation : MathOperation {
        StackValue DoMath(const StackValue &op1, const StackValue &op2) const final {
            return (static_cast<T1>(op1) || static_cast<T2>(op2)) ? true : false;
        }
    };

    using Operations = std::vector<std::shared_ptr<Operation>>;
    enum OperationType {
        Plus, Minus, Mul, Div, Less, Greater, Equality, And, Or, UnaryMinus, Print, Assign , Read,
    };

    using UnaryKey = std::tuple<OperationType, Variable_Type>;
    static const std::map<UnaryKey, std::shared_ptr<Operation>> kUnaries{
            {{UnaryMinus, integer}, std::shared_ptr<Operation>(new UnaryMinusOperation<int>)},
            {{Print, string}, std::shared_ptr<Operation>(new PrintOperation<std::string>)},
            {{Print, integer}, std::shared_ptr<Operation>(new PrintOperation<int>)},
            {{Print, boolean}, std::shared_ptr<Operation>(new PrintOperation<bool>)},
            ///
            {{Read, string}, std::shared_ptr<Operation>(new ReadOperation<std::string>)},
            {{Read, integer}, std::shared_ptr<Operation>(new ReadOperation<int>)},
            {{Read, boolean}, std::shared_ptr<Operation>(new ReadOperation<bool>)},
    };

    using BinaryKey = std::tuple<OperationType, Variable_Type , Variable_Type>;
    ///
    static const std::map<BinaryKey,std::shared_ptr<Operation>> kBinaries{
            {{Plus, string, string},std::shared_ptr<Operation>(new PlusOperation<std::string,std::string>)},
            {{Plus, integer, integer}, std::shared_ptr<Operation>(new PlusOperation<int,int>)},
            {{Plus, integer, boolean}, std::shared_ptr<Operation>(new PlusOperation<int,bool>)},
            {{Plus, boolean, integer}, std::shared_ptr<Operation>(new PlusOperation<bool,int>) },
            {{Plus, boolean, boolean}, std::shared_ptr<Operation>(new PlusOperation<bool,bool>)},
            ///
            {{Minus, integer, integer}, std::shared_ptr<Operation>(new MinusOperation<int,int>)},
            {{Minus, integer, boolean}, std::shared_ptr<Operation>(new MinusOperation<int,bool>)},
            {{Minus, boolean, integer}, std::shared_ptr<Operation>(new MinusOperation<bool,int>) },
            {{Minus, boolean, boolean}, std::shared_ptr<Operation>(new MinusOperation<bool,bool>)},
            ///
            {{Mul, integer, integer}, std::shared_ptr<Operation>(new MulOperation<int,int>)},
            {{Mul, integer, boolean}, std::shared_ptr<Operation>(new MulOperation<int,bool>)},
            {{Mul, boolean, integer}, std::shared_ptr<Operation>(new MulOperation<bool,int>) },
            {{Mul, boolean, boolean}, std::shared_ptr<Operation>(new MulOperation<bool,bool>)},
            ///
            {{Div, integer, integer}, std::shared_ptr<Operation>(new DivOperation<int,int>)},
            {{Div, integer, boolean}, std::shared_ptr<Operation>(new DivOperation<int,bool>)},
            {{Div, boolean, integer}, std::shared_ptr<Operation>(new DivOperation<bool,int>) },
            {{Div, boolean, boolean}, std::shared_ptr<Operation>(new DivOperation<bool,bool>)},
            ///
            {{Less, string, string}, std::shared_ptr<Operation>(new LessOperation<std::string,std::string>)},
            {{Less, integer, integer}, std::shared_ptr<Operation>(new LessOperation<int,int>) },
            {{Less, integer, boolean}, std::shared_ptr<Operation>(new LessOperation<int,bool>)},
            {{Less, boolean, integer}, std::shared_ptr<Operation>(new LessOperation<bool,int>)},
            {{Less, boolean, boolean}, std::shared_ptr<Operation>(new LessOperation<bool,bool>)},
            ///
            {{Greater, string, string}, std::shared_ptr<Operation>(new GreaterOperation<std::string,std::string>)},
            {{Greater, integer, integer}, std::shared_ptr<Operation>(new GreaterOperation<int,int>) },
            {{Greater, integer, boolean}, std::shared_ptr<Operation>(new GreaterOperation<int,bool>)},
            {{Greater, boolean, integer}, std::shared_ptr<Operation>(new GreaterOperation<bool,int>)},
            {{Greater, boolean, boolean}, std::shared_ptr<Operation>(new GreaterOperation<bool,bool>)},
            ///
            {{Equality, string, string}, std::shared_ptr<Operation>(new EqualOperation<std::string,std::string>)},
            {{Equality, integer, integer}, std::shared_ptr<Operation>(new EqualOperation<int,int>) },
            {{Equality, integer, boolean}, std::shared_ptr<Operation>(new EqualOperation<int,bool>)},
            {{Equality, boolean, integer}, std::shared_ptr<Operation>(new EqualOperation<bool,int>)},
            {{Equality, boolean, boolean}, std::shared_ptr<Operation>(new EqualOperation<bool,bool>)},
            ///
            {{And, integer, integer}, std::shared_ptr<Operation>(new AndOperation<int,int>)},
            {{And, integer, boolean}, std::shared_ptr<Operation>(new AndOperation<int,bool>)},
            {{And, boolean, integer}, std::shared_ptr<Operation>(new AndOperation<bool,int>)},
            {{And, boolean, boolean}, std::shared_ptr<Operation>(new AndOperation<bool,bool>)},
            ///
            {{Or, integer, integer}, std::shared_ptr<Operation>(new OrOperation<int,int>)},
            {{Or, integer, boolean}, std::shared_ptr<Operation>(new OrOperation<int,bool>)},
            {{Or, boolean, integer}, std::shared_ptr<Operation>(new OrOperation<bool,int>)},
            {{Or, boolean, boolean}, std::shared_ptr<Operation>(new OrOperation<bool,bool>)},
            ///
            {{Assign, string, string}, std::shared_ptr<Operation>(new AssignOperation<std::string,std::string>)},
            {{Assign, integer, integer}, std::shared_ptr<Operation>(new AssignOperation<int,int>) },
            {{Assign, integer, boolean}, std::shared_ptr<Operation>(new AssignOperation<int,bool>)},
            {{Assign, boolean, integer}, std::shared_ptr<Operation>(new AssignOperation<bool,int>)},
            {{Assign, boolean, boolean}, std::shared_ptr<Operation>(new AssignOperation<bool,bool>)},
    };

    static const std::map<Lexem::LexemType, execution::OperationType> kOperationsMap{
            {Lexem::Lexemand, execution::And},{Lexem::Lexemor, execution::Or}, {Lexem::LexemAssign, execution::Assign},
            {Lexem::LexemAdd, execution::Plus},{Lexem::LexemSub, execution::Minus},{Lexem::LexemMul, execution::Mul},
            {Lexem::LexemDiv, execution::Div},{Lexem::LexemEquality, execution::Equality},{Lexem::LexemLess, execution::Less},
            {Lexem::LexemGreater, execution::Greater},
    };

}





////////////////////////////////////////////////////////////////////////////////////
class Parser{
    Scanner scanner;
    using type_size = std::pair<execution::Variable_Type,std::size_t>;
    using Variables_Types = std:: unordered_map <std::string,type_size>;
    Variables_Types variables_types;
    int currentword_index;
    void AddVariables(const std::vector<std::string>& var_names,type_size t_s);
    const Lexem& TakeLexem()const;
    type_size DefineType(const std::string& var_name)const;
    type_size NewOperation(type_size op1,type_size op2,const Lexem& operation);
    void NextLexem();
    bool HasLexem()const;
    bool TryLexem(Lexem::LexemType type)const;
    void S();
    void HEAD();
    void DESCRIPTIONS();
    void MAIN();
    void DESCRIPTIONLINES();
    void IDENTIFIER_LIST(std:: vector<std:: string> & variables);
    type_size TYPE();
    std:: size_t STRING_SIZE();
    void OPERATORS();
    void OPERATOR();
    void OPERATORS_();
    type_size ASSIGNMENT(const std::string& op1_name);
    type_size EXPRESSION();
    type_size X();
    type_size Y(type_size op1_ts);
    type_size A();
    type_size B(type_size op1_ts);
    type_size C();
    type_size D(type_size op1_ts);
    type_size E();
    type_size F(type_size op1_ts);
    bool SIGN();
    type_size TERM();
    void OPERATOR_IF();
    void ELSE();
    void CYCLE_FOR();
    void CYCLE_WHILE();
    void PRINT();
    void READ();
public:
    execution:: Operations operations;
    explicit Parser(const char* filename);
    void Run();
};

Parser::type_size Parser::NewOperation(type_size op1,type_size op2,const Lexem& operation)
{
    execution::OperationType operation_ = execution::kOperationsMap.at(operation.type);
    auto it = execution::kBinaries.find(std::make_tuple(operation_,op1.first,op2.first));
    if(it == execution::kBinaries.end()){
        throw std::runtime_error("type mismatch at line: " + std::to_string(TakeLexem().line));
    }
    operations.emplace_back(it->second);
    if((operation_ == execution::Plus) || (operation_ == execution::Minus) || (operation_ == execution::Mul) ||
    (operation_ == execution::Div))
        return (op1.first == execution::string) ? type_size(execution::string,0):type_size(execution::integer,0);
    else if((operation_ == execution::Less) || (operation_ == execution::Greater) || (operation_ == execution::Equality) ||
    (operation_ == execution::And) || (operation_ == execution::Or))
        return type_size(execution::boolean,0);
    else
        return op1;
}

Parser::type_size Parser::DefineType(const std::string &var_name) const
{
    auto it = variables_types.find(var_name);
    if (it ==  variables_types.end()) {
        throw std::runtime_error("variable: " + var_name + " does not defined");
    }
    return it->second;
}

void Parser::AddVariables(const std::vector<std::string> &var_names, Parser::type_size t_s)
{
    for (const std::string& var_name : var_names)
    {
        auto p = variables_types.emplace(var_name, t_s);
        if (!p.second) {
            throw std::runtime_error("variable: " + var_name + " is described more than once");
        }
        operations.emplace_back(new execution::AddVariableOperation(var_name, t_s.first, t_s.second));
    }
}

void Parser::Run()
{
    S();
}

Parser::Parser(const char* filename) :
        scanner(filename)
//scanner("/Users/ivan/CLionProjects/lexer/textfile.txt")
{
    currentword_index = 0;
}

bool Parser::HasLexem() const
{
    return (scanner.size() > currentword_index );
}

bool Parser::TryLexem(Lexem::LexemType type) const
{
        return (TakeLexem().type == type);
}

void Parser::NextLexem()
{
    currentword_index++;
}

const Lexem& Parser::TakeLexem() const
{
    if(!HasLexem()){
        std:: cout << "there is no lexem!!!\n";
        throw std::runtime_error("no lexems!");
    }
    return scanner.GetLexem(currentword_index);
}

void Parser::S()
{
    HEAD();
    DESCRIPTIONS();
    MAIN();
}

void Parser::HEAD()
{
    if(!TryLexem(Lexem::Lexemprogram)){
        throw std::runtime_error("expected: program,line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
    if(!TryLexem(Lexem::LexemIdentifier)){
        throw std::runtime_error("expected identifier!,line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
    if(!TryLexem(Lexem::LexemSemicolon)){
        throw std::runtime_error("expected \';\',line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
}

void Parser::DESCRIPTIONS()
{
    if(!TryLexem(Lexem::Lexemvar)){
        throw std::runtime_error("expected var!, line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
    DESCRIPTIONLINES();
}

void Parser::DESCRIPTIONLINES()
{
    if(TryLexem(Lexem::LexemIdentifier)){
        std:: vector <std::string> variables;
        variables.emplace_back(TakeLexem().value);
        NextLexem();
        IDENTIFIER_LIST(variables);
        if(!TryLexem(Lexem::LexemColon)){
            throw std::runtime_error("expected: \':\',line: " + std::to_string(TakeLexem().line));
        }
        NextLexem();
        AddVariables(variables,TYPE());
        if(!TryLexem(Lexem::LexemSemicolon)){
            throw std::runtime_error("expected: \';\',line: " + std::to_string(TakeLexem().line));
        }
        NextLexem();
        DESCRIPTIONLINES();
    }
}

void Parser::IDENTIFIER_LIST(std:: vector<std:: string>& variables)
{
    if(TryLexem(Lexem::LexemComma)){
        NextLexem();
        if(!TryLexem(Lexem::LexemIdentifier)){
            throw std::runtime_error("expected: identifier,line: " + std::to_string(TakeLexem().line));
        }
        variables.emplace_back(TakeLexem().value);
        NextLexem();
        IDENTIFIER_LIST(variables);
    }
}

Parser::type_size Parser::TYPE()
{
    execution::Variable_Type type;
    std::size_t size = 0;
    if(TryLexem(Lexem::Lexeminteger)) {
        NextLexem();
        type = execution::Variable_Type::integer;
    }
    else if(TryLexem(Lexem::Lexemstring)) {
        NextLexem();
        size = STRING_SIZE();
        type = execution::Variable_Type::string;
    }
    else if(TryLexem(Lexem::Lexembool)) {
        NextLexem();
        type = execution::Variable_Type::boolean;
    }
    else{
        throw std::runtime_error("expected: TYPE,line: " + std::to_string(TakeLexem().line));
    }
    return Parser::type_size(type,size);
}

std::size_t Parser::STRING_SIZE()
{
    std::size_t size = 0;
    if(TryLexem(Lexem:: LexemLeftSquare)){
        NextLexem();
        if(!TryLexem(Lexem:: LexemIntegerConst))
            throw std::runtime_error("expected: string size,line: " + std::to_string(TakeLexem().line));
        size = std:: stoi(TakeLexem().value);
        NextLexem();
        if(!TryLexem(Lexem:: LexemRightSquare))
            throw std::runtime_error("expected: \'[\',line: " + std::to_string(TakeLexem().line));
        NextLexem();
    }
    return size;
}

void Parser::MAIN()
{
    if(!TryLexem(Lexem::Lexembegin)){
        throw std::runtime_error("expected: begin,line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
    OPERATORS();
    if(!TryLexem(Lexem::Lexemend)){
        throw std::runtime_error("expected: end,line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
    if(!TryLexem(Lexem::LexemPoint)){
        throw std::runtime_error("expected: \'.\',line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
    if(HasLexem()){
        throw std::runtime_error("expected: nothing");
    }
}

void Parser::OPERATORS()
{
    OPERATOR();
    OPERATORS_();
}

void Parser::OPERATORS_()
{
    if(TryLexem(Lexem::LexemSemicolon)){
        NextLexem();
        OPERATOR();
        OPERATORS_();
    }
}

void Parser::OPERATOR()
{
    if(TryLexem(Lexem::Lexembegin)){
        NextLexem();
        OPERATORS();
        if(!TryLexem(Lexem::Lexemend)){
            throw std::runtime_error("expected: end,line: " + std::to_string(TakeLexem().line));
        }
        NextLexem();
    }
    else if(TryLexem(Lexem::LexemIdentifier)) {
        std::string op1_name = TakeLexem().value;
        NextLexem();
        ASSIGNMENT(op1_name);
    }
    else if(TryLexem(Lexem::Lexemif)){
        NextLexem();
        OPERATOR_IF();
    }
    else if(TryLexem(Lexem:: Lexemfor)){
        NextLexem();
        CYCLE_FOR();
    }
    else if(TryLexem(Lexem::Lexemwhile)){
        NextLexem();
        CYCLE_WHILE();
    }
    else if(TryLexem(Lexem::LexemPrint)){
        NextLexem();
        PRINT();
    }
    else if(TryLexem(Lexem::LexemRead)){
        NextLexem();
        READ();
    }
}

Parser:: type_size Parser::ASSIGNMENT(const std::string& op1_name)
{
    type_size op1 = DefineType(op1_name);
    operations.emplace_back(new execution::VariableOperation(op1_name));
    if(!TryLexem(Lexem:: LexemAssign)){
        throw std::runtime_error("expected: \':=\',line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
    type_size op2 = EXPRESSION();
    NewOperation(op1,op2,Lexem::LexemAssign);
    return op1;
}

void Parser::CYCLE_FOR()
{
    if(!TryLexem(Lexem::LexemIdentifier)){
        throw std::runtime_error("expected: identifier, line: " + std::to_string(TakeLexem().line));
    }
    std::string op1_name = TakeLexem().value;
    NextLexem();
    ASSIGNMENT(op1_name);
    if(!TryLexem(Lexem::Lexemto)){
        throw std::runtime_error("expected: to,line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
    std::size_t label2 = operations.size();
    operations.emplace_back(new execution::VariableOperation(op1_name));
    type_size op2_type = EXPRESSION();
    NewOperation(DefineType(op1_name),op2_type,Lexem::LexemLess);
    std::size_t label1 = operations.size();
    operations.emplace_back(nullptr);
    if(!TryLexem(Lexem::Lexemdo)){
        throw std::runtime_error("expected: \'do\',line: " + std::to_string(TakeLexem().line));

    }
    NextLexem();
    OPERATOR();
    operations.emplace_back(new execution::VariableOperation(op1_name));
    operations.emplace_back(new execution::VariableOperation(op1_name));
    operations.emplace_back(new execution::ValueOperation(1));
    NewOperation(DefineType(op1_name),type_size(execution::integer,0),Lexem::LexemAdd);
    NewOperation(DefineType(op1_name),type_size(execution::integer,0),Lexem::LexemAssign);
    operations.emplace_back(new execution::GoOperation(label2));
    operations[label1].reset(new execution::IfOperation(operations.size()));
}

void Parser::READ()
{
    if(!TryLexem(Lexem::LexemLeftParanthesis)){
        throw std::runtime_error("expected: \'(\',line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
    Lexem op = TakeLexem();
    NextLexem();
    type_size type = DefineType(op.value);
    operations.emplace_back(new execution::VariableOperation(op.value));
    operations.emplace_back(execution::kUnaries.at(std::make_tuple(execution::Read,type.first)));
    NewOperation(type,type,Lexem::LexemAssign);
    if(!TryLexem(Lexem::LexemRightParanthesis)){
        throw std::runtime_error("expected: \')\',line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
}

void Parser::PRINT()
{
    if(!TryLexem(Lexem::LexemLeftParanthesis)){
        throw std::runtime_error("expected: \'(\',line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
    type_size expr_type = EXPRESSION();
    operations.emplace_back(execution::kUnaries.at(std::make_tuple(execution::Print,expr_type.first)));
    if(!TryLexem(Lexem::LexemRightParanthesis)){
        throw std::runtime_error("expected: \')\'");
    }
    NextLexem();
}

void Parser::OPERATOR_IF()
{
    EXPRESSION();
    std::size_t label1 = operations.size();
    operations.emplace_back(nullptr);
    if(!TryLexem(Lexem::Lexemthen)){
        throw std::runtime_error("expected: \'then\',line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
    OPERATOR();
    operations[label1].reset(new execution::IfOperation(operations.size()));
    ELSE();
}

void Parser::ELSE()
{
    if(TryLexem(Lexem::Lexemelse)){
        NextLexem();
        OPERATOR();
    }
}

void Parser::CYCLE_WHILE()
{
    std::size_t label1 = operations.size();
    EXPRESSION();
    std::size_t label2 = operations.size();
    operations.emplace_back(nullptr);
    if(!TryLexem(Lexem::Lexemdo)){
        throw std::runtime_error("expected: \'do\',line: " + std::to_string(TakeLexem().line));
    }
    NextLexem();
    OPERATOR();
    operations.emplace_back(new execution:: GoOperation(label1));
    operations[label2].reset(new execution:: IfOperation(operations.size()));
}

Parser::type_size Parser::EXPRESSION()
{
    type_size op1_ts = X();
    return Y(op1_ts);
}

Parser::type_size Parser::Y(type_size op1_ts)
{
    while((TryLexem(Lexem::Lexemand)) || (TryLexem(Lexem::Lexemor))){
        Lexem operation = TakeLexem();
        NextLexem();
        type_size type_next_op = X();
        op1_ts = NewOperation(op1_ts,type_next_op,operation);
    }
    return op1_ts;
}

Parser::type_size Parser::X()
{
    type_size op1_ts = A();
    return B(op1_ts);
}

Parser::type_size Parser::A()
{
    type_size op1_ts = C();
    return  D(op1_ts);
}

Parser::type_size Parser::B(type_size op1_ts)
{
    while(TryLexem(Lexem:: LexemLess) || (TryLexem(Lexem:: LexemGreater)) || (TryLexem(Lexem:: LexemEquality))){
        Lexem operation = TakeLexem();
        NextLexem();
        type_size type_next_op = A();
        op1_ts = NewOperation(op1_ts,type_next_op,operation);
    }
    return op1_ts;
}

Parser::type_size Parser:: C()
{
    type_size op1_ts = E();
    return F(op1_ts);
}

Parser::type_size Parser::D(type_size op1_ts)
{
    while(TryLexem(Lexem::LexemAdd) || (TryLexem(Lexem::LexemSub))){
        Lexem operation = TakeLexem();
        NextLexem();
        type_size type_next_op = C();
        op1_ts = NewOperation(op1_ts,type_next_op,operation);
    }
    return op1_ts;
}

Parser::type_size Parser:: F(type_size op1_ts)
{
    while(TryLexem(Lexem::LexemMul) || (TryLexem(Lexem::LexemDiv))){
        Lexem operation = TakeLexem();
        NextLexem();
        type_size type_next_op = E();
        op1_ts = NewOperation(op1_ts,type_next_op,operation);
    }
    return op1_ts;
}

Parser::type_size Parser:: E()
{
    bool has_unary_minus = SIGN();
    type_size op1_ts = TERM();
    if(has_unary_minus)
        operations.emplace_back(execution::kUnaries.at(std::make_tuple(execution::UnaryMinus,op1_ts.first)));
    return op1_ts;
}

bool Parser::SIGN()
{
    bool has_unary_minus = false;
    while(TryLexem(Lexem::LexemAdd) || (TryLexem(Lexem::LexemSub))){
        if(TakeLexem().type == Lexem::LexemSub)
            has_unary_minus = !has_unary_minus;
        NextLexem();
    }
    return has_unary_minus;
}

Parser::type_size Parser::TERM()
{
    std::pair<execution::Variable_Type ,std::size_t> curr_type;
    if(TryLexem(Lexem::LexemIdentifier) || TryLexem(Lexem:: LexemIntegerConst) || TryLexem(Lexem:: LexemStringConst) || TryLexem(Lexem:: LexemBoleanConst) ){
        switch(TakeLexem().type){
            case Lexem::LexemIdentifier:{
                operations.emplace_back(new execution::VariableOperation(TakeLexem().value));
                curr_type = std::pair<execution::Variable_Type ,std::size_t >(DefineType(TakeLexem().value));
                break;
            }
            case Lexem::LexemIntegerConst:{
                operations.emplace_back(new execution::ValueOperation(std::stoi(TakeLexem().value)));
                curr_type = std::pair<execution::Variable_Type ,std::size_t >(execution::integer,0);
                break;
            }
            case Lexem::LexemStringConst:    {
                operations.emplace_back(new execution::ValueOperation(TakeLexem().value));
                curr_type =  std::pair<execution::Variable_Type ,std::size_t >(execution::string,0);
                break;
            }
            case Lexem::LexemBoleanConst:    {
                operations.emplace_back(new execution::ValueOperation((TakeLexem().value == "true")));
                curr_type = std::pair<execution::Variable_Type ,std::size_t >(execution::boolean,0);
                break;
            }
        }
        NextLexem();
        return curr_type;
    }
    else if(TryLexem(Lexem::LexemLeftParanthesis)){
        NextLexem();
        type_size type_next_op = EXPRESSION();
        if(!TryLexem(Lexem::LexemRightParanthesis)){
            throw std::runtime_error("expected: \')\'");
        }
        NextLexem();
        return type_next_op;
    }
    else{
        throw std::runtime_error("expected Expression");
    };
}

/////////////////////////////////////////////////////////////////////////////////////////////////

    int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "expected argument" << std::endl;
        return 1;
    }
        try {
           // Parser parser("/Users/ivan/ClionProjects/lexer/textfile.txt");
            Parser parser(argv[1]);

            parser.Run();
            std :: cout << "correct\n";
            execution::Context context;
            while(context.operation_index < parser.operations.size()) {
                const auto &operation = parser.operations[context.operation_index];
                ++context.operation_index;
                operation->Do(context);
            }
        }
        catch(const std::exception& e){
            std::cerr << "error: " << e.what() << std::endl;
            return 1;
        }
        return 0;
}