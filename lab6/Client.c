//=================================================================================================================================================
// Student Name: Ahmed Ahmed
// Student#: 0632851
//
// COIS 3380
// Lab 6
//
// Purpose:The main purpose of this lab work is to great two programs that can talk to each other over a communication channel (socket in this case)
//         This program uses the technique of real life client-server. That is the client program sets up a socket connection with the server, once
//         the connection is successful, it sends a message to the server with a filename (through the commanline parameter).
//         The server then opens the file and copies its content to the client in form of "packets" of small chunks of size. Once the server sends
//         all the file contect, they both close their copy files. Lastly, the client disconnects from the communicaton channel, and the server waits
//         for another connection.
//
// Client-side: this is the programming part of the Client-side, it is creates a socket connection, send a filename, it lastly ends the connection
//              once it recieves the file content from the server.
//
//
// REFERENCES:  I am creditting the following links for this program
//
// https://www.geeksforgeeks.org/c-program-for-file-transfer-using-udp/
// https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
// https://cboard.cprogramming.com/c-programming/151437-client-server-code-sends-contents-file.html
//=================================================================================================================================================

//Libraries used
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

//Define Directives (Macro Definitions)
#define IP_ADDRESS "127.0.0.1" //localhost
#define PORT 52851 //my unique port
#define fName "lab_sourcefile_local_clone" //file name to copy the contents

//A boolean expression for the commandline arguments
typedef enum { false = 0, true = !false } bool;

//A boolean structure for the commandline Arguments
struct Arguments {
  bool x;
  char* filepath;
};
//getArgs function to collect arguments given in the commanline
void getArgs( int argc, char *argv[], struct Arguments *args );

//Main Program
int main(int argc, char
*argv[]) {
  //Declare Variable used in the main
  int cSocket; //client side socket
  struct sockaddr_in remote_addr; //struct type of sockaddr_in
  char buffer[BUFSIZ]; //buffer containing BUFSIZE bytes
  ssize_t totalBytes; //totalBytes recieved from the server
  int leftBytes = 0; //bytes left to recieve from the server
  char fileSize[BUFSIZ]; // file size recieved from the server
  struct Arguments args; //A struct to hold commandline arguments
  getArgs(argc, argv, &args); //getArgs function to get args from the commandline

  //Check if the filepath is given
  if(args.x == false){
    //if not then print an error message
    printf("CLIENT: ERROR! NO VALID ARGUMENT OPTION!\n");
    return -1; //returns -1
  }
  //set the memory of the structure to be 0
  memset(&remote_addr, 0, sizeof(remote_addr));
  //set IIP_ADDRESS
  remote_addr.sin_family = AF_INET; //type of address
  inet_pton(AF_INET, IP_ADDRESS, &(remote_addr.sin_addr)); //setup the adrress
  remote_addr.sin_port = htons(PORT); //set the port
  //set client socket
  cSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (cSocket == -1) {
    printf("CLIENT: ERROR! COULD NOT CREATE CLIENT SOCKET\n");
  }
  //create client-server connection
  if (connect(cSocket, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1) {
    printf("CLIENT: ERROR! COULD NOT CONNECT TO THE SERVER\n");
  }
  //print out a message to the server indicatin the filepath and string size
  fprintf(stdout, "CLIENT: filepath is: %s\n", args.filepath);
  if (send(cSocket, args.filepath, strlen(args.filepath), 0) < 0) {
    printf("CLIENT: ERROR! COULD NOT SEND THE FILEPATH\n");
  }
  //Get file size from the server
  recv(cSocket, fileSize, BUFSIZ, 0);
  printf("CLIENT: Receiving a file of size %s bytes\n", fileSize);
  //Openthe file to copy the contents of from the server
  FILE *copyFile = fopen(fName, "ab+");//create if there's none
  if (copyFile == NULL) {
    printf("CLIENT: ERROR! CANNOT OPEN THE FILE!\n");
  }
  leftBytes = atoi(fileSize); //Set total file size
  //A while loop to print the data exchange between the Client and the server
  while ( (leftBytes > 0) && ((totalBytes = recv(cSocket, buffer, sizeof(buffer), 0)) > 0))
  {
    fwrite(buffer, sizeof(char), totalBytes, copyFile);
    leftBytes -= totalBytes;
    if(leftBytes > 0){
      fprintf(stdout, "CLEINT: Received %d bytes and waiting for: %d bytes\n", totalBytes, leftBytes);
    } else {
      fprintf(stdout, "CLIENT: Recieved A total of %d bytes.\n", totalBytes);
      break;
    }
  }
  fprintf(stdout, "CLIENT: File recieved successfully.\n" );
  fprintf(stdout, "CLIENT: Hunging up,Bye.\n" );
  fclose(copyFile); //Close the copy file
  close(cSocket); //close the scoket
  return 0; //return nothing

}//End of the main program
//getArgs (Gets argumetns from the commandline)
void getArgs(int argc, char *argv[], struct Arguments *args) {
  int k; // variable to loop through the Arguments
  extern char *optarg;
    //A while loop to go through the argments from the commandline
  while (optind < argc)
  {
    if((k = getopt(argc, argv, "x:")) != -1)
    {
      //if else statement for the Arguments option
     if (k='x')
        {
          args->x = true;
          args->filepath = optarg;
         }
        else
        {
          printf("INVALID OPTION!: %c. \n", optopt);
        }
    }
  }
}
