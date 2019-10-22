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

    def aigo(self):
        for p in self.board.pieces:
            pp = self.board.pieces[p]
            if not pp.is_red:
                moves = pp.get_move_locs(self.board)
                if moves[0]:
                    self.drop(pp.x, pp.y)
                    self.drop(moves[0][0], moves[0][1])
                    return

    def drop(self, rx, ry):
        ok = self.board.select(rx, ry, self.player_is_red)
        if ok:
            self.player_is_red = not self.player_is_red
            self.view.showMsg("Red" if self.player_is_red else "Green")
        self.view.draw_board(self.board)
        return ok

    def callback(self, event):
        print(event.x, event.y)
        rx, ry = Global.coord_real2board(event.x), Global.coord_real2board(event.y)
        if self.drop(rx, ry):
            self.aigo()



game = ChessGame()
game.start()
# while True:
#     game.aigo()
#     game.aigo()
