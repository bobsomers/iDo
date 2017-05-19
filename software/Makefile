COMPILER=c++
FLAGS=-std=c++11 -Wall -Werror -g -O2
BUILD_DIR=build

# Definitions for showtime binary.
SHOWTIME_BUILD_DIR=$(BUILD_DIR)/showtime
SHOWTIME_SRC_DIR=showtime
SHOWTIME_BIN=$(SHOWTIME_BUILD_DIR)/showtime
SHOWTIME_SRCS=$(wildcard $(SHOWTIME_SRC_DIR)/*.cpp)
SHOWTIME_OBJS=$(SHOWTIME_SRCS:%.cpp=$(BUILD_DIR)/%.o)
SHOWTIME_INCLUDES=-Inanogui/include \
				 					 -Inanogui/ext/eigen \
				 					 -Inanogui/ext/glfw/include \
				 					 -Inanogui/ext/nanovg/src \
				 					 -Iportaudio/include \
		 							 -Ifftw/fftw-3.3.6-pl2/api \
				 					 -I.
SHOWTIME_LIBS=-Lnanogui/build -lnanogui \
		 					-Lnanogui/build/ext_build/glfw/src -lglfw3 \
		 					-Lportaudio/build \
		 					-lportaudio \
		 					-Lfftw/fftw-3.3.6-pl2/.libs \
		 					-lfftw3f \
							-framework OpenGL \
							-framework Cocoa \
							-framework IOKit \
							-framework CoreFoundation \
							-framework CoreVideo

all: $(SHOWTIME_BIN)

.PHONY: all deps depclean clean builddir run

deps:
	git submodule update --init --recursive
	mkdir -p nanogui/build
	pushd nanogui/build && \
		cmake -DNANOGUI_BUILD_EXAMPLE:BOOL=OFF -DNANOGUI_BUILD_PYTHON:BOOL=OFF -DNANOGUI_INSTALL:BOOL=OFF -DNANOGUI_BUILD_SHARED:BOOL=OFF .. && \
		make -j4 && \
		popd

	mkdir -p portaudio/build
	pushd portaudio/build && \
		../configure && \
		make -j4 && \
		popd

	pushd fftw && \
		curl -O http://www.fftw.org/fftw-3.3.6-pl2.tar.gz && \
		tar -xzf fftw-3.3.6-pl2.tar.gz
		pushd fftw/fftw-3.3.6-pl2 && \
			./configure --enable-float && \
			make -j4 && \
			popd

depclean:
	rm -rf nanogui/build
	rm -rf portaudio/build

clean:
	rm -rf $(BUILD_DIR)

builddir:
	mkdir -p $(SHOWTIME_BUILD_DIR)

run: $(SHOWTIME_BIN)
	$(SHOWTIME_BIN)

$(SHOWTIME_BIN): builddir $(SHOWTIME_OBJS)
	$(COMPILER) $(FLAGS) $(SHOWTIME_LIBS) -o $(SHOWTIME_BIN) $(SHOWTIME_OBJS)

-include $(SHOWTIME_OBJS:.o=.d)

$(SHOWTIME_BUILD_DIR)/%.o: $(SHOWTIME_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(SHOWTIME_INCLUDES) -o $@ -c $<
	@$(COMPILER) $(FLAGS) $(SHOWTIME_INCLUDES) -MM $< > $(SHOWTIME_BUILD_DIR)/$*.d
	@sed -e 's|^|$(SHOWTIME_BUILD_DIR)/|' -i '' $(SHOWTIME_BUILD_DIR)/$*.d
