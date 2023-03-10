//
// Created by Andrei Budilovskii on 10.03.2023.
//

#pragma once
#include <stack>
#include <array>

struct Game final {
    enum struct FieldState{
        Empty, X, O
    };
    enum struct PlayerSequence{
        X, O
    };
    FieldState GetCurrent(){
        switch (playerSequence) {
            case PlayerSequence::X:
                return FieldState::X;
            case PlayerSequence::O:
                return FieldState::O;
        }
    }
    void ChangePlayerSequence(){
        switch (playerSequence) {
            case PlayerSequence::X:
                playerSequence = PlayerSequence::O;
                break;
            case PlayerSequence::O:
                playerSequence = PlayerSequence::X;
                break;
        }
    }
    FieldState CheckAll(size_t i, size_t j){
        FieldState fieldState = CheckVertical(j);
        if (fieldState != FieldState::Empty) return fieldState;
        fieldState = CheckHorizontal(i);
        if (fieldState != FieldState::Empty) return fieldState;
        fieldState = CheckFirstDiagonal();
        if (fieldState != FieldState::Empty) return fieldState;
        fieldState = CheckSecondDiagonal();
        return fieldState;
    }
    void DoStep(size_t i, size_t j){
        if (i > 2 || j > 2) throw std::out_of_range("indexes must be in [0, 2]");
        if (arr[i][j] != FieldState::Empty) throw std::logic_error("Поле занято");
        arr[i][j] = GetCurrent();
        ChangePlayerSequence();
    }
    void UndoStep(size_t i, size_t j){
        if (i > 2 || j > 2) throw std::out_of_range("indexes must be in [0, 2]");
        if (arr[i][j] == FieldState::Empty) throw std::logic_error("Поле пусто");
        arr[i][j] = FieldState::Empty;
        ChangePlayerSequence();
    }
    [[nodiscard]] auto& GetArr() const{
        return arr;
    }
    auto GetSequence() const{
        return playerSequence;
    }
    Game(){
        std::for_each(arr.begin(), arr.end(),
                      [](auto& row){std::fill(row.begin(), row.end(), FieldState::Empty);});
    }
private:
    FieldState CheckVertical(size_t index){
        size_t first = 0;
        size_t second = 0;
        for (size_t i = 0; i < 3; i++){
            switch (arr[i][index]) {
                case FieldState::Empty:
                    break;
                case FieldState::X:
                    first++;
                    break;
                case FieldState::O:
                    second++;
                    break;
            }
        }
        if (first == 3) return FieldState::X;
        if (second == 3) return FieldState::O;
        return FieldState::Empty;
    }
    FieldState CheckHorizontal(size_t index){
        size_t first = 0;
        size_t second = 0;
        for (size_t i = 0; i < 3; i++){
            switch (arr[index][i]) {
                case FieldState::Empty:
                    break;
                case FieldState::X:
                    first++;
                    break;
                case FieldState::O:
                    second++;
                    break;
            }
        }
        if (first == 3) return FieldState::X;
        if (second == 3) return FieldState::O;
        return FieldState::Empty;
    }
    FieldState CheckFirstDiagonal(){
        size_t first = 0;
        size_t second = 0;
        for (size_t i = 0; i < 3; i++){
            switch (arr[i][i]) {
                case FieldState::Empty:
                    break;
                case FieldState::X:
                    first++;
                    break;
                case FieldState::O:
                    second++;
                    break;
            }
        }
        if (first == 3) return FieldState::X;
        if (second == 3) return FieldState::O;
        return FieldState::Empty;
    }
    FieldState CheckSecondDiagonal(){
        size_t first = 0;
        size_t second = 0;
        for (size_t i = 0; i < 3; i++){
            switch (arr[i][2 - i]) {
                case FieldState::Empty:
                    break;
                case FieldState::X:
                    first++;
                    break;
                case FieldState::O:
                    second++;
                    break;
            }
        }
        if (first == 3) return FieldState::X;
        if (second == 3) return FieldState::O;
        return FieldState::Empty;
    }
    PlayerSequence playerSequence = PlayerSequence::X;
    std::array<std::array<FieldState, 3>, 3> arr;
};

struct Command{
    size_t i;
    size_t j;
    Game& game;
    void Do(){
        game.DoStep(i, j);
    }
    void Undo(){
        game.UndoStep(i, j);
    }
};

struct GameWithHistory{
    using FieldState = Game::FieldState;
    using PlayerSequence = Game::PlayerSequence;
    void OneTurn(size_t i, size_t j){
        if (GetState() != FieldState::Empty) throw std::logic_error("Игра завершена");
        auto command = Command{.i = i, .j = j, .game = game};
        command.Do();
        commands.push(command);
    }
    void Revert(){
        if (!commands.empty()){
            commands.top().Undo();
            commands.pop();
        }
    }
    FieldState GetState(){
        if (commands.empty()){
            return Game::FieldState::Empty;
        }
        auto& [i, j, _] = commands.top();
        return game.CheckAll(i, j);
    }

    [[nodiscard]] const auto& GetArr() const {
        return game.GetArr();
    }
    [[nodiscard]] auto GetSequence(){
        return game.GetSequence();
    }
private:
    Game game;
    std::stack<Command> commands;
};