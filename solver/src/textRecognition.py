import cv2
import numpy as np
import pytesseract
from PIL import Image

def textRecognition(s):
    print(pytesseract.image_to_string(Image.open(s)))
