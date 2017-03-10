#!/bin/sh
#function change ip
#see ipconfig.ini
#call ipcfgdo.sh

if [ -r /mnt/user/xtdcu/set/djs/gddyjc/gd/ipconfig.ini ]; then
	IPCFGFILE=/mnt/user/xtdcu/set/djs/gddyjc/gd/ipconfig.ini
elif [ -r /mnt/xtdcu/set/djs/gddyjc/gd/ipconfig.ini ]; then
	IPCFGFILE=/mnt/xtdcu/set/djs/gddyjc/gd/ipconfig.ini
fi

if [ "$IPCFGFILE" == "" ]; then
	echo "Warning:ipconfig.ini is not find!!!"
	exit 1
else
	IP_eth0=$(grep '^IP_eth0' $IPCFGFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
	NETMASK_eth0=$(grep '^NETMASK_eth0' $IPCFGFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
	GW_eth0=$(grep '^GW_eth0' $IPCFGFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
	MAC_eth0=$(grep '^MAC_eth0' $IPCFGFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
fi

if [ "$IP_eth0" == "" ]; then
	IP_eth0=192.168.1.100
fi
if [ "$NETMASK_eth0" == "" ]; then
	NETMASK_eth0=255.255.255.0
fi
if [ "$GW_eth0" == "" ]; then
	GW_eth0=192.168.1.1
fi
if [ "$MAC_eth0" == "" ]; then
	MAC_eth0=00:01:02:B4:56:58
fi

/mnt/xtdcu/script/ipcfgdo.sh "eth0" "$MAC_eth0" "$IP_eth0" "$NETMASK_eth0" "$GW_eth0"


