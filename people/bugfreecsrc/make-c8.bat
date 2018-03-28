@rem Makefile for Microsoft C/C++ 8.0
cl -AL -c -W4 -G2s rand.c
cl -AL -c -W4 -G2s heap.c
cl -AL -c -W4 -G2s test.c
link test heap rand;
