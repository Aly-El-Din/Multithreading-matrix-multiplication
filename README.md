# Multithreading matrix multiplication in Linux

By: Aly El Din Mohamed El Sayed

Junior CSED student.

# Introduction

All of computer science and engineers must have exposed to a matrix multiplication problem, either as mathematically or a as a coding one. Certainly the size of the matrix impacts the size and time of solving the problem, so large-sized matrices could lead to substantial slowness in algorithm running time, hence the traditional method could not be used to solve such problem.

Being aware of threads and how they work and communicate is so crucial to understand how we would implement the solution, and this link that might help you:

[Thread in Operating System - GeeksforGeeks](https://www.geeksforgeeks.org/thread-in-operating-system/)

# Algorithm

There are 2 approaches to tackle this problem, first is to represent each row of Mat1 as an independent thread to be multiplied to all columns of Mat2, second is to represent each element of Mat1 as an independent thread to be multiplied to all mapped elements of Mat2.


