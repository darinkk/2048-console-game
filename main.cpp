#include <iostream>
#include <math.h>
#include <random>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <cstdlib> //Для очищення

using namespace std;

enum direction { UP, DOWN, LEFT, RIGHT };

struct cell{
    int number;
    int i;
    int j;
    bool filled;
    map<direction, cell*> neighbors; //Додано мап з сусідами
};

//Нова функція initialization
void initialization(cell (&field)[5][5], vector<cell>& freeCells) {
    for (int i = 0; i < 5; i++) { 
        for (int j = 0; j < 5; j++) {
            field[i][j].number = 0;
            field[i][j].i = i;
            field[i][j].j = j;
            field[i][j].filled = false;
            freeCells.push_back(field[i][j]);

            //field[i][j].neighbors[UP] = [i,j, &field]() {if (i - 1 >= 0) { return &field[i - 1][j]; } else { return nullptr; } };
            if (i - 1 >= 0) { field[i][j].neighbors[UP] = &field[i - 1][j]; }
            else { field[i][j].neighbors[UP] = nullptr; }

            if(i+1 <= 4){ field[i][j].neighbors[DOWN] = &field[i + 1][j]; }
            else { field[i][j].neighbors[DOWN] = nullptr; }

            if (j - 1 >= 0) { field[i][j].neighbors[LEFT] = &field[i][j - 1]; }
            else { field[i][j].neighbors[LEFT] = nullptr; }

            if (j + 1 <= 4) { field[i][j].neighbors[RIGHT] = &field[i][j + 1]; }
            else { field[i][j].neighbors[RIGHT] = nullptr; } 
        }
    }
    //Перевірка
    //cout << endl << field[1][3].neighbors["UP"]->i << field[1][3].neighbors["UP"]->j;
    //cout << endl << field[1][3].neighbors["DOWN"]->i << field[1][3].neighbors["DOWN"]->j;
    //cout << endl << field[1][3].neighbors["LEFT"]->i << field[1][3].neighbors["LEFT"]->j;
    //cout << endl << field[1][3].neighbors["RIGHT"]->i << field[1][3].neighbors["RIGHT"]->j << endl;
}

//Нова функція printRules та трохи змінена printField
void printRules() {
    cout << "Enter: " << endl
        << "    W - Move Up" << endl
        << "    S - Move Down" << endl
        << "    A - Move Left" << endl
        << "    D - Move Right" << endl
        << "    end - To Quit" << endl;
    cout << endl;
}
void printField(cell field[5][5]){
    system("cls");
    printRules();
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
        }
    }
    return isInVector;
}
void checkFreeCells(vector<cell>& freeCells){
    freeCells.erase(remove_if(freeCells.begin(), freeCells.end(),
                              [](const cell& c) { return c.filled; }), freeCells.end());
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
            if(!field[a][g].filled){
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
        }
    }
}

//Функції moveUp та moveDown об'єднані в одну moveVertical що приймає додаткову змінну типу direction
void moveVertical(cell field[5][5], vector<cell>& freeCells,direction dir) {

    int startRow = 1; //Визначаю за замовчуванням змінні для руху вгору
    int endRow = 4;
    int step = 1;

    if (dir == DOWN) { //Перевіряю чи рухаємося вниз
        startRow = 3;
        endRow = 0;
        step = -1;
    }

    for (int i = startRow; i != endRow + step; i += step) {
        for (int j = 0; j < 5; j++) {
            if (field[i][j].filled) {
                int k = i;
                while ((dir == UP ? k - 1 >= 0 : k + 1 < 5) && 
                    (!field[dir == UP ? k - 1 : k + 1][j].filled || (field[dir == UP ? k - 1 : k + 1][j].number == field[k][j].number))) {
                    if (!field[dir == UP ? k - 1 : k + 1][j].filled) {
                        field[dir == UP ? k - 1 : k + 1][j].number = field[k][j].number;
                        field[dir == UP ? k - 1 : k + 1][j].filled = true;
                        field[k][j].number = 0;
                        field[k][j].filled = false;
                    }
                    else if (field[dir == UP ? k - 1 : k + 1][j].number == field[k][j].number) {
                        field[dir == UP ? k - 1 : k + 1][j].number *= 2;
                        field[dir == UP ? k - 1 : k + 1][j].filled = true;
                        field[k][j].number = 0;
                        field[k][j].filled = false;
                        break;
                    }
                    k += (dir == UP ? -1 : 1);
                }
            }
        }
    }
    checkFreeCells(freeCells);
    addFreeCells(field, freeCells);
    addNumber(field, freeCells);
    printField(field);
}

//Функції moveLeft та moveRight об'єднані в одну moveVertical що приймає додаткову змінну типу direction
void moveHorizontal(cell field[5][5], vector<cell>& freeCells, direction dir){
    int startRow = 1; //Визначаю за замовчуванням змінні для руху вліво
    int endRow = 4;
    int step = 1;

    if (dir == RIGHT) { //Перевіряю чи рухаємося вправо
        startRow = 3;
        endRow = 0;
        step = -1;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = startRow; j != endRow + step; j += step) {
            if (field[i][j].filled) {
                int k = j;
                while ((dir == LEFT ? k - 1 >= 0 : k + 1 < 5)
                    && (!field[i][dir == LEFT ? k - 1 : k + 1].filled || (field[i][dir == LEFT ? k - 1 : k + 1].number == field[i][k].number))) {
                    if (!field[i][dir == LEFT ? k - 1 : k + 1].filled) {
                        field[i][dir == LEFT ? k - 1 : k + 1].number = field[i][k].number;
                        field[i][dir == LEFT ? k - 1 : k + 1].filled = true;
                        field[i][k].number = 0;
                        field[i][k].filled = false;
                    }
                    else if (field[i][dir == LEFT ? k - 1 : k + 1].number == field[i][k].number) {
                        field[i][dir == LEFT ? k - 1 : k + 1].number *= 2;
                        field[i][dir == LEFT ? k - 1 : k + 1].filled = true;
                        field[i][k].number = 0;
                        field[i][k].filled = false;
                        break;
                    }
                    k += (dir == LEFT ? -1 : 1);
                }
            }
        }
    }
    checkFreeCells(freeCells);
    addFreeCells(field, freeCells);
    addNumber(field, freeCells);
    printField(field);
}

//Функція checkMove виправлена і тепер перевіряє можливість ходу за допомогою cell.neighbors
bool checkMove(cell field[5][5], vector<cell>& freeCells){
    vector<direction> dir = { UP, DOWN, LEFT, RIGHT };
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (direction d : dir) {
                if (field[i][j].neighbors[d] != nullptr) {
                    if (field[i][j].neighbors[d]->number == 0 || field[i][j].neighbors[d]->number == field[i][j].number) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
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
            moveVertical(field, freeCells, UP);
            //moveUp(field,freeCells);
        }else if(input == "S" || input == "s"){
            moveVertical(field, freeCells, DOWN);
            //moveDown(field,freeCells);
        }else if(input == "D" || input == "d"){
            moveHorizontal(field, freeCells, RIGHT);
            //moveRight(field,freeCells);
        }else if(input == "A" || input == "a"){
            moveHorizontal(field, freeCells, LEFT);
            //moveLeft(field,freeCells);
        }
    }

}


void game(){
    cell field[5][5];
    vector<cell> freeCells;
    initialization(field, freeCells); //Додана функція ініціалізації поля (а саме клітинок у ньому)
    field[1][2].number = 2; //Перше значення завжди в одному й тому ж місці і завжди 2 (спробуй виправити)
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
