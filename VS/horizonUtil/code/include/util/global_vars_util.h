#pragma once
#include "util/globals_util.h"

struct DLLAPI_UTIL GlobalsUtil {
	GlobalsUtil();
	struct sysEventKind* sysEvent;
	struct sysTimerKind* sysTimer;
};

extern DLLAPI_UTIL struct GlobalsUtil* Gu;

