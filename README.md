
# CHARLIE

composed and programmed by [Amogh Dwivedi](https://www.amoghdwivedi.com)
### About
This was my final project for [David Cardona's](https://college.berklee.edu/faculty/david-cardona) Digital Instrument Building course, finished in May 2024, with some revisions made in November 2024. 

Check out [this video](https://youtu.be/u3bhTx4QBjI) to learn more about the functionality of this instrument and the compositional techniques I used.

### Microcontroller setup

* the microcontroller setup can be found in the video.

* remember to install the [SHT31-D library](https://www.adafruit.com/product/2857)

* upload the code onto your Teensy 4.0 using Arduino IDE

* further reading
    * [photocell setup](https://learn.adafruit.com/photocells/overview)
    * [ultrasonic sensor](https://projecthub.arduino.cc/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1)
    * [millis() instead of delay()](https://www.norwegiancreations.com/2017/09/arduino-tutorial-using-millis-instead-of-delay/)

### Max/MSP setup
* Max 8.6 or later should work.

* copy files the files in the Library folder into your Max Library.

* install the rnbo guitar pedals through the Package Manager in Max. 

### Further development
* Further testing needs to be done with the the photocell. Currently, a 10kΩ pulldown resistor is being used, which "will quickly saturate", as per the [Adafruit resource on photocells](https://learn.adafruit.com/photocells/using-a-photocell). Replacements can be made in order to "detect bright light differences better". This would be suitable for more accuracies anytime before dusk, which is when most people are in the esplanade anyway.
* One year may be a slightly ambitious duration length, considering the practicalities of setting the instrument. Another possibilities would be to host the installation at various points in the year, in order to have different movements (similar to "4 seasons"). 
* A laptop setup itself is slightly awkward. Other possible solutions include using Daisy/Raspberry Pi microcontrollers, and potentially foregoing real time synthesis in favor of mixing between bounced files. 
