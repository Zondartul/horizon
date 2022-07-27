#include "util/global_vars_util.h"
#include "util/event.h"
#include "util/timer.h"

struct GlobalsUtil* Gu = 0;

GlobalsUtil::GlobalsUtil() {
	sysEvent = new sysEventKind();
	sysTimer = new sysTimerKind(*sysEvent);
}