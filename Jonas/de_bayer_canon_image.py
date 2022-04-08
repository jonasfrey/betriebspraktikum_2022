## Mathmatics libraries
import numpy as np
import math
import cv2 

## Image Processing libraries
import skimage
from skimage import exposure

import scipy.misc as misc

import numpy 

import rawpy
import imageio

## Visual and plotting libraries
import matplotlib.pyplot as plt


## Reading a RAW file:
# rawImg = rawpy.imread('IMG_7710.CR2')
rawImg = rawpy.imread('./../canon_mars_green_red.CR2')
rgb = rawImg.postprocess(no_auto_bright=True, output_bps=16)
rgb = rawImg.postprocess(output_bps=16)

raw_img_normalized = rawImg.raw_image / pow(2,16)
raw_img_8bit = (raw_img_normalized * pow(2,8)).astype('uint8')

print(raw_img_8bit)
print(raw_img_normalized.shape)

n_height = raw_img_normalized.shape[0]
n_width = raw_img_normalized.shape[1]

n_height = int(n_height/2) * 2
n_width = int(n_width/2) * 2

# cv2.imwrite("testraw.jpg", raw_img_8bit)

n_img_debayered_height = int(n_height / 2)
n_img_debayered_width = int(n_width / 2)
n_color_channels = 3

o_debayered_img = numpy.zeros(
    [
        n_img_debayered_height, 
        n_img_debayered_width, 
        n_color_channels
    ],
    dtype=numpy.uint8
)



n_y = 0
while n_y < n_height-2:
    n_x = 0
    while n_x < n_height-2:
        n_channel_index = 0 # it is rgb but all channels have the same vualue anyway, the image is black white
        n_val1 = raw_img_normalized[n_y][n_x]
        n_val2 = raw_img_normalized[n_y][n_x+1]
        n_val3 = raw_img_normalized[n_y+1][n_x]
        n_val4 = raw_img_normalized[n_y+1][n_x+1]


        a_debayered_pixel = numpy.array(
                [   
                    int(n_val1 * 255),
                    int(n_val2 * 255),
                    int(n_val3 * 255),
                ],
        dtype=numpy.uint8)
        
        o_debayered_img[int(n_y/2)][int(n_x/2)] = a_debayered_pixel
        # print(a_debayered_pixel)
        n_x = n_x + 2

    n_y = n_y + 2 


cv2.imwrite('o_debayered_img_canon_raw.jpg', o_debayered_img)
# print(rgb)