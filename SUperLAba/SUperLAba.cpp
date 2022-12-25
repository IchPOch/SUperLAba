#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cstring>
#include <map>
using namespace std;

enum rangeWeapon
{
    melee = 0,
    range = 1,
};


// Узел queue
struct Node
{
    // (x, y) представляет собой координаты ячейки матрицы, а
    // `dist` представляет их минимальное расстояние от источника
    int x, y, dist;
    Node* prevNode;
};

// Ниже массивы детализируют все четыре возможных перемещения из ячейки
int row[] = { -1, 0, 0, 1, 1, -1, 1, -1 };
int col[] = { 0, -1, 1, 0, 1, -1, -1, 1 };

// Функция проверки возможности перехода на позицию (строка, столбец)
// от текущей позиции. Функция возвращает false, если (строка, столбец)
// недопустимая позиция или имеет значение 0 или уже посещено.
bool isValid(vector<vector<int>> const& mat, vector<vector<bool>>& visited,
    int row, int col) {

    return (row >= 0 && row < mat.size()) && (col >= 0 && col < mat[0].size())
        && mat[row][col] && !visited[row][col];
}

// Находим кратчайший маршрут в матрице `mat` из источника
// ячейка (i, j) в ячейку назначения (x, y)
Node* findShortestPath(vector<vector<int>> const& mat, pair<int, int>& src,
    pair<int, int>& dest)
{
    // базовый случай: неверный ввод
    if (mat.size() == 0 || mat[src.first][src.second] == 0 ||
        mat[dest.first][dest.second] == 0) {
        return NULL;
    }

    // Матрица `M × N`
    int M = mat.size();
    int N = mat[0].size();

    // строим матрицу `M × N` для отслеживания посещенных ячеек
    vector<vector<bool>> visited;
    visited.resize(M, vector<bool>(N));

    // создаем пустую queue
    queue<Node> q;

    // получаем исходную ячейку (i, j)
    int i = src.first;
    int j = src.second;

    // помечаем исходную ячейку как посещенную и ставим исходный узел в queue
    visited[i][j] = true;
    q.push({ i, j, 0, NULL });

    // сохраняет длину самого длинного пути от источника к месту назначения
    Node min_node;
    int min_dist = INT_MAX;

    // цикл до тех пор, пока queue не станет пустой
    while (!q.empty())
    {
        // удалить передний узел из очереди и обработать его
        Node node = q.front();
        q.pop();

        // (i, j) представляет текущую ячейку, а `dist` хранит ее
        // минимальное расстояние от источника
        int i = node.x, j = node.y, dist = node.dist;

        // если пункт назначения найден, обновляем `min_dist` и останавливаемся
        if (i == dest.first && j == dest.second)
        {
            min_node = node;
            min_dist = dist;
            break;
        }

        // проверяем все четыре возможных перемещения из текущей ячейки
        // и ставим в queue каждое допустимое движение
        for (int k = 0; k < 8; k++)
        {
            // проверяем, можно ли выйти на позицию
            // (i + row[k], j + col[k]) от текущей позиции
            if (isValid(mat, visited, i + row[k], j + col[k]))
            {
                // отметить следующую ячейку как посещенную и поставить ее в queue
                visited[i + row[k]][j + col[k]] = true;
                q.push({ i + row[k], j + col[k], dist + 1, new Node({node.x, node.y, node.dist, node.prevNode}) });
            }
        }
    }

    if (min_dist != INT_MAX) {
        return &min_node;
    }

    return NULL;
}

class Weapon {
protected:
    string name;
    int addDamage;
    int maxDiceNumber;
    int numberDiceRoll;

    rangeWeapon range;
public:
    Weapon(string name, int addDamage, int maxDiceNumber, int numberDiceRoll, rangeWeapon range)
    {
        this->name = name;
        this->addDamage = addDamage;
        this->maxDiceNumber = maxDiceNumber;
        this->range = range;
    }

