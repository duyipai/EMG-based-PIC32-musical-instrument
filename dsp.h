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
    unsigned char arrA[10];
    unsigned char arrB[10];
    unsigned char arrC[10];
} receiver;

void pushStatus();
struct status getNote();
#endif // 
