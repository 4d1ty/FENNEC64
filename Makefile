CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++20

SRC_DIR = src
BUILD_DIR = build

YIP_SRC = $(SRC_DIR)/yip.cpp
VIXEN_SRC = $(SRC_DIR)/vixen.cpp

YIP_BIN = $(BUILD_DIR)/yip
VIXEN_BIN = $(BUILD_DIR)/vixen

SFML_INCLUDE = $(SRC_DIR)/include
SFML_LIB = $(SRC_DIR)/lib	

SFML_LIBS = \
	-lsfml-graphics-s \
	-lsfml-window-s \
	-lsfml-system-s \
	-lfreetype \
	-lopengl32 \
	-lgdi32 \
	-lwinmm \
	-lws2_32 \
	-lopenal \
	-lflac \
	-lvorbisenc \
	-lvorbisfile \
	-lvorbis \
	-logg

SFML_FLAGS = -DSFML_STATIC

all: yip vixen

yip: $(BUILD_DIR) $(YIP_BIN)

vixen: $(BUILD_DIR) $(VIXEN_BIN)


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(YIP_BIN): $(YIP_SRC)
	$(CXX) $(CXXFLAGS) $(YIP_SRC) -o $(YIP_BIN)

$(VIXEN_BIN): $(VIXEN_SRC)
	$(CXX) $(CXXFLAGS) \
		-I $(SFML_INCLUDE) \
		-L $(SFML_LIB) \
		$(VIXEN_SRC) -o $(VIXEN_BIN) \
		$(SFML_FLAGS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all yip vixen clean
