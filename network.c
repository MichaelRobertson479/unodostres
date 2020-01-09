// headers
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <dirent.h>
    #include <sys/shm.h>
    #include <sys/ipc.h>
    #include <sys/sem.h>
    #include <time.h>
// headers

// memory
    #define NUMOFPLAYERS 71232
    #define PLAYERS 23456
    #define TURNCOUNT 34567
    #define PLAYERNUM 45678
// memory

int myNumber;
int players;
int turnMem;
int turn;

void init() {

    int inputPlayers;
    int playerNumber;
    int numOfPlayers;
    int* data;
    numOfPlayers = shmget(NUMOFPLAYERS, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);

    if (numOfPlayers == -1) {
        printf("Segment already exists\n");
        numOfPlayers = shmget(NUMOFPLAYERS, sizeof(int), 0644);
        data = shmat(numOfPlayers, 0, 0);
        printf("Number of players: %d\n", *data);

        if (*data == 0) {
            printf("Game is full\n");
            exit(0);
        }

        else {
            printf("Joining game...\n");

            //decrease "number of players"
            (*data)--;

            playerNumber = shmget(PLAYERNUM, sizeof(int), 0644);
            data = shmat(playerNumber, 0, 0);

            //set "myNumber"
            myNumber = *data;

            //increase "player number"
            (*data)++;

            //set "players"
            inputPlayers = shmget(PLAYERS, sizeof(int), 0644);
            data = shmat(inputPlayers, 0, 0);
            players = *data;

            //set turn
            turnMem = shmget(TURNCOUNT, sizeof(int), 0644);
            data = shmat(turnMem, 0, 0);
            turn = *data;
        }
    }

    else {
        printf("Segment created\n");
        data = shmat(numOfPlayers, 0, 0);
        printf("Input number of players: \n");
        
        //change to input later on
        int input = 3;

        //set "number of players"
        *data = input;

        //decrement "number of players"
        (*data)--;

        //set local and shared "players"
        inputPlayers = shmget(PLAYERS, sizeof(int), IPC_CREAT | 0644);
        data = shmat(inputPlayers, 0, 0);
        *data = input;
        players = input;

        //make self player 1
        myNumber = 1;

        //set up for next player
        playerNumber = shmget(PLAYERNUM, sizeof(int), IPC_CREAT | 0644);
        data = shmat(playerNumber, 0, 0);
        *data = 2;

        //set turn count
        turnMem = shmget(TURNCOUNT, sizeof(int), IPC_CREAT | 0644);
        data = shmat(turnMem, 0, 0);
        *data = 1;
    }
}

int main() {

    init();

    int data;

    printf("You are player %d\n", myNumber);
    printf("There are %d players in this game\n", players);

    //change to while not win condition
    while (true) {

        //save turn locally
        turnMem = shmget(TURNCOUNT, sizeof(int), 0644);
        data = shmat(turnMem, 0, 0);
        turn = *data;

        if (turn % players == myNumber) {
            printf("It's your turn\n")
            
            //temporary action
            wait(10);
        }

        else {

            //until turn changes, keep getting it
            while (*data != (turn + 1)) {
                turnMem - shmget(TURNCOUNT, sizeof(int), 0644);
                data = shmat(turnMem, 0, 0);
            }

            turn = *data;
            printf("It is turn %d\n", turn);

            //temporary action
            wait(10);
        }

    }

    return 0;
}