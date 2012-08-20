#include "StdAfx.h"
#include "AutoInit.h"
#include "..\Adaboost\AdaBoost.h"
#include <time.h> 

static AutoInit initializationInstance;

AutoInit::AutoInit(void)
{
	time_t t = time( 0 );
	srand((unsigned int)t);
}


AutoInit::~AutoInit(void)
{
	//¼ì²éÄÚ´æÐ¹Â©!
	_CrtDumpMemoryLeaks();
}
