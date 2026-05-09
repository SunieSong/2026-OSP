===================================================
  Build Instructions & Program Description
===================================================

----------------------------------------------------
1. LoG.cpp
----------------------------------------------------

PURPOSE
  Performs edge detection on both grayscale and color
  images using the Laplacian of Gaussian (LoG) filter.
  First applies a Gaussian filter to remove noise,
  then applies a Laplacian filter to detect edges.
  Output is visualized by mapping values to [0, 1]
  using NORM_MINMAX normalization.

COMPILE
  g++ LoG.cpp -o LoG \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./LoG

HOW TO ADJUST PARAMETERS
  Open LoG.cpp and find:

      int window_radius = 2;
      double sigma_t = 2.0;
      double sigma_s = 2.0;

  - window_radius (N)
      Kernel radius. Kernel size = (2N+1) x (2N+1)
      Larger value = wider Gaussian smoothing region.
      Fewer fine edges detected, stronger noise suppression.
      Default: 2  →  5x5 kernel

  - sigma_t
      Standard deviation of Gaussian in x-direction.
      Larger value = stronger smoothing along x-axis.
      Vertical edges become less prominent.
      Default: 2.0

  - sigma_s
      Standard deviation of Gaussian in y-direction.
      Larger value = stronger smoothing along y-axis.
      Horizontal edges become less prominent.
      Default: 2.0

NOTE
  When sigma_t != sigma_s, an anisotropic Gaussian
  is applied, which emphasizes edges in one direction
  over the other.
  Input image file is "lena.jpg".
  Place the image in the same directory before running.
  Six windows are displayed:
    Grayscale / RGB /
    Gaussian blur / Gaussian blur RGB /
    Laplacian filter / Laplacian filter RGB

----------------------------------------------------
2. Canny.cpp
----------------------------------------------------

PURPOSE
  Performs edge detection on a grayscale image using
  OpenCV's Canny edge detector (cv::Canny).
  Internally applies Gaussian smoothing, Sobel filtering,
  non-maximum suppression, and double thresholding
  (hysteresis thresholding) in sequence.
  Output is a single-channel 8-bit binary edge map.

COMPILE
  g++ Canny.cpp -o Canny \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./Canny

HOW TO ADJUST PARAMETERS
  Open Canny.cpp and find:

      double threshold1 = 50.0;
      double threshold2 = 150.0;
      int    apertureSize = 3;
      bool   L2gradient = false;

  - threshold1 (T_L)
      Lower bound for hysteresis thresholding.
      Pixels below T_L are rejected as non-edges.
      Pixels between T_L and T_H are accepted only if
      connected to a strong edge pixel.
      Smaller value = more weak edges connected.
      Default: 50.0

  - threshold2 (T_H)
      Upper bound for hysteresis thresholding.
      Pixels above T_H are always accepted as edges.
      Larger value = fewer strong edges detected.
      Recommended ratio T_L:T_H = 1:2 ~ 1:3.
      Default: 150.0

  - apertureSize
      Kernel size of the internal Sobel filter.
      Must be 3, 5, or 7.
      Larger value = smoother gradient, less localization.
      Default: 3

  - L2gradient
      Gradient magnitude computation method.
      false = L1 norm: |Gx| + |Gy|  (faster)
      true  = L2 norm: sqrt(Gx^2 + Gy^2)  (more accurate)
      Default: false

NOTE
  Input image file is "lena.jpg".
  Place the image in the same directory before running.
  Two windows are displayed:
    Grayscale / Canny

----------------------------------------------------
3. Harris_corner.cpp
----------------------------------------------------

PURPOSE
  Performs corner detection using OpenCV's Harris
  corner detector (cv::cornerHarris).
  Optionally applies non-maximum suppression
  (custom implementation) and subpixel refinement
  (cv::cornerSubPix) to improve corner localization.
  Supports three input images:
    checkerboard.png / checkerboard2.jpg / lena.jpg

COMPILE
  g++ Harris_corner.cpp -o Harris \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./Harris

HOW TO ADJUST PARAMETERS
  Open Harris_corner.cpp and find:

      bool NonMaxSupp = false;
      bool Subpixel = true;

      int blockSize = 2;
      int ksize = 3;
      double k = 0.04;

  - NonMaxSupp
      Toggle non-maximum suppression.
      true  = keep only local maximum corner pixels
              within a 5x5 window (radius=2).
      false = keep all pixels above threshold.
      Default: false

  - Subpixel
      Toggle subpixel refinement using cornerSubPix.
      true  = refine corner positions to sub-pixel accuracy.
      false = use integer pixel positions only.
      Default: true

  - blockSize
      Window size for computing the second moment matrix M.
      Larger value = more neighborhood gradient info used,
      smoother but less precise corner localization.
      Default: 2

  - ksize
      Aperture size of the internal Sobel filter
      used for gradient computation.
      Must be 3, 5, or 7.
      Default: 3

  - k
      Harris detector free parameter in:
      R = det(M) - k * trace(M)^2
      Typical range: 0.04 ~ 0.06.
      Larger value = fewer corners detected.
      Default: 0.04

  To change input image, comment/uncomment in main():

      Mat input = imread("checkerboard.png", IMREAD_COLOR);
   // Mat input = imread("checkerboard2.jpg", IMREAD_COLOR);
   // Mat input = imread("lena.jpg", IMREAD_COLOR);

NOTE
  Displayed windows vary depending on flag settings:
    Always shown:
      Harris Response / Harris Corner
    NonMaxSupp=true:
      Harris Corner (Non-max)
    Subpixel=true:
      Harris Corner (subpixel)
  Corner counts are printed to the terminal
  after each processing stage.

===================================================