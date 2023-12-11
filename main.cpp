#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>
#include <conio.h>
#include <sstream>

#define MAX_WORD_LENGTH 100
#define MAX_WRONG_GUESSES 5
#define NUMBER_OF_STAGES 5
#define WORDS_PER_STAGE 10
void outtextxy_int(int x, int y, int value) {
    char num_str[10];
    sprintf(num_str, "%d", value);
    outtextxy(x, y, num_str);
}
void drawHangman(int wrongGuesses) {
    // Coordinates for the hangman
    int headX = 200, headY = 100, headRadius = 30;
    int bodyStartY = headY + headRadius, bodyEndY = bodyStartY + 100;
    int armStartY = bodyStartY + 30, armEndY = armStartY + 50;
    int legStartY = bodyEndY, legEndY = legStartY + 50;

    // Draw the gallows
    line(150, 50, 150, 300); // Vertical line
    line(150, 50, 200, 50); // Horizontal line
    line(200, 50, 200, 70); // Small vertical line for hanging

    // Draw the hangman based on the number of wrong guesses
    if (wrongGuesses >= 1) {
        // Draw head
        circle(headX, headY, headRadius);
    }
    if (wrongGuesses >= 2) {
        // Draw body
        line(headX, bodyStartY, headX, bodyEndY);
    }
    if (wrongGuesses >= 3) {
        // Draw both arms in one stage
        line(headX, armStartY, headX - 30, armEndY); // Left arm
        line(headX, armStartY, headX + 30, armEndY); // Right arm
    }
    if (wrongGuesses >= 4) {
        // Draw both legs in one stage
        line(headX, legStartY, headX - 30, legEndY); // Left leg
        line(headX, legStartY, headX + 30, legEndY); // Right leg
    }
    if (wrongGuesses >= 5) {
        // Optionally, draw the face (eyes, mouth) on the head
        // to represent the final stage of the game being lost.
        // For example, drawing two eyes:
        circle(headX - 10, headY - 10, 2); // Left eye
        circle(headX + 10, headY - 10, 2); // Right eye
        // And a sad mouth:
        arc(headX, headY, 200, 340, 10);
    }
}

void initializeWord(const char *word, char *hiddenWord) {
    int len = strlen(word);
    for (int i = 0; i < len; i++) {
        hiddenWord[i] = '_';
    }
    hiddenWord[len] = '\0';
}
void getInputFromGraphicsWindow(char *input, int maxLen) {
    int index = 0;
    char str[2] = "\0"; // Temporary string for output
    while (index < maxLen - 1) {
        char ch = getch(); // Get character without echo
        if (ch == 13) { // Enter key
            break; // End input
        } else if (ch == 8) { // Backspace key
            if (index > 0) { // If there's something to delete
                index--; // Go back one character
                input[index] = '\0'; // Delete it
                // "Delete" it on the screen as well
                outtextxy(10 + textwidth(input), 100, "  ");
            }
        } else { // Regular character input
            input[index++] = ch; // Add to string
            input[index] = '\0'; // Null-terminate
            str[0] = ch; // Prepare for output
            outtextxy(10 + textwidth(input) - textwidth(str), 100, str); // Show on screen
        }
    }
    input[index] = '\0'; // Ensure null-terminated
}
void updateHiddenWord(const char *word, char *hiddenWord, char guess) {
    int len = strlen(word);
    for (int i = 0; i < len; i++) {
        if (word[i] == guess) {
            hiddenWord[i] = guess;
        }
    }
}

char toLowercase(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 'a';
    }
    return c;
}


void displayMenuAndGetChoice(int &choice) {
    // Variables to hold the menu text
    char menuText[10][1000] = {
        "Menu:",
        "1. Multiplayer",
        "2. Single Player",
        "3. Rules",
        "4. Quit"
    };
    
    // Clear the graphics window and display the menu
    cleardevice();
    for (int i = 0; i < 5; i++) {
        outtextxy(10, 10 + i * 15, menuText[i]);
    }

    // Temporary string to display the choice
    char strChoice[3] = {0};
    outtextxy(10, 85, menuText[5]);

    // Get the choice from the user using the graphics window
    do {
        char ch = getch(); // Wait for the user to press a key
        if (ch >= '1' && ch <= '4') { // Check if the key is between 1 and 4
            strChoice[0] = ch; // Assign the character to the string
            strChoice[1] = '\0'; // Null-terminate the string
            outtextxy(220, 85, strChoice); // Show the choice on the screen
            choice = ch - '0'; // Convert the character to an integer
            break; // Exit the loop since we have a valid choice
        }
    } while (true);
}

