#include "hist_func.h"
using namespace std;

void hist_eq_Color(Mat &input, Mat &equalized, G(*trans_func)[3], float **CDF);

int main() {

    Mat input = imread("input.jpg", IMREAD_COLOR);
    if (input.empty()) { fprintf(stderr, "Cannot open input.jpg\n"); return -1; }

    Mat equalized_RGB = input.clone();

    FILE *f_PDF_RGB, *f_equalized_PDF_RGB, *f_trans_func_eq_RGB;
#ifdef _WIN32
    fopen_s(&f_PDF_RGB, "PDF_RGB.txt", "w+");
    fopen_s(&f_equalized_PDF_RGB, "equalized_PDF_RGB.txt", "w+");
    fopen_s(&f_trans_func_eq_RGB, "trans_func_eq_RGB.txt", "w+");
#else
    f_PDF_RGB = fopen("PDF_RGB.txt", "w+");
    f_equalized_PDF_RGB = fopen("equalized_PDF_RGB.txt", "w+");
    f_trans_func_eq_RGB = fopen("trans_func_eq_RGB.txt", "w+");
#endif

    float **PDF_RGB = cal_PDF_RGB(input);
    float **CDF_RGB = cal_CDF_RGB(input);

    G trans_func_eq_RGB[L][3] = {};

    hist_eq_Color(input, equalized_RGB, trans_func_eq_RGB, CDF_RGB);

    float **equalized_PDF_RGB = cal_PDF_RGB(equalized_RGB);

    for (int i = 0; i < L; i++) {
        // Order: B G R  (OpenCV channel order)
        fprintf(f_PDF_RGB, "%d\t%f\t%f\t%f\n", i,
                PDF_RGB[i][2], PDF_RGB[i][1], PDF_RGB[i][0]);  // R G B
        fprintf(f_equalized_PDF_RGB, "%d\t%f\t%f\t%f\n", i,
                equalized_PDF_RGB[i][2], equalized_PDF_RGB[i][1], equalized_PDF_RGB[i][0]);
        fprintf(f_trans_func_eq_RGB, "%d\t%d\t%d\t%d\n", i,
                (int)trans_func_eq_RGB[i][2],   // R
                (int)trans_func_eq_RGB[i][1],   // G
                (int)trans_func_eq_RGB[i][0]);  // B
    }

    // plot: 채널별 PDF를 1D 배열로 뽑아서 plot()에 전달
    // B=0, G=1, R=2
    float pdfR[L], pdfG[L], pdfB[L];
    float pdfR_eq[L], pdfG_eq[L], pdfB_eq[L];
    for (int i = 0; i < L; i++) {
        pdfR[i] = PDF_RGB[i][2]; pdfG[i] = PDF_RGB[i][1]; pdfB[i] = PDF_RGB[i][0];
        pdfR_eq[i] = equalized_PDF_RGB[i][2];
        pdfG_eq[i] = equalized_PDF_RGB[i][1];
        pdfB_eq[i] = equalized_PDF_RGB[i][0];
    }
    plot(pdfR, "PDF", "Original PDF - R");
    plot(pdfG, "PDF", "Original PDF - G");
    plot(pdfB, "PDF", "Original PDF - B");
    plot(pdfR_eq, "PDF", "Equalized PDF - R");
    plot(pdfG_eq, "PDF", "Equalized PDF - G");
    plot(pdfB_eq, "PDF", "Equalized PDF - B");

    for (int i = 0; i < L; i++) free(PDF_RGB[i]); free(PDF_RGB);
    for (int i = 0; i < L; i++) free(CDF_RGB[i]); free(CDF_RGB);
    for (int i = 0; i < L; i++) free(equalized_PDF_RGB[i]); free(equalized_PDF_RGB);

    fclose(f_PDF_RGB);
    fclose(f_equalized_PDF_RGB);
    fclose(f_trans_func_eq_RGB);

    namedWindow("RGB", WINDOW_AUTOSIZE);
    imshow("RGB", input);
    namedWindow("Equalized_RGB", WINDOW_AUTOSIZE);
    imshow("Equalized_RGB", equalized_RGB);

    waitKey(0);
    return 0;
}

void hist_eq_Color(Mat &input, Mat &equalized, G(*trans_func)[3], float **CDF) {
    // compute per-channel transfer function: T(r) = (L-1) * CDF(r)
    for (int r = 0; r < L; r++)
        for (int c = 0; c < 3; c++)
            trans_func[r][c] = (G)((L - 1) * CDF[r][c]);

    // apply transfer function pixel by pixel
    for (int i = 0; i < input.rows; i++)
        for (int j = 0; j < input.cols; j++)
            for (int c = 0; c < 3; c++)
                equalized.at<Vec3b>(i, j)[c] =
                    trans_func[input.at<Vec3b>(i, j)[c]][c];
}