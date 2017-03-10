#!/bin/sh
#function: usb update control script

USBLOGFILE=/mnt/data/log/usb.log
USBROOTDIR=/usb/kge

#remove usb log
#rm -f $USBLOGFILE

#prompt
#echo "`date +%f`: Usb device plug in......" >> $USBLOGFILE

#copy system info
mkdir -p $USBROOTDIR
DEVFILE=/mnt/system/syscfg.ini
DEVADDR=$(grep '^RTUA' $DEVFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
CURDATE=$(date +%m%d%H%M)
if [ "$DEVADDR" == "" ]; then
	DEVADDR=unknow
fi
mkdir -p $USBROOTDIR/$DEVADDR
if [ -r $USBROOTDIR/$DEVADDR/$CURDATE.tgz ]; then
	echo "`date +%f`: Log file ($CURDATE.tgz) is existing!" >> $USBLOGFILE
else
	echo "`date +%f`: Copy system info start......" >> $USBLOGFILE
	tar -cvf $USBROOTDIR/$DEVADDR/$CURDATE.tgz /mnt/data /mnt/dyjc /mnt/system >> $USBLOGFILE
	echo "`date +%f`: Copy system info OK!" >> $USBLOGFILE
fi

#update system
USBUPDTKEY=$USBROOTDIR/update/update.key
USBUPDTTGZ=$USBROOTDIR/update/update.tgz
CURVERFILE=/mnt/system/sysrun.inf

if [ -r $USBUPDTKEY ]; then
	UPDTVER=$(grep '^MPVersion' $USBUPDTKEY | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
fi
if [ -r $CURVERFILE ]; then
	CURRVER=$(grep '^MPVersion' $CURVERFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
fi
if [ "$UPDTVER" == "" ]; then
	UPDTVER=0.00
fi
if [ "$CURRVER" == "" ]; then
	CURRVER=9.99
fi
if [ "$UPDTVER" \> "$CURRVER" ]; then
	echo "`date +%f`: System will update ($CURRVER -> $UPDTVER)" >> $USBLOGFILE
	if [ -r $USBUPDTTGZ ]; then
		mkdir -p /mnt/update
		cp -f $USBUPDTTGZ /mnt/update/update.tgz
		echo "`date +%f`: System will reboot......" >> $USBLOGFILE
		touch /mnt/update/auto_update.key
		exit 0
	fi
	echo "`date +%f`: $USBUPDTTGZ does not find!" >> $USBLOGFILE
elif [ -r $USBUPDTKEY ]; then
	echo "`date +%f`: Can't update, because of update version($UPDTVER) less than current system version($CURRVER)!" >> $USBLOGFILE
else
	echo "`date +%f`: Not find update key: $USBUPDTKEY" >> $USBLOGFILE
fi

#exec user script
USERSCRIPT=$USBROOTDIR/kgeusbop.sh
if [ -r $USERSCRIPT ]; then
	echo "`date +%f`: Exec user script($USERSCRIPT) start......" >> $USBLOGFILE
	cp -f $USERSCRIPT /tmp/kgeusbop.sh
	chmod +x /tmp/kgeusbop.sh
	/tmp/kgeusbop.sh
	echo "`date +%f`: Exec user script OK!" >> $USBLOGFILE
else
	echo "`date +%f`: Not find user script: $USERSCRIPT" >> $USBLOGFILE
fi

#umount usb device
umount /usb

