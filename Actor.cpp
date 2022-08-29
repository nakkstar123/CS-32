#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
#include <queue>
#include <stack>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
void TunnelMan::tryMove(Direction dir){
    
    int tx = getX();
    int ty = getY();
    switch (dir){
        case left:
            if (tx != 0 && !getWorld()->hasBoulder(tx-1, ty)) moveTo(tx-1, ty);
            break;
        case right:
            if (tx != 60 && !getWorld()->hasBoulder(tx+1, ty)) moveTo(tx+1, ty);
            break;
        case down:
            if (ty != 0 && !getWorld()->hasBoulder(tx, ty - 1)) moveTo(tx, ty-1);
            break;
        case up:
            if (ty != 60 && !getWorld()->hasBoulder(tx, ty+1)) moveTo(tx, ty+1);
            break;
        default:
            break;
            
    }
    
}

void TunnelMan::doSomething(){
    if (!isAlive()) return;
    int tx = getX();
    int ty = getY();
    
    bool playSound = false;
    
    for (int dx = 0; dx < 4; dx++){
        for (int dy = 0; dy < 4; dy++){
            if (getWorld()->hasEarth(tx+dx, ty+dy)){
                getWorld()->dig(tx+dx, ty+dy);
                playSound = true;
            }
        }
    }
    
    if (playSound) getWorld()->playSound(SOUND_DIG);
    
    int ch;
    Direction currDir = getDirection();
    if (getWorld()->getKey(ch) == true) // user hit a key this tick!
    {
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                if (currDir == left) tryMove(left);
                else setDirection(left);
                break;
                         
            case KEY_PRESS_RIGHT:
                if (currDir == right) tryMove(right);
                else setDirection(right);
                break;
                
            case KEY_PRESS_DOWN:
                if (currDir == down) tryMove(down);
                else setDirection(down);
                break;

            case KEY_PRESS_UP:
                if (currDir == up) tryMove(up);
                else setDirection(up);
                break;
                
            case KEY_PRESS_ESCAPE:
                m_hp = 0;
                break;
                
            case 'z':
            case 'Z':{
                
                if (m_nSonar == 0) return;
                m_nSonar--;
                
                
                getWorld()->revealViaSonar(tx, ty);
                
                break;
            }
                
            case KEY_PRESS_TAB:
            {
                if (m_nGold != 0){
                    GoldNugget* gld_ptr = new GoldNugget(getX(), getY(), true, false, false, getWorld());
                    getWorld()->addObject(gld_ptr);
                    m_nGold--;
                }
                break;
            }
                
            case KEY_PRESS_SPACE:
            {
                if (m_nSquirts == 0) return;
                m_nSquirts--;
                
                int newX = 0;
                int newY = 0;
                
                switch (currDir) {
                    case up:
                        newX = getX();
                        newY = getY() + 4;
                        break;
                    case down:
                        newX = getX();
                        newY = getY() - 4;
                        break;
                    case left:
                        newX = getX() - 4;
                        newY = getY();
                        break;
                    case right:
                        newX = getX() + 4;
                        newY = getY();
                        break;
                        
                    default:
                        break;
                }
                
                bool canSquirt = true;
                
                if (getWorld()->hasEarth(newX, newY)) canSquirt = false;
                if (getWorld()->hasBoulder(newX, newY)) canSquirt = false; // actually needs to check for 3 unit radius from boulder
                
                if (canSquirt){
                    Squirt* sqrt_ptr = new Squirt(newX, newY, currDir, getWorld());
                    getWorld()->addObject(sqrt_ptr);
                }
                
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                
                
                
            }
                break;
            default:
                break;
                
        }
        
    }
    
    
}

bool Boulder::isAlive(){
    return (m_state != 3);
}

