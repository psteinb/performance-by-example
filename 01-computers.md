---
layout: page
title: Computers and Coffee Machines
subtitle: A code-based introduction to memory hierarchy
minutes: 40
---

> ## Learning Objectives {.objectives}
>
> * Reminder of Unit Tests
> * memory layout of n-dimensional structures in memory
> * Conceptual understanding of CPU Caches and their impact on runtime

Lola Likely has accepted a part-time job to financially fortify her PhD studies at the Institute of Things (_IoT_). For this, she is asked to help write some software that is used to manage and maintain the status and activity of all the web-capable smart devices that the IoT operates in house. This piece of software will keep track of simple things like location, if the device is on, in standby or off. It shall also produce some statistics of the usage of the devices with reference to time of day etc. All in all, this app will be used by the IT department and the administration to monitor and charge for the operation of any smart devices per group in order to keep track of the infrastructure costs, taxation needs and alike.

Lola is first asked to look at some code that is supposed to obtain the number of devices that are on in the building. For this, her predecessors decided to store the information in a 2D data structure that is laid out just as the building of the IoT is setup. This static data structure is updated automatically whenever it is queried. She can assume that `iot::devices` always reflects the current state of all smart devices in house.

In order to access the status of any device in room 12 on the first floor, the following method has to be called.

~~~ {.cpp}
int floor = 1;
int room = 11;
int n_active = iot::active_devices(floor,room)
~~~

She heard from a friend of hers, that writing tests before any implementation is added is the latest and greatest software development method in the industry. As she wants to keep her job chances high, she decides to try this method and train this way of writing code on the job. To start with, she wants to know if any devices are active in the basement, i.e. `level = 0`:

~~~ {.cpp}
#include "iot.hpp"
#include "gtest/gtest.h"

TEST(access_works,anything_on_first_level) {

  int n_active = 0;
  for(int r = 0;r<iot::n_rooms();++r)
    n_active += (iot::active_devices(0,r));
}
~~~

