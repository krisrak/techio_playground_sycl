# Buffers and Accessors

The sections below shows how SYCL Buffers can be used to manage memory when offloading computation to heterogeneous devices.

SYCL introduces to concepts of Buffers and Accessors to move, control and modify memory on device. 

## Buffers

Buffers encapsulate the data object that need to be managed on device.

Below are examples of how buffers are created for data objects that need to the used on device.

Pointer data:
```cpp
int *data = (int *)malloc(N * sizeof(int));

buffer<int, 1> buf(data, range<1>(N));
```

Vector data:
```cpp
std::vector v<int>(N);

buffer<int, 1> buf(v.data(), v.size());
```

## Accessors

Accessors are used to request control of buffer objects to access the memory.

The code below show how an accessor is defined to modify the buffer object on device code. Notice the `read_write` mode, which will give permission for device code to read and modify the data object.

```cpp
auto A = buf.get_access<access::mode::read_write>(h);
```

Once the accessor is defined, the kernel code can modify this accessor

```cpp
h.parallel_for(range<1>(N), [=](id<1> i){
  A[i] *= 2;
});
``` 

The code to create buffer and modify a data object on device will look like this:

```cpp
queue q;
int v = vector<int>(N);
buffer<int, 1> buf(v.data(), v.size());
q.submit([&] (handler &h){
  auto A = buf.get_access<access::mode::read_write>(h);
  h.parallel_for(range<1>(N), [=](id<1> i){
    A[i] += 1;
  });
});
```

## Memory Synchronization

Once the memory is moved and modified on device, it has to copied back to the host. This memory synchronization can be done various ways:
- Synchronize on Buffer Destruction
- Synchronize with Host Accessor

### Synchronize on Buffer Destruction

One way to synchronize memory is by destructing the buffer after computation is complete, this is accomplished by having a scope around the buffer creation and kernel function. The code below shows how memory is synchronized after computation on device:

@[Synchronize on Buffer Destruction]({"stubs": ["buffer_bd.cpp"],"command": "sh /project/target/run_buffer_bd.sh"})

### Synchronize with Host Accessor

Another way to synchronize memory is by using an host accessor on the host side which is a blocking call that will wait for the computation to complete with the associated buffer object and then the memory is copied back to the host. The code below shows how memory is synchronized using host accessor after computation on device:

@[Synchronize with Host Accessor]({"stubs": ["buffer_ha.cpp"],"command": "sh /project/target/run_buffer_ha.sh"})

## Data Dependency between Kernel Executions

Accessors create data dependency that can order kernel executions. If 2 kernels use the same buffer then the second kernel execution will wait for the first to complete. This is avery important feature of accessors and it will make sure that same data is not overwritten by two or more kernels at the same time. The next 2 code examples demonstrats this implicit data dependency functionality of accessors. 

In the example below the initial value of data is `10`, the first kernel adds `2` and second kernel add `3`, since both kernels are using the same buffer, the execution does not overlap. The final result is `15`

@[Data Dependency between Kernel Executions #1]({"stubs": ["buffer_ad.cpp"],"command": "sh /project/target/run_buffer_ad.sh"})

In the example below, there are 2 vectors initiaized to value `10`, the first kernel adds `2` to `v1` and second kernel add `3` to `v2`, since both kernels are using different buffers, the execution does may overlap. But the final kernel has dependency on both `v1` and `v2`, the execution of this kernel will wait until the first and second kernels are complete, which will give a result of `25`. Try it out:

@[Data Dependency between Kernel Executions #2]({"stubs": ["buffer_ad2.cpp"],"command": "sh /project/target/run_buffer_ad2.sh"})

### Resources

[Data Parallel C++ Reference](https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/dpc-compiler.html)

[Data Parallel C++ Specification](https://spec.oneapi.com/versions/latest/elements/dpcpp/source/index.html)