void Boulder::doSomething(){
    if (m_state == 3) return; // do nothing if it's dead
    
    
    else if (m_state == 0){
        
        bool canFall = true;
        
        int bx = getX();
        int by = getY();
        
        for (int i = 0; i < 4; i++){
            if ((getWorld()->hasEarth(bx+i, by-1))) canFall = false;
        }
        
        if (canFall) m_state = 1;
        
    }
    
    else if (m_state == 1){
        
        if (m_wait == 0) m_state = 2;
        else m_wait--;
        
    }
    
    else if (m_state == 2){
        
        bool canFall = true;
        int bx = getX();
        int by = getY();
        for (int i = 0; i < 4; i++){
            if ((getWorld()->hasEarth(bx+i, by-1))) canFall = false;
        }
        
        if (by == 0){
            m_state = 3;
            return;
        }
        
        
        if (canFall){
            moveTo(bx, by-1);
            GameController::getInstance().playSound(SOUND_FALLING_ROCK);
            
            bool dont_need = false;
            getWorld()->annoyNearbyProtestors(getX(), getY(), dont_need, 100, 500);
            getWorld()->annoyNearbyTunnelMan(getX(), getY(), 100);
        }
        
        else{
            m_state = 3;
            return;
        }
        
        // implement tunnelman/protestor annoyance
        
        
    }
    
}

bool Squirt::isAlive(){
    return (m_dist > 0);
}

bool Squirt::canMove(){
    
    Direction curr = getDirection();
    int x = getX();
    int y = getY();
    
    StudentWorld* this_world = getWorld(); // don't really need a new variable here...
    
    switch (curr) {
        case up:
            
            if (y == 64 || this_world->hasEarth(x, y+1) || this_world->hasBoulder(x, y+1))
            
                return false;
            
            break;
        case down:
            
            if (y == 0 || this_world->hasEarth(x, y-1) || this_world->hasBoulder(x, y-1))
            
                return false;
            break;
            
        case right:
            
            if (x == 64 || this_world->hasEarth(x+1, y) || this_world->hasBoulder(x+1, y))
            
                return false;
            break;
        case left:
            
            if (x == 0 || this_world->hasEarth(x-1, y) || this_world->hasBoulder(x-1, y))
            
                return false;
            break;
        default:
            break;
            
    }
    
    return true;
}

void Squirt::doSomething(){
    
    if (!isAlive()) return;
    
    bool annoyed_protestor = false;
    getWorld()->annoyNearbyProtestors(getX(), getY(), annoyed_protestor, 2, 100);
    
    if (annoyed_protestor) m_dist = 0;
    
    if (!canMove()) {
        m_dist = 0;
    }
    
    
    Direction curr = getDirection();
    int x = getX();
    int y = getY();
    // abstract to a moveInCurrDir function??
    
    switch (curr) {
        case up:
            moveTo(x, y+1);
            break;
        
        case down:
            moveTo(x, y-1);
            break;
            
        case right:
            moveTo(x+1, y);
            break;
            
        case left:
            moveTo(x-1, y);
            break;
            
        default:
            break;
    }

    m_dist--;
    
    }

bool Barrel::nearTunnelMan(double dist){
    
    int tx = getWorld()->getTX();
    int ty = getWorld()->getTY();
    
    if (getWorld()->dist(getX(),tx,getY(), ty) <= dist) return true;
    
    return false;
}

void Barrel::doSomething(){
    if (!isAlive()) return;
    if (!isVisible()){
        if (nearTunnelMan(4.0)) {
            setVisible(true);
            return;
        }
    }
    
    else {
        if (nearTunnelMan(3.0)){
            m_alive = false;
            getWorld()->playSound(SOUND_FOUND_OIL);
            getWorld()->increaseScore(1000);
            getWorld()->collectOil();
            
            // If necessary, the Barrel may inform the StudentWorld object that it was picked up. Once all Barrels on the level have been picked up, the player finishes the level and may advance to the next level.
            
        }
    }
    
}

bool GoldNugget::nearTunnelMan(double dist){
    
    int tx = getWorld()->getTX();
    int ty = getWorld()->getTY();
    
    if (getWorld()->dist(getX(),tx,getY(), ty) <= dist) return true;
    
    return false;
}

void GoldNugget::doSomething(){
    if (!isAlive()) return;
    if (!isVisible()){
        if (nearTunnelMan(4.0)) {
            setVisible(true);
            return;
        }
    }
    
    else {
        if (nearTunnelMan(3.0) && m_pick){
            m_alive = false;
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseScore(10);
            getWorld()->collectGold();
            
            // Implement pickup-able by protestors
            
        }
        
        if (!m_pick && getWorld()->bribeNearbyProtestor(getX(), getY())){
            m_alive = false;
        }
    }
    
    if (!m_perm){
        if (m_wait == 0) m_alive = false;
        else m_wait--;
    }
    
}

