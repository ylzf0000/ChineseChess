import _thread
import time
import threading

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

    def aigo(self, is_red):
        for k in self.board.pieces:
            p = self.board.pieces[k]
            if p.is_red == is_red:
                moves = p.get_move_locs()
                if moves and moves[0]:
                    # print(self.str_rg[p.is_red], p.name,
                    #       ': from', p.x, p.y, ' to', moves[0][0], moves[0][1])
                    self.select(p.x, p.y)
                    # print(self.board.selected_piece)
                    time.sleep(0.1)
                    self.select(moves[0][0], moves[0][1])
                    self.board.selected_piece = None
                    return
                else:
                    continue

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
            self.aigo(True)
            time.sleep(1)
            self.aigo(False)
            # tkinter.

    def on_click_board(self, event):
        if self.board.is_game_over():
            return
        # print(event.x, event.y)
        rx, ry = Global.coord_real2board(event.x), Global.coord_real2board(event.y)

        if self.select(rx, ry):
            self.aigo(False)


game = ChessGame()
game.start()
