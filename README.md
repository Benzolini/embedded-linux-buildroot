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

## Current Status

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

## Learning Journey

This repository documents the development process and configuration of the Embedded Linux system.

Buildroot is the first build system used in this project. A separate repository will be created later for the Yocto Project implementation.
