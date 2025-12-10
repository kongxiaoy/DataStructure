#include "../include/GameBoard.h"
#include <algorithm>
#include <random>
#include <chrono>

GameBoard::GameBoard() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator = std::mt19937(seed);
    reset();
}

void GameBoard::reset() {
    // Initialize the board to be 4x4 with all values set to 0
    board = Board(4, std::vector<int>(4, 0));// 创建一个 4×4 的二维向量，所有位置初始值为 0
    std::uniform_int_distribution<int> dist(0, 3);  // [0, 3] uniform distribution 位置的分布随机
    std::uniform_int_distribution<int> value_dist(0, 1);// 数值随机
    
    // Add two initial tiles
    for (int i = 0; i < 2; ++i) {
        int x = dist(generator);
        int y = dist(generator);
        while (board[x][y] != 0) { // Ensure we don't place a tile on an occupied cell
            x = dist(generator);
            y = dist(generator);
        }
        board[x][y] = (value_dist(generator) + 1) * 2; // Place either a 2 or a 4
    }
}

int GameBoard::move(Direction direction) {
    // Slide and merge, anyother?
    // TODO 1
    Board original = board;// 保存移动前的棋盘状态用于判断是否有实际移动
    
    slide(direction);
    int score = merge(direction);
    slide(direction);// #?猜测实现
    
       

    // Add a new tile after every move
    // You can refer to the reset() function to see how to add a new tile
    // TODO 2
    if (original != board) {
        std::uniform_int_distribution<int> dist(0, 3);       
        std::uniform_int_distribution<int> value_dist(0, 1); 
        int x = dist(generator);
        int y = dist(generator);// 生成位置
        while (board[x][y] != 0) {
            x = dist(generator);
            y = dist(generator);
        }
        
        int temp = value_dist(generator);
        
        board[x][y] = (value_dist(generator) + 1) * 2; // Place either a 2 or a 4
    }
    return score;
}

void GameBoard::slide(Direction direction) {
    switch (direction) {
        case Direction::UP:
            for (int col = 0; col < 4; ++col) {// 按列遍历
                int target = 3;// 目标位为3，即最上层
                for (int row = 3; row >= 0; --row) {// 从顶行开始
                    if (board[row][col] != 0) {// 如果有数字
                        std::swap(board[target][col], board[row][col]);// 进行最上层与当前有数字位的交换
                        if (target != row) board[row][col] = 0;// 若有数字位不是当前最高则删掉原数字
                        --target;
                    }
                }
            }
            break;
        case Direction::DOWN:
            for (int col = 0; col < 4; ++col) {
                int target = 0;
                for (int row = 0; row <= 3; ++row) {
                    if (board[row][col] != 0) {
                        std::swap(board[target][col], board[row][col]);
                        if (target != row) board[row][col] = 0;
                        ++target;
                    }
                }
            }
            break;
        case Direction::LEFT:
            for (int row = 0; row < 4; ++row) {
                int target = 0;
                for (int col = 0; col <= 3; ++col) {
                    if (board[row][col] != 0) {
                        std::swap(board[row][target], board[row][col]);
                        if (target != col) board[row][col] = 0;
                        ++target;
                    }
                }
            }
            break;
        case Direction::RIGHT:
            for (int row = 0; row < 4; ++row) {
                int target = 3;
                for (int col = 3; col >= 0; --col) {
                    if (board[row][col] != 0) {
                        std::swap(board[row][target], board[row][col]);
                        if (target != col) board[row][col] = 0;
                        --target;
                    }
                }
            }
            break;
    }
}

int GameBoard::merge(Direction direction) {
    int moveScore = 0;
    switch (direction) {
        case Direction::UP:
            for (int col = 0; col < 4; ++col) {// 按列遍历
                for (int row = 3; row > 0; --row) {// 从顶行向下遍历,但是不考虑最后一行
                    if (board[row][col] != 0 && board[row][col] == board[row - 1][col]) {// 若当前位置有数字并且下面一行有相同数字
                        board[row][col] *= 2;// 原位置改为合并后
                        moveScore += board[row][col];// #？得分记作合成前数字加
                        board[row - 1][col] = 0;// 下一行清除
                    }
                }
            }
            break;
        case Direction::DOWN:
            for (int col = 0; col < 4; ++col) {
                for (int row = 0; row < 3; ++row) {
                    if (board[row][col] != 0 && board[row][col] == board[row + 1][col]) {
                        board[row][col] *= 2;
                        moveScore += board[row][col];
                        board[row + 1][col] = 0;
                    }
                }
            }
            break;
        case Direction::LEFT:
            for (int row = 0; row < 4; ++row) {
                for (int col = 0; col < 3; ++col) {
                    if (board[row][col] != 0 && board[row][col] == board[row][col + 1]) {
                        board[row][col] *= 2;
                        moveScore += board[row][col];
                        board[row][col + 1] = 0;
                    }
                }
            }
            break;
        case Direction::RIGHT:
            for (int row = 0; row < 4; ++row) {
                for (int col = 3; col > 0; --col) {
                    if (board[row][col] != 0 && board[row][col] == board[row][col - 1]) {
                        board[row][col] *= 2;
                        moveScore += board[row][col];
                        board[row][col - 1] = 0;
                    }
                }
            }
            break;
    }

    return moveScore;
}

bool GameBoard::isGameOver() const {
    // Check if there are any empty tiles
    // TODO
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if(board[row][col] == 0)
            return false;
        }  
    }
    // Check if any adjacent tiles can merge
    // TODO
    // 检查相邻是否有可合并
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            // 检查右侧相邻
            if (col < 3 && board[row][col] == board[row][col + 1])
                return false;
            // 检查下方相邻
            if (row < 3 && board[row][col] == board[row + 1][col])
                return false;
        }
    }

    return true;
}

bool GameBoard::hasWinningTile() const {
    // Check if there is a tile with the value 2048
    // TODO
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if(board[row][col] == 16)
            return true;
        }  
    }
    return false;
}


const Board &GameBoard::getBoard() const {
    return board;
}