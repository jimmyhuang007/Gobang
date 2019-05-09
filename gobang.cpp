#include <iostream>
#include <memory>
#include <vector>
using namespace std;

const int gsize = 15;
//Check end of string to see if it ends with a new line
string CheckEnd(const string &s)
{
  string temp = s;
  if (temp.back() == 13) {
    temp.pop_back();
  }
  return temp;
}

// white piece W, black piece B, none is +
class Cell {
  int x;
  int y;
  char piece;
  int o;

public:
  Cell(int xi, int yi)
  {
    x = xi;
    y = yi;
    o = 0;
    piece = '+';
  }
  int getx();
  int gety();
  char getp();
  void changep(char p);
};

int Cell::getx()
{
  return x;
}
int Cell::gety()
{
  return y;
}
char Cell::getp()
{
  return piece;
}
void Cell::changep(char p)
{
  piece = p;
}

class Grid {
  vector<vector<Cell>> grid;

public:
  Grid();
  void gview();
  void insert(char c, int i, char p);
  bool vcheck(int x, int y, char p);
  bool cellcheck(int x, int y, char p);
  bool cellcheck2(int x, int y, char p);
};

Grid::Grid()
{
  grid.clear();
  grid.resize(gsize);
  for (int i = 0; i < gsize; i++) {
    for (int j = 0; j < gsize; j++) {
      grid.at(i).emplace_back(Cell(i, j));
    }
  }
}

void Grid::gview()
{
  cout << "------------------" << endl;
  cout << "   "; // empty space before A
  for (int c = 0; c < 15; c++) {
    char letter = 'A' + c;
    cout << letter;
  }
  cout << endl;
  for (int i = 0; i < gsize; i++) {
    if (i < 9) {
      cout << (i + 1) << "  ";
    }
    else {
      cout << i + 1 << " ";
    }
    for (int j = 0; j < gsize; j++) {
      cout << grid[i][j].getp();
    }
    cout << endl;
  }
  cout << "------------------" << endl;
}

void Grid::insert(char c, int x, char p)
{
  int y = c - 'A';
  grid[x][y].changep(p);
}

// X and Y are flipped
bool Grid::cellcheck(int x, int y, char p)
{
  if ((0 > x) || (x >= gsize) || (0 > y) || (y >= gsize) || (!(grid[y][x].getp() == p))) {
    return 0;
  }
  else {
    return 1;
  }
}

// X and Y are flipped
bool Grid::cellcheck2(int x, int y, char p)
{
  if (grid[y][x].getp() == p) {
    // cout << "At x:" << x << " At y:" << y<< " Char:" << grid[x][y].getp() << endl;
    return 1;
  }
  else {
    return 0;
  }
}

bool Grid::vcheck(int x, int y, char p)
{
  int a = 0;
  int b = 0;
  int c = 0;
  int d = 0;
  int cx;
  int cy;
  // a is horizontal, b is vertical, c is diagonal up-left to low right, d is diagonal up-right to low-left

  // a horizontal
  cx = x + 1;
  cy = y;
  while (cellcheck(cx, cy, p)) {
    cx++;
    a++;
  }
  cx = x - 1;
  cy = y;
  while (cellcheck(cx, cy, p)) {
    cx--;
    a++;
  }

  // b vertical
  cx = x;
  cy = y + 1;
  while (cellcheck(cx, cy, p)) {
    cy++;
    b++;
  }
  cx = x;
  cy = y - 1;
  while (cellcheck(cx, cy, p)) {
    cy--;
    b++;
  }

  //c diagonal up-left to low right
  cx = x + 1;
  cy = y + 1;
  while (cellcheck(cx, cy, p)) {
    cx++;
    cy++;
    c++;
  }
  cx = x - 1;
  cy = y - 1;
  while (cellcheck(cx, cy, p)) {
    cx--;
    cy--;
    c++;
  }

  //d diagonal up-left to low right
  cx = x - 1;
  cy = y + 1;
  while (cellcheck(cx, cy, p)) {
    cx--;
    cy++;
    d++;
  }
  cx = x + 1;
  cy = y - 1;
  while (cellcheck(cx, cy, p)) {
    cx++;
    cy--;
    d++;
  }

  if ((a == 4) || (b == 4) || (c == 4) || (d == 4)) {
    return 1;
  }
  else {
    return 0;
  }
}

void helpguide()
{
  cout << "---------HELP---------" << endl;
  cout << "Gomoku is a game of with the objective of connecting 5 pieces vertically, horizontally or diagonally in a straight line." << endl;
  cout << "If you would like to restart, please type 'restart' at cmd" << endl;
  cout << "If you would like to surrender, pleast type 'gg' at cmd" << endl;
  cout << "----------------------" << endl;
}

