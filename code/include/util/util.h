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
    logmessage(string msg_, const char* file_, int line_); 
    logmessage(string msg_);   
    logmessage(const logmessage& lmsg); 
};
string toString(logmessage);
class exKind : public exception {
    public:
    logmessage msg;
    exKind(logmessage msg_);
    virtual const char* what() const noexcept;
};
#define ex(...) exKind(logmessage(fstring( __VA_ARGS__ ), __FILE__, __LINE__ ))
#define DECLARE_enum_class_bitwise_operators(T) \
	bool operator|(T lhs, T rhs); \
	bool operator&(T lhs, T rhs); \
	T & operator|=(T & lhs, T rhs); \
	T & operator&=(T & lhs, T rhs); 
#define DEFINE_ecbo_op_helper(T , op) \
bool operator op (T lhs, T rhs){ \
	using underlying = typename std::underlying_type< T >::type; \
	return (bool)(static_cast< T >( \
		static_cast<underlying>(lhs) op \
		static_cast<underlying>(rhs) \
	));\
}
#define DEFINE_ecbo_asop_helper(T , asop, op) \
T &operator asop (T &lhs, T rhs){ \
	using underlying = typename std::underlying_type< T >::type; \
	lhs = static_cast< T >( \
		static_cast<underlying>(lhs) op \
		static_cast<underlying>(rhs) \
	);\
	\
	return lhs;\
}
#define DEFINE_enum_class_bitwise_operators(T) \
	DEFINE_ecbo_op_helper(T, |); \
	DEFINE_ecbo_op_helper(T, &); \
	DEFINE_ecbo_asop_helper(T, |=, |); \
	DEFINE_ecbo_asop_helper(T, &=, &);
#endif
