#pragma once

#include <algorithm>
#include <cstdint>
#include <exception>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>

namespace usu
{
    template <typename T>
    class vector
    {
      public:
        using size_type = std::size_t;
        using reference = T&;
        using pointer = std::shared_ptr<T[]>;
        using value_type = T;
        using resize_type = std::function<size_type(size_type)>;

        //-------------------------
        // Iterator Code
        //------------------------

        class iterator
        {
            using iterator_category = std::forward_iterator_tag;

          public:
            iterator() :
                iterator(nullptr) {}

            iterator(pointer ptr);
            iterator(size_type pos, pointer ptr);

            iterator(const iterator& obj);
            iterator(iterator&& obj) noexcept;
            iterator& operator=(const iterator& rhs);
            iterator& operator=(iterator&& rhs);
            iterator operator++();
            iterator operator++(int);
            iterator operator--();
            iterator operator--(int);

            T* operator->();
            usu::vector<T>::reference operator*();
            bool operator==(const iterator& rhs);
            bool operator!=(const iterator& rhs);

          private:
            usu::vector<T>::size_type m_pos;
            usu::vector<T>::pointer m_data;
        };

        //-------------------------
        // Vector Code
        //-------------------------

        vector();
        vector(size_type size);
        vector(resize_type resize);
        vector(size_type size, resize_type resize);
        vector(std::initializer_list<T> list);
        vector(std::initializer_list<T> list, resize_type resize);

        reference operator[](size_type index);
        void add(T value);
        void insert(size_type index, T value);
        void remove(size_type index);
        void clear();
        size_type size();
        size_type capacity();
        iterator begin();
        iterator end();

      private:
        size_type m_size;
        size_type m_capacity;
        resize_type m_resize_type;
        std::shared_ptr<T[]> m_data;
    };


    //------------------
    // Vector code
    //-----------------

    template <typename T>
    usu::vector<T>::vector()
    {
        this->m_size = 0;
        this->m_capacity = 10;
        this->m_resize_type = [](size_type currentCapacity) -> size_type
        {
            return currentCapacity * 2;
        };
        this->m_data = std::make_shared<T[]>(this->m_capacity);
    }

    template <typename T>
    usu::vector<T>::vector(usu::vector<T>::size_type size) :
        vector()
    {
        this->m_size = size;
        this->m_capacity = this->m_size > 10 ? this->m_size * 2 : 10;
    }

    template <typename T>
    usu::vector<T>::vector(usu::vector<T>::resize_type resize) :
        vector()
    {
        this->m_resize_type = resize;
    }

    template <typename T>
    usu::vector<T>::vector(usu::vector<T>::size_type size, usu::vector<T>::resize_type resize) :
        vector()
    {
        this->m_size = size;
        this->m_capacity = this->m_size > 10 ? this->m_size * 2 : 10;
        this->m_resize_type = resize;
    }

    template <typename T>
    usu::vector<T>::vector(std::initializer_list<T> list) :
        vector()
    {
        for (auto i = list.begin(); i != list.end(); ++i)
        {
            this->add(*i);
        }
    }

    template <typename T>
    usu::vector<T>::vector(std::initializer_list<T> list, usu::vector<T>::resize_type resize) :
        vector(list)
    {
        this->m_resize_type = resize;
    }

    template <typename T>
    void usu::vector<T>::add(T value)
    {
        if (this->m_capacity == this->m_size)
        {
            this->m_capacity = this->m_resize_type(this->m_capacity);
            std::shared_ptr<T[]> temp = std::make_shared<T[]>(this->m_capacity);

            std::copy(this->m_data.get(), this->m_data.get() + this->m_size, temp.get());
            this->m_data = temp;
        }
       
        this->m_data[this->m_size++] = value; // possbily remove - 1
    }

    template <typename T>
    usu::vector<T>::reference usu::vector<T>::operator[](usu::vector<T>::size_type index)
    {
        if (index >= this->m_size)
        {
            throw std::range_error("Index out of bounds");
        }
        return this->m_data[index];
    }

