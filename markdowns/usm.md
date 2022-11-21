# Unified Shared Memory

The sections below introduces Unified Shared Memory (USM) which can be used to manage memory using pointer-based approach to memory management.

The code below shows how USM is initialized

```cpp
int *data = malloc_shared<int>(N, q);
```

You can also initialize USM in a more familiar c/c++ `malloc` style declaration.

```cpp
int *data = static_cast<int *>(malloc_shared(N * sizeof(int), q));
```

Once the USM is allocated, the same `data` object can be accessed on host aswell as device code. The code below shows how USM allocation can be modified on device and how the memory is synchronized after computation using `wait()` method.

```cpp
q.parallel_for(range<1>(N), [=] (id<1> i){
  data[i] *= 2;
}).wait();
```

USM allocations must be deallocated using `free` with `queue` as a parameter.

```cpp
free(data, q);
```

## Types of USM

USM supports implicit and explicit methods for data movement.

| Type | function call | Description | Accessible on Host | Accessible on Device |
|:---|:---|:---|:---:|:---:|
| Device Allocation | `malloc_device` | Allocation on device, __explicit__ data movement | NO | YES |
| Host Allocation | `malloc_host` |Allocation on host, __implicit__ data movement | YES | YES |
| Shared Allocation | `malloc_shared` | Allocation can migrate between host and device, __implicit__ data movement | YES | YES |

### Implicit Data Movement using USM

The code below uses `malloc_shared` to allocate memory that can be accessed on host or device by referencing the same pointer object and the data movement is implicitly handled bycompiler implimentation.

@[USM Implicit Data Movement]({"stubs": ["usm_implicit.cpp"],"command": "sh /project/target/run_usm_implicit.sh"})

Notice that in the above code, all we do is allocate memory using `malloc_shared`, all the data movement is done implicitly.

### Explicit Data Movement using USM

The code below uses `malloc_device` to allocate memory on the device and the data movement is explicitly moved using `memcpy` method. This will offer more controlled data movement to tune applications for desired performance.

@[USM Explicit Data Movement]({"stubs": ["usm_explicit.cpp"],"command": "sh /project/target/run_usm_explicit.sh"})

Notice that we have used a new `queue` property called `in_order()` queue property, this is a new extension in DPC++ which make sure that the 3 tasks will execute sequentially, without this, the tasks executions can overlap by default.

```cpp
queue q(property::queue::in_order{});
```
The 3 tasks are:
- `q.memcpy` to copy memory from host to device
- `q.parallel_for` is the kernel function to do computation on device
- `q.memcpy` to copy back memory from device to host

The `q.wait()` will wait for the final task to complete before host can print the result.

As you can see that handling data dependency in kernel executions are very important when using USM, the next few sections explains other ways to handle data dependency.

## Data Dependency with Kernel Executions

Data dependency can be handled using one or more of the following methods:
- `wait()` on queue
- `in_order` queue property to force all the kernels to execute sequentially
- `depends_on()` method to specify events it has to wait on before execution
- specify events in `parallel_for` as an argument

Lets look an example for each of the above methods to handle data dependency when using USM 

### wait on queue

In the example below the initial value of data is `10`, the first kernel adds `2` and second kernel add `3`, since the first kernel has a `wait()` on queue, the execution does not overlap. The final result is `15`.

_NOTE: This method works, but `q.wait()` will block execution on host, so you will have to make sure using this does not affect overall performance of the application._

@[USM Data Dependency with wait()]({"stubs": ["usm_wait.cpp"],"command": "sh /project/target/run_usm_wait.sh"})

### `in_order` queue property

In the example below the initial value of data is `10`, the first kernel adds `2` and second kernel add `3`, since the queue property `in_order` is set, the execution does not overlap. The final result is `15`.

_NOTE: This method works, but using `in_order` queue property will not overlap kernel executions even if there is no dependency and may affect overall performance, so best to use this method if you know all kernels associated with the queue have data dependency._

@[USM Data Dependency with in_order queue property]({"stubs": ["usm_inorder.cpp"],"command": "sh /project/target/run_usm_inorder.sh"})

### `depends_on()` method

In the example below, there are 2 arrary initiaized to value `10`, the first kernel adds `2` to `d1` and second kernel add `3` to `d2`, since both kernels are using different allocations, the execution does may overlap. But the final kernel has dependency on both `d1` and `d2`, the execution of this kernel is made to wait until the first and second kernels are complete by using `h.depends_on({e1, e2})`, which will give a result of `25`. Try it out:

@[USM Data Dependency with depends_on() method ]({"stubs": ["usm_dependson.cpp"],"command": "sh /project/target/run_usm_dependson.sh"})

### Specify task dependency in `parallel_for`

In the example below, there are 2 arrary initiaized to value `10`, the first kernel adds `2` to `d1` and second kernel add `3` to `d2`, since both kernels are using different allocations, the execution does may overlap. But the final kernel has dependency on both `d1` and `d2`, the execution of this kernel is made to wait until the first and second kernels are complete by specifying events as an argument `parallel_for`, which will give a result of `25`. Try it out.

This method is every similar to using `depends_on`, but takes away the verbosity and make the code simplified.

@[USM Data Dependency with events in parallel_for ]({"stubs": ["usm_event.cpp"],"command": "sh /project/target/run_usm_event.sh"})

From the above 4 examples there are many ways to handle data dependency in kernel executions each with its own downsides or simplicity, use the one that makes most sense.

### Resources

[Data Parallel C++ Reference](https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/dpc-compiler.html)

[Data Parallel C++ Specification](https://spec.oneapi.com/versions/latest/elements/dpcpp/source/index.html)


