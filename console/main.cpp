#include <iostream>
#include "game.hpp"
#include "magic_enum.hpp"

void Print(const GameWithHistory& game){
    const auto& arr = game.GetArr();
    for (auto& row: arr){
        for (auto elem : row){
            switch (elem) {
                case Game::FieldState::X:
                    std::cout << "X ";
                    break;
                case Game::FieldState::O:
                    std::cout << "O ";
                    break;
                case Game::FieldState::Empty:
                    std::cout << "* ";
                    break;
            }
        }
        std::cout << '\n';
    }
}

enum struct ConsoleCommands{
    restart, revert, turn
};

int main() {
    while (true){
        GameWithHistory game;
        bool ended_game = false;
        while (!ended_game){
            Print(game);
            std::cout << "Ход: " << magic_enum::enum_name(game.GetSequence()) << '\n';
            std::cout << "Введите, что нужно сделать: \n"
                         "restart - перезапуск игры, \n"
                         "revert - откатить последний ход, \n"
                         "turn - сделать новый ход: \n";
            std::string temp;
            std::cin >> temp;
            auto command = magic_enum::enum_cast<ConsoleCommands>(temp);
            while (!command){
                std::cout << "Ошибка при вводе, повторите ввод, что нужно сделать: \n"
                             "restart - перезапуск игры, \n"
                             "revert - откатить последний ход, \n"
                             "turn - сделать новый ход: ";
                std::cin >> temp;
                command = magic_enum::enum_cast<ConsoleCommands>(temp);
            }
            switch (*command) {
                case ConsoleCommands::restart:
                    ended_game = true;
                    break;
                case ConsoleCommands::revert:
                    game.Revert();
                    break;
                case ConsoleCommands::turn:
                    {
                        std::string temp_i, temp_j;
                        std::cout << "Введите индексы(строка, столбец): ";
                        std::cin >> temp_i >> temp_j;
                        try{
                            game.OneTurn(std::stoi(temp_i), std::stoi(temp_j));
                        }
                        catch (std::exception& exception){
                            std::cout << "Произошла ошибка: " << exception.what() << "\n";
                        }
                    }
                    break;
            }
            if (game.GetState() != Game::FieldState::Empty){
                std::cout << "Win by " << magic_enum::enum_name(game.GetState()) << '\n';
            }
        }
    }
}
