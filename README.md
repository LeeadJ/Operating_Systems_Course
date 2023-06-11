# Operating Systems Course

This project consists of several tasks related to computer science concepts and programming. Each task has its own README file with detailed instructions and explanations. This general README serves as a summary of the tasks and their respective README files.

## Task 1: GDB Basics

### Description

Task 1 involves generating a secret key based on personal information and submitting it for a job application. A key generator program is provided, and the task is to add oneself to the list of talented individuals and generate a key.

### Files

- `secret`: Key generator program
- `decrypt`: Test tool for decrypting the generated key

## Task 2: Basic Shell

### Description

Task 2 focuses on Linux practice and consists of three parts: 
file operations, dynamic libraries, and basic shell implementation. The task involves implementing two small programs, `cmp` and `copy`, and coding libraries for encoding and decoding text. It also requires creating a shell program named `stshell`.

### Files

- `cmp`: Program for comparing two files
- `copy`: Program for copying a file
- `codecA.so`: Shared library for encoding and decoding text using Method A
- `codecB.so`: Shared library for encoding and decoding text using Method B
- `encode`: Tool for encoding text using the selected library
- `decode`: Tool for decoding text using the selected library
- `stshell`: Shell program with various features

## Task 3: IPC

### Description

Task 3 involves implementing a chat tool that supports communication over a network and includes a performance testing utility.
The chat tool allows sending and receiving messages between clients, while the performance testing utility measures the time it takes to transmit data using different communication styles.

### Files

- `stnc`: Network communication tool for chat functionality and performance testing

## Task 4: Design Pattern 1

### Description

Task 4 requires implementing a chat system supporting an unlimited number of clients using the reactor design pattern. 
The task involves creating a reactor library that handles file descriptors and functions for handling events. The API for the reactor
library includes functions for creating, starting, and stopping the reactor, as well as adding file descriptors to be monitored.

### Files

- `st_reactor.so`: Reactor library implementing the reactor design pattern

Please refer to the individual README files for each task for more detailed information on how to run and use the programs and libraries.

## Task 5: Design Pattern 2

This repository contains an implementation of a multi-threaded pipeline using active objects and a queue. The pipeline processes a series of numbers, performs calculations, and checks for prime numbers in a parallel manner.

### Overview
The pipeline consists of four stages, each implemented as an Active Object (AO) with specific responsibilities. The stages are as follows:

- Stage A: Prime Number Checking
- Stage B: Multi-Threaded Queue
- Stage C: Active Object Implementation
- Stage D: Pipeline Tasks

## Installation

Clone the repository:

```bash
git clone https://github.com/your/repository.git

Navigate to the project directory:

`cd project-directory`

Follow the instructions provided in the individual README files for each task to compile and run the programs and libraries.
```

This general README provides an overview of the project and its tasks. For more specific details on each task, please refer to the respective README files provided.




