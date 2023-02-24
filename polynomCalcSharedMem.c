#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void freeM(char ** , int); //declaration function
double res(char * ); //declaration function
double num; //global number saving the variable input to the function
double result1; //global number saving the result of each from the separate function
int main() {
    char str[512];
    key_t key;
    int i, numOfSons, j, m;
    while (1) {
        double result = 0;
        result1 = 0;
        printf("Enter \"POLYNOM, VAL\":\n");
        fgets(str, 512, stdin); //input from user
        str[strlen(str) - 1] = '\0';
        if (strcmp(str, "done") == 0) //if input is done, finish
            exit(0);
        numOfSons = 0, j = 0, m = 0;
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
        for (i = 0; str[i] != '\0'; i++) {
            if (str[i] == 'x') //count the 'x' for forking
                numOfSons++;
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
        free(temp);
        char ** p = (char ** ) malloc(sizeof(char * ) * numOfSons);
        for (i = 0; str[i] != '\0'; i++) { //separate the string to functions
            if (str[i] == '+') {
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
        if (numOfSons == 0) { //there is no 'x'
            printf("%f\n", result);
            freeM(p, numOfSons);
            continue;
        }
        int shm_id;
        pid_t x[numOfSons]; //creating array of sons
        //create shared memory
        if ((key = ftok("/tmp", 'y')) == -1) { //create the key
            perror("ftok() failed");
            exit(1);
        }
        shm_id = shmget(key, sizeof(double) * numOfSons, IPC_CREAT | IPC_EXCL | 0600);//create the memory, if exists already print error, for read and write
        if (shm_id == -1) {
            perror("shmget() failed");
            exit(1);
        }
        for (i = 0; i < numOfSons; i++) { //each of the sons call res function
            x[i] = fork();
            if (x[i] == 0) {
                if ((key = ftok("/tmp", 'y')) == -1) {
                    perror("ftok() failed");
                    exit(1);
                }
                shm_id = shmget(key, 0, 0600); //access to the shared memory
                double * shm_ptr; //pointer to the shared memory
                shm_ptr = (double * ) shmat(shm_id, NULL, 0);
                if ( * shm_ptr == -1) {
                    perror("shmat failed");
                    exit(1);
                }
                *(shm_ptr + i) = res(p[i]); //saving the result of each son in the shared memory
                freeM(p,numOfSons);
                shmdt(shm_ptr); //free the pointer to the shared memory
                exit(0);
            }
            if (x[i] < 0) { //fork failed
                perror("fork fail");
                exit(1);
            }
        }
        for (i = 0; i < numOfSons; i++) {
            waitpid(x[i], NULL, 0); //father waiting to his children
            if ((key = ftok("/tmp", 'y')) == -1) { //creating the key
                perror("ftok() failed");
                exit(1);
            }
            shm_id = shmget(key, 0, SHM_RDONLY); //access the shared memory read only
            double * d = (double * ) shmat(shm_id, NULL, 0); //pointer to the shared memory
            result += * (d + i); //collecting the result from the shared memory
            shmdt(d);//free the pointer of the shared memory
        }
        if (shmctl(shm_id, IPC_RMID, NULL) == -1) { //destroy the shared memory
            perror("shmctl failed");
            exit(EXIT_FAILURE);
        }
        freeM(p, numOfSons);
        printf("%f\n", result);
    }
}
double res(char * temp) { //function calculate each part of the polinom
    int j;
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
            if (temp[i - 1] == 'x') {
                if (strchr(temp + i, '^')) {
                    check = strchr(temp + i, '^');
                    int * n = (int * )(check);
                    temp1 = (char * ) malloc(sizeof(char) * (strlen(temp) - i - (*n) + 1));
                    temp1[0] = '\0';
                    strncpy(temp1, temp + i, (strlen(temp) - i - * n));
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
    result1 += sum * sum2;
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
