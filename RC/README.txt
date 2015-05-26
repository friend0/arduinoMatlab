Version 0.3
- ADD: Landing gear support [#24]
- CHG: PPMOut may use any pin as output pin
- ADD: Wing type mixing [#25]
- ADD: Tail type mixing [#26]
- ADD: Flaps support [#27]
- ADD: Airbrake support [#29]
- BUG: PPMIn detects loss of signal [#34]
- ADD: Aileron differential [#35]
- ADD: FlycamOne support [#40]
- ADD: Smooth transitions on switches [#33]
- ADD: Throttle hold
- ADD: Input to input mix [#31]

Version 0.2
- BUG: Incorrect timings in PPMOut [#19]
- CHG: Use of Timer1 refactored for shared use [#18]
- CHG: Signal properties Center and Travel have been moved to utility functions [#20]
- ADD: PPMIn, PPM reader/decoder (trainer port) [#2]
- ADD: ServoIn, Servo signal reader/decoder [#16]
- ADD: ServoOut, Servo signal writer/encoder [#17]
- CHG: Remove all use of normalized values from Signal generating/reading code [#22]
- CHG: Timer1 code and interrupts moved to separate class [#23]

Version 0.1
- ADD: AIPin, analog input; trim, reverse, calibration
- ADD: Channel, transformations; end points, reverse, subtrim
- ADD: Curve, for throttle and pitch curves
- ADD: DIPin, digital input; reverse
- ADD: DualRates
- ADD: Expo, for exponential input
- ADD: Gyro, support AVCS and Normal gyros
- ADD: PPMOut, configurable PPM generator; channels, pulse length, servo center, servo travel, invert, pin
- ADD: Swashplate, support all common types and mixing