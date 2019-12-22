import copy
import itertools
import sys

import numpy as np

from .env import BoardGameEnv


def strfboard(board):
    s = ''
    for i in range(9):
        for j in range(10):
            s += str(board[i][j]) + ' '
    return s


letters = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i']
dict_letters = {'a': 0, 'b': 1, 'c': 2, 'd': 3, 'e': 4, 'f': 5, 'g': 6, 'h': 7, 'i': 8, }
numbers = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
dict_numbers = {'0': 0, '1': 1, '2': 2, '3': 3, '4': 4,
                '5': 5, '6': 6, '7': 7, '8': 8, '9': 9}


def mv_to_str(x1, y1, x2, y2):
    return letters[x1] + numbers[y1] + letters[x2] + numbers[y2]


def str_to_mv(s):
    return dict_letters[s[0]], dict_numbers[s[1]], \
           dict_letters[s[2]], dict_numbers[s[3]]


dict_mv = {}


# 创建所有合法走子UCI，size 2086
def create_uci_labels():
    labels_array = []
    shi_labels = ['d7e8', 'e8d7', 'e8f9', 'f9e8', 'd0e1', 'e1d0', 'e1f2', 'f2e1',
                  'd2e1', 'e1d2', 'e1f0', 'f0e1', 'd9e8', 'e8d9', 'e8f7', 'f7e8']
    xiang_labels = ['a2c4', 'c4a2', 'c0e2', 'e2c0', 'e2g4', 'g4e2', 'g0i2', 'i2g0',
                    'a7c9', 'c9a7', 'c5e7', 'e7c5', 'e7g9', 'g9e7', 'g5i7', 'i7g5',
                    'a2c0', 'c0a2', 'c4e2', 'e2c4', 'e2g0', 'g0e2', 'g4i2', 'i2g4',
                    'a7c5', 'c5a7', 'c9e7', 'e7c9', 'e7g5', 'g5e7', 'g9i7', 'i7g9']

    for l1 in range(9):
        for n1 in range(10):
            destinations = [(t, n1) for t in range(9)] + \
                           [(l1, t) for t in range(10)] + \
                           [(l1 + a, n1 + b) for (a, b) in
                            [(-2, -1), (-1, -2), (-2, 1), (1, -2), (2, -1), (-1, 2), (2, 1), (1, 2)]]  # 马走日
            for (l2, n2) in destinations:
                if (l1, n1) != (l2, n2) and l2 in range(9) and n2 in range(10):
                    move = letters[l1] + numbers[n1] + letters[l2] + numbers[n2]
                    dict_mv[move] = len(labels_array)
                    labels_array.append(move)

    for p in shi_labels:
        dict_mv[p] = len(labels_array)
        labels_array.append(p)

    for p in xiang_labels:
        dict_mv[p] = len(labels_array)
        labels_array.append(p)

    return labels_array


labels_mv = create_uci_labels()

EMPTY = 0
BLACK = 1  # 红
WHITE = -1  # 黑

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
    return 0 <= x <= 8 and 0 <= y <= 9
    # return x >= 0 and y >= 0 and x <= 8 and y <= 9


def is_injiugong(x, y):
    return is_inboard(x, y) and arr_injiugong[x][y] == 1


def flip_x(x):
    return 8 - x


def flip_y(y):
    return 9 - y


def flip_xy(x, y):
    return 8 - x, 9 - y


def sq_forward(y, player):
    return y - 1 if player == BLACK else y + 1


def xiang_pin(x1, y1, x2, y2):
    return (x1 + x2) // 2, (y1 + y2) // 2


# def ma_pin(x1, y1, x2, y2):
#     pass


def is_self_half(y, player):
    return y >= 5 if player == BLACK else y <= 4


def is_away_half(y, player):
    return y <= 4 if player == BLACK else y >= 5


def is_same_half(y1, y2):
    return (y1 <= 4 and y2 <= 4) or (y1 >= 5 and y2 >= 5)


def side_tag(player):
    return 8 if player == BLACK else 16


def opp_side_tag(player):
    return 16 if player == BLACK else 8


def is_self_chess(pc, player):
    return (side_tag(player) & pc) != 0


def is_opp_chess(pc, player):
    return (opp_side_tag(player) & pc) != 0


def gen_moves(board, player):
    # print(sys._getframe().f_code.co_name)
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
                    if not is_inboard(pin_x, pin_y) or board[pin_x][pin_y] != 0:
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
    # print(sys._getframe().f_code.co_name)
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
    # print(sys._getframe().f_code.co_name)
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
    x, y = src_x, sq_forward(src_y, player)
    if is_inboard(x, y) and board[x][y] == opp_tag + PIECE_BING:
        return True
    for dx in (-1, 1):
        x, y = src_x + dx, src_y
        if is_inboard(x, y) and board[x][y] == opp_tag + PIECE_BING:
            return True
    # 2.Ma
    for i in range(4):
        x, y = src_x + shi_delta[i][0], src_y + shi_delta[i][1]
        if (not is_inboard(x, y)) or \
                (is_inboard(x, y) and board[x][y] != 0):
            continue
        for j in range(2):
            x = src_x + ma_check_delta[i][j][0]
            y = src_y + ma_check_delta[i][j][1]
            if is_inboard(x, y) and board[x][y] == opp_tag + PIECE_MA:
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


def board_to_input(board):
    input_arr = np.zeros(shape=(9, 10, 14))
    for i in range(9):
        for j in range(10):
            if board[i][j] != 0:
                n = board[i][j] - 16 + 7 \
                    if board[i][j] >= 16 else board[i][j] - 8
                input_arr[i][j][n] = 1
    return input_arr


class ChineseChessEnv(BoardGameEnv):

    def __init__(self, board_shape=(9, 10), render_characters='+ox'):
        super().__init__(board_shape=board_shape,
                         illegal_action_mode='pass', render_characters=render_characters,
                         allow_pass=False)

    def reset(self):
        # super().reset()
        self.board = init_board
        self.player = BLACK
        self.depth = 0
        return self.board, self.player

    def is_valid(self, state, action):
        # print(sys._getframe().f_code.co_name)
        board, player = state
        x1, x2, y1, y2 = str_to_mv(labels_mv[action[0]])
        return is_legalmove(board, x1, x2, y1, y2, player)

    def get_valid(self, state):
        # print(sys._getframe().f_code.co_name)
        board, player = state
        mvs = gen_moves(board, player)
        A = np.zeros((2086,), dtype=float)
        for mv in mvs:
            s = mv_to_str(mv[0], mv[1], mv[2], mv[3])
            A[dict_mv[s]] = 1
        return A

    def has_valid(self, state):
        # print(sys._getframe().f_code.co_name)
        return True

    def get_winner(self, state):
        # print(sys._getframe().f_code.co_name)
        board, player = state
        if is_mate(board, player):
            return -player
        if not is_jiang_exist(board, player):
            return -player
        if not is_jiang_exist(board, -player):
            return player
        if self.depth >= 100:
            return 0
        return None

    def get_next_state(self, state, action):
        # print(sys._getframe().f_code.co_name)
        board, player = state
        board = copy.deepcopy(board)
        x1, x2, y1, y2 = str_to_mv(labels_mv[action[0]])
        move_piece(board, x1, x2, y1, y2)
        self.depth += 1
        return board, -player
