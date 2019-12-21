# coding=utf-8
from ctypes import *

import RL
from RL.agent import train
from PyQt5.QtWidgets import QApplication, QMainWindow
from mainwindow import *
if __name__ == '__main__':
    # input()


    # pp[1] = 122

    RL.agent.train()
    # setMaxDepth(4)
    # start()
    # app = QApplication(sys.argv)
    # mainWindow = MainWindow()
    # mainWindow.show()
    # sys.exit(app.exec_())
