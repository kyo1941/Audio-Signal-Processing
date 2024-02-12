#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "fft.h"
#include "sinc.h"
#include "fir_filter.h"
#include "wave.h"
#include "window_function.h"

int main(void) {
  MONO_PCM pcm0, pcm1;
  int n, m, k, J, L, N, offset, frame, number_of_frame;
  double fe, delta, *b, *w, *b_real, *b_imag, *x_real, *x_imag, *y_real,
      *y_imag;
  double *x2_real, *x2_imag, *y2_real, *y2_imag;

  FILE *fp1, *fp2;

  fp1 = fopen("spec1.txt", "w");
  fp2 = fopen("spec2.txt", "w");

  mono_wave_read(
      &pcm0, "sample04.wav"); /* WAVEファイルからモノラルの音データを入力する */

  pcm1.fs = pcm0.fs;                            /* 標本化周波数 */
  pcm1.bits = pcm0.bits;                        /* 量子化精度 */
  pcm1.length = pcm0.length;                    /* 音データの長さ */
  pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

  fe = 1000.0 / pcm0.fs;    /* エッジ周波数 */
  delta = 1000.0 / pcm0.fs; /* 遷移帯域幅 */

  J = (int)(3.1 / delta + 0.5) - 1; /* 遅延期の数 */
  if (J % 2 == 1) {
    J++; /* J+1が奇数になるように調整する */
  }

  b = calloc((J + 1), sizeof(double)); /* メモリの確保 */
  w = calloc((J + 1), sizeof(double)); /* メモリの確保 */

  Hanning_window(w, (J + 1)); /* ハニング窓 */

  FIR_LPF(fe, J, b, w); /* FIRフィルタの設計 */

  L = 256; /* フレームの長さ */
  N = 512; /* DFTのサイズ */

  number_of_frame = pcm0.length / L; /* フレームの数 */

  b_real = calloc(N, sizeof(double)); /* メモリの確保 */
  b_imag = calloc(N, sizeof(double)); /* メモリの確保 */
  x_real = calloc(N, sizeof(double)); /* メモリの確保 */
  x_imag = calloc(N, sizeof(double)); /* メモリの確保 */
  y_real = calloc(N, sizeof(double)); /* メモリの確保 */
  y_imag = calloc(N, sizeof(double)); /* メモリの確保 */

  x2_real = calloc(N, sizeof(double)); /* メモリの確保 */
  x2_imag = calloc(N, sizeof(double)); /* メモリの確保 */
  y2_real = calloc(N, sizeof(double)); /* メモリの確保 */
  y2_imag = calloc(N, sizeof(double)); /* メモリの確保 */

  for (frame = 0; frame < number_of_frame; frame++) {
    offset = L * frame;

    /* x(n)のFFT */
    for (n = 0; n < N; n++) {
      x_real[n] = 0.0;
      x_imag[n] = 0.0;
    }
    for (n = 0; n < L; n++) {
      x_real[n] = pcm0.s[offset + n];
    }
    FFT(x_real, x_imag, N);

    /* b(m)のFFT */
    for (m = 0; m < N; m++) {
      b_real[m] = 0.0;
      b_imag[m] = 0.0;
    }
    for (m = 0; m <= J; m++) {
      b_real[m] = b[m];
    }
    FFT(b_real, b_imag, N);

    /* フィルタリング */
    for (k = 0; k < N; k++) {
      y_real[k] = x_real[k] * b_real[k] - x_imag[k] * b_imag[k];
      y_imag[k] = x_imag[k] * b_real[k] + x_real[k] * b_imag[k];
    }
    IFFT(y_real, y_imag, N);

    /* フィルタリングの結果の連結 */
    for (n = 0; n < L * 2; n++) {
      if (offset + n < pcm1.length) {
        pcm1.s[offset + n] += y_real[n];
      }
    }
  }

  for (int n = 0; n < N; n++) {
    x2_real[n] = pcm0.s[n];
    x2_imag[n] = 0.0;
    y2_real[n] = pcm1.s[n];
    y2_imag[n] = 0.0;
  }

  FFT(x2_real, x2_imag, N);
  FFT(y2_real, y2_imag, N);

  for (int i = 0; i < N; i++) {
    fprintf(fp1, "%d %f\n", i * pcm0.fs / N,
            sqrt(pow(x2_real[i], 2) + pow(x2_imag[i], 2)));
    fprintf(fp2, "%d %f\n", i * pcm1.fs / N,
            sqrt(pow(y2_real[i], 2) + pow(y2_imag[i], 2)));
  }

  mono_wave_write(&pcm1,
                  "ex6_5.wav"); /* WAVEファイルにモノラルの音データを出力する */

  free(pcm0.s); /* メモリの解放 */
  free(pcm1.s); /* メモリの解放 */
  free(b);      /* メモリの解放 */
  free(w);      /* メモリの解放 */
  free(b_real); /* メモリの解放 */
  free(b_imag); /* メモリの解放 */
  free(x_real); /* メモリの解放 */
  free(x_imag); /* メモリの解放 */
  free(y_real); /* メモリの解放 */
  free(y_imag); /* メモリの解放 */

  free(x2_real); /* メモリの解放 */
  free(x2_imag); /* メモリの解放 */
  free(y2_real); /* メモリの解放 */
  free(y2_imag); /* メモリの解放 */

  fclose(fp1);
  fclose(fp2);

  return 0;
}
