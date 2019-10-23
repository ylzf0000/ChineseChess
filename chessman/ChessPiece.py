class ChessPiece:
    selected = False
    is_king = False

    def __init__(self, x, y, is_red, board):
        self.x = x
        self.y = y
        self.is_red = is_red
        self.board = board

    def get_move_locs(self):
        moves = []
        for x in range(9):
            for y in range(10):
                if (x, y) in self.board.pieces and self.board.pieces[x, y].is_red == self.is_red:
                    continue
                if self.can_move(self.board, x - self.x, y - self.y):
                    moves.append((x, y))
        return moves

    def move(self, dx, dy):
        nx, ny = self.x + dx, self.y + dy
        if (nx, ny) in self.board.pieces:
            self.board.remove(nx, ny)
        self.board.remove(self.x, self.y)
        self.x += dx
        self.y += dy
        self.board.pieces[self.x, self.y] = self
        return True

    def count_pieces(self, board, x, y, dx, dy):
        sx = dx / abs(dx) if dx != 0 else 0
        sy = dy / abs(dy) if dy != 0 else 0
        nx, ny = x + dx, y + dy
        x, y = x + sx, y + sy
        cnt = 0
        while x != nx or y != ny:
            if (x, y) in board.pieces:
                cnt += 1
            x += sx
            y += sy
        return cnt
