# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  lmake_config = debug

else ifeq ($(config),release)
  lmake_config = release

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := lmake

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

lmake:
ifneq (,$(lmake_config))
	@echo "==== Building lmake ($(lmake_config)) ===="
	@${MAKE} --no-print-directory -C . -f lmake.make config=$(lmake_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f lmake.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   lmake"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"