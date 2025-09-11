CS24BTECH11020
Coding Assignment - 1
Final Report for all questions 
1. Re-Ordering a linked-List : 
Input : Comma seperated integers and -1 at the end to imply the end of input string.
-> Our code first creates a linkedList in the order if input 
Re-Ordering: all the even indexed terms are grouped at the start and then odd indexed terms are grouped 
example: 
Input:
2, 1, 3, 5, 6, 4, 7, -1
Output:
2, 3, 6, 7, 1, 5, 4

How did we solve!: 
we connected the linkedlist's even indexed terms with the next even indexed term and similarly connected odd indexed terms wit the next odd indexed terms and finally connected the last even term to the first odd indexed terms - this maintains at first the group of even indexed terms (of original LL) then it connects this to the group of odd indexed terms , resulting in what we need and finally printing them out - comma seperated.

how to run the code : g++ linkedListreOrdering.cpp -o linkedListreOrdering
                      ./linkedListreOrdering

2. Implementing doubly linked list using only one pointer x->np:
Input Format:

• First line will contain k, which indicates the number of operations to
be performed.
• Next k lines will contain the details of each operation as follows:

The input starts with an integer indicating the operation to be per-
formed, followed by the necessary values for that operation.

The operations are defined as follows:
– 1: Search for the value x in the list.
– 2: Insert the value x into the list.
– 3: Delete the value x from the list.
– 4: Reverse the list.
Output Format:
It consists of k lines corresponding to each k operations.
The output depends on the operation performed:
• For Search:
– Output: ’Element is present’ if the value is found.
– Output: ’Element is not present’ if the value is not found.
• For Insert:
– Output: The list after inserting the value, with the most recently
inserted element appearing first.
• For Delete:
– Output: The list after deleting the specified value.
– Output: If the deleting element is not present then print ’Element
is not present’.
• For Reverse:
– Output: The list after reversing it.
An example:
Input:
9
2 10 %Insert 10%
2 20
4 %Reverse%
1 20 %Search 20%
1 30
2 40
3 30 %Delete 30%
3 40
4
Output:

3

10
20 10
10 20
Element is present
Element is not present
40 10 20
Element is not present
10 20
20 10

how to run code: g++ dllOnePtr.cpp -o dllOnePtr
                 ./dllOnePtr
example run: 
9
2
10
Our dll till now : 
10 
2
20
Our dll till now : 
20 10 
4
Our dll till now : 
10 20 
1
20
Element is present
1
30
Element is not present
2
40
Our dll till now : 
40 10 20 
3
30
Element is not present
3
40
Our dll till now : 
10 20 
4
Our dll till now : 
20 10 

due to the nature of the code , while we are entering the input sequentially - the corresponding output is outputted there itself 
