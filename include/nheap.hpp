#ifndef NHEAP_HPP
#define NHEAP_HPP

#include <iostream>
#include <climits>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <functional>
#include "definitions.hpp"

template <typename T>
class NHeap
{

  public:

    std::function<bool(T&, T&)> comparator = [](T& a, T&b) -> bool {
        return a > b;
    };

    NHeap(int aridity)
    {
        m_aridity = aridity;
        m_heap.reserve(64);
        reset();
    }

    void push(T number)
    {
        unsigned int lastIndex = m_heap.size();
        unsigned int capacity = m_heap.capacity();
        if (lastIndex >= capacity)
            m_heap.reserve(capacity * 2);
        m_heap.push_back(number);
        m_heapfy(lastIndex);
        m_counter += 1;
    }

    void setCapacity(U32 capacity)
    {
        m_heap.resize(capacity);
    }

    bool update(T element)
    {
        int nodeIndex = m_findElementIndex(element);
        if (nodeIndex >= 0)
        {
            // TODO:    change the aproach, take the element,
            //          update it, select the minimun child,
            //          use this child to occupy it's place,
            //          put it self and other remainig children
            //          to the last index and make the heapfy up.
            m_heap[nodeIndex] = element;
            // try to update upwards.
            U32 n_swaps = m_heapfy(nodeIndex);
            // if no swap was made upper, then maybe some swap
            // can be needed down
            if (n_swaps == 0)
                m_heapfyDown(nodeIndex);
            return true;
        }

        return false;
    }

    bool empty()
    {
        return m_counter == 0;
    }

    bool hasNext()
    {
        return m_counter > 0;
    }

    T& top() {
        return m_heap[m_rootIndex];
    }

    void pop() {

        // the extracted
        T next = m_heap[m_rootIndex];
        m_swapMap.erase(next);

        // replace the first element with the last one
        if (m_counter - 1 > m_rootIndex)
            m_heap[m_rootIndex] = m_heap[m_counter - 1];
        // ... or remove the first element if the heap is empty
        else m_heap.erase(m_heap.begin() + m_rootIndex);

        m_counter -= 1;
        // verify if the heapfy for all the tree from end to begining
        m_heapfyDown(m_rootIndex);
    }

    void reset()
    {
        m_rootIndex = 0;
        m_counter = 0;
        m_heap.clear();
        m_heap.erase(m_heap.begin(), m_heap.end());
        m_swapMap.clear();
    }

    int size()
    {
        return m_heap.size();
    }

    std::vector<T> heapSorted()
    {
        std::vector<T> sortedArray;

        while (hasNext())
        {
            T next = top();
            pop();
            sortedArray.push_back(next);
        }

        return sortedArray;
    }

  private:
    std::vector<T> m_heap;
    std::map<T, U32> m_swapMap;
    int m_aridity;
    int m_counter;
    int m_rootIndex;

    int childNth(int index, int childNumber)
    {
        int nodeIndex = m_aridity * index + childNumber + 1;
        return nodeIndex;
    }

    bool hasChildNth(int index, int childNumber)
    {
        int nodeIndex = m_aridity * index + childNumber + 1;
        return nodeIndex < m_rootIndex + m_counter;
    }

    int parent(int nodeIndex)
    {
        int parentIndex = (int)((float)(nodeIndex - 1) / (float)m_aridity);
        return parentIndex;
    }

    U32 m_heapfy(int nodeIndex)
    {
        // if i'm root don't do anything
        if (nodeIndex == m_rootIndex)
            return 0;

        int parentIndex = parent(nodeIndex);
        // get vector references, much more elegant to use swap
        T &parent = m_heap[parentIndex];
        T &node = m_heap[nodeIndex];

        // Store index
        m_swapMap[parent] = parentIndex;
        m_swapMap[node] = nodeIndex;

        // if parent is higher than watching node, change their places in memory
        if (comparator(parent, node))
        {
            // Store swapped index
            m_swapMap[parent] = nodeIndex;
            m_swapMap[node] = parentIndex;
            m_swap(parent, node);
            return m_heapfy(parentIndex) + 1;
        }

        return 0;
    }

    void m_heapfyDown(int nodeIndex)
    {

        if (nodeIndex >= m_counter + 1)
            return;

        // find the minimun child and make the way throughout it
        int minimumChildIndex = m_findMinimumChildIndex(nodeIndex);
        if (minimumChildIndex < 0) return;

        T &childValue = m_heap[minimumChildIndex];
        T &nodeValue = m_heap[nodeIndex];

        // Update swap indexes
        m_swapMap[nodeValue] = nodeIndex;

        // if minimun child is higher than it's parent, swap both
        if (comparator(nodeValue, childValue)) {
            // Store index
            m_swapMap[childValue] = nodeIndex;
            m_swapMap[nodeValue] = minimumChildIndex;
            m_swap(nodeValue, childValue);
            // swap only one child, not all of them
            m_heapfyDown(minimumChildIndex);
        }
    }

    int m_findMinimumChildIndex(int nodeIndex) {
        // find the minimun child and make the way throughout it
        if (hasChildNth(nodeIndex, 0)) {
            int tmpChildIndex = childNth(nodeIndex, 0);
            T tmpChildValue = m_heap[tmpChildIndex];

            // Update swap indexes
            m_swapMap[tmpChildValue] = tmpChildIndex;

            for (int i = 1; i < m_aridity; i++) {
                if (hasChildNth(nodeIndex, i)) {
                    int anotherChildIndex = childNth(nodeIndex, i);
                    T anotherChildValue = m_heap[anotherChildIndex];
                    // Update swap indexes
                    m_swapMap[anotherChildValue] = anotherChildIndex;

                    // Update the minimum child
                    if (comparator(tmpChildValue, anotherChildValue)) {
                        tmpChildValue = anotherChildValue;
                        tmpChildIndex = anotherChildIndex;
                    }
                }
            }

            return tmpChildIndex;
        }

        return -1;
    }

    int m_findElementIndex(T &element)
    {

        auto it = m_swapMap.find(element);
        // if iterator is valid return it stored value
        if (it != m_swapMap.end())
            return m_swapMap[element];
        // otherwise, make a linear search
        for (unsigned int i = 0; i < m_heap.size(); i++)
        {
            if (m_heap[i] == element)
                return i;
        }

        return -1;
    }

    void m_swap(T &l, T &r)
    {
        T t = l;
        l = r;
        r = t;
    }
};

#endif /*NHEAP_HPP*/
