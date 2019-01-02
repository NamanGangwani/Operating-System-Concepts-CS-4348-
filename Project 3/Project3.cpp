/*
    Naman Gangwani
    nkg160030
    April 23, 2018
    CS 4348.002
*/

#include <iostream>
#include <fstream>
#include <queue>
#include <climits>

void FCFS(int data[][2], int numJobs);
void RR(int data[][2], int numJobs);
void SPN(int data[][2], int numJobs);
void SRT(int data[][2], int numJobs);
void HRRN(int data[][2], int numJobs);
void FB(int data[][2], int numJobs);

int main(int argc, char* argv[])
{
    // If the type of algorithm was not specified in the command line, throw an error and exit
    if (argc < 2)
    {
        std::cerr << "ERROR: no scheduling algorithm specified in command line \nPlease specify FCFS, RR, SPN, SRT, HRRN, FB, or ALL.\n";
        return 1;
    }

    std::string algorithm = argv[1];

    std::ifstream jobs("jobs.txt"); // text file input
    // Error if the file does not exist
    if (!jobs)
    {
        std::cerr << "ERROR: unable to open file jobs.txt";
        return 1;
    }

    // Keeping track of the job, arrival time, and service time in the file per line
    char job;
    int arrivalTime, serviceTime, numLines = 0;
    while(jobs >> job >> arrivalTime >> serviceTime)
        ++numLines;
    // Closes, clears read buffer, and reopens file to actually read in the data now that number of lines is known
    jobs.close();
    jobs.clear();
    jobs.open("jobs.txt");
    /* Reads in jobs, arrival times, and service times of the jobs and saves them for later usage */
    int data[numLines][2], i = 0;
    while (jobs >> job >> arrivalTime >> serviceTime)
    {
        data[i][0] = arrivalTime;
        data[i++][1] = serviceTime;
    }

    /* Runs the specified algorithm(s) by the user */
    if (algorithm.compare("FCFS") == 0)
        FCFS(data, numLines);
    else if (algorithm.compare("RR") == 0)
        RR(data, numLines);
    else if (algorithm.compare("SPN") == 0)
        SPN(data, numLines);
    else if (algorithm.compare("SRT") == 0)
        SRT(data, numLines);
    else if (algorithm.compare("HRRN") == 0)
        HRRN(data, numLines);
    else if (algorithm.compare("FB") == 0)
        FB(data, numLines);
    else if (algorithm.compare("ALL") == 0)
    {
        std::cout << "FCFS: " << std::endl;
        FCFS(data, numLines);
        std::cout << "\nRR: " << std::endl;
        RR(data, numLines);
        std::cout << "\nSPN: " << std::endl;
        SPN(data, numLines);
        std::cout << "\nSRT: " << std::endl;
        SRT(data, numLines);
        std::cout << "\nHRRN: " << std::endl;
        HRRN(data, numLines);
        std::cout << "\nFB: " << std::endl;
        FB(data, numLines);
    }

    // Close the job file and exit
    jobs.close();
    return 0;
}

/*
    Print from A to some letter in alphabetical order on the screen
*/
void printCharacters(int numJobs)
{
    // Prints the job characters
    for (int i = 0; i < numJobs; i++)
        std::cout << (char)(i + 65) << " ";
    std::cout << std::endl;
}

/*
    Print the current job's activity
*/
void printX(int activeJob)
{
    for (int i = 0; i < activeJob; i++)
        std::cout << "  ";
    std::cout << "X" << std::endl;
}

/*
    First Come First Serve
*/
void FCFS(int data[][2], int numJobs)
{
    printCharacters(numJobs);

    // Copy the service times into a new array to allow for mutating
    int serviceTimes[numJobs];
    for (int i = 0; i < numJobs; i++)
        serviceTimes[i] = data[i][1];
    bool allClear;
    std::queue <int> q;
    int time = 0, activeJob = -1;
    do
    {
        // Add any jobs that arrived to the queue
        for (int i = 0; i < numJobs; i++)
            if (data[i][0] == time)
                q.push(i);

        // If there is an opening for a job to run
        if (activeJob == -1)
        {
            // Assign the current job to run
            if (!q.empty())
                activeJob = q.front();
            else
            {
                time++;
                std::cout << std::endl;
                continue;
            }
        }

        // Print the activity of the current job
        printX(activeJob);

        if (--serviceTimes[activeJob] == 0) // If the current job has finished its service time
        {
            // Allow next job in queue to execute
            activeJob = -1;
            q.pop();
        }

        // Check to see if all the jobs have completed
        allClear = true;
        for (int i = 0; i < numJobs; i++)
        {
            allClear = allClear && (serviceTimes[i] == 0);
            if (!allClear) break;
        }

        // Increment the time
        time++;
    } while (!allClear); // Repeat until all jobs have completed
}

