//============================================================================================================================================== 
// Student Name: Ahmed Ahmed 
// Student#: 0632851 
// 
// COIS 3380 
// Lab 5 
// 
// Purpose: The main purpose of this lab work is to great a program that demonstrates my knowledge on signal processing in Unix. 
// I am required to write a C program that contains three signal handler functions. 
// 
// 1st: The 1st is to handle the SIGINT (CTRL-C) signal, it should increment a counter that ounputs the nymber of times the user 
//            presss CTRL-C 
// This handler should also contain a MAX (5 times) that is invoked if the user exceeds and the program should display a message to 
// indicate that the MAX has been exceeded. 
// 
// 2nd: The 2nd handler deals with the SIGQUIT signal (CTRL-\). if the user sends this signal, the signal handler 
// should create a new process(fork) 
// 
// 
// 3rd: The final handler catches the SIGUSR1 signal, sent by the child process, it then prints a message that the program is over 
// and then exit 
// 
// 
// 
// 
// REFERENCES: some of the codes in this program is adapted from the sample_codes provided in /home/COIS/3380/sample_code 
//============================================================================================================================================== 

//Libraries
#include <unistd.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/wait.h> 

//Defining & Inializing MAX with value 5 (maximum number to press CTRL-C by the user)
#define MAX 5 

//Prototypes 
void sigintHandler (int sigint); // handles SIGINT (CTRL-C) signal 
void sigquitHandler (); // handles SIGQUIT (CTRL-\) signal  
void sigusr1Handler (); // handles SIGUSR1 signal (sent by the child process) 

//Varialbes 
int sigintCounter = 0; //Declaring and Initializing a counter for the number of time of CTRL-C the user presses 
int status; //the status for the wait call 
pid_t childPid; //Child Process Id 

//=============================================Main Program===========================================================//
int main() 
{
        //signal handlers
        signal (SIGINT, sigintHandler); //SIGINT (CTRL-C) signl handler
        signal (SIGQUIT, sigquitHandler); //SIGQUIT (CTRL-\) signal handler
        signal (SIGUSR1, sigusr1Handler); //SIGUSR1 signal (sent by the child process)
        //An endless for loop waiting for signals
        for (;;)
        {
                printf("Wait for another signal\n");
                pause(); //pauses and waits for a signal
        }
}

//============================================SIGINT signal handler===================================================// 
/*
 The is to handle the SIGINT (CTRL-C) signal, it increment a counter that ounputs the nUmber of times the user presses CTRL-C
 The MAX (5 times) that is invoked if the user presses CTRL-C more than the MAX value (5 times), it the prints a message that
 indicate the MAX has been exceeded. 
*/ 
void sigintHandler (int sigint) //SIGINT handler 
{
        //increments the sigintCounter
        sigintCounter++;
        // check if the sigintCounter is less than the MAX value to press CTRL-C
        if (sigintCounter < MAX)
            {
                //if not print this message and indicate the number of times CTRL-C is pressed
                printf("\tThis is the %d time you pressed ctrl-c\n", sigintCounter);
                signal (SIGINT, sigintHandler); //read the signal handler
        }
        // check if the sigintCounter reached the MAX value to press CTRL-C
        else if (sigintCounter == MAX)
        {
                // if then print this message to indicate MAX value to press CTRL-C is reached
                printf("\tYOU REACHED THE MAX VALUE TO PRESS CTRL-C\n");
                signal (SIGINT, sigintHandler); //read the signal handler
        }
        // proceed to the else condtion if none of the above is true
        else
        {
                exit(0); // exit and terminate the program
        }
}

//============================================SIGQUIT signal handler===================================================// 
/*
 The SIGQUIT signal handler deals with the SIGQUIT signal (CTRL-\). if the user sends this signal, the signal handler
             should create a new process(fork) */ 
//SIGQUIT (CTRL-\)handler 
void sigquitHandler () {
       //Create a child process
        childPid = fork();
        
        //Check if the child process has been forked
        if (childPid == 0)
        {
                //if the above condtion is satisfied, print this...
                printf("\tI am the child and I am sending a signal\n");
                //KILL () and send the signal to SIGUSR1 signal handler
                kill(getppid(), SIGUSR1);
                exit(0); //Exit the child process
        }
        else
        { //wait for the SIGUSER1 to be sent by the child process
                wait(&status);
        }
}

//============================================SIGUSER1 signal handler===================================================// 
/*
 This signal handler catches the SIGUSR1 signal, sent by the child process, it then prints a message that the program is over
              and then exits (terminates the main program) 
*/ 
//SIGUSER1 received from the child process 
void sigusr1Handler () {
        //Prints that the child has sent the signal and exits the parent process
        printf("\tChild sent a signal so I guess you are bored, have a great day!\n");
        exit(0); // EXIT and end the program
}
