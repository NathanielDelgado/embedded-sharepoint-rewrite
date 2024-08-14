#include "stm32x4xx_hal_init.h"

int main(void) {

    HAL_MspInit();
    SystemClock_Config();

    int count = 0;
    
    count = count + 1;
    
    return 0;
}
