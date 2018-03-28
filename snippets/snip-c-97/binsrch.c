/*
**  BINSRCH.C - Binary search arrays for a key.
**
**  Public domain demo by Bob Stout
*/

#include <stdio.h>
#include <stddef.h>
#include "binsrch.h"

#if !(__cplusplus)

/*
**  BinSearchI(): Search a int key
**
**  Arguments: 1 - Address to return the key position
**             2 - Key to find
**             3 - Pointer to the beginning of the array
**             4 - Number of elements in the array
**
**  Returns: Index if found, else -1
*/

int BinSearchI(int key, int *r, size_t n)
{
      int high, i, low;

      if (n > 1)
      {
            for (low = 0, high = n-1;  high - low > 1; )
            {
                  i = (high+low) / 2;
                  if (key == r[i])
                        return i;
                  else if (key < r[i])
                        high = i;
                  else  low  = i;
            }
            if (key == r[high])
                  return high;
            else if (key == r[low])
                  return low;
            else  return -1;
      }
      else if (r[0] == key)
            return 0;
      else  return -1;
}

/*
**  BinSearchC(): Search a character key
**
**  Arguments: 1 - Address to return the key position
**             2 - Key to find
**             3 - Pointer to the beginning of the array
**             4 - Number of elements in the array
**
**  Returns: Index if found, else -1
*/

int BinSearchC(signed char key, signed char *r, size_t n)
{
      int high, i, low;

      if (n > 1)
      {
            for (low = 0, high = n-1;  high - low > 1; )
            {
                  i = (high+low) / 2;
                  if (key == r[i])
                      return i;
                  else if (key < r[i])
                        high = i;
                  else  low  = i;
            }
            if (key == r[high])
                  return high;
            else if (key == r[low])
                  return low;
            else  return Error_;
      }
      else if (r[0] == key)
            return 0;
      else  return -1;
}

/*
**  BinSearchB(): Search a byte key
**
**  Arguments: 1 - Address to return the key position
**             2 - Key to find
**             3 - Pointer to the beginning of the array
**             4 - Number of elements in the array
**
**  Returns: Index if found, else -1
*/

int BinSearchB(BYTE key, BYTE *r, size_t n)
{
      int high, i, low;

      if (n > 1)
      {
            for (low = 0, high = n-1;  high - low > 1; )
            {
                  i = (high+low) / 2;
                  if (key == r[i])
                      return i;
                  else if (key < r[i])
                        high = i;
                  else  low  = i;
            }
            if (key == r[high])
                  return high;
            else if (key == r[low])
                  return low;
            else  return -1;
      }
      else if (r[0] == key)
            return 0;
      else  return -1;
}

/*
**  BinSearchS(): Search a short key
**
**  Arguments: 1 - Address to return the key position
**             2 - Key to find
**             3 - Pointer to the beginning of the array
**             4 - Number of elements in the array
**
**  Returns: Index if found, else -1
*/

int BinSearchS(signed short key, signed short *r, size_t n)
{
      int high, i, low;

      if (n > 1)
      {
            for (low = 0, high = n-1;  high - low > 1; )
            {
                  i = (high+low) / 2;
                  if (key == r[i])
                        return i;
                  else if (key < r[i])
                        high = i;
                  else  low  = i;
            }
            if (key == r[high])
                  return high;
            else if (key == r[low])
                  return low;
            else  return -1;
      }
      else if (r[0] == key)
            return 0;
      else  return -1;
}

/*
**  BinSearchW(): Search a word key
**
**  Arguments: 1 - Address to return the key position
**             2 - Key to find
**             3 - Pointer to the beginning of the array
**             4 - Number of elements in the array
**
**  Returns: Index if found, else -1
*/

int BinSearchW(WORD key, WORD *r, size_t n)
{
      int high, i, low;

      if (n > 1)
      {
            for (low = 0, high = n-1;  high - low > 1; )
            {
                  i = (high+low) / 2;
                  if (key == r[i])
                        return i;
                  else if (key < r[i])
                        high = i;
                  else  low  = i;
            }
            if (key == r[high])
                  return high;
            else if (key == r[low])
                  return low;
            else  return -1;
      }
      else if (r[0] == key)
            return 0;
      else  return -1;
}

