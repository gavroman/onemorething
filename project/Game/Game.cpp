    //
// Created by arugaf on 08.05.19.
//
#include "Game.h"

bool ah_shit_here_we_go_again = false;
bool multiplayer = false;
bool multiplayer_2 = false;

Game::Game() {
    main_theme.openFromFile("../source/sounds/main_theme.ogg");
    sound_track.openFromFile("../source/sounds/soundtrack.ogg");
    main_theme.setLoop(true);
    sound_track.setLoop(true);
    background_texture.loadFromFile("../source/menu/Background.png");
    background_texture.setSmooth(true);
    background.setTexture(background_texture);
    
    maps = {"Dark_map.tmx",
            "Grass_map.tmx",
            "Snow_map.tmx",
            "Mixed_map.tmx",
            "Mixed_map_v2.tmx"};

    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 1080), "One More Thing", sf::Style::Fullscreen);
    //window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 750), "One More Thing");
    status = MAIN_MENU;
       //status = RUN_GAME;                       //  откоментить для дебага 
       //characters = {0, 1, 2, 3, 4, 5, 6, 7};   //
    process_event();
}

void Game::process_event() {
    while (window->isOpen()) {
        switch (status) {
            case MAIN_MENU: {
                if (main_theme.getStatus() !=  sf::SoundSource::Status::Playing) {
                    sound_track.stop();
                    main_theme.play();
                }
                show_main_menu();
                break;
            }
            case START_MENU: {
                show_choice_menu();
                break;
            }
            case MAP_MENU: {
                show_mp_menu();
                break;
            }
            case RUN_GAME: {
                if(multiplayer) {
                    status = START_MENU;
                    multiplayer = false;
                    multiplayer_2 = true;
                    break;
                }
                run_game(maps[2]);
                break;
            }
        }
    }
}

void Game::show_mp_menu() {
    Multiplayer_menu menu(window->getSize().x, window->getSize().y);
    window->setFramerateLimit(12);
    while (window->isOpen()) {
        sf::Event event = sf::Event();
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyReleased: {
                    switch (event.key.code) {
                        case sf::Keyboard::Up: {
                            menu.move_up();
                            break;
                        }
                        case sf::Keyboard::Down: {
                            menu.move_down();
                            break;
                        }
                        case sf::Keyboard::W: {
                            menu.move_up();
                            break;
                        }
                        case sf::Keyboard::S: {
                            menu.move_down();
                            break;
                        }
                        case sf::Keyboard::Return: {
                            switch (menu.selected_item) {
                                case 0: {
                                    status = START_MENU;
                                    multiplayer = false;
                                    return;
                                }
                                case 1: {
                                    status = START_MENU;
                                    multiplayer = true;
                                    return;
                                }
                                default: {
                                    continue;
                                }
                            }
                        }
                        default: {
                            continue;
                        }
                    }
                    break;
                }

                case sf::Event::Closed: {
                    window->close();
                    break;
                }

                case sf::Event::MouseMoved: {
                    menu.process_mouse(sf::Mouse::getPosition());
                    break;
                }

                case sf::Event::MouseButtonReleased: {
                    menu.process_mouse(sf::Mouse::getPosition());
                    switch (menu.selected_item) {
                        case 0: {
                            status = START_MENU;
                            multiplayer = false;
                            return;
                        }
                        case 1: {
                            status = START_MENU;
                            multiplayer = true;
                            return;
                        }
                        default: {
                            continue;
                        }
                    }
                }

                default: {
                    continue;
                }
            }
        }
        window->clear();
        window = menu.draw(std::move(window));
        window->display();
    }
}

