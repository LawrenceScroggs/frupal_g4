//CS300, Group Project: Land of Frupal
//Group 4

//This map file contains all the functons for the map
//each Gronik represents a cell on the map and can contain
//an item

#include "map.h"

bool debug = true;  //TOGGLE THIS TO MAKE THE MAP VISIBLE

Grovnik::Grovnik(Type new_type)
{
	type = new_type;
	entity = NULL;
  draw = ' ';
}

Type Grovnik::get_type()
{
  return type;
}

Grovnik::~Grovnik()
{
	if(entity)
	{
		delete entity;
	}
}

void Grovnik::toggle_vis(bool vis)
{
    int temp = (int) type;

    if(vis && type > 3)
    {
      temp -= 4; 
      type = static_cast <Type> (temp);
    }

    else if(!vis && type < 4)
    {
        temp +=4;
        type = static_cast <Type> (temp);
    }
}

void Grovnik::build_ent(int ent,int i,int dia_x,int dia_y)
{
 switch(ent)
  {
      case 'D':   //DIAMOND
        entity = new Royal_Diamond();
        draw = DIAMOND;
      break;

      case 'W':   //TREASURE
        entity = new Item();
        entity->init(i);
        draw = TREASURE;
      break;

      case 'T':   //TOOL
        entity = new Tools();
        entity->init(i);
        if(i == 2)
          draw = BINOCULARS;
        else if(i == 3)
          draw = SHIP;
        else
          draw = TOOL;
      break;

      case 'F':   //FOOD
        entity = new Food();
        entity->init(i);
        draw = FOOD;
      break;

      case 'O':   //OBSTACLE
        entity = new Obstacle();
        entity->init(i);
        draw = OBSTACLE;
      break;

      case 'C':   //CLUE
        entity = new Clue;
        entity->init(dia_x,dia_y);
        draw = CLUE;
      break;
  }
}

//Returns the the character to draw in a grovnik
char Grovnik::get_char()
{
  return draw;
}

//Return the pointer of the entity in the grovnik
Entity* Grovnik::get_ent()
{
  return entity;
}

Map::Map()
{
  map = NULL;
}

//Takes in the file name, opens an in file stream and populates the map
//Expects the first line to be the player spawn in x,y
//Map is lines 1-128, expected format #ofcells,Type:#ofcells,Type:etc, 128 cells per line
//Lines 129+ contain the item infromation
void Map::build(string file_name)
{
  int x = 0, y = 0, i = 0, grov_count = 0, new_type = 0, dia_x = 0, dia_y = 0;
  char tempg[4];
  char tempt[2];
  char tempi[2];
  char tempx[4];
  char tempy[4];
  ifstream in;

  in.open(file_name);

  
  if(!in)   //Check if file exists, exit if not
  {
    mvprintw(0,0,"NO MAP FILE FOUND");
    exit(-1);
  } 

  else
  {
    map = new Grovnik**[128];

    in.get(tempg,4,',');
    in.ignore(100,'\n');       //Ignore player spawn
    
    while(in && !in.eof() && y < 128)
    {
      map[y] = new Grovnik*[128];
      while(x < 128)
      {
        in.get(tempg,4,',');
        in.ignore(4,',');
        in.get(tempt,4,':');
        in.ignore(4,':');
        
        grov_count = stoi(tempg);
        new_type = stoi(tempt);
        if(debug)
          new_type-=4; //THIS MAKES THE WHOLE MAP VISIBLE
                
        int current_x = x;
        while(x < (current_x + grov_count))
        {
          if(x > 127)   //Bounds check
          {
            break;
          } 

          Type to_add = static_cast<Type>(new_type);
          map[y][x] = new Grovnik(to_add);
          x++;
        }
      }
      x = 0;
      in.ignore(100,'\n'); 
      ++y;
    }

    //Grab and build items
    in.get(tempt,4,',');
    in.ignore(4,',');
    while(in && !in.eof())
    {
      in.get(tempx,4,',');
      in.ignore(4,',');
      in.get(tempy,4,':');
      in.ignore(4,':');
      new_type = (int)tempt[0];
      x = stoi(tempx);
      y = stoi(tempy);
            
      switch(new_type)
      {
          case 'D':   //DIAMOND
            map[y][x]->build_ent(new_type);
            break;

          case 'W':   //TREASURE
            in.get(tempi,4,':');
            in.ignore(4,':');
            i = stoi(tempi); 
            map[y][x]->build_ent(new_type,i);
            break;

          case 'T':   //TOOL
            in.get(tempi,4,':');
            in.ignore(4,':');
            i = stoi(tempi);
            map[y][x]->build_ent(new_type,i);
            break;

          case 'F':   //FOOD
            in.get(tempi,4,':');
            in.ignore(4,':');
            i = stoi(tempi);
            map[y][x]->build_ent(new_type,i);
            break;

          case 'O':   //OBSTACLE
            in.get(tempi,4,':');
            in.ignore(4,':');
            i = stoi(tempi);
            map[y][x]->build_ent(new_type,i);
            break;

          case 'C':   //CLUE
            in.get(tempi,4,':');
            in.ignore(4,':');
            i = stoi(tempi);
            map[y][x]->build_ent(new_type,dia_x,dia_y);
            break;
      }

      in.ignore(4,'\n'); //Go to next line/item
      in.get(tempt,2,',');
      in.ignore(2,',');

    }
  }

  in.close();
  in.clear();

}

