#ifndef _EFSM_CONFIG_H_
#define _EFSM_CONFIG_H_

//FSM_CHECK_STATE_EN宏用于判断是否检查state参数是否正常
//0:关闭检查
//1:开启检查
#if (!defined EFSM_CHECK_STATE_EN)
    #define EFSM_CHECK_STATE_EN 0
#endif

//最多检查几个状态
#if (defined EFSM_CHECK_STATE_EN) && (EFSM_CHECK_STATE_EN != 0)
    #define EFSM_CHECK_STATE_NUMB 16
#endif

//是否使用断言
//0:关闭断言
//1:开启断言
#if (!defined EFSM_USE_ASSERT)
    #define EFSM_USE_ASSERT 0
#endif

#endif

