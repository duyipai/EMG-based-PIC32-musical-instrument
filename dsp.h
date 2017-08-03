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
    unsigned char arrA[30];
    unsigned char arrB[30];
    unsigned char arrC[30];
} receiver;

void pushStatus();
struct status getNote();
#endif // 