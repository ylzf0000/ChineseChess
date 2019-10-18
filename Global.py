# 全局变量和函数都写在这
import os

# 全局变量
cur_path = os.path.abspath('.')
image_path = 'images/IMAGES_X/'
image_chess_path = 'images/IMAGES_X/COMIC/'

# 全局函数
def coord_board2real(x):
    if x <= 40:
        return 0
    else:
        return (x - 40) / 72 + 1


def coord_real2board(x):
    return 40 + 72 * x
