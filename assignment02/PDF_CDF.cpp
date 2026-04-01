#include "hist_func.h"
using namespace std;

int main() {

    Mat input = imread("input.jpg", IMREAD_COLOR);
    if (input.empty()) { fprintf(stderr, "Cannot open input.jpg\n"); return -1; }

    Mat input_gray;
    cvtColor(input, input_gray, COLOR_RGB2GRAY);

    FILE *f_PDF, *f_CDF;
#ifdef _WIN32
    fopen_s(&f_PDF, "PDF.txt", "w+");
    fopen_s(&f_CDF, "CDF.txt", "w+");
#else
    f_PDF = fopen("PDF.txt", "w+");
    f_CDF = fopen("CDF.txt", "w+");
#endif

    float *PDF = cal_PDF(input_gray);
    float *CDF = cal_CDF(input_gray);

    for (int i = 0; i < L; i++) {
        fprintf(f_PDF, "%d\t%f\n", i, PDF[i]);
        fprintf(f_CDF, "%d\t%f\n", i, CDF[i]);
    }

    // plot PDF and CDF
    float *PDF_plot = cal_PDF(input_gray);
    float *CDF_plot = cal_CDF(input_gray);
    plot(PDF_plot, "PDF", "PDF");
    plot(CDF_plot, "CDF", "CDF");
    free(PDF_plot);
    free(CDF_plot);

    free(PDF);
    free(CDF);
    fclose(f_PDF);
    fclose(f_CDF);

    namedWindow("RGB", WINDOW_AUTOSIZE);
    imshow("RGB", input);
    namedWindow("Grayscale", WINDOW_AUTOSIZE);
    imshow("Grayscale", input_gray);

    waitKey(0);
    return 0;
}