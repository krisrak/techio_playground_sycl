# DPC++ Memory Management

The sections below shows different ways to manage memory when offloading computation to heterogeneous devices.

- Buffers and Accessors
- Unified Shared Memory (USM)

 __Buffers and Accessors__ are new concepts introduced in SYCL specification, it is powerful, handles data dependency elegantly and makes handling 1-2-3 dimentional data objects easy, but may make the code more verbose since it requires creating buffers for each data object and corresponding accessor need to be created to manage memory on device side.

__Unified Shared Memory__ uses a more familiar pointer-based approach to manage memory in heterogeneous programming and may be more easy to get started coming from C++ programming background. Code is also easy to follow since host and device side use the same data objects. Easy when dealing with pointer data objects.

Below are basic examples using both the concepts of memory management, the next couple lessons explains each in more details.

## Buffers and Accessors

SYCL specification introduces concepts of Buffers and Accessors to move, control and modify memory on device. Buffers encapsulate the data object that need to be managed on device and Accessors are used to request control of buffer objects to access or modify the memory.

@[buffer.cpp]({"stubs": ["buffer.cpp"],"command": "sh /project/target/run_buffer.sh"})

## Unified Shared Memory

USM is pointer-based approach to manage memory in DPC++ language.

@[usm.cpp]({"stubs": ["usm.cpp"],"command": "sh /project/target/run_usm.sh"})

## Summary

Both offer the same memory management functionality for heterogeneous programming, use which ever is easy and fits your application.

More details about __Buffers and Accessors__ and __Unified Shared Memory__ are in the next 2 lessons.

### Resources

[Data Parallel C++ Reference](https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/dpc-compiler.html)

[Data Parallel C++ Specification](https://spec.oneapi.com/versions/latest/elements/dpcpp/source/index.html)


