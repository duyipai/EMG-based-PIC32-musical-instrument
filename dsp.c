#include "dsp.h"
static unsigned char numOfHit();
static unsigned char statusEq(status * a, status * b);
struct accumStatus
{
    status statusArr[10];
    unsigned char size=0;
    unsigned char frontIndex=0;
};

static status nullStatus;
static accumStatus globalStatus;

status getNote()
{
    if (globalStatus.size<9)
    {
        return nullStatus;
    }
    else
    {
        if (numOfHit(globalStatus.statusArr, globalStatus.statusArr[frontIndex])>4)
        {
            return globalStatus.statusArr[frontIndex];
        }
        else
        {
            return nullStatus;
        }
    }
}

static unsigned char statusEq(status * a, status * b)
{
    unsigned char res=0;
    if(a->statusA==b->statusA)
    {
        if(a->statusB==b->statusB)
        {
            if(a->statusC==b->statusC)
            {
                res=1;
            }
        }
    }
    return res;
}

static unsigned char numOfHit(accumStatus * arr, status * element)
{
    unsigned char num=0;
    for(unsigned char i=0;i<10;++i)
    {
        if(statusEq(arr+i, element))
        {
            ++num;
        }
    }
    return num;
}