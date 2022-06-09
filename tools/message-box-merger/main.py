import sys
from PIL import Image

CENTER = Image.open('center.bmp')
N = Image.open('N.bmp')
S = Image.open('S.bmp')
W = Image.open('W.bmp')
E = Image.open('E.bmp')
NW = Image.open('NW.bmp')
NE = Image.open('NE.bmp')
SW = Image.open('SW.bmp')
SE = Image.open('SE.bmp')
WIDTH = 32
HEIGHT = 32

def do():
    if len(sys.argv) != 3:
        print('Bad argument count')
        return
    width = int(sys.argv[1])
    height = int(sys.argv[2])
    result = Image.new('RGBA', (width*WIDTH, height*HEIGHT), (0, 0, 0, 0))
    result.paste(NW, (0, 0))
    result.paste(NE, ((width-1)*WIDTH, 0))
    result.paste(SW, (0, (height-1)*HEIGHT))
    result.paste(SE, ((width-1)*WIDTH, (height-1)*HEIGHT))
    # left
    for i in range(height-2):
        result.paste(W, (0, (i+1)*HEIGHT))
    # right
    for i in range(height-2):
        result.paste(E, ((width-1)*WIDTH, (i+1)*HEIGHT))
    # top
    for i in range(width-2):
        result.paste(N, ((i+1)*WIDTH, 0))
    # bottom
    for i in range(width-2):
        result.paste(S, ((i+1)*WIDTH, (height-1)*HEIGHT))
    # center
    for i in range(height-2):
        for j in range(width-2):
            result.paste(CENTER, ((j+1)*WIDTH, (i+1)*HEIGHT))
    
    result.save('result.bmp')

do()