/*
    Round Robin Algorithm
    Quantum = 1
*/
void RR(int data[][2], int numJobs)
{
    printCharacters(numJobs);

    // Copy the service times into a new array to allow for mutating
    int serviceTimes[numJobs];
    for (int i = 0; i < numJobs; i++)
        serviceTimes[i] = data[i][1];
    bool allClear;
    std::queue <int> q;
    // Add any jobs that arrived to the queue
    for (int i = 0; i < numJobs; i++)
        if (data[i][0] == 0)
            q.push(i);

    int time = 0, activeJob = -1;
    do
    {
        if (!q.empty())
        {
            // Assign the current job to run
            activeJob = q.front();
            printX(activeJob);
        } else
        {
            time++;
            // Add any jobs that arrived to the queue
            for (int i = 0; i < numJobs; i++)
                if (data[i][0] == time)
                    q.push(i);
            std::cout << std::endl; // No activity in this time segment
            continue;
        }
        // Decrease active job's service time and dequeue it immediately (since quantum = 1)
        serviceTimes[activeJob]--;
        q.pop();

        // Increment the time
        time++;

        // Add any jobs that arrived to the queue
        for (int i = 0; i < numJobs; i++)
            if (data[i][0] == time)
                q.push(i);

        if (serviceTimes[activeJob] > 0) // If the current job has not finished its service time
            q.push(activeJob); // Add it back to the queue

        // Check to see if all the jobs have completed
        allClear = true;
        for (int i = 0; i < numJobs; i++)
        {
            allClear = allClear && (serviceTimes[i] == 0);
            if (!allClear) break;
        }

    } while (!allClear); // Repeat until all jobs have completed
}

/*
    Shortest Process Next
*/
void SPN(int data[][2], int numJobs)
{
    printCharacters(numJobs);

    // Copy the service times into a new array to allow for mutating
    int serviceTimes[numJobs];
    for (int i = 0; i < numJobs; i++)
        serviceTimes[i] = data[i][1];
    bool allClear;
    std::queue <int> q;
    int time = 0, activeJob = -1, timeLeft = 0;
    do
    {
        if (activeJob == -1) // If there are no active jobs
        {
            // Loop through all valid jobs to find the next active job
            int shortest = INT_MAX;
            for (int i = 0; i < numJobs; i++)
                if (serviceTimes[i] > 0 && data[i][0] <= time && data[i][1] < shortest)
                {
                    activeJob = i;
                    timeLeft = data[i][1];
                    shortest = timeLeft;
                }
            if (activeJob == -1) // If no valid active job was found
            {
                // The system is idle
                time++;
                std::cout << std::endl;
                continue;
            }
        }

        // Print the activity of the current job
        printX(activeJob);

        // If the active job has completed, allow another job to take over
        if (--serviceTimes[activeJob] == 0)
            activeJob = -1;

        // Check to see if all the jobs have completed
        allClear = true;
        for (int i = 0; i < numJobs; i++)
        {
            allClear = allClear && (serviceTimes[i] == 0);
            if (!allClear) break;
        }

        // Increment the time
        time++;
    } while (!allClear); // Repeat until all jobs have completed
}

/*
    Shortest Remaining Time
*/
void SRT(int data[][2], int numJobs)
{
    printCharacters(numJobs);

    // Copy the service times into a new array to allow for mutating
    int serviceTimes[numJobs];
    for (int i = 0; i < numJobs; i++)
        serviceTimes[i] = data[i][1];
    bool allClear;
    int time = 0, activeJob = -1;
    do
    {
        // Loop through all valid jobs to find the next active job
        int shortest = INT_MAX;
        for (int i = 0; i < numJobs; i++)
            if (data[i][0] <= time && serviceTimes[i] > 0 && serviceTimes[i] < shortest)
            {
                activeJob = i;
                shortest = serviceTimes[i];
            }
        if (activeJob == -1) // If no valid active job was found
        {
            // The system is idle
            time++;
            std::cout << std::endl;
            continue;
        }

        // Print the activity of the current job
        printX(activeJob);

        // Decrements the amount of time left for the current active job
        serviceTimes[activeJob]--;

        // Check to see if all the jobs have completed
        allClear = true;
        for (int i = 0; i < numJobs; i++)
        {
            allClear = allClear && (serviceTimes[i] == 0);
            if (!allClear) break;
        }

        // Active job is no longer active
        activeJob = -1;

        // Increment the time
        time++;
    } while (!allClear); // Repeat until all jobs have completed
}

