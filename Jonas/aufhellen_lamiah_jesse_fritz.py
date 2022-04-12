from cmath import e
import cv2 
import os
import random 
import numpy as np
import matplotlib.pyplot as plt
from astropy.io import fits as fritz

b_os_is_windows = os.name == 'nt'

s_path_file_name = './../2019-10-26T03-26-55_M51_Clear_280s_big.jpg'

s_path_file_name = './../2019-10-26T03-26-55_M51_Clear_280s.fts'

if(b_os_is_windows):
    s_path_file_name = "\\".join(s_path_file_name.split("/")) #filename.split(separator), sign_between_arrays.join() merges arrays 

# o_img = cv2.imread(s_path_file_name)
counter = 0

o_hdulist = fritz.open(s_path_file_name)
a_img = o_hdulist[0].data
print(np.shape(a_img)) #wie Matrix, nicht verschachtelter array

n_img_height = a_img.shape[0]
n_img_width = a_img.shape[1]

n_below_blacklevel = 0 #threshhold
counter = 0

def MTF(x,m):
    return ((m - 1)*x)/((2*m - 1)*x - m)

# a_img = (MTF(a_img, 1/2)).astype(np.uint16)


# loop over the image, pixel by pixel
for n_y in range(0,n_img_height):
    for n_x in range(0,n_img_width):
        # threshold the pixel
        n_pixel_value = a_img[n_y][n_x]
        a_img[n_y][n_x] = MTF(n_pixel_value, 1/2); 
        print("n_pixel_value:" + str(n_pixel_value))

        # if n_value < n_below_blacklevel:
        #     f_set_pixel_value(a_pixel,0)
        # elif n_value > n_below_blacklevel:
            
        #     f_set_pixel_value(a_pixel, min((MTF(n_value, 1/2)), 255)) #Funktion min() nimmt immer das kleinste der Argumente
       


print(n_img_height)
print(n_img_width)
print(n_img_height*n_img_width)

print(np.max(a_img))

# cv2.imwrite(__file__.split(".")[0] +".fits", o_img)
cv2.imwrite(__file__.split(".")[0] +".jpg", a_img)

# print(o_img)git