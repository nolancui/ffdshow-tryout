/*
 * copyright (C) 2003 the ffmpeg project
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef SVQ1_VLC_H
#define SVQ1_VLC_H

/* values in this table range from 0..3; adjust retrieved value by +0 */
static const uint8_t svq1_block_type_vlc[4][2] = {
 /* { code, length } */
    { 0x1, 1 },  { 0x1, 2 },  { 0x1, 3 },  { 0x0, 3 }

};

/* values in this table range from -1..6; adjust retrieved value by -1 */
static const uint8_t svq1_intra_multistage_vlc[6][8][2] = {
 /* { code, length } */
{
    { 0x1, 5 },  { 0x1, 1 },  { 0x3, 3 },  { 0x2, 3 },
    { 0x3, 4 },  { 0x2, 4 },  { 0x0, 5 },  { 0x1, 4 }
},{
    { 0x1, 4 },  { 0x3, 2 },  { 0x5, 3 },  { 0x4, 3 },
    { 0x3, 3 },  { 0x2, 3 },  { 0x0, 4 },  { 0x1, 3 }
},{
    { 0x1, 5 },  { 0x1, 1 },  { 0x3, 3 },  { 0x0, 5 },
    { 0x3, 4 },  { 0x2, 3 },  { 0x2, 4 },  { 0x1, 4 }
},{
    { 0x1, 6 },  { 0x1, 1 },  { 0x1, 2 },  { 0x0, 6 },
    { 0x3, 4 },  { 0x2, 4 },  { 0x1, 5 },  { 0x1, 4 }
},{
    { 0x1, 6 },  { 0x1, 1 },  { 0x1, 2 },  { 0x3, 5 },
    { 0x2, 5 },  { 0x0, 6 },  { 0x1, 5 },  { 0x1, 3 }
},{
    { 0x1, 7 },  { 0x1, 1 },  { 0x1, 2 },  { 0x1, 3 },
    { 0x1, 4 },  { 0x1, 6 },  { 0x0, 7 },  { 0x1, 5 }
}
};

/* values in this table range from -1..6; adjust retrieved value by -1 */
static const uint8_t svq1_inter_multistage_vlc[6][8][2] = {
 /* { code, length } */
{
    { 0x3, 2 },  { 0x5, 3 },  { 0x4, 3 },  { 0x3, 3 },
    { 0x2, 3 },  { 0x1, 3 },  { 0x1, 4 },  { 0x0, 4 }
},{
    { 0x3, 2 },  { 0x5, 3 },  { 0x4, 3 },  { 0x3, 3 },
    { 0x2, 3 },  { 0x1, 3 },  { 0x1, 4 },  { 0x0, 4 }
},{
    { 0x1, 1 },  { 0x3, 3 },  { 0x2, 3 },  { 0x3, 4 },
    { 0x2, 4 },  { 0x1, 4 },  { 0x1, 5 },  { 0x0, 5 }
},{
    { 0x1, 1 },  { 0x3, 3 },  { 0x2, 3 },  { 0x3, 4 },
    { 0x2, 4 },  { 0x1, 4 },  { 0x1, 5 },  { 0x0, 5 }
},{
    { 0x1, 1 },  { 0x3, 3 },  { 0x2, 3 },  { 0x3, 4 },
    { 0x2, 4 },  { 0x1, 4 },  { 0x1, 5 },  { 0x0, 5 }
},{
    { 0x1, 1 },  { 0x1, 2 },  { 0x1, 3 },  { 0x3, 5 },
    { 0x2, 5 },  { 0x1, 5 },  { 0x1, 6 },  { 0x0, 6 }
}
};

