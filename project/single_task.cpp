#include <CL/sycl.hpp>
using namespace sycl;

static const int N = 4;

int main(){
  queue q;

  int *data = malloc_shared<int>(N, q);
  for(int i=0; i<N; i++) data[i] = i;

  q.single_task([=](){
    for(int i=0;i<N;i++){
      data[i] *= 2;
    }
  }).wait();

  for(int i=0; i<N; i++) std::cout << data[i] << std::endl;
  free(data, q);
  return 0;
}
