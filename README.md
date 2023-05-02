# CADDeck
![System Photo](images/systemphoto.jpg)

This is a combined joystick and touch panel display suitable for use with common CAD programs. It includes:
1. Two axis joystick (e.g. for X, Y pan)
2. Hallsensor in the joystick Knob (e.g. for zoom) with push Sensor (e.g. move to the component in connection with the joystick)
3. There is a second Hall sensor in the knob. With a slight turn to the right or left, you can rotate the component.
4. Ten switches around the base (e.g. measure, ESC...)
5. LCD touch panel with up to 10 screens of 12 buttons

All H/W buttons as well as the touch panel buttons are programmable through a web page.

It connects to the PC via Bluetooth using the HID interface (i.e. it looks like a keyboard and mouse to the PC). A USB-C port is provided for power and to download code. Location of the USB-C port on the left side is not ideal, but that is where it is on the screen being used. It should be possble to run power to the unit and hardwire it to the screen rather than using the USB-C for power but I have not tried this.

## Touch Panel
The touch panel is adapted from Dustin Watt's FreeTouchDeck project (https://github.com/DustinWatts/FreeTouchDeck.git) which I modified to increase the button count, etc.  (https://github.com/andrewfernie/FreeTouchDeckWT32.git). For this project I added support for the joystick, encoder, and buttons.  The easiest way to get it going is to use a WT32-SC01 board (http://www.wireless-tag.com/portfolio/wt32-sc01/). Purchase the basic board, not the "Plus" version as the Plus uses a parallel interface to the LCD panel and doesn't leave enough IO lines free for the joystick and buttons, etc. The board includes an ESP32 module, and a 3.5" LCD with capacitive touchscreen.


## S/W Build Notes:
1. The build uses the PlatformIO IDE for VSCode. Install VSCode, then the PlatformIO plugin and you should be good to go.  
2. You need to make sure that the appropriate pins are defined for your hardware configuration. I built mine for the WT32-SC01 and have built the code for a handwired ESP32 module + resistive touchscreen. 
3. You may see build messages like
        
        In file included from include/CADDeck.h:14,
                 from include/SaveConfig.h:3,
                 from src/SaveConfig.cpp:1:
        .pio/libdeps/esp-wrover-kit/TFT_eSPI/TFT_eSPI.h:909:8: warning: #warning >>>>------>> TOUCH_CS pin not defined, TFT_eSPI touch functions will not be available! [-Wcpp]
        #warning >>>>------>> TOUCH_CS pin not defined, TFT_eSPI touch functions will not be available!`
    
    You can get rid of these by defining TOUCH_CS in platformio.ini to any unused pin that is a valid output pin. The alternative is to not define TOUCH_CS (delete the line, or put a semi-colon in front of it) and just ignore the messages.

        #define TOUCH_CS=33

4. The system tries to display the logo in "CADDeck_logo.bmp" at startup. If you don't want anything displayed make sure that the file doesn't exist. In this case you will see a message like the following on the serial port. As long as you didn't weant anything displayed at startup ignore the message.
        
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
A Fusion360 model of the case and STL files are included in the CADDeck repository. The main body is printed in two parts.
The upper part is screwed inside the housing with M3 head screws. M3 thread inserts are used in the upper part (six positions),
Then the bottom is attached to the top with M3 pan head screws. 

![Case](images/Case.png)

![Case](images/FrontBottomView.png)

![Case](images/BottomView.png)

There is no obvious way to attach the screen to the case, so I attached it with fixed standoffs at the back and two insertable standoffs at the front.
If necessary, it can also be pressed against the seal with two screws. But wasn't necessary for me.
![Screen Attachment](images/ScreenAttachment.png)
![Screen Attachment](images/displayinstallation.png)

4 4 x M3 pan head screws come through the bottom of the case and are used to attach both the joystick and the switch plate with the 10 switches..

## Joystick knob
The joystick button consists of several 3D printed parts. Three guide pins hold the two parts together and the base with the magnets is attached with three self-tapping screws. The touch sensor is secured in the lid with glue, the lid is pressed into the top of the wheel and should hold without glue.

![Knob](images/guidesleeve.png)

Inside the button are several magnets that repel each other, keeping it in a floating position. I used a little super glue to attach the magnets.
In the pictures directory you will find a representation of how the magnets (N+S) are arranged.

![Knob](images/buttonbelow.png)

