William Lopez
12/6/2023

CS-361L
University Of New Mexico

CS361 Programming Project

The project goal was to implement the random pivot quicksort, quad heapsort, and 3-way mergesort.
Then to benchmark and record the runtime of each based on various input sizes.

When starting the program there are four quickstart arrays, which the user can select from.
These arrays were used to benchmark the sorting algorithms. First is an array of random double
precision floating point numbers. Second is an array of random integers between 0 and 100,
making where duplicates very likely. Third is an array of random integers. Fourth is a reverse
sorted array. The user is asked for the size of the array and if they want it printed after each
sort. Very large arrays will take a while to print and generate but this is not counted in the runtime
of each sorting algorithm. They will then be told how each sorting algorithm did.

A custom array from user input can be given. Parameters and values can be given one at a time to the program
or copy and paste directly into the command line. Within the zip are several files that can be copied and pasted
into the command line at the start of the program.

Example of input for custom array:
c    -- For selecting the custom array option
100    -- Size of the array
0    -- If values are doubles or integers. 0 for integers, 1 for doubles.
1     -- Values to be placed into the array. The program expects numbers equal
2   	to the size of the array.
3
..
...
100