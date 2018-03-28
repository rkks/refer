This is a tracker for top X sites in last Y requests.

Create a tracker once, 

  tracker_t *t = tracker_new(Y, X);

Then, pass every hit to the tracker, 

  tracker_hit(t, uri, when)

The top sites in the cache can be enumerated as 
shown in the show_tracker_top example function.

Background
----------
The challenge with a top-hit tracker is that you want the algorithm
to run in a finite amount of space. In other words, you don't want to
keep a counter for every URL that ever gets hit. But the first time
you get a hit for a URL, you need to start counting, so you know if
its ever going to be in the top X sites. How to reconcile these two
opposing goals? The approach used here is to keep a fixed size list
of sites (that's the parameter Y, or "cache size"). When a URL hit
is recorded, its either in the cache (in which case, just increment
its count, and note if its crossed the threshold for the 'top X'); 
if it wasn't in cache, then throw out the oldest entry, and enter it.

Here, the top X is determined in terms of "absolute hits" since the 
URL has been in the cache. In order to avoid falling out of the cache
a site has to get hit often enough that Y other sites do not get hit
between two successive hits to it. So Y should be much larger than X.

Why the top list?
~~~~~~~~~~~~~~~~~
If the cache alone were maintained without the 'top list' it would
be possible to simply calculate the 'top X' by sorting the entire
Y items in the cache. However, the design goal of this library is
to never sort the entire cache of Y (because its large), so, at 
each hit we simply test whether it's crossed into the 'top X'. 
(If it has, we need to add it into the top X, and remove an entry 
that's fallen out).

Contiguous
~~~~~~~~~~
The cache of Y items is stored in one contiguous memory buffer.
There is no runtime reallocation. The goal is to run in constant
space and time.

Before the cache fills up, the program keeps track of unused 
slots in the cache, but once it fills up, it stays full forever,
and only the oldest item is deleted (to re-use it's slot) as needed.

How does oldest entry get thrown out of cache?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The members (structures) of the cache itself are entered into a hash
table. (The hash table runs "through them"; they are contiguously
stored in one buffer as noted before). Every time a hit is recorded,
the structure representing that URL is repositioned to the tail of
the hash table (this is done without moving the structure; by
deleting it from the hash table and re-adding it, it is moved to the
tail, but this is accomplished via pointer manipulation only). Thus
the whole hash is an LRU list where the oldest item is at the head.
When a new URL results in expunging the oldest item, it is done via
simply deleting the hash head and adding the new item. All this is
constant time.

Not just for URL's
~~~~~~~~~~~~~~~~~~
This example is based on URL's but this top-tracker library is really
for any 'labelled' object which have 'hits' (among a potentially infinite
universe of labelled objects). In other words, top words in a language,
would be just as applicable a use for the top tracker as URL's).
