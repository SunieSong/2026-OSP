#include "hist_func.h"
using namespace std;

void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF);
void hist_matching(Mat& input, Mat& matched, G* trans_func_z, G* trans_func_ref);

int main() {
    Mat input     = imread("input.jpg",  IMREAD_COLOR);
    Mat reference = imread("lena.jpg",   IMREAD_COLOR);
    if (input.empty() || reference.empty()) {
        fprintf(stderr, "Cannot open image files\n"); return -1;
    }

    Mat input_gray, reference_gray;
    cvtColor(input,     input_gray,     COLOR_RGB2GRAY);
    cvtColor(reference, reference_gray, COLOR_RGB2GRAY);

    Mat equalized_input = input_gray.clone();
    Mat equalized_ref   = reference_gray.clone();

    float* CDF_input = cal_CDF(input_gray);
    float* CDF_ref   = cal_CDF(reference_gray);

    G trans_func_input[L] = { 0 };
    G trans_func_ref  [L] = { 0 };

    hist_eq(input_gray,     equalized_input, trans_func_input, CDF_input);
    hist_eq(reference_gray, equalized_ref,   trans_func_ref,   CDF_ref);

    Mat matched = input_gray.clone();
    G trans_func_z[L] = { 0 };

    // hist_matching maps equalized input → matched using inverse of ref transfer func
    hist_matching(equalized_input, matched, trans_func_z, trans_func_ref);

    // BUG FIX: free CDF arrays (were commented out / omitted)
    free(CDF_input);
    free(CDF_ref);

    ///////////////////////// files //////////////////////////
    FILE *f_trans_func_matching_gray, *f_hist, *f_matched_hist;
#ifdef _WIN32
    fopen_s(&f_trans_func_matching_gray, "matching_function_gray.txt",      "w+");
    fopen_s(&f_hist,                     "matching_original_hist_gray.txt",  "w+");
    fopen_s(&f_matched_hist,             "matched_hist_gray.txt",            "w+");
#else
    f_trans_func_matching_gray = fopen("matching_function_gray.txt",     "w+");
    f_hist                     = fopen("matching_original_hist_gray.txt","w+");
    f_matched_hist             = fopen("matched_hist_gray.txt",          "w+");
#endif

    for (int i = 0; i < L; i++)
        fprintf(f_trans_func_matching_gray, "%d\t%d\n", i, (int)trans_func_z[i]);

    int histSize = L;
    float ran[] = { 0, 256 };
    const float* range = { ran };

    Mat hist, matched_hist;
    calcHist(&input_gray, 1, 0, Mat(), hist,         1, &histSize, &range);
    calcHist(&matched,    1, 0, Mat(), matched_hist,  1, &histSize, &range);

    for (int i = 0; i < histSize; i++) {
        fprintf(f_hist,         "%d\t%f\n", i, hist.at<float>(i));
        fprintf(f_matched_hist, "%d\t%f\n", i, matched_hist.at<float>(i));
    }

    fclose(f_trans_func_matching_gray);
    fclose(f_hist);
    fclose(f_matched_hist);

    // plot histograms
    float *pdf_input   = cal_PDF(input_gray);
    float *pdf_matched = cal_PDF(matched);
    plot(pdf_input,   "PDF", "Original PDF (gray)");
    plot(pdf_matched, "PDF", "Matched PDF (gray)");
    free(pdf_input);
    free(pdf_matched);

    namedWindow("Input",     WINDOW_AUTOSIZE); imshow("Input",     input_gray);
    namedWindow("Reference", WINDOW_AUTOSIZE); imshow("Reference", reference_gray);
    namedWindow("Matched",   WINDOW_AUTOSIZE); imshow("Matched",   matched);

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

    trans_func_z[0] = trans_func_ref[0] <= 0 ? 0 : trans_func_z[0];

    for (int i = 0; i < input.rows; i++)
        for (int j = 0; j < input.cols; j++)
            matched.at<G>(i, j) = trans_func_z[input.at<G>(i, j)];
}