class Controls {
  Grid &g;
  int step;
  bool blackt; // Black is 1 and white is 0
  bool victory;
  int cmdcode;

public:
  Controls(Grid &gd);
  void turnwrap();
  void aturn();
  bool cmd();
  void restart();
  void gg();
  void BlackWin();
  void WhiteWin();
};
Controls::Controls(Grid &gd) : g{gd}, step{0}, blackt{true}, victory{false}, cmdcode{0} {}

void Controls::turnwrap()
{
  while (!victory) {
    aturn();

    if (cmdcode) {
      if (cmdcode == 1) {
        blackt = !blackt;
        victory = true;
      }
      else if (cmdcode == 2) {
        cout << "Restarting..." << endl;
        break;
      }
      else if (cmdcode == 3) {
        blackt = true;
        victory = true;
      }
      else if (cmdcode == 4) {
        blackt = false;
        victory = true;
      }
      cout << "Command Code:" << cmdcode << endl;
      goto vic;
    }

    cout << "Turn:" << step << endl;
    g.gview();
  vic:
    if (victory) {
      if (blackt) {
        cout << "Player 1 Black is Victorious!" << endl;
      }
      else {
        cout << "Player 2 White is Victorious!" << endl;
      }
    }
    else {
      blackt = !blackt;
      step++;
    }
  }
}

void Controls::aturn()
{
  string tinput;
  if (blackt) {
    cout << "Player 1 Black" << endl;
  }
  else {
    cout << "Player 2 White" << endl;
  }
  bool runaturn = true;
  while (runaturn) {
    cout << "Please enter coordinates or cmd for commands:";
    cin >> tinput;
    tinput = CheckEnd(tinput);
    if (tinput == "cmd") {
      bool brk = cmd();
      if (brk) {
        break;
      }
      else {
        continue;
      }
    }
    else if (((tinput.length() == 2) || (tinput.length() == 3)) &&
             ('A' <= tinput.at(0)) && (tinput.at(0) <= 'O')) {
      int xcoord, ycoord;
      char xchar = tinput.at(0);
      tinput.erase(0, 1);

      ycoord = stoi(tinput) - 1;
      xcoord = xchar - 'A';

      if ((ycoord > 15) || (0 > ycoord)) {
        cout << "Invalid X-axis, Please enter x-coordinates 0~15" << endl;
        continue;
      }

      //Main
      if (g.cellcheck2(xcoord, ycoord, '+')) {
        char inpiece = blackt ? 'B' : 'W';
        g.insert(xchar, ycoord, inpiece);
        if (g.vcheck(xcoord, ycoord, inpiece)) {
          victory = true;
        }
      }
      else {
        cout << "The cell is occupied" << endl;
        continue;
      }
      runaturn = false;
    }
    else {
      cout << "Invalid input, Please enter coordinates in format of 'A1' or cmd" << endl;
      continue;
    }
  }
}

bool Controls::cmd()
{
  string cmdinput;
  cout << "Commands available: gg, help, restart, back, blackw, whitew," << endl;
  cin >> cmdinput;
  if (cmdinput == "gg") {
    cmdcode = 1;
    return true;
  }
  else if (cmdinput == "restart") {
    cmdcode = 2;
    return true;
  }
  else if (cmdinput == "blackw") {
    cmdcode = 3;
    return true;
  }
  else if (cmdinput == "whitew") {
    cmdcode = 4;
    return true;
  }
  else if (cmdinput == "help") {
    helpguide();
    return false;
  }
  else if (cmdinput == "back") {
    return false;
  }
  else {
    cout << "Invalid Command" << endl;
    return false;
  }
}

int main()
{
  try {
    string input;
    cout << "Welcome to a game of Gomoku" << endl;
    while (1) {
      cout << "Commands available: start, help, end," << endl;
      cin >> input;
      input = CheckEnd(input);
      if (input == "help") {
        helpguide();
        continue;
      }
      else if (input == "start") {
        shared_ptr<Grid> gp = make_shared<Grid>();
        shared_ptr<Controls> ctrl = make_shared<Controls>(*gp);
        gp->gview();
        ctrl->turnwrap();
        cout << "---------Game Over---------" << endl;
      }
      else if (input == "end") {
        cout << "Thank you for playing Gomoku. Closing the program..." << endl;
        break;
      }
      else {
        cout << "Invalid command" << endl;
        continue;
      }
    }
  }
  catch (...) {
    cout << "Game Crashed" << endl;
  }
}
