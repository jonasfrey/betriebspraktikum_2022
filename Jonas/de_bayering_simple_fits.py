import cv2 
import os
import random 
import numpy
from astropy.io import fits as fritz
import rawpy

# sometimes 
b_os_is_windows = os.name == 'nt'

s_path_file_name = './../2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W.fts'
s_path_file_name = './../canon_mars_green_red.CR2'
o_img_raw = rawpy.imread(s_path_file_name).raw_image
# print(o_img_raw.shape)
# exit()

#  s_path_output_file = __file__.split(".")[0]
s_path_output_file = "2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W_debayered.jpg" # static filename
s_path_output_file_suffix = "_default"

if(b_os_is_windows):
    s_path_file_name = "\\".join(s_path_file_name.split("/"))

# o_img = cv2.imread(s_path_file_name)
o_hdulist = fritz.open(s_path_file_name)

o_header = o_hdulist[0].header

# print(o_header.keys)
# exit()

o_img = o_hdulist[0].data

n_height = o_img.shape[0]
n_width = o_img.shape[1]
print("n_height")
print(n_height)
print("n_width")
print(n_width)

# n_height_array = len(o_img)
# n_width_array = len(o_img[0])
# print("n_height_array")
# print(n_height_array)
# print("n_width_array")
# print(n_width_array)

N_ASCII_R = numpy.uint8(ord("r")) #ord gets the ascii number for char 'r'
N_ASCII_G = numpy.uint8(ord("g"))
N_ASCII_B = numpy.uint8(ord("b"))
# [
#     ["r", "g"],
#     ["g", "b"],
# ]
a_color_map = numpy.array(
        [
            numpy.array(
                [N_ASCII_R, N_ASCII_G]
            ),
            numpy.array(
                [N_ASCII_G, N_ASCII_B]
            ),
        ],
        dtype=numpy.uint8 # ascii chars R G B or r g b
    )
n_color_map_height = len(a_color_map)
n_color_map_width = len(a_color_map[0])

def f_s_color_by_xy(n_x, n_y):

    s_color = a_color_map[n_y%n_color_map_height][n_x%n_color_map_width]
    
    return s_color


a_img8 = (o_img / pow(2,8)).astype('uint8')
n_img_height = o_img.shape[0] ## important index 0 [0] is height
n_img_width = o_img.shape[1] ## importatnt index 1 [1] is width

n_color_channels = 3

o_numpy_int_type = numpy.uint8
# o_numpy_int_type = numpy.uint16


o_debayered_img = numpy.zeros(
    [
        n_img_height, 
        n_img_width, 
        n_color_channels
    ],
    dtype=numpy.uint8
)
a_img8_normalized = (a_img8 / pow(2,8))

for n_y in range(0, n_img_height): 
    for n_x in range(0, n_img_width):
        n_val1_normalized = a_img8_normalized[n_y][n_x]
        s_color_current = f_s_color_by_xy(n_x, n_y)
        # o_a_o_color = {
        #     "r": numpy.array([0], o_numpy_int_type),
        #     "g": numpy.array([0], o_numpy_int_type),
        #     "b": numpy.array([0], o_numpy_int_type),
        # }
        # numpy.append(o_a_o_color[s_color_current], n_val1)
        # o_debayered_pixel = numpy.array(
        #         [
        #             o_a_o_color["b"].mean(),
        #             o_a_o_color["g"].mean(),
        #             o_a_o_color["r"].mean()
        #         ],
        # dtype=o_numpy_int_type)

        a_colors_r = numpy.array([0], o_numpy_int_type)
        a_colors_g = numpy.array([0], o_numpy_int_type)
        a_colors_b = numpy.array([0], o_numpy_int_type)
        # # o_a_o_color[s_color_current].append(n_val1)
        if(s_color_current == N_ASCII_R):
            numpy.append(a_colors_r, n_val1_normalized)
        if(s_color_current == N_ASCII_G):
            numpy.append(a_colors_g, n_val1_normalized)
        if(s_color_current == N_ASCII_B):
            numpy.append(a_colors_b, n_val1_normalized)

        
        o_debayered_img[n_y][n_x][0] = n_val1_normalized*pow(2,8) # b
        o_debayered_img[n_y][n_x][1] = 0 # g
        o_debayered_img[n_y][n_x][2] = 0 # r
        # print(a_debayered_pixel)
    if(n_y%100==0):
        print("x|y: "+str(n_x)+"|"+str(n_y))



# for n_y in range(0, n_img_height): 
#     for n_x in range(0, n_img_width):
#         o_debayered_img[n_y][n_x][0] = random.random()*255
#         o_debayered_img[n_y][n_x][1] = random.random()*255
#         o_debayered_img[n_y][n_x][2] = random.random()*255



s_path_imwrite = s_path_output_file+"_"+s_path_output_file_suffix+".jpg"
b_written = cv2.imwrite(s_path_imwrite, o_debayered_img)
if(b_written):
    print("image written to "+ s_path_imwrite)

    

# print(o_img)