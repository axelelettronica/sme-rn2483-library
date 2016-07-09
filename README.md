# SmartEverything Telit SE868-AS GPS System
----

This is a library for the ST VL6180X component.

The SE868-AS is a GPS Smart antenna module featuring an embedded antenna
with an optimized RF path and standard SMT mounting.

The SE868-AS features an 11 x 11 mm QFN package based on a low-power consumption
MT3337 GPS chipset and an embedded 9x9mm antenna on top of the module.

The SE868-AS is designed to track and navigate GPS and QZSS constellations,
giving the positioning data though standard UART at a very low power rate.

The SE868-AS provides customers customers with a seamless integration solution,
whether the RF chain is subject to several constraints in terms of host PCB
or if a preassembled antenna solution is required.

The SE868-AS module has the same form, fit, and protocol as its multi-constellation counterpart,
the SE868-A. This enables integrators and developers to design their applications once
and take advantage of the flexibility afforded by the combination of the different and scalable GNSS technologies.

The Jupiter SE868-AS, with its companion multi-GNSS module SE868-A,
extends the Telit smart antenna portfolio and provides the optimum mix of performances,
cost and  product scalability.

The Jupiter SE868-AS supports ephemeris file injection (A-GPS) as well as
Satellite Based Augmentation System (SBAS) to increase position accuracy.
Its onboard software engine is able to locally predict ephemeris up to three days in advance,
starting from ephemeris data broadcast by GNSS satellites received by the module.
An external or host memory is required to store the Ephemeris files.


[* Telit  SE868-A Home Page *](http://www.telit.com/products/product-service-selector/product-service-selector/show/product/jupiter-se868-as/)


It was principally designed to work with the SmartEverything board, but could
be easily adapt and use on every Arduino and Arduino Certified boards.

Written by Seve <seve@axelelettronica.it>.

## Repository Contents
-------------------
* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE.
* **/src** - Source files for the library (.cpp, .h).
* **library.properties** - General library properties for the Arduino package manager.

## Releases  
---  
#### v1.0.0 First Release  
#### v1.0.1 Second Release 19-Dec-2015  
* Fixed Issue:<br>  
    Added example to set the GPS module in standby to reduce power consumption<br>.  
 
## Documentation
--------------

* **[Installing an Arduino Library Guide](http://www.arduino.cc/en/Guide/Libraries#toc3)** - How to install a SmartEverything library on the Arduino IDE using the Library Manager


## License Information
-------------------

Copyright (c) Amel Technology. All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
