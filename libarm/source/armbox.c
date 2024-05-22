#include <stdint.h>

/* **** */

typedef uint32_t (*armbox_fn)(const uint32_t rn, const uint32_t sop);

static uint32_t __armbox_wb(uint32_t *const p2rd, const uint32_t rn, const uint32_t sop, armbox_fn fn)
{
	const uint32_t rd = fn(rn, sop);
	if(p2rd) *p2rd = rd;
	return(rd);
}

/* **** */

static uint32_t _add(const uint32_t rn, const uint32_t sop)
{ return(rn + sop); }

static uint32_t _and(const uint32_t rn, const uint32_t sop)
{ return(rn & sop); }

static uint32_t _asr(const int32_t rm, const uint8_t rs)
{ return(rm >> rs); }

static uint32_t _bic(const uint32_t rn, const uint32_t sop)
{ return(_and(rn, ~sop)); }

static uint32_t _cmn(const uint32_t rn, const uint32_t sop)
{ return(_add(rn, sop)); }

static uint32_t _cmp(const uint32_t rn, const uint32_t sop)
{ return(rn - sop); }

static uint32_t _eor(const uint32_t rn, const uint32_t sop)
{ return(rn ^ sop); }

static uint32_t _lsl(const uint32_t rm, const uint8_t rs)
{ return(rm << rs); }

static uint32_t _lsr(const uint32_t rm, const uint8_t rs)
{ return(rm >> rs); }

static uint32_t _mov(const uint32_t rn, const uint32_t sop)
{ return(sop); }

static uint32_t _mvn(const uint32_t rn, const uint32_t sop)
{ return(~sop); }

static uint32_t _orr(const uint32_t rn, const uint32_t sop)
{ return(rn | sop); }

static uint32_t _ror(const uint32_t rm, const uint8_t rs)
{ return(_lsl(rm, -rs & 31) | _lsr(rm, rs)); }

static uint32_t _rsb(const uint32_t rn, const uint32_t sop)
{ return(_cmp(sop, rn)); }

static uint32_t _sub(const uint32_t rn, const uint32_t sop)
{ return(_cmp(rn, sop)); }

/* **** */

#define _DEFUN(_op, _shift) \
	uint32_t armbox##_op##_shift(uint32_t *const p2rd, const uint32_t rn, const uint32_t rm, const uint32_t rs) \
	{ return(_op(rn, _shift(rm, rs))); }

#define _DEFUN_WB(_op, _shift) \
	uint32_t armbox##_op##_shift##_wb(uint32_t *const p2rd, const uint32_t rn, const uint32_t rm, const uint32_t rs) \
	{ return(__armbox_wb(p2rd, rn, _shift(rm, rs), _op)); }

#define DEFUN(_op) \
	_DEFUN(_op, _asr) _DEFUN(_op, _lsl) _DEFUN(_op, _lsr) _DEFUN(_op, _ror)

#define DEFUN_WB(_op) \
	_DEFUN_WB(_op, _asr) _DEFUN_WB(_op, _lsl) _DEFUN_WB(_op, _lsr) _DEFUN_WB(_op, _ror)

DEFUN(_add)
DEFUN_WB(_add)
DEFUN(_and)
DEFUN_WB(_and)
DEFUN(_bic)
DEFUN_WB(_bic)
DEFUN(_eor)
DEFUN_WB(_eor)
DEFUN(_mov)
DEFUN_WB(_mov)
DEFUN(_mvn)
DEFUN_WB(_mvn)
DEFUN(_cmn)
DEFUN(_cmp)
DEFUN(_rsb)
DEFUN_WB(_rsb)
DEFUN(_sub)
DEFUN_WB(_sub)
