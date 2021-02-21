# PGMImageOverlay
Overlays a PGM image on to another PGM image and creates a new image "Output.pgm"
This program supports reading images in both P2 and P5 format and outputs a image to P2 format with a max value of 255

Takes in two mandatory arguments that are :
	1. file path for main image 
	2. file path for image to overlay on to the main image 

This program also supports optional flag that is used to specify the Overlay Operation type.
The Overlay Operation flags support are the following:
	1. "-a" - averages the value of both image 
	2. "-l" - replaces pixel value only if overlaying image pixel value is less than main image.
	3. "-g" - replaces pixel value only if overlaying image pixel value is greater than main image.

In the absence of a flag the overlay image's pixel replaces main image's pixel value without any condition or additional operation.

BUILD: make

USEAGE: ./ImageOverlay TestImage_1_P2_640x480.pgm SmileyImage200x200pgm
USEAGE: ./ImageOverlay TestImage_1_P2_640x480.pgmSmileyImage200x200pgm -l
USEAGE: ./ImageOverlay TestImage_1_P2_640x480.pgmSmileyImage200x200pgm -g
USEAGE: ./ImageOverlay TestImage_1_P2_640x480.pgmSmileyImage200x200pgm -a