===================================================
  Build Instructions & Program Description
===================================================

----------------------------------------------------
1. GaussianGray_Sep.cpp
----------------------------------------------------

PURPOSE
  Applies Gaussian filtering to a grayscale image using
  separable 1D Gaussian kernels. This reduces computation
  while producing the same smoothing effect as 2D Gaussian filtering.

COMPILE
  g++ -std=c++17 GaussianGray_Sep.cpp \
    $(pkg-config --cflags --libs opencv4) -o GaussianGray_Sep

RUN
  ./GaussianGray_Sep

HOW TO ADJUST PARAMETERS
  Open GaussianGray_Sep.cpp and find the Gaussian filter call.

  Example:
      output = gaussianfilter(input_gray, 1, 1, 1, "adjustkernel");

  - input image
      Change "lena.jpg" to another image file if needed.

  - n
      Kernel radius.
      Actual kernel size = (2*n + 1).
      Example: n=1 -> 3x3, n=2 -> 5x5

  - sigmaT, sigmaS
      Standard deviations of the Gaussian kernel.
      Larger values produce stronger smoothing.

  - opt
      Boundary processing option.
      Available options:
      "zero-paddle" : outside pixels are treated as 0
      "mirroring"   : boundary is handled by reflection
      "adjustkernel": kernel is re-normalized near borders

NOTE
  This program first converts the input image to grayscale
  and then applies separable Gaussian filtering.

----------------------------------------------------
2. GaussianGray.cpp
----------------------------------------------------

PURPOSE
  Applies Gaussian filtering to a grayscale image using
  a standard 2D Gaussian kernel in order to smooth the image
  and reduce noise.

COMPILE
  g++ -std=c++17 GaussianGray.cpp \
    $(pkg-config --cflags --libs opencv4) -o GaussianGray

RUN
  ./GaussianGray

HOW TO ADJUST PARAMETERS
  Open GaussianGray.cpp and find the Gaussian filter call.

  Example:
      output = gaussianfilter(input_gray, 1, 1, 1, "adjustkernel");

  - input image
      Change "lena.jpg" to another image file if needed.

  - n
      Kernel radius.
      Actual kernel size = (2*n + 1).

  - sigmaT, sigmaS
      Gaussian standard deviations.
      Larger values produce more blur.

  - opt
      Boundary processing option:
      "zero-paddle", "mirroring", "adjustkernel"

NOTE
  This version uses a full 2D Gaussian kernel instead of
  separable filtering.

----------------------------------------------------
3. GaussianRGB_Sep.cpp
----------------------------------------------------

PURPOSE
  Applies Gaussian filtering to a color image using
  separable 1D Gaussian kernels for each RGB channel.

COMPILE
  g++ -std=c++17 GaussianRGB_Sep.cpp \
    $(pkg-config --cflags --libs opencv4) -o GaussianRGB_Sep

RUN
  ./GaussianRGB_Sep

HOW TO ADJUST PARAMETERS
  Open GaussianRGB_Sep.cpp and find:

      output = gaussianfilter(input, 1, 1, 1, "zero-paddle");

  or the commented alternatives:
      "mirroring"
      "adjustkernel"

  - input image
      Change "lena.jpg" to another image file if needed.

  - n
      Kernel radius.
      Actual kernel size = (2*n + 1).

  - sigmaT, sigmaS
      Gaussian standard deviations.
      Larger values produce stronger smoothing.

  - opt
      Boundary processing option:
      "zero-paddle", "mirroring", "adjustkernel"

NOTE
  This program processes RGB channels separately and applies
  Gaussian filtering in separable form.

----------------------------------------------------
4. GaussianRGB.cpp
----------------------------------------------------

PURPOSE
  Applies Gaussian filtering to a color image using
  a standard 2D Gaussian kernel for RGB channels.

COMPILE
  g++ -std=c++17 GaussianRGB.cpp \
    $(pkg-config --cflags --libs opencv4) -o GaussianRGB

RUN
  ./GaussianRGB

