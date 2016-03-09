---
layout: page
title: Threading for the rest of us
minutes: 45
---

> ## Learning Objectives {.objectives}
>
> * parallel_for to add two large vectors
> * parallel_reduce to sum up one vector
> * understand tbb OO model
> * pros/cons of implicit multi-threading

Lola is quite happy with the plain threads implementation that she has come up with. She still wonders if there are easier ways of doing the parallel sum that she requires or the difference between time points. She wanders the web and decides that Intel Threading Building Blocks (TBB) might be a good fit for her case.

> ## No Holy Wars {.callout}
>
> As this is entirely teaching material, a decision had to be made what parallel programming framework/approach is to be taught in terms of implicit multi-threading. The curious reader should be advised to look at them after finishing this lesson, the most common are:
> 
> * Intel Threading Building Blocks (free open-source, [website](https://www.threadingbuildingblocks.org/))
> * OpenMP (free open-source coming with most compilers, [website](https://www.openmp.org/))
> * HPX (free open-source coming, [website](http://stellar-group.org/libraries/hpx/))
>
> This material does not recommend TBB over the other approaches. It has to be decided mostly case-by-case which is the best for an application or library.


Starting out with a test again, she has to update what had considerably.

```
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

TEST(power_difference,parallel) {

  std::vector<device_t> initial_device_info = iot::device_info;
  std::vector<device_t> delta(iot::size());
  iot::update_device_info(device_info);
  std::vector<device_t> updated_device_info = iot::device_info;

  tbb::blocked_range<size_t> brange(0,size());
  delta_apply ops(&initial_device_info,
		  &updated_device_info,
		  &delta);
  
  for(int i = 0;i<repeats;++i){
    tbb::parallel_for(brange,ops);
  }
  
  for(size_t i = 0;i<20;++i)
    EXPECT_NE(delta[i].power_consumption,0);
  
  }
  
```

There are a couple of things to observe here:

* `tbb::parallel_for`, TBB offers a simple function that requires a blocked range and an operation
* `delta_apply ops`, in good C++ tradition, the operation must be a functor (or function object) or a lambda function
* `tbb::blocked_range<size_t> brange(0,size());`, by default, a blocked range splits up the virtual range of numbers from 0 to `size()` and saves the limits of each chunk internally (`tbb::parallel_for` will access it later on)

To get more insight, let's have a look at the functor:

```
struct delta_apply{

  const std::vector<device_t>* lhs;
  const std::vector<device_t>* rhs;
  std::vector<device_t>* res;

  delta_apply(const std::vector<device_t>* _lhs,
	      const std::vector<device_t>* _rhs,
	      std::vector<device_t>*	   _res):
    lhs(_lhs),
    rhs(_rhs),
    res(_res){}

  ~delta_apply(){}

  void operator()(const tbb::blocked_range<size_t>& range) const {
    std::vector<device_t>* local_res = res;
    
    for( size_t i=range.begin(); i!=range.end(); ++i )
      (*local_res)[i] = (*lhs)[i]-(*rhs)[i];

  }
};
```

`tbb::parallel_for` requires the functor to expose the bracket operator as given by `void operator()(const tbb::blocked_range<size_t>& range) const `, any other signature will not even compile. As one can see, the blocked range that comes into the function, exposes the begin and end of the chunk of data the operations need to work on (see `range.begin()` and `range.end()` method calls). With this, the amount of work to be done is always clear and not subject to errors. The infamous const-away cast in `std::vector<device_t>* local_res = res;` is a legacy of the library and simply has to be done as it is enforced by the `const` keyword of the expected `operator()` signature.

Lola thinks that this appears to be more flexible. This approach allows flexible swap in of different operation functors and reuse the `blocked_range`. However, the functor has grown quite big for this simple task and must amount to the same number of lines of source code than her `std::thread` based solution. She then discovers that TBB offers more parallel primitives than parallel_for:

* parallel_for
* parallel_do
* parallel_reduce
* parallel_scan
* parallel_sort

Lola realizes that parallel_reduce is the operation of choice for her problem of efficiently calculating the sum of the currently consumed power. So she looks into that by starting with a test:

```
TEST(power_total,parallel) {

  std::vector<device_t> dinfo = iot::device_info;
  
  tbb::blocked_range<device_t*> brange(&dinfo[0],&dinfo[0] + size());

  power_sum functor;

  auto start_t = std::chrono::high_resolution_clock::now();
  for(int i = 0;i<repeats;++i){
    tbb::parallel_reduce(brange,functor);
  }

  EXPECT_GT(functor.result,0.);
}
```

As Lola is exploring the library, she decides to go down a different route this time. Instead of invoking the brange over a virtual set of indices. She hands over the pointer to the begin and end of the `device_info` list directly `tbb::blocked_range<device_t*> brange(&dinfo[0],&dinfo[0] + size())`. Again, there is a function object to invoke `power_sum functor` and the actual parallel dispatchment is performed by `tbb::parallel_reduce(brange,functor)`.

```
struct power_sum {

  double result;

  power_sum():
    result(0){}
  
  power_sum(power_sum& _rhs, tbb::split ):
    result(0.)
  {}

  ~power_sum(){}

  void operator()(const tbb::blocked_range<device_t*>& range) {
    
    double local = result;
    
    for( device_t* i=range.begin(); i!=range.end(); ++i )
      local += i->power_consumption;

    result = local;

  }

  void join(power_sum& _rhs){ result += _rhs.result; }
};
```

There are some piculiarities to take note of here:

* `power_sum(power_sum& _rhs, tbb::split )`, this copy constructor is required by TBB
* `void operator()(const tbb::blocked_range<device_t*>& range)`, this is the actual bracket operator that does the heavy lifting. Note, it does not need to be declared const anymore.
* `void join(power_sum& _rhs)`, this function is required as well

So what's going on here? `parallel_reduce` is a variant of the map-reduce pattern even though we do not have a mapping step here. This idiom is very common not only in implicit multi-threading libraries/frameworks.

**figure goes here**

First, partial results are computed on a per data chunk aka per thread basis. After this is complete (there is a barrier inside TBB to ensure this), the individual partial results are summed by means of the `join` function. 
