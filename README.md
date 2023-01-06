# video-decompression-and-display

Made by : 
Antoine Aubin - Melvin Gidel - Paul Galand

## To use the program 
```sh
./mpeg2dec.sh #Will compile mpeg2dec lib

make

./main --video [video_path] --framerate [ips] --ppm --flags [flag_id]
```

## Help
- --flags: 0=FLAG_TOP_FIELD_FIRST - 1=FLAG_REPEAT_FIRST_FIELD - 2=FLAG_PROGRESSIVE_FRAME
- --video (-v): path of the video to use
- --framerate (-f): force the image per seconds of the display
- --ppm (-p): render all ppm of the video in the folder ppm/
