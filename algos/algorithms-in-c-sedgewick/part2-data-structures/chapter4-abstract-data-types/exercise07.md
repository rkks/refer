**Exercise 4.7**: Using the conventions of Exercise 4.6, give a way to insert
asterisks in the sequence `E A S Y` so that the sequence of values returned by
the *pop* operations is 1. `E A S Y`; 2. `Y S A E`; 3. `A S Y E`; 4. `A Y E S`;
or, in each instance, prove that no such sequence exists.

1. `E * A * S * Y *`
2. `E A S Y * * * *`
3. `E A * S * Y * *`
4. `E A * S Y *` produces `A Y`, but the next *pop* yields `S`. So impossible.