void Game::run_game(const std::string& xml_file_path) {
    std::vector<std::unique_ptr<Player>> players;

    Loading loading_screen(window->getSize().x, window->getSize().y);

    window = loading_screen.draw(std::move(window));
    Map btl_fld("../source/game_map/" + xml_file_path);

    if (multiplayer_2) {
        window = loading_screen.draw(std::move(window));
        players.push_back(std::make_unique<Human>(btl_fld, PLAYER1, characters_2)); //создание игроков
        window = loading_screen.draw(std::move(window));
        players.push_back(std::make_unique<Human>(btl_fld, PLAYER2, characters));
        multiplayer_2 = false;
    } else {
        window = loading_screen.draw(std::move(window));
        players.push_back(std::make_unique<Human>(btl_fld, PLAYER1, characters)); //создание игроков
        window = loading_screen.draw(std::move(window));
        players.push_back(std::make_unique<Bot>(btl_fld, PLAYER2, std::vector<int>({0, 5, 2, 7, 4})));
    }
    //window = loading_screen.draw(std::move(window));
    btl_fld.get_adj_matrix();

    window->setFramerateLimit(12);
    
    Current_player curr_plr = PLAYER1;
    bool made_turn = false;
    //window->requestFocus();
    while (window->isOpen()) {
        /*if (!window->hasFocus()) {
            continue;
        }*/
        if (!made_turn) {
            made_turn = players[curr_plr]->make_turn(btl_fld, *window);
            if (!players[curr_plr]->get_chars_size()) {
                sf::Texture win_scroll;
                win_scroll.loadFromFile("../source/menu/Win_scroll.png");
                sf::Sprite win(win_scroll);

                sf::Font font;
                font.loadFromFile("../source/menu/Enchanted_Land.otf");
                sf::Text win_text("Player1 wins!", font, 50);

                if (curr_plr == PLAYER1) {
                    win_text.setString("Player2 wins!");
                    win_text.setOutlineColor(sf::Color::Red);
                } else {
                    win_text.setOutlineColor(sf::Color::Cyan);

                }
                win_text.setPosition(window->getSize().x / 2 - 93, window->getSize().y / 2 - 25);
                win_text.setFillColor(sf::Color::Black);
                win_text.setOutlineThickness(2);

                window->draw(win);
                window->draw(win_text);
                window->display();
                while (window->isOpen()) {
                    sf::Event event;
                    while (window->pollEvent(event)) {
                        switch (event.type) {
                            case sf::Event::Closed: {
                                window->close();
                                break;
                            }

                            case sf::Event::KeyReleased: {
                                status = MAIN_MENU;
                                return;
                            }
                        }
                    }
                }
            }
        }
        if (made_turn and players[curr_plr]->is_all_idle()) {
            made_turn = false;
            curr_plr = (curr_plr == PLAYER1) ? PLAYER2 : PLAYER1;
        }

        if (ah_shit_here_we_go_again) {
            status = MAIN_MENU;
            ah_shit_here_we_go_again = false;
            return;
        }

        if (sound_track.getStatus() !=  sf::SoundSource::Status::Playing) {
            main_theme.stop();
            sound_track.play();
        }
        window->clear();
        window->draw(background);
        btl_fld.draw(*window);

        for (int i = 0; i != players[PLAYER1]->get_chars_size(); i++) {
            //players[PLAYER1]->get_char(i)->draw(*window, btl_fld);
            players[PLAYER1]->get_char(i)->animate();
        }
        for (int i = 0; i != players[PLAYER2]->get_chars_size(); i++) {
            //players[PLAYER2]->get_char(i)->draw(*window, btl_fld);
            players[PLAYER2]->get_char(i)->animate();
        } 
        window->display();
        //std::this_thread::sleep_for(std::chrono::milliseconds(160)); // установлен лимит кадров средсвами SFML
    }
}

void Game::show_main_menu() {
    Menu menu(window->getSize().x, window->getSize().y);
    window->setFramerateLimit(12);
    while (window->isOpen()) {
        sf::Event event = sf::Event();
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyReleased: {
                    switch (event.key.code) {
                        case sf::Keyboard::Up: {
                            menu.move_up();
                            break;
                        }
                        case sf::Keyboard::Down: {
                            menu.move_down();
                            break;
                        }
                        case sf::Keyboard::W: {
                            menu.move_up();
                            break;
                        }
                        case sf::Keyboard::S: {
                            menu.move_down();
                            break;
                        }
                        case sf::Keyboard::Return: {
                            switch (menu.selected_item) {
                                case 0: {
                                    status = MAP_MENU;
                                    return;
                                }
                                case 1: {
                                    window->close();
                                    break;
                                }
                                default: {
                                    continue;
                                }
                            }
                        }
                        default: {
                            continue;
                        }
                    }
                    break;
                }

                case sf::Event::Closed: {
                    window->close();
                    break;
                }

                case sf::Event::MouseMoved: {
                    menu.process_mouse(sf::Mouse::getPosition());
                    break;
                }

                case sf::Event::MouseButtonReleased: {
                    menu.process_mouse(sf::Mouse::getPosition());
                    switch (menu.selected_item) {
                        case 0: {
                            status = MAP_MENU;
                            return;
                        }
                        case 1: {
                            window->close();
                            break;
                        }
                        default: {
                            continue;
                        }
                    }
                }

                default: {
                    continue;
                }
            }
        }
        window->clear();
        window = menu.draw(std::move(window));
        window->display();
    }
}

