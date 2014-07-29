## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e430 linker.cmd package/cfg/eink_pe430.oe430

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/eink_pe430.xdl
	$(SED) 's"^\"\(package/cfg/eink_pe430cfg.cmd\)\"$""\"D:/Studium/Projekte/Seminararbeit/quelltxt/eink/.config/xconfig_eink/\1\""' package/cfg/eink_pe430.xdl > $@
	-$(SETDATE) -r:max package/cfg/eink_pe430.h compiler.opt compiler.opt.defs
