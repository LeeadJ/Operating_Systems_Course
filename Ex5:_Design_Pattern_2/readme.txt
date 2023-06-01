Ex5 Execution Explanation

The code consists of a multi-threaded pipeline that processes a series of numbers, performs calculations, and checks for prime numbers. 
It is divided into four stages implemented as active objects (AOs) using synchronized queues, mutex locks, and condition variables for thread safety and synchronization.
The pipeline execution begins with Stage A, where random six-digit numbers are generated. 
These numbers are then passed sequentially to Stage B, Stage C, and finally Stage D. 
At each stage, the numbers are processed, printed, checked for primality using the isPrime function, and modified as per the assignment instructions. 
The modified numbers are then passed to the next stage, forming a cyclic flow.

To run the program, compile the code using a C compiler and execute the compiled program with command-line arguments. 
The command syntax is as follows: ./st_pipeline <num_tasks> [seed]
where <num_tasks> represents the number of tasks to be processed in the pipeline, and [seed] (optional) denotes the seed value for random number generation. 
If no seed is provided, a default seed will be used.

Example:
./st_pipeline 2

Output:

325627
true
325638
false
325625
325627
188300
false
188311
true
188298
188300


The output displays the numbers, prime checks (true/false), and modified values at each stage. 
This example showcases the concurrent execution and proper functioning of the pipeline.