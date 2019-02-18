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

void encDec(int in_pipe[], int out_pipe[], char option, char * fileName, int numRails)
{
    char buffer[BUFFER_SIZE];
    char outBuffer[BUFFER_SIZE];

    //Closing unnecesary communication
    preparePipes(in_pipe, out_pipe);

    printf("Option received: %c\n", option);
    printf("path received: %s\n", fileName);
    printf("Number of rails: %d\n", numRails);

    //Reading text from file
    FILE * file;
    file = fopen(fileName, "r");

    //Checking if file exists
    if(file == NULL)
    {
        printf("Error: File not found\n");
    }

    //Obtaining string from file
    fgets(buffer, BUFFER_SIZE, file);

    printf("Read string: %s\n", buffer);

    //Encode option
    if(option == '1')
    {
        int dir = 1;
        int charCount = 0;
        char rails[numRails][BUFFER_SIZE];
        
        int railCount[numRails];
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

        //Printing rails
        for(int i=0; i<numRails; i++)
        {
            printf("%s\n", rails[i]);
        }

        //printing encrypted string
        for(int i=0; i<numRails; i++)
        {
            strcat(outBuffer, rails[i]);
        }

        printf("Encoded text: %s\n", outBuffer);

    }else{
        //Decode option
        if(option == '2')
        {

        }else{
            printf("Error: Invalid option\n");
        }
    }

    strcpy(buffer, "ejemplo de path");
    printf("Buffer desde el hijo: %s\n", buffer);
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

                printf("Introduce the file name/path:\n");
                scanf("%s", fileName);

                printf("Introduce the number of Rails:\n");
                scanf("%d", &numRails);

                printf("path: %s\trails: %d\n", fileName, numRails);

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

                    printf("Buffer desde el padre: %s\n", buffer);
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

/*void createProcess()
{
    //variables to fork and pipe
    pid_t pid;
    int child_to_parent[2];
    int parent_to_child[2];

    //creating child process
    pid = fork();

    //Opening pipe
    if(pipe(child_to_parent) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if(pipe(parent_to_child) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    //parent process
    if(pid > 0)
    {
        ui(child_to_parent, parent_to_child);
        wait(NULL);
    }else{
        //Child process
        if(pid == 0)
        {
            encDec(parent_to_child, child_to_parent);
        }else{
            printf("Error: Unable to fork\n");
            exit(EXIT_FAILURE);
        }
    }
}*/