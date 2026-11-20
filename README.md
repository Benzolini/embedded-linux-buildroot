# Embedded Linux – Buildroot on Raspberry Pi B+

## Project Overview

This project is part of my Embedded Linux learning journey.

The goal is to build a minimal, custom Embedded Linux system using **Buildroot** and deploy it on a Raspberry Pi Model B+ V1.2.

The development environment runs on Windows 11 with WSL2 (Ubuntu). Buildroot performs the cross-compilation on the development PC. The Raspberry Pi is used as the target hardware.

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
| Linux kernel | Raspberry Pi kernel |
| Init system | BusyBox init |
| SSH server | Dropbear |
| Root filesystem | ext4 |

## Project Goals

- Understand the Embedded Linux build process
- Configure and build a custom Linux system with Buildroot
- Understand cross-compilation and toolchains
- Boot Linux on the Raspberry Pi
- Access the system via SSH
- Explore Linux GPIO, SPI, I2C and UART
- Develop and integrate custom applications
- Understand the Linux kernel and device tree
- Later compare Buildroot with Yocto Project

## Repository Structure

```text
raspberrypi-bplus/
├── configs/
│   └── raspberrypi_bplus_defconfig
├── board/
│   └── raspberrypi-bplus/
├── app/
├── patches/
├── .gitignore
└── README.md
```

## Build Environment

Buildroot is installed separately from this project repository.

```text
~/embedded-linux/
├── buildroot-2026.08.1/
└── projects/
    └── raspberrypi-bplus/
```

## Build

The project configuration is stored in:

```text
configs/raspberrypi_bplus_defconfig
```

The Buildroot source tree is located at:

```text
~/embedded-linux/buildroot-2026.08.1
```

## Status

The following milestones have been completed:

- [x] Buildroot 2026.08.1 configured for Raspberry Pi B+ (BCM2835 / ARMv6)
- [x] Buildroot image successfully built
- [x] Image flashed to microSD card
- [x] Raspberry Pi successfully booted with the custom Buildroot image
- [x] Ethernet networking configured via DHCP
- [x] SSH access configured and tested using Dropbear
- [x] GPIO17 tested using the Linux sysfs GPIO interface
- [x] Custom C application cross-compiled for ARM
- [x] LED blinking application tested successfully on the Raspberry Pi
- [x] Configurable blink interval and clean shutdown using CTRL+C implemented
- [x] Custom application integrated as a Buildroot package using BR2_EXTERNAL
- [x] Application automatically installed to `/usr/bin/gpio_test` in the target root filesystem
- [x] GPIO17 automatically exported by the application at startup
- [x] Updated Buildroot image successfully built, flashed and tested on the Raspberry Pi
- [x] LED blinking application verified to work immediately after boot without manual GPIO export

## Learning Journey

This repository documents the development process and configuration of the Embedded Linux system.

Buildroot is the first build system used in this project. A separate repository will be created later for the Yocto Project implementation.

## Building the System

### Requirements

- Ubuntu / WSL2
- Buildroot 2026.08.1
- Raspberry Pi B+ (BCM2835 / ARMv6)
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

## GPIO Test Application and Init Service

The project includes a custom C application integrated as a Buildroot package.

- Application: `/usr/bin/gpio_test`
- Init script: `/etc/init.d/S99gpio-test`
- Default blink interval: 500 ms
- GPIO: BCM GPIO17 (physical pin 11)

The application automatically exports GPIO17 and configures it as an output.

The BusyBox init system executes the `S99gpio-test` script during system startup. This starts the LED blinking automatically.

### Controlling the Service

Run these commands on the Raspberry Pi via SSH:

```bash
/etc/init.d/S99gpio-test start
/etc/init.d/S99gpio-test stop
/etc/init.d/S99gpio-test restart
```

The service runs the application in the background. Stopping it sends SIGINT, allowing the application to terminate cleanly and set GPIO17 to LOW.