/* values in this table range from 0..255; adjust retrieved value by +0 */
static const uint16_t svq1_intra_mean_vlc[256][2] = {
 /* { code, length } */
    { 0x37, 6 },  { 0x56, 7 },  { 0x1, 17 },  { 0x1, 20 },
    { 0x2, 20 },  { 0x3, 20 },  { 0x0, 20 },  { 0x4, 20 },
    { 0x5, 20 },  { 0x3, 19 },  { 0x15, 11 },  { 0x42, 9 },
    { 0x14, 11 },  { 0x3, 14 },  { 0x2, 14 },  { 0x1, 15 },
    { 0x1, 16 },  { 0x1, 12 },  { 0x2B, 10 },  { 0x18, 11 },
    { 0xC, 11 },  { 0x41, 9 },  { 0x78, 8 },  { 0x6C, 8 },
    { 0x55, 7 },  { 0xF, 4 },  { 0xE, 4 },  { 0x34, 6 },
    { 0x51, 7 },  { 0x72, 8 },  { 0x6E, 8 },  { 0x40, 9 },
    { 0x3F, 9 },  { 0x3E, 9 },  { 0x3D, 9 },  { 0x3C, 9 },
    { 0x3B, 9 },  { 0x3A, 9 },  { 0x39, 9 },  { 0x38, 9 },
    { 0x37, 9 },  { 0x43, 9 },  { 0x46, 9 },  { 0x47, 9 },
    { 0x45, 9 },  { 0x44, 9 },  { 0x49, 9 },  { 0x48, 9 },
    { 0x4A, 8 },  { 0x79, 8 },  { 0x76, 8 },  { 0x77, 8 },
    { 0x71, 8 },  { 0x75, 8 },  { 0x74, 8 },  { 0x73, 8 },
    { 0x6A, 8 },  { 0x55, 8 },  { 0x70, 8 },  { 0x6F, 8 },
    { 0x52, 8 },  { 0x6D, 8 },  { 0x4C, 8 },  { 0x6B, 8 },
    { 0x40, 7 },  { 0x69, 8 },  { 0x68, 8 },  { 0x67, 8 },
    { 0x66, 8 },  { 0x65, 8 },  { 0x64, 8 },  { 0x63, 8 },
    { 0x62, 8 },  { 0x61, 8 },  { 0x60, 8 },  { 0x5F, 8 },
    { 0x5E, 8 },  { 0x5D, 8 },  { 0x5C, 8 },  { 0x5B, 8 },
    { 0x5A, 8 },  { 0x59, 8 },  { 0x58, 8 },  { 0x57, 8 },
    { 0x56, 8 },  { 0x3D, 7 },  { 0x54, 8 },  { 0x53, 8 },
    { 0x3F, 7 },  { 0x51, 8 },  { 0x50, 8 },  { 0x4F, 8 },
    { 0x4E, 8 },  { 0x4D, 8 },  { 0x41, 7 },  { 0x4B, 8 },
    { 0x53, 7 },  { 0x3E, 7 },  { 0x48, 8 },  { 0x4F, 7 },
    { 0x52, 7 },  { 0x45, 8 },  { 0x50, 7 },  { 0x43, 8 },
    { 0x42, 8 },  { 0x41, 8 },  { 0x42, 7 },  { 0x43, 7 },
    { 0x3E, 8 },  { 0x44, 7 },  { 0x3C, 8 },  { 0x45, 7 },
    { 0x46, 7 },  { 0x47, 7 },  { 0x48, 7 },  { 0x49, 7 },
    { 0x4A, 7 },  { 0x4B, 7 },  { 0x4C, 7 },  { 0x4D, 7 },
    { 0x4E, 7 },  { 0x58, 7 },  { 0x59, 7 },  { 0x5A, 7 },
    { 0x5B, 7 },  { 0x5C, 7 },  { 0x5D, 7 },  { 0x44, 8 },
    { 0x49, 8 },  { 0x29, 8 },  { 0x3F, 8 },  { 0x3D, 8 },
    { 0x3B, 8 },  { 0x2C, 8 },  { 0x28, 8 },  { 0x25, 8 },
    { 0x26, 8 },  { 0x5E, 7 },  { 0x57, 7 },  { 0x54, 7 },
    { 0x5F, 7 },  { 0x62, 7 },  { 0x63, 7 },  { 0x64, 7 },
    { 0x61, 7 },  { 0x65, 7 },  { 0x67, 7 },  { 0x66, 7 },
    { 0x35, 6 },  { 0x36, 6 },  { 0x60, 7 },  { 0x39, 8 },
    { 0x3A, 8 },  { 0x38, 8 },  { 0x37, 8 },  { 0x36, 8 },
    { 0x35, 8 },  { 0x34, 8 },  { 0x33, 8 },  { 0x32, 8 },
    { 0x31, 8 },  { 0x30, 8 },  { 0x2D, 8 },  { 0x2B, 8 },
    { 0x2A, 8 },  { 0x27, 8 },  { 0x40, 8 },  { 0x46, 8 },
    { 0x47, 8 },  { 0x26, 9 },  { 0x25, 9 },  { 0x24, 9 },
    { 0x23, 9 },  { 0x22, 9 },  { 0x2E, 8 },  { 0x2F, 8 },
    { 0x1F, 9 },  { 0x36, 9 },  { 0x1D, 9 },  { 0x21, 9 },
    { 0x1B, 9 },  { 0x1C, 9 },  { 0x19, 9 },  { 0x1A, 9 },
    { 0x18, 9 },  { 0x17, 9 },  { 0x16, 9 },  { 0x1E, 9 },
    { 0x20, 9 },  { 0x27, 9 },  { 0x28, 9 },  { 0x29, 9 },
    { 0x2A, 9 },  { 0x2B, 9 },  { 0x2C, 9 },  { 0x2D, 9 },
    { 0x2E, 9 },  { 0x2F, 9 },  { 0x30, 9 },  { 0x35, 9 },
    { 0x31, 9 },  { 0x32, 9 },  { 0x33, 9 },  { 0x34, 9 },
    { 0x19, 10 },  { 0x2A, 10 },  { 0x17, 10 },  { 0x16, 10 },
    { 0x15, 10 },  { 0x28, 10 },  { 0x26, 10 },  { 0x25, 10 },
    { 0x22, 10 },  { 0x21, 10 },  { 0x18, 10 },  { 0x14, 10 },
    { 0x29, 10 },  { 0x12, 10 },  { 0xD, 10 },  { 0xE, 10 },
    { 0xF, 10 },  { 0x10, 10 },  { 0x11, 10 },  { 0x1A, 10 },
    { 0x1B, 10 },  { 0x1C, 10 },  { 0x1D, 10 },  { 0x1E, 10 },
    { 0x1F, 10 },  { 0x20, 10 },  { 0x13, 10 },  { 0x23, 10 },
    { 0x24, 10 },  { 0x9, 11 },  { 0x8, 11 },  { 0x7, 11 },
    { 0x27, 10 },  { 0x5, 11 },  { 0xB, 11 },  { 0x6, 11 },
    { 0x4, 11 },  { 0x3, 11 },  { 0x2, 11 },  { 0x1, 11 },
    { 0xA, 11 },  { 0x16, 11 },  { 0x19, 11 },  { 0x17, 11 },
    { 0xD, 11 },  { 0xE, 11 },  { 0xF, 11 },  { 0x10, 11 },
    { 0x11, 11 },  { 0x12, 11 },  { 0x13, 11 },  { 0x1, 14 }
};

