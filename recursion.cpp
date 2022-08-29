#include <iostream>
#include <string>

using namespace std;

int multiplyTwoNumbers(unsigned int m, unsigned int n)
{
    if (m == 0 || n == 0) return 0;
    if (n == 1) return m;
    return m + multiplyTwoNumbers(m, n-1);
}

int tabulationStation(int num, int digit)
{
    
    if (num > -10 && num < 10) return (num == digit || num == -1*digit);
    
    return (num%10 == digit || num%10 == -1*digit) + tabulationStation(num/10, digit);
}

string highFives(string n){
    
    if (n.size() == 1) return n;
    
    string first = n.substr(0,1); // first letter
    string second = n.substr(1,1); // second letter
    if (first == second) return first + "55" + highFives(n.substr(1));
    else return first + highFives(n.substr(1));
    
}

string jheriCurls(string str)
{
    
    string first = str.substr(0,1);
    string last = str.substr(str.size()-1);
    size_t n = str.size();
    
    if (first != "{") return jheriCurls(str.substr(1, n-1));
    if (last != "}") return jheriCurls(str.substr(0, n-1));
    
    return str;
    
}


bool aggregationNation(const int a[], int size, int target)
{
    if (size == 0) return (target == 0);
    if (size == 1) return (a[0] == target || target == 0);

    return (aggregationNation(a, size - 1, target) || aggregationNation(a, size - 1, target - a[size-1]));
}

bool onMyWayHome(string maze[], int nRows, int nCols,
                 int sr, int sc, int er, int ec){
    
    if (sr == er && sc == ec) return true;
    maze[sr][sc] = '#';
    if (maze[sr-1][sc] == '.') if (onMyWayHome(maze, nRows, nCols, sr-1, sc, er, ec)) return true;
    if (maze[sr+1][sc] == '.') if (onMyWayHome(maze, nRows, nCols, sr+1, sc, er, ec)) return true;
    if (maze[sr][sc-1] == '.') if (onMyWayHome(maze, nRows, nCols, sr, sc-1, er, ec)) return true;
    if (maze[sr][sc+1] == '.') if (onMyWayHome(maze, nRows, nCols, sr, sc+1, er, ec)) return true;
    return false;
}

int main() {
    // insert code here...
    
//    cout << highFives("aaaa") << endl;
//    cout << jheriCurls("4agh{3984}341") << endl;
////      Pseudocode Example:
//    int arr[3] = {2, 4, 8};
////    cout << aggregationNation(arr, 0, 1) << endl;
//    cout << aggregationNation(arr, 3, 6) << endl;
////         aggregationNation({2, 4, 8}, 3, 11)
////         aggregationNation({2, 4, 8}, 3, 0)
////         aggregationNation({}, 0, 0)
//
//    cout << multiplyTwoNumbers(0, 1) << endl;
//    cout << tabulationStation(12345, 6) << endl;
    
//        string maze[10] = {
//           "XXXXXXXXXX",
//           "X.......@X",
//           "XX@X@@.XXX",
//           "X..X.X...X",
//           "X..X...@.X",
//           "X....XXX.X",
//           "X@X....XXX",
//           "X..XX.XX.X",
//           "X...X....X",
//           "XXXXXXXXXX"
//        };
//
//        if (onMyWayHome(maze, 10, 10, 6, 4, 1, 1))
//            cout << "Solvable!" << endl;
//        else
//            cout << "Out of luck!" << endl;
    
        return 0;
}