    int getDamage()
    {
        int n = 0;
        for (int i = 0; i < this->numberDiceRoll; i++) {
            n += 1 + rand() % this->maxDiceNumber;
        }

        return n;
    }

    string getName()
    {
        return this->name;
    }

    rangeWeapon getTypeWeapon()
    {
        return this->range;
    }
};

enum WeaponNames
{
    bite,
    claw,
    shortSword,
    axe,
    bow,
};
//Укус, 2d4 + 2, рукопашное
//Когти, 2d6 + 4, рукопашное
//Короткий меч, 1d6 + 3, рукопашное
//Секира, 1d12 + 5, рукопашное
//Длинный лук, 1d8 + 3, дальнобойное

class WeaponBuilder
{
public:
    vector<Weapon*> initWeapon(vector<WeaponNames> weaponNames)
    {
        //Классы для оружий можно сделать статикой чтобы их не создавать каждый раз
        vector<Weapon*> weapons;
        for (int i = 0; i < weaponNames.size(); i++)
        {
            Weapon* weapon = NULL;
            switch (weaponNames[i])
            {
            case bite: weapon = new Weapon("Укус", 2, 2, 2, melee); break;
            case claw: weapon = new Weapon("Когти", 4, 2, 4, melee); break;
            case shortSword: weapon = new Weapon("Короткий меч", 3, 1, 6, melee); break;
            case axe: weapon = new Weapon("Секира", 5, 1, 12, melee); break;
            case bow: weapon = new Weapon("Длинный лук", 3, 1, 8, range); break;
            }

            if (weapon != NULL)
            {
                weapons.push_back(weapon);
            }
        }

        return weapons;
    }
};

class Creature {
protected:
    int health,
        armor,
        bonusAtack,
        bonusIniciative,

        addHp,
        maxDiceNumberForHp,
        numberDiceRoll,

        speed,
        positionX,
        positionY,

        teamId;

    string name;
    vector<Weapon*> weapons;


    void initWeapon(vector<WeaponNames> weaponNames) {
        this->weapons = WeaponBuilder().initWeapon(weaponNames);
    }

    int getArmor()
    {
        return this->armor;
    }

    int initHp(int addHp, int numberDiceRoll, int maxDiceNumberForHp)
    {
        this->health = addHp;

        for (int i = 0; i < numberDiceRoll; i++) {
            this->health += 1 + rand() % maxDiceNumberForHp;
        }

        return this->health;
    }

    int getAddHp()
    {
        return addHp;
    }

    void changeHP(int value) {
        this->health -= value;
        if (this->health < 0) { die(); }
    }

    /*virtual checkEnemyInRange(int enemyX, int enemyY, 'Интерфейс с далнсотью атаки');*/

    //TODO перенсти в оружие
    //TODO Чекать врагов в округе
    bool checkEnemyInRangeWeapon(int enemyX, int enemyY, Weapon* weapon)
    {
        int diffX = abs(this->positionX - enemyX);
        int diffY = abs(this->positionY - enemyY);

        if (diffX <= 1 && diffY <= 1)
        {
            if (weapon->getTypeWeapon() == melee) {
                return true;
            }
        }
        else if (weapon->getTypeWeapon() == range) {
            return true;
        }

        return false;
    }

    bool checkArmor(int arm) {
        int d20 = 1 + rand() % 20 + this->bonusAtack;
        if (d20 >= arm) {
            return true;
        }
        else { return false; }
    }

    void die() {
        this->health = 0;
    }
public:
    Creature(string name)
    {
        this->armor = armor;
        this->bonusAtack = bonusAtack;
        this->bonusIniciative = bonusIniciative;
        this->weapons = weapons;
        this->speed = speed;
        this->positionX = positionX;
        this->positionY = positionY;
        this->name = name;
    }

    Creature(string name, int teamId) : Creature(name)
    {
        this->teamId = teamId;
    }

    int getIniciative()
    {
        return this->bonusIniciative;
    }

