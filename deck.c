// headers
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/ipc.h>
    #include <sys/sem.h>
    #include <sys/shm.h>
    #include <sys/types.h>
    #include <errno.h>
    #include <string.h>
    #include <fcntl.h>
    #include <time.h>
// headers

// deck
    char * deck[] = {"R0","R1","R2","R3","R4","R5","R6","R7","R8","R9",
            "B0","B1","B2","B3","B4","B5","B6","B7","B8","B9",
            "G0","G1","G2","G3","G4","G5","G6","G7","G8","G9",
            "Y0","Y1","Y2","Y3","Y4","Y5","Y6","Y7","Y8","Y9", };
// deck

// hand
    struct hands {   
        char * arr[100]; 
        int size;
    };

    struct hands hand;
// hand

char * lastPlayed;

// only draws one card
char * draw() {
    char * card = deck[rand() % 40];
    hand.arr[hand.size] = card;
    hand.size++;
    return card;
}

// input is index of card in hand (one-indexed)
void play (int i) {

    i--;
    char * card = hand.arr[i];

    // if color is same or if value is same (respectively)
    if (hand.arr[i][0] == lastPlayed[0] || hand.arr[i][1] == lastPlayed[1]) {

        // "shift cards over"
        while (hand.arr[i] != NULL) {
            hand.arr[i] = hand.arr[i+1];
            i++;
        }

        lastPlayed = card;
        hand.size--;
    }

    // temp error msg
    else {
        printf("invalid card\n"); 
    }
}

void init() {
        
    hand.size = 0;

    draw();
    draw();
    draw();
    draw();
    draw();
}

int main(int argc, char *argv[]) {

    srand(time(0));

    // random initial card
    lastPlayed = deck[rand() % 40];

    // dummy variable
    int i;

    init();

    while (hand.size != 0) {

        printf("Top of the pile: %s\n",lastPlayed);

        // show hand
            printf("Your hand: ");

            i = 0;
            while (hand.arr[i] != NULL) {
                printf("%s ",hand.arr[i]);
                i++;
            }

            printf("\n");
        // show hand

        // get input
            i = 0;
            char input[50];
            fgets(input, 50, stdin);
            while (input[i] != '\n') {
                i++;
            }
            input[i] = '\0';
        // get input

        if (strcmp(input,"draw") == 0) {
            draw();
        }
        
        // play card
            else if (strstr(input,"play") != NULL) {
                printf("gets here\n");
                strsep(&input," ");
                i = atoi(input);

                if (i == 0) {
                    printf("Invalid index\n");
                }

                else {
                    play(i);
                }
            }
        // play card
    }

    return 0;
}