# a makefile that doesnt suck as much as the old one

#################################################################################################
#	GENERAL DIRECTIONS
#
#	to add files to the main build: put them in one of the following:
#	SRCMAIN SRCCMD SRCDCMD SRCRESP

#	if you add a source directory for some reason, make another SRC___ variable and add files to it
#	remember to also add it to VPATH so make can find the files in it

#	to add a module, you should add:
#	a module specific source variable (such as SRCMOD_MODNAME) and one for objects too
#	a target for the module (such as modulename: $(OBJMOD_MODNAME) $(LIBDIR)libmodname.so)
#	a second target for the module containing the assignment: OBJ_CURRENTMOD = $(OBJMOD_MODNAME)
#	an entry in the MODULES variable so you can compile all modules with 'make modules'
#	an entry in the SRCMOD variable so 'make depend' will search for dependancies of your src files

#	use 'make depend' to generate a file with dependancies, then edit dependancies.txt to suit the makefile
#	in future, this process will be automatic
#	try not to use filenames or directory names with spaces in them

#################################################################################################
#	MACROS

# general options
EXECNAME	      = IRCBot
ROOTDIR		      = ./

# output folders
LIVEDIR		      = $(ROOTDIR)../live/
LIVELIB		      = $(LIVEDIR)lib/
TESTDIR		      = $(ROOTDIR)../test/
TESTLIB		      = $(TESTDIR)lib/

# read only testing configuration directory
TESTCONFRO	      = $(ROOTDIR)testconf/

SRCMAINDIR	      = $(ROOTDIR)src/
SRCCMDDIR	      = $(SRCMAINDIR)Commands/
SRCDCMDDIR	      = $(SRCMAINDIR)DeferredCommands/
SRCRESPDIR	      = $(SRCMAINDIR)Responses/
SRCMODDIR	      = $(SRCMAINDIR)Modules/

# source search path
VPATH		      = $(SRCMAINDIR):$(SRCCMDDIR):$(SRCDCMDDIR):$(SRCRESPDIR):$(SRCMODDIR):$(ROOTDIR)

# source files
SRCMAIN		      = BotInfo.cpp BotPermissions.cpp Bottime.cpp CommandLine.cpp Hostname.cpp \
			HTTP.cpp IRCBot.cpp IRCBotNetIface.cpp IRC.cpp main.cpp Message.cpp \
			Messenger.cpp misc.cpp ModuleHandler.cpp ServerPingThread.cpp ServerProperties.cpp \
			Timer.cpp UserManager.cpp Scheduler.cpp module.cpp EventActions.cpp

SRCCMD		      = alias.cpp CmdAccess.cpp CmdAlias.cpp CmdAllow.cpp CmdBan.cpp CmdCheck.cpp \
			cmd.cpp CmdCtcpResp.cpp CmdDie.cpp CmdDNS.cpp CmdExempt.cpp CmdFactoid.cpp \
			CmdFlags.cpp CmdFwd.cpp CmdHaspaid.cpp CmdHelp.cpp CmdIP.cpp CmdJoin.cpp \
			CmdKick.cpp CmdLong.cpp CmdMemo.cpp CmdMode.cpp CmdModule.cpp CmdMonitor.cpp \
			CmdNick.cpp CmdOn.cpp CmdOnUser.cpp CmdOp.cpp CmdOption.cpp CmdPart.cpp \
			CmdPoke.cpp CmdPrefix.cpp CmdQueue.cpp CmdQuiet.cpp CmdQuit.cpp CmdRaw.cpp \
			CmdRepeat.cpp CmdSave.cpp CmdSay.cpp CmdTell.cpp CmdTime.cpp CmdTopic.cpp \
			CmdVoice.cpp CmdWhere.cpp

SRCDCMD		      = CommandCaller.cpp dcmd.cpp DeferredAccessCheck.cpp DeferredAllow.cpp DeferredFlags.cpp \
			FactoidProcessor.cpp Forwarder.cpp IgnoreAccessModifier.cpp IPReader.cpp KickWatcher.cpp \
			MemoSender.cpp MessageHandler.cpp ModeWatcher.cpp MonitorWatcher.cpp ServerPinger.cpp \
			UserWatcher.cpp WhoTracker.cpp

