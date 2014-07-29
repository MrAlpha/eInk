#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = D:/Programme/CCS6/grace_3_00_01_59/packages;D:/Programme/CCS6/msp430/MSP430ware_1_90_00_30/driverlib/packages;D:/Programme/CCS6/msp430/MSP430ware_1_90_00_30/driverlib;D:/Programme/CCS6/ccsv6/ccs_base;D:/Studium/Projekte/Seminararbeit/quelltxt/eink/.config
override XDCROOT = D:/Programme/CCS6/xdctools_3_30_03_47_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = D:/Programme/CCS6/grace_3_00_01_59/packages;D:/Programme/CCS6/msp430/MSP430ware_1_90_00_30/driverlib/packages;D:/Programme/CCS6/msp430/MSP430ware_1_90_00_30/driverlib;D:/Programme/CCS6/ccsv6/ccs_base;D:/Studium/Projekte/Seminararbeit/quelltxt/eink/.config;D:/Programme/CCS6/xdctools_3_30_03_47_core/packages;..
HOSTOS = Windows
endif
