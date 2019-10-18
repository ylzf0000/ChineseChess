import tkinter
import os
import ExpDef

def board_coord(x):
    return 30 + 40*x


class ChessView:
    root = tkinter.Tk()
    root.title("Chinese Chess")
    root.resizable(0, 0)
    can = tkinter.Canvas(root, width=800, height=600)
    can.pack(expand=tkinter.YES, fill=tkinter.BOTH)
    img = tkinter.PhotoImage(file="images/WHITE.GIF")
    can.create_image(0, 0, image=img, anchor=tkinter.NW)
    piece_images = dict()
    move_images = []
    def draw_board(self, board):
        self.piece_images.clear()
        self.move_images = []
        pieces = board.pieces
        for (x, y) in pieces.keys():
            self.piece_images[x, y] = tkinter.PhotoImage(file=pieces[x, y].get_image_file_name())
            self.can.create_image(board_coord(x), board_coord(y), image=self.piece_images[x, y])
        if board.selected_piece:
            for (x, y) in board.selected_piece.get_move_locs(board):
                self.move_images.append(tkinter.PhotoImage(file="images/OOS.GIF"))
                self.can.create_image(board_coord(x), board_coord(y), image=self.move_images[-1])
    def showMsg(self, msg):
        self.root.title(msg)
    def __init__(self, control):
        self.control = control
        self.can.bind('<Button-1>', self.control.callback)

    def start(self):
        tkinter.mainloop()
