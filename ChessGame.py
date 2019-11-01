import _thread
import copy
import sys
import time
import threading

import Alg
from ChessBoard import *
from ChessView import ChessView


class ChessGame:

    def __init__(self):
        self.board = ChessBoard()
        self.player_is_red = True
        self.view = ChessView(self, self.board)
        self.view.showMsg("Red")
        self.view.draw_board()

    def start(self):
        self.view.start()

    def ai_think(self):
        score, move = Alg.AlphaBeta(board=copy.deepcopy(self.board),
                                    depth=2,
                                    alpha=-sys.maxsize - 1,
                                    beta=sys.maxsize,
                                    is_red=self.player_is_red,
                                    is_root=True)
        print(move)
        return move

    def aigo(self):
        move = self.ai_think()
        self.select(move[0][0], move[0][1])
        time.sleep(0.1)
        self.select(move[1][0], move[1][1])

    def select(self, rx, ry):
        ok = self.board.select(rx, ry, self.player_is_red)
        if ok:
            self.player_is_red = not self.player_is_red
            self.view.showMsg("Red" if self.player_is_red else "Green")
        self.view.is_refresh = True
        return ok

    def run_ai2go(self, event):
        threading.Thread(target=self.ai2go, name='ai', daemon=True).start()
        # _thread.start_new_thread(self.ai2go, ())

    def ai2go(self):
        while not self.board.is_game_over():
            time.sleep(1)
            self.aigo()

    def on_click_board(self, event):
        if self.board.is_game_over():
            return
        rx, ry = Global.coord_real2board(event.x), Global.coord_real2board(event.y)
        if self.select(rx, ry):
            threading.Thread(target=self.aigo, name='ai', daemon=True).start()


game = ChessGame()
game.start()
