import Global
from chessman.ChessPiece import ChessPiece


class Ju(ChessPiece):

    def __init__(self, x, y, is_red, board):
        ChessPiece.__init__(self, x, y, is_red, board, 'Ju')

    def get_image_file_name(self):
        if self.selected:
            if self.is_red:
                return Global.image_chess_path + "RRS.GIF"
            else:
                return Global.image_chess_path + "BRS.GIF"
        else:
            if self.is_red:
                return Global.image_chess_path + "RR.GIF"
            else:
                return Global.image_chess_path + "BR.GIF"

    def get_move_locs(self):
        moves = []
        dx = (1, 0, -1, 0)
        dy = (0, 1, 0, -1)
        for i in range(4):
            x = self.x + dx[i]
            y = self.y + dy[i]
            while True:
                if not self.board.is_pos_legal(x, y):
                    break
                if (x, y) not in self.board.pieces:
                    moves.append((x, y))
                elif self.board.pieces[x, y].is_red != self.is_red:
                    moves.append((x, y))
                    break
                else:
                    break
                x += dx[i]
                y += dy[i]
        return moves
