#include <iostream>
#include <math.h>
#include <random>
#include <vector>
using namespace std;

struct cell{
    int number;
    int i;
    int j;
    bool filled;
};

void printField(cell field[5][5]){
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cout << field[i][j].number << "    ";
        }
        cout << endl;
    }
    cout << endl;
}
int createNumber(){
    int randomNumber = (rand() % 2 + 1) * 2;
    return randomNumber;
}
bool isInVector(int n, int b,vector<cell>& freeCells){
    bool isInVector = false;
    for(cell k : freeCells){
        if(k.i == n && k.j == b){
            isInVector = true;
            break;
        }else{};
    }
    return isInVector;
}
void checkFreeCells(vector<cell>& freeCells){
    bool isOllFree = false;
    if (freeCells.size() > 1){
        while (!isOllFree){
            for(int i = 0; i < freeCells.size(); i++){
                if (freeCells[i].filled){
                    freeCells.erase(freeCells.begin()+i);
                    break;
                }else {
                    isOllFree = true;
                    break;
                }
            }
        }
    } else {isOllFree = true;}
}
void addFreeCells(cell field[5][5], vector<cell>& freeCells){
    for (int a = 0; a <= 4; a++){
        for(int g = 0; g <= 4; g++){
            if(field[a][g].filled){
                if(!isInVector(a,g,freeCells)){
                    freeCells.push_back(field[a][g]);
                }
            }
        }
    }
}
void addNumber(cell field[5][5], vector<cell>& freeCells){
    int newNumber = createNumber();
    int randomCell = 0;
    bool added = false;
    while(!added){
        if(freeCells.size() > 1){
            randomCell = rand() % freeCells.size();
        }
        if(!field[freeCells[randomCell].i][freeCells[randomCell].j].filled){
            field[freeCells[randomCell].i][freeCells[randomCell].j].number = newNumber;
            field[freeCells[randomCell].i][freeCells[randomCell].j].filled = true;
            checkFreeCells(freeCells);
            addFreeCells(field,freeCells);
            added = true;
        }else{
            added = false;
        }
    }
}

