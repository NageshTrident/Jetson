#                                                                                 
#     Makefile        External        31 MAY 2018            
#                                                                                 
#     Copyright (C)  2009-2018  EPIX, Inc.  All rights reserved. 
#                                                                                 
#     THIS IS A MACHINE GENERATED COPY                                            
#                                                                                 
MAJVERSION := $(shell uname -r | sed -e s/-/./ | cut -f1-2 -d. | sed -e s/3.*/3.x/ | sed -e s/4.*/4.x/)
TARGETARCH := $(shell uname -m | sed -e s/i.86/i386/ | sed -e s/armv7l/armv7l/ | sed -e s/aarch64/aarch64/ )


# Link w. PIXCI driver symbols. This assumes XCAP's tree layout
# But doesn't work properly on all kernels, thus copy, below
KBUILD_EXTRA_SYMBOLS := $(subst ttysrc,src,$(shell pwd))/Module.symvers

# invoked from the 2.6, 3.x, or 4.x kernel build system?
ifdef KERNELRELEASE
obj-m := pixcitty.o
endif

# invoked from user or surrogate?
default:
ifeq "$(MAJVERSION)" "2.4"
	 gcc -O2 -minline-all-stringops -I/lib/modules/$(shell uname -r)/build/include -D__KERNEL__ -DMODULE pixcitty.c
	 strip --strip-debug --discard-all pixcitty.o
	 mv pixcitty.o pixcitty_$(TARGETARCH).o
	 rm pixcitty.o
endif
ifneq "" "$(filter $(MAJVERSION), 2.6 3.x 4.x )"
	 cp $(KBUILD_EXTRA_SYMBOLS) .
	 $(MAKE) -C /lib/modules/$(shell uname -r)/build SUBDIRS=$(shell pwd) modules
	 strip --strip-debug --discard-all pixcitty.ko
	 mv pixcitty.ko pixcitty_$(TARGETARCH).ko
	 - rm pixcitty.o
	 - rm pixcitty.mod.o
	 - rm pixcitty.mod.c
	 - rm -f .pixcitty.mod.o.cmd
	 - rm -f .pixcitty.o.cmd
	 - rm -f .pixcitty.ko.cmd
	 - rm -f .pixcitty.o.cmd
	 - rm -rf .tmp_versions
endif

clean:
	 - rm -f *.o
	 - rm -f *.ko
	 - rm -f .pixcitty.mod.o.cmd
	 - rm -f .pixcitty.o.cmd
	 - rm -f .pixcitty.ko.cmd
	 - rm -f .pixcitty.o.cmd
	 - rm -rf .tmp_versions
