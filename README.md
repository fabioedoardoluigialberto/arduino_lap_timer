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
Time will start when all lights are all off and a high sound will be the GO! signal.
Hit button again for every new lap (can be replaced by slot car sensor).
When the maximum number of laps has been reached (set to 20), the mean and best lap times are shown.
To end before the maximum number of laps is reached, hit the reset button.

## Note on first lap

The very first pass through the start is not counted as a lap.
This is because it is assumed that the car starts from right before the start line so after the GO! signal the first pass should not be considered a lap.
However, the time that passes from when lights are off to the when the car passes the line for the first time counts for the final mean time!
For instance, if 200 ms pass between GO! signal and 20 is the total number of laps, the delay will count as 10 ms every lap.
