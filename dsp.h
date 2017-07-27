#ifndef DSP_H
#define DSP_H
struct status
{
    unsigned char statusA=0;
    unsigned char statusB=0;
    unsigned char statusC=0;
};

status getNote();
#endif // 