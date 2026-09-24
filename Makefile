CC_FILES := $(shell find src -type f -name "*.c" 2>/dev/null)
HH_FILES := $(shell find src -type f -name "*.h" 2>/dev/null)
ALL_FILES := $(CC_FILES) $(HH_FILES)

.PHONY: all format check help check_files

all: format check

check_files:
ifeq ($(strip $(ALL_FILES)),)
	$(error Error: No .c or .h files found in src/ directory!)
endif

format: check_files
	@echo "Formatting files..."
	@echo $(ALL_FILES) | xargs -n 1 clang-format -i -style="{BasedOnStyle: Google, IndentWidth: 4, ColumnLimit: 110}"
	@echo "Formatting complete."

check: check_files
	@echo "Running cppcheck..."
	@cppcheck --enable=warning,performance,portability,style --std=c11 \
		--quiet $(ALL_FILES) || echo "cppcheck finished with warnings"
	@echo "Code check complete."

help:
	@echo "Available commands:"
	@echo "  make format    - Format all files in src/"
	@echo "  make check     - Run cppcheck for src/"
	@echo "  make all       - Format and check code"
