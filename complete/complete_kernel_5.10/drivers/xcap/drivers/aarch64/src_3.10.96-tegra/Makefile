#                                                                                  
#     Makefile        External        31 MAY 2018             
#                                                                                  
#     Copyright (C)  2006-2018  EPIX, Inc.  All rights reserved.  
#                                                                                  
#     THIS IS A MACHINE GENERATED COPY                                             
#                                                                                  
MAJVERSION := $(shell uname -r | sed -e s/-/./ | cut -f1-2 -d. | sed -e s/3.*/3.x/ | sed -e s/4.*/4.x/)
TARGETARCH := $(shell uname -m | sed -e s/i.86/i386/ | sed -e s/armv7l/armv7l/ | sed -e s/aarch64/aarch64/ )


# invoked from the 2.6, 3.x, 4.x kernel build system?
ifdef KERNELRELEASE
obj-m := pixci.o
pixci-objs := pixcipub.o pixcilnx.o
endif

# invoked from user or surrogate?
default:
ifeq "$(MAJVERSION)" "2.4"
	 gcc -O2 -minline-all-stringops -I/lib/modules/$(shell uname -r)/build/include -D__KERNEL__ -DMODULE -c pixcipub.c
	 ar -x pixcilnx_$(TARGETARCH)_3.10.96-tegra.a pixcilnx_3.10.96-tegra.o
	 ld -r -x -o pixci.o pixcipub.o pixcilnx_2.4.o
	 strip --strip-debug --discard-all pixci.o
	 mv pixci.o pixci_$(TARGETARCH).o
	 rm pixcipub.o
	 rm pixcilnx_3.10.96-tegra.o
endif
ifneq "" "$(filter $(MAJVERSION), 2.6 3.x 4.x )"
	 ar -x pixcilnx_$(TARGETARCH)_3.10.96-tegra.a pixcilnx_3.10.96-tegra.o
	 mv pixcilnx_3.10.96-tegra.o pixcilnx.o_shipped
	 $(MAKE) -C /lib/modules/$(shell uname -r)/build SUBDIRS=$(shell pwd) modules
	 strip --strip-debug --discard-all pixci.ko
	 mv pixci.ko pixci_$(TARGETARCH).ko
	 - rm pixcilnx.o
	 - rm pixcilnx.o_shipped
	 - rm pixcipub.o
	 - rm pixci.o
	 - rm pixci.mod.o
	 - rm pixci.mod.c
	 - rm -f .pixci.mod.o.cmd
	 - rm -f .pixci.o.cmd
	 - rm -f .pixci.ko.cmd
	 - rm -f .pixcipub.o.cmd
	 - rm -rf .tmp_versions
endif

clean:
	 - rm -f *.o
	 - rm -f *.ko
	 - rm -f .pixci.mod.o.cmd
	 - rm -f .pixci.o.cmd
	 - rm -f .pixci.ko.cmd
	 - rm -f .pixcipub.o.cmd
	 - rm -rf .tmp_versions
