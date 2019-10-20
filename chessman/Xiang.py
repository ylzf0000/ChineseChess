import Global
from ChessPiece import ChessPiece


class Xiang(ChessPiece):

    def get_image_file_name(self):
        if self.selected:
            if self.is_red:
                return Global.image_chess_path + "RBS.GIF"
            else:
                return Global.image_chess_path + "BBS.GIF"
        else:
            if self.is_red:
                return Global.image_chess_path + "RB.GIF"
            else:
                return Global.image_chess_path + "BB.GIF"

    def can_move(self, board, dx, dy):
        x, y = self.x, self.y
        nx, ny = x + dx, y + dy
        if (self.is_red and ny <5) or (not self.is_red and ny > 4):
            # print 'no river cross'
            return False

        if abs(dx) != 2 or abs(dy) != 2:
            # print 'not normal'
            return False
        sx, sy = dx / abs(dx), dy / abs(dy)
        if (x + sx, y + sy) in board.pieces:
            # print 'blocked'
            return False
        return True

    def __init__(self, x, y, is_red):
        ChessPiece.__init__(self, x, y, is_red)
