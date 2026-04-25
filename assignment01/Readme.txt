====================================================
  Assignment 01 - Image Rotation & Image Stitching
====================================================

----------------------------------------------------
1. rotate_skeleton_v2.cpp
----------------------------------------------------

PURPOSE
  Rotates an input image (lena.jpg) by a specified angle using
  inverse warping. Supports two interpolation methods:
  Nearest Neighbor and Bilinear Interpolation.

HOW TO RUN
  1. Place "lena.jpg" in the same directory as the source file.
  2. Compile:
       g++ -std=c++17 rotate_skeleton_v2.cpp -o rotate \
         $(pkg-config --cflags --libs opencv4)
  3. Run:
       ./rotate
  4. Two windows will appear:
       - "image"   : original image
       - "rotated" : rotated image

HOW TO ADJUST PARAMETERS
  Open rotate_skeleton_v2.cpp and find this line in main():

       rotated = myrotate<Vec3b>(input, 45, "bilinear");

  - angle (2nd argument)
      Rotation angle in degrees.
      Default: 45
      Example: change to 90 for 90-degree rotation.

  - opt (3rd argument)
      Interpolation method.
      "nearest"  : Nearest Neighbor Interpolation (faster, lower quality)
      "bilinear" : Bilinear Interpolation (slower, higher quality)
      Default: "bilinear"

----------------------------------------------------
2. stitching.cpp
----------------------------------------------------

PURPOSE
  Stitches two overlapping images (stitchingL.jpg, stitchingR.jpg)
  into a single panorama using Affine Transformation, Inverse
  Warping with Bilinear Interpolation, and Image Stitching.

HOW TO RUN
  1. Place "stitchingL.jpg" and "stitchingR.jpg" in the same
     directory as the source file.
     (Rename Img01.jpg → stitchingL.jpg, Img02.jpg → stitchingR.jpg)
  2. Compile:
       g++ -std=c++17 stitching.cpp -o stitch \
         $(pkg-config --cflags --libs opencv4)
  3. Run:
       ./stitch
  4. Three windows will appear:
       - "Left Image"  : I1 (stitchingL.jpg)
       - "Right Image" : I2 (stitchingR.jpg)
       - "result"      : merged panorama image
  5. The result is saved as "result.png".

HOW TO ADJUST PARAMETERS
  Open stitching.cpp and find this line in main():

       blend_stitching(I1, I2, I_f, bound_l, bound_u, 0.5);

  - alpha (last argument)
      Blending weight in the overlap region.
      Range : 0.0 ~ 1.0
      0.0 = use only I2 in overlap region
      1.0 = use only I1 in overlap region
      Default: 0.5 (equal blend of I1 and I2)

  Corresponding points (ptl_x, ptl_y, ptr_x, ptr_y):
      28 manually selected matching points between I1 and I2.
      Used to estimate the Affine Transformation matrix.
      Can be replaced with your own corresponding points.

====================================================