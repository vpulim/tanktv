#!/bin/sh

start()
{
    if [ -z "`pidof tanktv`" ]; then     
        if [ -n "`pidof gaya`" ]; then     
            kill -2 `pidof gaya` 2>&1 >/dev/null
        fi

        if [ "$?" == "0" ]; then
            sleep 3
        fi
    
        if [ -n "`pidof gaya`" ]; then     
            kill -9 `pidof gaya` 2>&1 >/dev/null
        fi

        if [ "$?" == "0" ]; then
            sleep 3
        fi
    
	cd /share/Apps/TankTV
        LD_LIBRARY_PATH=lib ./tanktv >/tmp/tanktv.log 2>&1 &
    fi
}

stop()
{
    if [ -n "`pidof tanktv`" ]; then
        kill -2 `pidof tanktv` 2>&1 >/dev/null
    fi
    
    if [ "$?" == "0" ]; then
        sleep 3
    fi
    
    if [ -n "`pidof tanktv`" ]; then
        kill -9 `pidof tanktv` 2>&1 >/dev/null
    fi

    if [ "$?" == "0" ]; then
        sleep 3
    fi

    if [ -z "`pidof gaya`" ]; then     
        /bin/gaya &
    fi
}

case "$1" in
    start)
    start
    ;;
    
    stop)
    stop
    ;;
    
    restart)
    stop
    sleep 1
    start
    ;;
    
    *)
    echo "Usage: {start|stop|restart}"
    ;;
esac
