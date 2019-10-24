import copy

from chessman.Bing import Bing
from chessman.Ju import Ju
from chessman.Ma import Ma
from chessman.Pao import Pao
from chessman.Shi import Shi
from chessman.Shuai import Shuai
from chessman.Xiang import Xiang


def EvaluateBoard(board, is_red):
    Types = (Shuai, Shi, Xiang, Ma, Ju, Pao, Bing)
    x = []
    w = (1000, 10, 10, 10, 10, 10, 10, -1100, -20, -20, -20, -20, -20, -20)
    for type in Types:
        x.append(board.num_piece(is_red, type))
    for type in Types:
        x.append(board.num_piece(not is_red, type))
    val = 0
    for i in range(len(w)):
        val += w[i] * x[i]
    return val


def AlphaBeta(board, depth, alpha, beta, is_red, is_root=False):
    if depth == 0:
        return EvaluateBoard(board, is_red), None

    move = None
    board_copy = copy.deepcopy(board)
    for _, _p in board.pieces.items():
        if not _p.get_move_locs() or _p.is_red != is_red:
            continue
        for mv in _p.get_move_locs():
            p = board_copy.pieces[_p.x, _p.y]
            print(p.name, 'from', p.x, p.y)
            print('to', mv[0], mv[1])
            p1 = copy.deepcopy(p)
            p2 = None
            if (mv[0],mv[1]) in board_copy.pieces:
                p2 = copy.deepcopy(board_copy.pieces[mv])
            p.move(mv[0] - p.x, mv[1] - p.y)
            val = -AlphaBeta(board_copy, depth - 1, -beta, -alpha, not is_red)[0]
            board_copy.pieces[_p.x, _p.y] = p1
            board_copy.pieces[mv] = p2 if p2 else None
            # print(val)
            # val = -(val[0])
            if val >= beta:
                return beta, None
            if val > alpha:
                alpha = val
                if is_root:
                    move = ((_p.x, _p.y), (mv[0], mv[1]))

    return alpha, move