HOW TO ADJUST PARAMETERS
  Open GaussianRGB.cpp and find the Gaussian filter call.

  Example:
      output = gaussianfilter(input, 1, 1, 1, "adjustkernel");

  - input image
      Change "lena.jpg" to another image file if needed.

  - n
      Kernel radius.
      Actual kernel size = (2*n + 1).

  - sigmaT, sigmaS
      Gaussian standard deviations.
      Larger values create more blur.

  - opt
      Boundary processing option:
      "zero-paddle", "mirroring", "adjustkernel"

NOTE
  This version uses direct 2D Gaussian convolution on a color image.

----------------------------------------------------
5. LaplacianGray.cpp
----------------------------------------------------

PURPOSE
  Applies Laplacian filtering to a grayscale image in order
  to emphasize edges and rapid intensity changes.

COMPILE
  g++ -std=c++17 LaplacianGray.cpp \
    $(pkg-config --cflags --libs opencv4) -o LaplacianGray

RUN
  ./LaplacianGray

HOW TO ADJUST PARAMETERS
  Open LaplacianGray.cpp and find the filter call.

  - input image
      Change "lena.jpg" to another image file if needed.

  - filter kernel
      If the Laplacian mask is explicitly written in the code,
      changing the mask values changes the edge response.

  - opt
      If boundary processing is implemented in the same style,
      available options are:
      "zero-paddle", "mirroring", "adjustkernel"

NOTE
  Laplacian is a second-derivative filter, so it is sensitive to noise.

----------------------------------------------------
6. LaplacianRGB.cpp
----------------------------------------------------

PURPOSE
  Applies Laplacian filtering to a color image in order
  to emphasize edges in RGB channels.

COMPILE
  g++ -std=c++17 LaplacianRGB.cpp \
    $(pkg-config --cflags --libs opencv4) -o LaplacianRGB

RUN
  ./LaplacianRGB

HOW TO ADJUST PARAMETERS
  Open LaplacianRGB.cpp and find the filter call.

  - input image
      Change "lena.jpg" to another image file if needed.

  - filter kernel
      If the Laplacian mask is explicitly defined in the code,
      modifying it changes the result.

  - opt
      If implemented, boundary processing options are:
      "zero-paddle", "mirroring", "adjustkernel"

NOTE
  The result may look stronger or noisier than Gaussian or mean filtering.

----------------------------------------------------
7. MeanFilterGray.cpp
----------------------------------------------------

PURPOSE
  Applies mean filtering to a grayscale image. Each pixel
  is replaced by the average of neighboring pixels.

COMPILE
  g++ -std=c++17 MeanFilterGray.cpp \
    $(pkg-config --cflags --libs opencv4) -o MeanFilterGray

RUN
  ./MeanFilterGray

HOW TO ADJUST PARAMETERS
  Open MeanFilterGray.cpp and find the mean filter call.

  - input image
      Change "lena.jpg" to another image file if needed.

  - n
      Kernel radius.
      Actual kernel size = (2*n + 1).
      Larger values produce stronger smoothing.

  - opt
      Boundary processing option, if implemented:
      "zero-paddle", "mirroring", "adjustkernel"

NOTE
  Mean filtering is simple and effective for smoothing,
  but it can blur edges significantly.

----------------------------------------------------
8. MeanFilterRGB.cpp
----------------------------------------------------

PURPOSE
  Applies mean filtering to a color image by averaging
  neighboring pixels in each RGB channel.

COMPILE
  g++ -std=c++17 MeanFilterRGB.cpp \
    $(pkg-config --cflags --libs opencv4) -o MeanFilterRGB

RUN
  ./MeanFilterRGB

HOW TO ADJUST PARAMETERS
  Open MeanFilterRGB.cpp and find the mean filter call.

  - input image
      Change "lena.jpg" to another image file if needed.

  - n
      Kernel radius.
      Actual kernel size = (2*n + 1).

  - opt
      Boundary processing option, if implemented:
      "zero-paddle", "mirroring", "adjustkernel"

NOTE
  This filter smooths RGB images but may reduce edge sharpness.

