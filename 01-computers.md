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

TEST(Call_Devices, Size_not_empty) {
	EXPECT_GT(iot::devices(0,0).size(),0);
}
~~~

~~~ {.output}
xxx bla bla xxx
~~~

She will add some more tests now, to explore the API of the `iot` namespace and it's features. In any case, she soon turns to the task at hand.

~~~ {.c}
TEST(Call_Devices, Sum_of_active) {

	size_t sum = 0;
	for(size_t room = 0;room < 128;++room){
		for(size_t level = 0;level < 16;++level){
			sum += iot::active_devices(level,room);
		}
	}
		
	EXPECT_GT(sum,0);
}
~~~

This is such a common operation, that she decides to put it into a function in order to reuse it later on.

> ## Header-Only versus A library {.callout}
>
> Some information about pros/cons.


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
