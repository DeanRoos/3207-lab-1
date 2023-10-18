MYps
Dean Roos
October 2023

USAGE:
MYps <opts>

Displays process info of all processes on machine matching user's uid

Options:
-p <pid> : displays process information only for given pid

-s : displays process state

-u = does NOT display utime (default is on)

-S : displays stime

-v : displays amount of virtual memory used by process

-c : does NOT display command line used to create process (defualt is on)

Program design:
Program uses a struct to store option data, individual functions to display each of the optionsn, and calls each of these funtions from main depending on options selected. Makes use of functions to create paths to /proc/ directory for specific PIDs. Parses stat, statm, cmdline, and status files in /proc/<pid>/ directory to obtain information.
