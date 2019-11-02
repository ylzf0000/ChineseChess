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
    dll_cc.start.restype = None
    dll_cc.getPlayer.restype = c_int
    dll_cc.getSquares.restype = POINTER(c_ubyte)
    dll_cc.getValRed.restype = c_int
    dll_cc.getValBlack.restype = c_int
    dll_cc.getNStep.restype = c_int
    dll_cc.getSqSelected.restype = c_int
    dll_cc.getMvLast.restype = c_int
    dll_cc.isFlipped.restype = c_int


def aiMove():
    dll_cc.aiMove()


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


def getMvLast():
    return dll_cc.getMvLast()


def isFlipped():
    return dll_cc.isFlipped()
