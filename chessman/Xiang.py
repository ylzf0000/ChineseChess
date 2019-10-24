import Global
from chessman.ChessPiece import ChessPiece


class Xiang(ChessPiece):

    def __init__(self, x, y, is_red, board):
        ChessPiece.__init__(self, x, y, is_red, board, 'Xiang')

    green_can_move_list = ((2, 0), (6, 0), (0, 2), (4, 2), (8, 2), (2, 4), (6, 4))
    red_can_move_list = ((2, 5), (6, 5), (0, 7), (4, 7), (8, 7), (2, 9), (6, 9))

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
        lst = self.red_can_move_list if self.is_red else self.green_can_move_list
        for pos in lst:
            dx = pos[0] - self.x
            dy = pos[1] - self.y
            if self.board.has_not_piece_or_diffcolor(pos, self.is_red) and \
                    abs(dx) == 2 and abs(dy) == 2 and \
                    not (self.x + dx / 2, self.y + dy / 2) in self.board.pieces:
                moves.append(pos)
        return moves