void Game::show_choice_menu() {
    Choice_menu menu(window->getSize().x, window->getSize().y);
    window->setFramerateLimit(60);
    while (window->isOpen()) {
        sf::Event event = sf::Event();
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyReleased: {
                    switch (event.key.code) {
                        case sf::Keyboard::Left: {
                            menu.move_left();
                            break;
                        }
                        case sf::Keyboard::Right: {
                            menu.move_right();
                            break;
                        }
                        case sf::Keyboard::A: {
                            menu.move_left();
                            break;
                        }
                        case sf::Keyboard::D: {
                            menu.move_right();
                            break;
                        }
                        case sf::Keyboard::Equal: {
                            menu.insert();
                            break;
                        }
                        case sf::Keyboard::Dash: {
                            menu.delete_char();
                            break;
                        }
                        case sf::Keyboard::Up: {
                            menu.move_up();
                            break;
                        }
                        case sf::Keyboard::Down: {
                            menu.move_down();
                            break;
                        }
                        case sf::Keyboard::W: {
                            menu.move_up();
                            break;
                        }
                        case sf::Keyboard::S: {
                            menu.move_down();
                            break;
                        }
                        case sf::Keyboard::Return: {
                            if (menu.is_playable()) {
                                status = RUN_GAME;
                                characters.assign(menu.selected_chars, menu.selected_chars + 5);
                                for (auto it : characters) {
                                    std::cout << it << std::endl;
                                }
                                if (multiplayer) {
                                    characters_2.assign(characters.begin(), characters.end());
                                    characters.clear();
                                }
                                return;
                            }
                            continue;
                        }
                        case sf::Keyboard::Escape: {
                            status = MAIN_MENU;
                            return;
                        }
                        default: {
                            continue;
                        }
                    }
                    break;
                }

                case sf::Event::MouseMoved: {
                    menu.process_mouse(sf::Mouse::getPosition());
                    break;
                }

                case sf::Event::MouseButtonReleased: {
                    switch (menu.selected_item) {
                        case 0: {
                            menu.selected_icon = 0;
                            continue;
                        }
                        case 1: {
                            menu.selected_icon = 1;
                            continue;
                        }
                        case 2: {
                            menu.selected_icon = 2;
                            continue;
                        }
                        case 3: {
                            menu.selected_icon = 3;
                            continue;
                        }
                        case 4: {
                            menu.selected_icon = 4;
                            continue;
                        }
                        case 5: {
                            menu.move_left();
                            continue;
                        }
                        case 6: {
                            menu.move_right();
                            continue;
                        }
                        case 7: {
                            menu.insert();
                            continue;
                        }
                        case 8: {
                            menu.delete_char();
                            continue;
                        }
                        case 9: {
                            status = MAIN_MENU;
                            return;
                        }
                        case 10: {
                            if (menu.is_playable()) {
                                status = RUN_GAME;
                                characters.assign(menu.selected_chars, menu.selected_chars + 5);
                                for (auto it : characters) {
                                    std::cout << it << std::endl;
                                }
                                return;
                            }
                            break;
                        }
                        default: {
                            continue;
                        }
                    }
                }

                case sf::Event::Closed: {
                    window->close();
                    break;
                }

                default: {
                    continue;
                }
            }
        }

        window->clear();
        window = menu.draw(std::move(window));
        window->display();
    }
}

Menu::Menu(float width, float height) {
    font.loadFromFile("../source/menu/Enchanted_Land.otf");

    gold.r = 212;
    gold.g = 175;
    gold.b = 55;

    background_texture.loadFromFile("../source/menu/Background.png");
    background_texture.setSmooth(true);
    background.setTexture(background_texture);

    main_menu_texture.loadFromFile("../source/menu/Main.png");
    main_menu_texture.setSmooth(true);
    main_menu.setTexture(main_menu_texture);
    //main_menu.setScale(width / 1920, height / 1080);
    main_menu.setPosition((width - main_menu_texture.getSize().x) / 2 + offset, (height - main_menu_texture.getSize().y) / 2);

    button_texture.loadFromFile("../source/menu/Button.png");
    button_texture.setSmooth(true);

    button.emplace_back(sf::Sprite(button_texture));
    button.emplace_back(sf::Sprite(button_texture));

    button[0].setPosition(main_menu.getPosition().x + (main_menu_texture.getSize().x - button_texture.getSize().x) / 2.0 - offset,
                       main_menu.getPosition().y + main_menu_texture.getSize().y / (NUM_OF_MAIN_MENU_BUTTONS + 1.0) + button_offset);
    button[1].setPosition(main_menu.getPosition().x + (main_menu_texture.getSize().x - button_texture.getSize().x) / 2.0 - offset,
                         main_menu.getPosition().y + main_menu_texture.getSize().y / (NUM_OF_MAIN_MENU_BUTTONS + 1.0) + button_offset * 1.6);

    menu_button.emplace_back(sf::Text());
    menu_button.emplace_back(sf::Text());

    menu_button[0].setFont(font);
    menu_button[0].setFillColor(sf::Color::White);
    menu_button[0].setOutlineColor(sf::Color::Black);
    menu_button[0].setOutlineThickness(1);
    menu_button[0].setString("Play");
    menu_button[0].setPosition(button[0].getPosition().x + button_texture.getSize().x / 2.0 - offset - text_offset_x,
                               button[0].getPosition().y + button_texture.getSize().y / 2.0 - text_offset_y);

    menu_button[1].setFont(font);
    menu_button[1].setFillColor(sf::Color::White);
    menu_button[1].setOutlineColor(sf::Color::Black);
    menu_button[1].setOutlineThickness(1);
    menu_button[1].setString("Quit");
    menu_button[1].setPosition(button[0].getPosition().x + button_texture.getSize().x / 2.0 - offset - text_offset_x,
                               button[1].getPosition().y + button_texture.getSize().y / 2.0 - text_offset_y);

    game_name.setFont(font);
    game_name.setFillColor(sf::Color::Black);
    game_name.setOutlineColor(gold);
    game_name.setOutlineThickness(3);
    game_name.setCharacterSize(60);
    game_name.setString("One More Thing");
    game_name.setPosition(width / 2 - 135, 200);

}

std::unique_ptr<sf::RenderWindow> Menu::draw(std::unique_ptr<sf::RenderWindow> window) {
    window->draw(background);
    window->draw(main_menu);

    for (int i = 0; i < menu_button.size(); i++) {
        if (i == selected_item) {
            menu_button[i].setFillColor(gold);
        } else {
            menu_button[i].setFillColor(sf::Color::White);
        }
    }

    for (const auto& it : button) {
        window->draw(it);
    }
    window->draw(game_name);

    for (const auto& it : menu_button) {
        window->draw(it);
    }

    return std::move(window);
}

