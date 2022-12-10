from utils import *
import sys
import os
import cv2


def main() : 
    print(sys.argv)
    if "help" in sys.argv :
        print("Options : ")
        print("arg1 : path to the video")
        print("ppm : render all frame of video in ppm (will display on screen by default")
    
    path_all_pgm = render_all_pgm(sys.argv[1])
    # Render all frames as ppm in a "result" folder
    if "ppm" in sys.argv : 
        if not os.path.exists("results") :
            os.mkdir("results")
        for path in path_all_pgm : 
            pgm_image = cv2.imread(path)
            ppm_image = yuv_to_bgr(pgm_image)
            new_path = "results/" + path.split('/')[-1].split('.')[0] + ".ppm"
            cv2.imwrite(new_path, ppm_image)
            print( "ppm done: " + path)
    # play the files as a video
    else : 
        for path in path_all_pgm : 
            pgm_image = cv2.imread(path)
            ppm_image = yuv_to_bgr(pgm_image)
            ppm_image = ppm_image.astype(np.uint8)
            cv2.imshow('display', ppm_image)
        cv2.destroyAllWindows()
main()