#ifndef TREEEXCEPTION_H
#define TREEEXCEPTION_H

#include <iostream>

class tree_exception : public std::exception
{
private:
    std::string exception_message;

public:
    tree_exception();
    void set_exception_message(std::string exception_message);
    const char *what() const noexcept;
};

tree_exception::tree_exception()
{
}

void tree_exception::set_exception_message(std::string exception_message)
{
    this->exception_message = exception_message;
}

const char* tree_exception::what() const noexcept
{
    return exception_message.c_str();
}

#endif // EXCEPTION_H
