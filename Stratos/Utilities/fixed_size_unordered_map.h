/*
 * StaticCircularBuffer.h
 *
 *  Created on: 20.10.2017
 *      Author: Karol
 */

#ifndef fixed_size_unordered_map_H_
#define fixed_size_unordered_map_H_

#include <cstddef>
#include <unordered_map>

template<class Key,class T>
class fixed_size_unordered_map
{
public:
	using iterator       = typename std::unordered_map<Key,T>::iterator;
	using const_iterator = typename std::unordered_map<Key,T>::const_iterator;

    iterator begin() { return unordered_map.begin(); };
    iterator end()   { return unordered_map.end(); };

	fixed_size_unordered_map(size_t predefined_capacity);

	iterator find ( const Key& k ) { return unordered_map.find(k); }
	const_iterator find ( const Key& k ) const { return unordered_map.find(k); }


	size_t erase ( const Key& k );

	bool insert(Key k,T elem);


private:
	std::unordered_map<Key,T> unordered_map;
	size_t capacity;
	size_t size;
};

template<class Key,class T>
fixed_size_unordered_map<Key,T>::fixed_size_unordered_map(size_t predefined_capacity)
{
	unordered_map.reserve(predefined_capacity);
	capacity = predefined_capacity;
	size = 0U;
}

template<class Key,class T>
size_t fixed_size_unordered_map<Key,T>::erase ( const Key& k )
{
	size_t retval = unordered_map.erase(k);
	if(retval == 1) {
		size--;
	};
	return retval;
}

template<class Key,class T>
bool fixed_size_unordered_map<Key,T>::insert(Key k,T elem)
{
	if(size == capacity)
	{
		return false;
	}
	else
	{
		size++;
		unordered_map.insert(std::make_pair(k,elem));
	}
	return true;
}
#endif /* fixed_size_unordered_map_H_ */
