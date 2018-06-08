/*************************************************************************/
/** CopyRight.															**/
/** FileName: MMU.c														**/
/** Author: XuYulin														**/
/** Version: 1.0.1.1													**/
/** Description: Declare the memory space as heap and manage it.		**/
/** Function List:														**/
/**	1 . mmu_Initialize()		Initialize MMU controler variable.		**/
/**	2 . mmu_AllocBlocks()		Allocate memory of heap blocks.			**/
/**	3 . mmu_AllocHeap()			Allocate memory in heap.				**/
/**	4 . mmu_FreeBlocks()		Release heap blocks.					**/
/**	5 . mmu_malloc()			Allocate a memory area in heap.			**/
/**	6 . mmu_realloc()			Resize a allocated memory area.			**/
/**	7 . mmu_free()				Release a allocated memory area.		**/
/**	8 . mmu_memcpy()			Copy a memory block to a new area.		**/
/**	9 . mmu_memcpy2()			Copy a memory block to a new area, no 	**/
/**								memory contamination occurs if memory	**/
/**								area crossed.							**/
/** 10. mmu_memset()			Fill a memory block by a byte.			**/
/**	11. mmu_Usage()				Get current memory usage.				**/
/** History:															**/
/**		2017/2/15	1.0.0.0		Created.								**/
/**		2017/2/16	1.0.0.1		Copy and set use memory alignment mode.	**/
/**		2017/2/17	1.0.1.1		Add mmu_realloc() function.				**/
/**																		**/
/*************************************************************************/

#include "MMU.h"
#include <stdint.h>

#define		BLOCK_SIZE(HEAP)		(HEAP>MMU_BLOCK_SIZE)?((HEAP-1)/MMU_BLOCK_SIZE+1):(1)
#define		HEAP_SIZE(BLOCK)		(BLOCK*MMU_BLOCK_SIZE)

static void		mmu_Initialize(void);
static size_t	mmu_AllocHeap(size_t uiSize);
static size_t	mmu_AllocBlocks(size_t uiBlockNumber);
static size_t	mmu_FreeBlocks(size_t uiStartIndex);


MMU_Object MMU_HEAP_OBJECT	= {
	/* State			*/MMU_STATE_UNINITIALIZED,
	/* Heap				*/{{0x00}, {0x00}},
	/* Const pointer	*/{0x00},
									  };



/*************************************************************************/
/** Function Name:	mmu_Initialize										**/
/** Purpose:		MMU data initialize.								**/
/** Resources:		Declared heap space memory and control data.		**/
/** Params:			None.												**/
/** Return:			None.												**/
/** Notice:		None.												**/
/*************************************************************************/
void mmu_Initialize(void)
{
	// Initialize heap memory.
	mmu_memset(MMU_HEAP_OBJECT.Heap.BlockState, MMU_BLOCK_STATE_UNUSE, MMU_BLOCK_QUANTITY);
    mmu_memset(MMU_HEAP_OBJECT.Heap.HeapMemory, 0x00, MMU_HEAP_SIZE);
    // Initialize const heap pointer.
	MMU_HEAP_OBJECT.Pointer.HeapHead = (void*)MMU_HEAP_OBJECT.Heap.HeapMemory;
	MMU_HEAP_OBJECT.Pointer.HeapEnd = MMU_HEAP_OBJECT.Heap.HeapMemory + MMU_HEAP_SIZE;

	// Set heap structure state.
	MMU_HEAP_OBJECT.State = MMU_STATE_INITIALIZED;
}

/*************************************************************************/
/** Function Name:	mmu_Usage.											**/
/** Purpose:		Get heap memory space usage.						**/
/** Resources:		Declared heap space memory.							**/
/** Params:																**/
/**	@uiFullNumber:		Base number of full usage.						**/
/** Return:			None.												**/
/** Notice:		None.												**/
/*************************************************************************/
uint16_t mmu_Usage(uint16_t uiFullNumber)
{
	uint16_t	uiUtilization;
	size_t		uiInuseBlocks;
	size_t		uiBlockIndex;

	uiUtilization = 0;
	uiInuseBlocks = 0;
	uiBlockIndex = 0;
	while(uiBlockIndex < MMU_BLOCK_QUANTITY)
	{
		if(MMU_HEAP_OBJECT.Heap.BlockState[uiBlockIndex] != MMU_BLOCK_STATE_UNUSE)
		{
			uiInuseBlocks ++;
		}
		uiBlockIndex ++;
	}

	uiUtilization = uiInuseBlocks * uiFullNumber / MMU_BLOCK_QUANTITY;

	return uiUtilization;
}