void Menu::move_up() {
    if (selected_item <= 0 || selected_item > menu_button.size() - 1) {
        selected_item = menu_button.size() - 1;
    } else {
        selected_item--;
    }
}

void Menu::move_down() {
    if (selected_item >= menu_button.size() - 1) {
        selected_item = 0;
    } else {
        selected_item++;
    }
}

void Menu::process_mouse(sf::Vector2i position) {
    for (int i = 0; i < button.size(); i++) {
        if (position.x >= button[i].getPosition().x &&
            position.x <= button[i].getPosition().x + button_texture.getSize().x &&
            position.y >= button[i].getPosition().y &&
            position.y <= button[i].getPosition().y + button_texture.getSize().y) {
            selected_item = i;
            break;
        } else {
            selected_item = DEFAULT_SELECT;
        }
    }
}

    Multiplayer_menu::Multiplayer_menu(float width, float height) {
        font.loadFromFile("../source/menu/Enchanted_Land.otf");

        gold.r = 212;
        gold.g = 175;
        gold.b = 55;

        background_texture.loadFromFile("../source/menu/Background.png");
        background_texture.setSmooth(true);
        background.setTexture(background_texture);

        main_menu_texture.loadFromFile("../source/menu/Main.png");
        main_menu_texture.setSmooth(true);
        main_menu.setTexture(main_menu_texture);
        //main_menu.setScale(width / 1920, height / 1080);
        main_menu.setPosition((width - main_menu_texture.getSize().x) / 2 + offset, (height - main_menu_texture.getSize().y) / 2);

        button_texture.loadFromFile("../source/menu/Button.png");
        button_texture.setSmooth(true);

        button.emplace_back(sf::Sprite(button_texture));
        button.emplace_back(sf::Sprite(button_texture));

        button[0].setPosition(main_menu.getPosition().x + (main_menu_texture.getSize().x - button_texture.getSize().x) / 2.0 - offset,
                              main_menu.getPosition().y + main_menu_texture.getSize().y / (NUM_OF_MAIN_MENU_BUTTONS + 1.0) + button_offset);
        button[1].setPosition(main_menu.getPosition().x + (main_menu_texture.getSize().x - button_texture.getSize().x) / 2.0 - offset,
                              main_menu.getPosition().y + main_menu_texture.getSize().y / (NUM_OF_MAIN_MENU_BUTTONS + 1.0) + button_offset * 1.6);

        menu_button.emplace_back(sf::Text());
        menu_button.emplace_back(sf::Text());

        menu_button[0].setFont(font);
        menu_button[0].setFillColor(sf::Color::White);
        menu_button[0].setOutlineColor(sf::Color::Black);
        menu_button[0].setOutlineThickness(1);
        menu_button[0].setString("Singleplayer");
        menu_button[0].setPosition(button[0].getPosition().x + button_texture.getSize().x / 2.0 - offset - text_offset_x,
                                   button[0].getPosition().y + button_texture.getSize().y / 2.0 - text_offset_y);

        menu_button[1].setFont(font);
        menu_button[1].setFillColor(sf::Color::White);
        menu_button[1].setOutlineColor(sf::Color::Black);
        menu_button[1].setOutlineThickness(1);
        menu_button[1].setString("     Multiplayer");
        menu_button[1].setPosition(button[0].getPosition().x + button_texture.getSize().x / 2.0 - offset - 53,
                                   button[1].getPosition().y + button_texture.getSize().y / 2.0 - text_offset_y);

        game_name.setFont(font);
        game_name.setFillColor(sf::Color::Black);
        game_name.setOutlineColor(gold);
        game_name.setOutlineThickness(3);
        game_name.setCharacterSize(60);
        game_name.setString("One More Thing");
        game_name.setPosition(width / 2 - 135, 200);

    }

    std::unique_ptr<sf::RenderWindow> Multiplayer_menu::draw(std::unique_ptr<sf::RenderWindow> window) {
        window->draw(background);
        window->draw(main_menu);

        for (int i = 0; i < menu_button.size(); i++) {
            if (i == selected_item) {
                menu_button[i].setFillColor(gold);
            } else {
                menu_button[i].setFillColor(sf::Color::White);
            }
        }

        for (const auto& it : button) {
            window->draw(it);
        }
        window->draw(game_name);

        for (const auto& it : menu_button) {
            window->draw(it);
        }

        return std::move(window);
    }

    void Multiplayer_menu::move_up() {
        if (selected_item <= 0 || selected_item > menu_button.size() - 1) {
            selected_item = menu_button.size() - 1;
        } else {
            selected_item--;
        }
    }

    void Multiplayer_menu::move_down() {
        if (selected_item >= menu_button.size() - 1) {
            selected_item = 0;
        } else {
            selected_item++;
        }
    }

    void Multiplayer_menu::process_mouse(sf::Vector2i position) {
        for (int i = 0; i < button.size(); i++) {
            if (position.x >= button[i].getPosition().x &&
                position.x <= button[i].getPosition().x + button_texture.getSize().x &&
                position.y >= button[i].getPosition().y &&
                position.y <= button[i].getPosition().y + button_texture.getSize().y) {
                selected_item = i;
                break;
            } else {
                selected_item = DEFAULT_SELECT;
            }
        }
    }

