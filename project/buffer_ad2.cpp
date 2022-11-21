#include <CL/sycl.hpp>
using namespace sycl;

static const int N = 4;

int main(){
  queue q;
  std::cout << "Device: " << q.get_device().get_info<info::device::name>() << std::endl;

  std::vector<int> v1(N, 10);
  std::vector<int> v2(N, 10);

  buffer<int, 1> buf1(v1.data(), v1.size());
  buffer<int, 1> buf2(v2.data(), v2.size());

  q.submit([&] (handler &h){
    auto A = buf1.get_access<access::mode::read_write>(h);
    h.parallel_for(range<1>(N), [=](id<1> i){
      A[i] += 2;
    });
  });
  q.submit([&] (handler &h){
    auto A = buf2.get_access<access::mode::read_write>(h);
    h.parallel_for(range<1>(N), [=](id<1> i){
      A[i] += 3;
    });
  });
  q.submit([&] (handler &h){
    auto A = buf1.get_access<access::mode::read_write>(h);
    auto B = buf2.get_access<access::mode::read>(h);
    h.parallel_for(range<1>(N), [=](id<1> i){ // <--- waits for first 2 kernel execution to complete 
      A[i] += B[i];
    });
  }).wait();
  
  for(int i=0; i<N; i++) std::cout << v1[i] << std::endl;
  return 0;
}
