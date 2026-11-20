# Embedded Linux – Buildroot on Raspberry Pi B+

## Project Overview

This project is part of my Embedded Linux learning journey.

The goal is to build a minimal, custom Embedded Linux system using **Buildroot** and deploy it on a Raspberry Pi Model B+ V1.2.

The development environment runs on Windows 11 with WSL2 (Ubuntu). Buildroot performs the cross-compilation on the development PC, while the Raspberry Pi is used as the target hardware.

The project focuses on understanding the Linux build process, cross-compilation, system configuration, hardware access and application integration.

## Hardware

| Component | Specification |
|---|---|
| Board | Raspberry Pi Model B+ V1.2 |
| SoC | Broadcom BCM2835 |
| CPU | ARM1176JZF-S |
| Architecture | ARMv6 |
| CPU frequency | 700 MHz |
| RAM | 512 MB |
| Storage | microSD |
| Ethernet | 10/100 Mbps |
| USB | 4x USB 2.0 |
| GPIO | 40-pin header |

## Software

| Component | Version / Configuration |
|---|---|
| Build system | Buildroot |
| Buildroot version | 2026.08.1 |
| Host system | Ubuntu on WSL2 |
| Target architecture | ARM |
| Linux kernel | 6.12.61 |
| Init system | BusyBox init |
| SSH server | Dropbear |
| Root filesystem | ext4 |
| External tree | BR2_EXTERNAL |

## Project Goals

- Understand the Embedded Linux build process
- Configure and build a custom Linux system with Buildroot
- Understand cross-compilation and toolchains
- Boot Linux on the Raspberry Pi
- Access the system via SSH
- Explore Linux GPIO, SPI, I2C and UART
- Develop and integrate custom applications
- Understand the Linux kernel and device tree
- Later compare Buildroot with the Yocto Project

## Repository Structure

```text
raspberrypi-bplus/
├── Config.in
├── external.desc
├── external.mk
├── configs/
│   └── raspberrypi_bplus_defconfig
├── board/
│   └── raspberrypi-bplus/
├── package/
│   └── gpio-test/
│       ├── Config.in
│       ├── gpio-test.mk
│       ├── S99gpio-test
│       └── src/
│           └── gpio_test.c
├── patches/
├── .gitignore
└── README.md
```

The repository is configured as a Buildroot external tree named `RASPBERRYPI_BPLUS`.

Custom applications are integrated as Buildroot packages under `package/`.

## Build Environment

Buildroot is installed separately from this project repository.

```text
~/embedded-linux/
├── buildroot-2026.08.1/
└── projects/
    └── raspberrypi-bplus/
```

The external tree contains the board configuration, custom packages and project-specific Buildroot configuration.

## Current Status

The following milestones have been completed:

### Buildroot System

- [x] Buildroot 2026.08.1 configured for Raspberry Pi B+ (BCM2835 / ARMv6)
- [x] Custom Buildroot external tree created using BR2_EXTERNAL
- [x] Project-specific Buildroot defconfig created
- [x] Buildroot image successfully built
- [x] Image flashed to microSD card
- [x] Raspberry Pi successfully booted with the custom Buildroot image
- [x] Ethernet networking configured via DHCP
- [x] SSH access configured and tested using Dropbear

### GPIO Application

- [x] GPIO17 tested using the Linux sysfs GPIO interface
- [x] Custom C application cross-compiled for ARM
- [x] LED blinking application tested successfully on the Raspberry Pi
- [x] Configurable blink interval implemented
- [x] Clean shutdown using CTRL+C implemented
- [x] Custom application integrated as a Buildroot package
- [x] Application automatically installed to `/usr/bin/gpio_test`
- [x] GPIO17 automatically exported by the application at startup
- [x] Updated Buildroot image successfully built, flashed and tested
- [x] LED blinking application verified to work immediately after boot

### Startup Service

- [x] BusyBox init service created to automatically start the GPIO application during boot
- [x] Application starts automatically in the background
- [x] GPIO service can be stopped, started and restarted using its init script
- [x] Clean shutdown through SIGINT implemented

