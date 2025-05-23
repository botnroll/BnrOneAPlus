# Bot'n Roll One A+ Arduino Library

Welcome to the Bot'n Roll One A+ Arduino library! 
This library enables seamless control of the Bot'n Roll One A+, allowing you to quickly program and interact with your Bot'n Roll One A+.<br>
For the most up-to-date C++ reference documentation please check [Online Documentation](https://botnroll.github.io/BnrOneAPlus/class_bnr_one_a_plus.html).

## About
The Bot'n Roll One A+ is a versatile and customizable robot designed for students, hobbyists, and educators. With its robust hardware and software capabilities, it is an excellent tool for learning and experimenting with robotics and Arduino programming.

![Bot'n Roll One A+](https://botnroll.com/one-a-plus/imgs/bnr_robot.jpg)

## Features
- Easy integration with Arduino® IDE, Raspberry Pi® 5, Open Roberta® Lab, Matlab Simulink® and others.
- Provides functions to control motors, sensors, and other peripherals.
- Full documentation and support provided.

## Installation
1. Open the Arduino IDE 2.0.
2. Go to **Tools > Manage Libraries...**.
3. Search for `BnrOneAPlus` in the Library Manager.
4. Click **Install** to add the library to your Arduino environment.
5. The library is now ready to use! Check out the examples in **File > Examples > BnrOne APlus**.

## Documentation and Resources
To get started with your Bot'n Roll One A+, refer to the following manuals:

- [Assembly Manual](https://botnroll.com/onea/downloads/Manual-Montagem-Bot'n-Roll-ONE-A.pdf)
- [Software Manual](https://botnroll.com/onea/downloads/Manual-Software-Bot'n-Roll-ONE-A.pdf) or [Online Documentation](https://botnroll.github.io/BnrOneAPlus/class_bnr_one_a_plus.html)
- [Extras Manual](https://botnroll.com/onea/downloads/Manual-Extras-Bot'n-Roll-ONE-A.pdf)

For further assistance, visit the [Support Page](https://www.botnroll.com/onea/).

## Examples
Here is a quick example to get you started:

```cpp
#include <BnrOneAPlus.h>   // Bot'n Roll ONE A+ library
#include <SPI.h>       // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;           // declaration of object variable

#define SSPIN  2       // Slave Select (SS) pin for SPI communication

void setup() 
{
    Serial.begin(115200);     // set baud rate to 115200bps for printing values
    one.spiConnect(SSPIN);   // start SPI communication module
    one.stop();              // stop motors
}

void loop()
{
    one.lcd2("    Forward "); // print data on LCD line 2
    one.move(50,50);          // Forward
    delay(1000);              // wait 1 second
    one.lcd2("     Stop   ");
    one.stop();               // Stop Motors
    delay(500);
    one.lcd2("   Backwards ");
    one.move(-50,-50);        // Backwards
    delay(1000);
    one.lcd2("     Stop   ");
    one.move(0,0);            // Stop Motors
    delay(500);
}
```

## Issues
If there is any issue, feel free to open a new issue in order for us to fix it and improve the library.

For more information about the Bot'n Roll One A+, visit [Bot'n Roll](https://www.botnroll.com).


== License ==

Copyright (c) botnroll.com. All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.
