## Mathmatics libraries
import numpy as np
import math
import cv2 

## Image Processing libraries
import skimage
from skimage import exposure

import scipy.misc as misc

import rawpy
import imageio

## Visual and plotting libraries
import matplotlib.pyplot as plt


## Reading a RAW file:
rawImg = rawpy.imread('IMG_7710.CR2')
rgb = rawImg.postprocess(no_auto_bright=True, output_bps=16)
rgb = rawImg.postprocess(output_bps=16)

raw_img_normalized = rawImg.raw_image / pow(2,16)
raw_img_8bit = (raw_img_normalized * pow(2,8)).astype('uint8')

print(raw_img_8bit)

cv2.imwrite("testraw.jpg", raw_img_8bit)

# cv2.imwrite('testraw.jpg', rgb)
# print(rgb)