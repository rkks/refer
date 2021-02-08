Exercise 4.31: A letter means put and an asterisk means get in the
sequence

```
E A S * Y * Q U E * * * S T * * * I O * N * * *
```

Give the sequence of values returned by the get operations when
this seqence of operations is performed on an initially empty FIFO
queue.


```
E  | E
A  | E A
S  | E A S
* E| A S
Y  | A S Y
* A| S Y
Q  | S Y Q
U  | S Y Q U
E  | S Y Q U E
* S| Y Q U E
* Y| Q U E
* Q| U E
S  | U E S
T  | U E S T
* U| E S T
* E| S T
* S| T
I  | T I
O  | T I O
* T| I O
N  | I O N
* I| O N
* O| N
* N|
```
