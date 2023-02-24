# Polynomial-Calculation
Authored by Shahar Amram


### ==Description==

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

Finally ,to exit from the program enter command input "done".


### ==Program DATABASE==
1.str = array of char size 512 , input from the user.

2.results = array of pointer, save the calculate from each thread.

3.threads = array of type pthread, represnt the threads.

4.p = array of type double char, separate the all string to different small string to calculate.


### ==Functions==

1.res - function type void*, get input of void* and cast it to char*.calculate the given string with the input number from the user.

2.freeM - function gets pointer to array of dynamic memory and free it, each by each than free the main memory.

3.atof()-build in function, change string to number type double.

4.pthread_create() - build in function, create the threads.

5.pthread_join() - build in function, 'wait' for each thread that create.



### ==Program Files==

polynomCalc.c - the main file.

### ==How to compile?==

gcc polynomCalc.c -o polynomCalc -lpthread

run: ./polynomCalc

==Input==

sentence -polinom 3 and down degree + ", " + value to put in the polinom (512 chars).

==Output==

solution for the polinom with the given value.(type double)
Illegal string , or build in function failed - the program will print error.
"done" input - exit the program succesfully