    void getInfo() {
        cout << endl << "-HP " << this->health << endl << "-Armor " << this->armor << endl
            << "-Bonus atack= " << this->bonusAtack << endl << "-Bonus iniciative= " << this->bonusIniciative << endl;
        for (int i = 0; i < weapons.size(); i++)
        {

            string nameWeapon = weapons[i]->getName();
            cout << "Weapon " + i << nameWeapon << endl;
        }
        cout << "-Move" << this->speed << endl;
    }

    int getTeamId()
    {
        return this->teamId;
    }

    string getName()
    {
        return this->name;
    }

    int getHp()
    {
        return this->health;
    }

    bool isAlive()
    {
        if (health <= 0) {
            return false;
        }

        return true;
    }


    pair<int, int> getCoordinate()
    {
        pair<int, int> coordinates = { this->positionX, this->positionY };
        return coordinates;
    }

    void setCoordinate(int positionX, int positionY)
    {
        cout << this->name << " перешёл на координаты " << "x - " << positionX << " y - " << positionY << std::endl;
        cout << this->name << " старые координаты " << "x - " << this->positionX << " y - " << this->positionY << std::endl;
        this->positionX = positionX;
        this->positionY = positionY;
    }

    void attack(Creature* enemy)
    {
        cout << this->name << " бьёт " << enemy->getName() << std::endl;

        if (enemy->checkArmor(enemy->getArmor())) {
            this->positionX;
            this->positionY;
            pair<int, int> enemyCoordinates = enemy->getCoordinate();
            Weapon* choosenWeapon = this->getWeaponForBitEbalo(enemyCoordinates.first, enemyCoordinates.second);

            enemy->changeHP(choosenWeapon->getDamage() + this->bonusAtack);
            cout << enemy->getName() << " получил удар:" << choosenWeapon->getDamage() + this->bonusAtack
                << " на урона" << " от " << choosenWeapon->getName()
                << " у живичка осталось хп - " << enemy->getHp() << std::endl;
        }
        else {
            cout << enemy->getName() << " увернулся от маслины" << std::endl;
        }
    }

    Weapon* getWeaponForBitEbalo(int enemyX, int enemyY)
    {
        for (int i = 0; i < this->weapons.size(); i++)
        {
            if (this->checkEnemyInRangeWeapon(enemyX, enemyY, this->weapons[i]))
            {
                return weapons[i];
            }
        }

        return NULL;
    }

    void setTeam(int i)
    {
        this->teamId = i;
    }
};

class Wolf :public Creature {
protected:
    int addHp = 2;
    int maxDiceNumberForHp = 8;
    int numberDiceRoll = 2;
    string name = "Wolf";
public:
    Wolf(string name) : Creature(name)
    {
        this->health = this->initHp(addHp, numberDiceRoll, maxDiceNumberForHp);
        this->armor = 13;
        this->bonusAtack = 4;
        this->bonusIniciative = 2;
        this->initWeapon(vector<WeaponNames>{ bite }); //Укус
        this->speed = 8;
    }

    Wolf(string name, int teamId) : Creature(name, teamId)
    {
        this->health = this->initHp(addHp, numberDiceRoll, maxDiceNumberForHp);
        this->armor = 13;
        this->bonusAtack = 4;
        this->bonusIniciative = 2;
        this->initWeapon(vector<WeaponNames>{ bite }); //Укус
        this->speed = 8;
    }
};

class Bear :public Creature {
protected:
    int addHp = 12;
    int maxDiceNumberForHp = 10;
    int numberDiceRoll = 4;
    string name = "Bear";
public:
    Bear(string name) : Creature(name)
    {
        this->health = this->initHp(addHp, numberDiceRoll, maxDiceNumberForHp);
        this->armor = 11;
        this->bonusAtack = 6;
        this->bonusIniciative = 0;
        this->initWeapon(vector<WeaponNames>{ claw }); //Когти
        this->speed = 8;
    }

