//**********************************************************************************************
// Code FileName: Shaw-Assn2-Prog.cpp
// Description: Program takes a file of integers and sorts the unique numbers into 2 doubly 
//      linked lists based upon whether or not the number is even or odd. Once the file has
//      processed all the numbers, it will display a message on the file name that was used, 
//      how many integers are in each list, and the numbers that are in each list. The program
//      will than open the second file of integers and determine if each integer currently exists
//      in the proper list. The program will print out whether or not the integer is found in the
//      proper list or not. 
// Class/Term: CS372 Spring 8 Week 1
// Designer: Kevin Shaw
// Functions:
//      fileValidation - Function validates both files are valid and open properly. 
//      getUserInput - Function gets user input for the filenames. 
//      doesFileOpen - Function determines if a file opens or not. 
//      fileExtCheck - Function validates the files are using a .txt file extension
//      initializeList - Function initializes an list to NULL values. 
//      listLength - Function determines the length of the list. 
//      emptyList - Function determines if the list is empty or not. 
//      numInList - Function determines if the number is in the list. 
//      orderedListInsertion - Function inserts a number into a list in ascending order. 
//      positionInList - Function determines where a number is in a current list.
//      displayList -  Function displays all numbers in the list to the screen.
//      destroyList - Function frees up all memory in a doubly linked list.
//      isEven - Function determines if a number is even or odd
//      readIntFile - Function reads in the first file and processes the data. 
//      readSearchFile - Function reads in the seconds file and processes the data. 
//**********************************************************************************************
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

// constants
const string PROGRAM_TITLE = "Doubly Linked List Program";
const int NUMBER_OF_ARGUMENTS = 3;
const int MAX_INT = 999;
const int MIN_INT = 1;
const int END_LINE_AFTER_TEN = 10;
const int HALF = 2;

// Structures
struct intNode {
    int value;
    intNode* next;
    intNode* back;
};
struct topList {
    int count;
    intNode* top;
};

// Prototypes 
void fileValidation (string& storeFile, string& searchFile);
string getUserInput (int fileNum);
bool doesFileOpen(string fileName);
bool fileExtCheck (string fileName);
void initializeList (topList& list);
int listLength (topList& list);
bool emptyList (topList& list);
intNode* numInList (topList& list, int searchItem);
bool orderedListInsertion (topList& list, int insertItem);
int positionInList(topList& list, int number);
void displayList(topList& list);
void destroyList (topList& list);
bool isEven (int number);
void readIntFile (topList& oddFile, topList& evenFile, string fileName);
void readSearchFile (topList& oddList, topList& evenList, string fileName);
/*********************************************************************************
|   Function: main
|   Description: Function main determines if the proper amount of command arguments 
|       have been entered. main then calls the fileValidation to validate both files
|       are valid before moving on. main then calls initializeList to initalize both
|       both lists. main then calls readIntFile to process the first file. next, main
|       then calls readSearchFile to process the second file. Lastly, main calls 
|       destroyList to free up the memory in both lists. 
|   Output: Returns 0 if successful.
|   Calls to: fileValidation, initializeList, readIntFile, readSearchFile,
|           destroyList
*********************************************************************************/
int main(int argc, char* argv[]) 
{
    topList oddList, evenList;
    string storeFile = "", searchFile = "";
    char* file1 = NULL;
    char* file2 = NULL;
    
    
    cout << PROGRAM_TITLE << endl << endl;
    
    // If... argument 1 entered...
    if (argc == NUMBER_OF_ARGUMENTS)
    {
        file1 = argv[1];
        storeFile = static_cast<string>(file1);
        file2 = argv[2];
        searchFile = static_cast<string>(file2);
    }
    
   // call file validation function
   fileValidation(storeFile, searchFile);
   
   
   initializeList(oddList);
   initializeList(evenList);
   
   readIntFile(oddList, evenList, storeFile);
   readSearchFile (oddList, evenList, searchFile);
   
   cout << endl;
   // Lastly... destroys both lists to free up memory.
   destroyList(evenList);
   destroyList(oddList);
   
    return 0;
}

