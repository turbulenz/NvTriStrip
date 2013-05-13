.PHONY: all
all: NvTriStripper

.PHONY: clean
clean: NvTriStripClean NvTriStripperClean

.PHONY: NvTriStrip
NvTriStrip:
	make -C NvTriStrip

.PHONY: NvTriStripClean
NvTriStripClean:
	make -C NvTriStrip clean

.PHONY: NvTriStripper
NvTriStripper: NvTriStrip
	make -C NvTriStripper

.PHONY: NvTriStripperClean
NvTriStripperClean:
	make -C NvTriStripper clean
