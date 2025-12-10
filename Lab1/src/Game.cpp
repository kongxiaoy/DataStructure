#include "../include/Game.h"

Game::Game() = default;

void Game::initGame() {
    gameBoard.reset();
    score = 0;
    moveCount = 0;
    historyGameBoards = std::stack<GameState>();
    startTime = std::chrono::system_clock::now();
    historyGameBoards.push(GameState(gameBoard, score, moveCount));  // Save initial state
}

void Game::updateGame(Direction direction) {
    // Save current state to history
    // TODO 1
    Board originalBoard = gameBoard.getBoard();
    
    // Get move score and update score
    // TODO 2
    int moveScore = gameBoard.move(direction);
    // 在移动成功即有分数时加
    Board newBoard = gameBoard.getBoard();
    bool boardChanged = (originalBoard != newBoard);
    
    if (boardChanged) {
        // 棋盘发生了变化，更新游戏状态
        score += moveScore;
        moveCount++;

        historyGameBoards.push(GameState(gameBoard, score, moveCount));
    } else {
        // 棋盘没有变化，移除刚才保存的状态
        historyGameBoards.pop();
    }
    
}

void Game::undoLastMove() {
    if (historyGameBoards.size() > 1) {
        // Remove current state
        // TODO 1

        historyGameBoards.pop();
        // Restore previous state
        // TODO 2
        GameState previousState = historyGameBoards.top();
        gameBoard = previousState.board;
        score = previousState.score;
        moveCount = previousState.moveCount;
    } 
}

bool Game::hasWon() const {
    return gameBoard.hasWinningTile();
}

bool Game::hasLost() const {
    return gameBoard.isGameOver();
}

double Game::getElapsedTime() const {
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = endTime - startTime;
    return elapsedSeconds.count();
}

int Game::getScore() const {
    return score;
}

int Game::getMoveCount() const {
    return moveCount;
}

const GameBoard& Game::getGameBoard() const {
    return gameBoard;
}