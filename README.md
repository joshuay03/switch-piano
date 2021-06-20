# Switch Piano

Switch Piano is an ATmega328P microcontroller project, written in C, and simulated using Tinkercad (COVID-19 -> No hands on practicals at uni, sigh...).
The circuit can be viewed at the following link: https://www.tinkercad.com/things/idAXq9fzEPD.

![Tinkercad initial view](https://user-images.githubusercontent.com/54629302/122638421-58505c00-d137-11eb-865c-41e97cc776f5.png)

The end result is 8 buttons, which represent 8 piano keys (excluding the black keys), from Midle C to C one octave above, which when pressed, plays the frequency of that specific note through the piezzo buzzer, lights up the LED, and displays the key being played on the LCD. The circuit consists of two Arduino Uno R3's, one to handle the LCD, and the other the buttons, LED and buzzer.
 
![Tinkercad circuit in action](https://user-images.githubusercontent.com/54629302/122638429-5dada680-d137-11eb-87c0-fad44bc7b1c3.png)

**Note:** At times, the simulation does not output any sound. Reloading the page usually solves this.
