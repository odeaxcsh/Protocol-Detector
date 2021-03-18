#if !defined(EXCEPTIONS_HPP)
#define EXCEPTIONS_HPP

#include <stdexcept>

class UnbalancedParanthesis : public std::exception
{
public:
    virtual const char *what() const noexcept
    {
        return "Unbalanced Paranthesis";
    }

private:
};

class ParanthesisExpected : public std::exception
{
public:
    virtual const char *what() const noexcept
    {
        return "Paranthesis Expected";
    }

private:
};

#endif
