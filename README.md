# summer-internship-2020

### 0713
+ 0713-1.cpp: random hard instance search for **Candidate 1** (consider semi-OCS)
+ 0713-2.cpp: gamma calculation for instances in *upper bound based on a triangle.pdf*
+ 0713-3.cpp: brute force verification of the above result, you can possibly adapt any algorithm inside

### 0718
+ fraction.h: implementation of fractions
+ simplex.h: simplex method using ***long double***
+ simplex_fraciton.h: simplex method using ***fraction***
+ calculate.cpp: calculate the offline upper-bound gamma for an instance(use ***long double***)
+ details.cpp: give corresponding algorithm for the upper-bound gamma of instances (1,2)+n*[(1,3), (1,2)] (use ***fraction***)
+ find.cpp: randomly create instances and find the best upper-bound among them(use ***long double***)

### 0725
Implementation of checking performance of an algorithm on a case. The algorithm should be contained in some \*.h and the case should be input. In \*.h, you need to implement
+ the definition of State, which contains the state variables used in your algorithm
+ operator *<* and *!=* for State
+ function *algorithm*, which returns all the possible runs along with their probabilities

You only need to compile *main.cpp* to run the program.
