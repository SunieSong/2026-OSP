#include "hist_func.h"
using namespace std;

void hist_eq(Mat &input, Mat &equalized, G *trans_func, float *CDF);

int main() {

    Mat input = imread("input.jpg", IMREAD_COLOR);
    if (input.empty()) { fprintf(stderr, "Cannot open input.jpg\n"); return -1; }

    Mat equalized_YUV;
    cvtColor(input, equalized_YUV, COLOR_RGB2YUV);

    Mat channels[3];
    split(equalized_YUV, channels);
    Mat Y = channels[0];   

    FILE *f_PDF_RGB, *f_equalized_PDF_YUV, *f_trans_func_eq_YUV;
#ifdef _WIN32
    fopen_s(&f_PDF_RGB, "PDF_RGB.txt", "w+");
    fopen_s(&f_equalized_PDF_YUV, "equalized_PDF_YUV.txt", "w+");
    fopen_s(&f_trans_func_eq_YUV, "trans_func_eq_YUV.txt", "w+");
#else
    f_PDF_RGB = fopen("PDF_RGB.txt", "w+");
    f_equalized_PDF_YUV = fopen("equalized_PDF_YUV.txt", "w+");
    f_trans_func_eq_YUV = fopen("trans_func_eq_YUV.txt", "w+");
#endif

    float **PDF_RGB = cal_PDF_RGB(input);
    float *CDF_YUV  = cal_CDF(Y);

    G trans_func_eq_YUV[L] = { 0 };

    hist_eq(Y, channels[0], trans_func_eq_YUV, CDF_YUV);  
    merge(channels, 3, equalized_YUV);
    cvtColor(equalized_YUV, equalized_YUV, COLOR_YUV2RGB);

    float **equalized_PDF_YUV = cal_PDF_RGB(equalized_YUV);

    for (int i = 0; i < L; i++) {
        fprintf(f_PDF_RGB, "%d\t%f\t%f\t%f\n", i,
                PDF_RGB[i][2], PDF_RGB[i][1], PDF_RGB[i][0]);  
        fprintf(f_equalized_PDF_YUV, "%d\t%f\t%f\t%f\n", i,
                equalized_PDF_YUV[i][2], equalized_PDF_YUV[i][1], equalized_PDF_YUV[i][0]);
        fprintf(f_trans_func_eq_YUV, "%d\t%d\n", i, (int)trans_func_eq_YUV[i]);
    }

    // plot: 원본 RGB 채널별 PDF + equalized 후 RGB 채널별 PDF
    float pdfR[L], pdfG[L], pdfB[L];
    float pdfR_eq[L], pdfG_eq[L], pdfB_eq[L];
    for (int i = 0; i < L; i++) {
        pdfR[i] = PDF_RGB[i][2]; pdfG[i] = PDF_RGB[i][1]; pdfB[i] = PDF_RGB[i][0];
        pdfR_eq[i] = equalized_PDF_YUV[i][2];
        pdfG_eq[i] = equalized_PDF_YUV[i][1];
        pdfB_eq[i] = equalized_PDF_YUV[i][0];
    }
    plot(pdfR, "PDF", "Original PDF - R");
    plot(pdfG, "PDF", "Original PDF - G");
    plot(pdfB, "PDF", "Original PDF - B");
    plot(pdfR_eq, "PDF", "Equalized(YUV) PDF - R");
    plot(pdfG_eq, "PDF", "Equalized(YUV) PDF - G");
    plot(pdfB_eq, "PDF", "Equalized(YUV) PDF - B");

    for (int i = 0; i < L; i++) free(PDF_RGB[i]); free(PDF_RGB);
    free(CDF_YUV);
    for (int i = 0; i < L; i++) free(equalized_PDF_YUV[i]); free(equalized_PDF_YUV);

    fclose(f_PDF_RGB);
    fclose(f_equalized_PDF_YUV);
    fclose(f_trans_func_eq_YUV);

    namedWindow("RGB", WINDOW_AUTOSIZE);
    imshow("RGB", input);
    namedWindow("Equalized_YUV", WINDOW_AUTOSIZE);
    imshow("Equalized_YUV", equalized_YUV);

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