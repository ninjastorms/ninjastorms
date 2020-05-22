
               _        _           _
         _ __ (_)_ __  (_) __ _ ___| |_ ___  _ __ _ __ ___  ___
        | '_ \| | '_ \ | |/ _` / __| __/ _ \| '__| '_ ` _ \/ __|
        | | | | | | | || | (_| \__ \ || (_) | |  | | | | | \__ \
        |_| |_|_|_| |_|/ |\__,_|___/\__\___/|_|  |_| |_| |_|___/
                     |__/


     Created by the Operating Systems and Middleware Group at Hasso
                 Plattner Institute Potsdam, Germany.

# Introduction

NinjaStorms is an attempt to create a simple RealTime Operating System which
is currently running on Lego Mindstorms EV3 and a virtual environment. This fork aims to add the required functionality for a TCP/IP stack.

## Building from Source

ninjastorms uses standard autotools to manage the build process. to configure
the sources, you need to execute the configure script. If you checked out the
sources from the repository, or there is no configure script present for any
other reason, you can generate it by executing

    ./autogen.sh

To configure the project, execute

    ./configure --host=<host> BOARD=<board> CFLAGS="-nostdlib -nostartfiles -ffreestanding"

where `host` is the appropriate host triplet for the target architecture,
for example `arm-none-eabi` for the EV3 or the qemu environments and `board`
is a supported board. See [Supported Boards](#supported-boards) for the board specific configure command.

Building the project requires a `host` prefixed toolchain capable of generating
executables for your target architecture. To generate a `host` prefixed
toolchain, in case your distribution does not provide one, see the
[generate-toolchain](https://github.com/ninjastorms/toolchain-generator)
project. Assuming a working toolchain for your architecture and a configured
source tree, the project is built by executing

    make

When the build completes, the kernel binary can be found in the source tree
as a file named `ninjastorms`. See [Supported Boards](#supported-boards) for the deployment
process of the built kernel.

## Supported Boards

ninjastorms is currently supported on the following target boards. If your
favourite platform is missing, feel free to port the system and send us a
pull request!

### `ev3` - Lego Mindstorms EV3

- target platform: arm-none-eabi
- deployment process: TODO

### `versatilepb` - embedded arm virtualization using qemu

- target platform: arm-none-eabi
- requirements:
  - Arch: `qemu qemu-arch-extra arm-none-eabi-newlib arm-none-eabi-gcc arm-none-eabi-binutils`
  - Ubuntu: `gcc-arm-none-eabi binutils-arm-none-eabi gdb-multiarch openocd autoconf libtool u-boot-tools qemu qemu-system-arm`
  - Debian: `gcc-arm-none-eabi binutils-arm-none-eabi gdb-arm-none-eabi openocd autoconf libtool u-boot-tools qemu qemu-system-arm`
  - feel free to add the requirements for other operating systems here
- configuration: configure ninjastorms for qemu virtual deployment with

        ./configure --host=arm-none-eabi BOARD=versatilepb CFLAGS="-nostdlib -nostartfiles -ffreestanding -mcpu=ARM926EJ-s"

- deployment process: execute the built kernel directly with

        qemu-system-arm -M versatilepb -m 128M -nographic -kernel ninjastorms

  The hardware access to the device periphery will fail silently, but the
  kernel output will still be shown.

## Further Reading

The kernel entry point is the `kernel_main` function in `kernel/main.c` - this
should be a good place to start reading into the source code.

We also collected information on the different supported boards in the
[hardware-docs](https://github.com/ninjastorms/hardware-docs) project.

Additional documentation can be found here:
[ninjastorms github wiki](https://github.com/ninjastorms/ninjastorms/wiki).
