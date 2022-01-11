#ifndef UTIL_GUARD
#define UTIL_GUARD
#include "stdlib.h"
#include <exception>
using std::exception;
#include <string>
using std::string;

void initUtil();
void selfdestruct();

template <typename T> T* copyToHeap(T &obj){
    T* ptr = new T();
    *ptr = obj;
    return ptr;
}


struct logmessage{
    float time;
    string msg;
    string file;
    int line;
    logmessage(string msg_, const char* file_, int line_); //message and address
    logmessage(string msg_);   //only message
    logmessage(const logmessage& lmsg); //copy-constructor
};
string toString(logmessage);

class exKind : public exception {
    public:
    logmessage msg;
    exKind(logmessage msg_);
    virtual const char* what() const noexcept;
};

#define ex(...) exKind(logmessage(fstring( __VA_ARGS__ ), __FILE__, __LINE__ ))

#endif
