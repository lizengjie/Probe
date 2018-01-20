#ifndef _DISPOSE_H_
#define _DISPOSE_H_

#include <stdint.h>

int32_t InquireUartRecvBuffer(uint8_t *pBuf, int8_t MaxBuffCount);
void Dispose(void);

#endif	

