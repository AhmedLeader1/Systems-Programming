//=================================================================================================================================================
// Student Name: Ahmed Ahmed
// Student#: 0632851
//
// COIS 3380
// Lab 6
//
// Purpose:The main purpose of this lab work is to great two programs that can talk to each other over a communication channel (socket in this case)
// 	   This program uses the technique of real life client-server. That is the client program sets up a socket connection with the server, once
// 	   the connection is successful, it sends a message to the server with a filename (through the commanline parameter).
// 	   The server then opens the file and copies its content to the client in form of "packets" of small chunks of size. Once the server sends
// 	   all the file contect, they both close their copy files. Lastly, the client disconnects from the communicaton channel, and the server waits
// 	   for another connection.
//
// Sever-side: this is the programming part of the Server-side, it receives a file name from the server, it then sends file content to the client.
//
//
//
// REFERENCES: I am creditting the following links for this program
//
// https://www.geeksforgeeks.org/c-program-for-file-transfer-using-udp/
// https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
// https://cboard.cprogramming.com/c-programming/151437-client-server-code-sends-contents-file.html
//
//
//=================================================================================================================================================

//Libraries used
#include <stdio.h>
#include <sys/sendfile.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>


//Define Directives (Macro Definitions)
#define BLOCKSIZE 1024
#define IP_ADDRESS "127.0.0.1" //localhost
#define PORT 52851 //my unique port
#define fName "lab_sourcefile_local_clone" //file name to copy the contents

//Main Program
int main(int argc, char *argv[]) {
  //Main vars
  // Socket vars
  int sSocket; //the server-side socket
  int currSocket; // the current soccet connection
  socklen_t socLength; //number of connections
  struct sockaddr_in sAddress; //server-side socket address
  struct sockaddr_in currAddress; //currently connected socket address

  //Declare The Variable used in the main program
  int filD; //File descriptor
  off_t offset; //file offset
  char fileSize[512]; //Size of the file
  struct stat fileInfo; //file information
  ssize_t totalBytes; //totalBytes sent to the client
  int sentBytes = 0; //the data size sent to the client
  int leftBytes; //bytes left to send to client

  //Create the server socket
  sSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (sSocket == -1) {
   printf("SERVER: ERROR: COULD NOT CREATE SERVER SOCKET");
  }
  //Setup the server address
  memset(&sAddress, 0, sizeof(sAddress)); //Set the memory for the server address
  sAddress.sin_family = AF_INET; //type of address
  inet_pton(AF_INET, IP_ADDRESS, &(sAddress.sin_addr)); // Setup the ipaddress
  sAddress.sin_port = htons(PORT); //Add in the port number
  //Bind the server socket to the server address
  if ((bind(sSocket, (struct sockaddr *)&sAddress, sizeof(struct sockaddr))) == -1) {
    printf("SERVER: ERROR! COULD NOT BIND");
  }
  //Listen on the server socket for up to 5 incoming connections
  if ((listen(sSocket, 5)) == -1) {
    printf("SERVER: ERROR TO GET RECIEVE CONNECTION");
  }
  //A while Loop to process the server process
  while(1) {
    // set the server-socket lenght
    socLength = sizeof(struct sockaddr_in);
    //set up a client connect
    currSocket = accept(sSocket, (struct sockaddr *)&currAddress, &socLength); //accepts client connection if there's one
    //check if there is ERROR in the conncetion process
    if (currSocket == -1) {
    //Print an ERROR message to indicate connection problem
     printf("SERVER: ERROR! COULD NOT ACCEPT SOCCKET");
    }
    //print the IP address connection recieved
    fprintf(stdout, "SERVER: Accepted connection from %s\n", inet_ntoa(currAddress.sin_addr));
    //set Up the filepath reciever
    char filePath[BUFSIZ];
    //recieves filepath length from the client
    int pathLength = recv(currSocket, filePath, BLOCKSIZE, 0);
    if(pathLength < 0){ // check if the path lenght
    printf("SERVER: ERROR! THERE IS NO FILEPATH GIVEN BY THE CLIENT");
    } else {
      fprintf(stdout, "SERVER: Retrieving the file content of: %s \n", filePath);
    }
    //Open the file requested by the client
    filD = open(filePath, O_RDONLY);
    if (filD == -1) { //check the file descriptor
      printf("SERVER: ERROR! COULD NOT OPEN THE FILE");
    }
    //Get the file size of the requested file
    if (fstat(filD, &fileInfo) < 0) {
      printf("SERVER: ERROR! COULD NOT GET FILE SIZE");
    }
    //print the file size
    fprintf(stdout, "SERVER: File Size: %d bytes\n", fileInfo.st_size);
    //copy the file size
    sprintf(fileSize, "%d", fileInfo.st_size);
    //Setup the total bytes of the file to be sent to the client
    totalBytes = send(currSocket, fileSize, sizeof(fileSize), 0);
    if (totalBytes < 0) {
      printf("SERVER: ERROR! COULD NOT SEND THE FILE TO THE CLIENT");
    }
    //set the file Offset at 0
    offset = 0;
    //remaining data to be sent
    leftBytes = fileInfo.st_size;
    //set data to be sent in BLOCKSIZES
    int remSize = BLOCKSIZE;
    // A while loop to process the sent and the remining data of the requested file
    while (leftBytes > 0) {
      //set the sent data of the requested file
      sentBytes = sendfile(currSocket, filD, &offset, remSize);
      //Subtract the sent data from the left data to be sent
      leftBytes -= sentBytes;
      //check if the leftBytes is less than the remSize (remaining data to send to the client)
      if(leftBytes < remSize) {
        //if then set the remSize to the leftBytes
        remSize = leftBytes;
      }
      // print a message to tell the client, the data I sent and the remaining data
      fprintf(stdout, "SERVER: I sent %d bytes, and remaining data is %d bytes\n", sentBytes, leftBytes);

    }
    // informa the user the file content retrieval has finished
    printf("SERVER: COMPLETED THE FILE SENDING PROCESS.\n");
  }
}
