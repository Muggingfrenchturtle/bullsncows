//kristan troy aguilar
//simon andrew labay
#include <iostream>
#include <cstdlib> //for generating random numbers
#include <time.h> //for generating seeds for the rng
                  //https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/
                  //https://cplusplus.com/reference/cstdlib/rand/

using namespace std;


class CnBplayer
{
    public :
        bool isCPU = true;
        int CPUDifficulty = 2; //1 = easy, 2 = medium
        int numberCode[4] = {0,0,0,0};

        int codeHistory[99]; //an array of numbercode, records will be made whenever the cpu guesses
                            // if modifying this array in a function, with the object passed as a parameter, make sure to pass the object by reference to modify this.
                            //also, this array may also include invalid inputs. currently this is only used so that medium CPUs dont guess the same thing twice, regardless of validity



        CnBplayer()
        {
            fill_n(codeHistory,99,0); //fills the content of codehistory to 0 on object creation
                                        //https://stackoverflow.com/questions/1065774/initialization-of-all-elements-of-an-array-to-one-default-value-in-c
        }
};

void mainMain();
void setPlayers(int numberOfPlayers, CnBplayer player[99]);
void playGame(int numberOfPlayers, CnBplayer player[99]);
bool compareCode(int enemyCode[4], int myCode[4]);
bool inputCheck(int input, bool isCPU, int inputLength);
int  cpuGuess(int cpuDifficulty, CnBplayer &cpu); //for some reason, we dont need to pass anything by reference for setplayers(), but we need to here, because we are modifying an array within the player class.
                                                  //guess its fine to modify normal class members in functions, but if we wanna change arrays, we NEED to pass stuff by reference

int main()
{
    //-------------------------------------
    bool power = true;
    string choose;
    //-------------------------------------

    while (power == true)
    {
        try
        {
            mainMain(); //we make a "second main" so we can handle errors and loop the program.
                        //not sure if there is an easier way in c++, but in java, usually, wrong inputs can "spoil" code. putting it in a function allows us to call in a shiny new "instance", rather than try to use the unusable "spoiled" code
            cout << "\ngo again? \n1 = yes \n2/anything else = no \n";
            cin >> choose;

            if (choose != "1")
            {
                cout << "\ngoodbye";
                power = false;
            }

        }
        catch (...)
        {
            cout << "\ninvalid input.\n";
        }
    }

    return 642024;
}




void mainMain()
{
    srand(time(0)); //generates a seed for the number generator. makes it based on time so that its unique every time?

    //-------------------------------------
    string choose;
    int numberOfPlayers = 0;
    //-------------------------------------

    cout << "\n\n ===================BULLS AND COWS===================\n";

    /*
    cout << "\nhow many players? : ";//ask how many players
    cin >> choose;
    */
    choose = "2";

    numberOfPlayers = stoi(choose);


    CnBplayer player[numberOfPlayers];//if input is valid, make array of player objects
                                      //huh, it actually throws an error if the input is wrong

    player[0].isCPU = false; //sets player 0 to human by default


    setPlayers(numberOfPlayers,player);


    playGame(numberOfPlayers, player);//start game




}

