#define swap(t,x,y)                                     \
do {                                                    \
    (unsigned char *)_0=(unsigned char *)(&(x));        \
    (unsigned char *)_1=(unsigned char *)(&(y));        \
    unsigned long _2 = (unsigned long)                  \
       ((unsigned char *)(&(x)+1)                       \
        - (unsigned char *)(&(x)));                     \
    while (_2--) {                                      \
        *_0 ^= *_1;                                     \
        *_1 ^= *_0;                                     \
        *_0 ^= *_1;                                     \
        _0++;                                           \
        _1++;                                           \
    }                                                   \
} while (0)
