from ChessBoard import *
from ChessView import ChessView


class ChessGame:
    board = ChessBoard()
    player_is_red = True

    def __init__(self):
        self.view = ChessView(self)
        self.view.showMsg("Red")
        self.view.draw_board(self.board)

    def start(self):
        self.view.start()

    def callback(self, event):
        print(event.x, event.y)
        rx, ry = Global.coord_real2board(event.x), Global.coord_real2board(event.y)
        if self.board.select(rx, ry, self.player_is_red):
            self.player_is_red = not self.player_is_red
            self.view.showMsg("Red" if self.player_is_red else "Green")
        self.view.draw_board(self.board)


game = ChessGame()
game.start()