int main() {
    const char *stageWords[NUMBER_OF_STAGES][WORDS_PER_STAGE] = {
            {"apple", "banana", "cherry", "date", "elderberry", "fig", "grape", "honeydew", "kiwi", "lemon"},
            {"carrot", "broccoli", "asparagus", "cauliflower", "corn", "cucumber", "eggplant", "jicama", "kale", "lettuce"},
            {"tiger", "elephant", "leopard", "giraffe", "hippopotamus", "rhinoceros", "zebra", "kangaroo", "panda", "gorilla"},
            {"violin", "guitar", "piano", "trumpet", "drums", "flute", "harp", "saxophone", "cello", "clarinet"},
            {"amazon", "nile", "yangtze", "mississippi", "yenesey", "yellow", "ob", "parana", "congo", "amur"}
    };
    int choice;
    char  word[MAX_WORD_LENGTH],hiddenWord[MAX_WORD_LENGTH],hiddenWordDisplay[100];
    int stage = 0;
    char guess;
    int wrongGuesses;
int gd = DETECT, gm;
initgraph(&gd, &gm, "path to the BGI folder");
    srand(time(NULL)); // Initialize random number generator

    do {
        displayMenuAndGetChoice(choice);
        switch (choice) {
            case 1:
                { cleardevice(); // Clear the graphics window
        outtextxy(10, 10, "Enter a word: ");

        getInputFromGraphicsWindow(word, MAX_WORD_LENGTH);

        initializeWord(word, hiddenWord);
        wrongGuesses = 0;

        while (wrongGuesses < MAX_WRONG_GUESSES) {
            cleardevice();

            // Display the hidden word
            strcpy(hiddenWordDisplay, "Hidden word: ");
            strcat(hiddenWordDisplay, hiddenWord);
            outtextxy(10, 30, hiddenWordDisplay);

            drawHangman(wrongGuesses);

            outtextxy(10, 50, "Enter your guess: ");
            guess = getch();
            guess = toLowercase(guess);

            if (strchr(word, guess) != NULL) {
                updateHiddenWord(word, hiddenWord, guess);
                if (strcmp(word, hiddenWord) == 0) {
                    // Player guessed correctly
                    strcpy(hiddenWordDisplay, "Hidden word: ");
                    strcat(hiddenWordDisplay, hiddenWord);
                    outtextxy(10, 30, hiddenWordDisplay);
                    outtextxy(10, 70, "Congratulations!");
                    getch();
                    break;
                }
            } else {
                wrongGuesses++;
            }
        }

        if (wrongGuesses == MAX_WRONG_GUESSES) {
            drawHangman(wrongGuesses);
            outtextxy(10, 70, "Game over!");
            getch();
            // Reset wrong guesses if you want to play again
            wrongGuesses = 0;
        }
        break;}

                      

            case 2:
{
srand(time(NULL));
stage=0;
while (stage < NUMBER_OF_STAGES) {
    cleardevice();
    const char *word = stageWords[stage][rand() % WORDS_PER_STAGE];
    initializeWord(word, hiddenWord);
    wrongGuesses = 0;

    while (wrongGuesses < MAX_WRONG_GUESSES) {
        cleardevice();

        // Display the current stage number
        char stageDisplay[MAX_WORD_LENGTH];
        sprintf(stageDisplay, "Stage: %d", stage + 1); // Add 1 to stage for 1-based numbering
        outtextxy(10, 10, stageDisplay);

        // Display the hidden word
        char hiddenWordDisplay[MAX_WORD_LENGTH * 2];
        strcpy(hiddenWordDisplay, "Hidden word: ");
        strcat(hiddenWordDisplay, hiddenWord);
        outtextxy(10, 30, hiddenWordDisplay);

        // Draw the hangman
        drawHangman(wrongGuesses);

        // Prompt for guess
        outtextxy(10, 50, "Enter your guess: ");
        guess = getch();
        guess = toLowercase(guess);

        if (strchr(word, guess) != NULL) {
            updateHiddenWord(word, hiddenWord, guess);
            if (strcmp(word, hiddenWord) == 0) {
            	strcpy(hiddenWordDisplay, "Hidden word: ");
            strcat(hiddenWordDisplay, hiddenWord);
                	outtextxy(10, 30, hiddenWordDisplay);
                outtextxy(10, 70, "Correct!");
                getch();
                break;
            }
        } else {
            wrongGuesses++;
        }
    }

    if (wrongGuesses == MAX_WRONG_GUESSES) {
        drawHangman(wrongGuesses);
        char gameOverMsg[MAX_WORD_LENGTH * 2];
        strcpy(gameOverMsg, "Game over! ");
     
        outtextxy(10, 70, gameOverMsg);
        getch();
        break;
    } else {
        stage++;
    }

    if (stage == NUMBER_OF_STAGES) {
        outtextxy(10, 90, "Congratulations!");
        outtextxy(10, 110, "You won!");
        getch();
        stage = 0;
    }
} break;}



            case 3:
            {
			
               cleardevice(); // Clear the graphics window

outtextxy(10, 10, "Rules of the Game:");

outtextxy(10, 30, "1. Objective: Guess the hidden word by suggesting letters within a limited number of guesses.");
outtextxy(10, 50, "2. Game Modes:");
outtextxy(30, 70, "- Multiplayer: One player enters a word, another guesses it.");
outtextxy(30, 90, "- Single Player: Guess a word randomly chosen from a list.");
outtextxy(10, 110, "3. Gameplay:");
outtextxy(30, 130, "- The hidden word is represented by underscores.");
outtextxy(30, 150, "- Each turn, suggest a letter.");
outtextxy(30, 170, "- Correct guesses reveal the letter(s) in the word.");
outtextxy(30, 190, "- Incorrect guesses count against your guess limit.");
outtextxy(10, 210, "4. Guesses:");
outtextxy(30, 230, "- Maximum of 5 wrong guesses allowed per word.");
outtextxy(10, 250, "5. Winning and Losing:");
outtextxy(30, 270, "- Correctly guess the word to win or advance to the next stage (Single Player).");
outtextxy(30, 290, "- Exceeding the wrong guess limit means a loss. In Single Player, you restart from stage 1.");
outtextxy(10, 310, "6. Letter Case: The game is not case-sensitive.");
outtextxy(10, 330, "7. Exiting: Select 'Quit' from the main menu to exit the game.");

getch();}


            case 4:
               {
				 printf("Exiting the game.\n");
                break;
}
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
        }
    } while (choice != 4); 
 closegraph();
    return 0;
}

