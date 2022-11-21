#include <CL/sycl.hpp>
using namespace sycl;

static const int N = 16;

int main(){
  queue q;
  std::cout << "Device: " << q.get_device().get_info<info::device::name>() << std::endl;

  int *data = (int *)malloc(N * sizeof(int));
  for(int i=0; i<N; i++) data[i] = i;

  buffer<int, 1> buf(data, range<1>(N));
  q.submit([&] (handler &h){
    auto A = buf.get_access<access::mode::read_write>(h);
    h.parallel_for(range<1>(N), [=](id<1> i){
      A[i] *= 2;
    });
  });
  buf.get_access<access::mode::read>();
  
  for(int i=0; i<N; i++) std::cout << data[i] << std::endl;
  free(data);
  return 0;
}
