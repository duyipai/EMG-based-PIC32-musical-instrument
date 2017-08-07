#include "dsp.h"
static unsigned char threshold = 170;
static unsigned char numOfHit();
static unsigned char statusEq(struct status * a, struct status * b);
struct accumStatus
{
    struct status statusArr[20];
    unsigned char size;
    unsigned char frontIndex;
};

static struct status nullStatus;
struct accumStatus globalStatus;

struct status getNote()
{
    if (globalStatus.size<19)
    {
        return nullStatus;
    }
    else
    {
        if (numOfHit(&(globalStatus.statusArr), &(globalStatus.statusArr[globalStatus.frontIndex]))>1)
        {
            return globalStatus.statusArr[globalStatus.frontIndex];
        }
        else
        {
            return nullStatus;
        }
    }
}

void pushStatus()
{
    struct status * modifiy = &(globalStatus.statusArr[globalStatus.frontIndex]);
    ++ globalStatus.size;
    ++globalStatus.frontIndex;
    if(globalStatus.frontIndex > 19)
    {
        globalStatus.frontIndex = 0;
    }

    unsigned char hit = 0;
    unsigned char i;
    for(i = 0; i < 10; ++i)
    {
        if(receiver.arrA[i] > threshold)
        {
            ++hit;
        }
    }
    if (hit > 6)
    {
        modifiy->statusA = 1;
    }
    else
    {
        modifiy->statusA = 0;
    }

    hit = 0;
    for(i = 0; i < 10; ++i)
    {
        if(receiver.arrB[i] > threshold)
        {
            ++hit;
        }
    }
    if (hit > 6)
    {
        modifiy->statusB = 1;
    }
    else
    {
        modifiy->statusB = 0;
    }

    hit = 0;
    for(i = 0; i < 10; ++i)
    {
        if(receiver.arrC[i] > threshold)
        {
            ++hit;
        }
    }
    if (hit > 6)
    {
        modifiy->statusC = 1;
    }
    else
    {
        modifiy->statusC = 0;
    }
}

static unsigned char statusEq(struct status * a, struct status * b)
{
    unsigned char res = 0;
    if(a->statusA == b->statusA)
    {
        if(a->statusB == b->statusB)
        {
            if(a->statusC == b->statusC)
            {
                res = 1;
            }
        }
    }
    return res;
}

static unsigned char numOfHit(struct accumStatus * arr, struct status * element)
{
    unsigned char num = 0;
    unsigned char i;
    for(i= 0; i < 20; ++i)
    {
        if(statusEq(&(arr->statusArr[i]), element))
        {
            ++num;
        }
    }
    return num;
}