    Bear(string name, int teamId) : Creature(name, teamId)
    {
        this->health = this->initHp(addHp, numberDiceRoll, maxDiceNumberForHp);
        this->health;
        this->armor = 11;
        this->bonusAtack = 6;
        this->bonusIniciative = 0;
        this->initWeapon(vector<WeaponNames>{ claw }); //Когти
        this->speed = 8;
    }
};

class Barbarian :public Creature {
protected:
    int addHp = 21;
    int maxDiceNumberForHp = 12;
    int numberDiceRoll = 2;
    string name = "Barbarian";
public:
    Barbarian(string name) : Creature(name)
    {
        this->health = this->initHp(addHp, numberDiceRoll, maxDiceNumberForHp);
        this->initWeapon(vector<WeaponNames>{ axe }); //Когти
        this->armor = 16;
        this->bonusAtack = 5;
        this->bonusIniciative = 2;
        this->speed = 8;
    }

    Barbarian(string name, int teamId) : Creature(name, teamId)
    {
        this->health = this->initHp(addHp, numberDiceRoll, maxDiceNumberForHp);
        this->initWeapon(vector<WeaponNames>{ axe }); //Когти
        this->armor = 16;
        this->bonusAtack = 5;
        this->bonusIniciative = 2;
        this->speed = 8;
    }
};

class Pathfinder :public Creature {
protected:
    int addHp = 16;
    int maxDiceNumberForHp = 10;
    int numberDiceRoll = 2;
    string name = "Pathfinder";
public:
    Pathfinder(string name) : Creature(name)
    {
        this->health = this->initHp(addHp, numberDiceRoll, maxDiceNumberForHp);
        this->armor = 15;
        this->bonusAtack = 5;
        this->bonusIniciative = 3;
        this->initWeapon(vector<WeaponNames>{ bow, shortSword }); //Когти
        this->speed = 6;
    }

    Pathfinder(string name, int teamId) : Creature(name, teamId)
    {
        this->health = this->initHp(addHp, numberDiceRoll, maxDiceNumberForHp);
        this->armor = 15;
        this->bonusAtack = 5;
        this->bonusIniciative = 3;
        this->initWeapon(vector<WeaponNames>{ bow, shortSword }); //Когти
        this->speed = 6;
    }
};

class Area {
private:
    int N;
    vector<Creature*> teamA;
    vector<Creature*> teamB;
    vector<vector<int>> map;

    void generateMap()
    {
        for (int i = 0; i < N; i++)
        {
            vector<int> mapRow;

            for (int j = 0; j < N; j++)
            {
                mapRow.push_back(1);
            }

            map.push_back(mapRow);
        }
    }

    void generatePositionForHeroes()
    {
        for (int i = 0; i < this->teamA.size(); i++)
        {
            bool isSetPosition = false;

            while (!isSetPosition)
            {
                int posX = rand() % N;
                int posY = rand() % N;

                int isPlaceTaken = map[posX][posY];
                if (isPlaceTaken == 1)
                {
                    map[posX][posY] = 0;
                    teamA[i]->setCoordinate(posX, posY);
                    isSetPosition = true;
                }
            }
        }

        for (int i = 0; i < teamB.size(); i++)
        {
            bool isSetPosition = false;

            while (!isSetPosition)
            {
                int posX = rand() % N;
                int posY = rand() % N;

                int isPlaceTaken = map[posX][posY];
                if (isPlaceTaken == 1)
                {
                    map[posX][posY] = 0;
                    teamB[i]->setCoordinate(posX, posY);
                    isSetPosition = true;
                }
            }
        }
    }

public:
    Area(int N, vector<Creature*> teamA, vector<Creature*> teamB)
    {
        this->N = N;
        this->teamA = teamA;
        this->teamB = teamB;

        generateMap();
        generatePositionForHeroes();
    }

