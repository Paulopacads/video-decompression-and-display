import numpy as np
import math
import os 
import shutil

# TODO make this function faster ! -> delete for loop
def yuv_to_bgr(yuv_image) : 
    def convert_pixel(y, u, v) :
        y -= 16
        u -= 128
        v -= 128 
        r:int = 1.164 * y + 1.596 * v
        g:int = 1.164 * y - 0.392 * u - 0.813 * v
        b:int = 1.164 * y + 2.017 * u 
        return [b, g, r]

    new_image = np.zeros((480, 720, 3), int)
    for i in range(0, 480) :
        for j in range(0, 720) : 
            y = yuv_image[i, j, 0]
            u = yuv_image[480 + math.floor(i / 2) , math.floor(j / 2), 0]
            v = yuv_image[480 + math.floor(i / 2) , 360 + math.floor(j / 2), 0]
            #print(y, u, v)
            rgb = convert_pixel(y, u, v)
            #print("Rgb :", rgb)
            new_image[i, j] = rgb
    return new_image

def render_all_pgm(video_path) : 
    path_tmp = "tmp"
    if os.path.exists(path_tmp) : 
        shutil.rmtree(path_tmp) #Delete the folder to delete all older files
    os.mkdir(path_tmp)
    os.chdir(path_tmp)
    os.system("./../tools/mpeg2dec/src/mpeg2dec ../" + video_path + " -o pgm")
    os.chdir("../")
    
    all_pgm_path = []
    for root, dirs, files in os.walk(path_tmp):
        for file in files:
            if file.endswith(".pgm") :
                full_path = os.path.join(root, file)
                all_pgm_path.append(full_path)
    
    return all_pgm_path