/*************************************************************************/
/** Function Name:	mmu_AllocBlocks.									**/
/** Purpose:		Alloc heap blocks.									**/
/** Resources:		Declared heap space memory.							**/
/** Params:																**/
/**	@uiBlockNumber:		Number of alloc blocks.							**/
/** Return:			Start index of alloc blocks.						**/
/** Notice:		None												**/
/*************************************************************************/
size_t mmu_AllocBlocks(size_t uiBlockNumber)
{
	size_t		uiStartBlockIndex;
    size_t		uiCountOfBlocks;
    size_t		uiBlockIndex;

    // No need search blocks or alloc blocks too much.
	if((0 == uiBlockNumber) || (MMU_BLOCK_QUANTITY < uiBlockNumber))
	{
		uiStartBlockIndex = MMU_INVALID_BLOCK_INDEX;
	}
	// Search for continuous blocks.
	else
	{
		// Variable initialization.
		uiStartBlockIndex	= 0;
		uiCountOfBlocks		= 0;
		uiBlockIndex		= 0;
		// Search continuous blocks.
        while((uiCountOfBlocks < uiBlockNumber) && (uiBlockIndex < MMU_BLOCK_QUANTITY))
		{
			if(MMU_BLOCK_STATE_UNUSE == MMU_HEAP_OBJECT.Heap.BlockState[uiBlockIndex])
			{
				uiBlockIndex ++;
				uiCountOfBlocks ++;
			}
			else
			{
				// Search failed, restart.
				uiBlockIndex ++;
				uiStartBlockIndex = uiBlockIndex;
				uiCountOfBlocks = 0;
			}
		}
		if(uiCountOfBlocks != uiBlockNumber)
		{
			uiStartBlockIndex = MMU_INVALID_BLOCK_INDEX;
		}
	}
	return uiStartBlockIndex;
}

/*************************************************************************/
/** Function Name:	mmu_AllocHeap.										**/
/** Purpose:		Allocate heap space in bytes.						**/
/** Resources:		Declared heap space memory.							**/
/** Params:																**/
/**	@uiSize:			Allocate heap size in bytes.					**/
/** Return:			Address offset form head of heap.					**/
/** Notice:		None												**/
/*************************************************************************/
size_t mmu_AllocHeap(size_t uiSize)
{
	size_t		uiAllocBlockNumber;
	size_t		uiAllocStartBlockIndex;
	size_t		uiAllocHeapOffset;

	// Initialize.
	if(MMU_HEAP_OBJECT.State == MMU_STATE_UNINITIALIZED)
	{
		mmu_Initialize();
	}
	// Alloc memory
	if(uiSize == 0)
	{
		uiAllocHeapOffset = MMU_INVALID_HEAP_INDEX;
	}
	else
	{
		uiAllocBlockNumber = BLOCK_SIZE(uiSize);
		uiAllocStartBlockIndex = mmu_AllocBlocks(uiAllocBlockNumber);
		if(MMU_INVALID_BLOCK_INDEX != uiAllocStartBlockIndex)
		{
			uiAllocHeapOffset = uiAllocStartBlockIndex * MMU_BLOCK_SIZE;
			// Mark head blocks of alloc memory.
			MMU_HEAP_OBJECT.Heap.BlockState[uiAllocStartBlockIndex] = MMU_BLOCK_STATE_START;
			uiAllocStartBlockIndex ++;
			uiAllocBlockNumber--;
			// Mark body blocks of alloc memory.
            while(uiAllocBlockNumber > 0)
			{
				MMU_HEAP_OBJECT.Heap.BlockState[uiAllocStartBlockIndex] = MMU_BLOCK_STATE_INUSE;
				uiAllocStartBlockIndex ++;
				uiAllocBlockNumber --;
			}
		}
		else
		{
			uiAllocHeapOffset = MMU_INVALID_HEAP_INDEX;
		}
	}
	return uiAllocHeapOffset;
}

