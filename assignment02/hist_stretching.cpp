#include "hist_func.h"

void linear_stretching(Mat &input, Mat &stretched, G *trans_func, G x1, G x2, G y1, G y2);

int main() {

    Mat input = imread("input.jpg", IMREAD_COLOR);
    if (input.empty()) { fprintf(stderr, "Cannot open input.jpg\n"); return -1; }

    Mat input_gray;
    cvtColor(input, input_gray, COLOR_RGB2GRAY);

    Mat stretched = input_gray.clone();

    FILE *f_PDF, *f_stretched_PDF, *f_trans_func_stretch;
#ifdef _WIN32
    fopen_s(&f_PDF, "PDF.txt", "w+");
    fopen_s(&f_stretched_PDF, "stretched_PDF.txt", "w+");
    fopen_s(&f_trans_func_stretch, "trans_func_stretch.txt", "w+");
#else
    f_PDF = fopen("PDF.txt", "w+");
    f_stretched_PDF = fopen("stretched_PDF.txt", "w+");
    f_trans_func_stretch = fopen("trans_func_stretch.txt", "w+");
#endif

    G trans_func_stretch[L] = { 0 };
    float *PDF = cal_PDF(input_gray);

    linear_stretching(input_gray, stretched, trans_func_stretch, 50, 110, 10, 110);
    float *stretched_PDF = cal_PDF(stretched);

    for (int i = 0; i < L; i++) {
        fprintf(f_PDF, "%d\t%f\n", i, PDF[i]);
        fprintf(f_stretched_PDF, "%d\t%f\n", i, stretched_PDF[i]);
        fprintf(f_trans_func_stretch, "%d\t%d\n", i, (int)trans_func_stretch[i]);
    }

// histogram plot 추가
float *PDF_plot = cal_PDF(input_gray);
float *stretched_PDF_plot = cal_PDF(stretched);

float max_val = 0;
for (int i = 0; i < L; i++) {
    if (PDF_plot[i] > max_val) max_val = PDF_plot[i];
    if (stretched_PDF_plot[i] > max_val) max_val = stretched_PDF_plot[i];
}

for (int i = 0; i < L; i++) {
    PDF_plot[i] /= max_val;
    stretched_PDF_plot[i] /= max_val;
}

plot(PDF_plot, "Original PDF", "PDF");
plot(stretched_PDF_plot, "Stretched PDF", "Stretched PDF");

    free(PDF_plot);
    free(stretched_PDF_plot);

    free(PDF);
    free(stretched_PDF);
    fclose(f_PDF);
    fclose(f_stretched_PDF);
    fclose(f_trans_func_stretch);

    namedWindow("Grayscale", WINDOW_AUTOSIZE);
    imshow("Grayscale", input_gray);
    namedWindow("Stretched", WINDOW_AUTOSIZE);
    imshow("Stretched", stretched);

    waitKey(0);
    return 0;
}

void linear_stretching(Mat &input, Mat &stretched, G *trans_func, G x1, G x2, G y1, G y2) {

    float constant = (y2 - y1) / (float)(x2 - x1);

    for (int i = 0; i < L; i++) {
        if (i >= 0 && i <= x1)
            trans_func[i] = (G)(y1 / (float)x1 * i);  
        else if (i > x1 && i <= x2)
            trans_func[i] = (G)(constant * (i - x1) + y1);
        else
            trans_func[i] = (G)((float)(L - 1 - y2) / (L - 1 - x2) * (i - x2) + y2);  
    }

    for (int i = 0; i < input.rows; i++)
        for (int j = 0; j < input.cols; j++)
            stretched.at<G>(i, j) = trans_func[input.at<G>(i, j)];
}