Sonar::Sonar(int x, int y, StudentWorld* world_ptr) : Actor(TID_SONAR, x, y, right, 1.0, 2, world_ptr){
    
    setVisible(true);
    
    m_alive = true;
    
    int level = getWorld()->getLevel();
    m_wait = std::max(100, 300 - 10*level);
    
    // only pickup-able by tunnelman again
}

bool Sonar::nearTunnelMan(double dist){
    
    int tx = getWorld()->getTX();
    int ty = getWorld()->getTY();
    
    if (getWorld()->dist(getX(),tx,getY(), ty) <= dist) return true;
    
    return false;
}

void Sonar::doSomething(){
    
    if (!isAlive()) return;
    
    if (nearTunnelMan(3.0)){
        m_alive = false;
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->collectSonar();
        getWorld()->increaseScore(75);
        
    }
    
    m_wait--;
    
    if (m_wait <= 0) m_alive = false;
    
}

Water::Water(int x, int y, StudentWorld* world_ptr): Actor(TID_WATER_POOL, x, y, right, 1.0, 2, world_ptr){
    
    setVisible(true);
    
    m_alive = true;
    
    int level = getWorld()->getLevel();
    m_wait = std::max(100, 300 - 10*level);
    
    // only pickup-able by tunnelman again
    
}

bool Water::nearTunnelMan(double dist){
    
    int tx = getWorld()->getTX();
    int ty = getWorld()->getTY();
    
    if (getWorld()->dist(getX(),tx,getY(), ty) <= dist) return true;
    
    return false;
}

void Water::doSomething(){
    
    if (!m_alive) return;
    
    if (nearTunnelMan(3.0)){
        m_alive = false;
        getWorld()->playSound(SOUND_GOT_GOODIE);
        
        for (int i = 0; i < 5; i++) getWorld()->collectSquirt(); // giving too many squirts?
        
        getWorld()->increaseScore(100);
        
        // Implement pickup-able by protestors
    }
    
    m_wait--;
    
    if (m_wait <= 0) m_alive = false;
    
    // implement temporary state, lifetime
    
}

Protestor::Protestor(int numSquaresToMove, StudentWorld* world_ptr, int TID) : Actor(TID, 60, 60, left, 1.0, 0, world_ptr){
    setVisible(true);
    m_alive = true;
    m_numSquaresToMove = numSquaresToMove; // need to be between 8 and 60 inclusive?
    
    if (TID == TID_PROTESTER){
        isHardcore = false;
        m_hp = 5;
    }
    else {
        isHardcore = true; 
        m_hp = 20;
    }
    
    
    leave = false;
    last_shout = 15;
    
    int level = getWorld()->getLevel();
    
    max_wait = std::max(0, 3 - level/4);
    
    m_wait = max_wait;
    
    m_perp = 0;
    
    made_exit_stack = false;
    
}

bool Protestor::nearTunnelMan(double dist){
    
    int tx = getWorld()->getTX();
    int ty = getWorld()->getTY();
    
    if (getWorld()->dist(getX(),tx,getY(), ty) <= dist) return true;
    
    return false;
}

bool Protestor::isFacingTunnelMan(){
    int tx = getWorld()->getTX();
    int ty = getWorld()->getTY();
    
    Direction currDir = getDirection();
    
    switch (currDir) {
        case up:
            if (ty - getY() >= 0) return true;
            break;
            
        case down:
            if (ty - getY() <= 0) return true;
            break;
            
        case left:
            if (tx - getX() <= 0) return true;
            break;
            
        case right:
            if (tx - getX() >= 0) return true;
            break;
            
        default:
            break;
    }
    
    return false;
    
}