/*
**  BinSearchL(): Search a long key
**
**  Arguments: 1 - Address to return the key position
**             2 - Key to find
**             3 - Pointer to the beginning of the array
**             4 - Number of elements in the array
**
**  Returns: Index if found, else -1
*/

int BinSearchL(signed long key, signed long *r, size_t n)
{
      int high, i, low;

      if (n > 1)
      {
            for (low = 0, high = n-1;  high - low > 1; )
            {
                  i = (high+low) / 2;
                  if (key == r[i])
                        return i;
                  else if (key < r[i])
                        high = i;
                  else  low  = i;
            }
            if (key == r[high])
                  return high;
            else if (key == r[low])
                  return low;
            else  return -1;
      }
      else if (r[0] == key)
            return 0;
      else  return -1;
}

/*
**  BinSearchDW(): Search a double word key
**
**  Arguments: 1 - Address to return the key position
**             2 - Key to find
**             3 - Pointer to the beginning of the array
**             4 - Number of elements in the array
**
**  Returns: Index if found, else -1
*/

int BinSearchDW(DWORD key, DWORD *r, size_t n)
{
      int high, i, low;

      if (n > 1)
      {
            for (low = 0, high = n-1;  high - low > 1; )
            {
                  i = (high+low) / 2;
                  if (key == r[i])
                        return i;
                  else if (key < r[i])
                        high = i;
                  else  low  = i;
            }
            if (key == r[high])
                  return high;
            else if (key == r[low])
                  return low;
            else  return -1;
      }
      else if (r[0] == key)
            return 0;
      else  return -1;
}

/*
**  BinSearchF(): Search a float key
**
**  Arguments: 1 - Address to return the key position
**             2 - Key to find
**             3 - Pointer to the beginning of the array
**             4 - Number of elements in the array
**
**  Returns: Index if found, else -1
*/

int BinSearchF(float key, float *r, size_t n)
{
      int high, i, low;

      if (n > 1)
      {
            for (low = 0, high = n-1;  high - low > 1; )
            {
                  i = (high+low) / 2;
                  if (key == r[i])
                        return i;
                  else if (key < r[i])
                        high = i;
                  else  low  = i;
            }
            if (key == r[high])
                  return high;
            else if (key == r[low])
                  return low;
            else  return -1;
      }
      else if (r[0] == key)
            return 0;
      else  return -1;
}

/*
**  BinSearchD(): Search a double key
**
**  Arguments: 1 - Address to return the key position
**             2 - Key to find
**             3 - Pointer to the beginning of the array
**             4 - Number of elements in the array
**
**  Returns: Index if found, else -1
*/

int BinSearchD(double key, double *r, size_t n)
{
      int high, i, low;

      if (n > 1)
      {
            for (low = 0, high = n-1;  high - low > 1; )
            {
                  i = (high+low) / 2;
                  if (key == r[i])
                        return i;
                  else if (key < r[i])
                        high = i;
                  else  low  = i;
            }
            if (key == r[high])
                  return high;
            else if (key == r[low])
                  return low;
            else  return -1;
      }
      else if (r[0] == key)
            return 0;
      else  return -1;
}

#else /* C++ */

//  See BINSRCH.H for binary search function template

#endif /* C/C++ */

#ifdef TEST

#include <stdlib.h>

#if !(__cplusplus)

#include <stdio.h>

