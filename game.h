// CS300:Frupal:Group 4
// This .h file will hold the game class

#include "menu.h"

class Game{

public:

  Game();
  Game(std::string file);
  // I don't foresee the Game class ever being dynamically allocated but just in
  // case?
  ~Game();
  void update(int key);
  //Player get_player();
  bool move_player(int to_x, int to_y, bool binocs, bool ship);
  int draw();
  int get_energy();
  void win();
  void lose();

private:
  WINDOW * window;
  std::string filename;
  int x;
  int y;
  int menu_start;
  Map map;
  Menu menu;
  Player player;
  int cursor_x;
  int cursor_y;
  bool inventory_open;

};
