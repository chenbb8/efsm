#ifndef _EFSM_HAL_H_
#define _EFSM_HAL_H_

#include "efsm_config.h"

#define EFSM_PRINT  printf

#if (defined EFSM_USE_ASSERT) && (EFSM_USE_ASSERT != 0)
#define EFSM_ASSERT(e) ((e) ? (void)0 : \
    Efsm_AssertFailed(#e, __FILE__, __LINE__))
#else
#define EFSM_ASSERT(e)
#endif

//临界区
#define EFSM_CRITICAL_ENTER()
#define EFSM_CRITICAL_EXIT()

void Efsm_AssertFailed(char *e, char *file, int line);

#endif
