---
layout: page
title: Computers and Coffee Machines
subtitle: A code-based introduction to memory hierarchy
minutes: 20
---
> ## Learning Objectives {.objectives}
>
> * Reminder of Unit Tests
> * memory layout of n-dimensional structures in memory
> * Conceptual understanding of CPU Caches and their impact on runtime

Lola Likely has accepted a part-time job to financially fortify her PhD studies at the Institute of Things (_IoT_). For this, she is asked to help write some software that is used to manage and maintain the status and activity of all the web-capable smart devices that the IoT operates in house. This piece of software will keep track of simple things like location, if the device is on, in standby or off. It shall also produce some statistics of the usage of the devices with reference to time of day etc. All in all, this app will be used by the IT department and the administration to monitor and charge for the operation of any smart devices per group in order to keep track of the infrastructure costs, taxation needs and alike.

Lola is first asked to look at some code that is supposed to obtain the number of devices that are on in the building. For this, her predecessors decided to store the information in a 2D data structure that is laid out just as the building of the IoT is setup. This static data structure is updated automatically whenever it is queried. She can assume that `iot::devices` always reflects the current state of all smart devices in house.

In order to access the status of any device in room 12 on the first floor, the following method has to be called.

~~~ {.c}
int floor = 1;
int room = 11;
auto room_ref = iot::devices(floor,room)
int answer = room_ref.active_devices()
~~~

She heard from a friend of hers, that writing tests before any implementation is added is the latest and greatest software development method in the industry. As she wants to keep her job chances high, she decides to try this method and train this way of writing code on the job.

~~~ {.c}
#include "iot.hpp"
#include "gtest/gtest.h"

TEST(access_works,in_first_room) {
	EXPECT_GT(iot::devices_active(0,0),0);
}
~~~

> ## Unit test libraries for C++ {.callout}
>
> We opted for [google-test](https://github.com/google/googletest) library, not because we prefer it or we were paid by google or any other unlikely reason. No, simply because the target audience of the first incarnation of this material was known be part of a community that uses/used google-test extensively. At the time of writing, there are other very good unit testing libraries out there: [boost unified test framework](http://www.boost.org/doc/libs/1_60_0/libs/test/doc/html/index.html), [cgreen](https://github.com/cgreen-devs/cgreen), [catch](https://github.com/philsquared/Catch), ... . In the end, it depends on your tast, the context you work in and what you require a unit test framework to do. So please, experiment!


~~~ {.output}
$ ./test_device_access 
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from access_works
[ RUN      ] access_works.in_first_room
[       OK ] access_works.in_first_room (0 ms)
[----------] 1 test from access_works (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (0 ms total)
[  PASSED  ] 1 test.
~~~

She will add some more tests now, to explore the API of the `iot` namespace and it's features. In any case, she soon turns to the task at hand. She wants to sum up the number of online devices. This needs to be performed very quickly as the information will be displayed in real-time in the foyer of the institute. If something hangs there, the IOTs good name is at stake. 

~~~ {.c}
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

~~~ {.c}
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

While writing the test, she discovers that a single runtime will not help her very much. As each individual room is independent of each other, the most optimal runtime should be given by the same operation but performed on a 1D array. She decides to compare the runtime of the `count_active` function to just summing up a 1D array of the same size than the number of devices in house.

~~~ {.c}
#include <chrono>

TEST(Call_Devices, runtime_comparison) {

  //real-life timing
  auto t_start = std::chrono::high_resolution_clock::now();
  size_t n_active = count_active();
  auto t_end = std::chrono::high_resolution_clock::now();
  double time_diff_ms = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count();

  //generate training data
  std::vector<int> training_data = synthetic_devices(iot::n_rooms()*iot::n_levels());
  t_start = std::chrono::high_resolution_clock::now();
  size_t n_active_synthetic = 0;

  for( size_t i = 0;i<training_data.size();++i)
    n_active_synthetic += training_data[i];
	
  t_end = std::chrono::high_resolution_clock::now();
  double synthetic_diff_ms = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count();

  //unit tests that fix the expected performace
  EXPECT_LT(synthetic_diff_ms,time_diff_ms);
  ASSERT_GT(synthetic_diff_ms,0);
  EXPECT_GT(synthetic_diff_ms/time_diff_ms,.90);
	
}
~~~

Lola sets herself the goal, that her implementation is as good as traversing a linear array by 10%. This is an ambitious goal, but she is aware of the fact that however the internal data structure of `iot` is laid out, it must boil down to a sequence of 1D arrays as this is the structure of the memory that is built into computers these days. She is quite surprise when she observes the results:


~~~ {.output}
$ ./test_runtime_device_access
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from Call_Devices
[ RUN      ] Call_Devices.runtime_comparison
real 724, synth 338      = 2.14201
test_runtime_device_access.cpp:57: Failure
Expected: (synthetic_diff_ms/time_diff_ms) > (.90), actual: 0.466851 vs 0.9
[  FAILED  ] Call_Devices.runtime_comparison (3 ms)
[----------] 1 test from Call_Devices (3 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (3 ms total)
[  PASSED  ] 0 tests.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] Call_Devices.runtime_comparison

 1 FAILED TEST
~~~

Wow! This is quite far from her goal. The time difference amounts up to `46.6851%`! That is a lot. Another thing she observes is, that everytime she runs the test suite, she gets a different result. A computer as such is a non-deterministic system and hence will never provide the exact same runtime. The stochastic variation maybe tiny and thus negligable in real life, but it is there. As the number of devices available is not very big ``, the resulting time differences are small as well and hence will fluctuate. For real life measurements, a repitition of the timing is needed to make the measurement independent of these statistical effects. As the time difference is so large now, we will skip this at this point in favor of brevity.


> ## Prope your Metal {.challenge}
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
