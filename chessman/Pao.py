import Global
from chessman.ChessPiece import ChessPiece


class Pao(ChessPiece):

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

    def can_move(self, board, dx, dy):
        if dx != 0 and dy != 0:
            # print 'no diag'
            return False
        nx, ny = self.x + dx, self.y + dy
        cnt = self.count_pieces(board, self.x, self.y, dx, dy)
        print('cnt', cnt)
        if (nx, ny) not in board.pieces:
            if cnt != 0:
                # print 'blocked'
                return False
        else:
            if cnt != 1:
                # print 'cannot kill'
                return False
        return True

    def __init__(self, x, y, is_red, board):
        ChessPiece.__init__(self, x, y, is_red, board)
