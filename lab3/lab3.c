//=================================================================================================================================
//        Student Name: Ahmed Ahmed 
//        Student#: 0632851
//        
//        COIS 3380
//        Lab 3
//
//        Purpose:The main purpose of this lab work is to great a program that can read and process directires by passing 
//                arguments from the commandline, the program also prints the details of files in the directories i.e:
//		  Inode, Type,UID, GID, Size,FileName & Modification Date.
//	     		
//			
//
//	  REFERENCES: some of the codes in this program is adapted from the sample_codes provided in /home/COIS/3380/sample_code
//=================================================================================================================================

//Libraries used
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <getopt.h>
#include <time.h>


//Declaring and Initializing a boolean
  typedef enum
    {
        false = 0,
        true = !false

    } bool;


//Constructing a struct that storesa Argument values (b and s) in the program
struct Args {

  char* dirPath;          //user's choice of dir path
  bool b; int bValue;     //"int bValue" represents the oldest file while "bool b" Represents the b flag
  bool s;  int sValue;    //"int sValue" represents the earlies file while "bool s" Represents the s flag


};

//Declaring functions 
  void getArgs( int argc, char *argv[], struct Args *args );
  void displayStatInfo(struct stat fileStat, char* filename);
  char* getFiletype(struct stat fileStat);

// char* to get formatDate
  char* formatDate(char* str, time_t val);


// Main Program
// REFERENCE: /home/COIS/3380/sample_code/dir_example.c
  int main( int argc, char *argv[] ) 
 
  { 
   struct Args args;           //Create the struct to hold the arguments
   getArgs(argc, argv, &args); //Get the use's arguments from the commandline

   //Check the presence of directory paths
   if(args.dirPath == NULL || args.dirPath == ""){
     printf("Sorry! There is no direcoy path given"); // Print error if no directory path is given
     return;
  }

  //Check if there is a slash "/" at the end of the file path in the user's commanline, if not add one
  if( args.dirPath[strlen(args.dirPath)-1] != '/')
	{
		strcat(args.dirPath, "/");
	}

  // formatDate returns the Modification Date
  char* formatDate(char* str, time_t val){
  strftime(str, 40, "%d.%m.%Y %H:%M:%S", localtime(&val));
  return str; // Return the fomatted date as a string
}

  
  // Initializing before and since dates (b and s)
   int beforeDays = args.bValue;
   int sinceDays = args.sValue;
 
   DIR *dirP;           // directory pointer
   struct dirent *dirE; // structure of a directory entry
 
  // open the directory function
   dirP = opendir(args.dirPath);

  //Check if the dir is none, then stop if it's none
   if (dirP == NULL) {
   printf("\nSorry! There is no such directory: '%s'\n", args.dirPath);
   return;
  }

  //Printing the Header contents
    printf("\n%-10s%-7s%-9s%-10s%-13s bytes%-29s%-15s\n\n","Inode", "Type", "UID", "GID", "SIZE", "Filename", "Modification Date");

  //stat variable to hold file inforamtion 
    struct stat file_status; 
  
  // A while loop to read the directory 
    
  while((dirE=readdir(dirP))!=NULL) 
  { 
    if (strcmp(dirE->d_name, ".") == 0 || strcmp(dirE->d_name, "..") == 0) 
    continue; // skip and continue

    //Setup the path of the file
    char file[PATH_MAX+1];
    strcpy(file, args.dirPath); 
    strcat(file, dirE->d_name); 
    
    //Read the file info
    if(stat(file, &file_status) == 0)
    { 
        // Check (-b or -s) flags from the commandline and display relevant info of the file if it exists
        if(args.b || args.s) 
        {
          //Date of oldest files 
          if(beforeDays) 
          {
            time_t currentTime = time(0); //Current Time
            double diff = difftime(currentTime, file_status.st_mtime); //Difference between current and the file's modified time.
            
            int days = diff/24/3600; //Convert seconds to days
            if(days < beforeDays) {
              
            continue; //Skip current one.
            }
          }
          //display SINCE.
          if(sinceDays) 
          {
            time_t currentTime = time(0); // Current Time
            double diff = difftime(currentTime, file_status.st_mtime); //Difference between current time and the file's modified time
            int days = diff/24/3600; //Convert seconds to days
            if(days > sinceDays){
              continue; //Skip and move to the next file
            }
          }
        }
        
        //Display the info
        displayStatInfo(file_status, dirE->d_name);
    } 
    else {
      //Print an "ERROR" message if file path is not found
      printf("INVALID FILE PATH:%s\n", file);
    }
  }

  closedir(dirP); // close the directory once file reading finishes

  return 0; 
} //End of the Main program


// "getArgs" Gets arguments from the commandline & passes them to the arguments structure 

  void getArgs(int argc, char *argv[], struct Args *args) {
  int x;
  extern char *optarg;
 
//A while Loop that goes through the arguments with get option
  while (optind < argc) {
    if((x = getopt(argc, argv, "s:b:")) != -1) {
     
      //a switch statement to navigate the options (-s and -b)
      switch(x) {
        case 's':
            args->s = true;
            args->sValue = atoi(optarg);
  break;
        case 'b':
            args->b = true;
            args->bValue = atoi(optarg);
            break;
            
        // incase the option is INVALID print an "ERROR" message
        default:
          printf("Sorry that is not a valid option\n");
          break;
      }
    }
    else {
        args->dirPath = argv[optind]; // Get the directory path
        optind++;
    }
  }//End the while loop
}

//A "dispStatlayInfo" function that displays the info from the structure
  
  void displayStatInfo(struct stat fileStat, char* filename){
  char* filetype = getFiletype(fileStat);
 
// Print the information
  printf("%-10ld%-7s%-9ld%-10ld%-13lld%-29s%-15s", (long) fileStat.st_ino, filetype, (long) fileStat.st_uid, (long)fileStat.st_gid, (long long)fileStat.st_size, filename, ctime(&fileStat.st_mtime));
 
}

// char* "Filetype" that pasrses out and returns the Filetype info
  
  char* getFiletype(struct stat fileStat){
  char* filetype;
 
 // A switch statement that parses out the file type information
 // REFERENCE: /home/COIS/3380/sample_code/file_example.c
  switch (fileStat.st_mode & S_IFMT)
  {
    case S_IFREG:
      filetype = "REG"; // REG file type
    break;
    case S_IFDIR:
      filetype = "DIR"; // DIR file type
      break;
    case S_IFIFO:
      filetype = "FIFO";//FIFO file type
      break;
    default:
      filetype = "UNKN"; //UNKN file type 
    break;
  }

  return filetype; // returns the type of the file
}

