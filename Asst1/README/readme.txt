PA1 README.TXT
____________________________________________________________________________

How to use:
Please run make on the makefile and then ./memgrind for results.
____________________________________________________________________________

In this case, we designed a malloc and free function using a static array that represented a block of memory from the global heap.  mymalloc in this case, used a metadata node that contained in it, the size of the block that was being allocated, the next pointer (linked list), and a simple integer that represents a boolean to see if the metadata was free or not.  mymalloc in this case, uses these nodes as a header to seperate the chunks of data that were allocated, therefore still providing contiguous blocks of memory like the real malloc does.  myfree also simulated the function free by simply changing a int variable (free) to let the computer know if this piece of data was okay to be used again, and calls upon a merge function to merge continuous blocks of free memory.  Inside these two functions, 2 subfunctions are used: FirstInitial() and Merge().
____________________________________________________________________________
mymalloc Function:
mymalloc is a function that simulates the real c function malloc, where it takes a size of int (x) and allocates memory from the global heap.  In our case, we are given a static char array of size 5000 to mimic such function.  We use memory for both storage of starting pointers to tell if blocks of memory are allocated in a node structure that contains the size, the next pointer, and the int state if its allocated or not.  mymalloc uses a first fit algorithm meaning the first block that can contain the requested size of (x) bytes is allocated to the user.  If this block exceeds such size, we simply use a cutting method to split the remaining amount of array memory to another node that has size of (remaining size - sizeof(node)) and it points to the next node.  This last part is efficient for the end case, since our last node is the remaining amount of storage in our array, and it can support the a split function quite easily.

FirstInitital Function:
FirstIntial function simply checks to see if our head node, which was initialized without its variables of free, size, and next pointer to be initialized.  If not, which means we ran mymalloc for the first time, sets our head node, which points to the first byte of memory in the memory array, to the size of the space of the rest of memory (5000 - sizeof(memNode)), a free integer of 1 (meaning it can be mymalloced), and pointed to nothing.  This subfunction is used only for mymalloc().

myfree Function:
myfree is a function that takes in a metadata node parameter, as well as a character file + an integer line for error checking.  myfree does a simple check to find the pointer as the metadata node to simply change the 0 free variable to a 1.  If the pointer parsed was never malloced, or exists in the middle of the allocated memory, it will not match the memory address of the metadata pointer in the linked list and therefore return an error.  Same goes if it does exist, but is already freed.  This function also calls upon a merge function that clumps free contiguous data together.


Merge Function:
The merge function is a function found in free that is used to "merge" 2 - 3 blocks of memory, if found.  Since merge is always being called in a case where it checks for a singular contiguous block of memory to be freed, after the freed pointer is also free, we can optimize this function clear of a while loop.  Instead of checking the whole list for a whole chain of contiguous memory, because merge is always called when there is contiguous memory (in this case 2 being the least and 3 being the most), merge will always make sure that after the call, and if free is not being called again, there will be no contiguous blocks of free memory in the array, which therefore helps malloc with the first fit algorithm.  This function works by simply adding the sizes of the metadata nodes to the leftmost node that is free and pointing that to the next occupied node. 

EXAMPLE
1 - 2 - 3 - 4 - 5 - NULL
(1 is freed)
merge checks, no free blocks that are contiguous.
(3 is freed)
merge checks, no free blocks that are contiguous.
(5 is freed)
merge checks, no free blocks that are contiguous.
(2 is freed)
merge checks, 1 (previous pointer) is freed, so we add 2's datasize to leftmost header, and repeat with 3.
1 - 4 - 5 - NULL // Done with one's header node still being technically free.

Even if 4 was freed in this case, we will never end up with contiguous blocks of space being free because merge checks the previous and next pointer, resulting in if 4 was freed in this case, it would've merged with 3 first, resulting in a singular node.

1 - 2 - 3 - 4 - 5 - NULL
(1 is freed)
merge checks, no free blocks that are contiguous.
(3 is freed)
merge checks, no free blocks that are contiguous.
(4 is freed)
merge checks, 3 (previous pointer) is freed, so we add 4's datasize to leftmost header.
1 - 2 - 3 - 5 - NULL //Done with 3's header node being free

memgrind.c

Testcase A:
Successfully returned 100 iterations of all 150 pointers being malloced and freed at once.

Testcase B:
Successfully returned 100 iterations of mallocing an integer pointer and storing such an address in the array and freeing it after the dynamic malloc of 100.  Malloc and free both returned successfull cases of 1500 iterations.

Testcase C:
Using a random seed pool generator, we can either free a pointer of 1 byte or malloc a pointer of 1 byte.  In the end, each iteration must malloc a total of 150 bytes and free them all.  Malloc and free both came back with successfull cases of 1500 iterations each.

Testcase D:
Using a random seed pool generator twice, we randomly generated the same number as testcase c, but this time we also generate a byte from 1 - 64 bytes.  We do this 150 times, also keeping an eye on not overflowing the amount of memory we pass by the array.  At the end, after 150 calls of malloc + random calls on free, we free the rest of the pointers.  This testcase always returned, a random number, but the equal number of instances of mymalloc and myfree being run with no errors to both mymalloc and myfree.
____________________________________________________________________________
// NOTE THAT TESTCASE A - D IS SUPPOSED TO HAVE NO ERRORS.  E AND F I COMPILE A LIST TO SHOW MERGE'S AFFECT ON MERGING DATA AND THE ABILITY OF SPLIT IN MYMALLC //
____________________________________________________________________________

OUTPUT FOR TESTCASES:

Test A:
Total number of mallocs: 15000
Total number of frees: 15000
Average running time (seconds): 0.000002

Test B:
Total number of mallocs: 15000
Successful mallocs: 15000
Total number of frees: 15000
Average running time (seconds): 0.000043
 
Test C:
Successful mallocs: 15000
Total number of frees: 15000
Average runtime (seconds): 0.000006
 
Test D:
Successful mallocs: 10394
Total number of frees: 10394
Average runtime (seconds): 0.000006
 


