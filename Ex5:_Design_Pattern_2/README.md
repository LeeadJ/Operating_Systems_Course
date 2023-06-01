# st_pipeline

This repository contains an implementation of a multi-threaded pipeline using active objects and a queue. The pipeline processes a series of numbers, performs calculations, and checks for prime numbers in a parallel manner.

## Overview

The pipeline consists of four stages, each implemented as an Active Object (AO) with specific responsibilities. The stages are as follows:

1. **Stage A: Prime Number Checking**
   - File: `is_prime.h`, `is_prime.c`
   - This stage provides a function `isPrime` that checks whether a given unsigned integer is a prime number.
   - The function returns `true` if the number is prime and `false` otherwise.

2. **Stage B: Multi-Threaded Queue**
   - Files: `queue.h`, `queue.c`
   - This stage implements a multi-threaded queue with Mutex lock protection.
   - The queue is designed to hold `void*` elements and allows waiting for a member without busy waiting, utilizing condition variables for synchronization.

3. **Stage C: Active Object Implementation**
   - Files: `activeObject.h`, `activeObject.c`
   - This stage provides an implementation of an Active Object (AO) with the following functions:
     - `CreateActiveObject`: Binds and runs an active object thread. The function selects a queue and receives a pointer to the function that will be called for each element in the queue.
     - `getQueue`: Returns a pointer to the queue of the active object. This function can be used to insert a member into the queue.
     - `stop`: Stops the active object and releases all memory structures of the object.

4. **Stage D: Pipeline Tasks**
   - Files: `st_pipeline.h`, `st_pipeline.c`
   - This stage orchestrates the pipeline tasks and sets up the active objects.
   - The pipeline performs a series of calculations and checks on the input numbers and displays the results.

## Usage

1. Clone the repository or download the source code files.

2. Compile the code using the provided makefile by running the following command in the terminal: `make`

3. Run the compiled program by executing the following command:
    - ./st_pipeline <num_tasks> [seed]
    - `<num_tasks>`: The number of tasks to be processed in the pipeline.
    - `[seed]` (optional): The seed value for random number generation. If not provided, a default seed will be used.

4. The program will execute the pipeline tasks and display the output.

## Pipeline Description

The pipeline performs a series of calculations and prime number checks on the input numbers. It consists of four stages executed in parallel:

1. **Stage A: Random Number Generation**
- The first active object generates `N` random six-digit numbers using the provided seed value (or a default seed if not provided).
- Each generated number is transferred to the next active object with a time delay of one thousandth of a second.

2. **Stage B: Print and Add 11**
- The second active object receives a number from the previous stage and performs the following tasks:
  - Prints the received number.
  - Checks if the number is prime using the `isPrime` function.
  - Prints `true` if the number is prime, or `false` otherwise.
  - Adds 11 to the number and transfers it to the next active object.

3. **Stage C: Print and Subtract 13**
- The third active object receives a number from the previous stage and performs the following tasks:
  - Prints the received number.
  - Checks if the number is prime using the `isPrime` function.
  - Prints `true` if the number is prime, or `false` otherwise.
  - Subtracts 13 from the number and transfers it to the next active object.

4. **Stage D: Print and Add 2**
- The fourth active object receives a number from the previous stage and performs the following tasks:
  - Prints the received number.
  - Adds 2 to the number and prints the new number.
  - Transfers the number back to the first active object in a cyclic manner.
  - The last number printed should be equal to the first number generated if implemented correctly.

## Dependencies

The code requires a C compiler, the pthread library for thread synchronization, and the math library for the `sqrt` function.

--- 

## Running Example

To demonstrate the functionality of the program, let's consider an example execution with two tasks.

![image](https://github.com/LeeadJ/Operating_Systems_Course/assets/77110578/4e8be0bb-2daf-41ad-a5bf-9867b919a1d0)


Explanation:

1. The first Active Object (AO) initializes the random generator with the given seed (or a generated seed if not provided) and generates two 6-digit random numbers: 325627 and 325638.
2. The first AO transfers the first number, 325627, to the second AO after a brief delay.
3. The second AO receives the number, prints it, checks if it is prime (which it is), and prints "true".
4. The second AO adds 11 to the number, resulting in 325638, and transfers it to the third AO after a delay.
5. The third AO receives the number, prints it, checks if it is prime (which it is not), and prints "false".
6. The third AO subtracts 13 from the number, resulting in 325625, and transfers it to the fourth AO after a delay.
7. The fourth AO receives the number, prints it, adds 2 to it, resulting in 325627, and prints the new number.
8. The fourth AO transfers the modified number back to the first AO after a delay.
9. The first AO receives the modified number, prints it, checks if it is prime (which it is), and prints "true".
10. The program completes execution after processing two tasks.

Please note that the output may vary due to the random number generation, but the sequence of operations and transformations will remain the same.

This example demonstrates the pipeline flow and the interaction between the Active Objects in the program.


