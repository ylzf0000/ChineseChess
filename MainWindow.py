import sys
from ui_mainwindow import *
from PyQt5.QtWidgets import QApplication, QMainWindow


class MainWindow(QMainWindow, Ui_MainWindow):
    squareSize = 73
    boardEdge = 20
    boardWidth = boardEdge + squareSize * 9 + boardEdge
    boardHeight = boardEdge + squareSize * 10 + boardEdge

    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        jpg = QtGui.QPixmap('images\IMAGES_X\WOOD.JPG')
        self.labelImg.resize(jpg.width(), jpg.height())
        self.labelImg.setPixmap(jpg)
