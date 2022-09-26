/**
 * @brief efsm是一个轻量级的函数指针有限状态机框架
 * @author chenbb8@126.com
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "efsm.h"
#include "efsm_hal.h"

/**
* @brief 构造EFSN
* @param ao[in/out]: 被操作的对象
* @param state: 初始化状态
*/
void Efsm_Ctor(efsm_t *ao, efsmState_t state)
{
    EFSM_ASSERT(ao != NULL);

    ao->state = state;
    ao->nextState = NULL;
    ao->evt = 0;
#if (defined EFSM_CHECK_STATE_EN) && (EFSM_CHECK_STATE_EN != 0)
    ao->stateRegIdx = 0;
#endif
    Efsm_EvtTrig(ao, EFSM_EVT_ENTRY);
}

/**
* @brief 为state参数检测功能，注册state
* @param ao[in/out]: 被操作的对象
* @param state: 被注册的state
*/
void Efsm_RegState(efsm_t *ao, efsmState_t state)
{
    EFSM_ASSERT(ao != NULL);

#if (defined EFSM_CHECK_STATE_EN) && (EFSM_CHECK_STATE_EN != 0)
    if (ao->stateRegIdx < EFSM_CHECK_STATE_NUMB)
    {
        ao->checkStateTbl[++ao->stateRegIdx] = state;
        ++ao->stateRegIdx;
    }
#else
    (void)ao;
    (void)state;
#endif
}

//获取最靠前的一个事件
static uint16_t Efsm_GetEvt(uint16_t evt)
{
    uint8_t i;
    for (i = 0; i < 16; ++i)
    {
        if (_BIT(i)&evt) {
            return _BIT(i);
        }
    }
    return 0;
}

/**
* @brief 运行EFSM
* @param ao[in/out]: 被操作的对象
* @note state: 非阻塞函数
*/
void Efsm_Hand(efsm_t *ao)
{
    uint16_t evtBit;

    EFSM_ASSERT(ao != NULL);

    evtBit = EFSM_EVT_COMMON;
    ao->state(ao, evtBit);
    while (ao->evt!=0)
    {
        evtBit = Efsm_GetEvt(ao->evt);
        if (evtBit)
        {
            ao->state(ao, evtBit);
            if ((evtBit==EFSM_EVT_EXIT)&&(ao->nextState!=NULL)) {
                EFSM_CRITICAL_ENTER();
                ao->state = ao->nextState;
                ao->nextState = NULL;
                EFSM_CRITICAL_EXIT();
                Efsm_EvtTrig(ao, EFSM_EVT_ENTRY);
            }
            EFSM_CRITICAL_ENTER();
            ao->evt &= ~evtBit;
            EFSM_CRITICAL_EXIT();
        }
    }
}

/**
* @brief 发送事件
* @param ao[in/out]: 被操作的对象
* @param evt: 被发送的事件
*/
void Efsm_EvtTrig(efsm_t *ao, uint16_t evt)
{
    EFSM_ASSERT(ao != NULL);

    EFSM_CRITICAL_ENTER();
    ao->evt |= evt;
    EFSM_CRITICAL_EXIT();
}

#if (defined EFSM_CHECK_STATE_EN) && (EFSM_CHECK_STATE_EN != 0)
//检测state是否已经被注册
static bool Efsm_CheckState(efsm_t *ao, efsmState_t state)
{
    int16_t readIdx = ao->stateRegIdx-1;
    efsmState_t *pTbl = &ao->checkStateTbl[0];

    while (readIdx >= 0)
    {
        if (pTbl[readIdx--] == state)
        {
            return true;
        }
    }
    return false;
}
#endif

/**
* @brief 切换状态
* @param ao[in/out]: 被操作的对象
* @param state: 被切换的状态
*/
void Efsm_Trans(efsm_t *ao, efsmState_t state)
{
    EFSM_ASSERT(ao != NULL);

#if (defined EFSM_CHECK_STATE_EN) && (EFSM_CHECK_STATE_EN != 0)
    if (!Efsm_CheckState(ao, state))
    {
        EFSM_PRINT("state error!\n");
        while(1);
    }
#endif
    EFSM_CRITICAL_ENTER();
    ao->nextState = state;
    Efsm_EvtTrig(ao, EFSM_EVT_EXIT);
    EFSM_CRITICAL_EXIT();
}


