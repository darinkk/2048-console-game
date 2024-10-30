#include <iostream>
#include <math.h>
#include <random>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <cstdlib> 

using namespace std;

enum direction { UP, DOWN, LEFT, RIGHT };

struct cell{
    int number;
    int i;
    int j;
    bool filled;
    map<direction, cell*> neighbors; //Додано мап з сусідами

    bool isMovable() {
        for (const auto& p : neighbors) {
            if (p.second != nullptr) {
                if (p.second->number == 0 || this->number == p.second->number) {
                    return true;
                }
            }
        }
        return false;
    }
    bool haveWinNumber() {
        if (number == 2048) { return true; }
        else { return false; }
    }

    //Новий метод для руху клітинок
    void moveCell(direction dir) {
        if (neighbors[dir] == nullptr) {
            return;
        }
        if (!neighbors[dir]->filled) {
            neighbors[dir]->number = number;
            neighbors[dir]->filled = true;
            number = 0;
            filled = false;
            neighbors[dir]->moveCell(dir);
        }
        else if (neighbors[dir]->number == number) {
            neighbors[dir]->number *= 2;
            neighbors[dir]->filled = true;
            number = 0;
            filled = false;
            return;
        }
    }
};

void initialization(cell (&field)[5][5], vector<cell>& freeCells) {
    for (int i = 0; i < 5; i++) { 
        for (int j = 0; j < 5; j++) {
            field[i][j].number = 0;
            field[i][j].i = i;
            field[i][j].j = j;
            field[i][j].filled = false;
            freeCells.push_back(field[i][j]);

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
}
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
    return (rand() % 2 + 1) * 2;
}
bool isInVector(int n, int b,vector<cell>& freeCells){
    for(cell k : freeCells){
        if(k.i == n && k.j == b){
            return true;
            break;
        }
    }
    return false;
}
void checkFreeCells(vector<cell>& freeCells){
    freeCells.erase(remove_if(freeCells.begin(), freeCells.end(),
                              [](const cell& c) { return c.filled; }), freeCells.end());
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


/*Логіка руху тепер реалізована за допомогою класів*/
class Move { //Абстрактний класс Move
protected:
    int startRow;
    int endRow;
    int step;
    direction dir;
public:
    Move(int start, int end, int s, direction d) {
        startRow = start;
        endRow = end;
        step = s;
        dir = d;
    }
    virtual ~Move() {};
    virtual void moveCell(cell field[5][5], vector<cell>& freeCells) = 0;
};
void Move::moveCell(cell field[5][5], vector<cell>& freeCells) {
    if (dir == UP || dir == DOWN) {
        for (int i = startRow; i != endRow + step; i += step) {
            for (int j = 0; j < 5; j++) {
                if (field[i][j].filled) {
                    field[i][j].moveCell(dir);
                }
            }
        }
    }
    else {
        for (int i = 0; i < 5; i++) {
            for (int j = startRow; j != endRow + step; j += step) {
                if (field[i][j].filled) {
                    field[i][j].moveCell(dir);
                }
            }
        }
    }
    checkFreeCells(freeCells);
    addFreeCells(field, freeCells);
    addNumber(field, freeCells);
    printField(field);
}

/*
Усі класи тепер наслідуються від напряму від Move,
так як для руху у будь якому напрямку ми маємо одну універсальну функцію
класи MoveVertical/MoveHorizontal більше не потрібні.
*/
class MoveUp : public Move { 
public:
    MoveUp() : Move(1, 4, 1, UP) {}
    virtual void moveCell(cell field[5][5], vector<cell>& freeCells) override {
       Move::moveCell(field, freeCells);
    }
}; 
class MoveDown : public Move { 
public:                                
    MoveDown() : Move(3, 0, -1, DOWN) {}
    virtual void moveCell(cell field[5][5], vector<cell>& freeCells) override {
        Move::moveCell(field, freeCells);
    }
};
class MoveLeft : public Move { 
public:                                
    MoveLeft() : Move(1, 4, 1, LEFT) {}
    virtual void moveCell(cell field[5][5], vector<cell>& freeCells) override {
        Move::moveCell(field, freeCells);
    }
};
class MoveRight : public Move {
public:
    MoveRight() : Move(3, 0, -1, RIGHT) {}
    virtual void moveCell(cell field[5][5], vector<cell>& freeCells) override {
        Move::moveCell(field, freeCells);
    }
};

class MoveFactory { 
public:
    static unique_ptr<Move> createMove(string dir) {
        if (dir == "W" || dir == "w") {
            return make_unique<MoveUp>();
        }
        else if (dir == "S" || dir == "s") {
            return make_unique<MoveDown>();
        }
        else if (dir == "A" || dir == "a") {
            return make_unique<MoveLeft>();
        }
        else if (dir == "D" || dir == "d") {
            return make_unique<MoveRight>();
        }
        else {
            return nullptr;
        }
    }
};

/*Кінець логіки руху*/

bool checkMove(cell field[5][5], vector<cell>& freeCells){ 
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (field[i][j].isMovable()) {
                return true;
                break;
            }
        }
    }
    return false;
}
bool checkWin(cell field[5][5]){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(field[i][j].haveWinNumber()){
                return true;
            }
        }
    }
    return false;
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
        }
        else {
            auto m = MoveFactory::createMove(input);
            if (m == nullptr) { move(field, freeCells); }
            m->moveCell(field, freeCells);
        }
    }
}

void game(){
    cell field[5][5];
    vector<cell> freeCells;
    initialization(field, freeCells); //Додана функція ініціалізації поля (а саме клітинок у ньому)
    field[1][2].number = createNumber(); 
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