#ifndef DSP_H
#define DSP_H
struct status
{
    unsigned char statusA;
    unsigned char statusB;
    unsigned char statusC;
};

struct Receiver
{
    unsigned char arrA[2];
    unsigned char arrB[2];
    unsigned char arrC[2];
} receiver;

void pushStatus();
struct status getNote();
#endif // 