    //TODO 
    Creature* findEnemy(Creature* hero, vector<Creature*> enemies)
    {
        pair<int, int> heroCoordinate = hero->getCoordinate();
        Creature* nearestEnemy = NULL;
        Node* nearestEnemyNode = NULL;
        int nearestRangeToEnemy = INT32_MAX;
        int index = 0;

        for (int i = 0; i < enemies.size(); i++) {
            pair<int, int> enemyCoordinate = enemies[i]->getCoordinate();

            map[enemyCoordinate.first][enemyCoordinate.second] = 1;
            map[heroCoordinate.first][heroCoordinate.second] = 1;

            Node* shortestPath = findShortestPath(map, heroCoordinate, enemyCoordinate);

            map[enemyCoordinate.first][enemyCoordinate.second] = 0;
            map[heroCoordinate.first][heroCoordinate.second] = 0;

            if (shortestPath != NULL && shortestPath->dist < nearestRangeToEnemy)
            {
                index = i;
                nearestEnemy = enemies[i];
                nearestEnemyNode = shortestPath;
            }
        }

        //Если чувак ближнего боя переместить его в врагу
        if (nearestEnemyNode != NULL && hero->getWeaponForBitEbalo(nearestEnemyNode->x, nearestEnemyNode->y) == NULL)
        {
            nearestEnemy->getCoordinate();
            hero->getCoordinate();

            pair<int, int> enemyCoordinate = enemies[index]->getCoordinate();

            map[enemyCoordinate.first][enemyCoordinate.second] = 1;
            map[heroCoordinate.first][heroCoordinate.second] = 1;

            Node* shortestPath = findShortestPath(map, heroCoordinate, enemyCoordinate);

            map[enemyCoordinate.first][enemyCoordinate.second] = 0;
            map[heroCoordinate.first][heroCoordinate.second] = 0;

            map[heroCoordinate.first][heroCoordinate.second] = 1;
            map[nearestEnemyNode->prevNode->x][nearestEnemyNode->prevNode->y] = 0;
            hero->setCoordinate(nearestEnemyNode->prevNode->x, nearestEnemyNode->prevNode->y);
        }
        else {
            //Если чувак дальнего боя 

            Creature* randEnemy = enemies[rand() % (enemies.size())];
            if (hero->getWeaponForBitEbalo(randEnemy->getCoordinate().first, randEnemy->getCoordinate().second) != NULL)
            {
                nearestEnemy = randEnemy;
            }
        }

        if (nearestEnemy == NULL)
        {
            return NULL;
        }

        return nearestEnemy;
    }

    void clearPosition(int x, int y)
    {
        this->map[x][y] = 1;
    }
};

class Game {
public:
    int round_count = 0;
    vector<Creature*> team1;
    vector<Creature*> team2;
    map<Creature*, int> listIniciative;
    Area* area;

    void StartGame() {
        InitializeGame();

        game();
    }

private:
    bool isGame = false;
    void InitializeGame()
    {
        this->team1 = vector<Creature*>{ new Bear("bear1", 1), new Bear("bear2", 1), new Wolf("wolf1", 1), new Wolf("wolf2", 1), new Wolf("wolf3", 1), new Wolf("wolf4", 1) };
        this->team2 = vector<Creature*>{ new Barbarian("barbarian1", 2), new Barbarian("barbarian2", 2), new Pathfinder("pathfinder1", 2), new Pathfinder("pathfinder2", 2) }; // 
        this->area = new Area(10, this->team1, this->team2);

        initIniciativeCreatures();
        isGame = true;

        coutInfoAboutTeam(this->team1, "команда зверей");
        coutInfoAboutTeam(this->team2, "команда людей");
        coutInfoAboutIniciative();
    }

    void initIniciativeCreatures()
    {
        for (int i = 0; i < this->team1.size(); i++) {
            this->listIniciative[this->team1[i]] = 1 + rand() % 20 + this->team1[i]->getIniciative();
        }

        for (int i = 0; i < this->team2.size(); i++) {
            this->listIniciative[this->team2[i]] = 1 + rand() % 20 + this->team2[i]->getIniciative();
        }

        //Сортировка по инициативе
        //for (int i = 0; i < this->listIniciative.size(); i++) {
        //    listIniciative
        //}
    }

