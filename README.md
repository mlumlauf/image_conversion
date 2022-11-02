# image_conversion

Project Title: Image Converter

Language: C

Motivation and Notes:
  - This project was completed for CMSC 15200 at the University of Chicago in Winter 2021.
  - See PZ-file-format.txt for details on PZ file formatting (there is no way to display this file as an image).

Description:
  This project converts an image in P3, P6, or a made-up format PZ to any one of the other formats. It converts
  images by parsing metadata and extracting individual bytes which dictate the individual pixels of the image.
  
Structure:
  The project is organized into a general header file, a utility function file, image conversion files with 
  conversion-specific and main functions, and a MakeFile. 
