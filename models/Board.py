#!/usr/bin/env python3

#
# EPITECH PROJECT, 2024
# GomokuAI
# File description:
# Board class
#

import array

class Board:
    def __init__(self):
        """
            Constructor of the Board class
            Initializes the board with a size of 19x19 (hardcoded)
        """
        self.boardSize = 19
        self.board = None
        self.initBoard()

    def get_board(self) -> array:
        """
            Getter of the board attribute
        """
        return self.board

    def set_board(self, board: array) -> None:
        """
            Setter of the board attribute
        """
        self.board = board

    def get_board_size(self) -> int:
        """
            Getter of the boardSize attribute
        """
        return self.boardSize

    def set_board_size(self, size: int) -> None:
        """
            Setter of the boardSize attribute
        """
        self.boardSize = size
        self.initBoard()

    def get_board_value(self, x: int, y: int) -> int:
        return self.board[x][y]

    def set_board_value(self, x: int, y: int, value: int) -> None:
        self.board[x][y] = value

    def initBoard(self) -> None:
        """
            Initializes the board with boardSize x boardSize dimensions
        """
        self.board = [[0 for _ in range(self.boardSize)] for _ in range(self.boardSize)]
        self.fillBoard()

    def fillBoard(self) -> None:
        """
            Fills the board with 0's
        """
        for i in range(self.boardSize):
            for j in range(self.boardSize):
                self.board[i][j] = 0

    def is_valid_move(self, x, y) -> bool:
        """
            Checks if the move is within bounds and the cell is empty.
        """
        if 0 <= x < self.boardSize and 0 <= y < self.boardSize:
            return self.board[x][y] == 0
        return False

    def display_board(self) -> None:
        """
            Displays the board in a readable format.
        """
        if self.board is not None:
            for row in self.board:
                print(" ".join(str(cell) for cell in row))
        else:
            print("Board is empty")
