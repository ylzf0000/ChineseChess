from ctypes import *




if __name__ == '__main__':
    dll = CDLL("dll_chinesechess\\x64\\Debug\\dll_chinesechess.dll")
    dll.Start()
    dll.AIMove()