SRCRESP		      = RspAccount.cpp RspConnect.cpp rsp.cpp RspInvite.cpp RspJoin.cpp RspKick.cpp RspMOTD.cpp \
			RspNick.cpp RspNickInUse.cpp RspNotice.cpp RspPart.cpp RspPrivmsg.cpp RspQuit.cpp

# source files for modules
SRCMOD_METRICS	      = Metrics.cpp
SRCMOD_CLONEWATCH     = CloneWatch.cpp
SRCMOD_STAT	      = Stat.cpp
SRCMOD_WORDFILTER     = WordFilter.cpp
SRCMOD_ANTIFLOOD      = AntiFlood.cpp FloodEvents.cpp FloodEngine.cpp

OBJMOD_METRICS	      = $(addprefix $(LIBOBJDIR), $(addsuffix .o, $(basename $(SRCMOD_METRICS))))
OBJMOD_CLONEWATCH     = $(addprefix $(LIBOBJDIR), $(addsuffix .o, $(basename $(SRCMOD_CLONEWATCH))))
OBJMOD_STAT	      = $(addprefix $(LIBOBJDIR), $(addsuffix .o, $(basename $(SRCMOD_STAT))))
OBJMOD_WORDFILTER     = $(addprefix $(LIBOBJDIR), $(addsuffix .o, $(basename $(SRCMOD_WORDFILTER))))
OBJMOD_ANTIFLOOD      = $(addprefix $(LIBOBJDIR), $(addsuffix .o, $(basename $(SRCMOD_ANTIFLOOD))))

SRCMOD		      = $(SRCMOD_METRICS) $(SRCMOD_CLONEWATCH) $(SRCMOD_STAT) $(SRCMOD_WORDFILTER) $(SRCMOD_ANTIFLOOD)

# object files
OBJMAIN		      = $(addsuffix .o, $(basename $(SRCMAIN)))
OBJCMD		      = $(addsuffix .o, $(basename $(SRCCMD)))
OBJDCMD		      = $(addsuffix .o, $(basename $(SRCDCMD)))
OBJRESP		      = $(addsuffix .o, $(basename $(SRCRESP)))

# all module targets
MODULES		      = metrics clonewatch stat wordfilter antiflood

# all objects
OBJALL		      = $(addprefix $(OBJDIR), $(OBJMAIN) $(OBJCMD) $(OBJDCMD) $(OBJRESP))

# include folders and directives
INCLUDELOCATIONS      = $(SRCMAINDIR) $(SRCCMDDIR) $(SRCDCMDDIR) $(SRCRESPDIR) $(SRCMODDIR) 
INC		      = $(addprefix -I, $(INCLUDELOCATIONS))

# referenced libraries and directives
REFERENCEDLIBRARIES   = dl rt
LIB                   = $(addprefix -l, $(REFERENCEDLIBRARIES))

# object directory
OBJDIR                = $(ROOTDIR)obj/
LIBOBJDIR	      = $(OBJDIR)lib/

# output library directory (for modules)
LIBDIR		      = $(ROOTDIR)lib/

# general compiler options
CXX		      = g++
CXXFLAGS	      = $(INC) -g
CXXLINK		      = -rdynamic

# compiler options for modules
CXXLIBFLAGS	      = $(INC) -g -fPIC
CXXLIBLINK	      = -shared

#################################################################################################
#	TARGETS

# implicit target
all: $(EXECNAME) modules

# module requisites
metrics:    OBJ_CURRENTMOD := $(OBJMOD_METRICS)
clonewatch: OBJ_CURRENTMOD := $(OBJMOD_CLONEWATCH)
stat:       OBJ_CURRENTMOD := $(OBJMOD_STAT)
wordfilter: OBJ_CURRENTMOD := $(OBJMOD_WORDFILTER)
antiflood:  OBJ_CURRENTMOD := $(OBJMOD_ANTIFLOOD)

antiflood:  $(OBJMOD_ANTIFLOOD) $(LIBDIR)libantiflood.so
metrics:    $(OBJMOD_METRICS) $(LIBDIR)libmetrics.so
clonewatch: $(OBJMOD_CLONEWATCH) $(LIBDIR)libclonewatch.so
stat:       $(OBJMOD_STAT) $(LIBDIR)libstat.so
wordfilter: $(OBJMOD_WORDFILTER) $(LIBDIR)libwordfilter.so

# target for primary executable
$(EXECNAME): $(OBJALL)
	$(CXX) $(CXXLINK) -o $@ $(OBJALL) $(LIB)

# target to build all modules
modules: $(MODULES)

