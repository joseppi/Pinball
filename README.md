# Pinball
This small project is based around a 2D pinball game based on the box2D library. The game consists of in making the maximum score until you lose your three lives.

On the [repository link](https://github.com/joseppi/Pinball) is posible to check for code and the release.

## Installation
Decompress the .zip file in any directory and execute the executable inside the decompressed folder.

## Functionalities
- The spring does impulse the ball for starting a new game.
- Flippers do move in limited angles to boost away from the ball.
- Bouncers (circular and triangular shapes) rebound the ball when hitting them. (The are two mini ones of them when actioning the       flippers; used for blocking the exit).
- Sensors make sure that when a bouncer is touched these change appearance.
- Score and lives mechanics are used when hitting the sensors and losing lives when the ball falls at the bottom of the pinball.
- Sound FX play at each mechanism (spring, flipper) is launched and when colliding too (bouncers), apart from the UI interaction. 

## Usage (keyboard)
- Left arrow: Activate left flipper and it's assigned mini bouncer (located behind the first wall).
- Right arrow: Activate right flipper and it's assigned mini bouncer (located behind the second wall).
- Spacebar: Apply force to the spring when starting a new game.
- R: Restart score and lives.
- M: Mute the music.
- +/-: Increase, decrease volume.

### Special features
- F1: Show logical 2D physics (shapes, figures and joints).

## Credits
- Vladimir Agache ([Github](https://github.com/VladimirA97))
- Josep Pi ([Github](https://github.com/joseppi))
- Albert Mas ([Github](https://github.com/albertmas))

## License
Copyright (c) [2017] [Vladimir Agache, Josep Pi, Albert Mas]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