void setPlayers(int numberOfPlayers, CnBplayer player[99])  //keeping the player array size to 99 introduces no issues as far as i know.
                                                            //we dont need to pass the player array by reference to modify it cuz it already is a memory address
{
    //-------------------------------------
    bool isSettingPlayers = true;
    string choose;
    string choose2;
    //-------------------------------------

    while(isSettingPlayers == true)//ask which players are cpu
    {

        for(int i = 0; i < numberOfPlayers; i++)//roll call players and their human status (loop through array)
        {
            cout << "\nplayer " << i;

            if (player[i].isCPU == true)
            {
                cout << " (CPU) (";

                switch(player[i].CPUDifficulty)
                {
                case 1 :
                    {
                        cout << "easy) ";
                        break;
                    }
                case 2 :
                    {
                        cout << "medium) ";
                        break;
                    }
                }
            }
            else
            {
                cout << " (human) ";
            }
        }

        //cout << "\n\n1 = start game \n2 = change a player to CPU/human\n3 = change CPU difficulty\n";
        cout << "\n\n1 = start game \n2 = change CPU difficulty\n";
        cin >> choose;

        /*
        if(stoi(choose) == 2)
        {
            cout << "which player to switch to CPU/human? (enter number) : ";
            cin >> choose;

            player[stoi(choose)].isCPU = !player[stoi(choose)].isCPU; //toggles the cpu status
        }

        else*/if (stoi(choose) == 2 /*3*/)
        {
            /*
            cout << "which CPU to change difficulty? (enter number) : ";
            cin >> choose;
            */
            choose = "1";

            cout << "\nwhich difficulty? \n1 = easy \n2 = medium\n";
            cin >> choose2;

            switch (stoi(choose2)) //you cant use strings with switch statements
            {
                case 1 :
                {
                    player[stoi(choose)].CPUDifficulty = 1;
                    break;
                }
                case 2 :
                {
                    player[stoi(choose)].CPUDifficulty = 2;
                    break;
                }
                default :
                {
                    player[stoi(choose)].CPUDifficulty = 2; //default to medium
                    break;
                }
            }

        }
        else
        {
            isSettingPlayers = false;
        }
    }
}

void playGame(int numberOfPlayers, CnBplayer player[99])
{
    //----------------CODE CREATION----------------
    //-------------------------------------
    int maxAttempts = 7;
    string input; //direct user input
    int inputNumber; //usually the above but converted to int. CPUs use this variable directly.
    int inputNumberArray[4]; //for passing to the compareCode and inputCheck function
    bool isInputting = true; //for dealing with potential invalid inputs
    //bool inputLoop = true;
    //-------------------------------------

    cout << "\n---------------------GAME START-----------------------\n";

    //loop through each player to search for CPUs, if a CPU, generate a random number
    //if not, ask human player to input their number code
    for(int i = 0; i < numberOfPlayers; i++)
    {
        if (player[i].isCPU == true)
        {
            isInputting = true;
            while (isInputting == true) //how fun, we can use the inputchecker to check input from the CPU too. that means the code compared to the human player is very similar (barring the string -> int conversion).
                                        //that also means with the assignment to the numbercode proper is redundantly reused. will sort that out if i have the extra time.
            {
                inputNumber = rand() % 9999 + 1; //creates the CPU's number code

                //cout << "\nplayer " << i << " is making their secret number...\n"; //temporarily removed so we dont get repeating outputs of this

                if (inputCheck(inputNumber, true, to_string(inputNumber).size() ) == true) //if input is valid
                {
                    cout <<"...player " << i << " created a code!\n";
                    isInputting = false;
                }
            }

            for(int j = 3; j > -1; j--) //assigns the number to the numbercode proper.
            {
                player[i].numberCode[j] = inputNumber % 10;

                inputNumber /= 10;
                //converting integers to an array : https://stackoverflow.com/questions/1860983/convert-integer-to-array
            }


        }
        else if (player[i].isCPU == false)
        {
            isInputting = true;
            while (isInputting == true)
            {
                cout << "\nplayer " << i << ", enter a 4 digit number for your secret code : ";
                cin >> input;

                if (inputCheck(stoi(input), false, input.size() ) == true) //if input is valid
                {
                    isInputting = false;
                }
            }


            inputNumber = stoi(input);//converts to int

                                        //assign to player's numbercode array proper
            for(int j = 3; j > -1; j--) //this for loop works in reverse due to the int -> int array conversion method working from last to first
            {
                player[i].numberCode[j] = inputNumber % 10;

                inputNumber /= 10;
                //converting integers to an array : https://stackoverflow.com/questions/1860983/convert-integer-to-array
            }
        }


    }

    /*
    cout << "\ndebug, number code menu for all players : \n";
    for (int i = 0; i < numberOfPlayers; i++)
    {
        cout << "player " << i << " : ";
        for(int j = 0; j < 4; j++)
        {
            cout << player[i].numberCode[j];
        }
        cout << "\n";
    }
    */



    //-------------------------------------------------------GAMEPLAY--------------------------------------------------------
    //---------------
    int playerNumberToFight = 0; //for now, players will "fight" against the player one number below them, with it wrapping to the last player if they are player 0

    bool didPlayerWin = false;
    //---------------


    //gameplay. loop through each player for 1 turn.
    for (int turns = 0; turns < maxAttempts; turns++)
    {

        cout << "\n-----------------TURN " << turns + 1 << "----------------------\n";

        for(int i = 0; i < numberOfPlayers; i++) //goes through each player for the one turn
        {
            if (player[i].isCPU == true)
            {
                isInputting = true;
                while (isInputting == true) //im assuming the same input conditions apply to guessing as well
                {
                    //cout << "\nplayer " << i << " guessing...\n"; //temporarily removed so we dont get repeating outputs of this

                    inputNumber = cpuGuess(player[i].CPUDifficulty, player[i]); //generates random 4 number code if its a CPU

                    if (inputCheck(inputNumber, true, to_string(inputNumber).size()) == true) //if input is valid
                    {
                        //add valid code to codeHistory, although we can pass arrays to functions to be read, we cant modify them. so we need to do it here.
                        isInputting = false;
                    }
                }

                cout << "player " << i << " guessed : " << inputNumber << "\n";
            }
            else if (player[i].isCPU == false)
            {
                isInputting = true;
                while (isInputting == true)
                {
                    cout << "\nplayer " << i << " : ";
                    cin >> input;

                    if (inputCheck(stoi(input), false, input.size() ) == true) //if input is valid
                    {
                        isInputting = false;
                    }
                }

                inputNumber = stoi(input);
            }


            for(int j = 3; j > -1; j--) //transforms the number to an array for passing
            {
                inputNumberArray[j] = inputNumber % 10;

                inputNumber /= 10;
            }


            if(i == 0) // if its player 0, target the last player
            {
                playerNumberToFight = numberOfPlayers - 1; // the -1 is there to compensate for arrays starting at 0
            }
            else //otherwise, target the person before you
            {
                playerNumberToFight = i - 1;
            }


            didPlayerWin = compareCode(player[playerNumberToFight].numberCode, inputNumberArray); //compare input with the number code of the other guy

            if (didPlayerWin == true)
            {
                cout << "\n player " << i << " wins! \n";
                turns += 9999; //gets out of the gameplay loop
                break;
                break; //these breaks prevent the next player from getting at turn after a winner happened
            }

        }
    }

    if (didPlayerWin == false)
    {
        cout << "\n no one wins! \n"; //runs if the for loop ends without anyone winning

    }


}

