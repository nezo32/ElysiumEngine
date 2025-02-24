CONFIG ?= debug
XMAKE = xmake

.PHONY: all clean build run install format lint compile-shaders debug release

all: clean compile-shaders set-target build-engine build-app
build: compile-shaders set-target build-engine build-app

compile-shaders:
	compile.bat

run:
	$(XMAKE) run

install:
	$(XMAKE) install


set-target:
	$(XMAKE) f -m $(CONFIG)

build-engine:
	$(XMAKE) build -vD elysium

build-app:
	$(XMAKE) build -vD app


debug:
	$(MAKE) all CONFIG=debug

release:
	$(MAKE) all CONFIG=release

clean: clean-xmake clean-build clean-output

clean-xmake:
	$(XMAKE) clean

clean-build:
	@powershell -Command "if (Test-Path 'build') { Remove-Item -Recurse -Force 'build' }"

clean-output:
	@powershell -Command "if (Test-Path 'ElysiumEngine') { Remove-Item -Recurse -Force 'ElysiumEngine' }"


format:
	@if not exist "C:\Program Files\LLVM\bin\clang-format.exe" ( \
		echo clang-format not found! Install LLVM/Clang. ) else ( \
		for /r %%f in (engine\*.cpp engine\*.h app\*.h app\*.cpp) do "C:\Program Files\LLVM\bin\clang-format.exe" -i "%%f" \
	)
