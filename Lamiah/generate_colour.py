import cv2

s_pic = "./../2019-10-26T03-26-55_M51_Clear_280s_big.jpg"
a_pic = cv2.imread(s_pic)

# o_pic = array with shape [height,width] 
# -> height-array: [pixel, pixel, pixel,...] 
# -> width-array: same
# pixel-array: [subpixel1,subpixel2,subpixel3]

n_pic_height = a_pic.shape[0] 
n_pic_width = a_pic.shape[1]
for i in a_pic:
    for t in i:
        a_pixel = t

    #for x in range(0, n_pic_height - 1):
    #for y in range(0, n_pic_width -1):
    #    a_pixel = y

def f_set_pixel_value(a_pixel):
    n_red = a_pixel[0]*0
    n_green = min(a_pixel[1]**2, 255)
    n_blue = min(a_pixel[2]*3, 255)
    a_pixel[0] = n_red
    a_pixel[1] = n_green
    a_pixel[2] = n_blue*2


for n_y in range(0, n_pic_height-1):
    for n_x in range(0, n_pic_width-1):
        a_pixel = a_pic[n_y][n_x] 
        f_set_pixel_value(a_pixel)
cv2.imwrite("generate_colour_pic.jpg", a_pic) #cv2.imwrite(filename, object) saves the object as filename


