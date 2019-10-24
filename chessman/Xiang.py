import Global
from chessman.ChessPiece import ChessPiece


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

    def get_move_locs(self):
        moves = []
        green_can_move_list = ((2, 0),
                               (6, 0),
                               (0, 2),
                               (4, 2),
                               (8, 2),
                               (2, 4),
                               (6, 4))

        red_can_move_list = ((2, 5),
                             (6, 5),
                             (0, 7),
                             (4, 7),
                             (8, 7),
                             (2, 9),
                             (6, 9))
        lst = red_can_move_list if self.is_red else green_can_move_list
        for pos in lst:
            dx = pos[0] - self.x
            dy = pos[1] - self.y
            if self.board.has_not_piece_or_diffcolor(pos, self.is_red) and \
                    abs(dx) == 2 and abs(dy) == 2 and \
                    not (self.x + dx / 2, self.y + dy / 2) in self.board.pieces:
                moves.append(pos)
        return moves

    # def can_move(self, board, dx, dy):
    #     x, y = self.x, self.y
    #     nx, ny = x + dx, y + dy
    #     if (self.is_red and ny <5) or (not self.is_red and ny > 4):
    #         # print 'no river cross'
    #         return False
    #
    #     if abs(dx) != 2 or abs(dy) != 2:
    #         # print 'not normal'
    #         return False
    #     sx, sy = dx / abs(dx), dy / abs(dy)
    #     if (x + sx, y + sy) in board.pieces:
    #         # print 'blocked'
    #         return False
    #     return True

    def __init__(self, x, y, is_red, board):
        ChessPiece.__init__(self, x, y, is_red, board, 'Xiang')
