// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_ARCH_X86_MM_MMU_H
#define CORNOS_ARCH_X86_MM_MMU_H

#define TYPE_INT_GATE32 0xE // type of 32-bit Interrupt Gate

/* Gate descriptors for interrupts and traps */
struct gate_desc_t {
	unsigned int offset_15_0 : 16; // low 16 bits of offset in segment
	unsigned int seg_sel : 16; // segment selector
	unsigned int args : 5; // # args, 0 for interrupt/trap gates
	unsigned int rsv : 3; // reserved (should be zero)
	unsigned int type : 4; // type(STS_{TG,IG32,TG32})
	unsigned int s : 1; // must be 0 (system)
	unsigned int dpl : 2; // descriptor(meaning new) privilege level
	unsigned int p : 1; // Present
	unsigned int offset_31_16 : 16; // high bits of offset in segment
};

/**
 * sel: Code segment selector
 * off: Offset in code segment
 * dpl: Descriptor Privilege Level
 * @param gate_desc
 */
static inline void set_gate(struct gate_desc_t *gate_desc,
				unsigned int gate_type,
				unsigned int seg_sel, unsigned int offset,
				unsigned int dpl)
{
	gate_desc->offset_15_0 = offset & 0xffff;
	gate_desc->seg_sel = seg_sel;
	gate_desc->args = 0;
	gate_desc->rsv = 0;
	gate_desc->type = gate_type;
	gate_desc->s = 0;
	gate_desc->dpl = dpl;
	gate_desc->p = 1;
	gate_desc->offset_31_16 = offset >> 16;
}

#endif // CORNOS_ARCH_X86_MM_MMU_H
