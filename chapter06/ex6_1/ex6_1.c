#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "window_function.h"
#include "sinc.h"
#include "fir_filter.h"

int main(void)
{
  MONO_PCM pcm0, pcm1;
  int n, m, J;
  double fe, delta, *b, *w;
  
  mono_wave_read(&pcm0, "sample04.wav"); /* WAVE�t�@�C�����烂�m�����̉��f�[�^����͂��� */
  
  pcm1.fs = pcm0.fs; /* �W�{�����g�� */
  pcm1.bits = pcm0.bits; /* �ʎq�����x */
  pcm1.length = pcm0.length; /* ���f�[�^�̒��� */
  pcm1.s = calloc(pcm1.length, sizeof(double)); /* �������̊m�� */
  
  fe = 1000.0 / pcm0.fs; /* �G�b�W���g�� */
  delta = 1000.0 / pcm0.fs; /* �J�ڑш敝 */
  
  J = (int)(3.1 / delta + 0.5) - 1; /* �x����̐� */
  if (J % 2 == 1)
  {
    J++; /* J+1����ɂȂ�悤�ɒ������� */
  }
  
  b = calloc((J + 1), sizeof(double)); /* �������̊m�� */
  w = calloc((J + 1), sizeof(double)); /* �������̊m�� */
  
  Hanning_window(w, (J + 1)); /* �n�j���O�� */
  
  FIR_LPF(fe, J, b, w); /* FIR�t�B���^�̐݌v */
  
  /* �t�B���^�����O */
  for (n = 0; n < pcm1.length; n++)
  {
    for (m = 0; m <= J; m++)
    {
      if (n - m >= 0)
      {
        pcm1.s[n] += b[m] * pcm0.s[n - m];
      }
    }
  }
  
  mono_wave_write(&pcm1, "ex6_1.wav"); /* WAVE�t�@�C���Ƀ��m�����̉��f�[�^���o�͂��� */
  
  free(pcm0.s); /* �������̉�� */
  free(pcm1.s); /* �������̉�� */
  free(b); /* �������̉�� */
  free(w); /* �������̉�� */
  
  return 0;
}
