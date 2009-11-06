#!/bin/sh

#move to correct folder fist
cd /share/Apps/TankTV

PATH=/bin:/usr/bin

start()
{
    if [ -z "`pidof lircd_apple`" ]; then     
        if [ -n "`pidof lircd`" ]; then     
            kill -2 `pidof lircd`
        fi
        /share/Apps/TankTV/lircd_apple        
    fi

    if [ -z "`pidof nmti`" ]; then     
        if [ -n "`pidof gaya`" ]; then     
            kill -2 `pidof gaya`
        fi
        LD_LIBRARY_PATH=/share/Apps/TankTV/lib ./nmti --dfb:mode=1280x720 >/tmp/nmti.log 2>&1 &
    fi
}

stop()
{
    if [ -z "`pidof lircd`" ]; then     
        if [ -n "`pidof lircd_apple`" ]; then     
            kill -2 `pidof lircd_apple` 2>&1 >/dev/null
        fi
        /bin/lircd
    fi

    if [ -n "`pidof nmti`" ]; then
        kill -2 `pidof nmti` 2>&1 >/dev/null
    fi
    
    if [ "$?" == "0" ]; then
    waitforclose
    fi
    
    if [ -n "`pidof nmti`" ]; then
        kill -9 `pidof nmti` 2>&1 >/dev/null
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
        while [ -n "`pidof nmti`" ] && [ "$retry" -le "10" ] ; do
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
