# arduino_lap_timer

Lap timer for slot car racing.

It shows a semaphore to start, then times all laps until a given number of laps or reset button is pressed, then shows mean time and best lap.

## Schematics

The schematics has been largely taken from the Crystal Ball project in the Arduino Uno Kit.
I only added one more button to reset the timer.
It uses a 2-rows LCD for displaying time and a potentiometer for contrast regulation.

# Usage

Hit the lap button.
A semaphore will be shown.
Time will start when all lights are off. Hit button again for a new lap (can be replaced by slot car sensor).
When the maximum number of laps has been reached, the mean and best lap times are shown.
To end before the maximum number of laps is reached, hit the reset button.
