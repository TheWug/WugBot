OBJ = alias.o BotPermissions.o BotInfo.o Cmdline.o cmd.o CmdAccess.o CmdAlias.o CmdAllow.o CmdBan.o CmdCtcpResp.o CmdDie.o CmdExempt.o CmdFactoid.o CmdFlags.o CmdFwd.o CmdHelp.o CmdIP.o CmdJoin.o CmdKick.o CmdMode.o CmdNick.o CmdOnUser.o CmdOn.o CmdOp.o CmdPart.o CmdPoke.o CmdPrefix.o CmdQueue.o CmdQuiet.o CmdQuit.o CmdRaw.o CmdSay.o CmdSave.o CmdTell.o CmdTopic.o CmdVoice.o dcmd.o Hostname.o IRCBot.o IRCBotNetIface.o IRC.o main.o Message.o misc.o rsp.o Rsp.o UserManager.o CommandCaller.o DeferredAccessCheck.o DeferredAllow.o DeferredFlags.o FactoidProcessor.o Forwarder.o IgnoreAccessModifier.o IPReader.o KickWatcher.o MessageHandler.o ModeWatcher.o UserWatcher.o WhoTracker.o Timer.o

IRCBot: $(OBJ)
	g++ -o IRCBot $(OBJ) -lrt
	mv IRCBot ..

alias.o: alias.cpp IRCBot.h IRCBotNetIface.h errors.h Hostname.h alias.h \
 cmd.h misc.h
BotInfo.o: BotInfo.cpp BotInfo.h
BotPermissions.o: BotPermissions.cpp BotPermissions.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h misc.h
CmdAccess.o: CmdAccess.cpp CmdAccess.h cmd.h Hostname.h DCmd.h \
 DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h \
 DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h \
 IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h \
 MessageHandler.h IRCBot.h IRCBotNetIface.h errors.h BotPermissions.h \
 misc.h
CmdAlias.o: CmdAlias.cpp CmdAlias.h cmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h misc.h alias.h
CmdAllow.o: CmdAllow.cpp CmdAllow.h cmd.h Hostname.h DCmd.h \
 DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h \
 DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h \
 IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h \
 MessageHandler.h IRCBot.h IRCBotNetIface.h errors.h BotPermissions.h \
 UserManager.h misc.h
CmdBan.o: CmdBan.cpp CmdBan.h cmd.h Hostname.h DCmd.h \
 DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h \
 DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h \
 IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h \
 MessageHandler.h IRCBot.h IRCBotNetIface.h errors.h misc.h
cmd.o: cmd.cpp cmd.h Hostname.h IRCBot.h IRCBotNetIface.h errors.h \
 BotPermissions.h UserManager.h
CmdCtcpResp.o: CmdCtcpResp.cpp CmdCtcpResp.h IRCBot.h IRCBotNetIface.h \
 errors.h Hostname.h
CmdDie.o: CmdDie.cpp CmdDie.h cmd.h Hostname.h
CmdExempt.o: CmdExempt.cpp CmdExempt.h cmd.h Hostname.h DCmd.h \
 DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h \
 DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h \
 IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h \
 MessageHandler.h IRCBot.h IRCBotNetIface.h errors.h misc.h
CmdFactoid.o: CmdFactoid.cpp CmdFactoid.h cmd.h Hostname.h DCmd.h \
 DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h \
 DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h \
 IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h \
 MessageHandler.h IRCBot.h IRCBotNetIface.h errors.h BotPermissions.h \
 misc.h
CmdFlags.o: CmdFlags.cpp CmdFlags.h cmd.h Hostname.h UserManager.h DCmd.h \
 DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h \
 DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h \
 IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h \
 MessageHandler.h IRCBot.h IRCBotNetIface.h errors.h misc.h
CmdFwd.o: CmdFwd.cpp CmdFwd.h cmd.h Hostname.h DCmd.h \
 DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h \
 DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h \
 IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h \
 MessageHandler.h IRCBot.h IRCBotNetIface.h errors.h misc.h
CmdHelp.o: CmdHelp.cpp CmdHelp.h cmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h
CmdIP.o: CmdIP.cpp CmdIp.h cmd.h Hostname.h DCmd.h DeferredAccessCheck.h \
 dcmd.h CommandCaller.h DeferredAllow.h DeferredFlags.h Forwarder.h \
 UserWatcher.h KickWatcher.h WhoTracker.h IPReader.h FactoidProcessor.h \
 ModeWatcher.h IgnoreAccessModifier.h MessageHandler.h IRCBot.h \
 IRCBotNetIface.h errors.h misc.h