Choice_menu::Choice_menu(float width, float height) {
    font.loadFromFile("../source/menu/Enchanted_Land.otf");

    gold.r = 212;
    gold.g = 175;
    gold.b = 55;

    gray = sf::Color(150, 150, 150);

    menu_name.setFont(font);
    menu_name.setFillColor(gold);
    menu_name.setOutlineColor(sf::Color::Black);
    menu_name.setOutlineThickness(3);
    menu_name.setCharacterSize(60);
    menu_name.setString("Gather your squad");
    menu_name.setPosition(width / 2 - menu_name_offset_x, menu_name_y);

    background_texture.loadFromFile("../source/menu/Background.png");
    background_texture.setSmooth(true);
    background.setTexture(background_texture);

    menu_texture.loadFromFile("../source/menu/Character.png");
    menu_texture.setSmooth(true);
    menu.setTexture(menu_texture);
    menu.setPosition((width - menu_texture.getSize().x) / 2, (height - menu_texture.getSize().y) / 2);

    choose_button_texture.loadFromFile("../source/menu/Choose_button.png");
    choose_button_texture.setSmooth(true);
    choose_button.setTexture(choose_button_texture);
    choose_button.setPosition(menu.getPosition().x + menu_texture.getSize().x / 2.0 - choose_button_offset_x,
                              menu.getPosition().y + menu_texture.getSize().y / 2.0 + choose_button_offset_y);

    close_button_texture.loadFromFile("../source/menu/Close_button.png");
    close_button_texture.setSmooth(true);
    close_button.setTexture(close_button_texture);
    close_button.setScale(0.6, 0.6);
    close_button.setPosition(menu.getPosition().x + menu_texture.getSize().x - close_button_offset_x,
                       menu.getPosition().y + close_button_offset_y);

    play_button_texture.loadFromFile("../source/menu/Play_button.png");
    play_button_texture.setSmooth(true);
    play_button.setTexture(play_button_texture);
    play_button.setScale(0.8, 0.8);
    play_button.setPosition(menu.getPosition().x + menu_texture.getSize().x - play_button_offset_x,
                            menu.getPosition().y + menu_texture.getSize().y - play_button_offset_y);

    left_button_texture.loadFromFile("../source/menu/Button_left.png");
    right_button_texture.loadFromFile("../source/menu/Button_right.png");
    left_button_texture.setSmooth(true);
    right_button_texture.setSmooth(true);
    left_button.setTexture(left_button_texture);
    right_button.setTexture(right_button_texture);
    left_button.setPosition(choose_button.getPosition().x - 2,
                            choose_button.getPosition().y + rl_button_offset_y);
    right_button.setPosition(choose_button.getPosition().x + choose_button_texture.getSize().x - 15,
                             choose_button.getPosition().y + rl_button_offset_y);

    plus_button_texture.loadFromFile("../source/menu/Plus_button.png");
    minus_button_texture.loadFromFile("../source/menu/Minus_button.png");
    plus_button_texture.setSmooth(true);
    minus_button_texture.setSmooth(true);
    plus_button.setTexture(plus_button_texture);
    minus_button.setTexture(minus_button_texture);
    plus_button.setPosition(menu.getPosition().x + menu_texture.getSize().x - pm_button_offset_x,
                            menu.getPosition().y + menu_texture.getSize().y - pm_button_offset_y);
    minus_button.setPosition(plus_button.getPosition().x - m_button_offset_x,
                            plus_button.getPosition().y + m_button_offset_y);

    ribbon_texture.loadFromFile("../source/menu/Ribbon.png");
    ribbon_texture.setSmooth(true);
    ribbon.setTexture(ribbon_texture);
    ribbon.setPosition(menu.getPosition().x + menu_texture.getSize().x - ribbon_offset_x,
                       menu.getPosition().y + menu_texture.getSize().y - ribbon_offset_y);
    ribbon.setScale(0.7, 0.7);

    char_names.emplace_back(sf::Text("Scout", font));
    char_names.emplace_back(sf::Text(" Archer", font));
    char_names.emplace_back(sf::Text("Swordmaster", font));
    char_names.emplace_back(sf::Text("Guardian", font));
    char_names.emplace_back(sf::Text("Wizard", font));
    char_names.emplace_back(sf::Text("Berserker", font));
    char_names.emplace_back(sf::Text("Conqueror", font));
    char_names.emplace_back(sf::Text("Druid", font));

    for (auto& it : char_names) {
        it.setOutlineColor(sf::Color::Black);
        it.setOutlineThickness(2);

        it.setOrigin(it.findCharacterPos(it.getString().getSize() / 2).x, 0);
        it.setPosition(menu_texture.getSize().x + char_name_offset_x, menu_texture.getSize().y - char_name_offset_y);
    }

    stat_bar_texture.loadFromFile("../source/menu/Stats.png");
    stat_bar_texture.setSmooth(true);

    stats.emplace_back(sf::Text("hp", font, 20));
    stats.emplace_back(sf::Text("dmg", font, 20));
    stats.emplace_back(sf::Text("spd", font, 20));

    for (auto& it : stats) {
        it.setOutlineColor(sf::Color::Black);
        it.setOutlineThickness(1);
    }

    for (int i = 0; i < 3; i++) {
        stat_bar.emplace_back(sf::Sprite(stat_bar_texture));
        stat_bar[i].setPosition(menu.getPosition().x + menu_texture.getSize().x - stat_bar_offset_x,
                                menu.getPosition().y + (stat_bar_texture.getSize().y - stat_bar_offset) * i + stat_bar_offset_y);
        stats[i].setPosition(stat_bar[i].getPosition().x - 17,
                             stat_bar[i].getPosition().y - 8);
    }

    for (int i = 0; i < 8; i++) {
        icons_texture.emplace_back(sf::Texture());
    }
    icons_texture[0].loadFromFile("../source/menu/Scout.png");
    icons_texture[1].loadFromFile("../source/menu/Archer.png");
    icons_texture[2].loadFromFile("../source/menu/Swordman.png");
    icons_texture[3].loadFromFile("../source/menu/Tank.png");
    icons_texture[4].loadFromFile("../source/menu/Wizard.png");
    icons_texture[5].loadFromFile("../source/menu/Berserker.png");
    icons_texture[6].loadFromFile("../source/menu/Knight.png");
    icons_texture[7].loadFromFile("../source/menu/Healer.png");

    for (auto& it : icons_texture) {
        it.setSmooth(true);
        icons.emplace_back(sf::Sprite(it));
    }

    for (auto& it : icons) {
        it.setColor(sf::Color(255, 255, 255, 230));
    }

    frame_texture.loadFromFile("../source/menu/Frame.png");
    frame_texture.setSmooth(true);
    for (int i = 0; i < 5; i++) {
        frame.emplace_back(sf::Sprite(frame_texture));
        frame[i].setPosition(menu.getPosition().x + frame_offset_x,
                         menu.getPosition().y + frame_offset_y + (frame_texture.getSize().y - frame_offset) * i);
    }

    chars_preview_texture.loadFromFile("../source/menu/Chars.png");
    chars_preview_texture.setSmooth(true);
    chars_preview.setTexture(chars_preview_texture);
    chars_preview.setTextureRect(sf::IntRect(0, 0, chars_preview_width, chars_preview_height));
    chars_preview.setPosition(menu.getPosition().x + chars_preview_offset_x, menu.getPosition().y + chars_preview_offset_y);

    for (auto& it : frame) {
        objects.emplace_back(&it);
    }
    objects.emplace_back(&left_button);
    objects.emplace_back(&right_button);
    objects.emplace_back(&plus_button);
    objects.emplace_back(&minus_button);
    objects.emplace_back(&close_button);
    objects.emplace_back(&play_button);
}

