# cs214-sorter2-multithreaded
This project is a multi-threaded version of the previous sorter1 project.

team: !failing members: Joseph Redling-Pace and Xiaopeng (Jake) Zhou

Note: when compiling our code, please use the included Makefile. If you decide not to, the pthread library must be included on compilation to ensure our code works.

We started this project by first modularizing the input flags from the command line so that the order of the 3 flags would not matter.

We then moved on to phasing out all the references to forking and related processes (such as the return status to keep count of total # of threads) and replacing them with respective threading code instead.
