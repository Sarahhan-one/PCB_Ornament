# Christmas Tree LED Ornament
This project creates a festive, green Christmas tree ornament featuring 10 twinkling LED lights and holiday tunes. :christmas_tree:

Inspired by [Justin’s ornament project](https://github.com/incanus/ornament) and [Breadstick Innovation's ornament](https://shop.breadstick.ca/products/christmas-tree-pre-built), this ornament is designed with an ATtiny85 microcontroller, making it perfect for a holiday decoration. 

![KakaoTalk_20241108_003517334](https://github.com/user-attachments/assets/751fccee-a0e6-40fa-a2e4-2da3b07997b6)
![Remove background project_crop](https://github.com/user-attachments/assets/a9354102-9ac7-42e1-9c3a-504289c94339)

**[Check out the video in action](https://youtu.be/ctR4pCAqMrQ?si=eUrizgF7dfWRZ8VW)**




## Features
* **LED Twinkling Effect**: Two groups of LEDs that fade and brighten in a twinkling effect.
* **Holiday Songs**: Play three Christmas classics—"Jingle Bells," "First Noel," and "O Holy Night"—by pressing the ornament's button to cycle through songs.
* **Power**: Powered by a 3V coin cell battery, operate with a slide switch for power control.

## Operation
Use the slide switch on the back to turn the ornament on or off. Once powered on, the LEDs will start twinkling in an alternating pattern.
Press the button to play holiday-themed songs. Each press advances to the next song in the sequence.

## Circuit
![image](https://github.com/user-attachments/assets/b5b5f301-a6c7-4c98-8272-16c417b00c7a)
To drive the LED display efficiently, the circuit uses a D331 (S8050) transistor to handle the LED current, 
ensuring that the ATtiny85 microcontroller isn’t overloaded. 
The 1kΩ base resistor provides sufficient current to fully saturate the transistor, enabling it to manage the 20 mA needed for five LEDs in parallel.
The pcb directory includes EasyEDA project files that outline the PCB design. 

## Program 
* **LED Control**:
  - Two groups of LEDs are set up to fade in and out alternately using Pulse Width Modulation (PWM).
A fadeValue variable determines the brightness, which increases and decreases to create a twinkling effect.
  - The `lightupLED()` function controls the intensity of each LED group, updating at regular intervals to maintain a smooth fading pattern.

* **Button Press and Song Selection**:
  - A debounced button press is implemented with `buttonState()` to avoid misreads.
Each button press increments the `currentSongIndex`, cycling through the three songs.
  - The `playSong()` function sets up the melody and tempo for each song, pulling notes from pre-defined arrays of tones and durations.

* **Audio Playback**:
  - The `playNote()` function generates sound based on the note and octave using a buzzer connected to the ATtiny85.
  - The `updateAudio()` function checks the timing to play each note in sequence, based on the song's tempo and the duration of each note.
 
### Happy Holidays! :christmas_tree:




