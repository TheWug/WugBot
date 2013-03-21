# hacks
NOTHING			=
SPACE			= $(NOTHING) $(NOTHING)

# primary settings
EXECNAME		= ircbot
ROOTDIR			= .

# destination folders
OUTPUT			= $(ROOTDIR)/../output
OUTPUTLIB		= $(ROOTDIR)/../output/lib
LIVEDIR			= ~/wugbot/live
TESTDIR			= ~/wugbot/test

# conf directory for testing
TESTCONF_READONLY	= ~/wugbot/testconf

# source folders
SRCCOREDIR		= $(ROOTDIR)/src
SRCMODDIR		= $(ROOTDIR)/src/mod
ALLSRCDIRS		= $(SRCCOREDIR):$(SRCMODDIR)

# object file folders
OBJCOREDIR		= $(ROOTDIR)/obj
OBJMODDIR		= $(ROOTDIR)/obj/mod

# libraries to link against
LINKLIBS		= dl rt

# make vpath
VPATH			= $(OUTPUT):$(OUTPUTLIB):$(ALLSRCDIRS)

# compiler directives
INCLUDEDIRECTIVE	= $(addprefix -I, $(subst :, $(SPACE), $(ALLSRCDIRS)))
LINKDIRECTIVE		= $(addprefix -l, $(LINKLIBS))

CXX			= g++
CXXFLAGSGENERAL		= $(INCLUDEDIRECTIVE) -g
CXXLINKGENERAL		= -rdynamic

CXXFLAGSMODULES		= $(INCLUDEDIRECTIVE) -g fPIC
CXXLINKMODULES		= -shared

# core source files
COREFILES 		= BotCore BotLogger BotTime CommonEvents Concurrency EventHandler Log NetInterface NetManager Properties StringUtil VectorUtil Main SignalHandler
CORESRC			= $(addsuffix .cpp, $(COREFILES))
COREOBJ			= $(addsuffix .o, $(COREFILES))

# modules
ALLMODULES		=

MODXFILES		= Blarg
MODXSRC			= $(addsuffix .cpp, $(MODXFILES))
MODXOBJ			= $(addsuffix .o, $(MODXFILES))

# files
EXECFILENAME		= $(OUTPUT)/$(EXECNAME)
COREOBJFILES		= $(addprefix $(OBJCOREDIR)/, $(COREOBJ))

# TARGETS
###############################################################################

# generic target
all: $(EXECFILENAME) modules

# target for all modules
modules: $(ALLMODULES)

# targets for primary executable
$(EXECNAME): $(EXECFILENAME)
$(EXECFILENAME): $(COREOBJFILES)
	$(CXX) $(CXXLINKGENERAL) -o $@ $(COREOBJFILES) $(LINKDIRECTIVE)

# target for objects
$(OBJCOREDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGSGENERAL) -c -o $@ $^

# target for module objects
$(OBJMODDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGSMODULES) -c -o $@ $^

# this is how you configure the files for a module
# modx:    OBJ_CURRENTMOD := $(MODXOBJ)
# modx:    $(MODXMOD) $(OUTPUTLIB)/libmodx.so

# general library file builder rule
.SECONDEXPANSION:
$(OUTPUTLIB)%.so: $$(OBJ_CURRENTMOD)
	$(CXX) $(CXXLINKMODULES) -o $@ $(OBJ_CURRENTMOD) $(LINKDIRECTIVE)

# OUTPUT/EXEC TARGETS
############################################################################

# start/stop by various methods
test:
	cd $(TESTDIR); ./$(EXECNAME) -nd;
debug:
	cd $(TESTDIR); gdb ./$(EXECNAME);
start:
	cd $(LIVEDIR); ./$(EXECNAME);
killall:
	killall $(EXECNAME);

# store whole compiled output
storelive: $(EXECNAME) $(ALLMODULES)
	-rm $(LIVEDIR)/$(EXECNAME)
	-rm $(LIVELIB)/*.so
	cp $(OUTPUT)/$(EXECNAME) $(LIVEDIR)
	cp $(OUTPUTLIB)/*.so $(LIVEDIR)/lib

storetest: $(EXECNAME) $(ALLMODULES)
	-rm $(TESTDIR)/$(EXECNAME)
	-rm $(TESTLIB)/*.so
	cp $(OUTPUT)/$(EXECNAME) $(TESTDIR)
	cp $(OUTPUTLIB)/*.so $(TESTDIR)/lib

# store targets for modules
storetestmod:
	-rm $(TESTDIR)/lib/*.so
	cp $(OUTPUTLIB)/*.so $(TESTDIR)/lib

storelivemod:
	-rm $(TESTDIR)/lib/*.so
	cp $(OUTPUTLIB)/*.so $(LIVEDIR)/lib

# HOUSEKEEPING
##############################################################################

# create all necessary directories just in case one is missing for some reason
dirs:
	-mkdir $(TESTDIR)
	-mkdir $(LIVEDIR)
	-mkdir $(TESTDIR)/lib
	-mkdir $(LIVEDIR)/lib
	-mkdir $(OUTPUT)
	-mkdir $(OUTPUTLIB)
	-mkdir $(OBJCOREDIR)
	-mkdir $(OBJMODDIR)

# cleans (removes just object files)
coreclean:
	-rm -- $(OBJCOREDIR)/*.o
modclean:
	-rm -- $(OBJMODDIR)/*.o
clean: coreclean modclean

# realcleans (removes object files and finished binaries)
corerealclean: coreclean
	-rm $(OUTPUT)/$(EXECNAME)
modrealclean: modclean
	-rm $(OUTPUTLIB)/*.so
realclean: corerealclean modrealclean

# dependency generation
depend: $(SRCMAIN) $(SRCCMD) $(SRCDCMD) $(SRCRESP) $(SRCMOD)
	$(CXX) -MM $(INC) $^ > dependancies.txt

# DEPENENCIES
####################################################################################

BotCore.o: BotCore.cpp BotCore.h BotLogger.h Log.h EventHandler.h BotTime.h Concurrency.h
BotLogger.o: BotLogger.cpp BotLogger.h Log.h StringUtil.h
BotTime.o: BotTime.cpp BotTime.h
CommonEvents.o: CommonEvents.cpp CommonEvents.h
Concurrency.o: Concurrency.cpp Concurrency.h
EventHandler.o: EventHandler.cpp EventHandler.h BotTime.h Concurrency.h
Log.o: Log.cpp Log.h
NetInterface.o: NetInterface.cpp NetInterface.h BotCore.h BotLogger.h Log.h EventHandler.h BotTime.h Concurrency.h StringUtil.h
NetManager.o: NetManager.cpp NetManager.h EventHandler.h BotTime.h Concurrency.h NetInterface.h CommonEvents.h StringUtil.h BotCore.h BotLogger.h Log.h
Properties.o: Properties.cpp Properties.h BotLogger.h Log.h BotCore.h EventHandler.h BotTime.h Concurrency.h StringUtil.h VectorUtil.h
StringUtil.o: StringUtil.cpp StringUtil.h
VectorUtil.o: VectorUtil.cpp VectorUtil.h
