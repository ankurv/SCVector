ABOUT:

Lot of systems generate large volume (billions) of increasing numbers. A normal vector can not hold them in memory. Even if it does memory soon gets fragmented enough to get that big continuous chunk of memory. This data structure aims to solve that.

Insertion: O(1)
Lookup   : O(n) 
Deletion : Not Supported


INSPIRATION/USE CASE:

In a nosql database the search in implemented using reverse indexes.
Reverse indexes are a list of sorted docIds against each search term.

In a really big database having billions of rowse, there can 100s of millions of items against each term. Only good thing is that they are sorted so we can do some magic to store and operate on them.

If a database has lets says around a billion records.
A query like *.txt hits on 100 million docIds, to hold these 100 million 8 bytes docIds in memory would require 800MB in a typical vector.
 It would be tough to allocate a 800MB continous chunk of memory from system. 

Its necessary to chunk these numbers into smaller manageable segments which system can possibly. To further reduce the memory presure they can be stored in delta compressed form. Typical memory requirement go down to around 1/6 times with this approach.


TODO:
Improve random access time for both forward and reverse iterations
   - Can be done using skip lists, but will add a lot of overhead
   