    template <typename T>
    void usu::vector<T>::insert(usu::vector<T>::size_type index, T value)
    {
        if (index > this->m_size)
        {
            throw std::range_error("Index out of bounds");
        }

        if (this->m_capacity == this->m_size)
        {
            this->m_capacity = this->m_resize_type(this->m_capacity);
            std::shared_ptr<T[]> temp = std::make_shared<T[]>(this->m_capacity);

            std::copy(this->m_data.get(), this->m_data.get() + this->m_size, temp.get());
            this->m_data = temp;
        }

        for (size_t i = this->m_size; i > index; --i)
        {
            this->m_data[i] = this->m_data[i - 1];
        }

        this->m_data[index] = value;
        this->m_size++;
    }

    template <typename T>
    void usu::vector<T>::remove(usu::vector<T>::size_type index)
    {
        if (index >= this->m_size)
        {
            throw std::range_error("Index out of bounds");
        }

        for (size_t i = index; i < this->m_size - 1; ++i)
        {
            this->m_data[i] = this->m_data[i + 1];
        }
        this->m_size--;
    }

    template <typename T>
    void usu::vector<T>::clear()
    {
        this->m_size = 0;
    }

    template <typename T>
    usu::vector<T>::size_type usu::vector<T>::size()
    {
        return this->m_size;
    }

    template <typename T>
    usu::vector<T>::size_type usu::vector<T>::capacity()
    {
        return this->m_capacity;
    }

    template <typename T>
    usu::vector<T>::iterator usu::vector<T>::begin()
    {
        return usu::vector<T>::iterator(this->m_data);
    }

    template <typename T>
    usu::vector<T>::iterator usu::vector<T>::end()
    {
        return usu::vector<T>::iterator(this->m_size, this->m_data);
    }

    //----------------------------
    // Iterator Code
    //---------------------------
    template <typename T>
    usu::vector<T>::iterator::iterator(usu::vector<T>::pointer ptr)
    {
        this->m_pos = 0;
        this->m_data = ptr;
    }

    template <typename T>
    usu::vector<T>::iterator::iterator(usu::vector<T>::size_type pos, usu::vector<T>::pointer ptr)
    {
        this->m_pos = pos;
        this->m_data = ptr;
    }

    template <typename T>
    usu::vector<T>::iterator::iterator(const usu::vector<T>::iterator& obj)
    {
        this->m_pos = obj.m_pos;
        this->m_data = obj.m_data;
    }

    template <typename T>
    usu::vector<T>::iterator::iterator(usu::vector<T>::iterator&& obj) noexcept
    {
        this->m_pos = obj.m_pos;
        this->m_data = obj.m_data;

        obj.m_pos = 0;
        obj.m_data = nullptr;
    }

    template <typename T>
    typename usu::vector<T>::iterator& usu::vector<T>::iterator::operator=(const usu::vector<T>::iterator& rhs)
    {
        this->m_pos = rhs.m_pos;
        this->m_data = rhs.m_data;

        return *this;
    }

    template <typename T>
    typename usu::vector<T>::iterator& vector<T>::iterator::operator=(usu::vector<T>::iterator&& rhs)
    {
        if (this != &rhs)
        {
            std::swap(this->m_pos, rhs.m_pos);
            std::swap(this->m_data, rhs.m_data);
        }
        return *this;
    }

    template <typename T>
    typename usu::vector<T>::iterator vector<T>::iterator::operator++()
    {
        this->m_pos++;

        return *this;
    }

    template <typename T>
    typename usu::vector<T>::iterator vector<T>::iterator::operator++(int)
    {
        iterator i = *this;

        this->m_pos++;
        return i;
    }

    template <typename T>
    typename usu::vector<T>::iterator vector<T>::iterator::operator--()
    {
        this->m_pos--;
        return *this;
    }

    template <typename T>
    typename usu::vector<T>::iterator vector<T>::iterator::operator--(int)
    {
        iterator i = *this;

        this->m_pos--;
        return i;
    }

    template <typename T>
    bool usu::vector<T>::iterator::operator==(const usu::vector<T>::iterator& rhs)
    {
        return this->m_pos == rhs.m_pos;
    }

    template <typename T>
    bool usu::vector<T>::iterator::operator!=(const usu::vector<T>::iterator& rhs)
    {
        return this->m_pos != rhs.m_pos;
    }

    template <typename T>
    T* usu::vector<T>::iterator::operator->()
    {
        return &(this->m_data[this->m_pos]);
    }

    template <typename T>
    usu::vector<T>::reference usu::vector<T>::iterator::operator*()
    {
        return this->m_data[this->m_pos];
    }

} // namespace usu