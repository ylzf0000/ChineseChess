# coding=utf-8
import sys
from ui_mainwindow import *
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.Qt import QPixmap, QPainter
from dll_func import *


class MainWindow(QMainWindow, Ui_MainWindow):
    squareSize = 73
    boardEdge = 20
    boardWidth = boardEdge + squareSize * 9 + boardEdge
    boardHeight = boardEdge + squareSize * 10 + boardEdge
    # 棋子编号
    # enum ENUM_PIECE
    # {
    #     PIECE_JIANG = 0,
    #     PIECE_SHI = 1,
    #     PIECE_XIANG = 2,
    #     PIECE_MA = 3,
    #     PIECE_JU = 4,
    #     PIECE_PAO = 5,
    #     PIECE_BING = 6,
    # };
    pcStr = ('K', 'A', 'B', 'N', 'R', 'C', 'P', '',
             'RK', 'RA', 'RB', 'RN', 'RR', 'RC', 'RP', '',
             'BK', 'BA', 'BB', 'BN', 'BR', 'BC', 'BP', '')
    # playerStr = ('R', 'B')
    selectedStr = ('', 'S')
    pcPath = 'images\\IMAGES_X\\COMIC\\'
    boardJpg = None

    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        self.setWindowTitle('中国象棋')
        self.boardJpg = QtGui.QPixmap('images\\IMAGES_X\\WOOD.JPG')
        self.labelImg.setText('')
        self.labelImg.resize(self.boardJpg.width(), self.boardJpg.height())
        # self.labelImg.setPixmap(self.boardJpg)
        self.painter = QPainter()

    def paintEvent(self, e):
        super(MainWindow, self).paintEvent(e)
        squares = getSquares()
        sqSelected = getSqSelected()
        self.painter.begin(self)
        bx = self.labelImg.geometry().x()
        by = self.labelImg.geometry().y()
        self.painter.drawPixmap(bx + 5, by + 5, self.boardJpg)
        for i in range(51, 204):
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
                # boardWidth = boardEdge + squareSize * 9 + boardEdge
                # boardHeight = boardEdge + squareSize * 10 + boardEdge
                x = i % 16 - 3
                y = i // 16 - 3
                px = self.boardEdge + self.squareSize * x
                py = self.boardEdge + self.squareSize * y
                self.painter.drawPixmap(px, py, gif)

        self.painter.end()
