# ESP_MQTT_Test
Simple MQTT Client program using ESP32 and Mosquito running on Raspberry PI.

As with all my projects, will strip back the code to the minumum needed, to keep it as simple as posible.

Want to test a couple of things here:

1: MQTT communication, followed RandonNerd tutorial about setting up the Raspberry PI MQTT. Its a great site.

2: Test power usage on Wroom board when using sleep mode.

Set up program to make Wifi connection, read ESP32 internal temperature, update Mqtt server and sleep for 30 seconds and repeat.

Monitored power usage and found the Wroom board takes around 50mA running 150mA with Wifi turned on and < 0,5mA in deep sleep

Next Step is to power board from 1100mA/hour 3.7V battery and see how long it runs. Not sure it 3.7V will make it through the 3v3 voltage regulator on the wroom board?

Following that, create Raspberry PI webserver and serve browser pages with data from the MQTT server (readings from the ESP32). Will need to use PHP for the first time?
Don't want to use Node Red, would rather program at a lower lever (C, C#, java)

