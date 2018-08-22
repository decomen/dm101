
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "mdtypedef.h"

#pragma pack(1)

// 原型, 不参与使用
typedef struct {
    mdUINT8 *pData;        /* 考虑malloc */
    mdUINT32 nFront;       /* 头指针 */
    mdUINT32 nRear;        /* 尾指针，若队列不空，指向队列尾元素的下一个位置 */
    mdUINT32 size;         /* 队列长度 */
} BaseQueue_t;

#pragma pack()

//宏定义提高效率
// read 在empty时不操作
// read 后移, get 不后移
// get 前需确保有足够元素
// remove 头指针后移1

// write 在full时不操作元素
// push 在full时先remove再write
#define nQueueSize(_q) ( (_q.size) / sizeof( _q.pData[0] ) )
#define bQueueFull(_q) (((_q.nRear + 1) % (nQueueSize(_q)) == _q.nFront) ? mdTRUE: mdFALSE)
#define bQueueEmpty(_q) ((_q.nFront == _q.nRear) ? mdTRUE : mdFALSE)
#define bQueueRead(_q, _e) ((_q.nFront == _q.nRear) ? mdFALSE : ((_e = _q.pData[_q.nFront]), (_q.nFront = (_q.nFront + 1) % (nQueueSize(_q))), (mdTRUE)))
#define bQueueRemove(_q) ((_q.nFront == _q.nRear) ? mdFALSE : ((_q.nFront = (_q.nFront + 1) % (nQueueSize(_q))), (mdTRUE)))
#define vQueueClear(_q) (_q.nFront = _q.nRear = 0)
#define xQueueGet(_q, _index) (_q.pData[(_q.nFront + (_index)) % (nQueueSize(_q))])
#define bQueueWrite(_q, _e) ((_q.nRear + 1) % (nQueueSize(_q)) == _q.nFront ? mdFALSE : ((_q.pData[_q.nRear] = _e), (_q.nRear = (_q.nRear + 1) % (nQueueSize(_q))), (mdTRUE)))
#define bQueuePush(_q, _e) ((_q.nRear + 1) % (nQueueSize(_q)) == _q.nFront ? (bQueueRemove(_q), bQueueWrite(_q, _e)) : (bQueueWrite(_q, _e), (mdTRUE)))
#define nQueueLen(_q) ((_q.nRear - _q.nFront + nQueueSize(_q)) % nQueueSize(_q))
#define bQueueOverHalf(_q) ((nQueueLen(_q)>=(nQueueSize(_q)>>1))? mdTRUE : mdFALSE)

#endif

