//==================================================================================================================================================
// COIS3380 Systems Programing Lab 2
// Command Line Arguments and File Processing
//
// Name: lab2.c
// Written by: Ahmed Ahmed
// Student #: 0632851
//
// Purpose: The main objective of this lab is to introduce the file processing and accessing information from the command line.
//          In this lab, I am going to simulate a simple grocery store "database" program. The following piece of codes are going
//          to read a "shopping list" of items, then the code will look into the "database" and extract the appropriate record for the targeted item.
//          Lastly, the program will calculate the cost of all of the items purchased and produce a simple sales slip.
//
// Libraries Used: sys/types.h, time.h, unistd.h, stdio.h, stdlib.h, fcntl.h
//
//===================================================================================================================================================

//Declaring the libraries used in the program 
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

// main program
int main(int arg, char *argv [])
{
   	 // Creating a sturcute to store the information from the database
 	 struct sku
         {
                 char description[40];   // stores the description of each item in the tatabse 
                 char unitofmeasure[15]; // stores the measurement of eahc item  (litres, kilograms etc)
                 float price;            // sores the unit price of each item in the database
         } sku_record;

	
	 //Declaring a timestamp variable to print the local time and date
  	   time_t CurrTime  = time(NULL);


        //Declariing variables
        FILE* f1;             // to store the barcode and value of the file 
	int db,ctrl,catch;    // variables for database and control to seek the next item in the database
	int ProductCode;      // variable for the barcode of the item in the shopping list
	int Value; 	      // variabe for the value of the item (number of items bought)
	int count; 	      // variable to store the total number of items bought
	float Total; 	      // variable to sum up all the price of all the items 
		
	// Open the shopping list file passed from the command line (user interface) 
	f1 = fopen(argv[1], "r");

        // cacth error if the file does not exist
        if (f1 == NULL)
        {
           printf("\nSorry! there is no such file. Please double check the name of the file you passed.\n"); // tell the user there is no such a file
           // create more space of easy reading and clarity 
	   printf("\n");
	   return 0; // return nothing
	 }
	
        // Printing the border syling and the welcomin message for the sales slip
	 printf("\n==============================================================================================================================\n");
	 printf("\t\t\t\t\t\t WELCOME TO MICKEY'S FOOD MART! \t\t\t\t\n");

	 // create more space for easy reading and clarity
 	 printf("");
 	 printf("");

	 // print the current date and time (based on when the user requires the informaiton of the shopping list)
  	 printf("\t\t\t\t\t\t\t\t\tDate:%s", ctime(&CurrTime));
	
	 // print the Description, unit meaure, # of items, and the extended price of eadch item
	 printf("\n\t\t\t\t\t\t\tUnit of\t\tUnit\t\t# of\tExtended");
	 printf("\n\tDescription\t\t\t\t\tMeasure\t\tPrice\t\titems\tprice");
	 printf("\n\t---------------------\t\t\t\t-----------\t-----\t\t-----\t-------");

         //opens the grocery_items.db in read only mode
         db = open("/home/COIS/3380/lab2/grocery_items.db", O_RDONLY);
       	 read(db, &sku_record, sizeof(sku_record));

	
	 // if the file exists, loop through the databse to scan, and print the each item's informaiton from the databse
	 
	  while (fscanf(f1,"%d %d",&ProductCode, &Value)==2) //start of the while loop
	     {	
		
		// Catch the error if the calucalted offset for the item gets past the last byte in the file
		catch = lseek(db, 0, SEEK_END);
		if (catch < sizeof(sku_record)*ProductCode) // check if the calculated offset is past the last byte in the file
		{
			printf("\nSorry the barcode size exceeds the one in the database\n"); // print an error message
		
			continue; // continue with the loop 
		}

		//Seek the next item in th file from the database
		ctrl = lseek(db, sizeof(struct sku)*ProductCode, SEEK_SET);
		
		// read the infomaation of the next item/itmes and store its infomation in sku_record
		read (db, &sku_record, sizeof(sku_record));

        	// Printing the information of the item(s)
	        printf("\n\t%-25s %30s %12.2f %12d %10.2f\n", sku_record.description, sku_record.unitofmeasure, sku_record.price, Value, sku_record.price*Value); 

 		// Get total count
		count = count+Value;

		// Get total price
		Total = Total + (sku_record.price*Value);
	     }// end of the while loop
     
	  //Print the total price of all the items in the sales slip 
	  printf("\n\t\t\t\t\t\t\t\t\t\t\tTotal:%7.2f\n", Total);
	
	  //Print the total number of item bought and "thank you message"
	  printf("\n\t\t\tThank you for shopping %d items from our affordabe store!\n\t\t\t\tHope you will come back soon!\n", count);     
	
	  // Printing for the styling in the sales slip
	  printf("\n============================================================================================================================\n");

	close(db); // close the databse file

}




