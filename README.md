# Styx
Simple game my friend (jdlogan) and I developed in two days for a one month C++ project class. The objective of the game is to score as many points as possible by avoiding the monsters (stars and eyeballs) and jumping as high up as you can. You gain altitude by jumping off of walls and platforms (pressing the Spacebar near them), and directing your player using WASD keys.

We developed this game using the CodeBlocks IDE. The best way to start up this game is to start it Styx\styx\Styx.cbp (project file for CodeBlocks). This eliminates the hassle from worrying about missing libraries (if this still happens, Google the error message for a potentially easy fix). If you can run it, there is a Styx.exe in the same folder, although this has some issues on certain computers, as SFML libraries might be missing .dlls on your computer.

Beware about running this game for too long, as game characters and objects outside the window remain in memory (we did not implement deletion of objects on time for the final presentation as we did not prioritize it).