bool compareCode(int enemyCode[4], int myCode[4])
{
    //---------------
    int bulls = 0;
    int cows = 0;
    bool winner = false;
    //---------------


    //find bulls
    //find cows
    for (int mcn = 0; mcn < 4; mcn++)
    {
        if(myCode[mcn] == enemyCode[mcn]) //find bulls
        {
            bulls++;
        }
        else //find cows if bull finding fails by looking through the whole enemyCode
        {
            for (int ecn = 0; ecn < 4; ecn++) //https://stackoverflow.com/a/19215070
            {
                if(myCode[mcn] == enemyCode[ecn])
                {
                    cows++;
                }
            }
        }



    }


    //output results
    cout << "scored " << bulls << " Bull/s, and " << cows << " Cow/s. \n\n";


    if (bulls == 4)
    {
        winner = true;
    }
    else
    {
        winner = false;
    }

    return winner;
}


int cpuGuess(int cpuDifficulty, CnBplayer &cpu) //this will only be used in actual gameplay, not in code creation
                                                //cpu is passed as reference so we can modify the coehistory array
{
    //-------------------
    int intToReturn = 0;
    bool isGuessing = true;

    bool similarFound;
    //-------------------

    if (cpuDifficulty == 1) //easy
    {
        intToReturn = rand() % 9999 + 1; //normal rng
    }
    else if (cpuDifficulty == 2) //medium
    {
        while (isGuessing == true)
        {
            //normal rng but with no repeated guesses
            intToReturn = rand() % 9999 + 1;

            /*
            cout << "debug. enter CPU guess : ";
            cin >> intToReturn; //debug code. makes the player do the input for the CPU for testing repeated guesses.
            */

            similarFound = false;
            //cout << "checking if " << intToReturn << " was guessed before.. \n";

            //loop though code history, check to see if the guess has been made before
            for (int i = 0; i < 99; i++) //uses the length of the codehistory array
            {
                if (intToReturn == cpu.codeHistory[i])
                {
                    //cout << "CPU guessed the same thing! " << intToReturn << " and " << cpu.codeHistory[i] << "\n";
                    similarFound = true;
                }
            }


            if (similarFound == false) //if it got through the above for loop without finding a similar, its valid. add the valid code to codehistory and end the while loop.
            {
                //cout << "didnt guess that before. adding result to codehistory. \n";


                //loop through the codehistory array and add to it
                for (int i = 0; i < 99; i++) //uses the length of the codehistory array
                {
                    if (cpu.codeHistory[i] == 0) //once it finds an empty slot (0), add the valid input to the history
                    {
                        //cout << "empty slot found.s\n";

                        cpu.codeHistory[i] = intToReturn;
                                                          //i think this keeps "resetting" is because its not actually changing the array proper. (typical issues with arrays and functions)
                                                          //although we can pass objects to functions and modify their values just fine, arrays are still a thingy.
                                                          //
                                                          //oookay, thats wierd, ive passed the object to this function as reference, and now its working as intended. (appending to the array)
                                                          //not sure why i didnt need to do this for setplayers()

                        i = 200; //ends the for loop via high value
                    }
                }

                /*
                cout << "debug, current codeHistory : ";
                for (int i = 0; i < 99; i++) //uses the length of the codehistory array
                {
                    cout << cpu.codeHistory[i] << " ";
                }
                cout << "\n";
                */

                isGuessing = false; //end the while loop and return the valid int
                                    //hmm, even if its set to false, it still sometimes does another loop for some reason
                                    //wait, no. there is no problem getting out of the while loop. this can end up firing again due to invalid inputs, as we dont do inputchecking here.
                                    //this behaviour is normal.
            }


        }

    }


    return intToReturn; //the return value may not be valid. as inputchecking is done outside, in playgame(). meaning there is a good chance that this function will run multiple times.
}



