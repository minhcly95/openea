USER_OBJS :=
LIBS := -ltinyxml2 -lboost_log_setup -lboost_log -lboost_system -lboost_thread -lpthread -lrestbed

CPP_OPENMP := ../src/misc/MultiThreading.cpp ../src/strategy/cmaes/CMAEvolutionStrategy.cpp
CPP_MPI := ../src/misc/Cluster.cpp ../src/evaluator/IndividualEvaluator.cpp

CPP_SRCS := $(shell find ../src -name "*.cpp" -not -path "../src/test/*" -not -path "../src/example/*" -not -path "../src/archive/*")
CPP_SRCS := $(filter-out $(CPP_OPENMP) $(CPP_MPI), $(CPP_SRCS))
							
OBJS_OPENMP := $(CPP_OPENMP:../%.cpp=./%.o)
OBJS_MPI := $(CPP_MPI:../%.cpp=./%.o)
OBJS := $(CPP_SRCS:../%.cpp=./%.o)

CPP_DEPS := $(shell find . -name "*.d")
SUB_DIRS := $(shell cd .. && find ./src -type d -not -path "./src/test" -not -path "./src/test/*"\
												-not -path "./src/archive" -not -path "./src/archive/*"\
												-not -path "./src/pch.h.gch" -not -path "./src/pch.h.gch/*"\
												-not -path "./src/example" -not -path "./src/example/*")

# Include all dependency files
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif

# All Target
all: libopenea.so

# Sub directory
$(SUB_DIRS):
	@echo 'Creating directory: $@'
	@-mkdir $(@)

# Compiler
$(OBJS): ./%.o: ../%.cpp
	@echo 'GCC C++ Compiler: $<'
	@g++ -std=c++14 -O3 -g0 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	
$(OBJS_OPENMP): ./%.o: ../%.cpp
	@echo 'GCC C++ Compiler with OpenMP: $<'
	@g++ -std=c++14 -O3 -g0 -Wall -c -fmessage-length=0 -fopenmp -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	
$(OBJS_MPI): ./%.o: ../%.cpp
	@echo 'MPI C++ Compiler: $<'
	@mpic++ -std=c++14 -O3 -g0 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"

# Linker
libopenea.so: $(SUB_DIRS) $(OBJS) $(OBJS_OPENMP) $(OBJS_MPI) $(USER_OBJS)
	@echo 'MPI C++ Linker: $@'
	@mpic++ -shared -fopenmp -o "$@" $(OBJS) $(OBJS_OPENMP) $(OBJS_MPI) $(USER_OBJS) $(LIBS)
	@echo ' '

# Other Targets
clean:
	-rm -rf $(OBJS) $(OBJS_OPENMP) $(OBJS_MPI) $(CPP_DEPS) libopenea.so
	-@echo ' '

.PHONY: all clean
.SECONDARY:

