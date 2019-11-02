from PyQt5.QtWidgets import QWidget
from PyQt5.Qt import QPixmap, QPainter
from PyQt5 import QtCore, QtGui, QtWidgets
from dll_func import *


class QChessBoard(QWidget):
    squareSize = 72
    boardEdge = 8
    # boardWidth = boardEdge + squareSize * 9 + boardEdge
    # boardHeight = boardEdge + squareSize * 10 + boardEdge
    pcStr = ('K', 'A', 'B', 'N', 'R', 'C', 'P', '',
             'RK', 'RA', 'RB', 'RN', 'RR', 'RC', 'RP', '',
             'BK', 'BA', 'BB', 'BN', 'BR', 'BC', 'BP', '')
    selectedStr = ('', 'S')
    pcPath = 'images\\IMAGES_X\\COMIC\\'
    boardJpg = None
    validSquares = (
        51, 52, 53, 54, 55, 56, 57, 58, 59,
        67, 68, 69, 70, 71, 72, 73, 74, 75,
        83, 84, 85, 86, 87, 88, 89, 90, 91,
        99, 100, 101, 102, 103, 104, 105, 106, 107,
        115, 116, 117, 118, 119, 120, 121, 122, 123,
        131, 132, 133, 134, 135, 136, 137, 138, 139,
        147, 148, 149, 150, 151, 152, 153, 154, 155,
        163, 164, 165, 166, 167, 168, 169, 170, 171,
        179, 180, 181, 182, 183, 184, 185, 186, 187,
        195, 196, 197, 198, 199, 200, 201, 202, 203,)

    def __init__(self, parent=None):
        super().__init__(parent)
        self.boardJpg = QtGui.QPixmap('images\\IMAGES_X\\WOOD.JPG')
        self.painter = QPainter()

    def mousePressEvent(self, e: QtGui.QMouseEvent):
        super().mousePressEvent(e)
        x, y = e.x(), e.y()
        x = round((x - 40) / self.squareSize)
        y = round((y - 40) / self.squareSize)
        print(x, y)

    def paintEvent(self, e):
        super().paintEvent(e)
        squares = getSquares()
        sqSelected = getSqSelected()
        self.painter.begin(self)
        self.painter.drawPixmap(0, 0, self.boardJpg)
        for i in self.validSquares:
            pc = squares[i]
            gifPath = None
            if pc != 0:
                gifPath = self.pcPath + self.pcStr[pc] + self.selectedStr[int(sqSelected == i)]
            elif sqSelected == i:
                gifPath = pcPath + 'OOS'
            else:
                continue
            if gifPath:
                gifPath += '.GIF'
                gif = QtGui.QPixmap(gifPath)
                x = i % 16 - 3
                y = i // 16 - 3
                px = self.boardEdge + self.squareSize * x
                py = self.boardEdge + self.squareSize * y
                self.painter.drawPixmap(px, py, gif)
        self.painter.end()
