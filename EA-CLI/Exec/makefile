USER_OBJS :=
LIBS := -lopenea -lboost_system -lboost_filesystem -ldl -lpthread

CPP_SRCS := $(shell find ../src -name "*.cpp" -not -path "../src/test/*" -not -path "../src/Add-on.cpp")
CPP_ADDON := ../src/Add-on.cpp $(shell find ../add-on -name "*.cpp")

OBJS := $(CPP_SRCS:../%.cpp=./%.o)
OBJS_ADDON := $(CPP_ADDON:../%.cpp=./%.o)

CPP_DEPS := $(shell find . -name "*.d")
SUB_DIRS := src $(shell cd .. && find add-on -type d)

# Include all dependency files
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif

# All Target
all: openea-run libopenea_addon.so

# Dependencies
dependents: ./../EA/SharedLib/libEA.so
	
../../EA/SharedLib/libEA.so:
	-cd ../../EA/SharedLib && $(MAKE) all

# Sub directory
$(SUB_DIRS):
	@echo 'Creating directory: $@'
	-mkdir $(@)
	@echo ' '

# Compiler
$(OBJS_ADDON): ./%.o: ../%.cpp
	@echo 'GCC C++ Compiler: $<'
	@g++ -std=c++14 -I"../../EA/src" -O3 -g0 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	
$(OBJS): ./%.o: ../%.cpp
	@echo 'GCC C++ Compiler: $<'
	@g++ -std=c++14 -I"../../EA/src" -O3 -g0 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"

# Linker
libopenea_addon.so: $(SUB_DIRS) $(OBJS_ADDON) $(USER_OBJS) ../../EA/SharedLib/libopenea.so
	@echo 'GCC C++ Linker: $@'
	@g++ -L"../../EA/SharedLib" -shared -o "$@" $(OBJS_ADDON) $(USER_OBJS) -lopenea

openea-run: $(SUB_DIRS) $(OBJS) $(USER_OBJS) ../../EA/SharedLib/libopenea.so
	@echo 'GCC C++ Linker: $@'
	@g++ -L"../../EA/SharedLib" -o "$@" $(OBJS) $(USER_OBJS) $(LIBS)

# Other Targets
clean:
	-rm -rf $(OBJS) $(OBJS_ADDON) $(CPP_DEPS) openea-run libopenea_addon.so
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

