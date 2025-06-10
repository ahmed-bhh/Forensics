# SPIFFS: Study and Forensic Analysis

## Usage Context

**SPIFFS (SPI Flash File System)** is a lightweight filesystem designed for embedded systems using NOR flash memory. It is widely used in microcontroller-based platforms such as the **ESP8266** and **ESP32**, where system resources (RAM, CPU, and storage) are extremely limited. Its minimal footprint and ease of integration make it ideal for **IoT devices**, **sensors**, and other low-power, low-resource embedded devices. Unlike traditional filesystems like FAT, SPIFFS does not support directories or timestamps; instead, files are referenced directly by their path-like names.

## Main Characteristics

- Designed for **NOR flash memory**
- **No directory hierarchy** — flat namespace using full file paths
- Supports **wear-leveling** and **power-loss resilience**
- Minimal **RAM usage** during runtime
- Files can be **overwritten in-place**, but block erasure happens in bulk

SPIFFS uses a **log-structured design**, which appends changes instead of modifying data in-place. This helps reduce flash wear and supports recovery after sudden power loss.

## File Deletion Behavior

When a file is deleted in SPIFFS, the system marks the corresponding **object headers** and **index pages** as deleted. However, the actual data remains in the memory until the corresponding flash pages are reclaimed and reused. These blocks are not immediately erased due to the constraints of NOR flash memory (which requires whole-block erasure). As a result, deleted file data can often be found and analyzed in the raw filesystem image before it gets overwritten — providing an opportunity for **forensic recovery**.

# SPIFFS Forensic Reader

A simple forensic tool that reads a SPIFFS (SPI Flash File System) image and displays:

- **Directories** inferred from file paths  
- **Files** and their offsets  
- **Printable content** found in each file  
- **Generic strings** and log fragments  
- _Work in progress:_ detection and recovery of deleted file remnants  

## Quick Start

1. **Build the tool**  
   ```bash
   make
 **run the scanner**  
./spiffs_reader my_spiffs.img

2. **results**
 ![alt text](image.png)

