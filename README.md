# ModularMinsk

Minsk in C using a modular design.

## Libraries

The point of this project is not necessarily the Minsk implementation,
but to motivate the development of several standalone libraries
for use in other projects.

Some of the libraries I use in this project
(and that you may copy into your own projects)
include:

- vec -- a type-generic vector using few macros
- annotate -- allows annotating parameters that shouldn't be NULL
- embed -- generates C code embedding a file as a string
  Dependencies: [argparse, nonstd]
- hash -- a type-generic hash table with a similar design to vec
  Dependencies: [vec, span]
- nonstd -- cross-platform replacement for non-standard functions
- span -- a type-generic span created from two pointers
- string -- a string library based on vec and span
  Dependencies: [vec, span]
