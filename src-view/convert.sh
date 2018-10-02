#!/bin/sh
# A small script to convert your PDF to any format, using ImageMagick convert. -S.H.
Format=gif
Arg=$1
if test "$1" = "--help" || test -z "$1"; then
printf "convert.sh [option] <input file>
Possible options:
	--help\t\tShow help
	--list\t\tShow possible output formats
	--format <f>\tSet output format (e.g. gif, mp4, jpg, ...)
	\t\tdefault: gif\n"
exit 0
elif test "$1" = "--format"; then
	Format=$2
	Arg=$3
elif test "$1" = "--list"; then
	convert -list format
	exit 0
fi

printf "Converting ${Arg}.pdf to ${Format}... "
convert -density 200 -delay 200 -background white -alpha remove ${Arg}.pdf ${Arg}.${Format}
printf "Done.\n"
