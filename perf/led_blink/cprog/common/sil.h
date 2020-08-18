/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *
 *  The above copyright holders grant permission gratis to use,
 *  duplicate, modify, or redistribute (hereafter called use) this
 *  software (including the one made by modifying this software),
 *  provided that the following four conditions (1) through (4) are
 *  satisfied.
 * 
 *  (1) When this software is used in the form of source code, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be retained in the source code without modification.
 * 
 *  (2) When this software is redistributed in the forms usable for the
 *      development of other software, such as in library form, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be shown without modification in the document provided
 *      with the redistributed software, such as the user manual.
 *
 * (3) When this software is redistributed in the forms unusable for the
 *     development of other software, such as the case when the software
 *     is embedded in a piece of equipment, either of the following two
 *     conditions must be satisfied:
 *
 * (a) The above copyright notice, this use conditions, and the
 *     disclaimer shown below must be shown without modification in
 *     the document provided with the redistributed software, such as
 *     the user manual.
 *
 * (b) How the software is to be redistributed must be reported to the
 *     TOPPERS Project according to the procedure described
 *     separately.
 *
 * (4) The above copyright holders and the TOPPERS Project are exempt
 *     from responsibility for any type of damage directly or indirectly
 *     caused from the use of this software and are indemnified by any
 *     users or end users of this software from any and all causes of
 *     action whatsoever.
 *
 *  THIS SOFTWARE IS PROVIDED "AS IS." THE ABOVE COPYRIGHT HOLDERS AND
 *  THE TOPPERS PROJECT DISCLAIM ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, ITS APPLICABILITY TO A PARTICULAR
 *  PURPOSE. IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS AND THE
 *  TOPPERS PROJECT BE LIABLE FOR ANY TYPE OF DAMAGE DIRECTLY OR
 *  INDIRECTLY CAUSED FROM THE USE OF THIS SOFTWARE.
 * 
 *  @(#) $Id: sil.h,v 1.7 2003/07/01 13:32:20 hiro Exp $
 */

#ifndef _SIL_H_
#define _SIL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  システムやプロセッサに依存する定義
 */
#include <sys_defs.h>

/*
 *  エンディアン定数の定義
 */
#define	SIL_ENDIAN_LITTLE	0	/* リトルエンディアン */
#define	SIL_ENDIAN_BIG		1	/* ビッグエンディアン */

#ifndef _MACRO_ONLY
/*
 *  Type Definitions
 */
#ifdef _int8_
typedef	_int8_				VB;		/* 型が定まらない8ビットの値 */
#endif /* _int8_ */
#ifdef _int16_
typedef	unsigned _int16_ 	UH;		/* 符号無し16ビット整数 */
typedef	_int16_				VH;		/* 型が定まらない16ビットの値 */
#endif /* _int16_ */
typedef	unsigned _int32_	UW;		/* 符号無し32ビット整数 */
typedef	_int32_				VW;		/* 型が定まらない32ビットの値 */
typedef	void				*VP;	/* 型が定まらないものへのポインタ */
typedef unsigned int		UINT;	/* 自然なサイズの符号無し整数 */

/*
 *  割込みロック状態の制御
 */
#ifndef SIL_PRE_LOC
#define	SIL_PRE_LOC	BOOL _sil_loc_ = sns_loc()
#define	SIL_LOC_INT()	((void)(!(_sil_loc_) \
				&& (sns_ctx() ? iloc_cpu() : loc_cpu())))
#define	SIL_UNL_INT()	((void)(!(_sil_loc_) \
				&& (sns_ctx() ? iunl_cpu() : unl_cpu())))
#endif /* SIL_PRE_LOC */

/*
 *  微少時間待ち
 */
extern void	sil_dly_nse(UINT dlytim);

/*
 *  エンディアンの反転
 */
#ifndef SIL_REV_ENDIAN_H
#define	SIL_REV_ENDIAN_H(data) \
	((VH)((((UH)(data) & 0xff) << 8) | (((UH)(data) >> 8) & 0xff)))
#endif /* SIL_REV_ENDIAN_H */

#ifndef SIL_REV_ENDIAN_W
#define	SIL_REV_ENDIAN_W(data) \
	((VW)((((UW)(data) & 0xff) << 24) | (((UW)(data) & 0xff00) << 8) \
		| (((UW)(data)>> 8) & 0xff00) | (((UW)(data) >> 24) & 0xff)))
#endif /* SIL_REV_ENDIAN_H */

/*
 *  メモリ空間アクセス関数
 */
#ifndef OMIT_SIL_ACCESS

/*
 *  8ビット単位の読出し／書込み
 */
