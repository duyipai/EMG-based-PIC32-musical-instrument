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
    unsigned char arrA[20];
    unsigned char arrB[20];
    unsigned char arrC[20];
} receiver;

void pushStatus();
struct status getNote();
#endif // 
