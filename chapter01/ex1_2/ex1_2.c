#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
  STEREO_PCM pcm0, pcm1;
  int n;
  
  stereo_wave_read(&pcm0, "c.wav"); /* WAVE�t�@�C������X�e���I�̉��f�[�^����͂��� */
  
  pcm1.fs = pcm0.fs; /* �W�{�����g�� */
  pcm1.bits = pcm0.bits; /* �ʎq�����x */
  pcm1.length = pcm0.length; /* ���f�[�^�̒��� */
  pcm1.sL = calloc(pcm1.length, sizeof(double)); /* �������̊m�� */
  pcm1.sR = calloc(pcm1.length, sizeof(double)); /* �������̊m�� */
  for (n = 0; n < pcm1.length; n++)
  {
    pcm1.sL[n] = pcm0.sL[n]; /* ���f�[�^�̃R�s�[ */
    pcm1.sR[n] = pcm0.sR[n]; /* ���f�[�^�̃R�s�[ */
  }

  stereo_wave_write(&pcm1, "d.wav"); /* WAVE�t�@�C���ɃX�e���I�̉��f�[�^���o�͂��� */
  
  free(pcm0.sL); /* �������̉�� */
  free(pcm0.sR); /* �������̉�� */
  free(pcm1.sL); /* �������̉�� */
  free(pcm1.sR); /* �������̉�� */
  
  return 0;
}
