# coding=utf-8
from ctypes import *

dll_cc = None


def init_DLL(platform, cfg):
    platform = 'x64' if platform == '64' else 'Win32'
    cfg = 'Release' if cfg == 'r' else 'Debug'
    dll_path = 'dll_chinesechess/' + platform + '/' + cfg + '/dll_chinesechess.dll'
    # dll_path = "dll_chinesechess\\x64\\Debug\\dll_chinesechess.dll" \
    #     if mode == 'd' else \
    #     "dll_chinesechess\\x64\\Release\\dll_chinesechess.dll"
    global dll_cc
    dll_cc = CDLL(dll_path)
    # 同步C函数签名
    dll_cc.aiMove.restype = c_int
    dll_cc.playerMove.restype = c_int
    dll_cc.playerMove.argtypes = (c_int, c_int)
    dll_cc.start.restype = None
    dll_cc.getPlayer.restype = c_int
    dll_cc.getSquares.restype = POINTER(c_ubyte)
    dll_cc.getValRed.restype = c_int
    dll_cc.getValBlack.restype = c_int
    dll_cc.getNStep.restype = c_int
    dll_cc.isSelfChess.restype = c_int
    dll_cc.isSelfChess.argtypes = (c_int, c_int)
    dll_cc.isOppChess.restype = c_int
    dll_cc.isOppChess.argtypes = (c_int, c_int)


def aiCustomMove():
    return dll_cc.aiMove()


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


def isSelfChess(player, pc):
    return dll_cc.isSelfChess(player, pc)


def isOppChess(player, pc):
    return dll_cc.isOppChess(player, pc)


def getPos_XY(x, y):
    return dll_cc.getPos_XY(x, y)


def setMaxDepth(d):
    dll_cc.setMaxDepth(d)


def getMaxDepth():
    return dll_cc.getMaxDepth()


def isMate():
    return dll_cc.isMate()


def getSrc(mv):
    return dll_cc.getSrc(mv)


def getDst(mv):
    return dll_cc.getDst(mv)


def getMove(src, dst):
    return dll_cc.getMove(src, dst)

def getRepStatus():
    return dll_cc.getRepStatus()

def getGameState():
    return dll_cc.getGameState()