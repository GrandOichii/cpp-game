from PIL import Image
from os import listdir
from os.path import splitext

SEEK_EXT = '.png'
TARGET_EXT = '.bmp'
# SEEK_EXT = '.bmp'
# TARGET_EXT = '.png'

for file in listdir('.'):
    name, ext = splitext(file)
    if ext != SEEK_EXT:
        continue
    img = Image.open(file)
    pixdata = img.load()

    width, height = img.size
    for y in range(height):
        for x in range(width):
            if pixdata[x, y] == (0, 0, 0, 0):
                pixdata[x, y] = (255, 255, 255, 0)
    img.save(name + TARGET_EXT)