### Next Development Steps

- [x] Replace the deprecated sysfs GPIO interface with the Linux GPIO character device API
- [x] Use **libgpiod 1.6.5** for GPIO access from the C application
- [x] Build and test the updated application on the Raspberry Pi
- [x] GPIO-Startup-Service mit der neuen Anwendung getestet

**Note:** libgpiod 1.6.5 has been selected as the target API for the next development step. The application migration has not yet been completed.

## GPIO Test Application

The project includes a custom C application integrated as a Buildroot package.

| Property | Configuration |
|---|---|
| Application | `gpio_test` |
| Target path | `/usr/bin/gpio_test` |
| GPIO | BCM GPIO17 |
| Physical pin | 11 |
| Default blink interval | 500 ms |
| Current GPIO interface | Linux sysfs GPIO |

The LED is connected to BCM GPIO17 through a current-limiting resistor, with the cathode connected to GND.

The current implementation uses the legacy sysfs GPIO interface. GPIO17 is automatically exported and configured as an output when the application starts.

## GPIO Startup Service

A BusyBox init service was created to start the GPIO application automatically during system boot.

The init script is installed at:

```text
/etc/init.d/S99gpio-test
```

BusyBox init executes the script during system startup. The service starts the application in the background with a default blink interval of 500 ms.

### Controlling the Service

Run these commands on the Raspberry Pi via SSH:

```sh
/etc/init.d/S99gpio-test start
/etc/init.d/S99gpio-test stop
/etc/init.d/S99gpio-test restart
```

Stopping the service sends SIGINT to the application, allowing it to terminate cleanly and set GPIO17 to LOW.

## Next Step: Modern GPIO Interface with libgpiod

The current application uses the legacy Linux sysfs GPIO interface.

The next development step is to migrate the application to the Linux GPIO character device API using **libgpiod 1.6.5**.

The character device interface provides access to GPIO controllers through device files such as:

```text
/dev/gpiochip0
```

Instead of exporting GPIOs through sysfs, the application will request GPIO lines through libgpiod and control their values using the library API.

This migration will replace the existing sysfs implementation.

**Status:** libgpiod 1.6.5 is selected and available in the Buildroot configuration. Application migration and hardware testing are still pending.

## Building the System

### Requirements

- Windows 11 with WSL2
- Ubuntu on WSL2
- Buildroot 2026.08.1
- Raspberry Pi Model B+ V1.2
- Raspberry Pi Imager
- microSD card

### Initial Build Configuration

Navigate to the Buildroot source directory:

```bash
cd ~/embedded-linux/buildroot-2026.08.1
```

Load the project-specific Buildroot configuration:

```bash
make BR2_EXTERNAL=$HOME/embedded-linux/projects/raspberrypi-bplus raspberrypi_bplus_defconfig
```

### Build the System

Build the complete system:

```bash
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin \
make BR2_EXTERNAL=$HOME/embedded-linux/projects/raspberrypi-bplus
```

The clean PATH is required in this WSL2 environment because inherited Windows PATH entries may contain spaces.

The generated SD card image is located at:

```text
output/images/sdcard.img
```

### Rebuilding the GPIO Test Package

After modifying the GPIO application or its package files, clean the package first:

```bash
make BR2_EXTERNAL=$HOME/embedded-linux/projects/raspberrypi-bplus gpio-test-dirclean
```

Then rebuild the system:

```bash
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin \
make BR2_EXTERNAL=$HOME/embedded-linux/projects/raspberrypi-bplus
```

### Flashing the Image

1. Copy `output/images/sdcard.img` to a Windows-accessible directory.
2. Open Raspberry Pi Imager.
3. Select the generated image.
4. Select the Raspberry Pi's microSD card.
5. Flash the image.

**Warning:** Flashing overwrites the selected SD card.

## Learning Journey

This repository documents the development process and configuration of the Embedded Linux system.

Buildroot is the first build system used in this project. A separate repository will be created later for the Yocto Project implementation.
