CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++20

SRC_DIR = src
BUILD_DIR = build

YIP_SRC = $(SRC_DIR)/yip.cpp
VIXEN_SRC = $(SRC_DIR)/vixen.cpp

YIP_BIN = $(BUILD_DIR)/yip
VIXEN_BIN = $(BUILD_DIR)/vixen

all: yip vixen

yip: $(BUILD_DIR) $(YIP_BIN)

vixen: $(BUILD_DIR) $(VIXEN_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(YIP_BIN): $(YIP_SRC)
	$(CXX) $(CXXFLAGS) $(YIP_SRC) -o $(YIP_BIN)

$(VIXEN_BIN): $(VIXEN_SRC)
	$(CXX) $(CXXFLAGS) $(VIXEN_SRC) -o $(VIXEN_BIN)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all yip vixen clean