/*************************************************************************/
/** Function Name:	mmu_FreeBlocks.										**/
/** Purpose:		Release inuse heap blocks.							**/
/** Resources:		Declared heap space memory.							**/
/** Params:																**/
/**	@uiStartIndex:		Start index of blocks to be released.			**/
/** Return:			Count of released blocks.							**/
/** Notice:		None.												**/
/*************************************************************************/
size_t mmu_FreeBlocks(size_t uiStartIndex)
{
	size_t	uiStartBlockIndex		= uiStartIndex;
	size_t	uiCountOfFreeBlocks		= 0;

	// Free head block of free area.
	MMU_HEAP_OBJECT.Heap.BlockState[uiStartBlockIndex] = MMU_BLOCK_STATE_UNUSE;
	uiStartBlockIndex ++;
	uiCountOfFreeBlocks++;
	// Free other inuse blocks
	while((MMU_HEAP_OBJECT.Heap.BlockState[uiStartBlockIndex] == MMU_BLOCK_STATE_INUSE) && (uiStartBlockIndex))
	{
		MMU_HEAP_OBJECT.Heap.BlockState[uiStartBlockIndex] = MMU_BLOCK_STATE_UNUSE;
		uiStartBlockIndex ++;
		uiCountOfFreeBlocks++;
	}

	return uiCountOfFreeBlocks;
}

/*************************************************************************/
/** Function Name:	mmu_malloc.											**/
/** Purpose:		Allocate a memory space in heap, this interface		**/
/**					will be called by the user.							**/
/** Resources:		Declared heap space memory.							**/
/** Params:																**/
/**	@uiSize:			Size of alloc memory space.						**/
/** Return:																**/
/**	@NULL				Alloc failed.									**/
/** Notice:		None.												**/
/*************************************************************************/
void* mmu_malloc(size_t uiSize)
{
    void* pReturn = NULL;
    size_t	uiAddressOffset;

    uiAddressOffset = mmu_AllocHeap(uiSize);
    if(MMU_INVALID_HEAP_INDEX != uiAddressOffset)
	{
		pReturn = MMU_HEAP_OBJECT.Heap.HeapMemory + uiAddressOffset;
	}
	return pReturn;
}

