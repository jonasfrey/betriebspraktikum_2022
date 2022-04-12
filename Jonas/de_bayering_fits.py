import cv2 
import os
import random 
import numpy
from astropy.io import fits as fritz
import rawpy
from PIL import Image


# sometimes 
b_os_is_windows = os.name == 'nt'

s_path_file_name = './../2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W.fts'
# s_path_output_file = __file__.split(".")[0] 
s_path_output_file = "2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W_debayered.jpg" # static filename
s_path_output_file_suffix = "_default"

if(b_os_is_windows):
    s_path_file_name = "\\".join(s_path_file_name.split("/"))


# o_img = cv2.imread(s_path_file_name)
o_hdulist = fritz.open(s_path_file_name)
o_header = o_hdulist[0].header




# print(o_header.keys)
# exit()

a_img = o_hdulist[0].data

s_path_file_name = './../canon_mars_green_red.CR2'
o_img_raw = rawpy.imread(s_path_file_name).raw_image
a_img = o_img_raw


n_height = a_img.shape[0]
n_width = a_img.shape[1]

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


n_color_channels = 3
o_numpy_int_type = numpy.uint8
# o_numpy_int_type = numpy.uint16
a_img_debayered_uint8 = numpy.zeros(
    [
        n_height, 
        n_width, 
        n_color_channels
    ],
    dtype=numpy.uint8, 

)

a_img_normalized = a_img / (pow(2,16)-1)

n_kernel_width = 3
n_kernel_height = 3

for n_y in range(0, n_height):
    print("n_y/(n_height):"+str(n_y)+"/"+str(n_height))
    for n_x in range(0, n_width):
        a_colors_r_uint8 = numpy.array([], numpy.uint8)
        a_colors_g_uint8 = numpy.array([], numpy.uint8)
        a_colors_b_uint8 = numpy.array([], numpy.uint8)

        for n_y_k in range(0, n_kernel_height):
            for n_x_k in range(0, n_kernel_width):
                # n_x_pok => n_x pixel on kernel
                n_x_pok = n_x + n_x_k - int(n_kernel_width/2)
                n_y_pok = n_y + n_y_k - int(n_kernel_height/2)
                if(
                    n_x_pok < 0 or
                    n_y_pok < 0 or
                    n_x_pok >= n_width or
                    n_y_pok >= n_height
                ):
                    continue
                # print("n_y_pok: "+str(n_y_pok))
                # print("n_x_pok: "+str(n_x_pok))
                n_val_pok = a_img_normalized[n_y_pok][n_x_pok]
                s_color_current = f_s_color_by_xy(n_x_pok, n_y_pok)

                # print("s_color_current: "+chr(s_color_current))
                # print("n_val_pok: "+str(int(n_val_pok*255)))
                if(s_color_current == N_ASCII_R):
                    a_colors_r_uint8 = numpy.append(a_colors_r_uint8, int(n_val_pok*(pow(2,8)-1)))
                if(s_color_current == N_ASCII_G):
                    a_colors_g_uint8 = numpy.append(a_colors_g_uint8, int(n_val_pok*(pow(2,8)-1)))
                if(s_color_current == N_ASCII_B):
                    a_colors_b_uint8 = numpy.append(a_colors_b_uint8, int(n_val_pok*(pow(2,8)-1)))


        # print("a_colors_r_uint8: "+str(numpy.average(a_colors_r_uint8)))
        # print("a_colors_g_uint8: "+str(numpy.average(a_colors_g_uint8)))
        # print("a_colors_b_uint8: "+str(numpy.average(a_colors_b_uint8)))

        
        a_img_debayered_uint8[n_y][n_x][2] = numpy.average(a_colors_r_uint8) # r
        # a_img_debayered_uint8[n_y][n_x][2] = random.random()*255;  
        a_img_debayered_uint8[n_y][n_x][1] = numpy.average(a_colors_g_uint8) # g
        # a_img_debayered_uint8[n_y][n_x][1] = random.random()*255;  
        a_img_debayered_uint8[n_y][n_x][0] = numpy.average(a_colors_b_uint8) # b
        # a_img_debayered_uint8[n_y][n_x][0] = random.random()*255;  




cv2.imwrite(s_path_output_file+"_"+s_path_output_file_suffix+"_cv2.jpg", a_img_debayered_uint8)
image=Image.fromarray(a_img_debayered_uint8,'BGR')
image.save(s_path_output_file+"_"+s_path_output_file_suffix+"_cv2.jpg",format='JPEG')

    