# coding=utf-8
from ctypes import *
from dll_func import *

if __name__ == '__main__':
    mode = 'd'
    init_DLL(mode)
    # Test
    start()
    aiMove()
    getSquares()
    print(getPlayer())
    print(getSquares())
    print(getValRed())
    print(getValBlack())
    print(getNStep())
    print(getSqSelected())
    print(getMvLast())
    print(isFlipped())
    # End Test
