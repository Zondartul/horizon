#include "util/stackInfo.hpp"
/// StackInfo uses execinfo.h when on GCC
/// fails silently on Windows

#ifdef LINUX
    #ifdef GCC
        #include "execinfo.h"
    #endif
#endif

stackInfo::stackInfo(){
    pimpl = 0;
    #ifdef LINUX
        #ifdef GCC
            void *array[100];
            int n_entries = backtrace(array, 100);
            char **symbols = backtrace_symbols(array, 100);
            for(int i = 0; i < n_entries; i++){
                std::string S(symbols[i]);
                codePos cp{"",S,0,0};
                trace.push_back(cp);
            }
        #endif
    #endif
}
