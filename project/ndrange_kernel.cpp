#include <CL/sycl.hpp>
using namespace sycl;

static const int N = 256;
static const int work_group_size = 64;

int main(){
  queue q;

  int *data = malloc_shared<int>(N, q);
  for(int i=0; i<N; i++) data[i] = i;

  q.parallel_for(nd_range<1>(N, work_group_size), [=] (nd_item<1> item){
    int i = item.get_global_id(0);
    data[i] *= 2;
  }).wait();

  for(int i=0; i<4; i++) std::cout << data[i] << std::endl;
  free(data, q);
  return 0;
}
