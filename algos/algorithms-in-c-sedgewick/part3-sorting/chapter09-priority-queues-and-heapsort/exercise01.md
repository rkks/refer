Exercise 9.1: A letter means _insert_ and an asterisk means _delete
the maximum_ in the sequence.

```
P R I O * R * * I * T * Y * * * Q U E * * * U * E
```

Give the sequence of values returned by the _delete the maximum_
operations.

```
P
P R
I P R
I O P R
I O P / R
I O P R / R
I O P / R R
I O / R R P
I I O / R R P
I I / R R P O
I I T / R R P O
I I / R R P O T
I I Y / R R P O T
I I / R R P O T Y
I / R R P O T Y I
/ R R P O T Y I I
U / R R P O T Y I I
/ R R P O T Y I I
E / R R P O T Y I I
```

Therefore the sequence returned is `R R P O T Y I I`.
