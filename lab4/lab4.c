//============================================================================================================================================== 
// Student Name: Ahmed Ahmed 
// Student#: 0632851 
// 
// COIS 3380 
// Lab 4 
// 
// Purpose:The main purpose of this lab work is to great a program that uses fork, execlp, wait and waitpid to create and 
// control several processes. 
// 
// The program should: create three processes(a parent process that creates two child processes which in turn create 
// in turn creates its own child (grandchild). 
// The parent process should wait until either of its child processes completes and prints PID fo the first process 
// 
// The first child should create another process and it should poll the hcild and sleep for 1 second (using WHOHANG system call) 
// The grandchild should sleep 3 and execute a "head -n 20 la4.c" command to list the content of the lab source code 
// 
// The second child porcess should in the file /home/COIS/lab2_document.txt and write out a local copy, called lab4_file_copy 
// in the current directory 
// 
// REFERENCES: some of the codes in this program is adapted from the sample_codes provided in /home/COIS/3380/sample_code 
//============================================================================================================================================== 

//Libraries used
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <string.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <sys/wait.h>

//main program 
int main() {
    //Creating the pid for the for the 1st and second generation(childrent and grandchildren)
    pid_t firstChild, secondChild, grandChild, pidgc1, pidgc2;
    int grandChildstatus; // the status returned by the process
    
    
    firstChild = fork(); //Create the first child fork process
   
   //Return ERROR if fork process is not successul
    if (firstChild< 0)
    {
        printf("ERROR! fork returned an error!\n");
        exit(0); // exit the process
    }
    // if fork is success, process the firstChild
    else if (firstChild == 0)
    {
        // print the firstChild statement
        printf("I am the first child\n");
        //Clone the secondChild process
        grandChild = fork();
        //Return ERROR if fork process is not successul
        if (grandChild < 0)
        {
            printf("ERROR! fork returned an error!\n");
            exit(0); //exit the process
        }
        //chekc if the Grandchild process is set
        else if (grandChild == 0)
        {
            printf("I am the grandchild\n"); // print the grandchild statement
            sleep(3); //Sleep for 3 seonds
            /*================Print contents fo the labsource code=======*/
            printf("=================Program Content is below===============");
            execlp("head", "head", "-n" "20", "lab4.c", NULL);
            exit(1);//exit the process
        }
        else
        {
            //Print the first child statement and the PID of the grandchild
            printf("I am the first child, grandchild has a PID of %d\n", grandChild);
            // while loop to wait for the grandChild to be done with its process
            while(1)
            {
                //using the WNOHANG option of the waitpid system call
                pidgc2 = waitpid(grandChild, NULL, WNOHANG);
        
                if (pidgc2 == -1) // check if the grandChild2 PID == -1
                {
                    printf("ERROR!\n"); //if then print an ERROR message
                    exit(0); //exit the process
                }
                else if (pidgc2 == 0) //Check if the PID of grandChild2 == 0
                {
                    printf("Come back again!\n"); // if then put on hold in the loop till it breaks
                    sleep(1); // sleep for 1 second
                }
                else if (pidgc2 == grandChild) // check if the PID of grandChild2 is == grandChild
                {
                    break; // if then break the waiting loop
                }
            }
            exit(0); //exit the process
        }
    }
    // proceed to the else conditon
    else
    {
        secondChild = fork(); //Create the second child process
        if (secondChild < 0)//Return ERROR if fork process is not successul
        {
            printf("ERROR! fork returned an error!\n"); // Print an Error message
            exit(0); // exit the process
        }
        else if (secondChild == 0) //check for the secondChild process
        {
            printf("I am the second child.\n"); //Print the statement for the second child
            //Declare the variable for the files we are going to use
            int file1, file2; // file1 (reading file), file2 (writing file)
            //Declare and initialize the bytes variable of the file the program is going reading
            int bytes = 0;
            //Declare and innitialize the char buffer to hold the file size
            char buffer[4096];
            // Declare the variable "Temp" to store the bytes
            size_t bytestor = 0;
            //Opens the source file in read only
            file1 = open("/home/COIS/3380/lab2_document.txt", O_RDONLY);
            do
            {
                //Reads the file in sections to the buffer
                bytestor = read(file1, buffer, (size_t)4096);
                if (bytestor <= 0)
                {
                    break;//break once file readind is complete
                }
                
                //Get the total sum of bytes
                bytes = bytes + bytestor;
            }
            
            //a while Loop that will allow file reading to be complete
            while (1);
            {
                // char for the file path
                char filpath[2056];
                
                //get the current working directory
                getcwd(filpath, sizeof(filpath));
                
                //pass the file name and path
                strcat(filpath, "/lab4_file_copy");
                //Open the second file or creates if it doesnt exist
                file2 = open(filpath, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH, 0644);
                
                //Write to the newly created or opened file in the current directory
                write(file2, buffer, strlen(buffer));
                close(file1); //clode file1 (reading file)
                close(file2); //close file2 (writing file)
                //Prints the bytes that were transfered
                printf("second child: %d bytes transferred.\n", bytes);
            }
	    exit(0); //exit the process
        }
        // print the parent's statement and the PID of the first child
        printf("I am the parent, the first child has a PID of %d\n", firstChild);
        // a while loop to wait for the second child to finish its process
        while(1)
        {
            //using the WNOHANG option of the waitpid system call
            pidgc1 = waitpid(-1, &grandChildstatus, WNOHANG);
            // chekc if the PID of the first child is == the secondChild
            if (pidgc1 == secondChild)
            {
                //Print once the second child process ends
                printf("second child has now finished with pid %d\n", secondChild);
                break;
            }
        }
        
        //print the grandcchild PID and the parent's statement
        printf("I am the parent, the second child has a PID of %d\n", secondChild);
        // a while loop for the first child to finish its process
        while (1)
        {
            pidgc1 = waitpid(-1, &grandChildstatus, WNOHANG);
            if (pidgc1 == firstChild)
            {
                //Prints when the child process has finished
                printf("First child has now finished with pid %d\n", firstChild);
                break;
            }
        }
    }
    exit(0); //exit the process
    
} //End of main program
