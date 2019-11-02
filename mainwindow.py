# coding=utf-8
import sys
from ui_mainwindow import *
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.Qt import QPixmap, QPainter
from dll_func import *


class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        self.setWindowTitle('中国象棋')

        # self.labelImg.setText('')
        # self.labelImg.resize(self.boardJpg.width(), self.boardJpg.height())
        # self.labelImg.setPixmap(self.boardJpg)



