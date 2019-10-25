import copy
import sys
import traceback

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
    w = (1500, 30, 30, 100, 300, 200, 100, -1000, -20, -20, -80, -260, -200, -90)
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
    pieces_copy = copy.deepcopy(board.pieces)
    for pos, piece in pieces_copy.items():
        locs = piece.get_move_locs()
        if not piece or not locs or piece.is_red != is_red:
            continue
        for mv_loc in locs:
            before_move_piece1 = pieces_copy[pos]
            before_move_piece2 = pieces_copy[mv_loc] if mv_loc in pieces_copy else None
            board.pieces[pos].move(mv_loc[0] - piece.x, mv_loc[1] - piece.y)
            print(depth, alpha, beta, is_red, pos)
            val = -AlphaBeta(board, depth - 1, -beta, -alpha, not is_red)[0]
            board.pieces[pos] = copy.deepcopy(before_move_piece1)
            if before_move_piece2 != None:
                board.pieces[mv_loc] = copy.deepcopy(before_move_piece2)
            if val >= beta:
                return beta, None
            if val > alpha:
                alpha = val
                if is_root:
                    move = (pos, mv_loc)

    return alpha, move
