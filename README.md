# MCU Instrument

Project Specifications

Using the Keil development environment, develop a program that will simultaneously perform the following tasks:


Task1: Operate one channel of the speaker as a Theremin, as specified in Lab 10.  Use Timer TIM1.


Task2: Operate the other channel of the speaker as a Toy Piano, as specified in Lab 9.  Use Timer TIM3.


Task3: Display notes that are entered from the piano keyboard on the TeraTerm terminal.  The display for each note should include the note number and the time (i.e., the millisecond count) when the note turns on and turns off.  The display format is illustrated by the following example:


Note 65 On 10666
Note 65 Off 11053
Note 62 On 11357
Note 62 Off 11644


Task4: Blink the green and blue LEDs.  The blink rates are independent and should range from 1 to 10 blinks per second.  Control the blink rates using the TeraTerm keyboard.  Press key B to increment the blue blink rate and key G to increment the green blink rate.  Incrementing a blink rate of 10 should produce a blink rate of 1.  The control keys are pressed between notes played on the piano, and the keystroke should be echoed.  For example, if the blue blink rate is increased by 2 after the first note and the green rate increased by 3 after the second note, the display would look as follows:


Note 65 On 10666
Note 65 Off 11053
bb
Note 62 On 11357
Note 62 Off 11644
ggg
Note 67 On 10666
Note 67 Off 11053


The four tasks should appear to operate simultaneously and should not interfere with each other.
