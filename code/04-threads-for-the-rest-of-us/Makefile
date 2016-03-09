GTEST_ROOT = /home/psteinb/software/googletest/master/

CXXFLAGS +=  -std=c++11 -I$(GTEST_ROOT)/include 

LDFLAGS += -L$(GTEST_ROOT)/lib -lgtest -pthread -ltbb #-lgmock

SRC_FILES = $(wildcard test*cpp)
DST_FILES = $(SRC_FILES:%.cpp=%)

all : $(DST_FILES)

% : %.cpp
	$(CXX) $< $(CXXFLAGS) $(LDFLAGS) -o $@

clean :
	rm -f -v $(DST_FILES)
