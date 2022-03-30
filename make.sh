#!/usr/bin/sh

ARGS=$*
DEBUG=0
OSCAR_HOME="${HOME}/compilers/c64/oscar64/bin"
OSCARC="$OSCAR_HOME/oscar64"
OSCAR_ARGS="-n -dNOFLOAT -dNOLONG -tf=crt"
if [ "$DEBUG" -eq 1 ];
then
    echo "$OSCARC $OSCAR_ARGS $ARGS"
else
    $OSCARC "$OSCAR_ARGS" "$ARGS"
fi