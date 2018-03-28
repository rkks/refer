@rem Makefile for Microsoft 32-bit C/C++ compiler
cl -Za -c -W4 rand.c
cl -Za -c -W4 heap.c
cl -Za -c -W4 test.c
link test heap rand
