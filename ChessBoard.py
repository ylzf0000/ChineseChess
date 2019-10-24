from chessman.Bing import *
from chessman.Ju import *
from chessman.Ma import *
from chessman.Pao import *
from chessman.Shi import *
from chessman.Shuai import *
from chessman.Xiang import *


class ChessBoard:

    def __init__(self):
        self.pieces = dict()
        self.pieces[4, 0] = Shuai(4, 0, False, self)

        self.pieces[0, 3] = Bing(0, 3, False, self)
        self.pieces[2, 3] = Bing(2, 3, False, self)
        self.pieces[4, 3] = Bing(4, 3, False, self)
        self.pieces[6, 3] = Bing(6, 3, False, self)
        self.pieces[8, 3] = Bing(8, 3, False, self)

        self.pieces[1, 2] = Pao(1, 2, False, self)
        self.pieces[7, 2] = Pao(7, 2, False, self)

        self.pieces[3, 0] = Shi(3, 0, False, self)
        self.pieces[5, 0] = Shi(5, 0, False, self)

        self.pieces[2, 0] = Xiang(2, 0, False, self)
        self.pieces[6, 0] = Xiang(6, 0, False, self)

        self.pieces[1, 0] = Ma(1, 0, False, self)
        self.pieces[7, 0] = Ma(7, 0, False, self)

        self.pieces[0, 0] = Ju(0, 0, False, self)
        self.pieces[8, 0] = Ju(8, 0, False, self)

        self.pieces[4, 9] = Shuai(4, 9, True, self)

        self.pieces[0, 6] = Bing(0, 6, True, self)
        self.pieces[2, 6] = Bing(2, 6, True, self)
        self.pieces[4, 6] = Bing(4, 6, True, self)
        self.pieces[6, 6] = Bing(6, 6, True, self)
        self.pieces[8, 6] = Bing(8, 6, True, self)

        self.pieces[1, 7] = Pao(1, 7, True, self)
        self.pieces[7, 7] = Pao(7, 7, True, self)

        self.pieces[3, 9] = Shi(3, 9, True, self)
        self.pieces[5, 9] = Shi(5, 9, True, self)

        self.pieces[2, 9] = Xiang(2, 9, True, self)
        self.pieces[6, 9] = Xiang(6, 9, True, self)

        self.pieces[1, 9] = Ma(1, 9, True, self)
        self.pieces[7, 9] = Ma(7, 9, True, self)

        self.pieces[0, 9] = Ju(0, 9, True, self)
        self.pieces[8, 9] = Ju(8, 9, True, self)

        self.selected_piece = None
        self.kill_king = None

    def is_game_over(self):
        return self.kill_king != None

    def can_move(self, x, y, dx, dy):
        return self.pieces[x, y].can_move(dx, dy)

    def move(self, x, y, dx, dy):
        p = self.pieces[x, y]
        print(Global.str_rg[p.is_red], p.name,
              ': from', p.x, p.y, ' to', x + dx, y + dy)
        return self.pieces[x, y].move(dx, dy)

    def remove(self, x, y):
        del self.pieces[x, y]


    def is_pos_legal(self, x, y):
        return x >= 0 and y >= 0 and x <= 8 and y <= 9

    # 从(cur_x,cur_y)向方向(dx, dy)搜索，看是否有子
    def has_a_piece(self, cur_x, cur_y, dx, dy):
        while self.is_pos_legal(cur_x, cur_y):
            if (cur_x, cur_y) in self.pieces:
                return (cur_x, cur_y)
            cur_x += dx
            cur_y += dy
        return None


    def has_not_piece_or_diffcolor(self, pos, is_red):
        return (pos not in self.pieces) or (self.pieces[pos].is_red != is_red)

    def select(self, x, y, player_is_red):
        if not self.selected_piece:
            if (x, y) in self.pieces and self.pieces[x, y].is_red == player_is_red:
                self.pieces[x, y].selected = True
                self.selected_piece = self.pieces[x, y]
            return False

        if not (x, y) in self.pieces:
            if self.selected_piece:
                ox, oy = self.selected_piece.x, self.selected_piece.y
                if self.can_move(ox, oy, x - ox, y - oy):
                    self.move(ox, oy, x - ox, y - oy)
                    self.pieces[x, y].selected = False
                    self.selected_piece = None
                    return True
            return False

        if self.pieces[x, y].selected:
            return False

        if self.pieces[x, y].is_red != player_is_red:
            ox, oy = self.selected_piece.x, self.selected_piece.y
            if self.can_move(ox, oy, x - ox, y - oy):
                if self.pieces[x, y].is_king:
                    self.kill_king = self.pieces[x, y].name
                    print("Game Over!")
                self.move(ox, oy, x - ox, y - oy)
                self.pieces[x, y].selected = False
                self.selected_piece = None
                return True
            return False
        for key in self.pieces.keys():
            self.pieces[key].selected = False
        self.pieces[x, y].selected = True
        self.selected_piece = self.pieces[x, y]
        return False
