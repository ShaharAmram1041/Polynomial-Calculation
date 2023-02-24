# Polynomial-Calculation
Authored by Shahar Amram


==Description==

Polynomial calculation using threads.

program contain 1 file:

polynomCalc.c:
The program calculate given polinom from 3 and down degree.
The program will get input of sentence (that represent function), and number (represent the value that go into the polinom) from the user in infinity loop.
the program check how many 'x' there is in the sentence, and create threads by the number of 'x'.
The sentence will be separated by the '+' command, and each of the separated smaller string will save in array of double pointer.
each of them will sent by every thread to void* function that return the calculate with the given number from tha user.
The program will wait for eacht thread by the 'join' command and collect the result to array.
finally, we summerize all the result to one and print the answar of the polinom.



