#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <queue>
#include <stack>
#include <map>
#define INF 1000000000

static int target[3][3] = {{1, 2, 3}, {8, 0, 4}, {7, 6, 5}};
static int dx[4] = {0, 1, 0, -1};
static int dy[4] = {1, 0, -1, 0};
static int pos[9][2] = {{1, 1}, {0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {2, 1}, {2, 0}, {1, 0}};

class Board {
  int a[3][3];
  int x, y;
public:
  Board() {
    clear();
  }

  void setStatus(int hash) {
    for (int i = 2; i >= 0; i--) {
      for (int j = 2; j >= 0; j--) {
        a[i][j] = hash % 9;
        hash /= 9;
        if (a[i][j] == 0) {
          x = i; y = j;
        }
      }
    }
  }

  void clear() {
    x = y = 1;
    memcpy(a, target, sizeof(a));
  }

  int getHash() {
    int hash = 0;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        hash = hash * 9 + a[i][j];
      }
    }
    return hash;
  }

  int move(int dir) {
    int xx = x + dx[dir];
    int yy = y + dy[dir];
    if (xx < 0 || xx > 2 || yy < 0 || yy > 2) {
      return -1;
    }
    std::swap(a[xx][yy], a[x][y]);
    x = xx;
    y = yy;
    return getHash();
  }

  int predict() {
    int p = 0;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        p += abs(pos[ a[i][j] ][0] - i) + abs(pos[ a[i][j] ][1] - j);
      }
    }
    return p;
  }

  void print() {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        printf("%d%c", a[i][j], j == 2 ? '\n' : ' ');
      }
    }
  }
};

std::map< int, std::pair<int, int> > hashTable;
std::priority_queue< std::pair< int, int > > que;
int bestStep = INF;
std::stack<int> sta;

int main(int argc, char **argv) {
  if (argc == 3) {
    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);
  }

  int st = 0, ed = 0;
  Board board;
  ed = board.getHash();
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      int x;
      std::cin >> x;
      st = st * 9 + x;
    }
  }

  hashTable[st] = std::make_pair(0, -1);
  board.setStatus(st);
  que.push(std::make_pair(-board.predict() / 2, st));


  //Here is the A* search
  while (!que.empty()) {
    std::pair<int, int> now = que.top();
    int step = hashTable[now.second].first;
    que.pop();
    if (now.second == ed) {
      break;
    }
    board.setStatus(now.second);

    for (int dir = 0; dir < 4; dir++) {
      int tmp = board.move(dir);
      if (tmp == -1) {
        continue;
      }
      if (hashTable.find(tmp) == hashTable.end()) {
        hashTable[tmp] = std::make_pair(step + 1, now.second);
        que.push(std::make_pair(-step - 1 - board.predict() / 2, tmp));
      }
      board.move((dir + 2) % 4);
    }
  }
  
  if (hashTable.find(ed) == hashTable.end()) {
    std::cout << "no solution" << std::endl;
    exit(0);
  }
  printf("%d\n", hashTable[ed].first);
  int tmp = ed;
  while (tmp != st) {
    sta.push(tmp);
    tmp = hashTable[tmp].second;
  }
  while (!sta.empty()) {
    printf("\n");
    tmp = sta.top();
    sta.pop();
    board.setStatus(tmp);
    board.print();
  }
}