# general object file builder rule
$(OBJDIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

# library object file builder rule
$(LIBOBJDIR)%.o: %.cpp
	$(CXX) $(CXXLIBFLAGS) -c -o $@ $^

# general library file builder rule
.SECONDEXPANSION:
$(LIBDIR)%.so: $$(OBJ_CURRENTMOD)
	$(CXX) $(CXXLIBLINK) -o $@ $(OBJ_CURRENTMOD) $(LIB)
# test targets
test:
	cd $(TESTDIR); $(ROOTDIR)$(EXECNAME) -nd;
debug:
	cd $(TESTDIR); gdb $(ROOTDIR)$(EXECNAME);
start:
	cd $(LIVEDIR); $(ROOTDIR)$(EXECNAME);

# store both target
store: storelive storetest

# store targets for test and live
storelive: $(EXECNAME)
	-rm $(LIVEDIR)$(EXECNAME)
	-rm $(LIVELIB)*.so
	cp $(ROOTDIR)$(EXECNAME) $(LIVEDIR)
	cp $(LIBDIR)*.so $(LIVELIB)

storetest: $(EXECNAME)
	-rm $(TESTDIR)$(EXECNAME)
	-rm $(TESTLIB)*.so
	cp $(ROOTDIR)$(EXECNAME) $(TESTDIR)
	cp $(LIBDIR)*.so $(TESTLIB)

# store targets for modules
storetestmod:
	-rm $(TESTLIB)*.so
	cp $(LIBDIR)*.so $(TESTLIB)

storelivemod:
	-rm $(LIVELIB)*.so
	cp $(LIBDIR)*.so $(LIVELIB)

# create all necessary directories just in case one is missing for some reason
dirs:
	-mkdir $(TESTDIR)
	-mkdir $(LIVEDIR)
	-mkdir $(TESTLIB)
	-mkdir $(LIVELIB)
	-mkdir $(OBJDIR)
	-mkdir $(LIBDIR)
	-mkdir $(LIBOBJDIR)

# resets the test directory configuration from the read only one
resettest:
	-rm $(TESTDIR)IRCBot.*
	-rm -rf $(TESTDIR)modules
	cp -R $(TESTCONFRO)* $(TESTDIR)

# deletes main program binary compiled shiz
clean:
	-rm -- $(ROOTDIR)$(EXECNAME) $(OBJDIR)*.o

# deletes shiz compiled for modules
modclean:
	-rm -- $(LIBOBJDIR)*.o $(LIBDIR)*.so

# deletes all the compiled things
realclean: clean modclean

# dumps dependancies into a temp file for you to peruse at your leisure
depend: $(SRCMAIN) $(SRCCMD) $(SRCDCMD) $(SRCRESP) $(SRCMOD)
	$(CXX) -MM $(INC) $^ > dependancies.txt

# this is much better but unfortunately doesnt work.
# depend: $(SRCMAIN) $(SRCCMD) $(SRCDCMD) $(SRCRESP) $(SRCMOD)
#	cd $(SRCMAINDIR); $(CXX) -MM $(INC) $(SRCMAIN) > dependancies.txt;
#	cd $(SRCCMDDIR); $(CXX) -MM $(INC) $(SRCCMD) >> dependancies.txt;
#	cd $(SRCDCMDDIR); $(CXX) -MM $(INC) $(SRCDCMD) >> dependancies.txt;
#	cd $(SRCRESPDIR); $(CXX) -MM $(INC) $(SRCRESP) >> dependancies.txt;
#	cd $(SRCMODDIR); $(CXX) -MM $(INC) $(SRCMOD) >> dependancies.txt;

#################################################################################################
#	DEPENDANCIES
# generated dependancies.  not quite autogenerated yet.  use make depend to generate something like this, then strip all directories

# DO NOT DELETE THIS LINE # MAKE DEPEND #

BotInfo.o: BotInfo.cpp BotInfo.h
BotPermissions.o: BotPermissions.cpp BotPermissions.h Hostname.h IRCBot.h IRCBotNetIface.h misc.h
Bottime.o: Bottime.cpp Bottime.h
CommandLine.o: CommandLine.cpp CommandLine.h BotInfo.h misc.h
Hostname.o: Hostname.cpp Hostname.h misc.h
HTTP.o: HTTP.cpp IRCBotNetIface.h HTTP.h misc.h
IRCBot.o: IRCBot.cpp IRCBot.h IRCBotNetIface.h Hostname.h BotInfo.h BotPermissions.h UserManager.h ModuleHandler.h misc.h Message.h DCmd.h DeferredAccessCheck.h dcmd.h Hostname.h CommandCaller.h DeferredAllow.h DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h MessageHandler.h ServerPinger.h MonitorWatcher.h IRC.h Cmd.h CmdAccess.h cmd.h CmdAlias.h CmdAllow.h CmdBan.h CmdCtcpResp.h CmdCheck.h CmdDie.h CmdDNS.h CmdExempt.h CmdFactoid.h CmdFlags.h CmdFwd.h CmdHaspaid.h CmdHelp.h CmdIp.h CmdJoin.h CmdKick.h CmdLong.h CmdMemo.h CmdModule.h CmdMonitor.h CmdMode.h CmdNick.h CmdOn.h CmdOnUser.h CmdOp.h CmdOption.h CmdPart.h CmdPoke.h CmdPrefix.h CmdQueue.h CmdQuiet.h CmdQuit.h CmdRaw.h CmdRepeat.h CmdSay.h CmdSave.h CmdTell.h CmdTime.h CmdTopic.h CmdVoice.h CmdWhere.h Rsp.h RspAccount.h rsp.h RspConnect.h RspInvite.h types.h RspJoin.h RspKick.h RspMOTD.h RspNick.h RspNickInUse.h RspNotice.h RspPart.h RspPrivmsg.h RspQuit.h errors.h ServerProperties.h Timer.h Bottime.h Scheduler.h EventActions.h
IRCBotNetIface.o: IRCBotNetIface.cpp IRCBot.h IRCBotNetIface.h Hostname.h Timer.h errors.h Bottime.h
IRC.o: IRC.cpp IRC.h Hostname.h ServerProperties.h misc.h
main.o: main.cpp IRCBot.h IRCBotNetIface.h Hostname.h BotInfo.h IRC.h Cmd.h CmdAccess.h cmd.h Hostname.h CmdAlias.h CmdAllow.h CmdBan.h CmdCtcpResp.h CmdCheck.h CmdDie.h CmdDNS.h CmdExempt.h CmdFactoid.h CmdFlags.h CmdFwd.h CmdHaspaid.h CmdHelp.h CmdIp.h CmdJoin.h CmdKick.h CmdLong.h CmdMemo.h CmdModule.h CmdMonitor.h CmdMode.h CmdNick.h CmdOn.h CmdOnUser.h CmdOp.h CmdOption.h CmdPart.h CmdPoke.h CmdPrefix.h CmdQueue.h CmdQuiet.h CmdQuit.h CmdRaw.h CmdRepeat.h CmdSay.h CmdSave.h CmdTell.h CmdTime.h CmdTopic.h CmdVoice.h CmdWhere.h misc.h CommandLine.h
Message.o: Message.cpp Message.h Hostname.h misc.h
Messenger.o: Messenger.cpp Messenger.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h
misc.o: misc.cpp misc.h
ModuleHandler.o: ModuleHandler.cpp ModuleHandler.h Hostname.h IRCBot.h IRCBotNetIface.h module.h Messenger.h
ServerPingThread.o: ServerPingThread.cpp ServerPingThread.h Hostname.h IRCBotNetIface.h IRCBot.h misc.h
ServerProperties.o: ServerProperties.cpp ServerProperties.h misc.h
Timer.o: Timer.cpp Timer.h
UserManager.o: UserManager.cpp Hostname.h UserManager.h misc.h
Scheduler.o: Scheduler.cpp Scheduler.h Bottime.h EventActions.h IRCBot.h IRCBotNetIface.h Hostname.h
module.o: module.cpp module.h Hostname.h Messenger.h IRCBot.h IRCBotNetIface.h
EventActions.o: EventActions.cpp EventActions.h IRCBot.h IRCBotNetIface.h Hostname.h
alias.o: alias.cpp IRCBot.h IRCBotNetIface.h Hostname.h alias.h cmd.h Hostname.h misc.h
CmdAccess.o: CmdAccess.cpp CmdAccess.h cmd.h Hostname.h IgnoreAccessModifier.h dcmd.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h misc.h
CmdAlias.o: CmdAlias.cpp CmdAlias.h cmd.h Hostname.h alias.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdAllow.o: CmdAllow.cpp CmdAllow.h cmd.h Hostname.h DeferredAllow.h dcmd.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h UserManager.h misc.h
CmdBan.o: CmdBan.cpp CmdBan.h cmd.h Hostname.h ModeWatcher.h dcmd.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdCheck.o: CmdCheck.cpp CmdCheck.h cmd.h Hostname.h ServerPinger.h dcmd.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h ServerPingThread.h misc.h
cmd.o: cmd.cpp cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h UserManager.h
CmdCtcpResp.o: CmdCtcpResp.cpp CmdCtcpResp.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdDie.o: CmdDie.cpp CmdDie.h cmd.h Hostname.h
CmdDNS.o: CmdDNS.cpp misc.h IRCBot.h IRCBotNetIface.h Hostname.h CmdDNS.h cmd.h Hostname.h
CmdExempt.o: CmdExempt.cpp CmdExempt.h cmd.h Hostname.h ModeWatcher.h dcmd.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdFactoid.o: CmdFactoid.cpp CmdFactoid.h cmd.h Hostname.h FactoidProcessor.h dcmd.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h misc.h
CmdFlags.o: CmdFlags.cpp CmdFlags.h cmd.h Hostname.h UserManager.h DeferredFlags.h dcmd.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdFwd.o: CmdFwd.cpp CmdFwd.h cmd.h Hostname.h Forwarder.h dcmd.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdHaspaid.o: CmdHaspaid.cpp CmdHaspaid.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h HTTP.h
CmdHelp.o: CmdHelp.cpp CmdHelp.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h
CmdIP.o: CmdIP.cpp CmdIp.h cmd.h Hostname.h DCmd.h DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h MessageHandler.h ServerPinger.h MonitorWatcher.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdJoin.o: CmdJoin.cpp CmdJoin.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h misc.h
CmdKick.o: CmdKick.cpp CmdKick.h cmd.h Hostname.h KickWatcher.h dcmd.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdLong.o: CmdLong.cpp CmdLong.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h HTTP.h misc.h
CmdMemo.o: CmdMemo.cpp CmdMemo.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h MemoSender.h dcmd.h
CmdMode.o: CmdMode.cpp CmdMode.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h misc.h
CmdModule.o: CmdModule.cpp CmdModule.h cmd.h Hostname.h module.h Hostname.h Messenger.h IRCBot.h IRCBotNetIface.h ModuleHandler.h misc.h
CmdMonitor.o: CmdMonitor.cpp CmdMonitor.h cmd.h Hostname.h MonitorWatcher.h dcmd.h BotPermissions.h Hostname.h misc.h IRCBot.h IRCBotNetIface.h IRC.h
CmdNick.o: CmdNick.cpp CmdNick.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h
CmdOn.o: CmdOn.cpp CmdOn.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h misc.h DCmd.h DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h MessageHandler.h ServerPinger.h MonitorWatcher.h
CmdOnUser.o: CmdOnUser.cpp CmdOnUser.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h UserWatcher.h dcmd.h
CmdOp.o: CmdOp.cpp CmdOp.h cmd.h Hostname.h ModeWatcher.h dcmd.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdOption.o: CmdOption.cpp CmdOption.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h Timer.h BotPermissions.h misc.h types.h RspInvite.h rsp.h
CmdPart.o: CmdPart.cpp CmdPart.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdPoke.o: CmdPoke.cpp CmdPoke.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h
CmdPrefix.o: CmdPrefix.cpp CmdPrefix.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h
CmdQueue.o: CmdQueue.cpp CmdQueue.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h misc.h
CmdQuiet.o: CmdQuiet.cpp CmdQuiet.h cmd.h Hostname.h ModeWatcher.h dcmd.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdQuit.o: CmdQuit.cpp CmdQuit.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdRaw.o: CmdRaw.cpp CmdRaw.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h
CmdRepeat.o: CmdRepeat.cpp CmdRepeat.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdSave.o: CmdSave.cpp CmdSave.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h UserManager.h BotPermissions.h misc.h module.h Messenger.h ModuleHandler.h
CmdSay.o: CmdSay.cpp CmdSay.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h misc.h
CmdTell.o: CmdTell.cpp CmdTell.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h misc.h
CmdTime.o: CmdTime.cpp CmdTime.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h Bottime.h misc.h
CmdTopic.o: CmdTopic.cpp CmdTopic.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdVoice.o: CmdVoice.cpp CmdVoice.h cmd.h Hostname.h ModeWatcher.h dcmd.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
CmdWhere.o: CmdWhere.cpp CmdWhere.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h
CommandCaller.o: CommandCaller.cpp CommandCaller.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h cmd.h
dcmd.o: dcmd.cpp dcmd.h Hostname.h
DeferredAccessCheck.o: DeferredAccessCheck.cpp DeferredAccessCheck.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
DeferredAllow.o: DeferredAllow.cpp DeferredAllow.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h cmd.h
DeferredFlags.o: DeferredFlags.cpp DeferredFlags.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h cmd.h
FactoidProcessor.o: FactoidProcessor.cpp FactoidProcessor.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h IRC.h BotPermissions.h misc.h
Forwarder.o: Forwarder.cpp Forwarder.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
IgnoreAccessModifier.o: IgnoreAccessModifier.cpp IgnoreAccessModifier.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h misc.h
IPReader.o: IPReader.cpp IPReader.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
KickWatcher.o: KickWatcher.cpp KickWatcher.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
MemoSender.o: MemoSender.cpp MemoSender.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
MessageHandler.o: MessageHandler.cpp MessageHandler.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
ModeWatcher.o: ModeWatcher.cpp ModeWatcher.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
MonitorWatcher.o: MonitorWatcher.cpp MonitorWatcher.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h misc.h
ServerPinger.o: ServerPinger.cpp misc.h ServerPingThread.h Hostname.h ServerPinger.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h
UserWatcher.o: UserWatcher.cpp UserWatcher.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
WhoTracker.o: WhoTracker.cpp WhoTracker.h dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h UserManager.h misc.h
RspAccount.o: RspAccount.cpp RspAccount.h rsp.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h UserManager.h misc.h
RspConnect.o: RspConnect.cpp RspConnect.h rsp.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h misc.h
rsp.o: rsp.cpp rsp.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h
RspInvite.o: RspInvite.cpp RspInvite.h rsp.h Hostname.h types.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h misc.h
RspJoin.o: RspJoin.cpp RspJoin.h rsp.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h UserManager.h misc.h
RspKick.o: RspKick.cpp RspKick.h rsp.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h UserManager.h misc.h
RspMOTD.o: RspMOTD.cpp RspMOTD.h rsp.h Hostname.h
RspNick.o: RspNick.cpp RspNick.h rsp.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h UserManager.h misc.h
RspNickInUse.o: RspNickInUse.cpp RspNickInUse.h rsp.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h misc.h
RspNotice.o: RspNotice.cpp RspNotice.h rsp.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h
RspPart.o: RspPart.cpp RspPart.h rsp.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h UserManager.h misc.h
RspPrivmsg.o: RspPrivmsg.cpp RspPrivmsg.h rsp.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h BotPermissions.h misc.h
RspQuit.o: RspQuit.cpp RspQuit.h rsp.h Hostname.h IRCBot.h IRCBotNetIface.h Hostname.h UserManager.h misc.h
Metrics.o: Metrics.cpp Metrics.h Hostname.h module.h Hostname.h Messenger.h IRCBot.h IRCBotNetIface.h UserManager.h misc.h
CloneWatch.o: CloneWatch.cpp CloneWatch.h Scheduler.h Bottime.h EventActions.h IRCBot.h IRCBotNetIface.h Hostname.h module.h Messenger.h Bottime.h IRCBot.h UserManager.h misc.h
Stat.o: Stat.cpp Stat.h module.h Hostname.h Messenger.h LineDBClass.h LineDB.h Bottime.h misc.h
WordFilter.o: WordFilter.cpp WordFilter.h module.h Hostname.h Messenger.h StringMatchTree.h IRCBot.h IRCBotNetIface.h misc.h
AntiFlood.o: AntiFlood.cpp AntiFlood.h module.h Hostname.h Messenger.h FloodEvents.h Bottime.h CQueue.h Scheduler.h Bottime.h EventActions.h IRCBot.h IRCBotNetIface.h FloodEngine.h Messenger.h Hostname.h IRCBot.h UserManager.h misc.h
FloodEvents.o: FloodEvents.cpp FloodEvents.h Bottime.h
FloodEngine.o: FloodEngine.cpp FloodEngine.h FloodEvents.h Bottime.h Messenger.h Hostname.h Scheduler.h Bottime.h EventActions.h IRCBot.h IRCBotNetIface.h Hostname.h IRCBot.h
