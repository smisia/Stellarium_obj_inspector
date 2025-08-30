# Stellarium object inspector

Currently, the python script lets you retrieve data about the object that was clicked in the Stellarium planetary software, since the current version of Stellarium's API does not support this feature (as far as I know). 

## Disclaimer
This is NOT and official extension for the Stellarium software only some useful scripts/programs!
<br/>The project is still ongoing it will get updates from time to time.
<br/>Feedback and suggestions is always welcome :)

## How to use it
1. You need to have the following libraries installed in order to work: `bs4`,`requests`,`json` and `Flask`, you can insall them with the simple PIP command
2. You have to launch Stellarium with the **Remote control** plugin enabled
3. (You have to set it to **Launch at startup** and when you click on the settings, the **Server enabled** and **Enabled automatically on startup** checkboxes shoud be ticked too)
4. Run the `Launch_Stellrium_data_fetch.bat` and the Flask web server automatically starts, and begins to send the chosen data trough HTTP, you just have to implement a GET method on the recieving end.
5. To exit press **Ctrl + C**

Legacy:
1. You just have to launch the `Launch_Stellrium_data_fetch.bat` file OR you can configure it by opening the `Stellarium_planet_data_fetcher.py` file in any text editor and edit the **User parameters**
2. You will recieve the data periodically about the clicked object, you just have to let it run if you want to extend it further.
3. To exit press **Ctrl + C**

## Notes
1. Is it still possible to use the `Stellarium_planet_data_fetcher.py` individually you just have to run it in a different .bat file (or from cmd). The standalone script got some small updates but still usable perfectly fine.

## TODO
[x] Create a sw for sending data to the MCU
[x] MCU data processing
[] MCU other functionality implementation
[] Create a HAT for MCU usability functions
[] Create rotator mechanism