/*********************************************************************************
 *  Function: fileValidation
 *  Description: Function fileValidation takes 2 strings as return parameters.
 *      If either or both of the filenames are blank, the program will issue a 
 *      message stating so and the program will ask for user input on both files.
 *      If neither are blank, the program will check if the file extension for 
 *      file 1 is valid (.txt). If the file extension is valid, it will 
 *      will attempt to open both files (not read, just open). If the file opens,
 *      it will mark file 1 as a valid file. If the file extension is not valid 
 *      or the file doesn't open, the program will prompt for user input and 
 *      validate again. Once file 1 is marked valid, the second file is put through
 *      the same validation. Once both files are marked valid, the function returns 
 *      to main with both filenames returned as return parameters. 
 *  Input: 
 *      Parameters:
 *          storeFile - string file name of the store file (file 1).
 *          searchFile - string file name of the store file (file 2).
 *  Output: 
 *          Both input parameters are also return parameters
 *  Calls to: getUserInput, fileExtCheck, doesFileOpen
*********************************************************************************/
void fileValidation (string& storeFile, string& searchFile)
{   
    bool fileOpens = false;     // Does the file open?
    bool fileExtValid = false;  // Is the file extension .txt?
    bool file1Valid = false;    // Is file 1 valid?
    bool file2Valid = false;    // Is file 2 valid?

    // If either file is blank, prompt for user input on both. 
    if ((storeFile == "") || (searchFile == ""))
    {
        cout << "One or Both Command Line arguments were not entered." << endl 
             << "Since 2 command line arguments were not entered," << endl
             << "you will need to enter both files manually." << endl;

        storeFile = getUserInput(1);
        searchFile = getUserInput(2);
    }
    // Inner do... while file1 isn't valid.
    do
    {
        fileExtValid = fileExtCheck(storeFile);
        
        if (!fileExtValid)
            cout << "File Extension for file 1 is not a valid type (.txt)" << endl;
        else
        {
            fileOpens = doesFileOpen(storeFile);
        }

        if (!fileOpens)
        {
            cout << "File 1 did not open properly. You will need to manually enter the filename." << endl;
            storeFile = getUserInput(1);
            fileExtValid = false;
            
        }
            
        
        if (fileExtValid && fileOpens)
            file1Valid = true;

    } while (!file1Valid); // end file1 validation do..while

    // Reset 2 flags for file 2's processing
    fileOpens = false;
    fileExtValid = false;

    // do... while file2 isn't valid
    do
    {
        fileExtValid = fileExtCheck(storeFile);

        if (!fileExtValid)
            cout << "File Extension for file 2 is not a valid type (.txt)" << endl;
        else
        {
            fileOpens = doesFileOpen(searchFile);
        }

        if (!fileOpens)
        {
            cout << "File 2 did not open properly. You will need to manually enter the filename." << endl;
            searchFile = getUserInput(2);
            fileExtValid = false;
            
        }
            
            
        if (fileExtValid && fileOpens)
            file2Valid = true;
        
    } while (!file2Valid); // end file2 validation do...while

}
/*********************************************************************************
 *  Function: getUserInput
 *  Description: Function getUserInput prompts the user to enter a file name for the 
 *                  proper file #. The file name is then checked to make sure the file
 *                  extension is valid (.txt). If the file extension is valid, the 
 *                  function then returns the string. If the file extension is not 
 *                  valid, the function loops again until the user has entered a
 *                  valid file name. 
 *  Input:
 *      Parameter: fileNum - value for while file number we are getting input for.
 *  Output: 
 *      Returns: fileName - validated string (valid file extension) for the name
 *                      of the file.
 *  Calls to: fileExtCheck
*********************************************************************************/
string getUserInput (int fileNum)
{
    string fileName;
    bool fileExtValid;

    do
    {
        cout << "Enter the path to file #" << fileNum << "." << endl;
        cin >> fileName;
        cout << endl;

        // If.. calls the fileExtCheck function to determine if the file extension is valid.
        if (fileExtCheck(fileName))
        {
            fileExtValid = true;
        }
        else
        {
            fileExtValid = false;
            cout << "The file extension must be .txt, please try again." << endl;
        }
    
    } while (!fileExtValid); // while input not valid...

    return fileName;
}

/*********************************************************************************
 *  Function: fileExtCheck
 *  Description: Function fileExtCheck gets a string as an input parameter and 
 *                  checks the string to determine if the string contains the 
 *                  substring ".txt". Returns True if the file extension is valid,
 *                  false if it does not. 
 *  Input:
 *      Parameter: fileName - string for the name of the file.
 *  Output: 
 *      Returns: validFile - bool value for if the file extension is valid.
*********************************************************************************/
bool fileExtCheck (string fileName)
{
    bool validFile = false;
    
    // If... the string contains ".txt" the file type is valid.
    if (fileName.find(".txt") != string::npos)
    {
        validFile = true;
    }
    
    return validFile;
}

