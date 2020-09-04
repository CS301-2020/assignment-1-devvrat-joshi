all: build

build: binaries
	@echo "Building"
	gcc shell.c -o shell
	./shell
binaries:
	@echo "Building ls"
	gcc ls.c -o customls
	@echo "Building mkdir"
	gcc mkdir.c -o custommkdir
	@echo "Building rm"
	gcc rm.c -o customrm
	@echo "Building cat"
	gcc cat.c -o customcat
	@echo "Building grep"
	gcc grep.c -o customgrep
	@echo "Building chmod"
	gcc chmod.c -o customchmod
	@echo "Building cp"
	gcc cp.c -o customcp
	@echo "Building mv"
	gcc mv.c -o custommv