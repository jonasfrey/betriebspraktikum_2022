import cv2 
import os
import random 

b_os_is_windows = os.name == 'nt'

s_path_file_name = './../2019-10-26T03-26-55_M51_Clear_280s_big.jpg'
if(b_os_is_windows):
    s_path_file_name = "\\".join(s_path_file_name.split("/"))

o_img = cv2.imread(s_path_file_name)


a_img = o_img
n_img_width = o_img.shape[0]
n_img_height = o_img.shape[1]

# loop over the image, pixel by pixel
for n_y in range(0,n_img_height):
    for n_x in range(0,n_img_width):
        # threshold the pixel
        a_pixel = a_img[n_y][n_x]
        a_pixel[0] = random.random() * 255
        a_pixel[1] = random.random() * 255
        a_pixel[2] = 0



cv2.imwrite(__file__.split(".")[0] +".jpg", o_img)

print(o_img)