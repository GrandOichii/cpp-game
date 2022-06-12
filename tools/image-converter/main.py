from PIL import Image
from os import listdir
from os.path import splitext

SEEK_EXT = '.png'
TARGET_EXT = '.bmp'

for file in listdir('.'):
    name, ext = splitext(file)
    if ext != SEEK_EXT:
        continue
    img = Image.open(file)
    r, g, b, a = img.split()
    img = Image.merge("RGB", (r, g, b))
    img.save(name + TARGET_EXT)


