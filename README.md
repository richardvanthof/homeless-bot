# Homeless bot

Interactive installation of a robot portraying the struggle of homelessness by begging for cans, based on the MakeBlock mBot robot platform 

## Requirements

- mBot robot
- VS Code with Platform IO plugin
- CH340 drivers

## Setup guide

To edit the code of the robot in the arduino IDE you need the mBlock drivers ([1](https://support.makeblock.com/hc/en-us/articles/4419572961943-Program-mBot-in-Arduino-IDE))

1. Download and install the CH340 driver required by mCore (restart your computer after the installation is done).
   1. ***[CH340 for Windows](http://www.wch-ic.com/downloads/CH341SER_EXE.html)\***
   2. ***[CH340 for MacOS](http://www.wch-ic.com/downloads/CH341SER_MAC_ZIP.html)\***
2. Open Arduino IDE, refer to ***[the documentation](https://github.com/Makeblock-official/Makeblock-Libraries)*** to download and configure the makeblock-library files.
3. Open `main.ino` in VS Code.
4. Turn on the mBot and connect it to your computer
5. Select the board over the `/dev/tty.wchusbserial***`port. Set it as an `Arduino Uno` board. 
6. You should now be able to compile and upload the code to the mBot