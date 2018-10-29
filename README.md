# ELgamal

This is a C-implementation of the Elgamal cryptosystem,
and was done by 

*) Cuate González Oliver Fernando, and

*) Chi-Domínguez Jesús-Javier


#-----------------------------------------------#

Getting Started:

These instructions will get you a copy of the 
project up and running on your local machine 
for development and testing purposes. 

#-----------------------------------------------#

Prerequisites:

Any version of gcc, and gmp library must be 
installed. In addition, you need two extra files:
one which contains the message and another with 
the following data:
The prime : 2 * k * q + 1, each prime factor of k 
(without multiplicity), q and 2. 

You can see the file primes.txt as an example.

#-----------------------------------------------#

Installing:

    gcc scheme.c -o scheme -lgmp

#-----------------------------------------------#

Running the tests: 
   
   just run ./scheme and follow the indications

