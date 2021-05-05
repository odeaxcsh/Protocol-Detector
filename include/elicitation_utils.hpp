#if !defined(ELICITATION_UTILS)
#define ELICITATION_UTILS

#include <vector>
#include <string>
#include <functional>
#include <type_traits>
#include <map>

#include "lexer.hpp"

int get_int(const std::vector<unsigned char> &);
std::string get_str(const std::vector<unsigned char> &);

template<typename dtype>
class Base_expression
{
public:
    virtual dtype eval(const std::map<std::string, std::vector<unsigned char>> &) const = 0;
};

template<typename dtype, dtype(*Operator)(dtype, dtype)> 
class Binary_operator : public Base_expression<dtype>
{
public:
    Binary_operator(const Base_expression<dtype> *r, const Base_expression<dtype> *l) : right(r), left(l)
    {

    }

    virtual dtype eval(const std::map<std::string, std::vector<unsigned char>> &variables) const
    {
        return Operator(left->eval(variables), right->eval(variables));
    }

private:
    const Base_expression<dtype> *right, *left;
};

template<typename dtype, dtype(*Operator)(dtype, dtype)>
class Unary_operator : public Base_expression<dtype>
{
public:
    Unary_operator(const Base_expression<dtype> &expr) : down_node(&expr)
    {

    }

    virtual dtype eval(const std::map<std::string, std::vector<unsigned char>> &variables) const
    {
        return Operator(down_node->eval(variables));
    }

private:
    const Base_expression<dtype> *down_node;
};

template<typename dtype>
class Value : public Base_expression<dtype>
{
public:
    Value(const dtype &val) : value(&val)
    {

    }

    virtual dtype eval(const std::map<std::string, std::vector<unsigned char>> &variables) const 
    {
        return this->value;
    }

private:
    const dtype *value;
};


template<typename dtype, typename ftype>
class Cast_Expression : public Base_expression<dtype>
{
public:
    Cast_Expression(const Base_expression<ftype> &val) : value(&val)
    {
        
    }

    virtual dtype eval(const std::map<std::string, std::vector<unsigned char>> &variables) const 
    {
        if constexpr (std::is_same_v<ftype, dtype>) {
            return this->value->eval(variables);
        }
    }

private:
    const Base_expression<ftype> *value;
};

template<typename dtype>
class Ref_Value : public Base_expression<dtype>
{
public:
    Ref_Value(std::string ref_name) : ref_name(ref_name)
    {

    }

    virtual dtype eval(const std::map<std::string, std::vector<unsigned char>> &variables) const 
    {
        if constexpr (std::is_same_v<dtype, int>) {
            return get_int(variables.at(ref_name));
        } else if constexpr (std::is_same_v<dtype, std::string>) {
            return get_str(variables.at(ref_name));
        } else {
            
        }
    }

private:
    const std::string ref_name;
};

class Formula_parser
{
public:
    Formula_parser()
    {

    }

    template <typename dtype>
    Base_expression<dtype> *parse(std::string expression) const;

    using Int_expressions = Base_expression<int>;
    using Int_Sum = Binary_operator<int, +[](int a, int b)->int { return a + b; }>;
    using Int_Mul = Binary_operator<int, +[](int a, int b)->int { return a * b; }>;
    using Int_Div = Binary_operator<int, +[](int a, int b)->int { return  a / b; }>;

    const static std::map<char, Token_type> defalut_operators;
    const static std::map<Token_type, int> priority;
};

#endif