std::unique_ptr<sf::RenderWindow> Choice_menu::draw(std::unique_ptr<sf::RenderWindow> window) {
    window->draw(background);
    window->draw(menu);
    window->draw(choose_button);
    window->draw(close_button);

    for (auto it : selected_chars) {
        play_button.setColor(sf::Color(170, 50, 50));
        if (selected_item == 10) {
            play_button.setColor(sf::Color::Red);
        }
        if (it == -1) {
            play_button.setColor(sf::Color::White);
            break;
        }
    }

    for (const auto& it : stat_bar) {
        window->draw(it);
    }

    for (const auto& it : stats) {
        window->draw(it);
    }

    if (current_frame >= char_stats[selected_char].hp) {
        window->draw(draw_stat(char_stats[selected_char].hp, sf::Color(60, 255, 113, 150), 0));
    } else {
        window->draw(draw_stat(current_frame, sf::Color(60, 255, 113, 150), 0));
    }
    if (current_frame >= char_stats[selected_char].dmg) {
        window->draw(draw_stat(char_stats[selected_char].dmg, sf::Color(255, 0, 0, 150), 1));
    } else {
        window->draw(draw_stat(current_frame, sf::Color(255, 0, 0, 150), 1));
    }
    if (current_frame >= char_stats[selected_char].spd) {
        window->draw(draw_stat(char_stats[selected_char].spd, sf::Color(127, 255, 212, 150), 2));
    } else {
        window->draw(draw_stat(current_frame, sf::Color(127, 255, 212, 150), 2));
    }

    window->draw(ribbon);

    window->draw(play_button);
    if (selected_char < 7) {
        window->draw(right_button);
    }
    if (selected_char > 0) {
        window->draw(left_button);
    }
    if (characters[selected_char]) {
        if (!characters[selected_char]) {
            plus_button.setColor(sf::Color(255, 255, 255, 100));
        } else {
            if (selected_item != 7) { // Костыль
                plus_button.setColor(sf::Color::White);
            }
        }
        window->draw(plus_button);

    } else {
        if (selected_chars[selected_icon] != selected_char) {
            minus_button.setColor(sf::Color(255, 255, 255, 100));
        } else {
            if (selected_item != 8) { // Костыль
                minus_button.setColor(sf::Color::White);
            }
        }
        window->draw(minus_button);
    }

    window->draw(menu_name);
    window->draw(char_names[selected_char]);

    for (const auto& it : frame) {
        window->draw(it);
    }

    for (int i = 0; i < 5; i++) {
        if (selected_chars[i] != -1) {
            change_pos(i);
            window->draw(icons[selected_chars[i]]);
        }
    }

    window->draw(select());

    window->draw(chars_preview);

    if (current_frame < max_stat) {
        current_frame += 6;
    }

    return std::move(window);
}

