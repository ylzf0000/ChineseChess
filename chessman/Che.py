import ExpDef
from ChessPiece import ChessPiece


class Che(ChessPiece):

    def get_image_file_name(self):
        if self.selected:
            if self.is_red:
                return ExpDef.image_chess_path + "RRS.GIF"
            else:
                return ExpDef.image_chess_path + "BRS.GIF"
        else:
            if self.is_red:
                return ExpDef.image_chess_path + "RR.GIF"
            else:
                return ExpDef.image_chess_path + "BR.GIF"

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
            if cnt != 0:
                # print 'cannot kill'
                return False
            print('kill a chessman')
        return True

    def __init__(self, x, y, is_red):
        ChessPiece.__init__(self, x, y, is_red)
