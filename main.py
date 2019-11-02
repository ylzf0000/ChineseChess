# coding=utf-8
from ctypes import *


def init_DLL(mode):
    dll_path = "dll_chinesechess\\x64\\Debug\\dll_chinesechess.dll" \
        if mode == 'd' else \
        "dll_chinesechess\\x64\\Release\\dll_chinesechess.dll"
    dll = CDLL(dll_path)
    # 同步C函数签名
    dll.aiMove.restype = None
    dll.start.restype = None
    dll.getPlayer.restype = c_int
    dll.getSquares.restype = POINTER(c_ubyte)
    dll.getValRed.restype = c_int
    dll.getValBlack.restype = c_int
    dll.getNStep.restype = c_int
    dll.getSqSelected.restype = c_int
    dll.getMvLast.restype = c_int
    dll.isFlipped.restype = c_int
    return dll


if __name__ == '__main__':
    mode = 'd'
    dll = init_DLL(mode)
    dll.start()
    dll.aiMove()
    dll.aiMove()
    dll.aiMove()
    dll.aiMove()
    dll.aiMove()
    squares = dll.getSquares()
    for i in range(256):
        print (squares[i])
