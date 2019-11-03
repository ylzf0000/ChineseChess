# coding=utf-8
from ctypes import *
from dll_func import *
from PyQt5.QtWidgets import QApplication, QMainWindow
from mainwindow import *

if __name__ == '__main__':
    # 'd'使用debug模式的DLL,'r'使用release模式的DLL
    mode = 'd'
    init_DLL(mode)
    setMaxDepth(3)
    start()
    app = QApplication(sys.argv)
    mainWindow = MainWindow()
    mainWindow.show()
    sys.exit(app.exec_())
