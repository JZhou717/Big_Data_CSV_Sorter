# cs214-sorter2-multithreaded
This project is a multi-threaded version of the previous sorter1 project.

team: !failing members: Joseph Redling-Pace and Xiaopeng (Jake) Zhou

We started this project by first modularizing the input flags from the command line so that the order of the 3 flags would not matter.

We then moved on to phasing out all the references to forking and related processes (such as the return status to keep count of total # of threads) and replacing them with respective threading code instead.
