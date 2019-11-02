import Global
from chessman.ChessPiece import ChessPiece


class Pao(ChessPiece):

    def __init__(self, x, y, is_red, board):
        ChessPiece.__init__(self, x, y, is_red, board, 'Pao')

    def get_image_file_name(self):
        if self.selected:
            if self.is_red:
                return Global.image_chess_path + "RCS.GIF"
            else:
                return Global.image_chess_path + "BCS.GIF"
        else:
            if self.is_red:
                return Global.image_chess_path + "RC.GIF"
            else:
                return Global.image_chess_path + "BC.GIF"

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
                else:
                    p = self.board.has_a_piece(x + dx[i], y + dy[i], dx[i], dy[i])
                    if p and self.board.pieces[p].is_red != self.is_red:
                        moves.append(p)
                    break
                x += dx[i]
                y += dy[i]
        return moves