#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "fft.h"
#include "iir_filter.h"
#include "wave.h"

int main(void) {
  MONO_PCM pcm0, pcm1;
  int n, m, I, J, N;
  double fc, Q, a[3], b[3];
  double *x_real, *x_imag, *y_real, *y_imag;

  FILE *fp1, *fp2, *fp3, *fp4;

  fp1 = fopen("data1.txt", "w");
  fp2 = fopen("data2.txt", "w");

  fp3 = fopen("spec1.txt", "w");
  fp4 = fopen("spec2.txt", "w");

  mono_wave_read(
      &pcm0, "sample04.wav"); /* WAVEファイルからモノラルの音データを入力する */

  pcm1.fs = pcm0.fs;                            /* 標本化周波数 */
  pcm1.bits = pcm0.bits;                        /* 量子化精度 */
  pcm1.length = pcm0.length;                    /* 音データの長さ */
  pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

  N = 2048;
  x_real = calloc(N, sizeof(double)); /* メモリの確保 */
  x_imag = calloc(N, sizeof(double)); /* メモリの確保 */
  y_real = calloc(N, sizeof(double)); /* メモリの確保 */
  y_imag = calloc(N, sizeof(double)); /* メモリの確保 */

  fc = 1000.0 / pcm0.fs; /* 遮断周波数 */
  Q = 1.0 / sqrt(2.0);   /* クオリティファクタ */
  I = 2;                 /* 遅延期の数 */
  J = 2;                 /* 遅延期の数 */

  IIR_LPF(fc, Q, a, b); /* IIRフィルタの設計 */

  /* フィルタリング */
  for (n = 0; n < pcm1.length; n++) {
    for (m = 0; m <= J; m++) {
      if (n - m >= 0) {
        pcm1.s[n] += b[m] * pcm0.s[n - m];
      }
    }
    for (m = 1; m <= I; m++) {
      if (n - m >= 0) {
        pcm1.s[n] += -a[m] * pcm1.s[n - m];
      }
    }

    fprintf(fp1, "%d %f\n", n, pcm0.s[n]);
    fprintf(fp2, "%d %f\n", n, pcm1.s[n]);
  }

  for (int n = 0; n < N; n++) {
    x_real[n] = pcm0.s[n];
    x_imag[n] = 0.0;
    y_real[n] = pcm1.s[n];
    y_imag[n] = 0.0;
  }

  FFT(x_real, x_imag, N);
  FFT(y_real, y_imag, N);

  for (int i = 0; i < N; i++) {
    fprintf(fp3, "%d %f\n", i * pcm0.fs / N,
            sqrt(pow(x_real[i], 2) + pow(x_imag[i], 2)));
    fprintf(fp4, "%d %f\n", i * pcm1.fs / N,
            sqrt(pow(y_real[i], 2) + pow(y_imag[i], 2)));
  }

  mono_wave_write(&pcm1,
                  "ex6_2.wav"); /* WAVEファイルにモノラルの音データを出力する */

  free(pcm0.s); /* メモリの解放 */
  free(pcm1.s); /* メモリの解放 */

  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  fclose(fp4);

  return 0;
}