> ## Unit test libraries for C++ {.callout}
>
> We opted for the [google-test](https://github.com/google/googletest) library, not because we prefer it or we were paid by google or any other unlikely reason. No, simply because the target audience of the first incarnation of this material was known to be part of a community that uses/used google-test extensively. At the time of writing, there are other very C++ good unit testing libraries out there: [boost unified test framework](http://www.boost.org/doc/libs/1_60_0/libs/test/doc/html/index.html), [cgreen](https://github.com/cgreen-devs/cgreen), [catch](https://github.com/philsquared/Catch), ... . In the end, it depends on your taste, the context you work in and what you require a unit test framework to do. So please, experiment!


~~~ {.output}
$ ./test_device_access 
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from access_works
[ RUN      ] access_works.anything_on_first_level
[       OK ] access_works.anything_on_first_level (2 ms)
[----------] 1 test from access_works (2 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (3 ms total)
[  PASSED  ] 1 test.
~~~

What a good feeling! Everything is green. She will add some more tests now, to explore the API of the `iot` namespace and it's features. In any case, she soon turns to the task at hand. She wants to sum up the number of online devices. This needs to be performed very quickly as the information will be displayed in real-time in the foyer of the institute. If something hangs there, the IOTs good name is at stake. 

~~~ {.cpp}
TEST(Call_Devices, Sum_of_active) {

	size_t sum = 0;
	for(size_t room = 0;room < iot::n_rooms();++room){
		for(size_t level = 0;level < iot::n_levels();++level){
			sum += iot::active_devices(level,room);
		}
	}

	EXPECT_LT(sum,std::pow(2,31));
	EXPECT_GT(sum,0);
}
~~~

This is such a common operation in her project, that she decides to put it into a function in order to reuse it later on. The test at the end just asks for any value greater than zero to act as a lower bound. The upper bound was set by the administration due to issues with number of overflow in a popular spreadsheat software. After watching the unit test complete, she has a feeling that the performance is not yet optimal.

> ## Header-Only versus shared libraries {.callout}
>
> In modern C++, there are two ways of putting reusable code modules into files and distribute them. Both have advantages and disadvantages:
>
> * header-only source code files
> * shared/static libraries or bundles thereof
>
> A header-only distribution can be included quickly into existing projects, they are primarily used to shared generic code that employs C++ templates. By this, they may (but must not) increase compile time of existing projects and allow an unobstructed look at the provided implementation.
>
> A shared library distribution often permits provisioning of very general (often templated) implementations, however the included code does not need to be recompiled if a client project is built. Further, the source code used is not visible by any client, only the API provided by the library. Further, shared libraries are build of a very mixed set of platforms, by this they can not contain very optimal code. For a thorough discussion of the subject, see 

Lola creates a new unit test to measure the performace of her implementation.

~~~ {.cpp}
#include <chrono>

std::size_t count_active() {

	size_t sum = 0;
	for(size_t room = 0;room < iot::n_rooms();++room){
		for(size_t level = 0;level < iot::n_levels();++level){
			sum += iot::active_devices(level,room);
		}
	}

    return sum;
}


TEST(Call_Devices, runtime_comparison) {

	auto t_start = std::chrono::high_resolution_clock::now();
    size_t n_active = count_active();
	auto t_end = std::chrono::high_resolution_clock::now();

    //?
}
~~~

While writing the test, she discovers that a single runtime appears to vary from execution to execution. She hence decides to run each function call for about 10 times.

> ## Computers are stochastic machines {.callout}
>
> Even though the hardware and wiring of the computer (laptop or workstation) under your fingers is static and "deterministic", the signals produced by such a complex system yield a considerable stochastic portion. Besides the noise produced by every transistor in your machine, while you perform a single time measurement inside a given process, many more processes are running inside the OS. Thus, if you want to judge the sustainable performance of a piece of code, conduct more than one measurements and record the results. Given the environment and purpose of a measurement, use statistical meaningful observables like mean, median and standard deviation to quantify runtimes.
>
> For library support, try (google/benchmark)[https://github.com/google/benchmark] library.

As each individual room is independent of each other, the most optimal runtime should be given by the same operation but performed on a 1D array (a 2D array is nothing else than a 1D). She decides to compare the runtime of the `count_active` function to just summing up a 1D array of the same size than the number of devices in house.

~~~ {.cpp}
#include <chrono>

TEST(Call_Devices, runtime_real) {

  auto t_start = std::chrono::high_resolution_clock::now();
  size_t n_active = 0;
  for(int i =0;i<repeats;++i)
    n_active = count_active();
  auto t_end = std::chrono::high_resolution_clock::now();
  double time_diff_mus = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count();
  time_diff_mus /= repeats;
  
  EXPECT_GT(time_diff_mus,0);
  std::cout << "real    : " << time_diff_mus << " ns\n";
  
}


TEST(Call_Devices, runtime_training) {

  std::vector<int> training_data = synthetic_devices(iot::n_rooms()*iot::n_levels());
  auto t_start = std::chrono::high_resolution_clock::now();
  size_t n_active_synthetic = 0;

  for(int i =0;i<repeats;++i)
    for( size_t i = 0;i<training_data.size();++i)
      n_active_synthetic += training_data[i];

  auto t_end = std::chrono::high_resolution_clock::now();
  double time_diff_mus = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count()/repeats;
  
  EXPECT_GT(time_diff_mus,0);
  std::cout << "training: " << time_diff_mus // << ", for " << iot::n_rooms()*iot::n_levels()*sizeof(int)/(1024) <<" KB of items
	    << " ns\n";
  
}

~~~

Lola sets herself the goal, that her implementation is as good as traversing a linear array by 10%. This is an ambitious goal, but she is aware of the fact that however the internal data structure of `iot` is laid out, it must boil down to a sequence of 1D arrays as this is the structure of the memory that is built into computers these days. She is quite surprise when she observes the results:


~~~ {.output}
$ ./test_runtime_device_access
[==========] Running 2 tests from 1 test case.
...
real    : 211410 mus
...
training: 89221 mus
...
[  PASSED  ] 2 tests.
~~~

Wow! This is quite far from her goal. The time difference amounts up to about `50%`! That is a lot. Looking at her implementation again, she discovers that she was traversed the array of device status information in a inconvenient way. The 2D array that mimicks the status of every device contains every room of one level, then all the rooms of the next one, etc. But the double loop to traverse it, looped first through all rooms but  the inner loop went through all the levels.

![](figures/wrong_cache_access_i0.svg)
![Item load in red, cache line load in orange](figures/wrong_cache_access_i1.svg)

Modern CPUs typically have a 3 level cache hierarchy built into them. The caches are high bandwidth but low volume volative memories that are printed on die. The motivation behind caches is to reread/rewrite items that are used often by the CPU at high speed. Once they are not needed anymore, they can be written to RAM eventually. As the CPU clock frequency is typically higher than the access to RAM, this helps provide enough data for the CPU to work on without long periods of idle time.

Optimally, the array should be traversed in the direction that the CPU puts data into cache lines.

![](figures/right_cache_access_i0.svg)
![](figures/right_cache_access_i1.svg)
![Item load in red, cache line load in orange](figures/right_cache_access_i2.svg)

In the illustrations above, the traversal of the loops first goes through all items that are in cache and then continues onto data that outside of it. Upon reaching the end of the cacheline, the hardware will put new data into the caches.

Lola changes her application accordingly and sees a runtime that is equivalent to looping through a 1D array.

~~~ {.output}
$ ./test_runtime_device_access
[==========] Running 3 tests from 1 test case.
...
real    : 211410 mus
...
training: 89221 mus
...
fast    : 140421 mus
...
[  PASSED  ] 3 tests.
~~~

Very interesting! So the way we traverse a vector or buffer does make a difference, especially when we move to large buffer sizes. Why is that? The reason for this boils down to the observation that

> **Computers are no coffee machines!**

For most every-day applications, this is of no importance though. If you click a button, it doesn't matter if the response from the computer comes at 100 Hz or 1000 Hz, either value is well below what your eye can distinguish. However, in data analysis, simulation and advanced visualisation things start to add up. Just like in this case, where we had a lot of items that needed to be traversed. With this, the very naive view of the computer internals falls apart and knowledge of the concepts behind computer architecture become important.

<div style="float: left;">
![The DRAM gap in modern CPU architectures.](figures/dram_gap.svg)
</div>

In the Moore's law era, the bandwidth at which memory is accessed didn't scale upwards as fast as the clock frequency of the CPUs. In order to hide this, a layered set of intermediate caches were introduced that can only hold a very limited number of data items but offer a very high bandwidth. So if a lot of data is needed, the hardware will (pre)fetch data into L3 cache when required, then load parts of it L2 and L1 until it finally can be moved into the registers that hold the input/output data of actual arithmetic or boolean arithmetic. 

> ## Probe your Metal {.challenge}
>
> Take the time and find out:
>
> 1. How much RAM the computer has that you are doing this lesson on?
> 2. How many cache levels it has?
> 3. What size the Level1 cache has?
> 4. What size the Level2 cache has?
> 5. What size the Level3 cache has?
>
> Share your results in the Etherpad.

> ## Show me your streams and I show you who you are {.challenge}
>
> Download the stream benchmark from [this ftp mirror](http://www.cs.virginia.edu/stream/FTP/Code/). Once compiled, the stream benchmark performs 4 simple operations (at best in parallel). All of which turn one or two input vectors/buffers into one result vector/buffer item-by-item. All of these operations are very sensitive to which the fact in which part of the memory they operate on. That is why, the stream benchmark is commonly used to validate or compare hardware.
>
> Look inside stream.c and find the preprocessor variable STREAM_ARRAY_SIZE. By default, it contains the size of the vector/buffer of double precision numbers. 
>
> 1. Run the stream benchmark with STREAM_ARRAY_SIZE being as large as 50% of your L1 cache.
> 1. Run the stream benchmark with STREAM_ARRAY_SIZE being as large as 50% of your L2 cache.
> 1. Run the stream benchmark with STREAM_ARRAY_SIZE being as large as 50% of your L3 cache.
> 1. Run the stream benchmark with STREAM_ARRAY_SIZE being as large as 4 times the size of your L3 cache.
> 
> Share your results in the Etherpad and discuss them.
