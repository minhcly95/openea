#!/usr/bin/make

CPPFLAGS := -std=c++14 -O3 -g0 -Wall -c -fmessage-length=0

USER_OBJS :=
LIBS := -lopenea -lboost_system -lboost_filesystem -ldl -lpthread

CPP_SRCS := $(shell find ./src -name "*.cpp")
OBJS := $(CPP_SRCS:./src/%.cpp=./obj/%.o)

CPP_DEPS := $(shell find . -name "*.d")
SUB_DIRS := $(shell find ./src/ -type d)
SUB_DIRS := $(SUB_DIRS:./src/%=./obj/%)

# All Target
all: openea-run

# Include all dependency files
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif

# Sub directory
$(SUB_DIRS):
	@echo 'Creating directory: $@'
	@-mkdir $(@)

# Compiler
$(OBJS): ./obj/%.o: ./src/%.cpp
	@echo 'GCC C++ Compiler: $<'
	@g++ -I"../EA/src" -DEA_CLI_RELEASE $(CPPFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"

# Linker
openea-run: $(SUB_DIRS) $(OBJS) $(USER_OBJS)
	@echo 'GCC C++ Linker: $@'
	@g++ -L"../EA" -o "openea-run" $(OBJS) $(USER_OBJS) $(LIBS)

# Other Targets
clean:
	@-rm -rf $(SUB_DIRS) openea-run

