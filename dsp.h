#ifndef DSP_H
#define DSP_H
struct status
{
    unsigned char statusA=0;
    unsigned char statusB=0;
    unsigned char statusC=0;
};

struct Receiver
{
    unsigned char arrA[30];
    unsigned char arrB[30];
    unsigned char arrC[30];
} receiver;

void pushStatus();
status getNote();
#endif // 