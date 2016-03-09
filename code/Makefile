modules = 01-computers_and_coffee_machines 02-fast_slow_inheritance/ 03-plain_threads/ 04-threads-for-the-rest-of-us/

FILES=$(shell find $(modules) -name "*pp" -or -name "Makefile" |grep -v test)
FILES+= $(shell find $(modules) -name "test_start_here.cpp")

package :
	tar cf performance-by-example-code.tgz $(FILES)
