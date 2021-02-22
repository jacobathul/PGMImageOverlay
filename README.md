# PGMImageOverlay
Overlays a PGM image onto another PGM image and creates a new image "Output.pgm"
This program supports reading images in both P2 and P5 format and outputs a image to P2 format with a max value of 255

Takes in two mandatory arguments that are :
1. File path for main image 
2. File path for image to overlay on the main image

This program also supports optional flag that is used to specify the Overlay Operation type.

The Overlay Operation flags support are the following:
1. ```"-a" ``` - averages the value of both image 
2.  ```"-l" ```- replaces pixel value only if overlaying image pixel value is less than main image.
3. ```"-g" ``` - replaces pixel value only if overlaying image pixel value is greater than main image.

In the absence of a flag the overlay image's pixel replaces main image's pixel value without any condition or additional operation.

## Build
To build run the following command in the terminal
```console
make
```
## Clean

To clean run the following command in the terminal
```console
make clean
```
## Run

To start the program run the following in the terminal
```console
 ./ImageOverlay TestImage_1_P2_640x480.pgm SmileyImage200x200.pgm -l
 ```


####  Other Usage examples

```console
 ./ImageOverlay TestImage_1_P2_640x480.pgm SmileyImage200x200.pgm
 ```
 ```console
 ./ImageOverlay TestImage_1_P2_640x480.pgm SmileyImage200x200.pgm -a
 ```
  ```console
 ./ImageOverlay TestImage_1_P2_640x480.pgm SmileyImage200x200.pgm -g
 ```
