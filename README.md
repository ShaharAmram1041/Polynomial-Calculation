# Polynomial-Calculation
Authored by Shahar Amram


## ==Description==

Polynomial calculation using threads and fork.

program contain 2 files:

### ***polynomCalcThreads.c***:<br>
Polynomial calculation by **threads**.<br>
The program calculate given polinom from 3 and down degree.
The program will get input of sentence (that represent function), and number (represent the value that go into the polinom) from the user in infinity loop.
the program check how many 'x' there is in the sentence, and create threads by the number of 'x'.
The sentence will be separated by the '+' command, and each of the separated smaller string will save in array of double pointer.
each of them will sent by every thread to void* function that return the calculate with the given number from tha user.
The program will wait for eacht thread by the 'join' command and collect the result to array.
finally, we summerize all the result to one and print the answar of the polinom.

### ***polynomCalcSharedMem.c***:<br>
Polynomial calculation by **forking and shared memory**.<br>
The program calculate given polinom from 3 and down degree.
The calculate will be with fork(),and shared memory that will be created during the program.
The program will get input of sentence (that represent function), and number (represent the value that go into the polinom) from the user in infinity loop.
the program check how many 'x' there is in the sentence, and create shared memory by the size number of 'x'.
The sentence will be separated by the '+' command, and each of the separated smaller string will save in array of double pointer.
In addition,the program will fork() several times by the numer of 'x', and each of the 'children' will sent different part of the polinom to calculate function.
The value of the calculate function will save in the shared memory.
The process of the father will wait for each of his sons, and in the end collect each result from the shared memory.
In the end, destroy the shared memory.
finally, we summerize all the result from the shared memory to one result and print.
each of them will sent by the every thread to void* function that return the calculate with the given number from tha user that go in the string.


Finally ,to exit from the program enter command input "done".<br>

## ==Program DATABASE==<br>
1.str = array of char size 512 , input from the user.<br>
2.results = array of pointer, save the calculate from each thread.<br>
3.threads = array of type pthread, represnt the threads.<br>
4.p = array of type double char, separate the all string to different small string to calculate.<br>
5.x = array of type pid_t, represent each of the 'sons' agter forking.<br>
6.key = all the process that want to get shared memory has the same key path.<br>
7.shm_id = represnt the access to the shared memory.<br>
<br>
## ==Functions==<br>
1.res - function type void*, get input of void* and cast it to char*.calculate the given string with the input number from the user.

2.freeM - function gets pointer to array of dynamic memory and free it, each by each than free the main memory.

3.atof()-build in function, change string to number type double.

4.pthread_create() - build in function, create the threads.

5.pthread_join() - build in function, 'wait' for each thread that create.

6.res - function type of double, return the calculate of the given string with the input number from the user.

7.ftok() - build in function create the key path for all process that want access to shared memory.

8.shmget()-build in function, create the shared memory or given access to that shared memory with the key.

9.shmat()- build in function, access the pointer to the shared memory.

10.shmdt() - build in function,free the pointer that get the shared memory.

11.fork()-build in function, create father and son processes.

12.waitpid()-build in function, the father wait for his son to finish his process.

13.shmctl()-build in function, destroy the shared memory that create in the program.




## ==How to compile?==
* ***polynomCalcThreads.c***:

gcc polynomCalcThreads.c -o polynomCalcThreads -lpthread
    
run: ./polynomCalcThreads<br><br>
     
* ***polynomCalcSharedMem.c***:

gcc polynomCalcSharedMem.c -o polynomCalcSharedMem 

run: ./polynomCalcSharedMem

## ==Input==

sentence -polinom 3 and down degree + ", " + value to put in the polinom (512 chars).

## ==Output==

solution for the polinom with the given value.(type double)
Illegal string , or build in function failed - the program will print error.
"done" input - exit the program succesfully

