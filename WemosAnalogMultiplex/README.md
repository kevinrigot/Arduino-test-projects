How to use an analog multiplexer like CD4051BE.

This is the continuation of [ShiftRegister](../WemosShiftRegister) test project.
In the previous project, I had 4 leds connected to a Shift register. In this one, there are 4 switch/push button to turn on/off the leds. There is also a potentiometer that will adjust the blinking rate. All these 5 inputs are connected to an analog multiplexer.


Reference: 
- https://www.youtube.com/watch?v=qTmBuIZk6KY
- https://www.ti.com/lit/ds/symlink/cd4051b.pdf

Components:
- Wemos D1 Lite
- Analog Multiplex CD4051BE
- 4x Resistor 10k ohm
- 2x push button, 2x switch button
- 1 potentiometer

Schema:
![Alt text](./schemas/AnalogMultiplex_bb.png?raw=true "Breadboard")
![Alt text](./schemas/analog_multiplex1.jpg?raw=true "Photo1")
![Alt text](./schemas/analog_multiplex2.jpg?raw=true "Photo2")
