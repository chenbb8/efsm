#include "stdio.h"
#include "efsm_hal.h"

void Efsm_AssertFailed(char *e, char *file, int line)
{
    EFSM_PRINT("%s,%s,%d",e, file, line);
}


