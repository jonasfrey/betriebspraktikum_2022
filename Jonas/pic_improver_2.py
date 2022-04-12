from cmath import e
import cv2 
import os
import random 
import numpy as np
from astropy.io import fits as fritz
from PIL import Image

b_os_is_windows = os.name == 'nt'

s_path_file_name = './../2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W.fts'
if(b_os_is_windows):
    s_path_file_name = "\\".join(s_path_file_name.split("/")) #filename.split(separator), sign_between_arrays.join() merges arrays 

counter = 0
o_hdulist = fritz.open(s_path_file_name)
a_img = o_hdulist[0].data
print(np.shape(a_img)) #wie Matrix, 2-dimensional verschachtelter Array
n_height = a_img.shape[0]
n_width = a_img.shape[1]
n_below_blacklevel = 0 #threshhold
counter = 0
def f_MTF(n_pixel, m): # 2 Parameter stehen bei Aufrufen zur Verfügung
    x = n_pixel
    return ((m - 1)*x)/((2*m - 1)*x - m)


# loop over the image, pixel by pixel
for n_y in range(0,n_height):
    for n_x in range(0,n_width):
        # threshold the pixel
        n_pixel = a_img[n_y][n_x] #Eintrag an n_x-ter Stelle im Eintrag an n_y-ter Stelle im array a_img
        # 
        
        n_pixel = int(n_pixel)/(2**16)
            

        # if n_pixel < n_below_blacklevel:
        #     f_MTF(a_pixel,0)
        # elif n_pixel > n_below_blacklevel:
            
        n_pixel = f_MTF(n_pixel, 1/2)
        
        a_img[n_y][n_x] = n_pixel*(2**16)
        # a_img[n_y][n_x] = n_pixel*2*(2**16)

        #Funktion min() nimmt immer das kleinste der Argumente
        # if n_value > n_below_blacklevel:
        #     f_set_pixel_value(a_pixel, min(n_value + 20, 255))

#a_img = a_img*2 -> arrays können 
# print(n_img_height)
# print(n_img_width)
# print(n_img_height*n_img_width)
# print(n_value)
# print(np.max(a_img))


#cv2.imwrite(__file__.split(".")[0] +".jpg", a_img)
image=Image.fromarray(a_img,'L')
image.save(__file__.split(".")[0] +".jpg",format='JPEG')
# print(o_img)git
