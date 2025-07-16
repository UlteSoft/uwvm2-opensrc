# Supported platforms

## POSIX Environment
* Linux: (x86\_64, i386, aarch64, arm, loongarch64, powerpc64, riscv, mips, etc.)-linux-(gnu, android, ohos, musl, llvm, mlibc, uclibc, etc.)
* Darwin: (macosx, iphoneos, watchos): (x86\_64, i386, aarch64, arm, powerpc64)-apple-darwin
* FreeBSD: (x86\_64, i386, aarch64, arm, powerpc64, etc.)-freebsd(Version)
* NetBSD: unknown(50+)-netbsd
* OpenBSD: unknown-openbsd
* DragonflyBSD: (x86\_64, i386, aarch64, arm, powerpc64, etc.)-dragonfly
* WASM-WASI (self-hosting): (wasm32, wasm64)-(wasip1, wasip2)-(none, threads, neoh)
* SerenityOS: (x86\_64, i686)-serenity
* SunOS: (sparc, x86)-(sunos, solaris)-gnu

## Windows NT
* Supported system versions: Windows NT 5.0 (Windows 2000, Windows Server 2000), Windows NT 5.1 (Windows XP), Windows NT 5.2 (Windows XP 64bit, Windows Server 2003), Windows NT 6.0 (Windows Vista, Windows Server 2008), Windows NT 6.1 (Windows 7, Windows Server 2008 R2), Windows NT 6.2 (Windows 8, Windows Server 2012), Windows NT 6.3 (Windows Blue, Windows Server 2012 R2), Windows NT 10.0 (Windows 10, Windows Server 2016, Windows Server 2019, Windows Server 2022, Windows 11, Windows Server 2025)
* MSVC: (x86\_64, i686, aarch64, arm, arm64ec)-windows-msvc
* MinGW: (x86\_64, i686, aarch64, arm, arm64ec)-windows-gnu (aka. unknown(4)-w64-mingw32)

## Windows 9x
* Supported system versions: Windows 9x 4.0 (Windows 95), Windows 9x 4.1 (Windows 98), Windows 9x 4.9 (Windows ME)
* MinGW: i686-windows-gnu (aka. i686-w64-mingw32)

## DOS
* MS-DOS, FreeDOS (DJGPP): i586-msdosdjgpp

## Host C Library Environment
* newlib: unknown-elf; (x86\_64, i386, etc.)-(cygwin, msys2)
* avr: avr-elf

# C++ Version Required
- Full ISO C++26

# Compiler Support
- LLVM Clang >= 21 (recommended)
- GCC >= 16
- (MSVC hasn't support C++26 currently, not supported at this time)

# C++ Standard Library Support (Version is the same as the version released with the corresponding supported compiler)
- GNU libstdc++ >= 15
- LLVM libc++ >= 21
- MSVC STL >= 202501
