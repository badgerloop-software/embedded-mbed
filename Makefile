# Allow the user to specify different values for 'PROFILE' and 'TOOLCHAIN'
PROFILE?=develop
TOOLCHAIN?=GCC_ARM

# Parse setup.yml to determine the build target
TARGET       := $(shell grep -Po '(^\s+target:\s+)\K.+' setup.yml)
# Check that a value for 'target' was found in setup.yml
ifndef TARGET
$(error Did not find a value for 'target' in setup.yml)
endif
BUILD_DIR    := cmake_build/$(TARGET)/$(PROFILE)/$(TOOLCHAIN)
BINARY       := embedded-mbed.bin
START_MACROS := ^\# --- START MACROS ---$$
END_MACROS   := ^\# ---- END MACROS ----$$
# Search for files matching *.{cpp,h}, but don't search mbed-os/ or cmake_build/
SOURCES      := $(shell find . -type d \( -path ./mbed-os -o -path ./cmake_build \) -prune -o -type f -regex '.*\.\(cpp\|h\)' -print)


.PHONY: flash
flash: mountpoint compile
	cp $(BUILD_DIR)/$(BINARY) $(MOUNTPOINT)

.PHONY: compile
compile: $(BUILD_DIR)/$(BINARY)

$(BUILD_DIR)/$(BINARY): $(BUILD_DIR)/mbed_config.cmake setup.yml tests.yml CMakeLists.txt parseConfigs.py $(SOURCES)
	./parseConfigs.py
	cmake -S . -B $(BUILD_DIR) -GNinja
	cmake --build $(BUILD_DIR)

.PHONY: configure
configure: $(BUILD_DIR)/mbed_config.cmake

$(BUILD_DIR)/mbed_config.cmake: mbed-os mbed_app.json
	mbed-tools configure -t $(TOOLCHAIN) -m $(TARGET)

.PHONY: deploy
deploy: mbed-os

mbed-os: mbed-os.lib
	mbed-tools deploy

# Check that MOUNTPOINT was defined as a CLA
.PHONY: mountpoint
mountpoint:
ifndef MOUNTPOINT
	$(error variable 'MOUNTPOINT' not set)
endif

.PHONY: clean
clean: clean-cmakelists
	rm -rf cmake_build/ mbed-os/

.PHONY: clean-cmakelists
clean-cmakelists:
	sed -i '/$(START_MACROS)/,/$(END_MACROS)/{/$(START_MACROS)/!{/$(END_MACROS)/!d}}' CMakeLists.txt
