#include "util/zerror.hpp"
#include <iostream>

zError::zError(std::string new_msg, 
    eSeverity new_severity, 
    eBlame new_blame):
    severity(new_severity),
    blame(new_blame),
    msg(new_msg),
    stack(stackInfo()){}

const char* zError::what(){return msg.c_str();}

void push(zError err){
    switch(err.severity){
        case zError::INFO:
        case zError::WARNING:
            std::cout << err.what() << std::endl;
            return;
        break;
        case zError::ERROR:
            std::cerr << err.what() << std::endl;
            return;
        break;
        case zError::NONE:
        case zError::FATAL:
        default:
            std::cerr << err.what() << std::endl;
            throw err;
        break;
    }
}
