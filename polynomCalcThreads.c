#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void freeM(char ** , int); //declaration function
void * res(void * ); //declaration function
double num; //global number saving the variable input to the function
double * result1; //global pointer return the result of each thread
int main() {
    char str[512];
    int i, numOfThreads, j, m;
    while (1) {
        double result = 0;
        printf("Enter \"POLYNOM, VAL\":\n");
        if(fgets(str, 512, stdin) == NULL) //input from user
            exit(1);
        str[strlen(str) - 1] = '\0';
        if (strcmp(str, "done") == 0) //if input is done, finish
            exit(0);
        numOfThreads = 0, j = 0, m = 0;
        if (!(strchr(str, ','))) { //if there is no ','
            fprintf(stderr, "error with the string\n");
            continue;
        }
        if (strstr(str, "**") || strstr(str, "++") || strstr(str, "^^") ||
            strstr(str, "+*") || strstr(str, "*+") || strstr(str, "^*") || strstr(str, "*^") ||
            strstr(str, "^+") || strstr(str, "+^")) { //double command
            fprintf(stderr, "error with the string\n");
            continue;
        }
        for (i = 0; i < strlen(str); i++) {
            if (str[i] == 'x') //count the 'x' for making number of threads
                numOfThreads++;
            if (str[i] == ',')
                break;
        }
        if (strchr(str + i + 1, ',')) { //if there is another ','
            fprintf(stderr, "error with the string\n");
            continue;
        }
        if (str[i - 1] == ' ') { // backspace before the ','
            fprintf(stderr, "error with the string\n");
            continue;
        }
        if (str[i + 1] != ' ' || str[i + 2] == ' ') {
            fprintf(stderr, "error with the string\n");
            continue;
        }
        if (str[i + 1] == ' ' && str[i + 2] == '\0') {
            fprintf(stderr, "error with the string\n");
            continue;
        }
        char * temp = (char * ) malloc(sizeof(char) * (strlen(str) - i - 2) + 1);
        temp[0] = '\0';
        strncpy(temp, str + i + 2, (strlen(str) - i - 2));
        temp[strlen(str) - i - 2] = '\0';
        if (strchr(temp, 'x')) { //if input variable contain 'x'
            free(temp);
            fprintf(stderr, "error with the string\n");
            continue;
        }
        num = atof(temp); //num contains the variable that enter the function
        char ** p = (char ** ) malloc(sizeof(char * ) * numOfThreads);
        for (i = 0; str[i] != '\0'; i++) { //separate the string to functions
            if (str[i] == '+') { //separate the string to functions
                p[m] = (char * ) malloc(sizeof(char) * (i - j) + 1);
                p[m][0] = '\0';
                strncpy(p[m], str + j, i - j);
                p[m][i - j] = '\0';
                if (strchr(p[m], 'x'))
                    m++;
                else {
                    result += atof(p[m]);
                    free(p[m]);
                }
                j = i + 1;
            }
            if (str[i] == ',' && strchr(str + j, 'x')) {
                p[m] = (char * ) malloc(sizeof(char) * (i - j) + 1);
                p[m][0] = '\0';
                strncpy(p[m], str + j, i - j);
                p[m][i - j] = '\0';
                break;
            }
            if (str[i] == ',' && !(strchr(str + j, 'x'))) {
                char * c = (char * ) malloc(sizeof(char) * (i - j) + 1);
                c[0] = '\0';
                strncpy(c, str + j, i - j);
                c[i - j] = '\0';
                result += atof(c);
                free(c);
                break;
            }
        }
        double * results[numOfThreads]; //creating pointer for save the result of each thread
        int status;
        pthread_t threads[numOfThreads]; //creating array of threads
        for (i = 0; i < numOfThreads; i++) { //create each thread
            status = pthread_create( & threads[i], NULL, res, p[i]);
            if (status != 0) { //thread fail
                fputs("pthread create fail", stderr);
                exit(1);
            }
        }
        for (i = 0; i < numOfThreads; i++) { //waiting for all each thread and save his return value in results array
            status = pthread_join(threads[i], (void ** ) & results[i]);
            if (status != 0) {
                fputs("pthread create fail", stderr);
                exit(1);
            }
        }
        for (int count = 0; count < numOfThreads; count++) //sum of all the results
            if(results[count] != NULL)
                result += (*results[count]);

        freeM(p, numOfThreads);
        free(temp);
        printf("%f\n", result);
        for (i = 0; i < numOfThreads; i++) //free each memory that send from the res function
            free(results[i]);
    }
}
void * res(void * tmp) { //function calculate each part of the polinom
    int j;
    int len = (int) strlen((char * )(tmp)); //length of the string
    char * temp = (char * ) malloc(sizeof(char) * len + 1);
    temp[0] = '\0';
    strcpy(temp, (char * )(tmp)); //copy the original string to the new one temp
    temp[len] = '\0';
    char * temp1 = NULL, * temp2 = NULL, * check;
    for (int i = 0; temp[i] != '\0'; i++) {
        if (temp[i] == '*') {
            if (temp2 != NULL) {
                temp1 = (char * ) malloc(sizeof(char) * (strlen(temp) - i) + 1);
                temp1[0] = '\0';
                strncpy(temp1, temp + i + 1, strlen(temp) - i);
                temp1[strlen(temp) - i] = '\0';
                break;
            }
            if (i != 0 && temp[i - 1] == 'x') {
                if (strchr(temp + i, '^')) {
                    check = strchr(temp + i, '^');
                    int * n = (int * )(check);
                    temp1 = (char * ) malloc(sizeof(char) * (strlen(temp) - i - (*n)) + 1);
                    temp1[0] = '\0';
                    strncpy(temp1, temp + i, (strlen(temp) - i - (*n)));
                    temp1[strlen(temp) - i - (*n)] = '\0';
                    i = * n;
                    continue;
                } else {
                    temp1 = (char * ) malloc(sizeof(char) * (strlen(temp) - i) + 1);
                    temp1[0] = '\0';
                    strncpy(temp1, temp + i + 1, (strlen(temp) - i));
                    temp1[strlen(temp) - i] = '\0';
                    break;
                }
            }
            temp1 = (char * ) malloc(sizeof(char) * i + 1);
            temp1[0] = '\0';
            strncpy(temp1, temp, i);
            temp1[i] = '\0';
        }
        if (temp[i] == '^') {
            temp2 = (char * ) malloc(sizeof(char) + 1);
            temp2[0] = '\0';
            strncpy(temp2, temp + i + 1, 1);
            temp2[1] = '\0';
        }
    }
    double sum = 0, sum1 = 0, sum2 = 1;
    if (temp1 != NULL)
        sum += atof(temp1);

    if (temp1 == NULL)
        sum = 1;

    if (temp2 != NULL) {
        sum1 = atof(temp2);
        for (j = 0; j < sum1; j++)
            sum2 *= num;
    }
    if (temp2 == NULL)
        sum2 = num;

    result1 = (double * ) malloc(sizeof(double)); //creating memory to return the result value
    *result1 = 0;
    *result1 += sum * sum2;
    free(temp);
    free(temp1);
    free(temp2);
    return result1;
}
void freeM(char ** command, int w) { //function free allocate memory
    int i;
    for (i = 0; i < w; i++) {
        free(command[i]); //free each of the dynamic array
        command[i] = NULL;
    }
    free(command); //free the main memory
    command = NULL;
}
