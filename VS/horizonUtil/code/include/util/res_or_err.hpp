#pragma once
#include "util/zerror.hpp"
#include <cassert>
#include <optional>
/// a Result type that either contains a value, or an error message.

template<typename T, typename E> class Result{
private:
    std::optional<T> res;
public:
    std::optional<E> err;
    Result(T new_res):res(new_res){}
    Result(E new_err):err(new_err){}
    bool ok(){return !err;}
    T& val(){assert(ok()); return *res;}
};

template<typename T, typename E> T maybe(Result<T,E> res){
    if(res.ok()){
        return res.val();
    }else{
        res.err->severity = zError::WARNING;
        push(*res.err);
        return T{0};
    }
}

template<typename T, typename E> T require(Result<T,E> res){
    if(res.ok()){
        return res.val();
    }else{
        res.err->severity = zError::FATAL;
        push(*res.err);
    }
}
