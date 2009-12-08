#!/bin/sh

#move to correct folder fist
cd /share/Apps/TankTV

PATH=/bin:/usr/bin

start()
{
    if [ -z "`pidof tanktv`" ]; then     
        if [ -n "`pidof gaya`" ]; then     
            kill -2 `pidof gaya` 2>&1 >/dev/null
        fi
        LD_LIBRARY_PATH=/share/Apps/TankTV/lib ./tanktv >/tmp/tanktv.log 2>&1 &
    fi
}

stop()
{
    if [ -n "`pidof tanktv`" ]; then
        kill -2 `pidof tanktv` 2>&1 >/dev/null
    fi
    
    if [ "$?" == "0" ]; then
    waitforclose
    fi
    
    if [ -n "`pidof tanktv`" ]; then
        kill -9 `pidof tanktv` 2>&1 >/dev/null
    fi

    if [ "$?" == "0" ]; then
    waitforclose
    fi

    if [ -z "`pidof gaya`" ]; then     
        gaya &
    fi
}

waitforclose()
{
    retry=0
        while [ -n "`pidof tanktv`" ] && [ "$retry" -le "10" ] ; do
            retry=$(( $retry + 1 ))
                sleep 1
    done
}

case "$1" in
    "start")
        start
        ;;
        
    "stop")
        stop
        ;;
        
    "restart")
        stop
        start
        ;;
esac
