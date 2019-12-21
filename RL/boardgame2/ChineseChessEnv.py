import copy
import itertools

import numpy as np

from .env import BoardGameEnv

def strfboard(board):
    s = ''
    for i in range(9):
        for j in range(10):
            s += str(board[i][j]) + ' '
    return s

EMPTY = 0
RED = 1  # BLACK
BLACK = -1  # WHITE

PIECE_JIANG = 0
PIECE_SHI = 1
PIECE_XIANG = 2
PIECE_MA = 3
PIECE_JU = 4
PIECE_PAO = 5
PIECE_BING = 6

init_board = np.array(
    [[20, 19, 18, 17, 16, 17, 18, 19, 20],
     [0, 0, 0, 0, 0, 0, 0, 0, 0],
     [0, 21, 0, 0, 0, 0, 0, 21, 0],
     [22, 0, 22, 0, 22, 0, 22, 0, 22],
     [0, 0, 0, 0, 0, 0, 0, 0, 0],
     [0, 0, 0, 0, 0, 0, 0, 0, 0],
     [14, 0, 14, 0, 14, 0, 14, 0, 14],
     [0, 13, 0, 0, 0, 0, 0, 13, 0],
     [0, 0, 0, 0, 0, 0, 0, 0, 0],
     [12, 11, 10, 9, 8, 9, 10, 11, 12]]).T

arr_injiugong = np.array([
    [0, 0, 0, 1, 1, 1, 0, 0, 0],
    [0, 0, 0, 1, 1, 1, 0, 0, 0],
    [0, 0, 0, 1, 1, 1, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 1, 1, 1, 0, 0, 0],
    [0, 0, 0, 1, 1, 1, 0, 0, 0],
    [0, 0, 0, 1, 1, 1, 0, 0, 0]
]).T

jiang_delta = ((1, 0), (0, 1), (-1, 0), (0, -1))
shi_delta = ((1, 1), (-1, 1), (-1, -1), (1, -1))
xiang_delta = ((2, 2), (-2, 2), (-2, -2), (2, -2))
# 马的步长，以帅(将)的步长作为马腿（固定起点）
ma_delta = (((2, -1), (2, 1)), ((1, 2), (-1, 2)),
            ((-2, 1), (-2, -1)), ((-1, -2), (1, -2)))
ma_delta2 = ((2, -1), (2, 1), (1, 2), (-1, 2),
             (-2, 1), (-2, -1), (-1, -2), (1, -2))
# 马将军的步长，以仕(士)的步长作为马腿（固定终点）
ma_check_delta = (((2, 1), (1, 2)), ((-1, 2), (-2, 1)),
                  ((-2, -1), (-1, -2)), ((1, -2), (2, -1)))

ma_pin_dict = {-2: -1, -1: 0, 1: 0, 2: 1}


# jiang_legalmove = {'10', '01', '-10', '0-1'}
# shi_legalmove = {'11', '-11', '-1-1', '1-1'}
# xiang_legalmove = {'22', '-22', '-2-2', '2-2'}


def is_jiang_legalmove(x1, y1, x2, y2):
    dx, dy = x2 - x1, y2 - y1
    return (dx, dy) in jiang_delta
    # s = str(dx) + str(dy)
    # return s in jiang_legalmove


def is_shi_legalmove(x1, y1, x2, y2):
    dx, dy = x2 - x1, y2 - y1
    return (dx, dy) in shi_delta
    # s = str(dx) + str(dy)
    # return s in shi_legalmove


def is_xiang_legalmove(x1, y1, x2, y2):
    dx, dy = x2 - x1, y2 - y1
    return (dx, dy) in xiang_delta
    # s = str(dx) + str(dy)
    # return s in xiang_legalmove


def ma_pin(x1, y1, x2, y2):
    dx, dy = x2 - x1, y2 - y1
    if not (dx, dy) in ma_delta2:
        return x1, y1
    return x1 + ma_pin_dict[dx], y1 + ma_pin_dict[dy]





