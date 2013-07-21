===========
color
===========

color is opensource program that allows to colorize console output
eg. from compilation log, syslog or any other log displayed on unix console.
It allows to define your own set of coloring rules, but comes with predefined 
ready to used schemes for gcc output, syslog, apachelog etc. To see more, see 
/etc/color.conf for information about defined color schemes.

How to use color?
=========
usage:

[some command] | color [color-scheme]

examples:
$ cat /var/log/syslog | color syslog

$ cat /var/log/messages/ | color

$ make 2>&1 | color

