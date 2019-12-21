from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QWidget, QMessageBox
from PyQt5.Qt import QPixmap, QPainter


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
        self.oosGif = QtGui.QPixmap('images\\IMAGES_X\\COMIC\\OOS.GIF')
        self.painter = QPainter()
        self.isFlipped = False
        self.gameState = 0
        self.sqSelected = 0
        self.mv_cur = None
        self.mv_last = None

    def mousePressEvent(self, e: QtGui.QMouseEvent):
        super().mousePressEvent(e)
        x, y = e.x(), e.y()
        x = round((x - 40) / self.squareSize) + 3
        y = round((y - 40) / self.squareSize) + 3
        pos = getPos_XY(x, y)
        player = getPlayer()
        pc = getSquares()[pos]
        if isSelfChess(player, pc):
            self.sqSelected = pos
            self.repaint()
            return

        if self.sqSelected != 0 and not isSelfChess(player, pc):
            print(self.sqSelected, pos)
            ret = playerMove(self.sqSelected, pos)
            if ret != 1:
                self.sqSelected = 0
                return
            self.mv_last = self.mv_cur
            self.mv_cur = getMove(self.sqSelected, pos)
            self.sqSelected = 0
            self.repaint()
            self.gameState = getGameState()
            print(self.gameState)
            if self.gameState != 0:
                QMessageBox.information(self, '游戏结束！', '游戏结束！')
                return
            self.mv_last = self.mv_cur
            self.mv_cur = aiCustomMove()
            self.sqSelected = 0
            self.repaint()
            self.gameState = getGameState()
            print(self.gameState)
            if self.gameState != 0:
                QMessageBox.information(self, '游戏结束！', '游戏结束！')
                return


    def printBoard(self):
        print('\nBoard:')
        for y in range(16):
            for x in range(16):
                print(getSquares()[getPos_XY(x, y)], end=',')
            print()

    def drawOOS(self, pos):
        if pos == 0:
            return
        x = pos % 16 -3
        y = pos // 16 -3
        x = self.boardEdge + self.squareSize * x
        y = self.boardEdge + self.squareSize * y
        self.painter.drawPixmap(x, y, self.oosGif)

    def paintEvent(self, e):
        super().paintEvent(e)
        squares = getSquares()
        self.painter.begin(self)
        self.painter.drawPixmap(0, 0, self.boardJpg)
        for i in self.validSquares:
            pc = squares[i]
            gifPath = None
            if pc != 0:
                gifPath = self.pcPath + self.pcStr[pc] + self.selectedStr[int(self.sqSelected == i)]
            else:
                continue
            gifPath += '.GIF'
            gif = QtGui.QPixmap(gifPath)
            x = i % 16 - 3
            y = i // 16 - 3
            px = self.boardEdge + self.squareSize * x
            py = self.boardEdge + self.squareSize * y
            self.painter.drawPixmap(px, py, gif)
        src1 = getSrc(self.mv_last)
        src2 = getSrc(self.mv_cur)
        dst1 = getDst(self.mv_last)
        dst2 = getDst(self.mv_cur)
        self.drawOOS(src1)
        self.drawOOS(src2)
        self.drawOOS(dst1)
        self.drawOOS(dst2)
        self.drawOOS(self.sqSelected)
        self.painter.end()
