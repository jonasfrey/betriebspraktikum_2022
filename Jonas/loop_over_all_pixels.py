from re import I
import cv2 
import os
import random 

b_os_is_windows = os.name == 'nt'

s_path_file_name = './../2019-10-26T03-26-55_M51_Clear_280s_big.jpg'
if(b_os_is_windows):
    s_path_file_name = "\\".join(s_path_file_name.split("/"))

o_img = cv2.imread(s_path_file_name)

def f_set_pixel_value(a_pixel,n_value):
    a_pixel[0] = n_value
    a_pixel[1] = n_value
    a_pixel[2] = n_value

a_img = o_img
n_img_width = o_img.shape[0]
n_img_height = o_img.shape[1]

# threshhold 
n_below_blacklevel = 255/18

n_pixel_count_below_blacklevel = 0

# loop over the image, pixel by pixel
for n_y in range(0,n_img_height):
    for n_x in range(0,n_img_width):
        # threshold the pixel
        a_pixel = a_img[n_y][n_x]
        n_value = a_pixel[0]
        # 
        n_value = int(n_value)

<<<<<<< HEAD
        n_value_normalized = n_value / 100
=======
        n_value_normalized = n_value / 40
>>>>>>> 974530d57113bc9292fd89d039eaf49f95801e4d

        # 12 , 12/32 = 0.375  -> 0.375 * 255 => 95
        # 9  , 9/32  = 0.28   -> 0.28 * 255  => 71
        # 50 , 50/32 = 1.56   -> 1.56 * 255

        f_set_pixel_value(
            a_pixel,
            min(n_value_normalized*255, 255)
        )

        if n_value < n_below_blacklevel:
            n_pixel_count_below_blacklevel = n_pixel_count_below_blacklevel + 1
        # if n_value < n_below_blacklevel:
        #     # f_set_pixel_value(a_pixel,255)
        #     f_set_pixel_value(a_pixel,0)


        # if n_value > n_below_blacklevel:
        #     f_set_pixel_value(
        #         a_pixel,
        #         min(n_value * n_value *0.3, 255)
        #     )



print("n_pixel_count_below_blacklevel")
print(n_pixel_count_below_blacklevel)
cv2.imwrite("loop_pic.jpg", o_img)

# print(o_img)
