## ATTiny-based Ultrasonic Parking Proximity Sensor ##

### Background ###
This is my attempt to put together a parking assist device that will do the obvious and help me not hit the wall in my garage. Could I use a tennis ball on a string like a god damn neanderthal? Sure, but where's the fun in that? =)

### Parts List (Incomplete) ###
* ATTiny85 (or maybe 84) will be the brains of the operation
* HC-SR04 ultrasonic sensor from eBay as the eyes (ears?)
* RGB LED Ring with 24 WS2812b 5050 LED chips around a 66mm Ring (NeoPixel like)
* SPST Switch for setting the desired distance from the wall
* 1000µF Electrolytic Capacitor for power buffering the Neopixel ring
* 470Ω Resistor on the data line between MCP and Neopixel ring
* 2.1mm DC Barrel Jack for power input
* (2) 3.5mm TRS jacks for connecting the sensor head to the main board


### 3D Printed Enclosures
#### Ultrasonic Sensor Enclosure
<div style='text-align:center'>
<a href="/hardware/mounting/renders/transducer_housing_solid_export.png?raw=true"><img src="/hardware/mounting/renders/transducer_housing_xray_export.png" alt="X-Ray View" width="200"></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<a href="/hardware/mounting/renders/transducer_housing_xray_export.png?raw=true"><img src="/hardware/mounting/renders/transducer_housing_xray_export.png" alt="X-Ray View" width="200"></a>
</div>
* Wired to main enclosure for placement options

#### Main board and display unit enclosure
* Forthcoming
