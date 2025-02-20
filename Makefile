CONFIG ?= release

XMAKE = xmake
BUILD_DIR = build

.PHONY: all clean rebuild run install format lint

all: compile-shaders
	$(XMAKE) f -m $(CONFIG)
	$(XMAKE) build -vD elysium
	$(XMAKE) build -vD app

clean:
	$(XMAKE) clean
	@powershell -Command "if (Test-Path 'build') { Remove-Item -Recurse -Force 'build' }"
	@powershell -Command "if (Test-Path 'ElysiumEngine') { Remove-Item -Recurse -Force 'ElysiumEngine' }"

rebuild: clean all

run:
	$(XMAKE) run

install:
	$(XMAKE) install

format:
	@if not exist "C:\Program Files\LLVM\bin\clang-format.exe" ( \
		echo clang-format not found! Install LLVM/Clang. ) else ( \
		for /r %%f in (src\*.cpp src\*.h) do "C:\Program Files\LLVM\bin\clang-format.exe" -i "%%f" \
	)

debug: clean
	$(MAKE) all CONFIG=debug

release: clean
	$(MAKE) all CONFIG=release

compile-shaders:
	compile.bat