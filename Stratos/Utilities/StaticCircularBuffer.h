/*
 * StaticCircularBuffer.h
 *
 *  Created on: 20.10.2017
 *      Author: Karol
 */

#ifndef STATICCIRCULARBUFFER_H_
#define STATICCIRCULARBUFFER_H_

#include <cstddef>
#include <array>


template<class T, size_t TSize>
class StaticCircullarBuffer
{
public:
    StaticCircullarBuffer();

    T& GetHeadElement();
    T& GetTailElement();

    void Push();
    void Pop();

    bool IsBufferFull() const;
private:
    std::array<T,TSize> buffer;
    size_t head;
    size_t tail;
    size_t size;
    size_t capacity;
};





template<class T, size_t TSize>
StaticCircullarBuffer<T,TSize>::StaticCircullarBuffer()
{
    head = 0;
    tail = 0;
    size = 0;
    capacity = TSize;


}

template<class T, size_t TSize>
T& StaticCircullarBuffer<T,TSize>::GetHeadElement()
{
    return buffer[head];
}

template<class T, size_t TSize>
T& StaticCircullarBuffer<T,TSize>::GetTailElement()
{
    return buffer[tail];
}

template<class T, size_t TSize>
void StaticCircullarBuffer<T,TSize>::Push()
{
    head++;
    size++;
    if(head == capacity)
    {
        head = 0;
    }
}

template<class T, size_t TSize>
void StaticCircullarBuffer<T,TSize>::Pop()
{
    tail++;
    size--;
    if(tail == capacity)
    {
        tail = 0;
    }
}

template<class T, size_t TSize>
bool StaticCircullarBuffer<T,TSize>::IsBufferFull() const
{
	if(size == capacity)
		return true;

	return false;
}
#endif /* STATICCIRCULARBUFFER_H_ */