/*********************************************************************************
 *  Function: doesFileOpen
 *  Description: Function doesFileOpen gets a string file name as an input 
 *                  parameter and then tests to see if the file opens or not. 
 *                  Lastly, the function returns true or false depending if the 
 *                  file opened or not.  
 *  Input:
 *      Parameter: fileName - string containing the file name to be tested.
 *  Output: 
 *      Returns: fileOpened - bool value for if the file opened or not.
*********************************************************************************/
bool doesFileOpen(string fileName)
{
    bool fileOpened; // Does the file open?
    ifstream testOpen; // ifstream variable for the file. 
    
    // Attempts to open the file
    testOpen.open(fileName);
    
    // If file opens...
    if (testOpen)
        fileOpened = true;
    else // Else... file doesn't open.
        fileOpened = false;
    
    // Closes the file before returning
    testOpen.close();
    
    return fileOpened;
}

/*********************************************************************************
 *  Function: initializeList
 *  Description: Function initializeList takes a list as a parameter and initializes
 *                  the values to an empty list.
 *  Input:
 *      Parameter: list - either odd or even list to be initalized.
 *  Output: 
 *      Parameter: Input parameter list is also a return parameter. 
*********************************************************************************/
void initializeList (topList& list)
{
    list.count = 0;
    list.top = NULL;
    
}

/*********************************************************************************
 *  Function: listLength
 *  Description: Function listLength takes a list as a parameter and returns an
 *                  integer value for how many nodes the list contains.
 *  Input:
 *      Parameter: list - list of nodes to be counted. 
 *  Output: 
 *      Returns: list.count - integer value for how many nodes the list contains.
*********************************************************************************/
int listLength (topList& list)
{
    return list.count;
    
}

/*********************************************************************************
 *  Function: emptyList
 *  Description: Function emptyList takes a list as a return parameter and returns
 *                  whether or not the list is empty by checking if the top of the
 *                  list is pointing to NULL or not.  
 *  Input:
 *      Parameter: list - list of nodes to be tested.
 *  Output: 
 *      Returns: bool value for if the list.top is equal to NULL or not.
*********************************************************************************/
bool emptyList (topList& list)
{   
    return (list.top == NULL);
}

/*********************************************************************************
 *  Function: numInList
 *  Description: Function numInList takes a list and integer values as parameters.
 *                  Function then creates a new intNode and sets the value to NULL.
 *                  Function then creates a new intNode and sets the value to list.top
 *                  Function then traverses the list to see if the search value matches 
 *                  any value in the list. Lastly, numInList returns a pointer to the node
 *                  that matches the search value or NULL if the value is not in the list. 
 *  Input:
 *      Parameters: 
 *          list - list of intNodes to be traversed.
 *          searchItem - integer value to see if it matches any nodes. 
 *  Output:
 *      Returns: found - Pointer value of the node that matches the search item 
 *                  or NULL if not found.
*********************************************************************************/
intNode* numInList (topList& list, int searchItem)
{
    
    intNode* found = NULL;
   
    intNode* current = list.top;
    
    while ((current != NULL) && (found == NULL))
    {
        
        if (current->value == searchItem)
            found = current;
        else
            current = current->next;
    }
    
    return found;
}

