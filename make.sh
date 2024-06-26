#!/usr/bin/sh

ARGS=$*
DEBUG=0
OSCAR_HOME="${HOME}/oscar/oscar64/bin"
OSCARC="$OSCAR_HOME/oscar64"
OSCAR_ARGS="-n -dNOFLOAT -dNOLONG -tf=crt -pp"
if [ "$DEBUG" -eq 1 ];
then
    echo "$OSCARC $OSCAR_ARGS $ARGS"
else
    $OSCARC $OSCAR_ARGS $ARGS
fi