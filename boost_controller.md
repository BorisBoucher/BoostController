

# Square wave inputs

## __Engine RPM__
This input measure the engine RPM by counting ignition pulse from the ECM.

Signal range:
 * 500 to 7000 RPM
 * 6 cylinders, 4 strokes => 3 ignitions per rev
    * 1500 pulse to 21000 pulse per minute
 * Ignition pulse frequency range : 25 Hz to 350 Hz
 * Ignition pulse period range    : 40 ms to 2.857 ms
 * Measurement rez @ max freq     : 
    * 50 rpm step @ 7000 rpm
    * 6950 rpm => 20850 pulse per minutes
    * 347.5 Hz => 2.87 ms 
    * 2.857 ms - 2.877 ms => delta period ~19 µs 


## __Vehicle Speed__
This input measure the vehicle speed by counting speed sensor pulses.

Signal range:
 * 5 to 300 km/h
 * 3.6 Hz to 220 Hz (from test realized on running vehicle)
 * Period range : 4.545 ms to 277 ms
 * Measurement res @ max speed:
    * 1 km/h step @ 300 km/h
    * 299 km/h => 219.2666 Hz => 4.560 ms,
    * 4.560 ms - 4.545 ms => delta period ~15.2 µs

## __MAF__
This input measure the mass air flow by counting sensor pulses that are nearly proportional to volume air flow.
To be accurate, this data is complemented by barometric pressure and air temperature to allow
the ECU to compute the air mass.

Original 3000GT ECU did not provide full scale and high resolution value
for the MAF HZ, so we resample it in the BC to provide accurate logging data.

Signal range:
 * 15 Hz to 2600 Hz (15 Hz os roughly half the idle air flow, 2600 Hz is barely above maximum sensor capacity where is loose accuracy)
 * Period range : 66 ms to 384 µs
 * Measurement res @ max freq
    * 50 Hz step @ 2600 Hz
    * 2550 Hz => 392 µs
    * 392 µs - 384 µs => delta period = 8 µs

## __Input capture resolution__
All square wave inputs are acquired by using hardware input capture with timer 2.

The timer is based on a 16 bits counter and receive an input clock of 72MHz.

The most demanding signal is the MAF with an expected resolution better than 8µs.

We need to adjust the timer divisor to lower the number of counter overflow interrupt.

* Tick duration : $\frac{1}{72\,MHz} = 0.0138888\,µs$
* For 8µs, the tick counts is $\frac{8\,µs}{0.0138888\,µs} = 576\,ticks$

We can choose a divisor of 500 to have a resolution of $0.013888\,µs * 500 = 6.9444\,µs$

This leads to an overflow interrupt every 65536 * 6.9444 µs = ~455 ms, i.e. roughly 2 Hz.

# Analog Inputs

## __Throttle Position Sensor / TPS__
Signal 0..5 V (closed to fully open) from a potentiometer connected between +5 V and ground.

## __Manifold Absolute Pressure / MAP__
3 bar sensor

Signal 0..5V, 1V per bar (vacuum to 3 bar absolute, equivalent to 2 bar boost).

## __Fuel & Oil Pressure  / FP, OP__
150 PSI sensor

Signal is linear 0.5 to 4.5 V, 0.5V @ 0 PSI, 4.5V @ 150PSI (10 bar) => 2.58 bar/V.

## __Water temperature / WT__
