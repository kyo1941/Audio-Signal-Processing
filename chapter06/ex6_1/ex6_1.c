#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "fft.h"
#include "sinc.h"
#include "fir_filter.h"
#include "wave.h"
#include "window_function.h"

int main(void) {
  MONO_PCM pcm0, pcm1, pcm2;
  int n, m, J, N;
  double fe, delta, *b, *w;
  double *x_real, *x_imag, *y_real, *y_imag;

  FILE *fp1, *fp2, *fp3, *fp4, *fp5;

  fp1 = fopen("data1.txt", "w");
  fp2 = fopen("data2.txt", "w");
  fp3 = fopen("data3.txt", "w");
  fp4 = fopen("spec1.txt", "w");
  fp5 = fopen("spec2.txt", "w");

  mono_wave_read(
      &pcm0, "sample04.wav"); /* WAVEファイルからモノラルの音データを入力する */

  pcm1.fs = pcm0.fs;                            /* 標本化周波数 */
  pcm1.bits = pcm0.bits;                        /* 量子化精度 */
  pcm1.length = pcm0.length;                    /* 音データの長さ */
  pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

  fe = 1000.0 / pcm0.fs;    /* エッジ周波数 */
  delta = 1000.0 / pcm0.fs; /* 遷移帯域幅 */

  /* 遅延期の数（0.5を足して切り捨て = 四捨五入） */
  J = (int)(3.1 / delta + 0.5) - 1;
  if (J % 2 == 1) {
    J++; /* J+1が奇数になるように調整する */
  }

  pcm2.fs = pcm0.fs;
  pcm2.bits = pcm0.bits;
  pcm2.length = pcm0.length;
  pcm2.length += J; /* 畳み込みの出力信号の長さを適切にする */
  pcm2.s = calloc(pcm1.length, sizeof(double));

  N = 2048;
  x_real = calloc(N, sizeof(double)); /* メモリの確保 */
  x_imag = calloc(N, sizeof(double)); /* メモリの確保 */
  y_real = calloc(N, sizeof(double)); /* メモリの確保 */
  y_imag = calloc(N, sizeof(double)); /* メモリの確保 */

  b = calloc((J + 1), sizeof(double)); /* メモリの確保（フィルタ用の配列） */
  w = calloc((J + 1), sizeof(double)); /* メモリの確保（窓関数用の配列） */

  /* ハニング窓（フィルタの数がシンク関数の周期の整数倍とならにくいため）*/
  Hanning_window(w, (J + 1));

  FIR_LPF(fe, J, b, w); /* FIRフィルタの設計 */

  /* フィルタリング（出力信号の長さを入力信号の長さに揃える） */
  for (n = 0; n < pcm1.length; n++) {
    for (m = 0; m <= J; m++) {
      if (n - m >= 0) {
        pcm1.s[n] += b[m] * pcm0.s[n - m]; /* 畳み込みの計算 */
      }
    }
  }

  /* フィルタリング（出力信号の長さを適切にした場合） */
  for (n = 0; n < pcm2.length; n++) {
    for (m = 0; m <= J; m++) {
      if (n - m >= pcm0.length) {
        pcm2.s[n] += 0;
      } else if (n - m >= 0) {
        pcm2.s[n] += b[m] * pcm0.s[n - m];
      }
    }
  }

  for (int n = 0; n < N; n++) {
    x_real[n] = pcm0.s[n];
    x_imag[n] = 0.0;
    y_real[n] = pcm1.s[n];
    y_imag[n] = 0.0;
  }

  FFT(x_real, x_imag, N);
  FFT(y_real, y_imag, N);

  for (int i = 0; i < pcm2.length; i++) {
    if (i < pcm1.length) {
      fprintf(fp1, "%d %f\n", i, pcm0.s[i]);
      fprintf(fp2, "%d %f\n", i, pcm1.s[i]);
    }
    fprintf(fp3, "%d %f\n", i, pcm2.s[i]);
  }

  for (int i = 0; i < N; i++) {
    fprintf(fp4, "%d %f\n", i * pcm0.fs / N,
            sqrt(pow(x_real[i], 2) + pow(x_imag[i], 2)));
    fprintf(fp5, "%d %f\n", i * pcm1.fs / N,
            sqrt(pow(y_real[i], 2) + pow(y_imag[i], 2)));
  }

  mono_wave_write(&pcm1, "ex6_1.wav");
  mono_wave_write(
      &pcm2, "ex6_1_2.wav"); /* WAVEファイルにモノラルの音データを出力する */

  free(pcm0.s); /* メモリの解放 */
  free(pcm1.s); /* メモリの解放 */
  free(pcm2.s); /* メモリの解放 */
  free(b);      /* メモリの解放 */
  free(w);      /* メモリの解放 */

  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  fclose(fp4);
  fclose(fp5);

  return 0;
}
