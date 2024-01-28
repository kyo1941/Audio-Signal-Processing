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
  int n, m, J, L, offset, frame, number_of_frame;
  double fe, delta, *b, *w, *x, *y;
  
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
  
  L = 256; /* �t���[���̒��� */
  
  number_of_frame = pcm0.length / L; /* �t���[���̐� */
  
  x = calloc((L + J), sizeof(double)); /* �������̊m�� */
  y = calloc(L, sizeof(double)); /* �������̊m�� */
  
  for (frame = 0; frame < number_of_frame; frame++)
  {
    offset = L * frame;
    
    /* ���O�̃t���[���̌㔼��J�T���v������������ */
    for (n = 0; n < L + J; n++)
    {
      if (offset - J + n < 0)
      {
        x[n] = 0.0;
      }
      else
      {
        x[n] = pcm0.s[offset - J + n];
      }
    }
    
    for (n = 0; n < L; n++)
    {
      y[n] = 0.0;
    }
    
    /* �t�B���^�����O */
    for (n = 0; n < L; n++)
    {
      for (m = 0; m <= J; m++)
      {
        y[n] += b[m] * x[J + n - m];
      }
    }
    
    /* �t�B���^�����O���ʂ̘A�� */
    for (n = 0; n < L; n++)
    {
      pcm1.s[offset + n] = y[n];
    }
  }
  
  mono_wave_write(&pcm1, "ex6_3.wav"); /* WAVE�t�@�C���Ƀ��m�����̉��f�[�^���o�͂��� */
  
  free(pcm0.s); /* �������̉�� */
  free(pcm1.s); /* �������̉�� */
  free(b); /* �������̉�� */
  free(w); /* �������̉�� */
  free(x); /* �������̉�� */
  free(y); /* �������̉�� */
  
  return 0;
}
