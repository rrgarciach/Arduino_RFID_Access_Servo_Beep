RFID_Access_Servo_Beep
======================

Arduino project to perform a automation device for door-lock access with RFID/NFC ID cards using a servo motor and Bluetooth devices to sync data with database on remote server.

The device reads NFC cards using a NFC module and checks the access privileges sending the request through Bluetooth communication to a Bluetooth piconet mediator (this works as an interface) which connects with an Internet access point device (an Android device in this case). The Internet access point device returns the result through the Bluetooth and the Arduino perfoms the opening action if privileges are right.

This project it to hack doors hanging just its handle for the cases where theres no chance to modify the door's frame (see related diagram and pictures).