/*********************************************************************************
 *  Function: orderedListInsertion
 *  Description: Function orderedListInsertion takes a list and integer as input
 *              parameters. The function then creates 3 pointers for the current
 *              node, trailing node, and new node to be inserted. Then initializes a 
 *              new node and assigns insertItem (int) value to it. The function then
 *              checks to see if the list is empty, if empty, the function inserts
 *              the node into the list to create the first node. If the list is not 
 *              empty, the function traverses the list to find the position to insert
 *              the new node. There are 3 areas where an item can be inserted, 
 *              before the first node, in between to nodes in the list, or at the 
 *              end of the list. Once the item has been inserted, the function returns
 *              a bool value for if the item was inserted or not. 
 *  Input:
 *      Parameters: 
 *          list - list to be used for inserting the value
 *          insertItem - integer value for the number to insert
 *  Output: 
 *      Parameter: list - return parameter for the list including the newly inserted
 *                  node.
 *      Returns: success - bool value for if the item was inserted or not. 
 *  Calls to: emptyList
*********************************************************************************/
bool orderedListInsertion (topList& list, int insertItem)
{   
    
    bool found, success;
    intNode* current;
    intNode* trailCurrent;
    intNode* newNode;
    
    
    newNode = new intNode; // Creates a new intNode (not a pointer to a node)
    newNode->value = insertItem; // Assigns insertItem to newNode-> value
    newNode->next = NULL;   // initializes ->next pointer to NULL
    newNode->back = NULL;   // initializes ->back pointer to NULL
    
    
    if (emptyList(list))
    {
        
        success = true;
        list.top = newNode; // Points the list to the new node to begin the list
        list.count = 1; // List Count becomes 1
    }
    else
    {
       
        success = false;
        found = false;
        current = list.top;
        
        while (current != NULL && !found) // Search the list
        {
            if (current->value >= insertItem)
                    found = true;
            else
            {
                trailCurrent = current;
                current = current->next;
            }
        }        
        
        if (current == list.top) // insert newNode before first
        {
            list.top->back = newNode;
            newNode->next = list.top;
            list.top = newNode;
            
            success = true;
        }
        else // Inserting after first node...
        {
            // insert newNode between trailCurrent and current and not back of list
            if(current != NULL)
            {
                trailCurrent->next = newNode;
                newNode->back = trailCurrent;
                newNode->next = current;
                current->back = newNode;
                success = true;
                
            }
            else // insert at back of list.
            {
                
                trailCurrent->next = newNode;
                newNode->back = trailCurrent;
                success = true;
                
            } // end inner inner else
            
            
        } // end inner else
         
        list.count++;
    } // end outer else
    
    return success;
}

/*********************************************************************************
 *  Function: positionInList
 *  Description: Function positionInList takes a list and integer as input parameters.
 *              Function then initializes a count variable to track how far down the 
 *              list the node is. Function then calls the numInList function to determine 
 *              if and node is in the list and pointer to the node itself. The function 
 *              then analyzes the pointer value returned and determines whether or not the 
 *              value exists in the list (NUll or Not NULL). If not found in list, the
 *              function returns 0. If found, the function uses the backwards pointing
 *              pointers to determine how many nodes down in the list the value is and
 *              returns that value.  
 *  Input:
 *      Parameters: 
 *          list - list to be used for analysis and comparison.
 *          number - integer value to be searching the list for.
 *  Output: 
 *      Parameter: list is a return parameter. 
 *      Returns: position - integer value for the position of the found item in the
 *                  list.
 *  Calls to: numInList
*********************************************************************************/
int positionInList(topList& list, int number)
{   
    int position = 0;
    intNode* currentLocation;
   
    currentLocation = numInList(list, number);    
    
    // If not found in list
    if (currentLocation == NULL)
        position = 0;
    else // Else... found in the list.
    {
        /* While currentLocation is not NULL, traverse the list to count how 
         * many nodes down the node is. */
        while(currentLocation != NULL) 
        {
            currentLocation = currentLocation->back;
            
            position++;
        }
        
       
    }
    
    return position;
}

/*********************************************************************************
 *  Function: displayList
 *  Description: Function displayList takes a list as an input parameter. The 
 *              function traverses the list from front to back and reads out the
 *              value contained in each node. If there are 10 values displayed on 
 *              a line the function adds a newline to have the output jump down to
 *              the next line. 
 *  Input:
 *      Parameter: list - list to be traversed
 *  Output: 
 *      Parameter: list is also a return parameter.
*********************************************************************************/
void displayList(topList& list)
{
    int printCount = 1;
    
    intNode* current = list.top; //pointer to the list
    cout << "   ";
    while (current != NULL)
    {
        cout << setw(3) << current->value << "  ";
        current = current->next;
        
        if ( (printCount % END_LINE_AFTER_TEN) == 0)
            cout << endl << "   ";
        
        printCount ++;
    }
}