CmdJoin.o: CmdJoin.cpp CmdJoin.h cmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h BotPermissions.h misc.h
CmdKick.o: CmdKick.cpp CmdKick.h cmd.h Hostname.h DCmd.h \
 DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h \
 DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h \
 IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h \
 MessageHandler.h IRCBot.h IRCBotNetIface.h errors.h misc.h
Cmdline.o: Cmdline.cpp Cmdline.h BotInfo.h misc.h
CmdMode.o: CmdMode.cpp CmdMode.h cmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h BotPermissions.h misc.h
CmdNick.o: CmdNick.cpp CmdNick.h cmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h
CmdOn.o: CmdOn.cpp CmdOn.h cmd.h Hostname.h DCmd.h DeferredAccessCheck.h \
 dcmd.h CommandCaller.h DeferredAllow.h DeferredFlags.h Forwarder.h \
 UserWatcher.h KickWatcher.h WhoTracker.h IPReader.h FactoidProcessor.h \
 ModeWatcher.h IgnoreAccessModifier.h MessageHandler.h IRCBot.h \
 IRCBotNetIface.h errors.h BotPermissions.h misc.h
CmdOnUser.o: CmdOnUser.cpp CmdOnUser.h cmd.h Hostname.h DCmd.h \
 DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h \
 DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h \
 IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h \
 MessageHandler.h IRCBot.h IRCBotNetIface.h errors.h misc.h
CmdOp.o: CmdOp.cpp CmdOp.h cmd.h Hostname.h DCmd.h DeferredAccessCheck.h \
 dcmd.h CommandCaller.h DeferredAllow.h DeferredFlags.h Forwarder.h \
 UserWatcher.h KickWatcher.h WhoTracker.h IPReader.h FactoidProcessor.h \
 ModeWatcher.h IgnoreAccessModifier.h MessageHandler.h IRCBot.h \
 IRCBotNetIface.h errors.h misc.h
CmdPart.o: CmdPart.cpp CmdPart.h cmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h misc.h
CmdPoke.o: CmdPoke.cpp CmdPoke.h cmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h
CmdPrefix.o: CmdPrefix.cpp CmdPrefix.h cmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h
CmdQueue.o: CmdQueue.cpp CmdQueue.h cmd.h Hostname.h DCmd.h \
 DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h \
 DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h \
 IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h \
 MessageHandler.h IRCBot.h IRCBotNetIface.h errors.h BotPermissions.h \
 misc.h
CmdQuiet.o: CmdQuiet.cpp CmdQuiet.h cmd.h Hostname.h DCmd.h \
 DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h \
 DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h \
 IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h \
 MessageHandler.h IRCBot.h IRCBotNetIface.h errors.h misc.h
CmdQuit.o: CmdQuit.cpp CmdQuit.h cmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h misc.h
CmdRaw.o: CmdRaw.cpp CmdRaw.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h \
 errors.h
CmdSay.o: CmdSay.cpp CmdSay.h cmd.h Hostname.h IRCBot.h IRCBotNetIface.h \
 errors.h BotPermissions.h misc.h
CmdSave.o: CmdSave.cpp CmdSave.h cmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h UserManager.h BotPermissions.h misc.h
CmdTell.o: CmdTell.cpp CmdTell.h cmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h BotPermissions.h misc.h
CmdTopic.o: CmdTopic.cpp CmdTopic.h cmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h misc.h
CmdVoice.o: CmdVoice.cpp CmdVoice.h cmd.h Hostname.h DCmd.h \
 DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h \
 DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h \
 IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h \
 MessageHandler.h IRCBot.h IRCBotNetIface.h errors.h misc.h
CommandCaller.o: CommandCaller.cpp CommandCaller.h dcmd.h Hostname.h \
 IRCBot.h IRCBotNetIface.h errors.h cmd.h BotPermissions.h
dcmd.o: dcmd.cpp dcmd.h Hostname.h
DeferredAccessCheck.o: DeferredAccessCheck.cpp DeferredAccessCheck.h \
 dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h errors.h misc.h
DeferredAllow.o: DeferredAllow.cpp DeferredAllow.h dcmd.h Hostname.h \
 IRCBot.h IRCBotNetIface.h errors.h cmd.h misc.h
DeferredFlags.o: DeferredFlags.cpp DeferredFlags.h dcmd.h Hostname.h \
 IRCBot.h IRCBotNetIface.h errors.h cmd.h misc.h
