
import cv2 
import os
import random 
import numpy


# there are black white (single channel) images where each pixel has a brightness value
# each pixel has a certain filter in front of the sensor , red / green or blue, 
# we can combine 4 pixels to get a colored image 
# a_example_not_debayered = [
#     [ R, G, R, G, R, G, R, G],
#     [ G, B, G, B, G, B, G, B],
#     [ R, G, R, G, R, G, R, G],
#     [ G, B, G, B, G, B, G, B],
#     [ R, G, R, G, R, G, R, G],
#     [ G, B, G, B, G, B, G, B]
# ]
# # to combine the colors we take 4(single channel)pixel values, and combine them into 1 (three channel) pixel 
# a_example_not_debayered = [
    
#     [ [R,G,B],[R,G,B],[R,G,B] ],
#     [ [R,G,B],[R,G,B],[R,G,B] ],
#     [ [R,G,B],[R,G,B],[R,G,B] ]

# ]
# # now because we have 2 G values, we can take the arithmetic medium of both => (G+G)/2 => [R,(G+G)/2),B]
# a_example_not_debayered = [
    
#     [ [R,(G+G)/2),B],[R,(G+G)/2),B],[R,(G+G)/2),B] ],
#     [ [R,(G+G)/2),B],[R,(G+G)/2),B],[R,(G+G)/2),B] ],
#     [ [R,(G+G)/2),B],[R,(G+G)/2),B],[R,(G+G)/2),B] ]
    
# ]
# this pattern of 4 filters is called the bayer pattern, 
# it can vary and depends on the sensor
# example of patterns:
# R G G B 
# [..R, G..]
# [..G, B..]
# 
# G B R G 
# [..G, B..]
# [..R, G..]
#  
# B G G R
# [..B, G..]
# [..G, R..]


# sometimes 
b_os_is_windows = os.name == 'nt'

s_path_file_name = './../2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W.jpg'
s_path_output_file = "static.jpg" # static filename
s_path_output_file = __file__.split(".")[0] 

if(b_os_is_windows):
    s_path_file_name = "\\".join(s_path_file_name.split("/"))

o_img = cv2.imread(s_path_file_name)

a_img = o_img
n_img_height = o_img.shape[0] ## important index 0 [0] is height
n_img_width = o_img.shape[1] ## importatnt index 1 [1] is width

#optional resize 
n_resize_factor = 1.0 # no resizing
# n_resize_factor = 0.5 # 10 times smallerj
n_img_width = int(n_img_width*n_resize_factor)
n_img_height = int(n_img_height*n_resize_factor)

o_img = cv2.resize(
    o_img,
    (
        n_img_width,
        n_img_height
    ),
    n_resize_factor,
    n_resize_factor
)

# exit()
# pseudo crop image if width and height is not even
n_img_width_even = int(n_img_width/2) * 2
n_img_height_even = int(n_img_height/2) * 2

n_img_debayered_width = int(n_img_width_even/2)
n_img_debayered_height = int(n_img_height_even/2)

n_color_channels = 3
o_debayered_img = numpy.zeros(
    [
        n_img_debayered_height, 
        n_img_debayered_width, 
        n_color_channels
    ],
    dtype=numpy.uint8
)

def f_o_get_debayered_pixel(
    n_val1,
    n_val2,
    n_val3,
    n_val4,
    s_pattern_name = "rggb"
):
    global s_path_output_file

    s_path_output_file = s_path_file_name+"_"+s_pattern_name

    if(s_pattern_name == "rggb"): 
        o_debayered_pixel = numpy.array(
                [
                    (numpy.uint8(n_val4)),# b
                    (numpy.uint8( int((n_val2+n_val3)/2))),# g
                    (numpy.uint8(n_val1))# r
                ],
        dtype=numpy.uint8)

    if(s_pattern_name == "gbbr"): 
        o_debayered_pixel = numpy.array(
                [
                    (numpy.uint8(n_val1)),# b
                    (numpy.uint8( int((n_val2+n_val3)/2))),# g 
                    (numpy.uint8(n_val4))# r
                ],
        dtype=numpy.uint8)

    if(s_pattern_name == "gbgr"): 
        o_debayered_pixel = numpy.array(
                [
                    (numpy.uint8(n_val2)),# b
                    (numpy.uint8(int((n_val1+n_val3)/2))),# g 
                    (numpy.uint8(n_val4))# r
                ],
        dtype=numpy.uint8)


    return o_debayered_pixel 

n_y = 0
while n_y < n_img_height_even-2:
    n_x = 0
    while n_x < n_img_width_even-2:
        n_channel_index = 0 # it is rgb but all channels have the same vualue anyway, the image is black white
        n_val1 = int(a_img[n_y][n_x][n_channel_index])
        n_val2 = int(a_img[n_y][n_x+1][n_channel_index])
        n_val3 = int(a_img[n_y+1][n_x][n_channel_index])
        n_val4 = int(a_img[n_y+1][n_x+1][n_channel_index])

        # a_debayered_pixel = numpy.array(
        #     [
        #         (numpy.uint8(n_val1)),# r
        #         (numpy.uint8( int((n_val2+n_val3)/2))),# g 
        #         (numpy.uint8(n_val4))# b
        #     ],
        #     dtype=numpy.uint8)
        a_debayered_pixel = f_o_get_debayered_pixel(
            n_val1, 
            n_val2, 
            n_val3, 
            n_val4, 
            "gbgr"
        )
        
        o_debayered_img[int(n_y/2)][int(n_x/2)] = a_debayered_pixel
        # print(a_debayered_pixel)
        n_x = n_x + 2

    n_y = n_y + 2 



cv2.imwrite(s_path_output_file+".jpg", o_debayered_img)
    

# print(o_img)