main(int argc, char *argv[])
{
      double a1[] = { 1, 3, 17, 39, 66, 67, 88, 111, 222, 333, 345 };
      double a2[] = { 1, 3, 17, 39, 66, 67, 88, 111, 222, 333, 345, 678};
      double a3[] = { 5, 9, 37 };
      double a4[] = { 5, 9, 13 };
      double a5[] = { 5, 9 };
      double a6[] = { 6 };
      double a7[] = { -20, 1, 30 };
      double key;
      int    idx;
      
      while (--argc)
      {
            key = atof(*(++argv));
            if (-1 != (idx = BinSearchD(key, (double *)a1, sizeof(a1) / sizeof(key))))
                  printf("Key %f found at position %d in a1[]\n", key, idx);
            else  printf("Key %f not found in a1[]\n", key);
            if (-1 != (idx = BinSearchD(key, (double *)a2, sizeof(a2) / sizeof(key))))
                  printf("Key %f found at position %d in a2[]\n", key, idx);
            else  printf("Key %f not found in a2[]\n", key);
            if (-1 != (idx = BinSearchD(key, (double *)a3, sizeof(a3) / sizeof(key))))
                  printf("Key %f found at position %d in a3[]\n", key, idx);
            else  printf("Key %f not found in a3[]\n", key);
            if (-1 != (idx = BinSearchD(key, (double *)a4, sizeof(a4) / sizeof(key))))
                  printf("Key %f found at position %d in a4[]\n", key, idx);
            else  printf("Key %f not found in a4[]\n", key);
            if (-1 != (idx = BinSearchD(key, (double *)a5, sizeof(a5) / sizeof(key))))
                  printf("Key %f found at position %d in a5[]\n", key, idx);
            else  printf("Key %f not found in a5[]\n", key);
            if (-1 != (idx = BinSearchD(key, (double *)a6, sizeof(a6) / sizeof(key))))
                  printf("Key %f found at position %d in a6[]\n", key, idx);
            else  printf("Key %f not found in a6[]\n", key);
            if (-1 != (idx = BinSearchD(key, (double *)a7, sizeof(a7) / sizeof(key))))
                  printf("Key %f found at position %d in a7[]\n", key, idx);
            else  printf("Key %f not found in a7[]\n", key);
      }
      
      return EXIT_SUCCESS;
}

#else /* C++ */

#include <iostream.h>

main(int argc, char *argv[])
{
      double a1[] = { 1, 3, 17, 39, 66, 67, 88, 111, 222, 333, 345 };
      double a2[] = { 1, 3, 17, 39, 66, 67, 88, 111, 222, 333, 345, 678};
      double a3[] = { 5, 9, 37 };
      double a4[] = { 5, 9, 13 };
      double a5[] = { 5, 9 };
      double a6[] = { 6 };
      double a7[] = { -20, 1, 30 };
      double key;
      int    idx;

      while (--argc)
      {
            key = atof(*(++argv));
            if (-1 != (idx = BinSearch(key, a1, sizeof(a1) / sizeof(key))))
                  cout << "Key " << key << " found at position " << idx << " in a1[]" << endl;
            else  cout << "Key " << key << " not found in a1[]" << endl;
            if (-1 != (idx = BinSearch(key, a2, sizeof(a2) / sizeof(key))))
                  cout << "Key " << key << " found at position " << idx << " in a2[]" << endl;
            else  cout << "Key " << key << " not found in a2[]" << endl;
            if (-1 != (idx = BinSearch(key, a3, sizeof(a3) / sizeof(key))))
                  cout << "Key " << key << " found at position " << idx << " in a3[]" << endl;
            else  cout << "Key " << key << " not found in a3[]" << endl;
            if (-1 != (idx = BinSearch(key, a4, sizeof(a4) / sizeof(key))))
                  cout << "Key " << key << " found at position " << idx << " in a4[]" << endl;
            else  cout << "Key " << key << " not found in a4[]" << endl;
            if (-1 != (idx = BinSearch(key, a5, sizeof(a5) / sizeof(key))))
                  cout << "Key " << key << " found at position " << idx << " in a5[]" << endl;
            else  cout << "Key " << key << " not found in a5[]" << endl;
            if (-1 != (idx = BinSearch(key, a6, sizeof(a6) / sizeof(key))))
                  cout << "Key " << key << " found at position " << idx << " in a6[]" << endl;
            else  cout << "Key " << key << " not found in a6[]" << endl;
            if (-1 != (idx = BinSearch(key, a7, sizeof(a7) / sizeof(key))))
                  cout << "Key " << key << " found at position " << idx << " in a7[]" << endl;
            else  cout << "Key " << key << " not found in a7[]" << endl;
      }

      return EXIT_SUCCESS;
}

#endif /* C/C++ */

#endif /* TEST */
