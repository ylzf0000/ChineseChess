# 全局变量和函数都写在这
import os

# 全局变量
cur_path = os.path.abspath('.')
image_path = 'images/IMAGES_X/'
image_chess_path = 'images/IMAGES_X/COMIC/'
str_rg = ['Green: ', 'Red: ']

# 全局函数
def coord_real2board(x):
    if x <= 40:
        return 0
    else:
        return round((x - 40) / 73)


def coord_board2real(x):
    return 40 + 73 * x