def is_inboard(x, y):
    return x >= 0 and y >= 0 and x <= 8 and y <= 9


def is_injiugong(x, y):
    return is_inboard(x, y) and arr_injiugong[x][y] == 1


def flip_x(x):
    return 8 - x


def flip_y(y):
    return 9 - y


def flip_xy(x, y):
    return 8 - x, 9 - y


def sq_forward(y, player):
    return y - 1 if player == RED else y + 1


def xiang_pin(x1, y1, x2, y2):
    return (x1 + x2) // 2, (y1 + y2) // 2


# def ma_pin(x1, y1, x2, y2):
#     pass


def is_self_half(y, player):
    return y <= 4 if player == RED else y >= 5


def is_away_half(y, player):
    return is_self_half(y, -player)


def is_same_half(y1, y2):
    return (y1 <= 4 and y2 <= 4) or (y1 >= 5 and y2 >= 5)


def side_tag(player):
    return 8 if player == RED else 16


def opp_side_tag(player):
    return 16 if player == RED else 8


def is_self_chess(pc, player):
    return (side_tag(player) & pc) != 0


def is_opp_chess(pc, player):
    return (opp_side_tag(player) & pc) != 0


def gen_moves(board, player):
    mvs = []
    self_tag, opp_tag = side_tag(player), opp_side_tag(player)
    for x in range(9):
        for y in range(10):
            pc = board[x][y]
            if pc & self_tag == 0:
                continue
            piece = pc - self_tag
            if piece == PIECE_JIANG:
                for i in range(4):
                    dst_x, dst_y = x + jiang_delta[i][0], y + jiang_delta[i][1]
                    if not is_injiugong(dst_x, dst_y):
                        continue
                    pc_dst = board[dst_x][dst_y]
                    if pc_dst & self_tag == 0:
                        mvs.append((x, y, dst_x, dst_y))
            elif piece == PIECE_SHI:
                for i in range(4):
                    dst_x, dst_y = x + shi_delta[i][0], y + shi_delta[i][1]
                    if not is_injiugong(dst_x, dst_y):
                        continue
                    pc_dst = board[dst_x][dst_y]
                    if pc_dst & self_tag == 0:
                        mvs.append((x, y, dst_x, dst_y))
            elif piece == PIECE_XIANG:
                for i in range(4):
                    # 是象眼的位置
                    dst_x, dst_y = x + shi_delta[i][0], y + shi_delta[i][1]
                    if not (is_inboard(dst_x, dst_y) and
                            is_self_half(dst_y, player)
                            and board[dst_x][dst_y] == 0):
                        continue
                    dst_x += shi_delta[i][0]
                    dst_y += shi_delta[i][1]
                    pc_dst = board[dst_x][dst_y]
                    if pc_dst & self_tag == 0:
                        mvs.append((x, y, dst_x, dst_y))
            elif piece == PIECE_MA:
                for i in range(4):
                    pin_x, pin_y = x + jiang_delta[i][0], y + jiang_delta[i][1]
                    if board[pin_x][pin_y] != 0:
                        continue
                    for j in range(2):
                        dst_x, dst_y = x + ma_delta[i][j][0], y + ma_delta[i][j][1]
                        if not is_inboard(dst_x, dst_y):
                            continue
                        pc_dst = board[dst_x][dst_y]
                        if pc_dst & self_tag == 0:
                            mvs.append((x, y, dst_x, dst_y))
            elif piece == PIECE_JU:
                for i in range(4):
                    dst_x, dst_y = x + jiang_delta[i][0], y + jiang_delta[i][1]
                    while is_inboard(dst_x, dst_y):
                        pc_dst = board[dst_x][dst_y]
                        if pc_dst == 0:
                            mvs.append((x, y, dst_x, dst_y))
                        else:
                            if (pc_dst & opp_tag) == 1:
                                mvs.append((x, y, dst_x, dst_y))
                            break
                        dst_x += jiang_delta[i][0]
                        dst_y += jiang_delta[i][1]
            elif piece == PIECE_PAO:
                for i in range(4):
                    dst_x, dst_y = x + jiang_delta[i][0], y + jiang_delta[i][1]
                    while is_inboard(dst_x, dst_y):
                        pc_dst = board[dst_x][dst_y]
                        if pc_dst == 0:
                            mvs.append((x, y, dst_x, dst_y))
                        else:
                            break
                        dst_x += jiang_delta[i][0]
                        dst_y += jiang_delta[i][1]
                    dst_x += jiang_delta[i][0]
                    dst_y += jiang_delta[i][1]
                    while is_inboard(dst_x, dst_y):
                        pc_dst = board[dst_x][dst_y]
                        if pc_dst != 0:
                            if (pc_dst & opp_tag) == 1:
                                mvs.append((x, y, dst_x, dst_y))
                            break
                        dst_x += jiang_delta[i][0]
                        dst_y += jiang_delta[i][1]
            elif PIECE_BING:
                dst_x, dst_y = x, sq_forward(y, player)
                if is_inboard(dst_x, dst_y):
                    pc_dst = board[dst_x][dst_y]
                    if pc_dst & self_tag == 0:
                        mvs.append((x, y, dst_x, dst_y))
                if is_away_half(y, player):
                    for i in (-1, 1):
                        dst_x, dst_y = x + i, y
                        if is_inboard(dst_x, dst_y):
                            pc_dst = board[dst_x][dst_y]
                            if pc_dst & self_tag == 0:
                                mvs.append((x, y, dst_x, dst_y))
    return np.array(mvs)


