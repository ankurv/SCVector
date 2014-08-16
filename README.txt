ABOUT:

Lot of systems generate large volume of increasing numbers. A normal vector can't not hold them in memory even if it does memory soon gets fragmented enough to get that big continuous chunk of memory. This data structure aims to solve that.

Insertion: O(1)
Lookup   : O(n) 
Deletion : Not Supported


INSPIRATION/USE CASE:

In a nosql database the search in implemented using reverse indexes.
Reverse indexes are a list of sorted docIds against each search term.

When there are 100s of millions of rows the number of items against each term keeps increaing. Only good thing is that they are sorted so we can do some magic to store and operate on them.

If a database has lets says around a billion recors and a query like *.txt hits on 100 million docIds, to hold these 100 millions 8 bytes docIds in memory would require 800MB in a typical vector. It would be tought to allocate a 800MB continous chunk of memory from system. 

So its necessary to chunk these numbers into smaller manageable segments which system can allow. To further reduce the memory presure they can be stored in delta compressed form. Typicall memory requirement go down to around 1/6 times of the usually with this approach.

TODO:

Find a feasible way to reverse iterate on this
