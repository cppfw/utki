/**
 * @file PoolStored.hpp
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief Memory Pool.
 * Alternative memory allocation functions for simple objects.
 * The main purpose of this facilities is to prevent memory fragmentation.
 */

#pragma once

#include <new>
#include <list>
#include <vector>
#include <mutex>

#include "debug.hpp"
#include "Exc.hpp"
#include "SpinLock.hpp"


//#define M_ENABLE_POOL_TRACE
#ifdef M_ENABLE_POOL_TRACE
#	define M_POOL_TRACE(x) TRACE(<<"[POOL] ") TRACE(x)
#else
#	define M_POOL_TRACE(x)
#endif

namespace utki{




template <size_t element_size, std::uint32_t num_elements_in_chunk = 32> class MemoryPool{
#if M_COMPILER == M_COMPILER_MSVC //TODO: remove when MSVC supports aligas(), perhaps in VS2014
	__declspec(align(4))
#endif
	struct
#if M_COMPILER != M_COMPILER_MSVC  //TODO: remove when MSVC supports aligas(), perhaps in VS2014
	alignas(int)
#endif
	ElemSlot{
		std::uint8_t buf[element_size];
	};
	
	struct Chunk{
		size_t freeIndex = 0;//Used for first pass of elements allocation.
		
		ElemSlot elements[num_elements_in_chunk];
		
		std::vector<std::uint32_t> freeIndices;

		Chunk(){
			ASSERT(num_elements_in_chunk != 0)
		}
		
		std::uint32_t NumAllocated()const noexcept{
			ASSERT(this->freeIndex > this->freeIndices.size() || (this->freeIndex == 0 && this->freeIndices.size() == 0))
			return this->freeIndex - this->freeIndices.size();
		}
		
		//returns number of free slots in chunk
		std::uint32_t NumFree()const noexcept{
			ASSERT(num_elements_in_chunk >= this->NumAllocated())
			return num_elements_in_chunk - this->NumAllocated();
		} 
		
		bool IsFull()const noexcept{
			return this->freeIndex == num_elements_in_chunk && this->freeIndices.size() == 0;
		}
		
		bool IsEmpty()const noexcept{
			return this->freeIndex == this->freeIndices.size();
		}
		
		ElemSlot& Alloc(){
			if(this->freeIndices.size() != 0){
				ASSERT(this->freeIndex != 0)
				
				//use one of these to keep size of the vector at minimum
				std::uint32_t idx = this->freeIndices.back();
				this->freeIndices.pop_back();
				ASSERT(idx < this->freeIndex)
				return this->elements[idx];
			}
			ASSERT(this->freeIndex < num_elements_in_chunk)
			return this->elements[this->freeIndex++];
		}

		void Free(ElemSlot& e)noexcept{
			ASSERT(this->HoldsElement(e))
			std::uint32_t idx = std::uint32_t(&e - &this->elements[0]);
			ASSERT(idx < num_elements_in_chunk)
			this->freeIndices.push_back(idx);
		}
		
		bool HoldsElement(ElemSlot& e)const noexcept{
			ASSERT(num_elements_in_chunk != 0)
			return (&this->elements[0] <= &e) && (&e <= &this->elements[num_elements_in_chunk - 1]);
		}
		
	private:
		Chunk& operator=(const Chunk&);
	};

	typedef std::list<Chunk> T_ChunkList;
	T_ChunkList fullChunks;
	T_ChunkList chunks;
	
	utki::SpinLock lock;
	
public:
	~MemoryPool()noexcept{
		ASSERT_INFO(
				this->fullChunks.size() == 0 && this->chunks.size() == 0,
				"MemoryPool: cannot destroy memory pool because it is not empty. Check for static PoolStored objects, they are not allowed, e.g. static Ref/WeakRef are not allowed!"
			)
	}
	
public:
	void* Alloc_ts(){
		std::lock_guard<decltype(this->lock)> guard(this->lock);
		
		if(this->chunks.size() == 0){
			//create new chunk
			this->chunks.push_front(Chunk());
		}
		
		//get first chunk and allocate element from it
		ElemSlot& ret = this->chunks.front().Alloc();

		//if chunk became full, move it to list of full chunks
		if(this->chunks.front().IsFull()){
			this->fullChunks.splice(this->fullChunks.begin(), this->chunks, this->chunks.begin());
		}

		return reinterpret_cast<void*>(&ret);
	}

	void Free_ts(void* p)noexcept{
		if(p == 0){
			return;
		}
		
		std::lock_guard<decltype(this->lock)> guard(this->lock);
		
		ElemSlot& e = *reinterpret_cast<ElemSlot*>(p);
		
		for(typename T_ChunkList::iterator i = this->chunks.begin(); i != this->chunks.end(); ++i){
			if(i->HoldsElement(e)){
				i->Free(e);
				if(i->IsEmpty()){
					this->chunks.erase(i);
				}
				return;
			}
		}
		
		for(typename T_ChunkList::iterator i = this->fullChunks.begin(); i != this->fullChunks.end(); ++i){
			if(i->HoldsElement(e)){
				i->Free(e);
				this->chunks.splice(this->chunks.end(), this->fullChunks, i);
				return;
			}
		}
	}
};//~template class MemoryPool



template <size_t element_size, size_t num_elements_in_chunk> class StaticMemoryPool{
	static MemoryPool<element_size, num_elements_in_chunk> instance;
public:
	
	static void* Alloc_ts(){
		return instance.Alloc_ts();
	}
	
	static void Free_ts(void* p)noexcept{
		instance.Free_ts(p);
	}
};



template <size_t element_size, size_t num_elements_in_chunk> typename utki::MemoryPool<element_size, num_elements_in_chunk> utki::StaticMemoryPool<element_size, num_elements_in_chunk>::instance;



/**
 * @brief Base class for pool-stored objects.
 * If the class is derived from PoolStored it will override 'new' and 'delete'
 * operators for that class so that the objects would be stored in the
 * memory pool instead of using standard memory manager to allocate memory.
 * Storing objects in memory pool allows to avoid memory fragmentation.
 * PoolStored is only useful for systems with large amount of small and
 * simple objects which have to be allocated dynamically (i.e. using new/delete
 * operators).
 * NOTE: class derived from PoolStored SHALL NOT be used as a base class further.
 */
template <class T, unsigned num_elements_in_chunk> class PoolStored{

protected:
	//this should only be used as a base class
	PoolStored(){}

public:

	static void* operator new(size_t size){
		M_POOL_TRACE(<< "new(): size = " << size << std::endl)
		if(size != sizeof(T)){
			throw utki::Exc("PoolStored::operator new(): attempt to allocate memory block of incorrect size");
		}

		return StaticMemoryPool<sizeof(T), num_elements_in_chunk>::Alloc_ts();
	}

	static void operator delete(void *p)noexcept{
		StaticMemoryPool<sizeof(T), num_elements_in_chunk>::Free_ts(p);
	}

private:
};



}//~namespace ting