/* values in this table range from -256..255; adjust retrieved value by -256 */
static const uint16_t svq1_inter_mean_vlc[512][2] = {
 /* { code, length } */
    { 0x5A, 22 },  { 0xD4, 22 },  { 0xD5, 22 },  { 0xD6, 22 },
    { 0xD7, 22 },  { 0xD8, 22 },  { 0xD9, 22 },  { 0xDA, 22 },
    { 0xDB, 22 },  { 0xDC, 22 },  { 0xDD, 22 },  { 0xDE, 22 },
    { 0xDF, 22 },  { 0xE0, 22 },  { 0xE1, 22 },  { 0xE2, 22 },
    { 0xE3, 22 },  { 0xE4, 22 },  { 0xE5, 22 },  { 0xE6, 22 },
    { 0xE8, 22 },  { 0xCB, 22 },  { 0xE9, 22 },  { 0xEA, 22 },
    { 0xE7, 22 },  { 0xEC, 22 },  { 0xED, 22 },  { 0xEE, 22 },
    { 0xEF, 22 },  { 0xF0, 22 },  { 0xF1, 22 },  { 0xF2, 22 },
    { 0xF3, 22 },  { 0xF4, 22 },  { 0xF5, 22 },  { 0xF6, 22 },
    { 0xF7, 22 },  { 0xF8, 22 },  { 0x102, 22 },  { 0xEB, 22 },
    { 0xF9, 22 },  { 0xFC, 22 },  { 0xFD, 22 },  { 0xFE, 22 },
    { 0x100, 22 },  { 0x5C, 22 },  { 0x60, 22 },  { 0x101, 22 },
    { 0x71, 22 },  { 0x104, 22 },  { 0x105, 22 },  { 0xFB, 22 },
    { 0xFF, 22 },  { 0x86, 21 },  { 0xFA, 22 },  { 0x7C, 22 },
    { 0x75, 22 },  { 0x103, 22 },  { 0x78, 22 },  { 0xD3, 22 },
    { 0x7B, 22 },  { 0x82, 22 },  { 0xD2, 22 },  { 0xD1, 22 },
    { 0xD0, 22 },  { 0xCF, 22 },  { 0xCE, 22 },  { 0xCD, 22 },
    { 0xCC, 22 },  { 0xC3, 22 },  { 0xCA, 22 },  { 0xC9, 22 },
    { 0xC8, 22 },  { 0xC7, 22 },  { 0xC6, 22 },  { 0xC5, 22 },
    { 0x8B, 22 },  { 0xC4, 22 },  { 0xC2, 22 },  { 0xC1, 22 },
    { 0xC0, 22 },  { 0xBF, 22 },  { 0xBE, 22 },  { 0xBD, 22 },
    { 0xBC, 22 },  { 0xBB, 22 },  { 0xBA, 22 },  { 0xB9, 22 },
    { 0x61, 22 },  { 0x84, 22 },  { 0x85, 22 },  { 0x86, 22 },
    { 0x87, 22 },  { 0x88, 22 },  { 0x89, 22 },  { 0x8A, 22 },
    { 0x8C, 22 },  { 0x8D, 22 },  { 0x8E, 22 },  { 0x8F, 22 },
    { 0x90, 22 },  { 0x91, 22 },  { 0x92, 22 },  { 0x93, 22 },
    { 0x94, 22 },  { 0x95, 22 },  { 0x96, 22 },  { 0x97, 22 },
    { 0x98, 22 },  { 0x99, 22 },  { 0x9A, 22 },  { 0x9B, 22 },
    { 0x9C, 22 },  { 0x9D, 22 },  { 0x9E, 22 },  { 0x9F, 22 },
    { 0xA0, 22 },  { 0xA1, 22 },  { 0xA2, 22 },  { 0xA3, 22 },
    { 0xA4, 22 },  { 0xA5, 22 },  { 0xA6, 22 },  { 0xA7, 22 },
    { 0xA8, 22 },  { 0xA9, 22 },  { 0xAA, 22 },  { 0xAB, 22 },
    { 0x7F, 22 },  { 0x8F, 21 },  { 0xAC, 22 },  { 0xAD, 22 },
    { 0xAE, 22 },  { 0xAF, 22 },  { 0xB0, 22 },  { 0xB1, 22 },
    { 0x53, 20 },  { 0x90, 21 },  { 0xB2, 22 },  { 0x91, 21 },
    { 0xB3, 22 },  { 0xB4, 22 },  { 0x54, 20 },  { 0xB5, 22 },
    { 0xB6, 22 },  { 0x8C, 21 },  { 0x34, 19 },  { 0x3D, 18 },
    { 0x55, 20 },  { 0xB7, 22 },  { 0xB8, 22 },  { 0x8B, 21 },
    { 0x56, 20 },  { 0x3D, 19 },  { 0x57, 20 },  { 0x58, 20 },
    { 0x40, 19 },  { 0x43, 19 },  { 0x47, 19 },  { 0x2A, 18 },
    { 0x2E, 19 },  { 0x2C, 18 },  { 0x46, 19 },  { 0x59, 20 },
    { 0x49, 19 },  { 0x2D, 19 },  { 0x38, 18 },  { 0x36, 18 },
    { 0x39, 18 },  { 0x45, 19 },  { 0x28, 18 },  { 0x30, 18 },
    { 0x35, 18 },  { 0x20, 17 },  { 0x44, 19 },  { 0x32, 18 },
    { 0x31, 18 },  { 0x1F, 17 },  { 0x2F, 18 },  { 0x2E, 18 },
    { 0x2D, 18 },  { 0x21, 17 },  { 0x22, 17 },  { 0x23, 17 },
    { 0x24, 17 },  { 0x27, 16 },  { 0x23, 16 },  { 0x20, 16 },
    { 0x1D, 16 },  { 0x25, 16 },  { 0x1E, 16 },  { 0x24, 16 },
    { 0x2A, 16 },  { 0x26, 16 },  { 0x21, 15 },  { 0x29, 16 },
    { 0x22, 15 },  { 0x23, 15 },  { 0x24, 15 },  { 0x1B, 15 },
    { 0x1A, 15 },  { 0x1D, 15 },  { 0x1F, 15 },  { 0x27, 15 },
    { 0x17, 14 },  { 0x18, 14 },  { 0x19, 14 },  { 0x1B, 14 },
    { 0x1C, 14 },  { 0x1E, 14 },  { 0x25, 14 },  { 0x20, 14 },
    { 0x21, 14 },  { 0x13, 13 },  { 0x14, 13 },  { 0x15, 13 },
    { 0x16, 13 },  { 0x17, 13 },  { 0x18, 13 },  { 0x19, 13 },
    { 0x1A, 13 },  { 0x18, 12 },  { 0x17, 12 },  { 0x15, 12 },
    { 0x14, 12 },  { 0x13, 12 },  { 0x12, 12 },  { 0xF, 11 },
    { 0x10, 11 },  { 0x12, 11 },  { 0x13, 11 },  { 0x1B, 11 },
    { 0x1A, 11 },  { 0xE, 10 },  { 0x13, 10 },  { 0xF, 10 },
    { 0x10, 10 },  { 0x11, 10 },  { 0x12, 10 },  { 0xD, 9 },
    { 0x14, 9 },  { 0x15, 9 },  { 0xC, 9 },  { 0x13, 9 },
    { 0xF, 8 },  { 0xE, 8 },  { 0x10, 8 },  { 0x11, 8 },
    { 0xC, 7 },  { 0x9, 7 },  { 0xA, 7 },  { 0x8, 6 },
    { 0x9, 6 },  { 0x9, 5 },  { 0x8, 5 },  { 0x5, 4 },
    { 0x1, 1 },  { 0x3, 3 },  { 0x7, 5 },  { 0x6, 5 },
    { 0xB, 6 },  { 0xA, 6 },  { 0xE, 7 },  { 0xF, 7 },
    { 0xB, 7 },  { 0xD, 7 },  { 0xB, 8 },  { 0xD, 8 },
    { 0xC, 8 },  { 0xF, 9 },  { 0x10, 9 },  { 0x11, 9 },
    { 0xE, 9 },  { 0x12, 9 },  { 0x17, 10 },  { 0x14, 10 },
    { 0x16, 10 },  { 0x15, 10 },  { 0x19, 11 },  { 0x18, 11 },
    { 0x17, 11 },  { 0x16, 11 },  { 0x15, 11 },  { 0x14, 11 },
    { 0x11, 11 },  { 0x19, 12 },  { 0x1A, 12 },  { 0x16, 12 },
    { 0x1D, 12 },  { 0x1B, 12 },  { 0x1C, 12 },  { 0x20, 13 },
    { 0x1C, 13 },  { 0x23, 13 },  { 0x22, 13 },  { 0x21, 13 },
    { 0x1F, 13 },  { 0x1E, 13 },  { 0x1B, 13 },  { 0x1D, 13 },
    { 0x24, 14 },  { 0x16, 14 },  { 0x1A, 14 },  { 0x22, 14 },
    { 0x1D, 14 },  { 0x1F, 14 },  { 0x15, 14 },  { 0x23, 14 },
    { 0x18, 15 },  { 0x20, 15 },  { 0x29, 15 },  { 0x28, 15 },
    { 0x26, 15 },  { 0x25, 15 },  { 0x19, 15 },  { 0x1C, 15 },
    { 0x1E, 15 },  { 0x17, 15 },  { 0x2C, 16 },  { 0x2B, 16 },
    { 0x1C, 16 },  { 0x21, 16 },  { 0x2D, 16 },  { 0x28, 16 },
    { 0x1F, 16 },  { 0x1B, 16 },  { 0x1A, 16 },  { 0x22, 16 },
    { 0x2D, 17 },  { 0x32, 17 },  { 0x2C, 17 },  { 0x27, 17 },
    { 0x31, 17 },  { 0x33, 17 },  { 0x2F, 17 },  { 0x2B, 17 },
    { 0x37, 18 },  { 0x2A, 17 },  { 0x2E, 17 },  { 0x30, 17 },
    { 0x29, 17 },  { 0x28, 17 },  { 0x26, 17 },  { 0x25, 17 },
    { 0x2F, 19 },  { 0x33, 18 },  { 0x34, 18 },  { 0x30, 19 },
    { 0x3A, 18 },  { 0x3B, 18 },  { 0x31, 19 },  { 0x3C, 18 },
    { 0x2B, 18 },  { 0x29, 18 },  { 0x48, 19 },  { 0x27, 18 },
    { 0x42, 19 },  { 0x41, 19 },  { 0x26, 18 },  { 0x52, 20 },
    { 0x51, 20 },  { 0x3F, 19 },  { 0x3E, 19 },  { 0x39, 19 },
    { 0x3C, 19 },  { 0x3B, 19 },  { 0x3A, 19 },  { 0x25, 18 },
    { 0x38, 19 },  { 0x50, 20 },  { 0x37, 19 },  { 0x36, 19 },
    { 0x87, 21 },  { 0x4F, 20 },  { 0x35, 19 },  { 0x4E, 20 },
    { 0x33, 19 },  { 0x32, 19 },  { 0x4D, 20 },  { 0x4C, 20 },
    { 0x83, 22 },  { 0x4B, 20 },  { 0x81, 22 },  { 0x80, 22 },
    { 0x8E, 21 },  { 0x7E, 22 },  { 0x7D, 22 },  { 0x84, 21 },
    { 0x8D, 21 },  { 0x7A, 22 },  { 0x79, 22 },  { 0x4A, 20 },
    { 0x77, 22 },  { 0x76, 22 },  { 0x89, 21 },  { 0x74, 22 },
    { 0x73, 22 },  { 0x72, 22 },  { 0x49, 20 },  { 0x70, 22 },
    { 0x6F, 22 },  { 0x6E, 22 },  { 0x6D, 22 },  { 0x6C, 22 },
    { 0x6B, 22 },  { 0x6A, 22 },  { 0x69, 22 },  { 0x68, 22 },
    { 0x67, 22 },  { 0x66, 22 },  { 0x65, 22 },  { 0x64, 22 },
    { 0x63, 22 },  { 0x62, 22 },  { 0x8A, 21 },  { 0x88, 21 },
    { 0x5F, 22 },  { 0x5E, 22 },  { 0x5D, 22 },  { 0x85, 21 },
    { 0x5B, 22 },  { 0x83, 21 },  { 0x59, 22 },  { 0x58, 22 },
    { 0x57, 22 },  { 0x56, 22 },  { 0x55, 22 },  { 0x54, 22 },
    { 0x53, 22 },  { 0x52, 22 },  { 0x51, 22 },  { 0x50, 22 },
    { 0x4F, 22 },  { 0x4E, 22 },  { 0x4D, 22 },  { 0x4C, 22 },
    { 0x4B, 22 },  { 0x4A, 22 },  { 0x49, 22 },  { 0x48, 22 },
    { 0x47, 22 },  { 0x46, 22 },  { 0x45, 22 },  { 0x44, 22 },
    { 0x43, 22 },  { 0x42, 22 },  { 0x41, 22 },  { 0x40, 22 },
    { 0x3F, 22 },  { 0x3E, 22 },  { 0x3D, 22 },  { 0x3C, 22 },
    { 0x3B, 22 },  { 0x3A, 22 },  { 0x39, 22 },  { 0x38, 22 },
    { 0x37, 22 },  { 0x36, 22 },  { 0x35, 22 },  { 0x34, 22 },
    { 0x33, 22 },  { 0x32, 22 },  { 0x31, 22 },  { 0x30, 22 },
    { 0x2F, 22 },  { 0x2E, 22 },  { 0x2D, 22 },  { 0x2C, 22 },
    { 0x2B, 22 },  { 0x2A, 22 },  { 0x29, 22 },  { 0x28, 22 },
    { 0x27, 22 },  { 0x26, 22 },  { 0x25, 22 },  { 0x24, 22 },
    { 0x23, 22 },  { 0x22, 22 },  { 0x21, 22 },  { 0x20, 22 },
    { 0x1F, 22 },  { 0x1E, 22 },  { 0x1D, 22 },  { 0x1C, 22 },
    { 0x1B, 22 },  { 0x1A, 22 },  { 0x19, 22 },  { 0x18, 22 },
    { 0x17, 22 },  { 0x16, 22 },  { 0x15, 22 },  { 0x14, 22 },
    { 0x13, 22 },  { 0x12, 22 },  { 0x11, 22 },  { 0x10, 22 },
    { 0xF, 22 },  { 0xE, 22 },  { 0xD, 22 },  { 0xC, 22 },
    { 0xB, 22 },  { 0xA, 22 },  { 0x9, 22 },  { 0x8, 22 },
    { 0x7, 22 },  { 0x6, 22 },  { 0x5, 22 },  { 0x4, 22 },
    { 0x3, 22 },  { 0x2, 22 },  { 0x1, 22 },  { 0x0, 22 }
};

#endif