def is_legalmove(board, x1, y1, x2, y2, player):
    # 1.
    if not is_inboard(x1, y1) or not is_inboard(x2, y2):
        return False
    self_tag = side_tag(player)
    pc_src = board[x1][y1]
    if (pc_src & self_tag) == 0:
        return False
    # 2.
    pc_dst = board[x2][y2]
    if pc_dst & self_tag == 1:
        return False
    # 3.
    piece = pc_src - self_tag
    if piece == PIECE_JIANG:
        return is_injiugong(x2, y2) and is_jiang_legalmove(x1, y1, x2, y2)
    elif piece == PIECE_SHI:
        return is_injiugong(x2, y2) and is_shi_legalmove(x1, y1, x2, y2)
    elif piece == PIECE_XIANG:
        pin_x, pin_y = xiang_pin(x1, y1, x2, y2)
        return is_same_half(y1, y2) and is_xiang_legalmove(x1, y1, x2, y2) \
               and board[pin_x][pin_y] == 0
    elif piece == PIECE_MA:
        pin_x, pin_y = ma_pin(x1, y1, x2, y2)
        return pin_x != x1 and pin_y != y1 and board[pin_x][pin_y] == 0
    elif piece == PIECE_JU or piece == PIECE_PAO:
        dx, dy = 0, 0
        if y1 == y2:
            dx = -1 if x2 < x1 else 1
        elif x1 == x2:
            dy = -1 if y2 < y1 else 1
        else:
            return False
        pin_x, pin_y = x1 + dx, y1 + dy
        while pin_x != x2 and pin_y != y2 and board[pin_x][pin_y] == 0:
            pin_x += dx
            pin_y += dy
        if pin_x == x2 and pin_y == y2:
            return pc_dst == 0 or (pc_src - self_tag == PIECE_JU)
        elif pc_dst != 0 and (pc_src - self_tag == PIECE_PAO):
            pin_x += dx
            pin_y += dy
            while pin_x != x2 and pin_y != y2 and board[pin_x][pin_y] == 0:
                pin_x += dx
                pin_y += dy
            return pin_x == x2 and pin_y == y2
        else:
            return False
    elif piece == PIECE_BING:
        if is_away_half(y2, player) and y1 == y2 \
                and (x2 == x1 - 1 or x2 == x1 + 1):
            return True
        return sq_forward(y1, player) == y2 and x1 == x2
    return False


