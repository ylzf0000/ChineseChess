# coding=utf-8
from ctypes import *

dll_cc = None


def init_DLL(mode):
    dll_path = "dll_chinesechess\\x64\\Debug\\dll_chinesechess.dll" \
        if mode == 'd' else \
        "dll_chinesechess\\x64\\Release\\dll_chinesechess.dll"
    global dll_cc
    dll_cc = CDLL(dll_path)
    # 同步C函数签名
    dll_cc.aiMove.restype = None
    dll_cc.playerMove.restype = c_int
    dll_cc.playerMove.argtypes = (c_int, c_int)
    dll_cc.start.restype = None
    dll_cc.getPlayer.restype = c_int
    dll_cc.getSquares.restype = POINTER(c_ubyte)
    dll_cc.getValRed.restype = c_int
    dll_cc.getValBlack.restype = c_int
    dll_cc.getNStep.restype = c_int
    dll_cc.getSqSelected.restype = c_int
    dll_cc.setSqselected.restype = c_int
    dll_cc.setSqselected.argtypes = (c_int,)
    dll_cc.getMvLast.restype = c_int
    dll_cc.isFlipped.restype = c_int
    dll_cc.isSelfChess.restype = c_int
    dll_cc.isSelfChess.argtypes = (c_int, c_int)
    dll_cc.isOppChess.restype = c_int
    dll_cc.isOppChess.argtypes = (c_int, c_int)


def aiMove():
    dll_cc.aiMove()


# -1不合法 0走法被将军 1可以走且已经走
def playerMove(src, dst):
    return dll_cc.playerMove(src, dst)


def start():
    dll_cc.start()


def getPlayer():
    return dll_cc.getPlayer()


def getSquares():
    return dll_cc.getSquares()


def getValRed():
    return dll_cc.getValRed()


def getValBlack():
    return dll_cc.getValBlack()


def getNStep():
    return dll_cc.getNStep()


def getSqSelected():
    return dll_cc.getSqSelected()


def setSqselected(pos):
    return dll_cc.setSqselected(pos)


def getMvLast():
    return dll_cc.getMvLast()


def isFlipped():
    return dll_cc.isFlipped()


def isSelfChess(player, pc):
    return dll_cc.isSelfChess(player, pc)


def isOppChess(player, pc):
    return dll_cc.isOppChess(player, pc)


def getPos_XY(x, y):
    return dll_cc.getPos_XY(x, y)