/*********************************************************************************
 *  Function: destroyList
 *  Description: Function destroyList takes a list as an input parameter and 
 *               traverses the list freeing memory for each node until the list 
 *               is empty.
 *  Input:
 *      Parameter: list - list to be traversed and destroyed.
*********************************************************************************/
void destroyList (topList& list)
{
    intNode* current = list.top; // pointer to the top of the list.
    intNode* nextNode;
    
    while (current != NULL)
    {
        nextNode = current->next;
        delete current;
        current = nextNode;
    }
    
}

/*********************************************************************************
 *  Function: isEven
 *  Description: Function isEven takes a integer value as an input parameter. 
 *              The function than divides the integer value by 2 to find out if 
 *              there is a remainder. If remainder equals zero the integer is even
 *              else the integer is odd. Lastly, the function returns a bool value 
 *              for if the function is even (true) or odd (false). 
 *  Input:
 *      Parameter: number - current number to analyze.
 *  Output: 
 *      Returns: isEven - bool value for if the int is even or odd.
*********************************************************************************/
bool isEven (int number)
{
    bool isEven = false;
    
    // If the number divided by 2 leaves no remainder the number is even.
    if (number % HALF == 0) 
        isEven = true;
    
    return isEven;
}

/*********************************************************************************
 *  Function: readIntFile
 *  Description: Function readIntFile takes 2 doubly linked list and a string file 
 *              name as input parameters. The function opens the file and reads one
 *              integer at a time. Each time an integers is read in, the function
 *              calls isEven to determine which list the integer belongs to. The 
 *              function then calls numInList to determine if the integer is already
 *              in the corresponding list. If the integer is already in the list,
 *              the function will display a message stating so. If the function does 
 *              not exist in the list, the function will insert the integer into the
 *              list by calling the orderedListInsertion function. Once all integers
 *              have been read from the file, the function closes the file and then
 *              displays the file name of the file that was used. Next, the function
 *              displays how many nodes were inserted into the even list  by calling
 *              the displayList function, and then proceeds to display all integers 
 *              in the list to the screen by calling the display list function. Once 
 *              completed, the function will then do the same for the odd list. 
 *  Input:
 *      Parameters:
 *          oddList - List to be used for odd integers.
 *          evenList - List to be used for even integers.
 *          fileName - string for the file name of the file to open.
 *      File: storeFileIn - .txt file containing integers to add to the lists.
 *  Output:
 *      Parameters: oddList & evenList are both return parameters and will return
 *          a full list of nodes containing the proper integers that belong in the list.  
 *  Calls to: isEven, numInList, orderedListInsertion, listLength, displayList
*********************************************************************************/
void readIntFile (topList& oddList, topList& evenList, string fileName)
{
    ifstream storeFileIn;
    int currentNum;
    bool evenNumber;
    intNode* foundInList;
    bool insertSuccess = false;
    
    
    storeFileIn.open(fileName);
    
    cout << "Reading Input..." << endl;
    // priming read
    storeFileIn >> currentNum;
    
    if (storeFileIn)
    {
        
        // While there is data to be read in... Loop
        while (storeFileIn || !storeFileIn.eof())
        {
           
            // Calls isEven to determine if the number is even or odd
            evenNumber = isEven(currentNum); 
            
            
            /* If current number is between 1 and 999... continue analyzing number.
               else... Ignore number...*/
            if ((currentNum >= MIN_INT) && (currentNum <= MAX_INT))
            {
                if (evenNumber) // Number is even
                {
                   
                    // Is number in the list?
                    foundInList = numInList (evenList, currentNum);
                   
                    // If Number is found, the number will be ignored.
                    if (foundInList != NULL) 
                    {
                        cout << setw(3) << currentNum << " is a duplicate and will be ignored." << endl;
                    }
                    else // Number not in list
                    {
                        // Insert the current number into the even list
                        insertSuccess = orderedListInsertion(evenList, currentNum);

                        
                        
                    } // end inner else
                }
                else // Number is odd
                {
                    
                    // Is number in the list?
                    foundInList = numInList (oddList, currentNum);

                    // If Number is found, the number will be ignored.
                    if (foundInList != NULL) 
                    {
                        cout << setw(3) << currentNum << " is a duplicate and will be ignored." << endl;
                    }
                    else // Number not in list
                    {
                        // Insert the current number into the odd list
                        insertSuccess = orderedListInsertion(oddList, currentNum);

                       
                        
                    } // end inner else
                } // end else
            }// end number range if
            else
            {
                cout << currentNum << " is not between " << MIN_INT << " and " << MAX_INT
                     << " and will be ignored." << endl;
            }
            
            storeFileIn >> currentNum; // grab next number and then evaluate.
           
        } // end While
    
    } // end if
    else
    {
        cout << "Problem Reading file...." << endl;
    }
    
    storeFileIn.close(); // close the file
    
    cout << endl << "Lists created using file " << fileName << endl;
    
    
    cout << setw(3) << listLength(evenList) << " integers inserted into EVEN list." << endl;
    displayList(evenList);
    cout << endl << endl << setw(3) << listLength(oddList) << " integers inserted into ODD list." << endl;
    displayList(oddList);
	
    
    
    
    return;
}

