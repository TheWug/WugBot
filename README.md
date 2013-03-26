WugBot IRC Bot v2
======

NOTICE:

This codebase lives primarily in a private SVN repository I host myself.
That repo has some 61 revisions, and a previous functioning iteration.

Much of v2 remains unwritten, and thus much remains untested.  It's probably not currently
fit for any purpose aside from code review.

Directions for Use:
======

command line flags:

	-n
	--no-daemon
		Skips detach from terminal (most network interaction will be displayed to stdout)

	-d [file]
	--debug [file]
		Write contents of DEBUG log to [file] (stdout if file is omitted).
		Normally this information is discarded
		
	-c <file>
	--config-file <file>
		Reads specified file as config file 
		If directive omitted, config read from "ircbot.conf" in working directory

Config file structure
======
The following is a short example of the configuration format.

	// comment
	/* block comment */
	[Section]
	Name=Value

The minimum needed to get the program running is not yet known.  Once I have decided
what it should be, I'll update this.

Config Documentation:

The values listed are the default values used by the program (if a value is requested but not
specified in the config file, the value shown here will be used).

	[GENERAL]
	readonly=0			// If this property is "1", the bot will NEVER save config file changes.
	discarddebug=1			// If this property is "1", the bot will not save RAWIO.log or DEBUG.log.
	
	[NETWORKS]
	autoconnect=net1,...		// the list of networks to automatically connect to upon startup, comma delimited.
	networks=net1,net2,...		// the list of all saved networks, comma delimited.
	authmode=advanced		// the default auth method used by the bot. can be 'advanced' or 'hostname'.
	
	[NETWORK:NET1]			// NET1 is a string pulled from NETWORKS/networks
	port=6667			// port to connect to
	server=irc.esper.net		// server to connect to
	autojoin=#chan1,#...		// the list of channels to automatically join on startup
	
	nickname=UnconfiguredBot	// comma delimited list of nicknames, tried in order.
	ident=PlzKill			// the ident to send to the server
	hostname=*			// the hostname to send to the server (most servers ignore this field)
	realname=WugBot IRC Bot v2	// the real name to send to the server.
	
	nickserv_password=		// bot's nickserv password.  If blank, no NS login will occur.
	nickserv_username=		// bot's nickserv name.  If blank, no NS login will occur.
	
	nickserv_identify_command=NS IDENTIFY %u %p 
	// nickserv authentication protocol. %u and %p are replaced with username and password.
	
	authmode=NETWORKS.authmode	// the authentication method to use on this network.
	ssl=0				// not yet supported; specify "1" to connect over SSL.
