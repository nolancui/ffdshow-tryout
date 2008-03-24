/*****************************************************************************
 * cabac.h: h264 encoder library
 *****************************************************************************
 * Copyright (C) 2003 Laurent Aimar
 * $Id: cabac.h,v 1.1 2004/06/03 19:27:06 fenrir Exp $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************/

#ifndef _CABAC_H
#define _CABAC_H 1

typedef struct
{
    /* context */
    DECLARE_ALIGNED_16( uint8_t state[460] );

    int f8_bits_encoded; // only if using x264_cabac_size_decision()

    /* state */
    int i_low;
    int i_range;

    /* bit stream */
    int i_queue;
    int i_bytes_outstanding;

    uint8_t *p_start;
    uint8_t *p;
    uint8_t *p_end;

} x264_cabac_t;

extern const uint8_t x264_cabac_transition[128][2];
extern const uint16_t x264_cabac_entropy[128][2];

/* init the contexts given i_slice_type, the quantif and the model */
void x264_cabac_context_init( x264_cabac_t *cb, int i_slice_type, int i_qp, int i_model );

/* encoder only: */
void x264_cabac_encode_init ( x264_cabac_t *cb, uint8_t *p_data, uint8_t *p_end );
void x264_cabac_encode_decision( x264_cabac_t *cb, int i_ctx, int b );
void x264_cabac_encode_bypass( x264_cabac_t *cb, int b );
void x264_cabac_encode_ue_bypass( x264_cabac_t *cb, int exp_bits, int val );
void x264_cabac_encode_terminal( x264_cabac_t *cb );
void x264_cabac_encode_flush( x264_t *h, x264_cabac_t *cb );

static inline int x264_cabac_pos( x264_cabac_t *cb )
{
    return (cb->p - cb->p_start + cb->i_bytes_outstanding) * 8 + cb->i_queue;
}

/* internal only. these don't write the bitstream, just calculate bit cost: */

static inline void x264_cabac_size_decision( x264_cabac_t *cb, long i_ctx, long b )
{
    int i_state = cb->state[i_ctx];
    cb->state[i_ctx] = x264_cabac_transition[i_state][b];
    cb->f8_bits_encoded += x264_cabac_entropy[i_state][b];
}

static inline int x264_cabac_size_decision2( uint8_t *state, long b )
{
    int i_state = *state;
    *state = x264_cabac_transition[i_state][b];
    return x264_cabac_entropy[i_state][b];
}

static inline int x264_cabac_size_decision_noup( uint8_t *state, long b )
{
    return x264_cabac_entropy[*state][b];
}

#endif