#ifdef _int8_

Inline VB
sil_reb_mem(VP mem)
{
	return(*((volatile VB *) mem));
}

Inline void
sil_wrb_mem(VP mem, VB data)
{
	*((volatile VB *) mem) = data;
}

#endif /* _int8_ */

/*
 *  16ビット単位の読出し／書込み
 */
#ifdef _int16_

Inline VH
sil_reh_mem(VP mem)
{
	return(*((volatile VH *) mem));
}

Inline void
sil_wrh_mem(VP mem, VH data)
{
	*((volatile VH *) mem) = data;
}

#if SIL_ENDIAN == SIL_ENDIAN_BIG	/* ビッグエンディアンプロセッサ */

#define	sil_reh_bem(mem)	sil_reh_mem(mem)
#define	sil_wrh_bem(mem, data)	sil_wrh_mem(mem, data)

#ifndef OMIT_SIL_REH_LEM

Inline VH
sil_reh_lem(VP mem)
{
	VH	data;

	data = *((volatile VH *) mem);
	return(SIL_REV_ENDIAN_H(data));
}

#endif /* OMIT_SIL_REH_LEM */
#ifndef OMIT_SIL_WRH_LEM

Inline void
sil_wrh_lem(VP mem, VH data)
{
	*((volatile VH *) mem) = SIL_REV_ENDIAN_H(data);
}

#endif /* OMIT_SIL_WRH_LEM */
#else /* SIL_ENDIAN == SIL_ENDIAN_BIG *//* リトルエンディアンプロセッサ */

#define	sil_reh_lem(mem)	sil_reh_mem(mem)
#define	sil_wrh_lem(mem, data)	sil_wrh_mem(mem, data)

#ifndef OMIT_SIL_REH_BEM

Inline VH
sil_reh_bem(VP mem)
{
	VH	data;

	data = *((volatile VH *) mem);
	return(SIL_REV_ENDIAN_H(data));
}

#endif /* OMIT_SIL_REH_BEM */
#ifndef OMIT_SIL_WRH_BEM

Inline void
sil_wrh_bem(VP mem, VH data)
{
	*((volatile VH *) mem) = SIL_REV_ENDIAN_H(data);
}

#endif /* OMIT_SIL_WRH_BEM */
#endif /* SIL_ENDIAN == SIL_ENDIAN_BIG */
#endif /* _int16_ */

/*
 *  32ビット単位の読出し／書込み
 */

Inline VW
sil_rew_mem(VP mem)
{
	return(*((volatile VW *) mem));
}

Inline void
sil_wrw_mem(VP mem, VW data)
{
	*((volatile VW *) mem) = data;
}

#if SIL_ENDIAN == SIL_ENDIAN_BIG	/* ビッグエンディアンプロセッサ */

#define	sil_rew_bem(mem)	sil_rew_mem(mem)
#define	sil_wrw_bem(mem, data)	sil_wrw_mem(mem, data)

#ifndef OMIT_SIL_REW_LEM

Inline VW
sil_rew_lem(VP mem)
{
	VW	data;

	data = *((volatile VW *) mem);
	return(SIL_REV_ENDIAN_W(data));
}

#endif /* OMIT_SIL_REW_LEM */
#ifndef OMIT_SIL_WRW_LEM

Inline void
sil_wrw_lem(VP mem, VW data)
{
	*((volatile VW *) mem) = SIL_REV_ENDIAN_W(data);
}

#endif /* OMIT_SIL_WRW_LEM */
#else /* SIL_ENDIAN == SIL_ENDIAN_BIG *//* リトルエンディアンプロセッサ */

#define	sil_rew_lem(mem)	sil_rew_mem(mem)
#define	sil_wrw_lem(mem, data)	sil_wrw_mem(mem, data)

#ifndef OMIT_SIL_REW_BEM

Inline VW
sil_rew_bem(VP mem)
{
	VW	data;

	data = *((volatile VW *) mem);
	return(SIL_REV_ENDIAN_W(data));
}

#endif /* OMIT_SIL_REW_BEM */
#ifndef OMIT_SIL_WRW_BEM

Inline void
sil_wrw_bem(VP mem, VW data)
{
	*((volatile VW *) mem) = SIL_REV_ENDIAN_W(data);
}

#endif /* OMIT_SIL_WRW_BEM */
#endif /* SIL_ENDIAN == SIL_ENDIAN_BIG */
#endif /* OMIT_SIL_ACCESS */

#endif /* _MACRO_ONLY */

#ifdef __cplusplus
}
#endif

#endif /* _SIL_H_ */
