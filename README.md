# IDP-3510-04A Display Module

The **IDP-3510-04A** is a compact and versatile display module designed for intuitive control and monitoring applications.

![image](https://github.com/user-attachments/assets/4929b98f-e366-4604-90ca-72989cc787c6)


## Key Features

- **Display Panel**  
  - 3.5” TFT LCD  
  - Resolution: 320×240  

- **Graphics Controller**  
  - Integrated **FT813** from Bridgetek  
  - Embedded Video Engine (EVE) for advanced graphic rendering and touch control

- **Microcontroller Unit (MCU)**  
  - **ESP32-WROOM-32E**  
  - Onboard PCB antenna  
  - Supports **Wi-Fi**, **Bluetooth®**, and **Bluetooth LE**

- **Notification Features**  
  - Built-in **buzzer** for event alerts  


The datasheet can be downloaded from : https://brtchip.com/product/idp-3510-04a/

The built-in example displays Bridgetek logo, scans Wifi networks and shows the connected Wifi.

### Build and run

#### Install tool:
 - vscode and ESP-IDF extension
  
#### Configure the ESP-IDF extension:
 - Open the vscode, configure the ESP-IDF by command: "ESP-IDF: Configure ESP-IDF Extension"
 - The ESP-IDF setup tab will appears, add this setting:
	  - Select ESP-IDF version: -> v5.4.1 or any other version
      - Enter ESP-IDF container directory: -> [USER]\esp
      - Enter ESP-IDF Tools directory (IDF_TOOLS_PATH): ->  -> [USER]\esp\v5.4.1\esp-idf\tools

 - Click "Configure tool" button
 - Wait for the installer finish

#### Compile the source
 - Open ESP-IDF Terminal in VScode
``` 
      $ cd examples\wifi_scan
      $ idf.py build
```
#### Flash the binary file
 - Connect the Micro-USB cable to your PC, then open Device Manager and check under "Porrts (COM & LPT)" to identify the assigned COM port number (e.g., COM3).
```
      $ ipf.py -p COM3 flash
```
Example binary files are under examples/wifi_scan/example_binary. Users can use the following command to flash:
```
  $ esptool.py --chip esp32 -p COM3 -b 460800 --before=default_reset --after=hard_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 bootloader.bin 0x10000 wifi_scan.bin 0x8000 partition-table.bin 0x110000 storage.bin
```  