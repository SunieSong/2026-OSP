#include "hist_func.h"
using namespace std;

void hist_eq(Mat &input, Mat &equalized, G *trans_func, float *CDF);

int main() {

    Mat input = imread("input.jpg", IMREAD_COLOR);
    if (input.empty()) { fprintf(stderr, "Cannot open input.jpg\n"); return -1; }

    Mat input_gray;
    cvtColor(input, input_gray, COLOR_RGB2GRAY);

    Mat equalized = input_gray.clone();

    FILE *f_PDF, *f_equalized_PDF_gray, *f_trans_func_eq;
#ifdef _WIN32
    fopen_s(&f_PDF, "PDF.txt", "w+");
    fopen_s(&f_equalized_PDF_gray, "equalized_PDF_gray.txt", "w+");
    fopen_s(&f_trans_func_eq, "trans_func_eq.txt", "w+");
#else
    f_PDF = fopen("PDF.txt", "w+");
    f_equalized_PDF_gray = fopen("equalized_PDF_gray.txt", "w+");
    f_trans_func_eq = fopen("trans_func_eq.txt", "w+");
#endif

    float *PDF = cal_PDF(input_gray);
    float *CDF = cal_CDF(input_gray);
    G trans_func_eq[L] = { 0 };

    hist_eq(input_gray, equalized, trans_func_eq, CDF);
    float *equalized_PDF_gray = cal_PDF(equalized);

    for (int i = 0; i < L; i++) {
        fprintf(f_PDF, "%d\t%f\n", i, PDF[i]);
        fprintf(f_equalized_PDF_gray, "%d\t%f\n", i, equalized_PDF_gray[i]);
        fprintf(f_trans_func_eq, "%d\t%d\n", i, (int)trans_func_eq[i]);
    }

    // plot histograms
    float *PDF_plot         = cal_PDF(input_gray);
    float *equalized_PDF_plot = cal_PDF(equalized);
    plot(PDF_plot,           "PDF", "Original PDF (gray)");
    plot(equalized_PDF_plot, "PDF", "Equalized PDF (gray)");
    free(PDF_plot);
    free(equalized_PDF_plot);

    free(PDF);
    free(CDF);
    free(equalized_PDF_gray);
    fclose(f_PDF);
    fclose(f_equalized_PDF_gray);
    fclose(f_trans_func_eq);

    namedWindow("Grayscale", WINDOW_AUTOSIZE);
    imshow("Grayscale", input_gray);
    namedWindow("Equalized", WINDOW_AUTOSIZE);
    imshow("Equalized", equalized);

    waitKey(0);
    return 0;
}

void hist_eq(Mat &input, Mat &equalized, G *trans_func, float *CDF) {
    for (int i = 0; i < L; i++)
        trans_func[i] = (G)((L - 1) * CDF[i]);

    for (int i = 0; i < input.rows; i++)
        for (int j = 0; j < input.cols; j++)
            equalized.at<G>(i, j) = trans_func[input.at<G>(i, j)];
}