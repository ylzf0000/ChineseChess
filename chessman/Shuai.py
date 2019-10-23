import Global
from chessman.ChessPiece import ChessPiece


class Shuai(ChessPiece):
    is_king = True

    def get_image_file_name(self):
        if self.selected:
            if self.is_red:
                return Global.image_chess_path + "RKS.GIF"
            else:
                return Global.image_chess_path + "BKS.GIF"
        else:
            if self.is_red:
                return Global.image_chess_path + "RK.GIF"
            else:
                return Global.image_chess_path + "BK.GIF"

    def can_move(self, board, dx, dy):
        nx, ny = self.x + dx, self.y + dy
        if dx == 0 and self.count_pieces(board, self.x, self.y, dx, dy) == 0 and ((nx, ny) in board.pieces) and \
                board.pieces[nx, ny].is_king:
            return True
        if not (not self.is_red and 3 <= nx <= 5 and 0 <= ny <= 2) and not (
                self.is_red and 3 <= nx <= 5 and 7 <= ny <= 9):
            return False
        if abs(dx) + abs(dy) != 1:
            return False
        return True

    def __init__(self, x, y, is_red, board):
        ChessPiece.__init__(self, x, y, is_red, board)
