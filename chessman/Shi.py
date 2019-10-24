import Global

__author__ = 'Zhaoliang'
from chessman.ChessPiece import ChessPiece


class Shi(ChessPiece):
    # green_can_move_dict = dict()
    # green_can_move_dict[3, 0] = True
    # green_can_move_dict[5, 0] = True
    # green_can_move_dict[4, 1] = True
    # green_can_move_dict[3, 2] = True
    # green_can_move_dict[5, 2] = True

    green_can_move_list = ((3, 0), (5, 0), (4, 1), (3, 2), (5, 2))

    # red_can_move_dict = dict()
    # red_can_move_dict[3, 7] = True
    # red_can_move_dict[5, 7] = True
    # red_can_move_dict[4, 8] = True
    # red_can_move_dict[3, 9] = True
    # red_can_move_dict[5, 9] = True

    red_can_move_list = ((3, 7), (5, 7), (4, 8), (3, 9), (5, 9))

    def get_image_file_name(self):
        if self.selected:
            if self.is_red:
                return Global.image_chess_path + "RAS.GIF"
            else:
                return Global.image_chess_path + "BAS.GIF"
        else:
            if self.is_red:
                return Global.image_chess_path + "RA.GIF"
            else:
                return Global.image_chess_path + "BA.GIF"

    def get_move_locs(self):
        moves = []
        lst = self.red_can_move_list if self.is_red else self.green_can_move_list
        for pos in lst:
            dx = self.x - pos[0]
            dy = self.y - pos[1]
            if self.board.has_not_piece_or_diffcolor(pos,self.is_red) and \
                    abs(dx) == 1 and abs(dy) == 1:
                moves.append(pos)
        return moves

    # def can_move(self, board, dx, dy):
    #     nx, ny = self.x + dx, self.y + dy
    #     lst = self.red_can_move_list if self.is_red else self.green_can_move_list
    #     if (nx, ny) not in lst:
    #         return False
    #     return abs(dx) == 1 and abs(dy) == 1

    def __init__(self, x, y, is_red, board):
        ChessPiece.__init__(self, x, y, is_red, board, 'Shi')
