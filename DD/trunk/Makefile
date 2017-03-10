
#********define executable file name**********
#定义可执行程序的名称，需要根据实际情况进行更改
EXECUTABLE := dcu

#********define shared libs used by the lib,separated by space*********
#使用的共享库列表，以空格分开, 需要根据实际情况更改
LIBS :=stdc++ mcp3208
#define c++ compiler,such as ppc_8xx-g++,g++
#定义使用的C++编译器,例如ppc_8xx-g++,g++等

export CC=arm-none-linux-gnueabi-gcc

ifeq ($(CC),cc)
CC=arm-none-linux-gnueabi-gcc
endif



#define path of the shared lib to export
#输出的共享库所在路径

ifeq ($(EXPORTBASEPATH),)
EXPORTBASEPATH=./mnt/xtdcu/program
endif

EXPORTPATH:=$(EXPORTBASEPATH)
LIBPATH:=$(EXPORTPATH)/lib

# Now alter any implicit rules' variables if you like, e.g.:
# 现在来改变任何你想改动的隐含规则中的变量，例如    GDLOWER此为广东低成本编译开关
CFLAGS := -Wall -O2 -D ARCH_ARM -D DEV_6100 -D GDLOWER
CXXFLAGS := $(CFLAGS)

RM-F := rm -f

# You shouldn't need to change anything below this point.
# 从这里开始，你应该不需要改动任何东西。

#define model rules for c and cpp files
#定义模式规则
	
%.o:%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
%.o:%.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	
SOURCE_DIRS =include base share conn drule drule/gddyjc drule/gddyjc/ftsrule drule/p8090rule mrule mrule/gb2007 mrule/gb645 mrule/gb645/gd645 mrule/gb645/gdzb mrule/lgr mrule/lgr/lgd mrule/lgr/lgb mrule/mk mrule/mk/mk3 mrule/mk/mk6 brule brule/RouterStrategy config config/confuse server server/daemon server/httx server/dbcj server/dbcj/DbcjDebugCommand server/dbcj/GDDbcjManager server/dbcj/GDDbcjCommand server/ppp server/Lcd/lcdBottom  server/Lcd/DataGWjc  server/Lcd server/jilian 

SOURCE := $(wildcard *.c) $(wildcard *.cpp) 

SOURCE +=$(foreach DIR,$(SOURCE_DIRS),$(wildcard $(DIR)/*.c $(DIR)/*.cpp))

ALLOBJS :=$(foreach DIR,$(SOURCE_DIRS),$(wildcard $(DIR)/*.o))
ALLDEPS :=$(foreach DIR,$(SOURCE_DIRS),$(wildcard $(DIR)/*.d))

OBJS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
CMISSING_DEPS := $(filter-out $(wildcard $(CDEPS)),$(CDEPS))
CPPMISSING_DEPS := $(filter-out $(wildcard $(CPPDEPS)),$(CPPDEPS))


MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.c,$(MISSING_DEPS)) $(patsubst %.d,%.cpp,$(MISSING_DEPS)))
CPPFLAGS += -MD
.PHONY : everything deps objs clean veryclean rebuild
everything : $(EXECUTABLE)
deps : $(ALLDEPS)
objs : $(OBJS)
clean :
	$(RM-F) *.d
	$(RM-F) $(ALLOBJS)
	$(RM-F) $(ALLDEPS)
veryclean: clean
	$(RM-F) $(EXPORTPATH)/$(EXECUTABLE)
rebuild: veryclean everything
ifneq ($(CMISSING_DEPS),)
$(CMISSING_DEPS) :
	$(RM-F) $(patsubst %.d,%.o,$@)
	$(CC) -o $@ -MM -MMD $(patsubst %.d,%.c,$@)
endif
ifneq ($(CPPMISSING_DEPS),)
$(CPPMISSING_DEPS) :
	$(RM-F) $(patsubst %.d,%.o,$@)
	$(CC) -o $@ -MM -MMD $(patsubst %.d,%.cpp,$@)
endif
include $(ALLDEPS)

$(EXECUTABLE) : $(OBJS)
	$(CC) -L$(LIBPATH) -lm -s -o  $(EXPORTPATH)/$(EXECUTABLE) $(OBJS) $(LDLIBS) $(addprefix -l,$(LIBS)) conn/conn.a drule/gddyjc/gddyjc.a server/server.a base/base.a share/share.a
#	$(RM-F) *.d
