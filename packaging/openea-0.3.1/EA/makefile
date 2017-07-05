#!/usr/bin/make

CPPFLAGS := -std=c++14 -O3 -g0 -Wall -c -fmessage-length=0 -fPIC

USER_OBJS :=
LIBS := -ltinyxml2 -lboost_log_setup -lboost_log -lboost_system -lboost_thread -lpthread -lrestbed

CPP_OPENMP := ./src/misc/MultiThreading.cpp ./src/strategy/cmaes/CMAEvolutionStrategy.cpp
CPP_MPI := ./src/misc/Cluster.cpp ./src/evaluator/IndividualEvaluator.cpp

CPP_SRCS := $(shell find ./src -name "*.cpp")
CPP_SRCS := $(filter-out $(CPP_OPENMP) $(CPP_MPI), $(CPP_SRCS))
							
OBJS_OPENMP := $(CPP_OPENMP:./src/%.cpp=./obj/%.o)
OBJS_MPI := $(CPP_MPI:./src/%.cpp=./obj/%.o)
OBJS := $(CPP_SRCS:./src/%.cpp=./obj/%.o)

CPP_DEPS := $(shell find . -name "*.d")
SUB_DIRS := $(shell find ./src/ -type d)
SUB_DIRS := $(SUB_DIRS:./src/%=./obj/%)

# All Target
all: libopenea.so.0

# Include all dependency files
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif

test:
	@echo $(CPP_SRCS)
	@echo $(OBJS)
	@echo $(CPP_DEPS)
	@echo $(SUB_DIRS)

# Sub directory
$(SUB_DIRS):
	@echo 'Creating directory: $@'
	@-mkdir $(@)

# Compiler
./src/pch.h.gch: ./src/pch.h
	@echo 'GCC C++ Compiler: $<'
	@g++ $(CPPFLAGS) -o ./src/pch.h.gch ./src/pch.h

$(OBJS): ./obj/%.o: ./src/%.cpp ./src/pch.h.gch
	@echo 'GCC C++ Compiler: $<'
	@g++ $(CPPFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	
$(OBJS_OPENMP): ./obj/%.o: ./src/%.cpp
	@echo 'GCC C++ Compiler with OpenMP: $<'
	@g++ $(CPPFLAGS) -fopenmp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	
$(OBJS_MPI): ./obj/%.o: ./src/%.cpp
	@echo 'MPI C++ Compiler: $<'
	@mpic++ $(CPPFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"

# Linker
libopenea.so.0: $(SUB_DIRS) $(OBJS) $(OBJS_OPENMP) $(OBJS_MPI) $(USER_OBJS)
	@echo 'MPI C++ Linker: $@'
	@mpic++ -shared -fopenmp -Wl,-soname,"$@" -o "$@" $(OBJS) $(OBJS_OPENMP) $(OBJS_MPI) $(USER_OBJS) $(LIBS)
	@ln -s libopenea.so.0 libopenea.so
	@echo ' '

# Other Targets
clean:
	@-rm -rf $(SUB_DIRS) *.so *.so.* ./src/pch.h.gch