void moveUp(cell field[5][5], vector<cell>& freeCells){
    for (int i = 1; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (field[i][j].filled) {
                int k = i;
                while (k - 1 >= 0 && (!field[k - 1][j].filled || (field[k - 1][j].number == field[k][j].number))) {
                    if (!field[k - 1][j].filled) {
                        field[k - 1][j].number = field[k][j].number;
                        field[k - 1][j].filled = true;
                        field[k][j].number = 0;
                        field[k][j].filled = false;
                    } else if (field[k - 1][j].number == field[k][j].number) {
                        // Злиття клітин, якщо вони мають однакові значення
                        field[k - 1][j].number *= 2;
                        field[k - 1][j].filled = true;
                        field[k][j].number = 0;
                        field[k][j].filled = false;
                        break;  // Зупинка після злиття, щоб уникнути подвійного злиття
                    }
                    k--;
                }
            }
        }
    }
    checkFreeCells(freeCells);
    addFreeCells(field,freeCells);
    addNumber(field, freeCells);
    printField(field);
}
void moveDown(cell field[5][5], vector<cell>& freeCells) {
    for (int i = 3; i >= 0; i--) {
        for (int j = 4; j >= 0; j--) {
            if (field[i][j].filled) {
                int k = i;
                while (k + 1 < 5 && (!field[k + 1][j].filled || (field[k + 1][j].number == field[k][j].number))) {
                    if (!field[k + 1][j].filled) {
                        field[k + 1][j].number = field[k][j].number;
                        field[k + 1][j].filled = true;
                        field[k][j].number = 0;
                        field[k][j].filled = false;
                    } else if (field[k + 1][j].number == field[k][j].number) {
                        // Злиття клітин, якщо вони мають однакові значення
                        field[k + 1][j].number *= 2;
                        field[k + 1][j].filled = true;
                        field[k][j].number = 0;
                        field[k][j].filled = false;
                        break;  // Зупинка після злиття, щоб уникнути подвійного злиття
                    }
                    k++;
                }
            }
        }
    }
    checkFreeCells(freeCells);
    addFreeCells(field,freeCells);
    addNumber(field, freeCells);
    printField(field);
}
void moveLeft(cell field[5][5], vector<cell>& freeCells){
    for (int i = 0; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            if (field[i][j].filled) {
                int k = j;
                while (k - 1 >= 0 && (!field[i][k - 1].filled || (field[i][k - 1].number == field[i][k].number))) {
                    if (!field[i][k - 1].filled) {
                        field[i][k - 1].number = field[i][k].number;
                        field[i][k - 1].filled = true;
                        field[i][k].number = 0;
                        field[i][k].filled = false;
                    } else if (field[i][k - 1].number == field[i][k].number) {
                        // Злиття клітин, якщо вони мають однакові значення
                        field[i][k - 1].number *= 2;
                        field[i][k - 1].filled = true;
                        field[i][k].number = 0;
                        field[i][k].filled = false;
                        break;  // Зупинка після злиття, щоб уникнути подвійного злиття
                    }
                    k--;
                }
            }
        }
    }
    checkFreeCells(freeCells);
    addFreeCells(field,freeCells);
    addNumber(field, freeCells);
    printField(field);
}
void moveRight(cell field[5][5], vector<cell>& freeCells){
    for (int i = 4; i >= 0; i--) {
        for (int j = 3; j >= 0; j--) {
            if (field[i][j].filled) {
                int k = j;
                while (k + 1 < 5 && (!field[i][k + 1].filled || (field[i][k + 1].number == field[i][k].number))) {
                    if (!field[i][k + 1].filled) {
                        field[i][k + 1].number = field[i][k].number;
                        field[i][k + 1].filled = true;
                        field[i][k].number = 0;
                        field[i][k].filled = false;
                    } else if (field[i][k + 1].number == field[i][k].number) {
                        // Злиття клітин, якщо вони мають однакові значення
                        field[i][k + 1].number *= 2;
                        field[i][k + 1].filled = true;
                        field[i][k].number = 0;
                        field[i][k].filled = false;
                        break;  // Зупинка після злиття, щоб уникнути подвійного злиття
                    }
                    k++;
                }
            }
        }
    }
    checkFreeCells(freeCells);
    addFreeCells(field,freeCells);
    addNumber(field, freeCells);
    printField(field);
}
bool checkMove(cell field[5][5], vector<cell>& freeCells){
    if (freeCells.empty()){
        for (int i = 0; i < 5; i++){
            for (int j = 0; j < 5; j++){
                if ((i - 1 >= 0 && field[i][j].number == field[i - 1][j].number) ||
                    (i + 1 < 5 && field[i][j].number == field[i + 1][j].number) ||
                    (j - 1 >= 0 && field[i][j].number == field[i][j - 1].number) ||
                    (j + 1 < 5 && field[i][j].number == field[i][j + 1].number)) {
                    return true;
                }
            }
        }
        cout << "You are loser" << endl;
        return false;
    }else{
        return true;
    }
}
bool checkWin(cell field[5][5]){
    bool isWin = false;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(field[i][j].number == 2048){
                isWin = true;
            }
        }
    }
    return isWin;
}
void message(bool state){
    if(state){
        cout << "You are winner" << endl;
    }else {
        cout << "You are loser" << endl;
    }
}
void move(cell field[5][5], vector<cell>& freeCells){
    while (true) {
        string input;
        bool isWin = checkWin(field);
        bool moveIsPossible = checkMove(field,freeCells);
        if(isWin){
            message(isWin);
            break;
        }else if(moveIsPossible){
            getline(cin, input);
        }
        else {
            message(moveIsPossible);
            break;
        }

        if (input == "end") {
            break;
        }else if(input == "W" || input == "w"){
            moveUp(field,freeCells);
        }else if(input == "S" || input == "s"){
            moveDown(field,freeCells);
        }else if(input == "D" || input == "d"){
            moveRight(field,freeCells);
        }else if(input == "A" || input == "a"){
            moveLeft(field,freeCells);
        }

    }
}




void game(){
    cout << "Enter: " << endl
         <<"    W - Move Up" << endl
         <<"    S - Move Down" << endl
         <<"    A - Move Left" << endl
         <<"    D - Move Right" << endl
         <<"    end - To Quit" << endl;
    cell field[5][5];
    vector<cell> freeCells;
    for (int i = 0; i < 5; i++){
        for(int j = 0;j < 5; j++){
            field[i][j].number = 0;
            field[i][j].i = i;
            field[i][j].j = j;
            field[i][j].filled = false;
            freeCells.push_back(field[i][j]);
        }
    }
    printField(field);
    field[1][2].number = 2;
    field[1][2].filled = true;
    checkFreeCells(freeCells);
    addFreeCells(field,freeCells);
    addNumber(field, freeCells);
    printField(field);
    move(field,freeCells);
}

int main() {
    game();
    return 0;
}
