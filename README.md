This is an implementation of an ID library based on a hash table. Given a binary byte string of
arbitrary length, assuming an upper bound of 4K in length, it generates a 32-bit "ID" for the entry
that is consistent for all identical entries. The entry can later be queried based on the ID or
deleted so that the slot in the table can be re-used later. 

I used a hash table for this project because a hash table enables constant-time lookups and
insertions. To make the implementation as simple as possible, I elected to only store the input byte
string in the table and to use the index within the table as the ID. Unfortunately, I was unable to
implement a hash table that dynamically increases in size if it exceeds a given load factor because
the index would change when rehashing the data for the new table. This means the user must be sure
of the table length they choose when creating the table. 

To make the lookups and insertions as fast as possible, I elected to pre-allocate the entire table
with the maximum data length (4096). Although this can result in a substantial memory overhead,
especially for sparsely populated tables and tables with small input values, it saves the time of
allocating and freeing memory on each insertion and deletion.

The hash function I wrote is inspired by the [Wikipedia article on hash
functions](https://en.wikipedia.org/wiki/Hash_function#Hashing_variable-length_data) and based on
a hash function I found in the responses section of [this stackoverflow
page](https://stackoverflow.com/questions/8317508/hash-function-for-a-string). 

Since the data can be of arbitrary length and type, this library requires the user to provide the
length of the data whenever inserting a new item into the table. This is to prevent buffer overflows
from copying beyond the end of the buffer provided by the user if the input is less than the maximum
length of 4096 bytes. Furthermore, the user will have the cast the data to a `char*` type because
of how I implemented the hash function. 
I acknowledge that it was poor planning to build my hash function on the premise of working with a
string, and that I should have made the hash function itself account for varying data types,
possibly by taking a `void*` as its input argument and casting the data to another type if
necessary, such as an integer for a simpler hash function.

The test suite briefly checks whether the functions work as intended. Although I should have, I did
not have time to test whether the hash function can successfully try all possible slots, because it
is possible that it would result in a loop through a subset of the possible slots, meaning that it
may fail to insert data into a table that has an empty entry. 
As stated in the comment at the bottom of `test_id_library.c`, I could not find a way to confirm
whether `destroy_table()` worked correctly without using external tools such as `gdb` or `valgrind`
(both of which confirmed the function works properly in the test). 

**TO USE** 
Simply run `make` to build the library and the test suite. 
You can also run `make clean` to delete the executable for the test suite and the object files for
the test suite and the library. Lastly, you can use `make check` to run valgrind (if installed) to
confirm that there are no memory leaks.