//Uses the terminal size and player location to draw the map
//so the player is always on screen
//leaves a 25 char space on the right for the menu
Entity* Map::draw(WINDOW* &game_win, int cur_x, int cur_y, int play_x, int play_y)
{
  int size_x;
  int size_y;
  getmaxyx(game_win,size_y,size_x);
  size_x -= 26;
  Type grov = MEADOW_INV;
  char ent;

  //Offets to keep the player centered in the terminal
  offset[0] = play_x - (size_x / 2);
  offset[1] = play_y - (size_y / 2);

  start_color();
  init_pair(MEADOW, COLOR_BLACK, COLOR_GREEN);
  init_pair(SWAMP, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(WATER, COLOR_WHITE, COLOR_BLUE);
  init_pair(WALL, COLOR_BLACK, COLOR_WHITE);
  init_pair(HERO, COLOR_WHITE, COLOR_RED);
  init_pair(ROYAL, COLOR_WHITE, COLOR_CYAN);

  for(int y = 0; y < size_y; y++)
  {
    for(int x = 0; x < size_x; x++)
    {
      //Draws any out of bounds as invisible
      if(0 > (y + offset[1]) || 127 < (y + offset[1]) || 0 > (x + offset[0]) || 127 < (x + offset[0]))
      {
        grov = MEADOW_INV;
        ent = EMPTY;
      }
        
      else
      {
      grov = map[y + offset[1]][x + offset[0]]->get_type();   //Grab grovnik type
      ent = map[y + offset[1]][x + offset[0]]->get_char();    //Grab item char to draw
      }
      
      if(ent == DIAMOND && grov < MEADOW_INV)
      {
        attron(COLOR_PAIR(ROYAL));
        mvwaddch(game_win,y,x, '$');
        attroff(COLOR_PAIR(ROYAL));
      } else
      {
        switch(grov)
        {
          case MEADOW_VIS:
            attron(COLOR_PAIR(MEADOW));
            mvwaddch(game_win,y,x, ent);
            attroff(COLOR_PAIR(MEADOW));
            break;

          case SWAMP_VIS:
            attron(COLOR_PAIR(SWAMP));
            mvwaddch(game_win,y,x, ent);
            attroff(COLOR_PAIR(SWAMP));
            break;        

          case WATER_VIS:
            attron(COLOR_PAIR(WATER));
            mvwaddch(game_win,y,x, ent);
            attroff(COLOR_PAIR(WATER));
            break;        

          case WALL_VIS:
            attron(COLOR_PAIR(WALL));
            mvwaddch(game_win,y,x, ent);
            attroff(COLOR_PAIR(WALL));
            break;        

          default:
            break;
        }
      }
    }
  }

  //Draws the player center screen
  attron(COLOR_PAIR(HERO));
  mvwaddch(game_win,(size_y / 2),(size_x / 2), PLAYER);
  attroff(COLOR_PAIR(HERO));

  //Check if cursor is in bounds of map
  if(0 > (cur_y + offset[1]) || 127 < (cur_y + offset[1]) || 0 > (cur_x + offset[0]) || 127 < (cur_x + offset[0]))
  {
    grov = MEADOW_INV;
  } else
    grov = map[cur_y + offset[1]][cur_x + offset[0]]->get_type();

  mvwprintw(game_win,8,(size_x + 3),"Type: %s",name_type[(int)grov].c_str());
  //TODO add print entity in grov info to menu

  //Move cursor back to where the user left it
  wmove(game_win, cur_y, cur_x);

  return map[play_y][play_x]->get_ent();
}

//This update function will take the players location and binocular status
//and turn any grovniks around them visible
void Map::reveal(int play_x, int play_y, bool binocs)
{
  int check = 1;
  if(binocs)
    check = 2;

  for(int y = (play_y - check); y <= (play_y + check); y++)
  {
    if(0 > y || y > 127)
      continue;
    for(int x = (play_x - check); x <= (play_x + check); x++)
    {
      if(0 > x || x > 127)
        continue;

      map[y][x]->toggle_vis(true);
      cout<<"";
    }
  }
}

//Returns grovnik type for provided coordinates
Type Map::info(int x, int y)
{
  return map[y][x]->get_type();
}

Map::~Map()
{
	for(int x=0; x<128; x++)
	{
		for(int y=0; y<128; y++)
		{
          if(map[x][y])
          {
            delete map[x][y];			
            map[x][y] = NULL;
          }
		}
	}
}