def is_checked(board, player):
    self_tag = side_tag(player)
    opp_tag = opp_side_tag(player)
    src_x, src_y = 0, 0
    ok = False
    for src_x in range(9):
        for src_y in range(10):
            if board[src_x][src_y] == self_tag + PIECE_JIANG:
                ok = True
                break
        if ok:
            break
    if not ok:
        return False
    # 1.Bing
    if board[src_x][sq_forward(src_y, player)] == opp_tag + PIECE_BING:
        return True
    for dx in (-1, 1):
        if board[src_x + dx][src_y] == opp_tag + PIECE_BING:
            return True
    # 2.Ma
    for i in range(4):
        if board[src_x + shi_delta[i][0]][src_y + shi_delta[i][1]]:
            continue
        for j in range(2):
            pc_dst = board[src_x + ma_check_delta[i][j][0]][src_y + ma_check_delta[i][j][1]]
            if pc_dst == opp_tag + PIECE_MA:
                return True
    # 3.Ju Pao Jiang
    for i in range(4):
        dx, dy = jiang_delta[i][0], jiang_delta[i][1]
        dst_x, dst_y = src_x + dx, src_y + dy
        while is_inboard(dst_x, dst_y):
            pc_dst = board[dst_x][dst_y]
            if pc_dst != 0:
                if pc_dst == opp_tag + PIECE_JIANG or \
                        pc_dst == opp_tag + PIECE_JU:
                    return True
                break
            dst_x += dx
            dst_y += dy
        dst_x += dx
        dst_y += dy
        while is_inboard(dst_x, dst_y):
            pc_dst = board[dst_x][dst_y]
            if pc_dst != 0:
                if pc_dst == opp_tag + PIECE_PAO:
                    return True
                break
            dst_x += dx
            dst_y += dy
    return False


def add_piece(board, x, y, pc):
    board[x][y] = pc


def del_piece(board, x, y, pc):
    board[x][y] = 0


def move_piece(board, x1, y1, x2, y2):
    pc_killed = board[x2][y2]
    if pc_killed != 0:
        del_piece(board, x2, y2, pc_killed)
    pc_src = board[x1][y1]
    del_piece(board, x1, y1, pc_src)
    add_piece(board, x2, y2, pc_src)
    return pc_killed


def undo_move_piece(board, x1, y1, x2, y2, pc_killed):
    pc = board[x2][y2]
    del_piece(board, x2, y2, pc)
    add_piece(board, x1, y1, pc)
    if pc_killed != 0:
        add_piece(board, x2, y2, pc_killed)


def is_mate(board, player):
    mvs = gen_moves(board, player)
    for mv in mvs:
        pc_killed = move_piece(board, mv[0], mv[1], mv[2], mv[3])
        if not is_checked(board, player):
            undo_move_piece(board, mv[0], mv[1], mv[2], mv[3], pc_killed)
            return False
        else:
            undo_move_piece(board, mv[0], mv[1], mv[2], mv[3], pc_killed)
    return True


def is_jiang_exist(board, player):
    self_tag = side_tag(player)
    for x in range(9):
        for y in range(10):
            if board[x][y] == self_tag + PIECE_JIANG:
                return True
    return False


class ChineseChessEnv(BoardGameEnv):

    def __init__(self, board_shape=(9, 10), render_characters='+ox'):
        super().__init__(board_shape=board_shape,
                         illegal_action_mode='resign', render_characters=render_characters,
                         allow_pass=False)

    def reset(self):
        super().reset()
        self.board = init_board
        self.player = RED
        return self.board, self.player

    def is_valid(self, state, action):
        board, player = state
        return is_legalmove(board, action[0], action[1],
                            action[2], action[3], player)

    def get_valid(self, state):
        board, player = state
        return gen_moves(board, player)

    def has_valid(self, state):
        return True

    def get_winner(self, state):
        board, player = state
        if is_mate(board, player):
            return -player
        if not is_jiang_exist(board, player):
            return -player
        if not is_jiang_exist(board, -player):
            return player
        return None

    def get_next_state(self, state, action):
        board, player = state
        move_piece(board, action[0], action[1], action[2], action[3])
        return board, -player