bool inputCheck(int input, bool isCPU, int inputLength) //due to complications involving array decay, we find the length of the input BEFORE calling this function, and pass that value as a parameter. https://www.geeksforgeeks.org/5-different-methods-to-find-length-of-a-string-in-cpp/
{
    //----------------------
    bool isInputValid = false;
    //----------------------




    if (inputLength == 4) //if it passes that test, look for duplicates
    {
        //---------------
        int inputArray[4];
        int similarsDetected = 0;
        //---------------

        for(int j = 3; j > -1; j--) //transforms the number to an array
        {
            inputArray[j] = input % 10;

            input /= 10;
        }

        for(int i = 0; i < 4; i++) //pick a number in the array
        {
            for(int j = (i + 1); j < 4; j++) //then scan throughout the same array
                                            //"j = (i + 1)" is so that it dosent detect itself. we dont need to detect numbers "behind" the current number, because if there ARE duplicates, it wouldve detected it already.
            {
                if (inputArray[i] == inputArray[j]) //if it finds an equal
                {
                    if (isCPU == false)
                    {
                        cout << "\nsimilar detected. " << inputArray[i] << " and " << inputArray[j] << "";
                    }
                    similarsDetected++; //i increment an int value for every similar number detected, instead of simply making "isinputValid = false" here,
                                        //since i dont wanna deal with having to end/break abuncha loops and somehow later decide whether its valid or not. this is the best way i can think of.
                }
            }
        }

        if (similarsDetected == 0)
        {
            isInputValid = true; //if it manages to go through all that, mark input as valid
        }
        else
        {
            if (isCPU == false)
            {
                cout << "\nmake sure all numbers in your code are different from one another.\n";
            }
            isInputValid = false;
        }


    }
    else
    {
        if (isCPU == false)
        {
            cout << "\nnumber is an incorrect length! 4 digit numbers only.\n";
        }
        isInputValid = false;
    }



    return isInputValid;
}
