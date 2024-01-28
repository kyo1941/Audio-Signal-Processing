#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "iir_filter.h"
#include "wave.h"

int main(void) {
  MONO_PCM pcm0, pcm1;
  int n, m, I, J;
  double fc, Q, a[3], b[3];

  mono_wave_read(
      &pcm0, "sample04.wav"); /* WAVEファイルからモノラルの音データを入力する */

  pcm1.fs = pcm0.fs;                            /* 標本化周波数 */
  pcm1.bits = pcm0.bits;                        /* 量子化精度 */
  pcm1.length = pcm0.length;                    /* 音データの長さ */
  pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

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
  }

  mono_wave_write(&pcm1,
                  "ex6_2.wav"); /* WAVEファイルにモノラルの音データを出力する */

  free(pcm0.s); /* メモリの解放 */
  free(pcm1.s); /* メモリの解放 */

  return 0;
}
