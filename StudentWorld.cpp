#include "StudentWorld.h"
#include "GameController.h"
#include <string>
#include <queue>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::~StudentWorld(){
    for (int x = 0; x < 64; x++){
        for (int y = 0; y < 60; y++){
            if (m_earthArr[x][y] != nullptr) {
                delete m_earthArr[x][y];
            }

            m_earthArr[x][y] = nullptr;
        }
    }

    if (m_tunnelman != nullptr) delete m_tunnelman;
    m_tunnelman = nullptr;

    size_t s = m_gameObjects.size();
    for (int i = 0; i < s; i++){
        if (m_gameObjects[i] != nullptr) delete m_gameObjects[i];
        m_gameObjects[i] = nullptr;

    }
    
    std::vector<Actor*>::iterator it = m_gameObjects.begin();
    while (it != m_gameObjects.end()){
        it = m_gameObjects.erase(it);
    }
}


int StudentWorld::init(){
    fillEarth();
    
    TunnelMan* temp_tm = new TunnelMan(this);
    m_tunnelman = temp_tm;
    distributeBoulders();
    distributeOil();
    distributeGold();    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    setDisplayText();
    
    
    // MOVEMENT
    size_t s = m_gameObjects.size();
    for (int i = 0; i < s; i++){
        m_gameObjects[i]->doSomething();
    }
    
    m_tunnelman->doSomething();
    
    
    // NEW ACTORS
    
    trySpawningGoodies();
    trySpawningProtestors();
    
    
    // DEATHS
    std::vector<Actor*>::iterator it = m_gameObjects.begin();
    for (int i = 0; i < m_gameObjects.size(); i++){
        if (!(m_gameObjects[i]->isAlive())) {
            
            
            if(m_gameObjects[i]->getID() == TID_PROTESTER || m_gameObjects[i]->getID() == TID_HARD_CORE_PROTESTER) m_nProtestors--;
            
            delete m_gameObjects[i];
            m_gameObjects.erase(it + i); // maybe change this to a list for easier deletion
            i--; // sus
        }
    }
    
    if(!(m_tunnelman->isAlive())){
        playSound(SOUND_PLAYER_GIVE_UP);
        decLives();
        
        m_nProtestors = 0;
        m_protestor_time = 0;
        
        return GWSTATUS_PLAYER_DIED;
    }
        
    int level = getLevel();
    int totalOil = std::min(2 + level, 21);
    
    if (m_tunnelman->getOil() == totalOil){
        playSound(SOUND_FINISHED_LEVEL);
        
        m_nProtestors = 0;
        m_protestor_time = 0;
        
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    else return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    
    for (int x = 0; x < 64; x++){
        for (int y = 0; y < 60; y++){
            if (m_earthArr[x][y] != nullptr) {
                delete m_earthArr[x][y];
            }

            m_earthArr[x][y] = nullptr;
        }
    }
    
    
    

    if (m_tunnelman != nullptr) delete m_tunnelman;
    m_tunnelman = nullptr;

    size_t s = m_gameObjects.size();
    for (int i = 0; i < s; i++){
        if (m_gameObjects[i] != nullptr) delete m_gameObjects[i];
        m_gameObjects[i] = nullptr;

    }
    
    std::vector<Actor*>::iterator it = m_gameObjects.begin();
    while (it != m_gameObjects.end()){
        it = m_gameObjects.erase(it);
    }
    
}

std::string StudentWorld::formatter(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score){
    
    ostringstream oss;
    oss << "Lvl: " << setw(2) << level << " Lives: " << setw(1) << lives << " Hlth: " << setw(1) << health << setw(1) << "%" << " Wtr: " << setw(2) << squirts << " Gld: " << setw(1) << gold << " Oil Left: " << setw(1) << barrelsLeft << " Sonar: " << setw(1) << sonar << " Scr: " << setfill('0') <<setw(6) << score;
    return oss.str();
    
}

void StudentWorld::setDisplayText(){
    int level = getLevel();
    int lives = getLives();
    int health = m_tunnelman->getHealth() * 10;
    int squirts = m_tunnelman->getSquirts();
    int gold = m_tunnelman->getGold();

    int totalOil = std::min(2 + level, 21);
    int barrelsLeft = totalOil - m_tunnelman->getOil();
    int sonar = m_tunnelman->getSonar();
    int score = getScore();
    
    // need to change this formattign according to rules
    string s = formatter(level, lives, health, squirts, gold, barrelsLeft, sonar, score);
    
    setGameStatText(s);
}

void StudentWorld::fillEarth(){
    
    for (int i = 0; i < 64; i++){
        for (int j = 0; j < 60; j++){
            Earth* temp = new Earth(i, j);
            m_earthArr[i][j] = temp;
        }
        for (int j = 60; j < 64; j++){
            m_earthArr[i][j] = nullptr;
        }
    }
    
    for (int i = 30; i < 34; i++){
        for (int j = 4; j < 60; j++){
            delete m_earthArr[i][j];
            m_earthArr[i][j] = nullptr;
        }
    }
    
}

bool StudentWorld::hasEarth(int x, int y){
    
    return m_earthArr[x][y];
}

bool StudentWorld::hasBoulder(int x, int y){
    
    for (int i = 0; i < m_gameObjects.size(); i++){
        if (m_gameObjects[i]->getID() == TID_BOULDER){
            int bx = m_gameObjects[i]->getX();
            int by = m_gameObjects[i]->getY();
            
            if ((x<bx + 4 && x>bx - 4) && (y<by+4 && y>by - 4)) return true;

        }
    }
    
    return false;
    
}

bool StudentWorld::nearBoulder(int x, int y){
            
            size_t s = m_gameObjects.size();
            for (int i = 0; i < s; i++){
                if (m_gameObjects[i]->getID() == TID_BOULDER &&(dist(m_gameObjects[i]->getX(), x, m_gameObjects[i]->getY(), y) <= 3.0)) return true;
            }
    
    return false;
}

int StudentWorld::getTX() const{
    return m_tunnelman->getX();
}
int StudentWorld::getTY() const{
    return m_tunnelman->getY();
}

void StudentWorld::hurtTM(int amt){
    for (int i = 0; i < amt; i++){
        m_tunnelman->decHealth();
    }
}

void StudentWorld::addObject(Actor* actr_ptr){
    m_gameObjects.push_back(actr_ptr);
}

void StudentWorld::dig(int x, int y){
    if (m_earthArr[x][y]!=nullptr){
        delete m_earthArr[x][y];
    } m_earthArr[x][y] = nullptr;
}

void StudentWorld::distributeBoulders(){
    int level = getLevel();
    int B = std::min(level/2+2, 9);
    
//    B = 20;
    
    
    // REMOVE THIS
//    Boulder* test = new Boulder(30, 56, this);
//    m_gameObjects.push_back(test);
    
//    srand(static_cast<unsigned int>(time(nullptr)));
    
    int count = 0;
    
    while (count < B){
        int rx = rand()%59 + 1;
        int ry = rand()%36 + 20;
        
        if (canPlaceObject(rx, ry)){
            if (coveredInEarth(rx, ry)){ // need to fix edge tunnel case
        
                Boulder* boulder_ptr = new Boulder(rx, ry, this);
                m_gameObjects.push_back(boulder_ptr);
                
                // clear surrounding earth
                for (int dx = 0; dx < 4; dx++){
                    for (int dy = 0; dy < 4; dy++){
                        if (m_earthArr[rx+dx][ry+dy] != nullptr) {
                            delete m_earthArr[rx+dx][ry+dy];
                        }
                        
                        m_earthArr[rx+dx][ry+dy] = nullptr;
                    }
                }
                
                count++;
            }
        }
    }
}

void StudentWorld::distributeOil(){
    int level = getLevel();
    int L = std::min(2 + level, 21);
    
//    L = 40;

//    srand(static_cast<unsigned int>(time(nullptr)));
    
    int count = 0;
    
    while (count < L){
        int rx = rand()%61;
        int ry = rand()%57;
        
        if (canPlaceObject(rx, ry)){
            if (coveredInEarth(rx, ry)){
            
                Barrel* barrel_ptr = new Barrel(rx, ry, this);
                m_gameObjects.push_back(barrel_ptr);
                
                count++;
                
            }
        }
    }
}

void StudentWorld::distributeGold(){
    int level = getLevel();
    int G = std::max(5 - level/2, 2);
    
//    L = 40;

//    srand(static_cast<unsigned int>(time(nullptr)));
    
    int count = 0;
    
    while (count < G){
        int rx = rand()%61;
        int ry = rand()%57;
        
        if (canPlaceObject(rx, ry)){
            if (coveredInEarth(rx, ry)){
            
                GoldNugget* gold_ptr = new GoldNugget(rx, ry, false, true, true, this);
                m_gameObjects.push_back(gold_ptr);
                
                count++;
                
            }
        }
    }
}

void StudentWorld::trySpawningGoodies(){
    int level = getLevel();
    int G = level*25 + 300; // this seems too fast ...

    int p = rand()%G;
    
    if (p == 0){
        
        
        int r = rand()%5;
        
        if (r == 0){
        Sonar* sonar_ptr = new Sonar(0, 60, this);
        m_gameObjects.push_back(sonar_ptr);
        }
        
        else {
            
            int x;
            int y;
            findRandomEarthlessSpot(x, y);
            Water* water_ptr = new Water(x, y, this);
            m_gameObjects.push_back(water_ptr);
        }
    }
    
    
}

void StudentWorld::trySpawningProtestors(){
    
    int level = getLevel();
    int T = std::max(25, 200 - level);
    
    int p = 2 + level*1.5;
    
    int P = std::min(15, p);
//    int P = 3;
    
    if (m_protestor_time == 0){
        if (m_nProtestors != P){
            
            int r = rand()%53+8;
            
            int probabilityOfHardcore = std::min(90, level*10 + 30);
            
            int n = probabilityOfHardcore/10; // say its 3, then 3/10 should spawn hardcore
            int r2 = rand()%9; // 0 to 9 result
            
            bool hardcore = false;
            
            if (r2 < n) hardcore = true; // r2 needs to be 0, 1 or 2 so 3 out of possible 10, so this works
            
            
            if (hardcore){
                Protestor* temp_hard_prot = new HardcoreProtestor(r, this);
                m_gameObjects.push_back(temp_hard_prot);
            }
            else {
                Protestor* temp_prot = new Protestor(r, this, TID_PROTESTER);
                m_gameObjects.push_back(temp_prot);
            }
            m_nProtestors++;
        }
    }
    
    m_protestor_time++;
    
    if (m_protestor_time == T) m_protestor_time = 0;
    
}

void StudentWorld::revealViaSonar(int x, int y){
    size_t s = m_gameObjects.size();
    for (int i = 0; i < s; i++){
        if (dist(x,m_gameObjects[i]->getX(),y, m_gameObjects[i]->getY()) <= 12.0) m_gameObjects[i]->setVisible(true);
    }
}

double StudentWorld::dist(int x1, int x2, int y1, int y2){
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

bool StudentWorld::canPlaceObject(int x, int y){
    size_t s = m_gameObjects.size();
    for (int i = 0; i < s; i++){
        if (dist(x,m_gameObjects[i]->getX(),y, m_gameObjects[i]->getY()) <= 6.0) return false;
    }
    
    return true;
}

bool StudentWorld::coveredInEarth(int x, int y){
    for (int dx = 0; dx < 4; dx++){
        for (int dy = 0; dy < 4; dy++){
            if (!hasEarth(x+dx,y+dy)) {
                return false;
            }
        }
    }
    
    return true;
}

bool StudentWorld::hasAnyEarth(int x, int y){
    for (int dx = 0; dx < 4; dx++){
        for (int dy = 0; dy < 4; dy++){
            if (hasEarth(x+dx,y+dy)) {
                return true;
            }
        }
    }
    
    return false;
}

bool StudentWorld::bribeNearbyProtestor(int x, int y){
    
    size_t s = m_gameObjects.size();
    
    for (int i = 0; i < s; i++){
        if ((m_gameObjects[i]->getID() == TID_PROTESTER || m_gameObjects[i]->getID() == TID_HARD_CORE_PROTESTER) &&(dist(m_gameObjects[i]->getX(), x, m_gameObjects[i]->getY(), y) <= 3.0)){
            
                m_gameObjects[i]->setDead();
                return true; // can only bribe once
            
            }
        }
    
    return false;
    
}

    

void StudentWorld::annoyNearbyProtestors(int x, int y, bool &annoyed_any, int amt, int points){
    
    size_t s = m_gameObjects.size();
    
    annoyed_any = false;
    
    for (int i = 0; i < s; i++){
        if ((m_gameObjects[i]->getID() == TID_PROTESTER || m_gameObjects[i]->getID() == TID_HARD_CORE_PROTESTER)  &&(dist(m_gameObjects[i]->getX(), x, m_gameObjects[i]->getY(), y) <= 3.0)){
            
            if (m_gameObjects[i]->getID() == TID_HARD_CORE_PROTESTER && amt == 2){
                m_gameObjects[i]->getAnnoyed(amt, 2.5*points);
            }
            else m_gameObjects[i]->getAnnoyed(amt, points);
            
            annoyed_any = true;
        }
    }
    
}

void StudentWorld::annoyNearbyTunnelMan(int x, int y, int amt){
    
    if (dist(x, getTX(), y, getTY()) <= 3.0) m_tunnelman->getAnnoyed(amt, 0);
}

void StudentWorld::findRandomEarthlessSpot(int &x, int &y){

    
    bool found = false;
    int rx = -999;
    int ry = -999; // avoiding "may not be initialized" warnings
    
    while(!found){
    
        rx = rand()%61;
        ry = rand()%61;
        
        bool noEarth = true;
        
            for (int dx = 0; dx < 4; dx++){
                for (int dy = 0; dy < 4; dy++){
                    if (hasEarth(rx+dx,ry+dy)) {
                        noEarth = false;
                        break;
                    }
                } if (!noEarth) break;
            }
        
        if (noEarth) found = true;
        
        if (!canPlaceObject(rx, ry)) found = false;
        
    }
    
    x = rx;
    y = ry;
    
    
}



