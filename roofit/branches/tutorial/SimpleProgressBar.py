class SimpleProgressBar:
    """
    Simple progress bar:
    b = SimpleProgressBar(40,1000)
    print b.show(700)
    """
    def __init__(self,len,maxval):
        self.len = len
        self.maxval = maxval
        self.chars = (' ', '+')
        self.wrap = ('[', ']')
        self.filledc = 0
        self.blank = 0

    def _setP(self, p):
        self.filledc = int(round(float(self.len*p/self.maxval))) if p < self.maxval else self.len
        self.blank = self.len - self.filledc

    def show(self,v):
        if len:
            self._setP(v)
        out = []
        out.append(self.wrap[0])
        out.append(self.filledc*self.chars[1])
        out.append(self.blank*self.chars[0])
        out.append(self.wrap[1])
        return "".join(out)

