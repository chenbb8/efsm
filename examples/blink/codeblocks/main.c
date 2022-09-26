#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
//#include <unistd.h>
#include "efsm.h"

typedef struct {
    efsm_t super;
    uint16_t timeCount;
    uint32_t globalTick;
} led_t;
led_t ao_led;
#define AO (efsm_t*)led

void Led_StateOn(led_t *led, uint16_t evt);
void Led_StateOff(led_t *led, uint16_t evt);

void Led_Ctor(led_t *led)
{
    Efsm_Ctor((efsm_t*)led, (efsmState_t)Led_StateOn);
    led->globalTick = 0;
    EFSM_REG_STATE(Led_StateOn);//提前注册state，用于Efsm_Trans()的参数检测
    EFSM_REG_STATE(Led_StateOff);
}

void Led_StateOn(led_t *led, uint16_t evt)
{
    switch (evt)
    {
        case EFSM_EVT_EXIT:
            printf("%8d:Led_StateOn, Exit\n",led->globalTick);
            break;
        case EFSM_EVT_ENTRY:
            led->timeCount = 0;
            printf("%8d:Led_StateOn, Entry\n",led->globalTick);
            break;
        case EFSM_EVT_TICK:
            ++led->globalTick;
            if (++led->timeCount >= 2000) {
                EFSM_TRANS(Led_StateOff);
            }
            break;
    }
}
void Led_StateOff(led_t *led, uint16_t evt)
{
    switch (evt)
    {
        case EFSM_EVT_EXIT:
            printf("%8d:Led_StateOff, Exit\n",led->globalTick);
            break;
        case EFSM_EVT_ENTRY:
            led->timeCount = 0;
            printf("%8d:Led_StateOff, Entry\n",led->globalTick);
            break;
        case EFSM_EVT_TICK:
            ++led->globalTick;
            if (++led->timeCount >= 2000) {
                EFSM_TRANS(Led_StateOn);
            }
            break;
    }
}

int main()
{
    Led_Ctor(&ao_led);
    while (1)
    {
        Efsm_EvtTrig((efsm_t*)&ao_led, EFSM_EVT_TICK);
        Efsm_Hand((efsm_t*)&ao_led);
        Sleep(1);
    }
    system("pause");
    return 0;
}

