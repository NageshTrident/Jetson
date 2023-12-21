#!/bin/bash
#
# Install/Uninstall EPIX(R) PIXCI(R)
# pixci.o/pixci.ko and its pixci.rc.
# Does not compile pixci.o/pixci.ko.
# 18-Oct-2021
#
#
# First parameter: "install" or "uninstall"
#
# Second parameter: Absolute path to
# directory containing *.rc file, such as XCAP's
#   /usr/local/xcap/drivers
# or alternate directory. Default is '.'
#
# Third parameter: Absolute path to
# directory containing *.o/*.ko driver file, such as XCAP's
#   /usr/local/xcap/drivers/$(uname -m | sed -e s/i.86/i386/)/$(uname -r)
# or alternate directory. Default is '.'
#


if [ `id -u` -ne 0 ]; then
    echo "$0 must be run as root"
    exit 1
fi

src_dir2=$(pwd)
if [ -n "$2" ]; then
    src_dir2="$2"
fi
src_dir3=$(pwd)
if [ -n "$3" ]; then
    src_dir3="$3"
fi

if [ -d /etc/init.d ]; then
    # System V style
    script_dir=/etc/init.d
elif [ -d /etc/rc.d ]; then
    # BSD style
    script_dir=/etc/rc.d
else
    echo "Unrecognized init script system!" 1>&2
    exit 1
fi

if [ -d /etc/rc.d ]; then
    startup_dir=/etc/rc.d
else
    startup_dir=/etc
fi

ARCH=$(uname -m | sed -e s/i.86/i386/)
UNAMER=$(uname -r)
PIXCI_FILE=pixci_$ARCH.ko
PIXCI_FILE2=pixci.ko
if [ 5.10 == '2.4' ]; then
    PIXCI_FILE=pixci_$ARCH.o
    PIXCI_FILE2=pixci.o
fi

PIXCI_DIR=/lib/modules/$UNAMER/extra
if [ ! -d $PIXCI_DIR ]; then
    PIXCI_DIR=/lib/modules/$UNAMER/kernel/drivers/misc
fi
if [ ! -d $PIXCI_DIR ]; then
    PIXCI_DIR=/usr/sbin
fi

if [ "$1" == "install" ]; then
    if [ ! -f $src_dir3/$PIXCI_FILE ]; then
        echo "File $src_dir3/$PIXCI_FILE missing" 1>&2
        exit 1
    fi
    if [ ! -f $src_dir2/pixci.rc ]; then
        echo "File $src_dir2/pixci.rc missing" 1>&2
        exit 1
    fi
    install -c -m 555 -g root -o root $src_dir3/$PIXCI_FILE $PIXCI_DIR/$PIXCI_FILE2
    install -c -m 555 -g root -o root $src_dir2/pixci.rc $script_dir/pixci

    if which update-rc.d > /dev/null 2>&1; then
        update-rc.d pixci defaults 20
    else
        if [ -d $startup_dir/rc2.d ] ; then
            ln -sf $script_dir/pixci $startup_dir/rc2.d/S20pixci
        fi
        if [ -d $startup_dir/rc3.d ] ; then
            ln -sf $script_dir/pixci $startup_dir/rc3.d/S20pixci
        fi
        if [ -d $startup_dir/rc4.d ] ; then
            ln -sf $script_dir/pixci $startup_dir/rc4.d/S20pixci
        fi
        if [ -d $startup_dir/rc5.d ] ; then
            ln -sf $script_dir/pixci $startup_dir/rc5.d/S20pixci
        fi
    fi

    if which invoke-rc.d > /dev/null 2>&1; then
        # Debian requires ..
        invoke-rc.d pixci stop
        invoke-rc.d pixci start
    else
        sh $script_dir/pixci stop
        sh $script_dir/pixci start
    fi

elif [ "$1" == "uninstall" ]; then
    if which invoke-rc.d > /dev/null 2>&1; then
        # Debian requires ..
        invoke-rc.d pixci stop
    else
        sh $script_dir/pixci stop
    fi
    if which update-rc.d > /dev/null 2>&1; then
        rm -f $script_dir/pixci
        update-rc.d pixci remove
    else
        rm -f $startup_dir/rc2.d/S20pixci
        rm -f $startup_dir/rc3.d/S20pixci
        rm -f $startup_dir/rc4.d/S20pixci
        rm -f $startup_dir/rc5.d/S20pixci
        rm -f $script_dir/pixci
    fi
    rm -f $PIXCI_DIR/$PIXCI_FILE2
else
    echo "Usage: $0 {install|uninstall} "
    echo "Usage: $0 {install|uninstall} [dir w. pixci.rc] [dir w. pixci.ko]"
fi
