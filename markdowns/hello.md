# Hello DPC++

__Data Parallel C++__ or DPC++ is an `open`, `standards-based` and `cross-architecture` language for __heterogeneous computing__.

`DPC++` = `C++` + `SYCL` + `community extensions`

The extensions aim to __simplify programming__ and __enhance performance__ by enabling programming to hardware specific features

## DPC++ Demo

The DPC++ example shown below will allocate memory for an array which is initialized to some values, next the task of doubling the array values is offloaded to a device and then finally the result is printed out on the host.

Input array is inialized to: `0 1 2 3 4 5 6 7`

Expected output after computation on device: `0 2 4 6 8 10 12 14`

@[hello.cpp]({"stubs": ["hello.cpp"],"command": "sh /project/target/run_hello.sh"})

The sections below shows how to write a simple Data Parallel C++ (DPC++) program.

## Include Header File

DPC++ program will have to include the SYCL header file and optionally specify namespace

```cpp
#include <CL/sycl.hpp>

using namespace sycl;
```

## Setup a SYCL queue and target device

To execute a task on device we have to setup a SYCL queue:

```cpp
queue q;
```

The code above will select the default device which will pick the best device available on the system. You can also pick a specific device to offload computation, for example GPU:

```cpp
queue q(gpu_selector{});
```

You can target computation to any desired device, you can use `cpu_selector`, `gpu_selector`, `accelerator_selector` or `intel::fpga_selector` depending on your application.

## Setup Memory that can be accessed by target device

DPC++ introduces __Unified Shared Memory__ (USM), which will allow you to allocate memory that can be modified by both host and device, and also supports implicit or explicit way of moving memory between host and device.

```cpp
int *data = malloc_shared<int>(N, q);
```

`malloc_shared` will allocate memory that can be accessed on host and device, and implicitly moves the memory between host and device.

You can also use concept of SYCL __Buffers__ & __Accessors__ to manage memory on target device, this is explained in one of the other lessons, and more details about __Unified Shared Memory__ is also in future lesson.

## Offload compute task to target device

`q.parallel_for` will submit the task to the device associated with the queue, which will compute and modify the memory on device. The function is performed on `N` items. This function that executes on the device for each item is also to as `kernel` function or `lambda` function. We will learn more about how kernel function works in later lessons. 
```cpp
  q.parallel_for(range<1>(N), [=] (id<1> i){
    data[i] *= 2;
  }).wait();
```
`.wait()` will ensure that the computation is complete before proceeding further to print the output on host.

Since the memory is allocated using `malloc_shared`, the memory is implicitly moved to device before computation on device and is copied back to host after computation is complete. The __Unified Shared Memory__ lesson will explore more about Unified Shared Memory and operation.

## Compiling DPC++ code

To compile a DPC++ program, initialize environment variables and then use `dpcpp` to compile as shown below:
```
source /opt/intel/inteloneapi/setvars.sh

dpcpp hello.cpp
```

@[Check DPC++ Compiler Version]({"command": "sh /project/target/run_dpcpp_version.sh"})

## Summary

From the example you can notice that DPC++ uses familiar __C++__ constructs and __SYCL__ classes to offload computation to heterogeneous devices, and uses new DPC++ extension __"Unified Shared Memory"__ to simplify memory management on heterogeneous devices.

## Resources

[Data Parallel C++ Reference](https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/dpc-compiler.html)

[Data Parallel C++ Specification](https://spec.oneapi.com/versions/latest/elements/dpcpp/source/index.html)


