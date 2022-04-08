import cv2

s_pic = "./../2019-10-26T03-26-55_M51_Clear_280s_big.jpg"
o_pic = cv2.imread(s_pic)

# o_pic = array with shape [height,width] 
# -> height-array: [pixel, pixel, pixel,...] 
# -> width-array: same
# pixel-array: [subpixel1,subpixel2,subpixel3]

n_pic_height = o_pic.shape[0] 
n_pic_width = o_pic.shape[1]
print(n_pic_height)

