// CS300:Frupal:Group 4
// item header

#include "item.h"

class Player: public Entity
{

  public: 
    Player();
    ~Player();

    int get_energy();
    void add_energy(int energy);
    void build(string file);
    void draw(int menu_start, WINDOW* win);
    void display_inventory(int menu_start, WINDOW* &win);
    bool clear_obstacle(int menu_start, WINDOW* &win, Obstacle* optr);
    bool has_item(char* to_cmp);
    bool has_moved();
    void reset_location();
    void set_previous_location(int x, int y);
    bool has_binocs();
    bool has_boat();
    bool get_tool(Tools* &tptr);
    void eat_food(Food* fptr);
    void get_treasure(Item* treasure);
    void use_item(int i);
    void set_whiffles(int new_whiffles);

    //works for Tools, Food, and Items (aka treasure)
    //return int signals what to do with Item* pickup after use
    //check .cpp for specifics
    int pickup_item(int menu_start, WINDOW* &win, Item* pickup);

  private:
    int my_whiffles;
    int my_energy;
    bool has_binoculars;
    bool has_ship;
    
    Item ** my_items;
    WINDOW* win;

    int player_previous_x;
    int player_previous_y;

    
};
