# High Performance Computing

This project was made for the homonym core elective course in university.  
The teaching goal of this project was to build the basic step of the [PPDP  
algorithm](https://www-users.cs.umn.edu/~boley/publications/papers/PDDP.pdf) in a parallel execution implementation using the [CUDA Toolkit](https://developer.nvidia.com/cuda-toolkit).


## 0. Introduction

I was tasked to build 2 parallel implementations, one utilizing only the global memory of the GPU  
(naive implementation) & one utilitizing both global and shared memory, which is smaller in size  
and way faster in access times than the global memory (optimized implementation).  
To switch between implementations I used `#define Shared`  
(in source or as a linker flag in the makefile.)  
The Professor gave us remote access to a Linux x64 server with:  
* 1 CUDA GPU (Tesla C2075 - Comp. Capability 2.0)  
* 8 GB DDR3 RAM  
* 8 logical core cpu  
* Cuda Toolkit ver.6.0  

While I also had a local Windows 10 x64 system with:  
* 1 CUDA GPU (GeForce GTX 650 Ti - Comp. Capability 3.0)  
* 8 GB DDR3 RAM  
* Intel Core i7 4770 (8 logical cores)  
* Cuda Toolkit ver.9.0  

The minimum prerequisite to run the project is a x64  
system with Nvidia CUDA GPU of compute capability 2.0  
and Fermi Micro-Architecture, 4 GB RAM.  


## 1. Implementing the basic step of the PDDP algorithm

The algorithm takes input (from a dataset file) a M matrix `n x m`  
where `m >> n` which consists of double presicion numbers.   
The vector w is calculated as `w = 1/m * (M[:,1] + M[:,2] + ... + M[:,m])`  
where M[:,i] is the i-th column of M.  
The algorithm produces iteratively a `Xk` vector which is compared to  
the previously computed vector `Xk-1`.   
When the norm between them is lower than the epsilon constant  
(equal to `10^-6`) the algorithm has converged   
and the `Xk` vector is the result of the algorithm,  
an eigenvector based on which the input elements of M will be partitioned in 2 subsets.     
The pseudocode of the algorithm is shown below:  
![Pseudocode](https://github.com/NC0DER/AcademicProjects/blob/master/High%20Performance%20Computing/images/algorithm.jpg)  


## 2. Implementation Details

Before I made the parallel version of the algorithm,  
I timed various code segments that were executed most of the time,  
For the largest dataset provided, the norm calculation time   
took around `4ms` for each iteration of the `do..while()`,  
which is comparable to the time it took to launch a kernel function.    
The bottleneck of the algorithm is the Transposed Matrix Matrix Vector Multiplication.  

To simplify this operation:
 * The Matrix Vector is multiplied first and the resulting vector is multiplied  
   with the transposed matrix. 
 * I separated the 2 multiplications in 2 separate kernel functions, and got  
   better results in terms of execution speed.

To optimize kernel functions that contain `for` loops of specified length,   
I tried to execute the minimum dimension of the matrix inside the kernel   
wherever that was possible, to minify the work that each thread does.   
Spawning a large number of simpler threads wasn't an issue since most   
`CUDA` GPUs have a large number of cores.

I also used the `#pragma unroll` compiler directive which unrolls the loop  
in simple statements to further optimize the kernel function calls.

To print the execution time of each step of the iteration, or the entire duration of each,  
along with a few debug messages, in case of error, the debug flag must be defined in `main.cu` as 1.   
To switch between implementations `#define Shared` (in source or as a linker flag in the makefile) must be used.  


## 3. Implementation graphs

You can find the time performance graphs below:  
The Y-Axis shows the average execution time of every iteration for the 2 GPU's  
while the X-Axis shows the block size factor of each GPU thread.  
(e.g n = 16 => block size = 16 x 16 x 1).  

If you wish to access all graph images you can click
[this link](https://github.com/NC0DER/AcademicProjects/blob/master/High%20Performance%20Computing/images)  

For the largest dataset:   
![Tesla3](https://github.com/NC0DER/AcademicProjects/blob/master/High%20Performance%20Computing/images/tesla3.jpg)   
____  
![GTX3](https://github.com/NC0DER/AcademicProjects/blob/master/High%20Performance%20Computing/images/gtx3.jpg)  

It seems that the best execution times implementation-aside was for blocksize = 16 x 16 x 1.  
For this optimal block size the speedup for all 3 datasets is:  

**Tesla C2705**  
![Tesla](https://github.com/NC0DER/AcademicProjects/blob/master/High%20Performance%20Computing/images/tesla_speedup.jpg)  
____  
**GeForce GTX 650 Ti**   
![GTX](https://github.com/NC0DER/AcademicProjects/blob/master/High%20Performance%20Computing/images/gtx_speedup.jpg)  

## 4. Build && Run
`Windows`: I build using `VS2015 & CUDA 9.0`, you can use `VS2017 & CUDA 10.0`  
`Linux`: I build using the makefile in the following manner:  

### Build 
```bash
$ make fast  #Optimized implementation
nvcc main.cu pddp.cu wrappers.cu -o kernel_fast -arch=compute_20 -code=sm_20 -O3 --machine 64 -DSHARED
$ make slow  #Naive Implementation
nvcc main.cu pddp.cu wrappers.cu -o kernel_fast -arch=compute_20 -code=sm_20 -O3 --machine 64
```
**Warning: no make all rule provided**
### Run 
```bash
$ ./kernel_fast  #Same for kernel_slow
Syntax: ./<executable> <filename> <blocksize per dimension>
 *Note: Some kernels use the same blocksize for dimension X and Y
 *Note: The resulting blocksize is (e.g for input n) n x n x 1 for each dim (x, y, z)

**Note: Blocksize for each dimension X & Y much larger than 32
**Note: is not supported for cards with comp.capability < 6.0

$ ./kernel_fast dataset1.csv 16  #Same for kernel_slow
```