FactoidProcessor.o: FactoidProcessor.cpp FactoidProcessor.h dcmd.h \
 Hostname.h IRCBot.h IRCBotNetIface.h errors.h misc.h
Forwarder.o: Forwarder.cpp Forwarder.h dcmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h misc.h
Hostname.o: Hostname.cpp Hostname.h misc.h
IgnoreAccessModifier.o: IgnoreAccessModifier.cpp IgnoreAccessModifier.h \
 dcmd.h Hostname.h IRCBot.h IRCBotNetIface.h errors.h BotPermissions.h \
 misc.h
IPReader.o: IPReader.cpp IPReader.h dcmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h misc.h
IRCBot.o: IRCBot.cpp IRCBot.h IRCBotNetIface.h errors.h Hostname.h \
 BotInfo.h BotPermissions.h UserManager.h misc.h Message.h DCmd.h \
 DeferredAccessCheck.h dcmd.h CommandCaller.h DeferredAllow.h \
 DeferredFlags.h Forwarder.h UserWatcher.h KickWatcher.h WhoTracker.h \
 IPReader.h FactoidProcessor.h ModeWatcher.h IgnoreAccessModifier.h \
 MessageHandler.h IRC.h Cmd.h CmdAccess.h cmd.h CmdAlias.h CmdAllow.h \
 CmdBan.h CmdDie.h CmdExempt.h CmdFactoid.h CmdFlags.h CmdFwd.h CmdHelp.h \
 CmdIp.h CmdJoin.h CmdKick.h CmdMode.h CmdNick.h CmdOn.h CmdOnUser.h \
 CmdOp.h CmdPart.h CmdPoke.h CmdPrefix.h CmdQueue.h CmdQuiet.h CmdQuit.h \
 CmdRaw.h CmdSay.h CmdTell.h CmdTopic.h CmdVoice.h Rsp.h rsp.h
IRCBotNetIface.o: IRCBotNetIface.cpp IRCBot.h IRCBotNetIface.h errors.h \
 Hostname.h
IRC.o: IRC.cpp IRC.h Hostname.h misc.h
KickWatcher.o: KickWatcher.cpp KickWatcher.h dcmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h misc.h
main.o: main.cpp IRCBot.h IRCBotNetIface.h errors.h Hostname.h BotInfo.h \
 IRC.h Cmd.h CmdAccess.h cmd.h CmdAlias.h CmdAllow.h CmdBan.h CmdDie.h \
 CmdExempt.h CmdFactoid.h CmdFlags.h CmdFwd.h CmdHelp.h CmdIp.h CmdJoin.h \
 CmdKick.h CmdMode.h CmdNick.h CmdOn.h CmdOnUser.h CmdOp.h CmdPart.h \
 CmdPoke.h CmdPrefix.h CmdQueue.h CmdQuiet.h CmdQuit.h CmdRaw.h CmdSay.h \
 CmdTell.h CmdTopic.h CmdVoice.h misc.h Cmdline.h
Message.o: Message.cpp Message.h Hostname.h misc.h
MessageHandler.o: MessageHandler.cpp MessageHandler.h dcmd.h Hostname.h \
 IRCBot.h IRCBotNetIface.h errors.h misc.h
misc.o: misc.cpp misc.h
ModeWatcher.o: ModeWatcher.cpp ModeWatcher.h dcmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h misc.h
rsp.o: rsp.cpp rsp.h Hostname.h IRCBot.h IRCBotNetIface.h errors.h
Rsp.o: Rsp.cpp IRCBot.h IRCBotNetIface.h errors.h Hostname.h \
 BotPermissions.h UserManager.h Rsp.h rsp.h DCmd.h DeferredAccessCheck.h \
 dcmd.h CommandCaller.h DeferredAllow.h DeferredFlags.h Forwarder.h \
 UserWatcher.h KickWatcher.h WhoTracker.h IPReader.h FactoidProcessor.h \
 ModeWatcher.h IgnoreAccessModifier.h MessageHandler.h misc.h
UserManager.o: UserManager.cpp UserManager.h misc.h
UserWatcher.o: UserWatcher.cpp UserWatcher.h dcmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h misc.h
WhoTracker.o: WhoTracker.cpp WhoTracker.h dcmd.h Hostname.h IRCBot.h \
 IRCBotNetIface.h errors.h UserManager.h misc.h
Timer.o: Timer.cpp Timer.h

clean:
	rm *.o IRCBot

remake: clean IRCBot
