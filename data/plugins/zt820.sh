#!/bin/bash
# zt820.sh <PLUGIN> <ARGUMENTS>
ZT820_DIRECTORY="$PWD"
PLUGINS_DIRECTORY=plugins

if [ ! "$1" ] ; then
	echo "usage: $0 <PLUGIN> <ARGUMENTS>"
	exit
else
	PLUGIN_INPUT="$1"
fi

PLUGINS=`cd $ZT820_DIRECTORY/$PLUGINS_DIRECTORY/ && ls *.sh`
for PLUGIN in $PLUGINS; do
	if [ "$PLUGIN_INPUT" = "$PLUGIN" ] ; then
		exec $PLUGINS_DIRECTORY/$PLUGIN $2
	fi
done
