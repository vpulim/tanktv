#!/bin/sh

msg()
{
    echo "Please use the Gaya Web Services interface to start TankTV"
}

stop()
{
    /share/Apps/TankTV/daemon.cgi stop
}

case "$1" in
    "start")
        msg
        ;;
        
    "stop")
        stop
        ;;
        
    "restart")
	msg
        ;;
esac
