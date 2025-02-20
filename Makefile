CONFIG ?= release

XMAKE = xmake
BUILD_DIR = build

.PHONY: all clean rebuild run install format lint

all:
	$(XMAKE) f -m $(CONFIG)
	$(XMAKE) build -vD elysium
	$(XMAKE) build -vD app

clean:
	$(XMAKE) clean
	@powershell -Command "if (Test-Path 'build') { Remove-Item -Recurse -Force 'build' }"
	@powershell -Command "if (Test-Path 'lib') { Remove-Item -Recurse -Force 'lib' }"
	@powershell -Command "if (Test-Path 'includes') { Remove-Item -Recurse -Force 'includes' }"

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

debug:
	$(MAKE) all CONFIG=debug

release:
	$(MAKE) all CONFIG=release
