#ifndef UARTDEBUG_H__
#define UARTDEBUG_H__

#include <stdint.h>

void debug_init();

void debug_send(uint8_t *string);

#endif //UARTDEBUG_H__