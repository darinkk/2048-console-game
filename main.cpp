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

//Нова функція initialization
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
public:
    //direction dir;
    virtual ~Move() {};
    virtual void moveCell(cell field[5][5], vector<cell>& freeCells) = 0;
};

class MoveVertical : public Move { //Абстрактний класс MoveVertical що наслідує Move
private:
    direction dir;
    int startRow; 
    int endRow;
    int step;
public:
    MoveVertical(direction d, int start, int end, int s) {
        dir = d;
        startRow = start;
        endRow = end;
        step = s;
    }
    void moveCell(cell field[5][5], vector<cell>& freeCells) = 0;
};
//Визначаємо функцію moveCell за межами классу як функцію за замовчуванням для MoveVertical. 
//Таким чином клас MoveVertical залишиться абстрактним і його елемент не можна буде створити, а у класах нащадках ми зможено не думблювати
//велику частину коду яка ідентична для MoveUp та MoveDown
void MoveVertical::moveCell(cell field[5][5], vector<cell>& freeCells) {
    for (int i = startRow; i != endRow + step; i += step) {
        for (int j = 0; j < 5; j++) {
            if (field[i][j].filled) {
                field[i][j].moveCell(dir);
            }
        }
    }
    checkFreeCells(freeCells);
    addFreeCells(field, freeCells);
    addNumber(field, freeCells);
    printField(field);
}    

class MoveHorizontal : public Move { //Абстрактний класс MoveVertical що наслідує Move
 private:
     direction dir;
     int startRow;
     int endRow;
     int step;
 public:
     MoveHorizontal(direction d, int start, int end, int s) {
         dir = d;
         startRow = start;
         endRow = end;
         step = s;
     }
     void moveCell(cell field[5][5], vector<cell>& freeCells) = 0;
 };
 //Так само визначаємо функцію за замовчуванням для MoveHorizontal
void MoveHorizontal::moveCell(cell field[5][5], vector<cell>& freeCells) {
     for (int i = 0; i < 5; i++) {
         for (int j = startRow; j != endRow + step; j += step) {
             if (field[i][j].filled) {
                 field[i][j].moveCell(dir);
             }
         }
     }
     checkFreeCells(freeCells);
     addFreeCells(field, freeCells);
     addNumber(field, freeCells);
     printField(field);
 }

class MoveUp : public MoveVertical { //Клас MoveUp, що наслідується від MoveVertical та перевизначає функцію moveCell як функцію за замвчуванням для MoveVertical
public:
    MoveUp() : MoveVertical(UP, 1, 4, 1) {}
    virtual void moveCell(cell field[5][5], vector<cell>& freeCells) override {
       MoveVertical::moveCell(field, freeCells);
    }
};
class MoveDown : public MoveVertical { //Клас MoveDown, що наслідується від MoveVertical та перевизначає функцію moveCell як функцію за замвчуванням для MoveVertical
public:                                //різниця від MoveUp тут тільки у ініціалізації значень
    MoveDown() : MoveVertical(DOWN, 3, 0, -1) {}
    virtual void moveCell(cell field[5][5], vector<cell>& freeCells) override {
        MoveVertical::moveCell(field, freeCells);
    }
};
//Класи MoveLeft та MoveRight аналогічні до MoveUp/MoveDown тільки наслідуються від MoveHorizontal, що має іншу реалізацію moveCell
class MoveLeft : public MoveHorizontal { 
public:                                
    MoveLeft() : MoveHorizontal(LEFT, 1, 4, 1) {}
    virtual void moveCell(cell field[5][5], vector<cell>& freeCells) override {
        MoveHorizontal::moveCell(field, freeCells);
    }
};
class MoveRight : public MoveHorizontal {
public:
    MoveRight() : MoveHorizontal(RIGHT, 3, 0, -1) {}
    virtual void moveCell(cell field[5][5], vector<cell>& freeCells) override {
        MoveHorizontal::moveCell(field, freeCells);
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

//Функція checkMove виправлена і тепер перевіряє можливість ходу за допомогою cell.neighbors та методу клітинки isMovable
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

//Функція checkWin також тепер використовує метод клітинки haveWinNumber
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