    void coutInfoAboutIniciative()
    {
        map <Creature*, int> ::iterator iter = this->listIniciative.begin();

        cout << "Инициатива:" << std::endl;

        for (int i = 0; iter != this->listIniciative.end(); iter++, i++) {
            cout << "Имя: " << iter->first->getName() << " инициатива: " << iter->second << std::endl;
        }
    }

    void game()
    {
        while (isGame)
        {
            //stepCheckStatusCreatures();

            if (this->isGame == false)
            {
                return;
            }

            battle();
        }
    }

    void battle()
    {
        map <Creature*, int> ::iterator it = this->listIniciative.begin();

        for (int i = 0; it != this->listIniciative.end(); it++, i++) {
            if (it->first->isAlive() && isGame == true)
            {
                if (it->first->getTeamId() == 1) {
                    stepDamageEnemy(it->first, team2);
                }
                else {
                    stepDamageEnemy(it->first, team1);
                }
            }
        }
    }

    //TODO переписать методы определения id команды существа
    void stepCheckStatusCreatures()
    {
        map <Creature*, int> ::iterator it = this->listIniciative.begin();
        vector<Creature*> deadCreatures;

        for (int i = 0; it != this->listIniciative.end(); it++, i++) {
            it->first;
            if (!it->first->isAlive())
            {
                deadCreatures.push_back(it->first);
            }
        }

        for (int i = 0; i < deadCreatures.size(); i++) {
            if (deadCreatures[i]->getTeamId() == 1) {
                clearInfoAboutDeadCreature(deadCreatures[i], getIndexEnemy(deadCreatures[i], team1));
            }
            else {
                clearInfoAboutDeadCreature(deadCreatures[i], getIndexEnemy(deadCreatures[i], team2));
            }
        }


    }

    void clearInfoAboutDeadCreature(Creature* creature, int index)
    {
        int teamId = creature->getTeamId();

        this->listIniciative.erase(creature);
        this->area->clearPosition(creature->getCoordinate().first, creature->getCoordinate().second);

        if (teamId == 1) {
            auto iter = team1.cbegin(); // указатель на первый элемент
            this->team1.erase(iter + index);

            if (team1.empty()) {
                cout << "Человечество победило";
                coutInfoAboutTeam(team2, "Выжившие");
                gameOver();
            }
        }
        else {
            auto iter = team2.cbegin(); // указатель на первый элемент
            this->team2.erase(iter + index);

            if (team2.empty()) {
                cout << "Зверяки победили";
                coutInfoAboutTeam(team1, "Выжившие");
                gameOver();
            }
        }
    }

    void coutInfoAboutTeam(vector<Creature*> team, string title)
    {
        cout << std::endl;
        cout << title << std::endl;

        for (int i = 0; i < team.size(); i++)
        {
            cout << "Имя: " << team[i]->getName() << " " << "Хп: " << team[i]->getHp() << std::endl;
        }
    }

    void gameOver()
    {
        cout << "\nGame over!" << endl;
        isGame = false;
    }

    void stepDamageEnemy(Creature* creature, vector<Creature*> enemies)
    {
        Creature* nearestEnemy;
        int countEnemies = enemies.size();
        int index;

        nearestEnemy = this->area->findEnemy(creature, enemies);
        if (nearestEnemy != NULL)
        {
            index = getIndexEnemy(nearestEnemy, enemies);

            creature->attack(nearestEnemy);

            if (!nearestEnemy->isAlive())
            {
                this->area->clearPosition(nearestEnemy->getCoordinate().first, nearestEnemy->getCoordinate().second);
            }

            //Может работать не корректно и мб стоит это убрать
            stepCheckStatusCreatures();
        }
    }

    int getIndexEnemy(Creature* creature, vector<Creature*> enemies)
    {
        for (int i = 0; i < enemies.size(); i++)
        {
            if (enemies[i] == creature)
            {
                return i;
            }
        }
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    Game game = Game();
    game.StartGame();
    int i;
    cin >> i;
}