import Global
from chessman.ChessPiece import ChessPiece


class Bing(ChessPiece):

    def __init__(self, x, y, is_red, board):
        ChessPiece.__init__(self, x, y, is_red, board, 'Bing')

    def get_image_file_name(self):
        if self.selected:
            if self.is_red:
                return Global.image_chess_path + "RPS.GIF"
            else:
                return Global.image_chess_path + "BPS.GIF"
        else:
            if self.is_red:
                return Global.image_chess_path + "RP.GIF"
            else:
                return Global.image_chess_path + "BP.GIF"

    def get_move_locs(self):
        over_river = self.y <= 4 if self.is_red else self.y >= 5
        if not over_river:
            x = self.x
            y = self.y - 1 if self.is_red else self.y + 1
            if self.board.has_not_piece_or_diffcolor((x, y), self.is_red):
                return [(x, y)]
        else:
            dx = (-1, 0, 1)
            dy = (0, -1 if self.is_red else 1, 0)
            moves = []
            for i in range(3):
                x = self.x + dx[i]
                y = self.y + dy[i]
                if self.board.is_pos_legal(x, y) and \
                        self.board.has_not_piece_or_diffcolor((x, y), self.is_red):
                    moves.append((x, y))
            return moves