void Choice_menu::move_left() {
    if (selected_char <= 0) {
        return;
    } else {
        selected_char--;
    }

    chars_preview.setTextureRect(sf::Rect(chars_preview_width * selected_char, 0, chars_preview_width, chars_preview_height));

    current_frame = 0;
}

void Choice_menu::move_right() {
    if (selected_char >= 7) {
        return;
    } else {
        selected_char++;
    }

    chars_preview.setTextureRect(sf::Rect(chars_preview_width * selected_char, 0, chars_preview_width, chars_preview_height));

    current_frame = 0;
}

void Choice_menu::move_down() {
    if (selected_icon >= 4) {
        return;
    } else {
        selected_icon++;
    }
}

void Choice_menu::move_up() {
    if (selected_icon <= 0) {
        return;
    } else {
        selected_icon--;
    }
}

sf::RectangleShape Choice_menu::select() {
    sf::RectangleShape rect(sf::Vector2f(72, 72));
    rect.setOutlineThickness(1);
    rect.setOutlineColor(sf::Color::White);
    rect.setFillColor(sf::Color::Transparent);
    rect.setPosition(menu.getPosition().x + selected_offset_x,
                     menu.getPosition().y + selected_offset_y + selected_icon * (frame_texture.getSize().y - frame_offset));
    return rect;
}

void Choice_menu::insert() {
    if (selected_chars[selected_icon] != selected_char && characters[selected_char]) {
        if (selected_chars[selected_icon] != -1) {
            characters[selected_chars[selected_icon]] = true;
        }
        selected_chars[selected_icon] = selected_char;
        characters[selected_char] = false;
    }

    for (auto it : selected_chars) {
        std::cout << it << " ";
    }
    std::cout << std::endl;
}

void Choice_menu::delete_char() {
    if (selected_chars[selected_icon] == selected_char) {
        selected_chars[selected_icon] = -1;
        characters[selected_char] = true;
    }
    for (auto it : selected_chars) {
        std::cout << it << " ";
    }
    std::cout << std::endl;
}

bool Choice_menu::is_playable() {
    return play_button.getColor() == sf::Color::Red || play_button.getColor() == sf::Color(170, 50, 50);
}

sf::RectangleShape Choice_menu::draw_stat(int size, sf::Color bar_color, int index) {
    sf::RectangleShape stat_bar_rect(sf::Vector2f(size, stat_bar_texture.getSize().y - 11));
    stat_bar_rect.setFillColor(bar_color);
    stat_bar_rect.setPosition(stat_bar[index].getPosition().x + 12,
                              stat_bar[index].getPosition().y + 3);
    return stat_bar_rect;
}

void Choice_menu::change_pos(int icon_num) {
    icons[selected_chars[icon_num]].setPosition(menu.getPosition().x + selected_offset_x,
                     menu.getPosition().y + selected_offset_y + icon_num * (frame_texture.getSize().y - frame_offset));
}

void Choice_menu::process_mouse(sf::Vector2i position) {
    for (auto& it : objects) {
        it->setColor(sf::Color::White);
    }

    for (int i = 0; i < objects.size(); i++) {
        if (position.x >= objects[i]->getPosition().x &&
        position.x <= objects[i]->getPosition().x + objects[i]->getTexture()->getSize().x &&
        position.y >= objects[i]->getPosition().y &&
        position.y <= objects[i]->getPosition().y + objects[i]->getTexture()->getSize().y) {
            selected_item = i;
            objects[i]->setColor(gray);
            break;
        } else {
            selected_item = DEFAULT_SELECT;
        }
    }
}

Loading::Loading(float width, float height) {
    font.loadFromFile("../source/menu/Enchanted_Land.otf");

    gold.r = 212;
    gold.g = 175;
    gold.b = 55;

    background_texture.loadFromFile("../source/menu/Background.png");
    background_texture.setSmooth(true);
    background.setTexture(background_texture);

    loading_bar_texture.loadFromFile("../source/menu/LoadingBar.png");
    loading_bar_texture.setSmooth(true);
    loading_bar.setTexture(loading_bar_texture);
    loading_bar.setPosition((width - loading_bar_texture.getSize().x) / 2, (height - loading_bar_texture.getSize().y) / 2);

    loading.setFont(font);
    loading.setFillColor(gold);
    loading.setOutlineColor(sf::Color::Black);
    loading.setOutlineThickness(1);
    loading.setString("Loading...");
    loading.setCharacterSize(50);
    loading.setPosition(loading_bar.getPosition().x + loading_offset_x,
                        loading_bar.getPosition().y - loading_offset_y);
}

std::unique_ptr<sf::RenderWindow> Loading::draw(std::unique_ptr<sf::RenderWindow> window) {
    window->clear();

    window->draw(background);
    window->draw(loading_bar);
    window->draw(draw_bar());
    window->draw(loading);

    window->display();
    return std::move(window);
}

sf::RectangleShape Loading::draw_bar() {
    sf::RectangleShape rect(sf::Vector2f(loading_status, 4));
    rect.setOutlineThickness(1);
    rect.setOutlineColor(sf::Color::Black);
    rect.setFillColor(sf::Color::Red);
    rect.setPosition(loading_bar.getPosition().x + bar_offset_x,
                     loading_bar.getPosition().y + bar_offset_y);
    loading_status += 140;
    return rect;
}

