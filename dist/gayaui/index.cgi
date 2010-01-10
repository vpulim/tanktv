#!/bin/sh

echo "Content-Type: text/html"
echo
echo "<html><body>started...</body></html>"

if [ -d "/opt/sybhttpd/localhost.drives/HARD_DISK" ]; then
	DRIVE="HARD_DISK"
else
	DRIVE="SATA_DISK"
fi

wget -O /dev/null -q "http://localhost.drives:8883/$DRIVE/Apps/TankTV/daemon.cgi?start"