/*************************************************************************/
/** Function Name:	mmu_realloc.										**/
/** Purpose:		Resize a allocated memory space, this interface		**/
/**					will be called by the user.							**/
/** Resources:		Declared heap space memory.							**/
/** Params:																**/
/**	@pPointer:			Pointer of heap space to be resized.			**/
/**	@uiSize:			New size of memory space.						**/
/** Return:																**/
/**	@NULL				Resize failed.									**/
/** Notice:		None.												**/
/*************************************************************************/
void* mmu_realloc(void* pPointer, size_t uiSize)
{
    void*	pOldPointer;
    void*	pNewPointer;
    size_t	uiNewSize_Heap;
    size_t	uiOldSize_Blocks;
    size_t	uiNewSize_Blocks;
    size_t	uiChangedSize_Blocks;
    size_t	uiFreeBlocks;
    size_t	uiStartBlockOffset;
    size_t	uiBlockIndex;

    pOldPointer = pPointer;
    // Check pointer is valid.
    if( ((size_t)pOldPointer < (size_t)MMU_HEAP_OBJECT.Pointer.HeapHead) ||
		((size_t)pOldPointer > (size_t)MMU_HEAP_OBJECT.Pointer.HeapEnd)	 ||
		(pOldPointer == NULL))
	{
		pNewPointer = NULL;
	}
	else
	{
		// Get start block index of allocate heap.
		uiStartBlockOffset = ((size_t)pPointer - (size_t)MMU_HEAP_OBJECT.Pointer.HeapHead) / MMU_BLOCK_SIZE;
        if(MMU_HEAP_OBJECT.Heap.BlockState[uiStartBlockOffset] == MMU_BLOCK_STATE_UNUSE)
		{
			// Reallocate must be done after allocated.
			pNewPointer = NULL;
		}
		else
		{
			// Get number of allocated block before resize.
			uiBlockIndex = uiStartBlockOffset + 1;
			uiOldSize_Blocks = 1;
			while(MMU_HEAP_OBJECT.Heap.BlockState[uiBlockIndex] == MMU_BLOCK_STATE_INUSE)
			{
				uiBlockIndex++;
				uiOldSize_Blocks ++;
			}
			// Get number of allocated block after resize.
			uiNewSize_Heap = uiSize;
			uiNewSize_Blocks = BLOCK_SIZE(uiNewSize_Heap);

			//Fewer blocks
			if(uiOldSize_Blocks > uiNewSize_Blocks)
			{
				uiChangedSize_Blocks = uiOldSize_Blocks - uiNewSize_Blocks;
                while(uiChangedSize_Blocks != 0)
				{
					uiBlockIndex--;
					MMU_HEAP_OBJECT.Heap.BlockState[uiBlockIndex] = MMU_BLOCK_STATE_UNUSE;
					uiChangedSize_Blocks --;
				}
				pNewPointer = pOldPointer;
			}
			// More blocks.
			else if(uiOldSize_Blocks < uiNewSize_Blocks)
			{
				uiChangedSize_Blocks = uiNewSize_Blocks - uiOldSize_Blocks;
				uiFreeBlocks = 0;
				while((uiFreeBlocks < uiChangedSize_Blocks) && (MMU_HEAP_OBJECT.Heap.BlockState[uiBlockIndex] == MMU_BLOCK_STATE_UNUSE))
				{
					uiFreeBlocks++;
					uiBlockIndex++;
				}
				if(uiFreeBlocks == uiChangedSize_Blocks)
				{
					while(uiFreeBlocks != 0)
					{
						uiFreeBlocks--;
						uiBlockIndex--;
						MMU_HEAP_OBJECT.Heap.BlockState[uiBlockIndex] = MMU_BLOCK_STATE_INUSE;
					}
					// Pointer is no change.
					pNewPointer = pOldPointer;
				}
				else
				{
					// Allocate new heap space.
					pNewPointer = mmu_malloc(uiNewSize_Blocks * MMU_BLOCK_SIZE);
					if(NULL == pNewPointer)
					{
						// Allocate new space failed.
						//pNewPointer = NULL;
					}
					else
					{
                        mmu_memcpy(pNewPointer, pOldPointer, uiOldSize_Blocks * MMU_BLOCK_SIZE);
                        mmu_free(pOldPointer);
					}
				}
			}
			// No changed.
			else
			{
				// Allocate blocks no changed.
				pNewPointer = pOldPointer;
			}
		}
	}
	return pNewPointer;
}

/*************************************************************************/
/** Function Name:	mmu_free.											**/
/** Purpose:		Free a allocated memory space in heap, this			**/
/**					interface will be called by the user.				**/
/** Resources:		Declared heap space memory.							**/
/** Params:																**/
/**	@uiSize:			Pointer of heap space to be released.			**/
/** Return:			None.												**/
/** Notice:		None.												**/
/*************************************************************************/
void mmu_free(void*	pPointer)
{

	size_t	uiHeapOffset;
	size_t  uiHeapBlockOffset;

	// Initialize.
	if(MMU_HEAP_OBJECT.State == MMU_STATE_UNINITIALIZED)
	{
		mmu_Initialize();
	}

	if( ((size_t)pPointer < (size_t)MMU_HEAP_OBJECT.Pointer.HeapHead) ||
		((size_t)pPointer > (size_t)MMU_HEAP_OBJECT.Pointer.HeapEnd))
	{
		// Address of free pointer is invalid.
	}
	else
	{
		uiHeapOffset = (size_t)pPointer - (size_t)MMU_HEAP_OBJECT.Pointer.HeapHead;
		uiHeapBlockOffset = uiHeapOffset/MMU_BLOCK_SIZE;
		 // Free head block of free area.
		mmu_FreeBlocks(uiHeapBlockOffset);
	}
}

