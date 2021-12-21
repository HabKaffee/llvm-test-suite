// REQUIRES: opencl-aot, cpu

// RUN: %clangxx -fsycl -fsycl-targets=spir64_x86_64 %S/Inputs/common.cpp -o %t.out \
// RUN:          -fsycl-dead-args-optimization
// RUN: %CPU_RUN_PLACEHOLDER %t.out | FileCheck %t.out

#include <cmath>
#include <sycl/sycl.hpp>

class Kernel1Name;

const static sycl::specialization_id<int> SpecConst2{42};

int main() {
  sycl::queue Q;
  Q.submit([&](sycl::handler &CGH) {
    CGH.set_specialization_constant<SpecConst2>(1);
    CGH.single_task<class Kernel1Name>([=](sycl::kernel_handler KH) {
      (void)KH.get_specialization_constant<SpecConst2>();
    });
  });
  Q.wait();
  return 0;
  // CHECK: piMemRelease
}
