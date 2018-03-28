/*
** x86_segmentation_types.h
** X86 Segmentation Types
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __ARCH_X86_SEGMENTATION_TYPES_H__
# define __ARCH_X86_SEGMENTATION_TYPES_H__


/* GDT macro & types definition
 */
# define CODE_READ_EXEC		0xA
# define DATA_READ_WRITE	0x2


/* Temporary macro ...
 * We just have ring0 segment and no ldt
 */
# define BUILD_SEGMENT_REG_VALUE(index) BUILD_SEGMENT_REG_VALUE_EXT(0, 0, index)
# define BUILD_SEGMENT_REG_VALUE_EXT(ring, in_ldt, index)  ((( (ring) & 0x3) << 0)	\
							|  (((in_ldt)? 1:0) << 2) |	\
							   ((index) << 3) )

enum	_gdte_prop_g {
  PAGE_GRANULARITY = 1
};

enum	_gdte_prop_db {
  SEGMENT_16BITS = 0,
  SEGMENT_32BITS
};

enum	_gdte_prop_s {
  SYSTEM_SEGMENT = 0,
  CODE_OR_DATA_SEGMENT
};

enum	_gdte_prop_dpl {
  RING_0 = 0,
  RING_1,
  RING_2,
  RING_3,
};

enum	_gde_prop_present {
  SEGMENT_NOT_PRESENT = 0,
  SEGMENT_IS_PRESENT
};

union	_x86_gdte /* GDT descriptor */
{
  struct
  {
    unsigned	low;
    unsigned	high;
  } value;
  struct
  {
    unsigned	limit_1	: 16; /* Bits 15..00 */
    unsigned	base_1	: 16; /* Bits 15..00 */
    unsigned	base_2	:  8; /* Bits 23..16 */
    unsigned	type	:  4; /* Segment type */
    unsigned	s	:  1; /* Type (0=system, 1=code/data) */
    unsigned	dpl	:  2; /* Descriptor privilege level */
    unsigned	p	:  1; /* Segment present */
    unsigned	limit_2	:  4; /* Bits 19..16 */
    unsigned	avl	:  1; /* Available to programmer */
    unsigned	reserv	:  1; /* Reserved */
    unsigned	db	:  1; /* 0=16-bit, 1=32-bit */
    unsigned	g	:  1; /* Granularity (1=4KB) */
    unsigned	base_3	:  8; /* Bits 31..24 */
  } prop;
} __attribute__ ((packed, aligned(8)));
typedef union _x86_gdte	x86_gdte_t;

struct _x86_gdtr
{
  unsigned short	size;
  unsigned long		addr;
} __attribute__((packed, aligned(4)));
typedef struct _x86_gdtr	x86_gdtr_t;


#endif /* __ARCH_X86_SEGMENTATION_TYPES_H__ */
