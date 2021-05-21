#if !defined(PRINT_FORMAT_HPP_)
#define PRINT_FORMAT_HPP

#include <string>
#include <vector>
#include <map>

class Base_print_format
{
public:
    virtual std::string eval(const std::map<std::string, std::vector<unsigned char>> &, int) const = 0;
    
    bool in_print_level(int level)
    {
        return level <= print_level;
    }

    static Base_print_format *parse(const std::string &);

private:
    int print_level;
};

class Root_print_format : public Base_print_format
{
public:
    virtual std::string eval(const std::map<std::string, std::vector<unsigned char>> &, int) const override;

private:
    std::vector<Base_print_format *> format;
};

class Value_print_format : public Base_print_format
{
public:
    virtual std::string eval(const std::map<std::string, std::vector<unsigned char>> &, int) const override;

private:
    std::string value;
};

class Ref_print_format : public Base_print_format
{
public:
    virtual std::string eval(const std::map<std::string, std::vector<unsigned char>> &, int) const override;
    
    inline virtual std::string get_name() const
    {
        return ref_name;
    }

protected:
    std::string ref_name;
    enum class Type {
        INT,
        STR,
        HEX,
    } type;

    enum class Seperator {
        DOT,
        NEWLINE,
        SPACE,
    } seperator;
};

class Iteration_item : public Ref_print_format
{
public:    
    inline virtual std::string get_name() const override
    {
        return ref_name + (counter > 0 ? "[" + std::to_string(counter) + "]" : "");
    }

    inline void set_counter(int i)
    {
        this->counter = i;
    }

private:
    int counter;
};

class Iteration_print_format : public Base_print_format
{
public:
    virtual std::string eval(const std::map<std::string, std::vector<unsigned char>> &, int) const override;

private:
    std::vector<Base_print_format *> iterate_over;
};

#endif