Pause_menu::Pause_menu(float width, float height) {
    font.loadFromFile("../source/menu/Enchanted_Land.otf");

    gold.r = 212;
    gold.g = 175;
    gold.b = 55;

    menu_name.setFont(font);
    menu_name.setFillColor(sf::Color::Black);
    menu_name.setOutlineColor(gold);
    menu_name.setOutlineThickness(3);
    menu_name.setCharacterSize(60);
    menu_name.setString("Pause");
    menu_name.setPosition(width / 2 - 58, 205);

    menu_texture.loadFromFile("../source/menu/Main.png");
    menu_texture.setSmooth(true);
    menu.setTexture(menu_texture);
    menu.setPosition((width - menu_texture.getSize().x) / 2 + offset, (height - menu_texture.getSize().y) / 2);

    button_texture.loadFromFile("../source/menu/Button.png");
    button_texture.setSmooth(true);
    menu_button.emplace_back(sf::Text("Resume", font));
    menu_button.emplace_back(sf::Text("Main menu", font));
    menu_button.emplace_back(sf::Text(" Quit", font));
    for (int i = 0; i < NUM_OF_PAUSE_MENU_BUTTONS; i++) {
        button.emplace_back(sf::Sprite(button_texture));
        button[i].setPosition(menu.getPosition().x + (menu_texture.getSize().x - button_texture.getSize().x) / 2.0 - offset,
                              menu.getPosition().y + menu_texture.getSize().y / (NUM_OF_PAUSE_MENU_BUTTONS + 1.0) + button_offset_y + button_offset * i);
        menu_button[i].setFillColor(sf::Color::White);
        menu_button[i].setOutlineColor(sf::Color::Black);
        menu_button[i].setOutlineThickness(1);
        menu_button[i].setOrigin(menu_button[i].findCharacterPos(menu_button[i].getString().getSize() / 2).x, 0);
        menu_button[i].setPosition(button[i].getPosition().x + button_texture.getSize().x / 2.0 - offset + text_offset_x,
                                   button[i].getPosition().y + button_texture.getSize().y / 2.0 - text_offset_y);
    }
}

void Pause_menu::process(sf::RenderWindow *window, sf::Sprite& screenshot) {
    while (window->isOpen()) {
        sf::Event event = sf::Event();
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyReleased: {
                    switch (event.key.code) {
                        case sf::Keyboard::Up: {
                            move_up();
                            break;
                        }
                        case sf::Keyboard::Down: {
                            move_down();
                            break;
                        }
                        case sf::Keyboard::W: {
                            move_up();
                            break;
                        }
                        case sf::Keyboard::S: {
                            move_down();
                            break;
                        }
                        case sf::Keyboard::Return: {
                            switch (selected_item) {
                                case 0: {
                                    return;
                                }
                                case 1: {
                                    ah_shit_here_we_go_again = true;
                                    std::cout << "ZALUPA" << std::endl;
                                    return;
                                }
                                case 2: {
                                    window->close();
                                    break;
                                }
                                default: {
                                    continue;
                                }
                            }
                        }
                        case sf::Keyboard::Escape: {
                            return;
                        }
                        default: {
                            continue;
                        }
                    }
                    break;
                }

                case sf::Event::Closed: {
                    window->close();
                    break;
                }

                case sf::Event::MouseMoved: {
                    process_mouse(sf::Mouse::getPosition());
                    break;
                }

                case sf::Event::MouseButtonReleased: {
                    process_mouse(sf::Mouse::getPosition());
                    switch (selected_item) {
                        case 0: {
                            return;
                        }
                        case 1: {
                            ah_shit_here_we_go_again = true;
                            return;
                        }
                        case 2: {
                            window->close();
                            break;
                        }
                        default: {
                            continue;
                        }
                    }
                }

                default: {
                    continue;
                }
            }
        }
        window->clear();

        window->draw(screenshot);

        window->draw(menu);
        for (const auto& it : button) {
            window->draw(it);
        }
        window->draw(menu_name);

        for (int i = 0; i < menu_button.size(); i++) {
            if (i == selected_item) {
                menu_button[i].setFillColor(gold);
            } else {
                menu_button[i].setFillColor(sf::Color::White);
            }
        }
        for (const auto& it : menu_button) {
            window->draw(it);
        }

        window->display();
    }
}

void Pause_menu::move_up() {
        if (selected_item <= 0 || selected_item > menu_button.size() - 1) {
            selected_item = menu_button.size() - 1;
        } else {
            selected_item--;
        }
        menu_button[selected_item].setFillColor(gold);
    }

void Pause_menu::move_down() {
    if (selected_item >= menu_button.size() - 1) {
            selected_item = 0;
        } else {
            selected_item++;
        }
    menu_button[selected_item].setFillColor(gold);
}

void Pause_menu::process_mouse(sf::Vector2i position) {
    for (int i = 0; i < button.size(); i++) {
        if (position.x >= button[i].getPosition().x &&
            position.x <= button[i].getPosition().x + button_texture.getSize().x &&
            position.y >= button[i].getPosition().y &&
            position.y <= button[i].getPosition().y + button_texture.getSize().y) {
            selected_item = i;
            break;
        } else {
            selected_item = DEFAULT_SELECT;
        }
    }
}
