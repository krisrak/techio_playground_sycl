# Offloading Computation

The sections below explains how computations can be offloaded to heterogeneous devices using kernel functions and different approches of expressing parallelism.

## Types of Kernel invocations

The Kernel function can be invoked using one of the following method:

- Single Task
- Basic Parallel Kernel
- ND-Range Kernel
- Hierarchical Kernel

## Single Task

`single_task` method is used to __invoke a kernel function once__ on the target device. 

The code below uses `single_task` method to double each value of the array. The array is initialized to values `0 1 2 3`, and the kernel function will iterate through the array using a `for-loop` and double each value, and the result is printed on the host.

@[Single Task]({"stubs": ["single_task.cpp"],"command": "sh /project/target/run_single_task.sh"})

## Basic Parallel Kernel

`parallel_for` method is used to __invoke a kernel function on each iteration__ of the task. The `parallel_for` method is used to specify the global range and the kernel function is invoked for every work-item using the `id` or `item` class.

The code below uses `parallel_for` method to double each value of the array. The array is initialized to values `0 1 2 3`, and the kernel function is invoked for each item which will double the value. 

@[Basic Parallel Kernel]({"stubs": ["basic_parallel_kernel.cpp"],"command": "sh /project/target/run_basic_parallel_kernel.sh"})

## ND-Range Kernel

ND-Range kernel allows programmers who are used to programming in __OpenCL__ to continue using similar concepts and features for offloading executions.

ND-Range Kernels are similar to Basic Parallel Kernel where the kernel function is invoked on each of the iterations of the task, but allows __mapping executions to hardware__ resources to achieve better performance optimization. For example, a GPU hardware consists of a bunch of _compute units_ which has its own _local memory_. Grouping executions so that each groups executes on a single __Compute Unit__ can be useful to optimize for better occupancy on the hardware. The grouping is called `work-group` and the individual instance of kernel function is called `work-item`. Each group can also make use of __Local Memory__ associated this these compute units in the hardware for faster access, rather than repeated access to global memory. 

DPC++ also introduces the concept of __Sub-Groups__, which is a subset of work-items within a work-group that may map to vector hardware, work-items within the sub-group __execute simultaneously__ and may help in achieving higher performance. You can learn more about using __Local Memory__ and __Sub-Groups__ in future lessons. 

The ND-Range kernel is invoked using the `parallel_for` method by specifying global size of iterations and work-group size using the `nd_range` class and the `nd_item` class is used to get the index and other properties related to the work-item.

```cpp
h.parallel_for(nd_range<2>{global_size, work_group_size}, [=](nd_item<2> item){

});
```

The code below uses `parallel_for` method to double each value of the array. The array is size is `256` and the `work-group` is set to `64` so that all `64` work-items are executed on the same compute unit in the hardware. We will learn more of different ways to optimize computation using ND-Range kernels in future lessons.

@[ND-Range Kernel]({"stubs": ["ndrange_kernel.cpp"],"command": "sh /project/target/run_ndrange_kernel.sh"})

## Hierarchical Kernel

Hierarchical Kernel an alternative to ND-Range kernel way of expressing parallelism. Hierarchical Kernel provides an more __hierarchical syntax__ with a loop for executing on each work-group and an inner loop that executes on each work-item. 

Hierarchical Kernel aims to provide a more structures way to express parallel kernels with all the capabilities of optimizations of using local memory and other features that are possible in ND-Range kernels. This alternative may suit better for programmers not familiar to OpenCL programming.

`parallel_for_work_group` method is used to invoke a kernel function for every work-group, and the inner loop `parallel_for_work_item` is invoked for every work-item in the work-group.
```cpp
h.parallel_for_work_group(num_groups, work_group_size, [=](group<1> g) {
    group.parallel_for_work_item([&](h_item<1> item) {

    });
});
```
The code below uses `parallel_for_work_group` and `parallel_for_work_item` methods to double each value of the array. The array is size is `256` and the `work-group` is set to `64` so that all `64` work-items are executed on the same compute unit in the hardware. We will learn more of different ways to optimize computation using hierarchical kernels in future lessons. 

@[Hierarchical Kernel]({"stubs": ["hierarchical_kernel.cpp"],"command": "sh /project/target/run_hierarchical_kernel.sh"})

## Simplifications in DPC++

All of the above methods of expressing parallelism is defined in the SYCL specification. __DPC++ simplifies the language__ by removing some verbosity in programming using these methods.

Some of the simplifications are making lambda name optional for kernels and simplification of one-dimentional range declaration as shown below:

#### Kernel invocation in SYCL:
```cpp
h.parallel_for<class kernelA>(range<1>(N), [=](id<1> i){ ... });
``` 
#### Simplified Kernel invocation in DPC++:
```cpp
h.parallel_for(N, [=](auto i){ ... });
```

We have learned about using Buffers and USM for memory management. DPC++ also simplfies kernel invocation when using USM by directly calling `q.parallel_for` and removing need for using `q.submit` to wrap the kernel invocation.

#### Kernel invocation for buffers:
```cpp
q.submit([&] (handler &h){
    auto A = buf.get_access<access::mode::read_write>(h);
    h.parallel_for(range<1>(N), [=](id<1> i){
        A[i] *= 2;
    });
});
```
#### Kernel invocation for USM:
```cpp
q.parallel_for(range<1>(N), [=] (id<1> i){
    data[i] *= 2;
});
```

### Resources

[Data Parallel C++ Reference](https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/dpc-compiler.html)

[Data Parallel C++ Specification](https://spec.oneapi.com/versions/latest/elements/dpcpp/source/index.html)


