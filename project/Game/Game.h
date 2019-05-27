//
// Created by arugaf on 08.05.19.
//

#ifndef ONEMORETHING_GAME_H
#define ONEMORETHING_GAME_H

#define NUM_OF_MAIN_MENU_BUTTONS 2
#define NUM_OF_PAUSE_MENU_BUTTONS 3
#define DEFAULT_SELECT 100

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

#include "Player.h"
#include "Map.h"
#include "Character.h"

struct Stats {
    int hp;
    int dmg;
    int spd;
};

enum Game_status {MAIN_MENU = 0, START_MENU = 1, MAP_MENU = 2, RUN_GAME = 3};

class Game {
 public:
    Game();
    //Game(const int& map_id); // Пока не сделано меню

    void process_event();
    void show_main_menu();
    void show_choice_menu();
    void run_game(const std::string& xml_file_path);

    ~Game() = default;

 private:
    Game_status status;

    std::vector<int> characters;

    std::unique_ptr<sf::RenderWindow> window;

    std::vector<std::string> maps;
};

class Menu {
 public:
    Menu(float width, float height);

    void move_up();
    void move_down();

    int selected_item = DEFAULT_SELECT;

    std::unique_ptr<sf::RenderWindow> draw(std::unique_ptr<sf::RenderWindow> window);

    void process_mouse(sf::Vector2i position);

 private:
    sf::Texture background_texture;
    sf::Texture main_menu_texture;
    sf::Texture button_texture;

    sf::Sprite background;
    sf::Sprite main_menu;
    std::vector<sf::Sprite> button;

    sf::Font font;

    sf::Color gold;

    sf::Text game_name;
    std::vector<sf::Text> menu_button;

    const int offset = 12; // 29 справа 17 слева
    const int button_offset = 150;

    const int text_offset_x = 5;
    const int text_offset_y = 23;
};

// Решил забить на наследование, т.к. все равно все дублировать и переписывать почти все

class Choice_menu {
 public:
    Choice_menu(float width, float height);

    void process_mouse(sf::Vector2i position);
    int selected_item = DEFAULT_SELECT;

    void move_left();
    void move_right();
    void move_up();
    void move_down();
    void insert();
    void delete_char();

    int selected_char = 0;
    int selected_icon = 0;
    int selected_chars[5] = {-1, -1, -1, -1, -1};

    bool is_playable();

    std::unique_ptr<sf::RenderWindow> draw(std::unique_ptr<sf::RenderWindow> window);
 private:
    std::vector<sf::Sprite*> objects;

    std::vector<sf::Text> char_names;

    bool characters[8] = {true, true, true, true, true, true, true, true};

    sf::RectangleShape select();

    void change_pos(int icon_num);

    sf::Text menu_name;

    sf::Texture background_texture;
    sf::Texture menu_texture;
    sf::Texture choose_button_texture;
    sf::Texture close_button_texture;
    sf::Texture play_button_texture;
    sf::Texture left_button_texture;
    sf::Texture right_button_texture;
    sf::Texture plus_button_texture;
    sf::Texture minus_button_texture;
    sf::Texture ribbon_texture;
    sf::Texture frame_texture;
    sf::Texture chars_preview_texture;

    sf::Sprite background;
    sf::Sprite menu;
    sf::Sprite choose_button;
    sf::Sprite close_button;
    sf::Sprite play_button;
    sf::Sprite left_button;
    sf::Sprite right_button;
    sf::Sprite plus_button;
    sf::Sprite minus_button;
    sf::Sprite ribbon;
    std::vector<sf::Sprite> frame;
    sf::Sprite chars_preview;

    sf::Texture stat_bar_texture;
    std::vector<sf::Sprite> stat_bar;
    std::vector<sf::Text> stats;
    std::vector<Stats> char_stats = {
            {60, 40, 222},
            {50, 200, 30},
            {120, 100, 110},
            {222, 20, 30},
            {50, 200, 30},
            {80, 180, 120},
            {150, 100, 70},
            {50, 50, 50}
    };

    std::vector<sf::Texture> icons_texture;
    std::vector<sf::Sprite> icons;

    sf::RectangleShape draw_stat(int size, sf::Color bar_color, int index);

    sf::Font font;

    sf::Color gold;
    sf::Color gray;

    const int choose_button_offset_x = 32;
    const int choose_button_offset_y = 220;

    const int play_button_offset_x = 125;
    const int play_button_offset_y = 100;

    const int close_button_offset_x = 79;
    const int close_button_offset_y = 130;

    const int rl_button_offset_y = 9;

    const int selected_offset_x = 159;
    const int selected_offset_y = 269;

    const int pm_button_offset_x = 207;
    const int pm_button_offset_y = 175;
    const int m_button_offset_x = 44;
    const int m_button_offset_y = 10;

    const int ribbon_offset_x = 120;
    const int ribbon_offset_y = 50;

    const int menu_name_offset_x = 160;
    const int menu_name_y = 270;

    const int char_name_offset_x = 182;
    const int char_name_offset_y = 53;

    const int stat_bar_offset_x = 430;
    const int stat_bar_offset_y = 325;
    const int stat_bar_offset = 6;

    const int max_stat = 222;

    const int frame_offset_x = 145;
    const int frame_offset_y = 257;
    const int frame_offset = 10;

    const int chars_preview_width = 250;
    const int chars_preview_height = 252;
    const int chars_preview_offset_x = 435;
    const int chars_preview_offset_y = 370;

    int current_frame = 0;
};

class Map_menu {
 public:
    Map_menu(float width, float height);
    std::unique_ptr<sf::RenderWindow> draw(std::unique_ptr<sf::RenderWindow> window);

 private:
    sf::Texture background_texture;
    sf::Texture menu_texture;
    sf::Texture map_texture;

    sf::Sprite background;
    sf::Sprite menu;
};

class Pause_menu {
 public:
    Pause_menu(float width, float height);
    void process(sf::RenderWindow* window, sf::Sprite& screenshot);
    void process_mouse(sf::Vector2i position);

private:
    int selected_item = DEFAULT_SELECT;

    void move_up();
    void move_down();

    sf::Font font;

    sf::Color gold;

    sf::Texture menu_texture;
    sf::Texture button_texture;
    std::vector<sf::Sprite> button;


    // TODO:: ПЕРЕПОЛНЕНИЕ СТЕКА?


    sf::Text menu_name;
    std::vector<sf::Text> menu_button;

    sf::Sprite menu;

    const int offset = 12; // 29 справа 17 слева
    const int button_offset = 70;
    const int button_offset_y = 205;

    const int text_offset_x = 15;
    const int text_offset_y = 23;
};

class Loading {
 public:
    Loading(float width, float height);
    std::unique_ptr<sf::RenderWindow> draw(std::unique_ptr<sf::RenderWindow> window);

 private:
    sf::RectangleShape draw_bar();

    sf::Font font;
    sf::Color gold;
    sf::Text loading;

    sf::Texture background_texture;
    sf::Texture loading_bar_texture;

    sf::Sprite background;
    sf::Sprite loading_bar;
    int loading_status = 5;

    const int bar_offset_x = 12;
    const int bar_offset_y = 17;

    const int loading_offset_x = 160;
    const int loading_offset_y = 60;
};

#endif //ONEMORETHING_GAME_H