/*********************************************************************************
 *  Function: readSearchFile
 *  Description: Function readSearchFile takes 2 doubly linked lists and a string
 *              file name as input parameters. The function displays the name of
 *              the file it is opening to the screen. The function than opens the 
 *              search file and begins reading one integer at a time until it no 
 *              longer contains data. Each time an integer is read into the function
 *              the function calls isEven to determine if the integer is even or odd.
 *              The function then determines if the proper list is empty or not. If 
 *              the list is empty, the function displays that fact to the screen.
 *              If the list is not empty, the function then calls positionInList to 
 *              determine if the integer currently exists in the list and a position
 *              of the node in the list. If the position equals zero, the function will
 *              display a message that the number is not in the list, else if found
 *              the function will display what position in the list the integer is
 *              located. Once all integers have been read into the function, the 
 *              function will close the file and then return to main. 
 *  Input:
 *      Parameters:
 *          oddList - List to be used for odd integers.
 *          evenList - List to be used for even integers.
 *          fileName - string for the file name of the file to open.
 *      File: searchFileIn - .txt file containing integers to test against the lists.
 *  Output:
 *      Parameters: oddList & evenList are both return parameters and will return
 *          a full list of nodes containing the proper integers that belong in the list.  
 *  Calls to: isEven, emptyList, positionInList
*********************************************************************************/
void readSearchFile (topList& oddList, topList& evenList, string fileName)
{
    ifstream searchFileIn;
    int currentNum, currentPosition;
    bool evenNumber, listIsEmpty;
    
    
    searchFileIn.open(fileName);
    
    cout << endl << endl << "Search results for input file " << fileName << endl;
    
    // priming read
    searchFileIn >> currentNum;
    
    
    while(searchFileIn || !searchFileIn.eof())
    {
        
        evenNumber = isEven(currentNum);
        
        // If the number is even   else... the number is odd.
        if (evenNumber)
        {
            
            // Calls the emptyList function to determine if the list is empty.
            listIsEmpty = emptyList(evenList);
            
            // If... the list is empty, display message stating so.
            if (listIsEmpty)
            {
                cout << setw(3) << currentNum << " not found - EVEN list empty." << endl;
            }
            else// Else... the list is not empty
            {
                // Call the positionInList function to determine if the number is in the list.
                
                currentPosition = positionInList(evenList, currentNum); 
                
                // If... current position equals zero, number not found in list
                if (currentPosition == 0)
                {
                    cout << setw(3) << currentNum << " not found in EVEN list." << endl;
                }
                else// Else... the number is found in the list.
                {
                    cout << setw(3) << currentNum << " found " << currentPosition
                         << " numbers down in EVEN list." << endl;
                }
                
            }
            
        }
        else // Number is odd
        {
            
            listIsEmpty = emptyList(oddList);
            
            // If the list is empty... display message stating so.
            if (listIsEmpty)
            {
                cout << setw(3) << currentNum << " not found - ODD list empty." << endl;
            }
            else // Else... the list is not empty 
            {
                // Call the positionInList function to determine if the number is in the list. 
                currentPosition = positionInList(oddList, currentNum); 
                
                // If... current position equals zero, number not found in list
                if (currentPosition == 0)
                {
                    cout << setw(3) << currentNum << " not found in ODD list." << endl;
                }
                else // Else... the number is found in the list.
                {
                    cout << setw(3) << currentNum << " found " << currentPosition
                         << " numbers down in ODD list." << endl;
                } // End inner inner else.
                
            } // End inner else
            
        } // End outer else
        
        // Next priming read. 
        searchFileIn >> currentNum;
        
    } // End while...
    
       
    searchFileIn.close(); // close the file
    
    
    
    return;
}