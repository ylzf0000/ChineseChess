from ctypes import *

if __name__ == '__main__':
    mode = 'd'
    dll = CDLL("dll_chinesechess\\x64\\Debug\\dll_chinesechess.dll") \
        if mode == 'd' else CDLL("dll_chinesechess\\x64\\Release\\dll_chinesechess.dll")
    dll.Start()
    dll.AIMove()
    dll.AIMove()
    dll.AIMove()
    dll.AIMove()
    dll.AIMove()
