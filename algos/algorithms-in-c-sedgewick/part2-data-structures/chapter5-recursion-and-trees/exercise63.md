Exercise 5.63: Ordered trees are equivalent to balanced strings of
parentheses: An ordered tree either is null or is a sequence of ordered
trees enclosed in parentheses. Draw the ordered tree that correspond to
the string

```
( ( ( ) ( ( ) ( ) ) ( ) ) ( ( ) ( ) ( ) ) )
```

Let's give numbers to each parenthesis.
```
1( 2( 3( ) 4( 5( ) 6( ) ) 7( ) ) 8( 9( ) 10( ) 11( ) ) )
```

Then we can make a tree as

```
            1
         /    \
       2        8
     / | \    / | \
    3  4  7  9  10  11
      / \
     5   6
```
