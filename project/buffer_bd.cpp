#include <CL/sycl.hpp>
using namespace sycl;

static const int N = 16;

int main(){
  queue q;
  std::cout << "Device: " << q.get_device().get_info<info::device::name>() << std::endl;

  std::vector<int> v(N);
  for(int i=0; i<N; i++) v[i] = i;

  {
    buffer<int, 1> buf(v.data(), v.size());
    q.submit([&] (handler &h){
      auto A = buf.get_access<access::mode::read_write>(h);
      h.parallel_for(range<1>(N), [=](id<1> i){
        A[i] *= 2;
      });
    });
  } // <--- scope will destruct the buffer and synchronize memory
  
  for(int i=0; i<N; i++) std::cout << v[i] << std::endl;
  return 0;
}
