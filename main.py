# coding=utf-8
from ctypes import *
from dll_func import *
from PyQt5.QtWidgets import QApplication, QMainWindow
from MainWindow import *

if __name__ == '__main__':
    mode = 'd'
    init_DLL(mode)
    start()
    app = QApplication(sys.argv)
    mainWindow = MainWindow()
    mainWindow.show()
    sys.exit(app.exec_())
