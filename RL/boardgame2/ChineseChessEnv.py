import copy
import itertools

import numpy as np

from dll_func import *
from .env import EMPTY
from .env import BoardGameEnv
from .env import is_index

RED = 1
BLACK = -1


class ChineseChessEnv(BoardGameEnv):

    def __init__(self, board_shape=(16, 16), render_characters='+ox'):
        super().__init__(board_shape=board_shape,
                         illegal_action_mode='resign', render_characters=render_characters,
                         allow_pass=False)  # reversi does not allow pass

    def p01_11(self, p):
        return RED if p == 0 else BLACK

    def p11_01(self, p):
        return 0 if p == 1 else 1

    def reset(self):
        super().reset()
        start()
        self.board = getSquares()
        self.player = RED
        return self.board, self.player

    def is_valid(self, state, action):
        """
        Parameters
        ----
        state : (np.array, int)    board and player
        action : np.array   location

        Returns
        ----
        valid : np.array     current valid place for the player
        """
        board, p = state
        return bool(isLegalMove(board, action, self.p11_01(p)))

    def get_valid(self, state):
        board, player = state
        # p_mvs = POINTER(c_int)()
        p_mvs = newPointer2Int(16384)
        n = generateMoves(board, byref(p_mvs), self.p11_01(player))
        mvs = [p_mvs[i] for i in range(n)]
        deletePointer2Int(p_mvs)
        return mvs

    def has_valid(self, state):
        board, p = state
        return bool(hasLegalMove(board, self.p11_01(p)))
        # return len(self.get_valid(state)) > 0

    def get_winner(self, state):
        board, p = state
        s = getGameState()
        if s == 0:
            return None
        elif s == 1 or s == 7:
            return EMPTY
        elif s == 3:
            return p
        elif s == 5:
            return -p
        return None

    def get_next_state(self, state, action):
        board, p = state
        if self.is_valid(state, action):
            board = board[:256]
            makeMove(board, action, self.p11_01(p))
        return board, -self.p11_01(p)
