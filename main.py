from ctypes import *

if __name__ == '__main__':
    mode = 'd'
    dll_path = "dll_chinesechess\\x64\\Debug\\dll_chinesechess.dll" \
        if mode == 'd' else \
        "dll_chinesechess\\x64\\Release\\dll_chinesechess.dll"
    dll = CDLL(dll_path)
    dll.Start()
    dll.AIMove()
    dll.AIMove()
    dll.AIMove()
    dll.AIMove()
    dll.AIMove()
