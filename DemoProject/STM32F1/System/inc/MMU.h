#ifndef __INCLUDE_MMU_H__
#define __INCLUDE_MMU_H__
#include <stdint.h>
#include <stddef.h>

#define MMU_ALIGN_BYTE			(2)
#if		MMU_ALIGN_BYTE == 1
	#define MMU_OPERAND_TYPE	uint8_t
#elif	MMU_ALIGN_BYTE == 2
	#define MMU_OPERAND_TYPE	uint16_t
#elif	MMU_ALIGN_BYTE == 4
	#define MMU_OPERAND_TYPE	uint32_t
#else
	#error "Invalid align."
#endif


#define MMU_BLOCK_SIZE			(32)	// Memory block size
#define MMU_BLOCK_QUANTITY		(320)	// Memory block quantity
#define	MMU_HEAP_SIZE			(MMU_BLOCK_QUANTITY*MMU_BLOCK_SIZE) // Heap size 32*320=10240(10K)
#if		MMU_BLOCK_SIZE%MMU_ALIGN_BYTE > 0
	#error "Block size must be an integer multiple of alignment."
#endif


#define MMU_ALLOC_TABLE_SIZE	MMU_BLOCK_QUANTITY
#define MMU_INVALID_BLOCK_INDEX	((size_t)-1)
#define MMU_INVALID_HEAP_INDEX	((size_t)-1)

#ifndef NULL
#define NULL 0
#endif

typedef enum
{
	MMU_STATE_UNINITIALIZED	= 0,
	MMU_STATE_INITIALIZED,
}MMU_State;

typedef enum
{
	MMU_BLOCK_STATE_UNUSE	= 0x00,
	MMU_BLOCK_STATE_START	= 0x01,
	MMU_BLOCK_STATE_INUSE	= 0x02,
}MMU_BlockState;

typedef struct _t_mmu_action_
{
	void			(*MMU_Initialize)(void);
	uint16_t		(*MMU_Utilization)(void);

}MMU_Action;

typedef struct _t_mmu_data_
{
	uint8_t			HeapMemory[MMU_HEAP_SIZE];
	uint8_t			BlockState[MMU_BLOCK_QUANTITY];
}MMU_Data;

typedef struct _t_mmu_const_pointer_
{
	const void*		HeapHead;
	const void*		HeapEnd;
}MMU_ConstPointer;

typedef struct _t_mmu_object_
{
	MMU_State			State;
	MMU_Data			Heap;
	MMU_ConstPointer	Pointer;
}MMU_Object;

void*		mmu_malloc(size_t uiSize);
void*		mmu_realloc(void* pPointer, size_t uiSize);
void		mmu_free(void*	pPointer);
void*		mmu_memcpy(void* dest, const void* src, size_t length);
void*		mmu_memcpy2(void* pDest, const void* pSrc, size_t uiSize);
void		mmu_memset(void *s, uint16_t c, size_t length);
uint16_t	mmu_Usage(uint16_t uiFullNumber);

#endif
