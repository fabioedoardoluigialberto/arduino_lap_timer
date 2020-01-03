# arduino_lap_timer

Lap timer for slot car racing.

It shows a semaphore to start, then times all laps on each lane until a given number of laps or reset button is pressed, then shows mean and best lap as well as total time to complete 20 valid laps.
Tested on [Carrera Go! Ferrari Edition](https://www.amazon.com/Carrera-Ferrari-Spirit-Electric-Racing/dp/B07PFYGTJ1/ref=sr_1_1?keywords=carrera+go+ferrari&qid=1577680615&sr=8-1) (1:43).

## Schematics

The schematics has been largely taken from the Crystal Ball project in the Arduino Uno Kit and modified.
It has a 2-rows LCD for displaying time and a potentiometer for contrast regulation.
I use the button in the project to start and reset the timer.
I added two sensors to sense the passage of the car.
I also added the piezo to emit sounds during semaphore at every lap.

# Usage

Hit the start/reset button.
A semaphore will be shown.
Time will start when all lights are all off and a high sound will be the GO! signal.
Everytime the sensor senses the passage of the car, the lap time will be recorded and shown and the time will reset to 0.
When the maximum number of laps has been reached (set to 20), the mean and best lap times are shown as well as the total time to complete 20 valid laps.
To end before the maximum number of laps is reached, hit the start/reset button (will not include the current lap).

## Note on first lap

The very first pass through the start is not counted as a lap.
This is because it is assumed that the car starts from right before the start line so after the GO! signal the first pass should not be considered a lap.
However, the time that passes from when lights are off to the when the car passes the line for the first time counts for the final mean time!
For instance, if 200 ms pass between GO! signal and 20 is the total number of laps, the delay will count as 10 ms every lap.

## Valid laps

I found it useful to filter out invalid laps when testing things out.
Invalid laps are when the sensor is activate too quickly (laps < 1s) or when a lap took too long, e.g., when car went off track.
I considered an average time of 2.7 s to complete the track so a valid lap is below 2.7+5s.
This is useful because at then end, the total time will consider everything, including the time it takes to put a car back on the lane, but the mean and best lap will reflect actual complete laps without leaving the track.
The timing ends when 20 valid laps are completed, while the total time will continue to accumulate even for invalid laps!

## Note on the sensors

I started with an [infrared obstacle avoidance module](https://www.amazon.com/gp/product/B01I57HIJ0/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1).
Not the best perhaps but after tuning sensitivity to decently high, it seems to do the job when positioned close to the track.
A better option is definetely to use [reed sensors](https://www.amazon.com/Sensor-Module-Magnetron-Normally-Magnetic/dp/B01FDHG1CA/ref=sr_1_1?crid=353U749WXA2XZ&keywords=reed+sensor+module&qid=1577680755&sprefix=reed+se%2Caps%2C171&sr=8-1) mounted underneath the track under each lane.
Even better would perhaps be a [hall effect sensor](https://www.amazon.com/WINGONEER-Effect-KY-003-Magnetic-Arduino/dp/B06XHG9CYN/ref=sr_1_6?crid=2J5NX9VJBVSQ1&keywords=hall+effect+sensor+module&qid=1577680925&sprefix=hall+eff%2Caps%2C209&sr=8-6) but haven't tried yet.
