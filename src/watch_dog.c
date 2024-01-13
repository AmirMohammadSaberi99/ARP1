#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <signal.h>
#include <time.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <sys/select.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include "../headers/constant.h"
#include "../headers/log_functions.h"
#include "../headers/shared_memory.h"

bool checkTime(int startHour, int mStart, int startMinute, int startSecond);

/* Perform cleanup tasks and gracefully terminate the process*/

void cleanupAndTerminate()
{
    printf("Cleaning up and terminating.\n");

    /* End the execution of the process */
    exit(EXIT_SUCCESS);
}

/* Handler for gracefully handling the SIGINT (Ctrl+C) signal */
void handleSIGINT(int signal)
{
    if (signal == SIGINT)
    {
        printf("Caught SIGINT Signal!");
        cleanupAndTerminate();
    }
}

int main(int argumentCount, char const *argumentValues[])
{
  FILE *filePointer;
  char *authenticationToken;
  char dataBuffer[100];
  int pidOfProcess;
  int activityTimestampHour;
  int previousOperationMinute;
  int lastOperationSecond;
  bool activityStatusOfProcess;
  int numOfIdleProcesses = 0;
  char processStatusLogAddress [4][20] = {"./drone.txt", "./server.txt", "./window.txt", "./master.txt"};
  char activeProcessPIDs[4][20] = {"", "", "", ""};
  
  while(1){
    sleep(CHECK_TIME); /* Pause execution for 60 seconds and subsequently monitor the process activity status*/
    while (numOfIdleProcesses<NUMBER_OF_INACTIVE_PROCESS){
      /* Access the target process's logfile to read its PID */
      filePointer = fopen(processStatusLogAddress[numOfIdleProcesses], "r");
      /* Checking for errors and handling them appropriately */ 
      if (filePointer<0){
          printf("Failed to open the 'mxCommand' pipe; Error number: errno =%d\n", errno);
          exit(1); 
      } 

      /* Setting the cursor to the beginning of the file */
      fseek(filePointer, 0, SEEK_SET);
      /* Retrieving the first 50 bytes from the file */
      fread(dataBuffer, 50, 1, filePointer);
       /* Finalizing and closing the file*/
      fclose(filePointer);

      /* Obtaining the Process ID (PID) of the target process */
      authenticationToken = strtok(dataBuffer, ",");
      strcpy(activeProcessPIDs[numOfIdleProcesses], authenticationToken);
      pidOfProcess = atoi(authenticationToken);

      /* Obtaining the timestamp of the target process's last activity */
      authenticationToken = strtok(NULL,",");
      activityTimestampHour = atoi(authenticationToken);

      authenticationToken = strtok(NULL,",");
      previousOperationMinute = atoi(authenticationToken);

      authenticationToken = strtok(NULL,",");
      lastOperationSecond = atoi(authenticationToken);
      
      /* Displaying the last active time of the target process */
      printf("Process %d last activity time (H:M:S):%d:%d:%d \n",pidOfProcess, activityTimestampHour, previousOperationMinute, lastOperationSecond);
      /* Assessing the activity status of the target process in the last 60 seconds */
      printf("Verify the activity status of the target process in the last 60 seconds.\n");
      activityStatusOfProcess = checkTime(activityTimestampHour, previousOperationMinute, lastOperationSecond, INACTIVE_TIME_LIMIT);
      /* Check for inactivity of the process in the last 60 seconds */
      if(activityStatusOfProcess){
        /* Output details of the inactive process */
        printf("Process %d has been inactive for over 60 seconds...(\n\n", pidOfProcess);
        /* Increment the inactive process counter by 1 */
        numOfIdleProcesses = numOfIdleProcesses + 1;
        /* Check if all processes were inactive during the last 60 seconds */
        if(numOfIdleProcesses ==NUMBER_OF_INACTIVE_PROCESS){  
            printf("Terminating all processes:|\n");
            for(int i=0;i<NUMBER_OF_INACTIVE_PROCESS;i++){
                /* Issuing a 'Terminat signal' to all target processes */
                printf("Ending process %s\n", activeProcessPIDs[i]);
                kill(atoi(activeProcessPIDs[i]), SIGINT);
              }
            }
        }else{
            /* Skip the checking process if at least one process was active in the last 60 seconds */
            printf("Perfect!Process %d is currently active:)\n\n", pidOfProcess);
            numOfIdleProcesses = 0;
            break;
          }
      }
    }
    return 0; 
}

/* Function to verify the current activity status of a target process within a specified time limit, considering its last activity time */
bool checkTime(int hStart, int mStart, int sStart, int durationSeconds){
     time_t t = time(NULL);  
     struct tm tm = *localtime(&t);
     /* Check if the process's last activity time is within the specified time limit */  
     bool staus = (tm.tm_hour - hStart)*3600 + (tm.tm_min - mStart)*60 + (tm.tm_sec - sStart) > durationSeconds;
     if(staus){
       printf("Time limit exceeded\n");
       return true;
     }
     return false;      
}

