#include "hist_func.h"
using namespace std;
#include <vector>

void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF);
void hist_matching(Mat& input, Mat& matched, G* trans_func_z, G* trans_func_ref);

int main() {
    Mat input     = imread("input.jpg", IMREAD_COLOR);
    Mat reference = imread("lena.jpg",  IMREAD_COLOR);
    if (input.empty() || reference.empty()) {
        fprintf(stderr, "Cannot open image files\n"); return -1;
    }

    // Convert both images to YUV
    Mat yuv_input, yuv_ref;
    cvtColor(input,     yuv_input, COLOR_RGB2YUV);
    cvtColor(reference, yuv_ref,   COLOR_RGB2YUV);

    Mat channels_input[3], channels_ref[3];
    split(yuv_input, channels_input);
    split(yuv_ref,   channels_ref);

    // Save original Y before equalization (needed for CDF)
    Mat Y_input = channels_input[0].clone();
    Mat Y_ref   = channels_ref[0].clone();

    float* CDF_input = cal_CDF(Y_input);
    float* CDF_ref   = cal_CDF(Y_ref);

    G trans_func_input[L] = { 0 };
    G trans_func_ref  [L] = { 0 };

    // Equalize Y channels
    hist_eq(Y_input, channels_input[0], trans_func_input, CDF_input);
    hist_eq(Y_ref,   channels_ref[0],   trans_func_ref,   CDF_ref);

    // Matching: map equalized input Y → matched Y using inverse of ref transfer func
    Mat yuv_matched = yuv_input.clone();
    Mat channels_matched[3];
    split(yuv_matched, channels_matched);

    G trans_func_z[L] = { 0 };
    hist_matching(channels_input[0], channels_matched[0], trans_func_z, trans_func_ref);

    merge(channels_matched, 3, yuv_matched);
    cvtColor(yuv_matched, yuv_matched, COLOR_YUV2RGB);  // result in RGB

    free(CDF_input);
    free(CDF_ref);

    ///////////////////////// files //////////////////////////
    FILE *f_trans_func_matching_color, *f_hist, *f_matched_hist;
#ifdef _WIN32
    fopen_s(&f_trans_func_matching_color, "matching_function_color.txt",      "w+");
    fopen_s(&f_hist,                      "matching_original_hist_color.txt",  "w+");
    fopen_s(&f_matched_hist,              "matched_hist_color.txt",            "w+");
#else
    f_trans_func_matching_color = fopen("matching_function_color.txt",     "w+");
    f_hist                      = fopen("matching_original_hist_color.txt","w+");
    f_matched_hist              = fopen("matched_hist_color.txt",          "w+");
#endif

    for (int i = 0; i < L; i++)
        fprintf(f_trans_func_matching_color, "%d\t%d\n", i, (int)trans_func_z[i]);

    int histSize = L;
    float ran[] = { 0, 256 };
    const float* range = { ran };

    // Histograms of original input (RGB channels)
    vector<Mat> bgr_input, bgr_matched;
    split(input,       bgr_input);
    split(yuv_matched, bgr_matched);

    Mat histB, histG, histR, histB_m, histG_m, histR_m;
    calcHist(&bgr_input[0], 1, 0, Mat(), histB,   1, &histSize, &range);
    calcHist(&bgr_input[1], 1, 0, Mat(), histG,   1, &histSize, &range);
    calcHist(&bgr_input[2], 1, 0, Mat(), histR,   1, &histSize, &range);
    calcHist(&bgr_matched[0], 1, 0, Mat(), histB_m, 1, &histSize, &range);
    calcHist(&bgr_matched[1], 1, 0, Mat(), histG_m, 1, &histSize, &range);
    calcHist(&bgr_matched[2], 1, 0, Mat(), histR_m, 1, &histSize, &range);

    for (int i = 0; i < histSize; i++) {
      
        fprintf(f_hist,         "%d\t\t%f\t\t%f\t\t%f\n", i,
                histR.at<float>(i), histG.at<float>(i), histB.at<float>(i));
        fprintf(f_matched_hist, "%d\t\t%f\t\t%f\t\t%f\n", i,
                histR_m.at<float>(i), histG_m.at<float>(i), histB_m.at<float>(i));
    }

    fclose(f_trans_func_matching_color);
    fclose(f_hist);
    fclose(f_matched_hist);

    float **pdf_orig    = cal_PDF_RGB(input);
    float **pdf_matched = cal_PDF_RGB(yuv_matched);
    float pdfR[L], pdfG[L], pdfB[L], pdfR_m[L], pdfG_m[L], pdfB_m[L];
    for (int i = 0; i < L; i++) {
        pdfR[i] = pdf_orig[i][2];    pdfG[i] = pdf_orig[i][1];    pdfB[i] = pdf_orig[i][0];
        pdfR_m[i] = pdf_matched[i][2]; pdfG_m[i] = pdf_matched[i][1]; pdfB_m[i] = pdf_matched[i][0];
    }
    plot(pdfR,   "PDF", "Original PDF - R");
    plot(pdfG,   "PDF", "Original PDF - G");
    plot(pdfB,   "PDF", "Original PDF - B");
    plot(pdfR_m, "PDF", "Matched PDF - R");
    plot(pdfG_m, "PDF", "Matched PDF - G");
    plot(pdfB_m, "PDF", "Matched PDF - B");
    for (int i = 0; i < L; i++) free(pdf_orig[i]);    free(pdf_orig);
    for (int i = 0; i < L; i++) free(pdf_matched[i]); free(pdf_matched);

    namedWindow("Input",     WINDOW_AUTOSIZE); imshow("Input",     input);
    namedWindow("Reference", WINDOW_AUTOSIZE); imshow("Reference", reference);
    namedWindow("Matched",   WINDOW_AUTOSIZE); imshow("Matched",   yuv_matched);

    waitKey(0);
    return 0;
}

void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF) {
    for (int i = 0; i < L; i++)
        trans_func[i] = (G)((L - 1) * CDF[i]);

    for (int i = 0; i < input.rows; i++)
        for (int j = 0; j < input.cols; j++)
            equalized.at<G>(i, j) = trans_func[input.at<G>(i, j)];
}

void hist_matching(Mat& input, Mat& matched, G* trans_func_z, G* trans_func_ref) {

    for (int s = 0; s < L; s++)
        trans_func_z[s] = 0;

    int idx1 = L - 2;
    int idx2 = L - 1;
    while (idx1 >= 0) {
        int lo = (int)trans_func_ref[idx1];
        int hi = (int)trans_func_ref[idx2];
        if (lo > hi) { int tmp = lo; lo = hi; hi = tmp; }
        for (int i = lo; i <= hi; i++)
            trans_func_z[i] = (G)idx2;
        idx2 = idx1--;
    }

    for (int i = 0; i < input.rows; i++)
        for (int j = 0; j < input.cols; j++)
            matched.at<G>(i, j) = trans_func_z[input.at<G>(i, j)];
}