from cmath import e
import cv2 
import os
import random 
import numpy as np
import matplotlib.pyplot as plt

b_os_is_windows = os.name == 'nt'

s_path_file_name = './../2019-10-26T03-26-55_M51_Clear_280s_big.jpg'
if(b_os_is_windows):
    s_path_file_name = "\\".join(s_path_file_name.split("/"))

o_img = cv2.imread(s_path_file_name)


a_img = o_img
n_img_width = o_img.shape[0]
n_img_height = o_img.shape[1]
n_below_blacklevel = 255/18 #threshhold
counter = 0
def f_set_pixel_value(a_pixel, n_value): # 2 Parameter stehen bei Aufrufen zur Verfügung
    a_pixel[0] = n_value
    a_pixel[1] = n_value
    a_pixel[2] = n_value

# loop over the image, pixel by pixel
for n_y in range(0,n_img_height):
    for n_x in range(0,n_img_width):
        # threshold the pixel
        a_pixel = a_img[n_y][n_x]
        n_value = a_pixel[0]
        counter = counter + 1
        n_value = int(n_value)

        if n_value < n_below_blacklevel:
            f_set_pixel_value(a_pixel,0)
        elif n_value > n_below_blacklevel:
            
            f_set_pixel_value(a_pixel, min(n_value + np.log(n_value)), 255)) #Funktion min() nimmt immer das kleinste der Argumente
        # if n_value > n_below_blacklevel:
        #     f_set_pixel_value(a_pixel, min(n_value + 20, 255))

print(counter)
print(n_img_height)
print(n_img_width)
print(n_img_height*n_img_width)
print(n_value)


cv2.imwrite(__file__.split(".")[0] +".jpg", o_img)

# print(o_img)git