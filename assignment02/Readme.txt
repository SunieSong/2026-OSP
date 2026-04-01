===================================================
  Build Instructions & Program Description
===================================================

----------------------------------------------------
1. PDF_CDF.cpp
----------------------------------------------------

PURPOSE
  Computes the PDF (Probability Density Function) and
  CDF (Cumulative Distribution Function) of a grayscale
  image and visualizes the distributions.

COMPILE
  g++ -std=c++17 PDF_CDF.cpp \
    $(pkg-config --cflags --libs opencv4) -o PDF_CDF

RUN
  ./PDF_CDF

HOW TO ADJUST PARAMETERS
  Open PDF_CDF.cpp and find:

      Mat input = imread("input.jpg", IMREAD_COLOR);

  - input image
      Change "input.jpg" to another image file if needed.

----------------------------------------------------
2. hist_stretching.cpp
----------------------------------------------------

PURPOSE
  Enhances image contrast using linear histogram stretching
  based on a piecewise linear transformation function.

COMPILE
  g++ -std=c++17 hist_stretching.cpp \
    $(pkg-config --cflags --libs opencv4) -o hist_stretching

RUN
  ./hist_stretching

HOW TO ADJUST PARAMETERS
  Open hist_stretching.cpp and find:

      linear_stretching(input_gray, stretched,
                        trans_func_stretch, 50, 110, 10, 110);

  - x1, x2
      Input intensity range.
      Default: 50, 110

  - y1, y2
      Output intensity range.
      Default: 10, 110

  Example:
      (30, 150, 0, 255) → stronger contrast enhancement

----------------------------------------------------
3. hist_eq.cpp (Grayscale)
----------------------------------------------------

PURPOSE
  Performs histogram equalization on a grayscale image
  using the cumulative distribution function (CDF) to
  improve global contrast.

COMPILE
  g++ -std=c++17 hist_eq.cpp \
    $(pkg-config --cflags --libs opencv4) -o hist_eq

RUN
  ./hist_eq

HOW TO ADJUST PARAMETERS
  Open hist_eq.cpp:

      hist_eq(input_gray, equalized, trans_func_eq, CDF);

  - No adjustable parameters.
  - Uses full intensity range (0 ~ 255).

----------------------------------------------------
4. hist_eq_RGB.cpp
----------------------------------------------------

PURPOSE
  Applies histogram equalization independently to each
  RGB channel. Enhances contrast but may distort colors.

COMPILE
  g++ -std=c++17 hist_eq_RGB.cpp \
    $(pkg-config --cflags --libs opencv4) -o hist_eq_RGB

RUN
  ./hist_eq_RGB

HOW TO ADJUST PARAMETERS
  Open hist_eq_RGB.cpp:

      split(input, channels);

  - Each channel is processed independently.
  - No external parameters.

NOTE
  Color distortion may occur.

----------------------------------------------------
5. hist_eq_YUV.cpp
----------------------------------------------------

PURPOSE
  Performs histogram equalization only on the luminance (Y)
  channel in YUV color space, preserving color information
  while improving contrast.

COMPILE
  g++ -std=c++17 hist_eq_YUV.cpp \
    $(pkg-config --cflags --libs opencv4) -o hist_eq_YUV

RUN
  ./hist_eq_YUV

HOW TO ADJUST PARAMETERS
  Open hist_eq_YUV.cpp:

      cvtColor(input, equalized_YUV, COLOR_RGB2YUV);

  - Equalization is applied only on Y channel.
  - No adjustable parameters.

NOTE
  Produces more natural color compared to RGB method.

----------------------------------------------------
6. hist_matching_gray.cpp
----------------------------------------------------

PURPOSE
  Matches the histogram of a grayscale input image to that
  of a reference image by transforming intensity values.

COMPILE
  g++ -std=c++17 hist_matching_gray.cpp \
    $(pkg-config --cflags --libs opencv4) -o hist_matching_gray

RUN
  ./hist_matching_gray

HOW TO ADJUST PARAMETERS
  Open hist_matching_gray.cpp:

      Mat input     = imread("input.jpg", IMREAD_COLOR);
      Mat reference = imread("lena.jpg", IMREAD_COLOR);

  - input image
      Image to be transformed.

  - reference image
      Target histogram.

  Change file names to use different images.

----------------------------------------------------
7. hist_matching_color.cpp
----------------------------------------------------

PURPOSE
  Performs histogram matching on color images using the
  Y channel in YUV color space. Preserves color while
  adapting brightness distribution.

COMPILE
  g++ -std=c++17 hist_matching_color.cpp \
    $(pkg-config --cflags --libs opencv4) -o hist_matching_color

RUN
  ./hist_matching_color

HOW TO ADJUST PARAMETERS
  Open hist_matching_color.cpp:

      cvtColor(input, input_YUV, COLOR_RGB2YUV);

  - Matching is applied only on Y channel.
  - Reference image can be changed.

NOTE
  Produces more natural results than RGB matching.

===================================================