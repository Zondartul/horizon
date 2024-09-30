#pragma once
#include <string>
#include "util/stackInfo.hpp"
#include "util/globals_util.h"
/// zError is a deferred error that includes the stack trace
#undef WARNING
#undef ERROR

class DLLAPI_UTIL zError{
public:
    enum eSeverity{NONE, INFO, WARNING, ERROR, FATAL} severity;
    enum eBlame{NOBODY, INTERNAL, ENGINE, APP, USER} blame;
    zError(std::string new_msg, eSeverity new_severity = ERROR, eBlame new_blame = NOBODY);
    const char* what();
private:
    std::string msg;
    stackInfo stack;
};

/// push() sends the message up depending on type 
/// - throw for errors
/// - print for infos and warnings
/// - interface with IDE [problem] items if available
void DLLAPI_UTIL push(zError new_error); 

/// Severity:
/// NONE - enum not initialized
/// INFO - this is spam
/// WARNING - this warning can be ignored (style, deprecation, etc)
/// ERROR - something failed but we can keep going (e.g. missing texture)
/// FATAL - app must close now (e.g. stack is smashed)

/// Blame:
/// NOBODY - enum not initialized
/// INTERNAL - some code error in the engine
/// ENGINE - engine logic error
/// APP - app code or logic error
/// USER - user input error

