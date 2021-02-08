This table gives implementations of basic string-processing operations, using
two different C language primitives. The pointer approach leads to more compact
code, but the indexed-array approach is a more natural way to express the
algorithms and leads to code that is easier to understand. The pointer version
of the concatenate operation is the same as the indexed array version, and the
pointer version of prefixed compare is obtained from the normal compare in the
same way as for the indexed array version and is omitted. The implementations
all take time proportional to string length.

### Indexed array versions
* Compute string length (`strlen(a)`)
```c
for (i = 0; a[i] != 0; ++i) ; return i;
```
* Copy (`strcpy(a, b)`)
```c
for (i = 0; (a[i] = b[i]) != 0; ++i) ;
```
* Compare (`strcmp(a, b)`)
```c
for (i = 0; a[i] == b[i]; ++i)
  if (a[i] == 0) return 0;
return a[i] - b[i];
```
* Compare (prefix) (`strncmp(a, b, strlen(a))`)
```c
for (i = 0; a[i] == b[i]; ++i)
  if (a[i] == 0) return 0;
if (a[i] == 0) return 0;
return a[i] - b[i];
```
* Append (`strcat(a, b)`)
```c
strcpy(a+strlen(a), b)
```

### Equivalent pointer versios
* Compute string length (`strlen(a)`)
```c
b = a; while (*b++) ; return b-a-1;
```
* Copy (`strcpy(a, b)`)
```c
while (*a++ = *b++) ;
```
* Compare (`strcmp(a, b)`)
```c
while (*a++ == *b++)
  if (*(a-1) == 0) return 0;
return *(a-1) - *(b-1);
```
