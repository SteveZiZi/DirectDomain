
#********define shared lib short name**********
#定义共享库的短名称，需要根据实际情况进行更改
EXECUTABLE :=confuse


#********define shared libs used by the lib,separated by space*********
#使用的共享库列表，以空格分开, 需要根据实际情况更改
LIBS :=

SONAME=lib$(EXECUTABLE).so

#define c++ compiler,such as ppc_8xx-g++,g++
#定义使用的C++编译器,例如ppc_8xx-g++,g++等

ifeq ($(CC),cc)
CC=arm-uclibc-gcc
endif

#define path of the shared lib to export
#输出的共享库所在路径

ifeq ($(EXPORTBASEPATH),)
EXPORTBASEPATH=/mnt/hgfs/dyjc/program
ifeq ($(CC),ppc_8xx-gcc)
EXPORTBASEPATH=/opt/sinovee/ppc8xx-linux-2.0/target/mnt/ptu/program
endif
endif
EXPORTPATH:=$(EXPORTBASEPATH)/lib


# Now alter any implicit rules' variables if you like, e.g.:
# 现在来改变任何你想改动的隐含规则中的变量，例如
CFLAGS := -Wall -Os  -D ARCH_ARM
CXXFLAGS := $(CFLAGS)

RM-F := rm -f

# You shouldn't need to change anything below this point.
# 从这里开始，你应该不需要改动任何东西。

%.o:%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
%.o:%.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

SOURCE := $(wildcard *.c) $(wildcard *.cpp)
OBJS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
DEPS := $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.c,$(MISSING_DEPS)) $(patsubst %.d,%.cpp,$(MISSING_DEPS)))
CPPFLAGS += -MD
.PHONY : everything deps objs clean veryclean rebuild
everything : $(EXECUTABLE)
deps : $(DEPS)
objs : $(OBJS)
clean :
	$(RM-F) *.o
	$(RM-F) *.d
veryclean: clean
	$(RM-F) $(EXPORTPATH)/$(SONAME)
rebuild: veryclean everything
ifneq ($(MISSING_DEPS),)
$(MISSING_DEPS) :
	$(RM-F) $(patsubst %.d,%.o,$@)
endif
include $(DEPS)
$(EXECUTABLE) : $(OBJS)
	$(CC) -L$(EXPORTPATH)/ -s -nostartfiles -shared -o $(EXPORTPATH)/$(SONAME) $(OBJS) $(addprefix -l,$(LIBS))
