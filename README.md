# CADDeck
![System Photo](images/systemphoto.png)

This is a combined joystick and touch panel display suitable for use with common CAD programs. It includes:
1. Two axis joystick (e.g. for X, Y pan)
2. Thumbwheel encoder in the joystick grip (e.g. for zoom) with push switch (e.g. for select)
3. Four switches around joystick grip (e.g. hold one down while moving joystick for pan and another for rotate)
4. Four switches around the base (e.g. measure, ESC...)
5. LCD touch panel with up to 10 screens of 12 buttons

All H/W buttons as well as the touch panel buttons are programmable through a web page.

It connects to the PC via Bluetooth using the HID interface (i.e. it looks like a keyboard and mouse to the PC). A USB-C port is provided for power and to download code.
Location of the USB-C port on the left side is not ideal, but that is where it is on the screen being used.
It should be possble to run power to the unit and hardwire it to the screen rather than using the USB-C for power but I have not tried this.

## Touch Panel
The touch panel is adapted from Dustin Watt's FreeTouchDeck project (https://github.com/DustinWatts/FreeTouchDeck.git) which I modified to increase the button count,
etc.  (https://github.com/andrewfernie/FreeTouchDeckWT32.git). For this project I added support for the joystick, encoder, and buttons.
The easiest way to get it going is to use a WT32-SC01 board (http://www.wireless-tag.com/portfolio/wt32-sc01/).
Purchase the basic board, not the "Plus" version as the Plus uses a parallel interface to the LCD panel and doesn't leave enough IO lines free for the joystick and buttons,
etc. The board includes an ESP32 module, and a 3.5" LCD with capacitive touchscreen.


## S/W Build Notes:
1. The build uses the PlatformIO IDE for VSCode. Install VSCode, then the PlatformIO plugin and you should be good to go.  
2. You need to make sure that the appropriate pins are defined for your hardware configuration. I built mine for the WT32-SC01 and have built the code for a handwired ESP32 module + resistive touchscreen. 
3. You may see build messages like
        
        In file included from include/CADDeck.h:14,
                 from include/SaveConfig.h:3,
                 from src/SaveConfig.cpp:1:
        .pio/libdeps/esp-wrover-kit/TFT_eSPI/TFT_eSPI.h:909:8: warning: #warning >>>>------>> TOUCH_CS pin not defined, TFT_eSPI touch functions will not be available! [-Wcpp]
        #warning >>>>------>> TOUCH_CS pin not defined, TFT_eSPI touch functions will not be available!`
    
    You can get rid of these by defining TOUCH_CS in platformio.ini to any unused pin that is a valid output pin.
	The alternative is to not define TOUCH_CS (delete the line, or put a semi-colon in front of it) and just ignore the messages.

        #define TOUCH_CS=33

4. The system tries to display the logo in "CADDeck_logo.bmp" at startup. If you don't want anything displayed make sure that the file doesn't exist. In this case you will see a message like the following on the serial port.
   As long as you didn't weant anything displayed at startup ignore the message.
        
        [  1159][E][vfs_api.cpp:104] open(): /littlefs/logos/CADDeck_logo.bmp does not exist, no permits for creation

5. You may  see some error messages like 
   
        [vfs_api.cpp:104] open(): /littlefs/list does not exist 
        
   This is a known issue with ESPAsyncWebsServer related to LittleFS supporting folders while SPIFFS does not. See https://github.com/lorol/

6. Having trouble? There are some debug messages that can be enabled in FreeTouchDeck.h
   
        #define LOG_MSG_BASIC 1
        #define LOG_MSG_LEVEL 1  // 1=ERROR, 2=ERROR+WARN, 3=ERROR+WARN+INFO
        #define LOG_MSG_DEBUG 0
        #define LOG_MSG_TOUCH_DEBUG 0  // messages to console each time a touch is detected

   Try setting LOG_MSG_LEVEL to 3 and you will get quite a few more status messages


# Hardware Build

![Assembly](images/Assembly.png)

Most assembly is basic and is primarily wiring.  Complete 3D CAD models are provided in both Fusion360 and STEP formats in the hardware/model folder.

References to button numbers are as shown in this diagram:

![Button Numbers](images/ButtonLayout.png)


## Case
A Fusion360 model of the case, as well as STL files, are included in the CADDeck repository. The main case is printed in four pieces.
The two top sections are bolted together internal to the case with M3 cap screws and nuts.
M3 threaded inserts are used in the top front piece (six places), then the bottom is attached to the top with M3 flat head screws. 


![Case](images/Case.png)


There is no obvious way to attach the screen to the case,
so I soldered four 6mm long M2.5 brass standoffs to the PCB.

![Screen Attachment](images/ScreenAttachment.png)
M2.5 flathead screews come through the bottom part of the case and are used to both hold the case closed and to attach the screen.

## Joystick knob
The joystick knob consists of two more 3D printed parts.  Two M3 screws are used to hold the two parts together,
with threaded inserts in the bottom part. The perfboard to which the encoder and debounce/pullup components are mounted is clamped by the top and bottom parts. 

Four tactile buttons are located around the knob. I used a little hot glue to hold them in place against the bottom part of the knob. 

The first version of the knob only had two buttons, equivalent to Buttons 5 and 6. The idea was to hold one down while moving the joystick to pan the view and the other to rotate the view.
The problem was that I found it awkward to use Button 6 while holding the joystick in my left hand, and the same problem with Button 5 while holding the joystick in my right hand. So, I added Buttons 7 and 8.
I find that Buttons 5 and 8 work well with the left hand, and Buttons 6 and 7 work well with the right hand.

## Wiring
A [wiring diagram](hardware/Electrical/Wiring%20Diagram.pdf) is included in the hardware/electrical folder. Most of it is straighjtforward.

The touchy part is the joystick grip, and in particular the resistors and capacitors to debounce the encoder and pull the switches high. I used a small piece of perfboard to which I soldered 0805 size SMD components.
I also used 30AWG silicon stranded wire to connect the perfboard into the base. Too many wires, and not enough space.

![Perfboard for knob](images/pcb.png)

![Knob Interior](images/Knobinterior.png)

## Parts List
Qty 1 [ESP32-SC01 LCD touchscreen](https://www.aliexpress.com/item/1005004399769442.html)

Qty 1 [2mm 2x20 Male Header](https://www.aliexpress.com/item/1005001852671581.html)

Qty 1 [EVQWGD001 Encoder](https://www.aliexpress.com/item/1005002824178689.html)


Qty 4 [12mm x 12mm x 8.5mm High Tactile Pushbuttons](https://www.aliexpress.com/item/1005004735827784.html) 8mm, 9mm, or 10mm high wouild also work fine 

Qty 4 [8mm momentary pushbuttons](https://www.aliexpress.com/item/1005002898978166.html) (these are the ones I used, but are not great -
      I would prefer something with a better tactile feel)

Qty 1 [FrSky M9 joystick](https://www.aliexpress.com/item/32829691785.html)   This one is a bit pricey, but it is a good quality unit. Nothing special in terms of functionality -
       you just need something with analog X and Y outputs.


Qty 1 [PCF8575 I2C IO Expander](https://www.aliexpress.com/item/1005004433286881.html)

Qty5 10k 0805 SMD resistors for the encoder debounce and joystick button pullups

Qty5 10k 1/8W throughhole resistors for 8mm button pullups

Qty2 100nF 0805 SMD capacitors for the encoder debounce

Qty 8 M3 Brass inserts

Misc M3 hardware (nuts, screws, etc)

30AWG silicon stranded wire
r

# Setup
Once the unit is assembled and the display is coming up you should see the main page.
![Main Page](images/mainpage.png)

Go to the CAD Settings page.
![CAD Settings Page](images/cadsettings.png)

Then the Switch Monitor page.

![Switch Monitor Page](images/iodebug.png)

It displays the status of the joystick, encoder, and buttons. You should be able to see the result of each joystick movement or button selection.
Note that the values are shown as 1 when not selected and 0 when selected (pushed).

The joystick X and Y data may be bouncing around a bit. This is normal as the joystick is not calibrated at this point. To calibrate the joystick:
1. Go back to the CAD Settings page
2. Press the Joystick Zero button when the joystick is centered and still. This sets the zero point for the joystick. 
3. Press the Joystick Scale button and move the joystick around to the extremes in X and Y. You have 5s.
   Once the function finishes the calibration parameters will be calculated and the joystick should show a range of roughly +/- 1 in both X and Y. 
4. If you are happy with the calibration, press the Save CAD Configuration button and the parameters will be saved to the cadparams.json file in the ESP32.
   This file can be downloaded from the ESP32 to your PC via the CAD Settings page in the configurator.
   It is a good idea to do this and to copy the file to the data\config folder in the CADDeck code so that you won't lose the parameters next time you upload the data folder to the ESP32.
4. If you are not happy with the results, try again.

Bind your computer to the CADDeck using the Bluetooth setings page on your computer.

# Configuration
Note that all of the pages shown or mentioned above are samples. Every page can be reorganized and customized to suit your needs via the configurator.

## WiFi Configuration
The configurator is accessed via a web browser on a computer or tablet connected to the same network as the ESP32.
To enable the configurator go to the settings page on the touch panel and select the WiFi Enable button.  

![System Settings](images/systemsettings.png)

If you have already configured your wifi connection in wificonfig.json you should see a message saying that it has connected to the wifi and will display the IP address.
At that point you can go to your browser and enter the IP address top connect. The configurator will be displayed.

![Configurator Main Page](images/mainconfiguratorpage.png)

If you have not already configured the wifi you should see a message saying that an access point (AP) has been created with a network name and IP address.
You will need to connect your computer to the network provided, and then point your browser at the IP address.
Once the configurator screen is displayed in your browser go to the wifi page and enter your network SSID and password. Then select "Save WiFi Config" to save the settings. 

## Menu Configuration
The main change from the original FreeTouchDeck is to allow more buttons per page  - the screen area is just a bit smaller than the size of twelve StreamDeck buttons which seems reasonable.
The code is currently set to three rows of four buttons, but can be changed to three rows of five buttons (or two rows of four, etc.) by modifying CADDeck.h: "#define BUTTON_ROWS 3" and "#define BUTTON_COLS 5".
In principle it should support larger arrays, but not tested, and the configurator page is set to a maximum of 3x5. 

The configurator displays an array of three rows of five buttons, with a drop down list to select the menu page to be modified.
If you are using fewer than 3x5, simply ignore the extra rows and columns. On the to-do list is to generate the HTML dynamically to correspond with the number of rows and columns.
![Menu Configurator](images/MenuConfigurator.png)

Each Menu page has a 2D array of programmable buttons, and each button has an array of 3 actions that can be defined for when the button is pressed.

You can set different background colors for a button that performs an action (black background in the image above), or links to another menu (light blue in the image.)
This is controlled on the Settings page of the configurator.

If you don't want to use all buttons use the configurator to set the logo to "blank.bmp" and make sure that there are no actions defined for that button.
Any button with those characteristics will not be drawn.

One limitation is the size of the FLASH used for the configuration files and icons displayed on the touch screen. The icons are 75x75 24-bit color BMP files,
each one taking about 17kB. If you run out of space your only option is to check to see if any are unused and delete them.


## Hardware Buttons
Configuration of the hardware buttons is via a dedicated page within the same web-based configurator as for the LCD panel. 
![CAD Settings Configurator](images/CADSettingsConfigurator.png)

The top section includes:
1. Current CAD program: This is the program whose settings will be loaded when the CADDeck is restarted
2. X and Y scaling parameters for the joystick. These can be set manually, but a function is provided to capture data as the joystick is moved to its extremes and calculate the scalings for you. The output scaling is +/-1
3. Joystick deadzone. Any X or Y value with an absolute value below this number is set to zero.
4. Joystick sensitivity. Joystick values are multiplied by this as they are converted to mouse movement commands
5. Thumbwheel sensitivity. The encoder value is multiplied by this as it is converted to mouse movement commands
6. Steady Time: After the joystick has been centered for more than this amount of time, without a pan or rotate button selection, the joystick will return to its default mode (typically mouse pointer)

Below that is the button action definition. The same approach to defining the actions is used as for the LCD panel buttons. The actions are:
1. Selected CAD program. The program whose settings are being modifed in that part of the configurator. Separate definitions of these settings are kept for each of the five CAD programs available.
2. Selected Button. One of the nine buttons shown in the image to the left. Click  one of the indicated areas in the image then modify the actions for that button as needed.
3. The actions that must be taken to enable pan/rotate/zoom to be controlled by the joystick (i.e. the mouse) within the selected CAD program.
   Note that the definition of what is needed to control pan/rotate/zoom is separate from which button enables that mode.
   As an example, Solidworks needs the middle mouse button to be held down to rotate the view. So, JoystickRotate/Action1 is set to "Mouse Buttons" and Value1 is set to "Press Middle Button".
   To assign the Rotate function to button 8, click "8" in the image to the left, then set SelectedButton/Action1 to "CAD Functions" and Value1 to "Joystick Rotate"

![Configuring the Rotate function](images/ConfigureRotate.png)

At the very bottom is the "Save CAD Config" button. When selected the settings for all CAD programs are saved on the ESP32 in file cadparams.json

## Default Configuration
While everything can be changed via the configurator, there are some default settings provided with the code.
These are:

### Control assignments
1. Joystick moves the mouse pointer
2. Joystick with Button 5 held down pans the view
3. Joystick with Button 6 held down rotates the view
4. The encoder zooms in and out
5. The encoder Button 9 is the same as the left mouse button (select)
6. Button 1 is measure ("i" in Fusion360) (I assigned "i" as a keyboard shortcut to measure in Solidworks)
7. Button 2 sets the zoom to fit ("F6" in Fusion360), ("f" in Solidworks)
8. Button 3 unselects ("ESC" in Fusion360 and Solidworks)
9. Button 4 toggles object visibility ("v" in Fusion360) or hides object ("TAB" in Solidworks)

### Menus
Menu numbers are assigned as follows:
1. Menu 0: Main menu
2. Menu 1: System Settings
3. Menu 2: CAD Settings
4. Menu 3: Not used
5. Menu 4: Not used
6. Menu 5: Not used
7. Menu 6: Not used
8. Menu 7: Select active CAD program
9. Menu 8: Fusion360 actions
10. Menu 9: Not used
 

