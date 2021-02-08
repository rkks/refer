Exercise 4.34: An uppercase letter means _put_ at the
beginning, a lowercase letter means _put_ at the end, a
plus sign means _get_ from the beginning, and an asterisk
means _get_ from the end in the sequence

```
E A s + Y + Q U E * * + s t + * + I O * n + + *
```

Give the sequence of values returned by the _get_
operations when this sequence of operations is performed on
an initially empty deque.

```
E  | E
A  | A E
s  | A E s
+ A| E s
Y  | Y E s
+ Y| E s
Q  | Q E s
U  | U Q E s
E  | E U Q E s
* s| E U Q E
* E| E U Q
+ E| U Q
s  | U Q s
t  | U Q s t
+ U| Q s t
* t| Q s
+ Q| s
I  | I s
O  | O I s
* s| O I
n  | O I n
+ O| I n
+ I| n
* n|
```