bool Protestor::canReachTunnelMan(){
    
    int tx = getWorld()->getTX();
    int ty = getWorld()->getTY();
    
    if (getY() != ty && getX() != tx) return false;
    
    if (nearTunnelMan(4.0)) return false;
    
    if (getX() == tx){
        int y = std::min(ty, getY());
        int Y = std::max(ty, getY());
        for (int i = y; i < Y; i++){
            if (getWorld()->hasAnyEarth(tx, i) || getWorld()->nearBoulder(tx, i)) return false;
        }
    }
    
    else if (getY() == ty){
        int x = std::min(tx, getX());
        int X = std::max(tx, getX());
        for (int i = x; i < X; i++){
            if (getWorld()->hasAnyEarth(i, ty) || getWorld()->nearBoulder(i, ty)) return false;
        }
    }
    
    return true;
}

void Protestor::moveInCurrentDir(){
    
    int x = getX();
    int y = getY();
    Direction curr = getDirection();
        switch (curr){
            case left:
                if (x != 0) moveTo(x-1, y);
                break;
            case right:
                if (x != 60) moveTo(x+1, y);
                break;
            case down:
                if (y != 0) moveTo(x, y-1);
                break;
            case up:
                if (y != 60) moveTo(x, y+1);
                break;
            default:
                break;
                
        }
        

}

void Protestor::moveInDir(Actor::Direction dir){
    
    int x = getX();
    int y = getY();
    
        switch (dir){
            case left:
                if (x != 0) moveTo(x-1, y);
                break;
            case right:
                if (x != 60) moveTo(x+1, y);
                break;
            case down:
                if (y != 0) moveTo(x, y-1);
                break;
            case up:
                if (y != 60) moveTo(x, y+1);
                break;
            default:
                break;
                
        }
        

}

Actor::Direction Protestor::pickRandomDir(){

    bool valid = false;
    Direction dir = getDirection(); // to avoid unitialized error
    
    while (!valid){
    
        int r = rand()%4;
        
        switch (r) {
            case 0:
                
                if (!getWorld()->hasAnyEarth(getX(), getY() + 1) && !getWorld()->nearBoulder(getX(), getY() + 1) && getY() != 60){
                    dir = up;
                    valid = true;
                }
                
                break;
            case 1:
                if (!getWorld()->hasAnyEarth(getX()+1, getY()) && !getWorld()->nearBoulder(getX() + 1, getY()) && getX() != 60){
                    dir = right;
                    valid = true;
                }
                break;
            case 2:
                if (!getWorld()->hasAnyEarth(getX(), getY() - 1) && !getWorld()->nearBoulder(getX(), getY() - 1) && getY() != 0){
                    dir = down;
                    valid = true;
                }
                break;
            case 3:
                if (!getWorld()->hasAnyEarth(getX() - 1, getY()) && !getWorld()->nearBoulder(getX() - 1, getY()) && getX() != 0){
                    dir = left;
                    valid = true;
                }
                break;
                
            default:
                break;
        }
    }
    
    return dir;
}

bool Protestor::canMoveInDir(Direction dir){
    switch (dir) {
        case up:
            
            if (!getWorld()->hasAnyEarth(getX(), getY() + 1) && !getWorld()->nearBoulder(getX(), getY() + 1) && getY() != 60){
                return true;
            }
            
            break;
        case right:
            if (!getWorld()->hasAnyEarth(getX()+1, getY()) && !getWorld()->nearBoulder(getX() + 1, getY()) && getX() != 60){
                return true;
            }
            break;
        case down:
            if (!getWorld()->hasAnyEarth(getX(), getY() - 1) && !getWorld()->nearBoulder(getX(), getY() - 1) && getY() != 0){
                return true;
            }
            break;
        case left:
            if (!getWorld()->hasAnyEarth(getX() - 1, getY()) && !getWorld()->nearBoulder(getX() - 1, getY())&& getX() != 0){
                return true;
            }
            break;
            
        default:
            break;
    }
    
    return false;
    
}

bool Protestor::canMoveToLoc(int x, int y){
    if (!getWorld()->hasAnyEarth(x, y) && !getWorld()->nearBoulder(x, y) && x >= 0 && x <= 60 && y >= 0 && y <= 60 ){
        return true;
    }

    return false;
    
}


