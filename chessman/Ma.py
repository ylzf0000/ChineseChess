import Global
from chessman.ChessPiece import ChessPiece


class Ma(ChessPiece):

    def get_image_file_name(self):
        if self.selected:
            if self.is_red:
                return Global.image_chess_path + "RNS.GIF"
            else:
                return Global.image_chess_path + "BNS.GIF"
        else:
            if self.is_red:
                return Global.image_chess_path + "RN.GIF"
            else:
                return Global.image_chess_path + "BN.GIF"

    def get_move_locs(self):
        moves = []
        dx = (2, 1, -1, -2, -2, -1, 1, 2)
        dy = (1, 2, 2, 1, -1, -2, -2, -1)
        obstacles = ((1, 0),
                     (0, 1),
                     (0, 1),
                     (-1, 0),
                     (-1, 0),
                     (0, -1),
                     (0, -1),
                     (1, 0))
        for i in range(8):
            x = self.x + dx[i]
            y = self.y + dy[i]
            if not self.board.is_pos_legal(x, y):
                continue
            ox = self.x + obstacles[i][0]
            oy = self.y + obstacles[i][1]
            if self.board.has_not_piece_or_diffcolor((x, y), self.is_red) and \
                    (ox, oy) not in self.board.pieces:
                moves.append((x, y))
        return moves

    # def can_move(self, board, dx, dy):
    #     x, y = self.x, self.y
    #     nx, ny = x + dx, y + dy
    #     if dx == 0 or dy == 0:
    #         # print 'no straight'
    #         return False
    #     if abs(dx) + abs(dy) != 3:
    #         # print 'not normal'
    #         return False
    #     if (x if abs(dx) == 1 else x + dx / 2, y if abs(dy) == 1 else y + (dy / 2)) in board.pieces:
    #         # print 'blocked'
    #         return False
    #     return True

    def __init__(self, x, y, is_red, board):
        ChessPiece.__init__(self, x, y, is_red, board, 'Ma')
