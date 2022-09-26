#ifndef _EFSM_H_
#define _EFSM_H_

#include "efsm_config.h"

typedef struct structEfsm efsm_t;
typedef void (*efsmState_t)(efsm_t *ao, uint16_t evt);//状态的类型
#define _BIT(A) (1<<(A))
#define _EFSM_USER_BIT(B) ((EFSM_EVT_USER)<<(B))
enum {
    EFSM_EVT_ENTRY  = _BIT(0),//内部：进入事件
    EFSM_EVT_EXIT   = _BIT(1),//内部：退出事件
    EFSM_EVT_COMMON = _BIT(2),//内部：通用事件，每次调用Efsm_Hand()时候自动产生
    EFSM_EVT_TICK   = _BIT(3),//用户：已定义的tick事件
    EFSM_EVT_USER   = _BIT(4),//用户：自定义事件的起始bit
};

struct structEfsm {
    efsmState_t state;//当前状态
    efsmState_t nextState;//需要转换的状态
    uint16_t    evt;//事件
#if (defined EFSM_CHECK_STATE_EN) && (EFSM_CHECK_STATE_EN != 0)
    uint16_t stateRegIdx;//注册状态的索引
    efsmState_t checkStateTbl[EFSM_CHECK_STATE_NUMB];
#endif
};

void Efsm_Ctor(efsm_t *ao, efsmState_t state);//构造EFSN
void Efsm_RegState(efsm_t *ao, efsmState_t state);//为state参数检测功能，注册state
void Efsm_Hand(efsm_t *ao);//运行EFSM，非阻塞函数
void Efsm_EvtTrig(efsm_t *ao, uint16_t evt);//发送事件
void Efsm_Trans(efsm_t *ao, efsmState_t state);//切换状态
#define EFSM_REG_STATE(STATE) (Efsm_RegState((AO), (efsmState_t)(STATE)))//为当前对象注册state
#define EFSM_EVT_TRIG(EVT) (Efsm_EvtTrig((AO), (EVT)))//为当前对象发送事件
#define EFSM_TRANS(STATE) (Efsm_Trans((AO), (efsmState_t)(STATE)))//为当前对象切换状态

#endif // _EFSM_H_