----------------------------------------------------
9. SobelGray.cpp
----------------------------------------------------

PURPOSE
  Applies the Sobel operator to a grayscale image in order
  to detect image gradients and highlight edges.

COMPILE
  g++ -std=c++17 SobelGray.cpp \
    $(pkg-config --cflags --libs opencv4) -o SobelGray

RUN
  ./SobelGray

HOW TO ADJUST PARAMETERS
  Open SobelGray.cpp and find the Sobel filter call.

  - input image
      Change "lena.jpg" to another image file if needed.

  - direction
      Depending on the code, Sobel may be applied in the x direction,
      y direction, or both.

  - opt
      If boundary processing is implemented, available options may be:
      "zero-paddle", "mirroring", "adjustkernel"

NOTE
  Sobel filtering is based on first-order derivatives
  and is commonly used for edge detection.

----------------------------------------------------
10. SobelRGB.cpp
----------------------------------------------------

PURPOSE
  Applies the Sobel operator to a color image in order
  to detect edges in RGB channels.

COMPILE
  g++ -std=c++17 SobelRGB.cpp \
    $(pkg-config --cflags --libs opencv4) -o SobelRGB

RUN
  ./SobelRGB

HOW TO ADJUST PARAMETERS
  Open SobelRGB.cpp and find the Sobel filter call.

  - input image
      Change "lena.jpg" to another image file if needed.

  - direction
      Depending on the code, Sobel may be applied in the x direction,
      y direction, or both.

  - opt
      If implemented, boundary processing options may include:
      "zero-paddle", "mirroring", "adjustkernel"

NOTE
  Edge responses may differ between RGB channels.

----------------------------------------------------
11. UnsharpGray.cpp
----------------------------------------------------

PURPOSE
  Applies unsharp masking to a grayscale image in order
  to enhance edges and improve perceived sharpness.

COMPILE
  g++ -std=c++17 UnsharpGray.cpp \
    $(pkg-config --cflags --libs opencv4) -o UnsharpGray

RUN
  ./UnsharpGray

HOW TO ADJUST PARAMETERS
  Open UnsharpGray.cpp and find:

      float k = 0.7f;
      output = unsharpmask(input_gray, 3, 3, 3, k, "adjustkernel");

  or the commented alternatives:
      "mirroring"
      "zero-paddle"

  - input image
      Change "lena.jpg" to another image file if needed.

  - n
      Kernel radius for Gaussian blur.
      Actual kernel size = (2*n + 1).

  - sigmaT, sigmaS
      Gaussian standard deviations used in the low-pass filtering step.

  - k
      Sharpening control parameter.
      Larger values produce stronger sharpening.
      Default: 0.7

  - opt
      Boundary processing option:
      "zero-paddle", "mirroring", "adjustkernel"

NOTE
  The program first blurs the image using Gaussian filtering,
  then computes the sharpened result using unsharp masking.
  Excessively large k may amplify noise.

----------------------------------------------------
12. UnsharpRGB.cpp
----------------------------------------------------

PURPOSE
  Applies unsharp masking to a color image in order
  to enhance edges and improve image sharpness.

COMPILE
  g++ -std=c++17 UnsharpRGB.cpp \
    $(pkg-config --cflags --libs opencv4) -o UnsharpRGB

RUN
  ./UnsharpRGB

HOW TO ADJUST PARAMETERS
  Open UnsharpRGB.cpp and find the unsharp masking call.

  Example:
      float k = 0.7f;
      output = unsharpmask(input, 3, 3, 3, k, "adjustkernel");

  - input image
      Change "lena.jpg" to another image file if needed.

  - n
      Kernel radius for Gaussian blur.
      Actual kernel size = (2*n + 1).

  - sigmaT, sigmaS
      Gaussian standard deviations used before sharpening.

  - k
      Sharpening control parameter.
      Larger values produce stronger sharpening.

  - opt
      Boundary processing option:
      "zero-paddle", "mirroring", "adjustkernel"

NOTE
  This program sharpens a color image using Gaussian-based
  unsharp masking. Too much sharpening may create unnatural edges.

===================================================