/*************************************************************************/
/** Function Name:	mmu_memcpy											**/
/** Purpose:		Copy a memory block to a new area.					**/
/** Resources:		None.												**/
/** Params:																**/
/**	@pDest:				Destination address pointer.					**/
/**	@pSrc:				Source address pointer.							**/
/**	@uiSize:			Copy area size.									**/
/** Return:			Address offset form head of heap.					**/
/** Notice:		When source area and destination area crossed, data	**/
/**					will be contaminated.								**/
/*************************************************************************/
void* mmu_memcpy(void* pDest, const void* pSrc, size_t uiSize)
{
    size_t				uiOperandNum		= uiSize / MMU_ALIGN_BYTE;
	size_t				uiByteNum			= uiSize % MMU_ALIGN_BYTE;
    MMU_OPERAND_TYPE*	pOperandSrc			= (MMU_OPERAND_TYPE *)pSrc;
    MMU_OPERAND_TYPE*	pOperandDest		= (MMU_OPERAND_TYPE *)pDest;

	while(uiOperandNum--)
	{
		*pOperandDest++ = *pOperandSrc++;
	}
	while(uiByteNum--)
	{
		*(uint8_t*)pOperandDest++ = *(uint8_t*)pOperandSrc++;
	}
    return pDest;
}

/*************************************************************************/
/** Function Name:	mmu_memcpy2											**/
/** Purpose:		Copy a memory block to a new area.					**/
/** Resources:		None.												**/
/** Params:																**/
/**	@pDest:				Destination address pointer.					**/
/**	@pSrc:				Source address pointer.							**/
/**	@uiSize:			Copy area size.									**/
/** Return:			Address offset form head of heap.					**/
/** Notice:		None.												**/
/*************************************************************************/
void* mmu_memcpy2(void* pDest, const void* pSrc, size_t uiSize)
{
	uint8_t		*pOptSrcPtr, *pOptDestPtr;

	pOptSrcPtr = (uint8_t*)pSrc;
	pOptDestPtr = (uint8_t*)pDest;

	if((uiSize > 0) || ((size_t)pOptDestPtr != (size_t)pOptSrcPtr))
	{
		// Memory area crossed.
		if(((size_t)pOptSrcPtr<(size_t)pOptDestPtr) && ((size_t)pOptDestPtr-(size_t)pOptSrcPtr<uiSize))
		{
			pOptDestPtr = pOptDestPtr + uiSize - 1;
			pOptSrcPtr = pOptSrcPtr + uiSize - 1;
			while(uiSize-- != 0)
			{
				*(uint8_t*)pOptDestPtr-- = *(uint8_t*)pOptSrcPtr--;
			}
		}
		// Memory area not crossed.
		else
		{
			while(uiSize-- != 0)
			{
				*(uint8_t*)pOptDestPtr++ = *(uint8_t*)pOptSrcPtr++;
			}
		}
	}
    return pDest;
}

/*************************************************************************/
/** Function Name:	mmu_memset											**/
/** Purpose:		Fill the memory with the parameter value.			**/
/** Resources:		None.												**/
/** Params:																**/
/**	@puiMemPtr:			Pointer of will be filled memory space head.	**/
/**	@uiFillValue:		Fill value.										**/
/**	@uiSize:			Fill size.										**/
/** Return:			Address offset form head of heap.					**/
/** Notice:		Fill value not greater then 0xFF(255), higher part	**/
/**					will be ignored	if existed.							**/
/*************************************************************************/
void mmu_memset(void *puiMemPtr, uint16_t uiFillValue, size_t uiSize)
{
    size_t				uiOperandNum		= uiSize / MMU_ALIGN_BYTE;
	size_t				uiByteNum			= uiSize % MMU_ALIGN_BYTE;
    MMU_OPERAND_TYPE*	pDest				= (MMU_OPERAND_TYPE *)puiMemPtr;
    MMU_OPERAND_TYPE	uiFillOperand		= 0x0000;
    uint16_t			uiFillOperandIndex	= MMU_ALIGN_BYTE;

    while(uiFillOperandIndex --)
	{
		uiFillOperand <<= 8;
		uiFillOperand |= (uiFillValue & 0x00FF);
	}

    while(uiOperandNum--)
	{
		*pDest++ = uiFillOperand;
	}
	while(uiByteNum--)
	{
		*(uint8_t*)pDest++ = uiFillValue;
	}
}