/*
    Highest Response Ratio Next
*/
void HRRN(int data[][2], int numJobs)
{
    printCharacters(numJobs);

    // Copy the service times into a new array to allow for mutating
    int serviceTimes[numJobs];
    for (int i = 0; i < numJobs; i++)
        serviceTimes[i] = data[i][1];
    bool allClear;
    int time = 0, activeJob = -1;
    do
    {
        // Loop through all valid jobs to find the next active job
        int highest = INT_MIN;
        if (activeJob == -1)
        {
            for (int i = 0; i < numJobs; i++)
            {
                int r = (time - data[i][0] + data[i][1])/data[i][1];
                if (data[i][0] <= time && serviceTimes[i] > 0 && r > highest)
                {
                    activeJob = i;
                    highest = r;
                }
            }
            if (activeJob == -1) // If no valid active job was found
            {
                // The system is idle
                time++;
                std::cout << std::endl;
                continue;
            }
        }

        // Print the activity of the current job
        printX(activeJob);

        // Decrements the amount of time left for the current active job
        serviceTimes[activeJob]--;
        if (serviceTimes[activeJob] == 0)
            activeJob = -1;

        // Check to see if all the jobs have completed
        allClear = true;
        for (int i = 0; i < numJobs; i++)
        {
            allClear = allClear && (serviceTimes[i] == 0);
            if (!allClear) break;
        }

        // Increment the time
        time++;
    } while (!allClear); // Repeat until all jobs have completed
}

/*
    Multilevel Feedback
    Quantum = 1
*/
void FB(int data[][2], int numJobs)
{
    printCharacters(numJobs);

    // Copy the service times into a new array to allow for mutating
    int serviceTimes[numJobs];
    for (int i = 0; i < numJobs; i++)
        serviceTimes[i] = data[i][1];
    bool allClear;
    std::queue <int> q1, q2, q3;
    // Add any jobs that arrived to the queue
    for (int i = 0; i < numJobs; i++)
        if (data[i][0] == 0)
            q1.push(i);

    int time = 0, activeJob = -1, currentQueue = -1;
    do
    {
        /*  Search starting from the first queue.
            If first queue is empty, search the second queue
            If second queue is empty, search the third queue
            If third queue is empty and there are still no active jobs, the system is idle
        */
        if (!q1.empty())
        {
            // Assign the current job to run
            activeJob = q1.front();
            q1.pop();
            currentQueue = 1;
        } else if (!q2.empty())
        {
            // Assign the current job to run
            activeJob = q2.front();
            q2.pop();
            currentQueue = 2;
        } else if (!q3.empty())
        {
            // Assign the current job to run
            activeJob = q3.front();
            q3.pop();
            currentQueue = 3;
        }
        else
        {
            if (activeJob == -1) // If there is currently no active job
            {
                //The system is idle
                time++;
                // Add any jobs that arrived to the queue
                for (int i = 0; i < numJobs; i++)
                    if (data[i][0] == time)
                        q1.push(i);
                std::cout << std::endl; // No activity in this time segment
                continue;
            }
        }

        // Decrease active job's service time and dequeue it immediately (since quantum = 1)
        serviceTimes[activeJob]--;
        printX(activeJob);

        // Increment the time
        time++;

        // Add any jobs that arrived to the queue
        for (int i = 0; i < numJobs; i++)
            if (data[i][0] == time)
                q1.push(i);

        if (serviceTimes[activeJob] > 0) // If the current job has not finished its service time
        {
            if (!q1.empty() || !q2.empty() || !q3.empty())
            {
                if (currentQueue + 1 == 2)
                    q2.push(activeJob); // Add it back to the next queue
                else
                    q3.push(activeJob); // Add it back to the last queue
            }
        }
        else
            activeJob = -1; // Current job has finished

        // Check to see if all the jobs have completed
        allClear = true;
        for (int i = 0; i < numJobs; i++)
        {
            allClear = allClear && (serviceTimes[i] == 0);
            if (!allClear) break;
        }

    } while (!allClear); // Repeat until all jobs have completed
}
