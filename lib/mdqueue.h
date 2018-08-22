
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "mdtypedef.h"

#pragma pack(1)

// ԭ��, ������ʹ��
typedef struct {
    mdUINT8 *pData;        /* ����malloc */
    mdUINT32 nFront;       /* ͷָ�� */
    mdUINT32 nRear;        /* βָ�룬�����в��գ�ָ�����βԪ�ص���һ��λ�� */
    mdUINT32 size;         /* ���г��� */
} BaseQueue_t;

#pragma pack()

//�궨�����Ч��
// read ��emptyʱ������
// read ����, get ������
// get ǰ��ȷ�����㹻Ԫ��
// remove ͷָ�����1

// write ��fullʱ������Ԫ��
// push ��fullʱ��remove��write
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

