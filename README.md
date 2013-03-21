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

#comment
[Section]
Name=Value

The minimum needed to get the program running is not yet known.  Once I have decided
what it should be, I'll update this.
