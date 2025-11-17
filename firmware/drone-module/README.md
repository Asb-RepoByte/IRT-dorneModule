This is the module responsible for reading sensor data and sending it wirlessly to the fixed-module

- right now this is just a simple code to test that the nRF24L01 module is working as expected
- the scanTask scan the whole 2.4Ghz band looking for busy channels, the python file open the serial file and store
    data int a file so plotting and analyzing it later is easier.
- for that to work the serial monitor must be deactivated, no two tasks can read the same file at the same time the same goes for serial file "/dev/ttyUSB0" on linux or "COM0" on windows
