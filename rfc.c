//Saúl Enrique Labra Cruz A01020725
//activity 4 - Rail Fence Cipher

#include "rfc.h"

void preparePipes(int in_pipe[], int out_pipe[])
{
    close(in_pipe[1]);//pipe for reading
    close(out_pipe[0]);//pipe for writing
}

void closePipes(int in_pipe[], int out_pipe[])
{
    close(in_pipe[0]);
    close (out_pipe[1]);
}

void encode(int numRails, char * fileName, char * newFileName)
{
    //Variables to read from file
    char buffer[BUFFER_SIZE];
    FILE * inFile;
    inFile = fopen(fileName, "r");

    //Variables for output File
    FILE * newFile;
    strcpy(newFileName, "encoded_");
    strcat(newFileName, fileName);
    newFile = fopen(newFileName, "w");

    //Checking if input file exists
    if(inFile == NULL)
    {
        printf("Error: File not found\n");
    }

    while(fgets(buffer, BUFFER_SIZE, inFile) != NULL)
    {
        //Removing new line char
        if(buffer[strlen(buffer)-1] == '\n')
        {
            buffer[strlen(buffer)-1] = '\0';
        }

        //Variables to encode
        int charCount;
        int railCount[numRails];
        char rails[numRails][BUFFER_SIZE];
        char outBuffer[BUFFER_SIZE];
        
        //Initializing counters to 0
        charCount = 0;
        for(int i=0; i<numRails; i++)
        {
            railCount[i] = 0;
        }

        //Filling strings
        while(buffer[charCount] != EOF)
        {
            for(int i=0; i<numRails; i++)
            {
                rails[i][railCount[i]] = buffer[charCount];
                railCount[i]++;
                charCount++;
            }

            for(int i=numRails-2; i>0; i--)
            {
                rails[i][railCount[i]] = buffer[charCount];
                railCount[i]++;
                charCount++;
            }
        }

        //Appending rails
        strcpy(outBuffer, rails[0]);
        for(int i=1; i<numRails; i++)
        {
            strcat(outBuffer, rails[i]);
        }
        strcat(outBuffer, "\n");

        //saving encoded text into new file
        fputs(outBuffer, newFile);
    }

    fclose(inFile);
    fclose(newFile);
}

void decode(int numRails, char * fileName, char * newFileName)
{
    //Variables to read from file
    char buffer[BUFFER_SIZE];
    FILE * inFile;
    inFile = fopen(fileName, "r");

    //Variables for output File
    FILE * newFile;
    strcpy(newFileName, "decoded_");
    strcat(newFileName, fileName);
    newFile = fopen(newFileName, "w");

    //Checking if input file exists
    if(inFile == NULL)
    {
        printf("Error: File not found\n");
    }

    while(fgets(buffer, BUFFER_SIZE, inFile) != NULL)
    {
        //Removing new line char
        if(buffer[strlen(buffer)-1] == '\n')
        {
            buffer[strlen(buffer)-1] = '\0';
        }

        //Variables to decode
        char rfArr[numRails][strlen(buffer)];
        int charCount;
        int railPos;
        char outBuffer[BUFFER_SIZE];

        //Initializing counters to 0
        charCount = 0;
        railPos = 0;

        //Filling strings
        for(int activeRail=0; activeRail<numRails; activeRail++)
        {
            while(railPos < strlen(buffer))
            {
                for(int i=0; i<numRails; i++)
                {
                    if(railPos >= strlen(buffer))
                    {
                        break;
                    }

                    if(activeRail == i)
                    {
                        rfArr[i][railPos] = buffer[charCount];
                        charCount++;
                    }
                    railPos++;
                }

                for(int i=numRails-2; i>0; i--)
                {
                    if(railPos >= strlen(buffer))
                    {
                        break;
                    }

                    if(activeRail == i)
                    {
                        rfArr[i][railPos] = buffer[charCount];
                        charCount++;
                    }
                    railPos++;
                }
            }
            railPos = 0;
        }

        //Writing buffer
        int position = 0;
        while(position < strlen(buffer))
        {
            for(int i=0; i<numRails; i++)
            {
                outBuffer[position] = rfArr[i][position];
                position++;
            }

            for(int i=numRails-2; i>0; i--)
            {
                outBuffer[position] = rfArr[i][position];
                position++; 
            }
        }

        outBuffer[position] = '\n';
        outBuffer[position+1] = '\0';

        //saving decoded text into new file
        fputs(outBuffer, newFile);
    }

    fclose(inFile);
    fclose(newFile);
}

void encDec(int in_pipe[], int out_pipe[], char option, char * fileName, int numRails)
{
    char buffer[BUFFER_SIZE];
    char newFileName[20];

    //Closing unnecesary communication
    preparePipes(in_pipe, out_pipe);

    if(option == '1')
    {
        encode(numRails, fileName, newFileName);
    }else{
        if(option == '2')
        {
            decode(numRails, fileName, newFileName);
        }else{
            printf("Error: Invalid option\n");
        }
    }

    strcpy(buffer, newFileName);
    write(out_pipe[1], buffer, BUFFER_SIZE);
    
    closePipes(in_pipe, out_pipe);
}

void ui(){
    char option;
    char fileName[50];
    int numRails;

    //Process and communication variables
    char buffer[BUFFER_SIZE];
    pid_t pid;
    int child_to_parent[2];
    int parent_to_child[2];

    do{
        printf("---------------------------------------\n");
        printf("Rail Fence Cipher Decoder/Encoder\n\n");
        printf("Select your option:\n");
        printf("(1) Encode\n");
        printf("(2) Decode\n");
        printf("(0) Exit\n\n");

        scanf("%c", &option);

        if(option != '0')
        {
            if(option != '1' && option != '2')
            {
                printf("Invalid option: Try again!\n\n");
            }else{
                printf("Selected option: %c\n\n", option);

                printf("Introduce the file name:\n");
                scanf("%s", fileName);

                printf("Introduce the number of Rails:\n");
                scanf("%d", &numRails);

                printf("name: %s\trails: %d\n", fileName, numRails);

                //opening communication
                pipe(child_to_parent);
                pipe(parent_to_child);

                //creating new process
                pid = fork();

                //if it is the parent process
                if(pid > 0)
                {
                    int status;

                    //closing unecesary communication
                    preparePipes(child_to_parent, parent_to_child);

                    //waiting for child to finish
                    waitpid(pid, &status, 0);

                    read(child_to_parent[0], buffer, BUFFER_SIZE);

                    printf("\nNew file name: %s\n", buffer);
                }else{
                    //if it is the child process
                    if(pid == 0)
                    {
                        encDec(parent_to_child, child_to_parent, option, fileName, numRails);
                        break;
                    }
                }
            }
        }
        getc(stdin);
    }while(option != '0');

    closePipes(child_to_parent, parent_to_child);
    return;
}