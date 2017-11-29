# cs214-sorter2-multithreaded
This project is a multi-threaded version of the previous sorter1 project.

team: !failing members: Joseph Redling-Pace and Xiaopeng (Jake) Zhou

Note: when compiling our code, please use the included Makefile. If you decide not to, the pthread library must be included on compilation to ensure our code works.

We started this project by first modularizing the input flags from the command line so that the order of the 3 flags would not matter.

We then moved on to phasing out all the references to forking and related processes (such as the return status to keep count of total # of threads) and replacing them with respective threading code instead.

We had to alter the code and the Makefile to include thread libraries so that we can actually implement thread creation. We also wrote code for new structs that contain fields for the arguments used in the functions we call new threads on. We placed these in the header file.

We ran into some difficulties such as figuring out how to wait for threads to complete before returning from the main process, and how to ensure that we were actually multithreading as opposed to just waiting for each thread to finish before moving on in our program.
