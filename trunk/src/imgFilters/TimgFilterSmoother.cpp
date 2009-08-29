/*
 * Copyright (c) 2002-2006 Milan Cutka
 * based on VirtualDub Smoother filter by Avery Lee
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License,or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not,write to the Free Software
 * Foundation,Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "stdafx.h"
#include "TimgFilterSmoother.h"
#include "TblurSettings.h"

//================================= TimgFilterSmootherLuma::Tblur =================================
void TimgFilterSmootherLuma::Tblur::avgrow(const unsigned char *src,unsigned char *dst,unsigned int width)
{
 int r0=*src++;

 int sum=r0*4;

 unsigned int w=1;
 do
 {
  unsigned char c=src[0];
  sum+=c;
  ++src;
 } while(--w);

 w=3;
 do
 {
  unsigned char c=src[0];
  sum=sum+c-r0;
  *dst++=(unsigned char)(sum/5);
  ++src;
 } while(--w);

 w=width-6;
 do
 {
  unsigned char c=src[0],d=src[-5];
  sum=sum+c-d;
  *dst++=(unsigned char)(sum/5);
  ++src;
 } while(--w);

 r0=*src++;

 w=3;
 do
 {
  unsigned char d=src[-5];
  sum=sum-d+r0;
  *dst++=(unsigned char)(sum/5);
  ++src;
 } while(--w);
}
void TimgFilterSmootherLuma::Tblur::filtrow_1(unsigned char *sum,const unsigned char *src,unsigned int dx,stride_t stride,int thresh)
{
 long bitarray=0;
 src=src-stride;

 do
 {
  // Fetch surrounding pixels.
  unsigned char pl=*((src-1)+stride);
  unsigned char pr=*((src+1)+stride);
  unsigned char pu=*src;
  unsigned char pd=*(src+stride*2);

  // Compute gradient at pixel.
  int grad_x=pr-pl;
  int grad_y=pu-pd;

  bitarray >>=1;

  //        if (grad_x*grad_x+grad_y*grad_y > thresh) {
  if (smoother_blur_square_table[765+grad_x]+smoother_blur_square_table[765+grad_y] > thresh)
   bitarray |=16;

  *sum++=smoother_blur_codes[bitarray];
  ++src;
 } while(--dx>0);
 *sum++=smoother_blur_codes[bitarray>>1];
 *sum++=smoother_blur_codes[bitarray>>2];
}
TimgFilterSmootherLuma::Tblur::Tblur(unsigned int Idx,unsigned int Idy):dx(Idx),dy(Idy)
{
 for (int i=0;i<5;i++) sum_row[i]=new unsigned char[dx+2];
 for (int i=0;i<5;i++) avg_row[i]=new unsigned char[dx];
}
TimgFilterSmootherLuma::Tblur::~Tblur()
{
 for(int i=0;i<5;i++) if (sum_row[i]) delete []sum_row[i];
 for(int i=0;i<5;i++) if (avg_row[i]) delete []avg_row[i];
}
void TimgFilterSmootherLuma::Tblur::run(const unsigned char *src,stride_t srcStride,unsigned char *dst,stride_t dstStride,int g_thresh)
{
 long w,h,x;
 int next_row=0,next_avg_row=0;
 unsigned char *avg_rows[5];
 unsigned char *row[5];
 int i;

 const unsigned char *srcf;
 unsigned char *avg;

 srcf=src;

 avgrow(src+0*srcStride,avg_row[1],dx);
 memcpy(avg_row[2],avg_row[1],dx);
 memcpy(avg_row[3],avg_row[1],dx);
 avgrow(src+1*srcStride,avg_row[4],dx);

 memset(sum_row[1],0,dx+2);
 memset(sum_row[2],0,dx+2);
 memset(sum_row[3],0,dx+2);

 filtrow_1(sum_row[4],(srcf+1)+srcStride,dx-2,srcStride,g_thresh);

 h=dy;
 do
 {
  if (h>3)
   filtrow_1(sum_row[next_row],(srcf+1)+srcStride*2,dx-2,srcStride,g_thresh);
  else
   memset(sum_row[next_row],0,dx+2);

  avg=avg_row[next_avg_row];

  if (h>2)
   avgrow(src+2*srcStride,avg,dx);
  else
   memcpy(avg,avg_row[(next_avg_row+4)%5],dx);

  if (++next_row>=5) next_row=0;
  for(i=0;i<5;i++) row[i]=sum_row[(next_row+i)%5];
  if (++next_avg_row>=5) next_avg_row=0;
  for(i=0;i<5;i++) avg_rows[i]=avg_row[(next_avg_row+i)%5];

  w=dx;
  x=0;
  do
  {
   int s=(int)(row[0][x]&0xe0)+(int)row[1][x]+(int)row[2][x]+(int)row[3][x]+(int)(row[4][x]&0xe0);
   int A=s&31;

   if (A>3)
    *dst=*src;
   else
    {
     int B=s>>5;

     int p0=avg_rows[0][x];
     int p1=avg_rows[1][x];
     int p2=avg_rows[2][x];
     int p3=avg_rows[3][x];
     int p4=avg_rows[4][x];
     int r=p0+p1*2+p2*2+p3*2+p4;

     if (A>1)
      {
       int d=*src;
       r+=d<<3;
       *dst=(unsigned char)(r>>4);
      }
     else if (B>3)
      {
       int d=*src;
       r=r*3+(d<<3);
       *dst=(unsigned char)(r>>5);
      }
     else
      *dst=(unsigned char)(r>>3);
    }
   ++src;
   ++dst;
  } while(++x<w);
  src+=srcStride-dx;
  dst+=dstStride-dx;
  srcf+=srcStride;
 } while(--h>0);
}

const int TimgFilterSmootherLuma::Tblur::smoother_blur_square_table[765*2+1]=
{
 0x0008ee09,0x0008e810,0x0008e219,0x0008dc24,0x0008d631,0x0008d040,0x0008ca51,0x0008c464,
 0x0008be79,0x0008b890,0x0008b2a9,0x0008acc4,0x0008a6e1,0x0008a100,0x00089b21,0x00089544,
 0x00088f69,0x00088990,0x000883b9,0x00087de4,0x00087811,0x00087240,0x00086c71,0x000866a4,
 0x000860d9,0x00085b10,0x00085549,0x00084f84,0x000849c1,0x00084400,0x00083e41,0x00083884,
 0x000832c9,0x00082d10,0x00082759,0x000821a4,0x00081bf1,0x00081640,0x00081091,0x00080ae4,
 0x00080539,0x0007ff90,0x0007f9e9,0x0007f444,0x0007eea1,0x0007e900,0x0007e361,0x0007ddc4,
 0x0007d829,0x0007d290,0x0007ccf9,0x0007c764,0x0007c1d1,0x0007bc40,0x0007b6b1,0x0007b124,
 0x0007ab99,0x0007a610,0x0007a089,0x00079b04,0x00079581,0x00079000,0x00078a81,0x00078504,
 0x00077f89,0x00077a10,0x00077499,0x00076f24,0x000769b1,0x00076440,0x00075ed1,0x00075964,
 0x000753f9,0x00074e90,0x00074929,0x000743c4,0x00073e61,0x00073900,0x000733a1,0x00072e44,
 0x000728e9,0x00072390,0x00071e39,0x000718e4,0x00071391,0x00070e40,0x000708f1,0x000703a4,
 0x0006fe59,0x0006f910,0x0006f3c9,0x0006ee84,0x0006e941,0x0006e400,0x0006dec1,0x0006d984,
 0x0006d449,0x0006cf10,0x0006c9d9,0x0006c4a4,0x0006bf71,0x0006ba40,0x0006b511,0x0006afe4,
 0x0006aab9,0x0006a590,0x0006a069,0x00069b44,0x00069621,0x00069100,0x00068be1,0x000686c4,
 0x000681a9,0x00067c90,0x00067779,0x00067264,0x00066d51,0x00066840,0x00066331,0x00065e24,
 0x00065919,0x00065410,0x00064f09,0x00064a04,0x00064501,0x00064000,0x00063b01,0x00063604,
 0x00063109,0x00062c10,0x00062719,0x00062224,0x00061d31,0x00061840,0x00061351,0x00060e64,
 0x00060979,0x00060490,0x0005ffa9,0x0005fac4,0x0005f5e1,0x0005f100,0x0005ec21,0x0005e744,
 0x0005e269,0x0005dd90,0x0005d8b9,0x0005d3e4,0x0005cf11,0x0005ca40,0x0005c571,0x0005c0a4,
 0x0005bbd9,0x0005b710,0x0005b249,0x0005ad84,0x0005a8c1,0x0005a400,0x00059f41,0x00059a84,
 0x000595c9,0x00059110,0x00058c59,0x000587a4,0x000582f1,0x00057e40,0x00057991,0x000574e4,
 0x00057039,0x00056b90,0x000566e9,0x00056244,0x00055da1,0x00055900,0x00055461,0x00054fc4,
 0x00054b29,0x00054690,0x000541f9,0x00053d64,0x000538d1,0x00053440,0x00052fb1,0x00052b24,
 0x00052699,0x00052210,0x00051d89,0x00051904,0x00051481,0x00051000,0x00050b81,0x00050704,
 0x00050289,0x0004fe10,0x0004f999,0x0004f524,0x0004f0b1,0x0004ec40,0x0004e7d1,0x0004e364,
 0x0004def9,0x0004da90,0x0004d629,0x0004d1c4,0x0004cd61,0x0004c900,0x0004c4a1,0x0004c044,
 0x0004bbe9,0x0004b790,0x0004b339,0x0004aee4,0x0004aa91,0x0004a640,0x0004a1f1,0x00049da4,
 0x00049959,0x00049510,0x000490c9,0x00048c84,0x00048841,0x00048400,0x00047fc1,0x00047b84,
 0x00047749,0x00047310,0x00046ed9,0x00046aa4,0x00046671,0x00046240,0x00045e11,0x000459e4,
 0x000455b9,0x00045190,0x00044d69,0x00044944,0x00044521,0x00044100,0x00043ce1,0x000438c4,
 0x000434a9,0x00043090,0x00042c79,0x00042864,0x00042451,0x00042040,0x00041c31,0x00041824,
 0x00041419,0x00041010,0x00040c09,0x00040804,0x00040401,0x00040000,0x0003fc01,0x0003f804,
 0x0003f409,0x0003f010,0x0003ec19,0x0003e824,0x0003e431,0x0003e040,0x0003dc51,0x0003d864,
 0x0003d479,0x0003d090,0x0003cca9,0x0003c8c4,0x0003c4e1,0x0003c100,0x0003bd21,0x0003b944,
 0x0003b569,0x0003b190,0x0003adb9,0x0003a9e4,0x0003a611,0x0003a240,0x00039e71,0x00039aa4,
 0x000396d9,0x00039310,0x00038f49,0x00038b84,0x000387c1,0x00038400,0x00038041,0x00037c84,
 0x000378c9,0x00037510,0x00037159,0x00036da4,0x000369f1,0x00036640,0x00036291,0x00035ee4,
 0x00035b39,0x00035790,0x000353e9,0x00035044,0x00034ca1,0x00034900,0x00034561,0x000341c4,
 0x00033e29,0x00033a90,0x000336f9,0x00033364,0x00032fd1,0x00032c40,0x000328b1,0x00032524,
 0x00032199,0x00031e10,0x00031a89,0x00031704,0x00031381,0x00031000,0x00030c81,0x00030904,
 0x00030589,0x00030210,0x0002fe99,0x0002fb24,0x0002f7b1,0x0002f440,0x0002f0d1,0x0002ed64,
 0x0002e9f9,0x0002e690,0x0002e329,0x0002dfc4,0x0002dc61,0x0002d900,0x0002d5a1,0x0002d244,
 0x0002cee9,0x0002cb90,0x0002c839,0x0002c4e4,0x0002c191,0x0002be40,0x0002baf1,0x0002b7a4,
 0x0002b459,0x0002b110,0x0002adc9,0x0002aa84,0x0002a741,0x0002a400,0x0002a0c1,0x00029d84,
 0x00029a49,0x00029710,0x000293d9,0x000290a4,0x00028d71,0x00028a40,0x00028711,0x000283e4,
 0x000280b9,0x00027d90,0x00027a69,0x00027744,0x00027421,0x00027100,0x00026de1,0x00026ac4,
 0x000267a9,0x00026490,0x00026179,0x00025e64,0x00025b51,0x00025840,0x00025531,0x00025224,
 0x00024f19,0x00024c10,0x00024909,0x00024604,0x00024301,0x00024000,0x00023d01,0x00023a04,
 0x00023709,0x00023410,0x00023119,0x00022e24,0x00022b31,0x00022840,0x00022551,0x00022264,
 0x00021f79,0x00021c90,0x000219a9,0x000216c4,0x000213e1,0x00021100,0x00020e21,0x00020b44,
 0x00020869,0x00020590,0x000202b9,0x0001ffe4,0x0001fd11,0x0001fa40,0x0001f771,0x0001f4a4,
 0x0001f1d9,0x0001ef10,0x0001ec49,0x0001e984,0x0001e6c1,0x0001e400,0x0001e141,0x0001de84,
 0x0001dbc9,0x0001d910,0x0001d659,0x0001d3a4,0x0001d0f1,0x0001ce40,0x0001cb91,0x0001c8e4,
 0x0001c639,0x0001c390,0x0001c0e9,0x0001be44,0x0001bba1,0x0001b900,0x0001b661,0x0001b3c4,
 0x0001b129,0x0001ae90,0x0001abf9,0x0001a964,0x0001a6d1,0x0001a440,0x0001a1b1,0x00019f24,
 0x00019c99,0x00019a10,0x00019789,0x00019504,0x00019281,0x00019000,0x00018d81,0x00018b04,
 0x00018889,0x00018610,0x00018399,0x00018124,0x00017eb1,0x00017c40,0x000179d1,0x00017764,
 0x000174f9,0x00017290,0x00017029,0x00016dc4,0x00016b61,0x00016900,0x000166a1,0x00016444,
 0x000161e9,0x00015f90,0x00015d39,0x00015ae4,0x00015891,0x00015640,0x000153f1,0x000151a4,
 0x00014f59,0x00014d10,0x00014ac9,0x00014884,0x00014641,0x00014400,0x000141c1,0x00013f84,
 0x00013d49,0x00013b10,0x000138d9,0x000136a4,0x00013471,0x00013240,0x00013011,0x00012de4,
 0x00012bb9,0x00012990,0x00012769,0x00012544,0x00012321,0x00012100,0x00011ee1,0x00011cc4,
 0x00011aa9,0x00011890,0x00011679,0x00011464,0x00011251,0x00011040,0x00010e31,0x00010c24,
 0x00010a19,0x00010810,0x00010609,0x00010404,0x00010201,0x00010000,0x0000fe01,0x0000fc04,
 0x0000fa09,0x0000f810,0x0000f619,0x0000f424,0x0000f231,0x0000f040,0x0000ee51,0x0000ec64,
 0x0000ea79,0x0000e890,0x0000e6a9,0x0000e4c4,0x0000e2e1,0x0000e100,0x0000df21,0x0000dd44,
 0x0000db69,0x0000d990,0x0000d7b9,0x0000d5e4,0x0000d411,0x0000d240,0x0000d071,0x0000cea4,
 0x0000ccd9,0x0000cb10,0x0000c949,0x0000c784,0x0000c5c1,0x0000c400,0x0000c241,0x0000c084,
 0x0000bec9,0x0000bd10,0x0000bb59,0x0000b9a4,0x0000b7f1,0x0000b640,0x0000b491,0x0000b2e4,
 0x0000b139,0x0000af90,0x0000ade9,0x0000ac44,0x0000aaa1,0x0000a900,0x0000a761,0x0000a5c4,
 0x0000a429,0x0000a290,0x0000a0f9,0x00009f64,0x00009dd1,0x00009c40,0x00009ab1,0x00009924,
 0x00009799,0x00009610,0x00009489,0x00009304,0x00009181,0x00009000,0x00008e81,0x00008d04,
 0x00008b89,0x00008a10,0x00008899,0x00008724,0x000085b1,0x00008440,0x000082d1,0x00008164,
 0x00007ff9,0x00007e90,0x00007d29,0x00007bc4,0x00007a61,0x00007900,0x000077a1,0x00007644,
 0x000074e9,0x00007390,0x00007239,0x000070e4,0x00006f91,0x00006e40,0x00006cf1,0x00006ba4,
 0x00006a59,0x00006910,0x000067c9,0x00006684,0x00006541,0x00006400,0x000062c1,0x00006184,
 0x00006049,0x00005f10,0x00005dd9,0x00005ca4,0x00005b71,0x00005a40,0x00005911,0x000057e4,
 0x000056b9,0x00005590,0x00005469,0x00005344,0x00005221,0x00005100,0x00004fe1,0x00004ec4,
 0x00004da9,0x00004c90,0x00004b79,0x00004a64,0x00004951,0x00004840,0x00004731,0x00004624,
 0x00004519,0x00004410,0x00004309,0x00004204,0x00004101,0x00004000,0x00003f01,0x00003e04,
 0x00003d09,0x00003c10,0x00003b19,0x00003a24,0x00003931,0x00003840,0x00003751,0x00003664,
 0x00003579,0x00003490,0x000033a9,0x000032c4,0x000031e1,0x00003100,0x00003021,0x00002f44,
 0x00002e69,0x00002d90,0x00002cb9,0x00002be4,0x00002b11,0x00002a40,0x00002971,0x000028a4,
 0x000027d9,0x00002710,0x00002649,0x00002584,0x000024c1,0x00002400,0x00002341,0x00002284,
 0x000021c9,0x00002110,0x00002059,0x00001fa4,0x00001ef1,0x00001e40,0x00001d91,0x00001ce4,
 0x00001c39,0x00001b90,0x00001ae9,0x00001a44,0x000019a1,0x00001900,0x00001861,0x000017c4,
 0x00001729,0x00001690,0x000015f9,0x00001564,0x000014d1,0x00001440,0x000013b1,0x00001324,
 0x00001299,0x00001210,0x00001189,0x00001104,0x00001081,0x00001000,0x00000f81,0x00000f04,
 0x00000e89,0x00000e10,0x00000d99,0x00000d24,0x00000cb1,0x00000c40,0x00000bd1,0x00000b64,
 0x00000af9,0x00000a90,0x00000a29,0x000009c4,0x00000961,0x00000900,0x000008a1,0x00000844,
 0x000007e9,0x00000790,0x00000739,0x000006e4,0x00000691,0x00000640,0x000005f1,0x000005a4,
 0x00000559,0x00000510,0x000004c9,0x00000484,0x00000441,0x00000400,0x000003c1,0x00000384,
 0x00000349,0x00000310,0x000002d9,0x000002a4,0x00000271,0x00000240,0x00000211,0x000001e4,
 0x000001b9,0x00000190,0x00000169,0x00000144,0x00000121,0x00000100,0x000000e1,0x000000c4,
 0x000000a9,0x00000090,0x00000079,0x00000064,0x00000051,0x00000040,0x00000031,0x00000024,
 0x00000019,0x00000010,0x00000009,0x00000004,0x00000001,0x00000000,0x00000001,0x00000004,
 0x00000009,0x00000010,0x00000019,0x00000024,0x00000031,0x00000040,0x00000051,0x00000064,
 0x00000079,0x00000090,0x000000a9,0x000000c4,0x000000e1,0x00000100,0x00000121,0x00000144,
 0x00000169,0x00000190,0x000001b9,0x000001e4,0x00000211,0x00000240,0x00000271,0x000002a4,
 0x000002d9,0x00000310,0x00000349,0x00000384,0x000003c1,0x00000400,0x00000441,0x00000484,
 0x000004c9,0x00000510,0x00000559,0x000005a4,0x000005f1,0x00000640,0x00000691,0x000006e4,
 0x00000739,0x00000790,0x000007e9,0x00000844,0x000008a1,0x00000900,0x00000961,0x000009c4,
 0x00000a29,0x00000a90,0x00000af9,0x00000b64,0x00000bd1,0x00000c40,0x00000cb1,0x00000d24,
 0x00000d99,0x00000e10,0x00000e89,0x00000f04,0x00000f81,0x00001000,0x00001081,0x00001104,
 0x00001189,0x00001210,0x00001299,0x00001324,0x000013b1,0x00001440,0x000014d1,0x00001564,
 0x000015f9,0x00001690,0x00001729,0x000017c4,0x00001861,0x00001900,0x000019a1,0x00001a44,
 0x00001ae9,0x00001b90,0x00001c39,0x00001ce4,0x00001d91,0x00001e40,0x00001ef1,0x00001fa4,
 0x00002059,0x00002110,0x000021c9,0x00002284,0x00002341,0x00002400,0x000024c1,0x00002584,
 0x00002649,0x00002710,0x000027d9,0x000028a4,0x00002971,0x00002a40,0x00002b11,0x00002be4,
 0x00002cb9,0x00002d90,0x00002e69,0x00002f44,0x00003021,0x00003100,0x000031e1,0x000032c4,
 0x000033a9,0x00003490,0x00003579,0x00003664,0x00003751,0x00003840,0x00003931,0x00003a24,
 0x00003b19,0x00003c10,0x00003d09,0x00003e04,0x00003f01,0x00004000,0x00004101,0x00004204,
 0x00004309,0x00004410,0x00004519,0x00004624,0x00004731,0x00004840,0x00004951,0x00004a64,
 0x00004b79,0x00004c90,0x00004da9,0x00004ec4,0x00004fe1,0x00005100,0x00005221,0x00005344,
 0x00005469,0x00005590,0x000056b9,0x000057e4,0x00005911,0x00005a40,0x00005b71,0x00005ca4,
 0x00005dd9,0x00005f10,0x00006049,0x00006184,0x000062c1,0x00006400,0x00006541,0x00006684,
 0x000067c9,0x00006910,0x00006a59,0x00006ba4,0x00006cf1,0x00006e40,0x00006f91,0x000070e4,
 0x00007239,0x00007390,0x000074e9,0x00007644,0x000077a1,0x00007900,0x00007a61,0x00007bc4,
 0x00007d29,0x00007e90,0x00007ff9,0x00008164,0x000082d1,0x00008440,0x000085b1,0x00008724,
 0x00008899,0x00008a10,0x00008b89,0x00008d04,0x00008e81,0x00009000,0x00009181,0x00009304,
 0x00009489,0x00009610,0x00009799,0x00009924,0x00009ab1,0x00009c40,0x00009dd1,0x00009f64,
 0x0000a0f9,0x0000a290,0x0000a429,0x0000a5c4,0x0000a761,0x0000a900,0x0000aaa1,0x0000ac44,
 0x0000ade9,0x0000af90,0x0000b139,0x0000b2e4,0x0000b491,0x0000b640,0x0000b7f1,0x0000b9a4,
 0x0000bb59,0x0000bd10,0x0000bec9,0x0000c084,0x0000c241,0x0000c400,0x0000c5c1,0x0000c784,
 0x0000c949,0x0000cb10,0x0000ccd9,0x0000cea4,0x0000d071,0x0000d240,0x0000d411,0x0000d5e4,
 0x0000d7b9,0x0000d990,0x0000db69,0x0000dd44,0x0000df21,0x0000e100,0x0000e2e1,0x0000e4c4,
 0x0000e6a9,0x0000e890,0x0000ea79,0x0000ec64,0x0000ee51,0x0000f040,0x0000f231,0x0000f424,
 0x0000f619,0x0000f810,0x0000fa09,0x0000fc04,0x0000fe01,0x00010000,0x00010201,0x00010404,
 0x00010609,0x00010810,0x00010a19,0x00010c24,0x00010e31,0x00011040,0x00011251,0x00011464,
 0x00011679,0x00011890,0x00011aa9,0x00011cc4,0x00011ee1,0x00012100,0x00012321,0x00012544,
 0x00012769,0x00012990,0x00012bb9,0x00012de4,0x00013011,0x00013240,0x00013471,0x000136a4,
 0x000138d9,0x00013b10,0x00013d49,0x00013f84,0x000141c1,0x00014400,0x00014641,0x00014884,
 0x00014ac9,0x00014d10,0x00014f59,0x000151a4,0x000153f1,0x00015640,0x00015891,0x00015ae4,
 0x00015d39,0x00015f90,0x000161e9,0x00016444,0x000166a1,0x00016900,0x00016b61,0x00016dc4,
 0x00017029,0x00017290,0x000174f9,0x00017764,0x000179d1,0x00017c40,0x00017eb1,0x00018124,
 0x00018399,0x00018610,0x00018889,0x00018b04,0x00018d81,0x00019000,0x00019281,0x00019504,
 0x00019789,0x00019a10,0x00019c99,0x00019f24,0x0001a1b1,0x0001a440,0x0001a6d1,0x0001a964,
 0x0001abf9,0x0001ae90,0x0001b129,0x0001b3c4,0x0001b661,0x0001b900,0x0001bba1,0x0001be44,
 0x0001c0e9,0x0001c390,0x0001c639,0x0001c8e4,0x0001cb91,0x0001ce40,0x0001d0f1,0x0001d3a4,
 0x0001d659,0x0001d910,0x0001dbc9,0x0001de84,0x0001e141,0x0001e400,0x0001e6c1,0x0001e984,
 0x0001ec49,0x0001ef10,0x0001f1d9,0x0001f4a4,0x0001f771,0x0001fa40,0x0001fd11,0x0001ffe4,
 0x000202b9,0x00020590,0x00020869,0x00020b44,0x00020e21,0x00021100,0x000213e1,0x000216c4,
 0x000219a9,0x00021c90,0x00021f79,0x00022264,0x00022551,0x00022840,0x00022b31,0x00022e24,
 0x00023119,0x00023410,0x00023709,0x00023a04,0x00023d01,0x00024000,0x00024301,0x00024604,
 0x00024909,0x00024c10,0x00024f19,0x00025224,0x00025531,0x00025840,0x00025b51,0x00025e64,
 0x00026179,0x00026490,0x000267a9,0x00026ac4,0x00026de1,0x00027100,0x00027421,0x00027744,
 0x00027a69,0x00027d90,0x000280b9,0x000283e4,0x00028711,0x00028a40,0x00028d71,0x000290a4,
 0x000293d9,0x00029710,0x00029a49,0x00029d84,0x0002a0c1,0x0002a400,0x0002a741,0x0002aa84,
 0x0002adc9,0x0002b110,0x0002b459,0x0002b7a4,0x0002baf1,0x0002be40,0x0002c191,0x0002c4e4,
 0x0002c839,0x0002cb90,0x0002cee9,0x0002d244,0x0002d5a1,0x0002d900,0x0002dc61,0x0002dfc4,
 0x0002e329,0x0002e690,0x0002e9f9,0x0002ed64,0x0002f0d1,0x0002f440,0x0002f7b1,0x0002fb24,
 0x0002fe99,0x00030210,0x00030589,0x00030904,0x00030c81,0x00031000,0x00031381,0x00031704,
 0x00031a89,0x00031e10,0x00032199,0x00032524,0x000328b1,0x00032c40,0x00032fd1,0x00033364,
 0x000336f9,0x00033a90,0x00033e29,0x000341c4,0x00034561,0x00034900,0x00034ca1,0x00035044,
 0x000353e9,0x00035790,0x00035b39,0x00035ee4,0x00036291,0x00036640,0x000369f1,0x00036da4,
 0x00037159,0x00037510,0x000378c9,0x00037c84,0x00038041,0x00038400,0x000387c1,0x00038b84,
 0x00038f49,0x00039310,0x000396d9,0x00039aa4,0x00039e71,0x0003a240,0x0003a611,0x0003a9e4,
 0x0003adb9,0x0003b190,0x0003b569,0x0003b944,0x0003bd21,0x0003c100,0x0003c4e1,0x0003c8c4,
 0x0003cca9,0x0003d090,0x0003d479,0x0003d864,0x0003dc51,0x0003e040,0x0003e431,0x0003e824,
 0x0003ec19,0x0003f010,0x0003f409,0x0003f804,0x0003fc01,0x00040000,0x00040401,0x00040804,
 0x00040c09,0x00041010,0x00041419,0x00041824,0x00041c31,0x00042040,0x00042451,0x00042864,
 0x00042c79,0x00043090,0x000434a9,0x000438c4,0x00043ce1,0x00044100,0x00044521,0x00044944,
 0x00044d69,0x00045190,0x000455b9,0x000459e4,0x00045e11,0x00046240,0x00046671,0x00046aa4,
 0x00046ed9,0x00047310,0x00047749,0x00047b84,0x00047fc1,0x00048400,0x00048841,0x00048c84,
 0x000490c9,0x00049510,0x00049959,0x00049da4,0x0004a1f1,0x0004a640,0x0004aa91,0x0004aee4,
 0x0004b339,0x0004b790,0x0004bbe9,0x0004c044,0x0004c4a1,0x0004c900,0x0004cd61,0x0004d1c4,
 0x0004d629,0x0004da90,0x0004def9,0x0004e364,0x0004e7d1,0x0004ec40,0x0004f0b1,0x0004f524,
 0x0004f999,0x0004fe10,0x00050289,0x00050704,0x00050b81,0x00051000,0x00051481,0x00051904,
 0x00051d89,0x00052210,0x00052699,0x00052b24,0x00052fb1,0x00053440,0x000538d1,0x00053d64,
 0x000541f9,0x00054690,0x00054b29,0x00054fc4,0x00055461,0x00055900,0x00055da1,0x00056244,
 0x000566e9,0x00056b90,0x00057039,0x000574e4,0x00057991,0x00057e40,0x000582f1,0x000587a4,
 0x00058c59,0x00059110,0x000595c9,0x00059a84,0x00059f41,0x0005a400,0x0005a8c1,0x0005ad84,
 0x0005b249,0x0005b710,0x0005bbd9,0x0005c0a4,0x0005c571,0x0005ca40,0x0005cf11,0x0005d3e4,
 0x0005d8b9,0x0005dd90,0x0005e269,0x0005e744,0x0005ec21,0x0005f100,0x0005f5e1,0x0005fac4,
 0x0005ffa9,0x00060490,0x00060979,0x00060e64,0x00061351,0x00061840,0x00061d31,0x00062224,
 0x00062719,0x00062c10,0x00063109,0x00063604,0x00063b01,0x00064000,0x00064501,0x00064a04,
 0x00064f09,0x00065410,0x00065919,0x00065e24,0x00066331,0x00066840,0x00066d51,0x00067264,
 0x00067779,0x00067c90,0x000681a9,0x000686c4,0x00068be1,0x00069100,0x00069621,0x00069b44,
 0x0006a069,0x0006a590,0x0006aab9,0x0006afe4,0x0006b511,0x0006ba40,0x0006bf71,0x0006c4a4,
 0x0006c9d9,0x0006cf10,0x0006d449,0x0006d984,0x0006dec1,0x0006e400,0x0006e941,0x0006ee84,
 0x0006f3c9,0x0006f910,0x0006fe59,0x000703a4,0x000708f1,0x00070e40,0x00071391,0x000718e4,
 0x00071e39,0x00072390,0x000728e9,0x00072e44,0x000733a1,0x00073900,0x00073e61,0x000743c4,
 0x00074929,0x00074e90,0x000753f9,0x00075964,0x00075ed1,0x00076440,0x000769b1,0x00076f24,
 0x00077499,0x00077a10,0x00077f89,0x00078504,0x00078a81,0x00079000,0x00079581,0x00079b04,
 0x0007a089,0x0007a610,0x0007ab99,0x0007b124,0x0007b6b1,0x0007bc40,0x0007c1d1,0x0007c764,
 0x0007ccf9,0x0007d290,0x0007d829,0x0007ddc4,0x0007e361,0x0007e900,0x0007eea1,0x0007f444,
 0x0007f9e9,0x0007ff90,0x00080539,0x00080ae4,0x00081091,0x00081640,0x00081bf1,0x000821a4,
 0x00082759,0x00082d10,0x000832c9,0x00083884,0x00083e41,0x00084400,0x000849c1,0x00084f84,
 0x00085549,0x00085b10,0x000860d9,0x000866a4,0x00086c71,0x00087240,0x00087811,0x00087de4,
 0x000883b9,0x00088990,0x00088f69,0x00089544,0x00089b21,0x0008a100,0x0008a6e1,0x0008acc4,
 0x0008b2a9,0x0008b890,0x0008be79,0x0008c464,0x0008ca51,0x0008d040,0x0008d631,0x0008dc24,
 0x0008e219,0x0008e810,0x0008ee09
};
const unsigned char TimgFilterSmootherLuma::Tblur::smoother_blur_codes[]=
{
 0x00,0x20,0x21,0x41,0x21,0x41,0x42,0x62,
 0x21,0x41,0x42,0x62,0x42,0x62,0x63,0x83,
 0x20,0x40,0x41,0x61,0x41,0x61,0x62,0x82,
 0x41,0x61,0x62,0x82,0x62,0x82,0x83,0xa3,
};

//==================================== TimgFilterSmootherLuma ====================================
TimgFilterSmootherLuma::TimgFilterSmootherLuma(IffdshowBase *Ideci,Tfilters *Iparent):TimgFilter(Ideci,Iparent)
{
 blur1=NULL;
}
void TimgFilterSmootherLuma::done(void)
{
 if (blur1) delete blur1;blur1=NULL;
}
void TimgFilterSmootherLuma::onSizeChange(void)
{
 done();
}
bool TimgFilterSmootherLuma::is(const TffPictBase &pict,const TfilterSettingsVideo *cfg0)
{
 const TblurSettings *cfg=(const TblurSettings*)cfg0;
 return TimgFilter::is(pict,cfg0) && cfg->isSmoothLuma && cfg->smoothStrengthLuma;
}
HRESULT TimgFilterSmootherLuma::process(TfilterQueue::iterator it,TffPict &pict,const TfilterSettingsVideo *cfg0)
{
 if (is(pict,cfg0))
  {
   const TblurSettings *cfg=(const TblurSettings*)cfg0;
   init(pict,cfg->full,cfg->half);
   const unsigned char *srcY;
   getCur(FF_CSPS_MASK_YUV_PLANAR,pict,cfg->full,&srcY,NULL,NULL,NULL);
   unsigned char *dstY;
   getNext(csp1,pict,cfg->full,&dstY,NULL,NULL,NULL);
   if (!blur1) blur1=new Tblur(dx1[0],dy1[0]);
   blur1->run(srcY,stride1[0],dstY,stride2[0],cfg->smoothStrengthLuma);
  }
 return parent->deliverSample(++it,pict);
}

//==================================== TimgFilterSmootherChroma ===================================
TimgFilterSmootherChroma::TimgFilterSmootherChroma(IffdshowBase *Ideci,Tfilters *Iparent):TimgFilterSmootherLuma(Ideci,Iparent)
{
 blur2=NULL;
}
void TimgFilterSmootherChroma::done(void)
{
 TimgFilterSmootherLuma::done();
 if (blur2) delete blur2;blur2=NULL;
}
bool TimgFilterSmootherChroma::is(const TffPictBase &pict,const TfilterSettingsVideo *cfg0)
{
 const TblurSettings *cfg=(const TblurSettings*)cfg0;
 return TimgFilter::is(pict,cfg0) && cfg->isSmoothChroma && cfg->smoothStrengthChroma;
}
HRESULT TimgFilterSmootherChroma::process(TfilterQueue::iterator it,TffPict &pict,const TfilterSettingsVideo *cfg0)
{
 if (is(pict,cfg0))
  {
   const TblurSettings *cfg=(const TblurSettings*)cfg0;
   init(pict,cfg->full,cfg->half);
   const unsigned char *srcU,*srcV;
   getCur(FF_CSPS_MASK_YUV_PLANAR,pict,cfg->full,NULL,&srcU,&srcV,NULL);
   unsigned char *dstU,*dstV;
   getNext(csp1,pict,cfg->full,NULL,&dstU,&dstV,NULL);
   if (!blur1) blur1=new Tblur(dx1[1],dy1[1]);
   blur1->run(srcU,stride1[1],dstU,stride2[1],cfg->smoothStrengthChroma);
   if (!blur2) blur2=new Tblur(dx1[2],dy1[2]);
   blur2->run(srcV,stride1[2],dstV,stride2[2],cfg->smoothStrengthChroma);
  }
 return parent->deliverSample(++it,pict);
}
