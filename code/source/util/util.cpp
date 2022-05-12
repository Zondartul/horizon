#include "util.h"
#include "event.h"
#include "timer.h"
#include "stringUtils.h"
//void initUtil(){
	//initEvents();
	//initTimers();
	//printf("-------- util init done -----------\n");
//}
typedef int (*funcptr)(void);
void crash(){
	int x = *(int*)0;
}

/*
logmessage::logmessage(string msg_, const char* file_, int line_){
    time = getRealTime();
    msg = msg_;
    file = file_;
    line = line_;
    printf("constructed lmsg, t = %.3f, file = [%s], line = %d, msg = [%s]\n\n",time,file.c_str(),line,msg.c_str());
};
logmessage::logmessage(string msg_){
    time = getRealTime();
    msg = msg_;
    file = "";
    line = 0;
}
logmessage::logmessage(const logmessage& lmsg){
    time = lmsg.time;
    msg = lmsg.msg;
    file = lmsg.file;
    line = lmsg.line;
}
string toString(logmessage lmsg){
    return fstring("[%.3f]%s:%d: %s",lmsg.time,lmsg.file.c_str(),lmsg.line,lmsg.msg.c_str());
}
//extern vector<void*> getTrace();
exKind::exKind(logmessage msg_):msg(msg_){
    try{
        //vector<void*> tr = getTrace();
        msg.msg+="\ntrace:------\n";
        for(unsigned int I = 0; I < tr.size(); I++){
            void *p = tr[I];
            msg.msg += fstring("%d: in %p\n",I,p);
        }
        msg.msg+="end trace-----\n";
    }catch(...){}
}
const char* exKind::what() const noexcept{
    string S = fstring("[%.3f]%s:%d:",msg.time,msg.file.c_str(),msg.line);
    S += msg.msg;
    return stralloc(S.c_str());
}
*/
