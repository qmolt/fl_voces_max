# About

flvoces~ is an external for Max/MSP written in C. For Windows only (for now). flvoces outputs a chord (list) with an specific voicing detailed on its 3 inputs: 

- the first inlet is a list of the scale in integer format (i.e.: 0 2 4 5 7 9 11 (major scale))
- on the second inlet a list with the 'colors' or indexes of the scale that shape the chord
- and a list of octaves for each color

2nd and 3rd inlet must have the same size. 

It is possible to change the base from 12 to any other number using the message 'mod_base'.

A compiled external (.mxe64) and a help file (.maxhelp) are provided in the example folder, as always.


# Versions History

**0.1** 
- first version