The pressure sensor in the lid is held down while you move the joystick to pan the view back and forth. The button can be dragged up and down to zoom. It can also be rotated slightly left or right to rotate the view.

An M4x16mm spacer made of brass is pressed into the button from above (fix with some glue if necessary).
This is used to attach the button to the joystick and counter it from above with an M4 stud screw so that it sits firmly on the joystick axis..

## Wiring
A [wiring diagram](hardware/Electrical/Wiring%20Diagram.pdf) is included in the hardware/electrical folder. Most of it is straighjtforward.

The tricky part is the joystick knob and in particular the magnets and hall sensors. The upper Hall sensor is glued in directly and I soldered the lower one to a small perforated board beforehand and then attached it with superglue.

![Knob Interior](images/Knobinterior.png)


The cables from the touch sensor and the upper Hall sensor are routed down through holes. Please leave the cables a little longer, as the button will later be turned onto the joystick and countered with a grub screw from above.

I made plugs on the cables, both for the button, the joystick, the 10 buttons and the connections between the display and the PCF 8575.

![Perfboard for knob](images/pcb.png)

## Parts List
Qty 1 [ESP32-SC01 LCD touchscreen](https://www.aliexpress.com/item/1005004399769442.html)

Qty 1 [2mm 2x20 Male Header](https://www.aliexpress.com/item/1005001852671581.html)

Qty 1 [TTP223 Kapazitive Touch-Switch](https://de.aliexpress.com/item/33013050574.html?spm=a2g0o.productlist.main.5.34c063fbTcgCnU&algo_pvid=ee3d933d-e328-486a-b54d-4e8c9976bf73&algo_exp_id=ee3d933d-e328-486a-b54d-4e8c9976bf73-2&pdp_npi=3%40dis%21EUR%210.25%210.21%21%21%21%21%21%402145279016829361809717128d0790%2167182316577%21sea%21DE%21876101376&curPageLogUid=ozgoC29NqnOW)

Qty 10 [Cherry MX Taster](https://www.ebay.de/itm/183967039197) (These are available in different versions. With or without a click, heavy or light resistance.)

Qty 1 [FrSky M9 joystick](https://www.aliexpress.com/item/32829691785.html)   This one is a bit pricey, but it is a good quality unit. Nothing special in terms of functionality - you just need something with analog X and Y outputs.


Qty 1 [PCF8575 I2C IO Expander](https://www.aliexpress.com/item/1005004433286881.html)

Qty 1 [USB-C Adapter Board](https://de.aliexpress.com/item/1005003446036071.html?spm=a2g0o.order_list.order_list_main.55.72175c5fBl0s9h&gatewayAdapt=glo2deu)

Qty 2 [Hall sensors 49E](https://de.aliexpress.com/item/32910342894.html?spm=a2g0o.order_list.order_list_main.116.72175c5fBl0s9h&gatewayAdapt=glo2deu)

Qty 40 [balancing adhesive weights](https://www.ebay.de/itm/363221786745) (If necessary. The case was too light for me when I pull the button up. But you can also complain about something else.)

Qty 10 10k 1/8W throughhole resistors Cherry MX Taster

Qty 6 M3 Brass inserts

Qty 3 M2 Brass inserts

Qty 3 Self-tapping screw 2.2x5 (Magnetic Holder below)

Qty 11 Neodymium magnet N52 5x5x1 mm

Qty 6 Neodymium magnet N52 8x1 mm

Qty 3 Neodymium magnet N52 8x3 mm

Misc M3 and M2 hardware (nuts, screws, etc)

30AWG silicon stranded wire R

# Setup
Once the unit is assembled and the display is coming up you should see the main page.
![Main Page](images/mainpage.png)

Go to the CAD Settings page.
![CAD Settings Page](images/cadsettings.png)

Then the Switch Monitor page.

![Switch Monitor Page](images/iodebug.png)

It displays the status of the joystick, encoder, and buttons. You should be able to see the result of each joystick movement or button selection. Note that the values are shown as 1 when not selected and 0 when selected (pushed).

The joystick X and Y data may be bouncing around a bit. This is normal as the joystick is not calibrated at this point. To calibrate the joystick:
1. Go back to the CAD Settings page
2. Press the Joystick Zero button when the joystick is centered and still. This sets the zero point for the joystick. 
3. Press the Joystick Scale button and move the joystick around to the extremes in X and Y. You have 5s. Once the function finishes the calibration parameters will be calculated and the joystick should show a range of roughly +/- 1 in both X and Y. 
4. If you are happy with the calibration, press the Save CAD Configuration button and the parameters will be saved to the cadparams.json file in the ESP32. This file can be downloaded from the ESP32 to your PC via the CAD Settings page in the configurator. It is a good idea to do this and to copy the file to the data\config folder in the CADDeck code so that you won't lose the parameters next time you upload the data folder to the ESP32.
4.  If you are not happy with the results, try again.

Bind your computer to the CADDeck using the Bluetooth setings page on your computer.

# Configuration
Note that all of the pages shown or mentioned above are samples. Every page can be reorganized and customized to suit your needs via the configurator.

## WiFi Configuration
The configurator is accessed via a web browser on a computer or tablet connected to the same network as the ESP32. To enable the configurator go to the settings page on the touch panel and select the WiFi Enable button.  
![System Settings](images/systemsettings.png)

If you have already configured your wifi connection in wificonfig.json you should see a message saying that it has connected to the wifi and will display the IP address. At that point you can go to your browser and enter the IP address top connect. The configurator will be displayed.
![Configurator Main Page](images/mainconfiguratorpage.png)

If you have not already configured the wifi you should see a message saying that an access point (AP) has been created with a network name and IP address. You will need to connect your computer to the network provided, and then point your browser at the IP address. Once the configurator screen is displayed in your browser go to the wifi page and enter your network SSID and password. Then select "Save WiFi Config" to save the settings. 

## Menu Configuration
The main change from the original FreeTouchDeck is to allow more buttons per page  - the screen area is just a bit smaller than the size of twelve StreamDeck buttons which seems reasonable. The code is currently set to three rows of four buttons, but can be changed to three rows of five buttons (or two rows of four, etc.) by modifying CADDeck.h: "#define BUTTON_ROWS 3" and "#define BUTTON_COLS 5".  In principle it should support larger arrays, but not tested, and the configurator page is set to a maximum of 3x5. 

The configurator displays an array of three rows of five buttons, with a drop down list to select the menu page to be modified. If you are using fewer than 3x5, simply ignore the extra rows and columns. On the to-do list is to generate the HTML dynamically to correspond with the number of rows and columns.
![Menu Configurator](images/MenuConfigurator.png)

Each Menu page has a 2D array of programmable buttons, and each button has an array of 3 actions that can be defined for when the button is pressed.

You can set different background colors for a button that performs an action (black background in the image above), or links to another menu (light blue in the image.) This is controlled on the Settings page of the configurator.

If you don't want to use all buttons use the configurator to set the logo to "blank.bmp" and make sure that there are no actions defined for that button. Any button with those characteristics will not be drawn.

One limitation is the size of the FLASH used for the configuration files and icons displayed on the touch screen. The icons are 75x75 24-bit color BMP files, each one taking about 17kB.  If you run out of space your only option is to check to see if any are unused and delete them.


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
3. The actions that must be taken to enable pan/rotate/zoom to be controlled by the joystick (i.e. the mouse) within the selected CAD program. Note that the definition of what is needed to control pan/rotate/zoom is separate from which button enables that mode. As an example, Solidworks needs the middle mouse button to be held down to rotate the view. So, JoystickRotate/Action1 is set to "Mouse Buttons" and Value1 is set to "Press Middle Button". To assign the Rotate function to button 8, click "8" in the image to the left, then set SelectedButton/Action1 to "CAD Functions" and Value1 to "Joystick Rotate"
![Configuring the Rotate function](images/ConfigureRotate.png)
At the very bottom is the "Save CAD Config" button. When selected the settings for all CAD programs are saved on the ESP32 in file cadparams.json

## Default Configuration
While everything can be changed via the configurator, there are some default settings provided with the code. These are:

### Control assignments
1. Joystick moves the mouse pointer
2. Joystick with pressed button 0 pans the view
3. Rotate Joystick rotates the view
4. Pushing the joystick down or pulling it up zooms the view.
5. Button 1 is measure ("i" in Fusion360) (I assigned "i" as a keyboard shortcut to measure in Solidworks)
6. Button 2 sets the zoom to fit ("F6" in Fusion360), ("f" in Solidworks)
7. Button 3 unselects ("ESC" in Fusion360 and Solidworks)
8. Button 4 toggles object visibility ("v" in Fusion360) or hides object ("TAB" in Solidworks)

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
 

