# Installation
* Xboard and Fairymax
    * `sudo apt-get install xboard fairymax`

* Our chess engine
    * `git clone https://github.com/mihai-varga/chess-engine.git && cd chess-engine && make -j 2`
# How to play
* Our engine (white) vs Fairymax (black)
    * `make white`
* Our engine (black) vs Fairymax (white)
    * `make black`
* You (white) vs our engine (black)
    * `make hwhite`
* You (black) vs our engine (white)
    * `make hblack`
* Our engine vs itself
    * `make solomid`
