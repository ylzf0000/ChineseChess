class ChessPiece:

    def __init__(self, x, y, is_red, board, name):
        self.selected = False
        self.is_king = False
        self.x = x
        self.y = y
        self.is_red = is_red
        self.board = board
        self.name = name

    def can_move(self, dx, dy):
        return (self.x + dx, self.y + dy) in self.get_move_locs()

    def move(self, dx, dy):
        nx, ny = self.x + dx, self.y + dy
        if (nx, ny) in self.board.pieces:
            self.board.remove(nx, ny)
        self.board.remove(self.x, self.y)
        self.x += dx
        self.y += dy
        self.board.pieces[self.x, self.y] = self
        return True