from re import I
import cv2 
import os #interaction with operating system
import random 
import numpy

b_os_is_windows = os.name == 'nt'

s_path_file_name = './../2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W.jpg'
s_path_output_file = "static.jpg" # static filename
s_path_output_file = __file__.split(".")[0] +".jpg"

if(b_os_is_windows):
    s_path_file_name = "\\".join(s_path_file_name.split("/"))

o_img = cv2.imread(s_path_file_name) #cv2.imread() loads an image from specified file

a_img = o_img
n_img_width = o_img.shape[0]
n_img_height = o_img.shape[1]

#optional resize 
n_resize_factor = 1.0 # no resizing
n_resize_factor = 0.1 # 10 times smallerj
n_img_width = int(n_img_width*0.1)
n_img_height = int(n_img_height*0.1)


o_img = cv2.resize(
    o_img,
    (
        n_img_width,
        n_img_height
    ),
    n_resize_factor,
    n_resize_factor
)
# pseudo crop image if width and height is not even
n_img_width_even = int(n_img_width/2) * 2
n_img_height_even = int(n_img_height/2) * 2

n_img_debayered_width = int(n_img_width_even/2 )
n_img_debayered_height = int(n_img_height_even/2)

n_color_channels = 3
o_debayered_img_template = numpy.zeros(
    [
        n_img_debayered_width, 
        n_img_debayered_height, 
        n_color_channels
    ],
    dtype=numpy.uint8
) 
s_prefix = "a_debayered_img_"
o_debayered_img = {
    s_prefix+"n_val1|((n_val2+n_val3)/2)|n_val4|(bggr)": numpy.copy(o_debayered_img_template),
    s_prefix+"n_val4|((n_val2+n_val3)/2)|n_val1|(rggb)": numpy.copy(o_debayered_img_template),
    s_prefix+"n_val1|n_val2|((n_val3+n_val4)/2)|(rgbb)": numpy.copy(o_debayered_img_template),
    # s_prefix+"1_((2+3)/2)_4_(rggb)": numpy.copy(o_debayered_img_template),
    # s_prefix+"1_((2+3)/2)_4_(rrgb)": numpy.copy(o_debayered_img_template),
    # s_prefix+"1_((2+3)/2)_4_(ggrb)": numpy.copy(o_debayered_img_template),
    # s_prefix+"1_((2+3)/2)_4_(bbrg)": numpy.copy(o_debayered_img_template),
}

n_y = 0
while n_y < n_img_height_even:
    n_x = 0
    while n_x < n_img_width_even:
        n_channel_index = 0 # it is rgb but all channels have the same vualue anyway, the image is black white
        n_val1 = a_img[n_y][n_x][n_channel_index]
        n_val2 = a_img[n_y][n_x+1][n_channel_index]
        n_val3 = a_img[n_y+1][n_x][n_channel_index]
        n_val4 = a_img[n_y+1][n_x+1][n_channel_index]

        for s_key in o_debayered_img:
            a_parts = s_key.split(s_prefix)
            s_rgb_part = a_parts.pop()
            a_parts = s_rgb_part.split("|")
            s_r = a_parts[0]
            s_g = a_parts[1]
            s_b = a_parts[2]
            n_r = int(float(eval("int("+s_r+")")))
            n_g = int(float(eval("int("+s_g+")")))
            n_b = int(float(eval("int("+s_b+")")))

            o_debayered_img[s_key][int(n_x/2)][int(n_y/2)] = numpy.array(
                [
                    (numpy.uint8(n_r)),# r
                    (numpy.uint8(n_g)),# g 
                    (numpy.uint8(n_b))# b
                ],
                dtype=numpy.uint8)

        # print(a_debayered_pixel)
        n_x = n_x + 2
    n_y = n_y + 2 



for s_key in o_debayered_img:
    # print(s_key)
    print(o_debayered_img[s_key])
    print(
        cv2.imwrite(s_key.replace("/", "_through_")+".jpg", o_debayered_img[s_key])
    )

# print(o_img)