bool Protestor::currDirIsBlocked(){
    
    Direction curr = getDirection();
        
        switch (curr) {
            case up:
                
                if (!getWorld()->hasAnyEarth(getX(), getY() + 1) && !getWorld()->nearBoulder(getX(), getY() + 1) && getY() != 60){
                    return false;
                }
                
                break;
            case right:
                if (!getWorld()->hasAnyEarth(getX()+1, getY()) && !getWorld()->nearBoulder(getX() + 1, getY()) && getX() != 60){
                    return false;
                }
                break;
            case down:
                if (!getWorld()->hasAnyEarth(getX(), getY() - 1) && !getWorld()->nearBoulder(getX(), getY() - 1) && getY() != 0){
                    return false;
                }
                break;
            case left:
                if (!getWorld()->hasAnyEarth(getX() - 1, getY()) && !getWorld()->nearBoulder(getX() - 1, getY())&& getX() != 0){
                    return false;
                }
                break;
                
            default:
                break;
        }
    
    return true;
}

bool Protestor::isAtIntersection(Direction& dir){
    
    if (m_perp != 0){
        m_perp--;
        return false;
    }
    
    std::vector<Direction> viable_directions;
    
    
    Direction curr = getDirection();
    
    if (curr == up || curr == down){
        if (canMoveInDir(left)){
            viable_directions.push_back(left);
        }
        if (canMoveInDir(right)){
            viable_directions.push_back(right);
        }
    }
    
    else if (curr == left || curr == right){
        if (canMoveInDir(up)){
            viable_directions.push_back(up);
        }
        if (canMoveInDir(down)){
            viable_directions.push_back(down);
        }
    }
    
    size_t n = viable_directions.size();
    
    if (n == 0) return false;
    
    int r = rand()%n;
    
    dir =  viable_directions[r];
    
    m_perp = 200;
    
    return true;
    
}

void Protestor::doSomething(){
    
    if (!isAlive()) return;
        
    if (m_wait != 0){
        m_wait -= 1;
        return;
    }
    
    m_wait = max_wait;
    
    if (leave){
        
        if (!made_exit_stack){


            std::string path = pointToPath(60, 60);
            std::stack<Direction> st = createExitStack(path);

            m_exit_stack = st;

            made_exit_stack = true;
        }

        if (getX() == 60 && getY() == 60) {
            m_alive = false;
            return;
        }

        else{

            if (!m_exit_stack.empty()){
                Direction next_move = m_exit_stack.top();
                m_exit_stack.pop();
                setDirection(next_move);
                moveInDir(next_move);
                return;
            }

        }
        
        
    }
    
    if (!nearTunnelMan(4.0)){
        just_shouted = false;
        if (last_shout < 15) last_shout++;
    }
    
    if (nearTunnelMan(4.0) && just_shouted){
        
        if (last_shout < 15) {
            last_shout++;
            return;
        }
    }
    
    if (nearTunnelMan(4.0) && isFacingTunnelMan()){
        
        
        if (last_shout == 15){
            
            if (!just_shouted){
            
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            getWorld()->hurtTM(2);
                
            }
            
            just_shouted = true;
            
            last_shout = 0;
            
            return;
        }
        
        else last_shout ++;
        
    }
    
//    else if (!nearTunnelMan(4.0) && isHardcore){
//
//        int M = 16 + getWorld()->getLevel()*2;
//
//        std::string path = pointToPath(getWorld()->getTX(), getWorld()->getTY());
//
//        if (path.size() <= M){
//            std::stack<Direction> st = createExitStack(path);
//            if (!st.empty()){
//                Direction next_move = st.top();
//                st.pop();
//                setDirection(next_move);
//                moveInDir(next_move);
//
//            }
//            return;
//        }
//
//    }
    
    else if(canReachTunnelMan()){
        
        int tx = getWorld()->getTX();
        int ty = getWorld()->getTY();
        int x = getX();
        int y = getY();
        
        if (ty > y) setDirection(up);
        else if (ty < y) setDirection(down);
        else if (tx > x) setDirection(right);
        else if (tx < x) setDirection(left);
        
        moveInCurrentDir();
        
        m_numSquaresToMove = 0;
        
        return;
        
    }
    
    else {
        m_numSquaresToMove--;
        if (m_numSquaresToMove <= 0){
            Direction d = pickRandomDir();
            
            setDirection(d);
            int r = rand()%53 + 8;
            m_numSquaresToMove = r;
        }
        
        else{
            
            Direction new_dir;
            
            if (isAtIntersection(new_dir)){
                setDirection(new_dir);
                int r = rand()%53 + 8;
                m_numSquaresToMove = r;
            }
        }
        
    }
    
    
    
    moveInCurrentDir();
    
    if (currDirIsBlocked()) m_numSquaresToMove = 0;
    
}

