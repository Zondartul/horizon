#pragma once
#include <string>
#include <vector>
/// stackInfo is implementation-defined information about the stack trace of a function,
/// for tracing errors to code source lines.

struct codePos{
    std::string filepath;
    std::string function;
    int row;
    int col;
};

class stackInfo{
    public:
    stackInfo(); /// captures trace at the moment of construction
    std::vector<codePos> trace;
    void *pimpl;
};
