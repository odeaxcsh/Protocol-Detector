#if !defined(ELICITATION_UTILS)
#define ELICITATION_UTILS

#include <vector>
#include <string>
#include <functional>
#include <type_traits>
#include <map>

int get_int(const std::vector<unsigned char> &);
std::string get_str(const std::vector<unsigned char> &);

template<typename dtype>
class _Base_expression
{
public:
    virtual dtype eval() const = 0;
};

template<typename dtype, dtype(*)(dtype, dtype)>
class Binary_operator : public _Base_expression<dtype>
{
public:
    Binary_operator(const _Base_expression<dtype> &r, const _Base_expression<dtype> &l) : right(&r), left(&l)
    {

    }

    virtual dtype eval() const
    {
        return Operator(left->eval(), right->eval());
    }

private:
    _Base_expression<dtype> *right, *left;
};

template<typename dtype, typename Operator>
class Unary_operator : public _Base_expression<dtype>
{
public:
    Unary_operator(const _Base_expression<dtype> &expr) : down_node(&expr)
    {

    }

    virtual dtype eval() const
    {
        return Operator(down_node->eval());
    }

private:
    const _Base_expression<dtype> *down_node;
};

template<typename dtype>
class Value : public _Base_expression<dtype>
{
public:
    Value(const dtype &val) : value(&val)
    {

    }

    virtual dtype eval() const 
    {
        return this->value;
    }

private:
    const dtype *value;
};

template<typename dtype>
class Ref_Value : public _Base_expression<dtype>
{
public:
    Ref_Value(const std::map<std::string, std::vector<unsigned char>> &variables, const std::string &ref_name) :
     variables(&variables), ref_name(ref_name)
    {

    }

    virtual dtype eval() const 
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
    const std::map<std::string, std::vector<unsigned char>> &variables;
};

using int_expressions = _Base_expression<int>;
using Int_Sum = Binary_operator<int, +[](int a, int b)->int { return a + b; }>;
using Int_Mul = Binary_operator<int, +[](int a, int b)->int { return a * b; }>;
using Int_Div = Binary_operator<int, +[](int a, int b)->int { return a / b; }>;

using Str_Sum = Binary_operator<std::string, +[](std::string a, std::string b)->std::string { return a + b; }>;


#endif