void Protestor::getAnnoyed(int amt, int points){
    if (leave) return;
    m_hp-=amt;

    if (m_hp > 0){
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        int level = getWorld()->getLevel();
        int N = std::max(50,100 - level*10);
        
        m_wait = N;
        
    }
    

    else{
        leave = true;
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        m_wait = 0;
        getWorld()->increaseScore(points);
        
    }
    
    
}

void Protestor::setDead(){
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);
    leave = true;
}

void HardcoreProtestor::setDead(){
    
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(50);
    int level = getWorld()->getLevel();
    int ticks_to_stare = std::max(50, 100 - level * 10);
    setWait(ticks_to_stare);
}

std::string Protestor::pointToPath(int ex, int ey){

    class Point{
    private:
        int m_x;
        int m_y;
    public:
        Point(int x, int y){m_x = x; m_y = y;}
        void set_x(int x){m_x = x;}
        void set_y(int y){m_x = y;}
        
        int get_x(){return m_x;}
        int get_y(){return m_y;}
        
    };
    
    char maze[64][64];
    char prev[64][64];
    
    for (int i = 0; i < 61; i++){
        for (int j = 0; j < 64; j++){

            maze[i][j] = ' ';

        }
    }
    
    int x = getX();
    int y = getY();
    
    maze[y][x] = '@'; // remember need to switch x and y;

    std::queue<Point> q;
    q.push(Point(x, y));
    maze[y][x] = '#';
        
    
    prev[y][x] = '*';
    

    
    
    while (!q.empty()){
    
        Point curr = q.front();
        if (curr.get_x() == ex && curr.get_y() == ey){
            maze[ey][ex] = 'D';
            break;
        }
        
        x = curr.get_x();
        y = curr.get_y();
        
        q.pop();
        
        
        
        if (canMoveToLoc(x-1, y) && maze[y][x - 1]!= '#'){
            maze[y][x - 1] = '#';
            q.push(Point(x-1, y));
            prev[y][x - 1] = 'L';
            
//            if (path[path.size() - 1] != 'R') path += "L";
        }
        
        if (canMoveToLoc(x+1, y) && maze[y][x + 1]!= '#'){
            maze[y][x + 1] = '#';
            q.push(Point(x+1, y));
            prev[y][x + 1] = 'R';
//            if (path[path.size() - 1] != 'L') path += "R";
        }
        
        if (canMoveToLoc(x, y+1) && maze[y + 1][x]!= '#'){
            maze[y + 1][x] = '#';
            q.push(Point(x, y+1));
            prev[y+1][x] = 'U';
//            if (path[path.size() - 1] != 'D') path += "U";
        }
        
        if (canMoveToLoc(x, y-1) && maze[y - 1][x]!= '#'){
            maze[y - 1][x] = '#';
            q.push(Point(x, y - 1));
            prev[y - 1][x] = 'D';
//            if (path[path.size() - 1] != 'U') path += "D";
        }
        
    }
    
    
    
    std::string path;
    
    int i = ex;
    int j = ey;
    
    while (prev[j][i] != '*'){
        switch (prev[j][i]) {
            case 'U':
                j--;
                path += "U";
                break;

            case 'D':
                j++;
                path += "D";
                break;

            case 'R':
                i--;
                path += "R";
                break;
            case 'L':
                i++;
                path += "L";
                break;

            default:
                break;
        }
    }

    return path;
    
    
}

std::stack<Actor::Direction> Protestor::createExitStack(std::string path){
    
    std::stack<Direction> st;
    
    for (int i = 0; i < path.size(); i++){
        switch (path[i]) {
            case 'U':
                st.push(up);
                break;
            case 'D':
                st.push(down);
                break;
            case 'L':
                st.push(left);
                break;
            case 'R':
                st.push(right);
                break;
                
            default:
                break;
        }
    }
    
    return st;
}
