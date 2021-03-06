/* Copyright (c) 2014, Coded Project
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * * Neither the name of the Coded Project nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CODEDPROJECT_SEQUENCE_HPP
#define CODEDPROJECT_SEQUENCE_HPP

#include <type_traits>
#include <vector>

#include "sequence_expression_templates.hpp"

namespace CodedProject
{

template<typename T>
class Sequence : public std::vector<T>, public SequenceExpression<Sequence<T>>
{
public:
    using typename std::vector<T>::value_type;
    using typename std::vector<T>::size_type;
    using std::vector<T>::size;
    using std::vector<T>::at;
    using std::vector<T>::operator[];

    Sequence() = default;
    Sequence(size_type n);
    Sequence(size_type n, value_type const& value);
    template<typename InputIterator> Sequence(InputIterator first, InputIterator last);
    template<typename E> Sequence(SequenceExpression<E> const& expression);
    Sequence(Sequence<T> const& rhs) = default;
    Sequence(Sequence<T> && rhs ); // Damn you VS2013 for not generating move contructors!
    Sequence(std::initializer_list<value_type> list);
    Sequence<T>& operator=(Sequence<T> const& rhs) = default;
    Sequence<T>& operator=(Sequence<T> && rhs );
    ~Sequence() = default;

    template<typename U> 
    typename std::enable_if<!SequenceExpressionTraits<U>::is_sequence, Sequence<T>&>::type
        operator=(U const& rhs);
    template<typename U> Sequence<T>& operator=(Sequence<U> const& rhs);


    /* Unfortunately, overloads for all assignments which take the same
     * sequence type (Sequence<T>) are required in order to allow assignement
     * (or compound assignement) from a SequenceExpression type.
     * (Without it the conversion from SequenceExpression<Sequence<T>> to
     * Sequence<U> requires two conversions; hence overload resolution fails!)
     *
     * Not sure how to fix this one yet... However, because I'm sure there is a
     * solution (i.e. a way to remove the extra overload) I have just duplicated
     * the code in the appropriate functions...
     * - neurotempest 28/9/14
     */
    template<typename U>
    typename std::enable_if<!SequenceExpressionTraits<U>::is_sequence, Sequence<T>&>::type
        operator+=(U const& rhs);
    Sequence<T>& operator+=(Sequence<T> const& rhs);
    template<typename U> Sequence<T>& operator+=(Sequence<U> const& rhs);

    template<typename U>
    typename std::enable_if<!SequenceExpressionTraits<U>::is_sequence, Sequence<T>&>::type
        operator-= (U const& rhs);
    Sequence<T>& operator-=(Sequence<T> const& rhs);
    template<typename U> Sequence<T>& operator-= (Sequence<U> const& rhs);

    template<typename U> 
    typename std::enable_if<!SequenceExpressionTraits<U>::is_sequence, Sequence<T>&>::type
        operator*= (U const& rhs);
    Sequence<T>& operator*=(Sequence<T> const& rhs);
    template<typename U> Sequence<T>& operator*= (Sequence<U> const& rhs);

    template<typename U>
    typename std::enable_if<!SequenceExpressionTraits<U>::is_sequence, Sequence<T>&>::type
        operator/= (U const& rhs);
    Sequence<T>& operator/=(Sequence<T> const& rhs);
    template<typename U> Sequence<T>& operator/= (Sequence<U> const& rhs);
};

template<typename T>
Sequence<T>::Sequence(size_type n) :
    std::vector<T>(n)
{
}

template<typename T>
Sequence<T>::Sequence(size_type n, value_type const& value) :
    std::vector<T>(n, value)
{
}

template<typename T>
template<typename InputIterator>
Sequence<T>::Sequence(InputIterator first, InputIterator last) :
    std::vector<T>(first, last)
{
}

template<typename T>
template<typename E> 
Sequence<T>::Sequence(SequenceExpression<E> const& expression) :
    std::vector<T>(expression.size())
{
    for(size_type i=0; i<this->size(); ++i)
        (*this)[i] = expression[i];
}

template<typename T>
Sequence<T>::Sequence(std::initializer_list<value_type> list) :
    std::vector<T>(list)
{
}

template<typename T>
Sequence<T>::Sequence(Sequence<T> && rhs) :
    std::vector<T>(std::move(rhs))
{
}

template<typename T>
Sequence<T>& Sequence<T>::operator= (Sequence<T> && rhs )
{
    std::vector<T>::operator=(std::move(rhs));
    return *this;
}

template<typename T>
template<typename U>
typename std::enable_if<!SequenceExpressionTraits<U>::is_sequence, Sequence<T>&>::type
    Sequence<T>::operator= (U const& rhs)
{
    for(auto& element : *this)
        element = rhs;
    return *this;
}


template<typename T>
template<typename U>
Sequence<T>& Sequence<T>::operator=(Sequence<U> const& rhs)
{
    this->resize( rhs.size() );
    auto this_element = this->begin();
    for(auto const& rhs_element : rhs)
    {
        *this_element = rhs_element;
        ++this_element;
    }
    return *this;
}

template<typename T>
template<typename U>
typename std::enable_if<!SequenceExpressionTraits<U>::is_sequence, Sequence<T>&>::type
    Sequence<T>::operator+=(U const& rhs)
{
    for(auto& element : *this)
        element+=rhs;
    return *this;
}

template<typename T>
Sequence<T>& Sequence<T>::operator+=(Sequence<T> const& rhs)
{
    auto this_element = this->begin();
    for(auto const& rhs_element : rhs)
    {
        *this_element+=rhs_element;
        ++this_element;
    }
    return *this;
}

template<typename T>
template<typename U>
Sequence<T>& Sequence<T>::operator+=(Sequence<U> const& rhs)
{
    auto this_element = this->begin();
    for(auto const& rhs_element : rhs)
    {
        *this_element+=rhs_element;
        ++this_element;
    }
    return *this;
}

template<typename T>
template<typename U>
typename std::enable_if<!SequenceExpressionTraits<U>::is_sequence, Sequence<T>&>::type
    Sequence<T>::operator-=(U const& rhs)
{
    for(auto& element : *this)
        element-=rhs;
    return *this;
}

template<typename T>
Sequence<T>& Sequence<T>::operator-=(Sequence<T> const& rhs)
{
    auto this_element = this->begin();
    for(auto const& rhs_element : rhs)
    {
        *this_element-=rhs_element;
        ++this_element;
    }
    return *this;
}

template<typename T>
template<typename U>
Sequence<T>& Sequence<T>::operator-=(Sequence<U> const& rhs)
{
    auto this_element = this->begin();
    for(auto const& rhs_element : rhs)
    {
        *this_element-=rhs_element;
        ++this_element;
    }
    return *this;
}

template<typename T>
template<typename U>
typename std::enable_if<!SequenceExpressionTraits<U>::is_sequence, Sequence<T>&>::type
    Sequence<T>::operator*= (U const& rhs)
{
    for(auto& element : *this)
        element*=rhs;
    return *this;
}

template<typename T>
Sequence<T>& Sequence<T>::operator*=(Sequence<T> const& rhs)
{
    auto this_element = this->begin();
    for(auto const& rhs_element : rhs)
    {
        *this_element*=rhs_element;
        ++this_element;
    }
    return *this;
}

template<typename T>
template<typename U>
Sequence<T>& Sequence<T>::operator*= (Sequence<U> const& rhs)
{
    auto this_element = this->begin();
    for(auto const& rhs_element : rhs)
    {
        *this_element*=rhs_element;
        ++this_element;
    }
    return *this;
}

template<typename T>
template<typename U>
typename std::enable_if<!SequenceExpressionTraits<U>::is_sequence, Sequence<T>&>::type
    Sequence<T>::operator/= (U const& rhs)
{
    for(auto& element : *this)
        element/=rhs;
    return *this;
}

template<typename T>
Sequence<T>& Sequence<T>::operator/=(Sequence<T> const& rhs)
{
    auto this_element = this->begin();
    for(auto const& rhs_element : rhs)
    {
        *this_element/=rhs_element;
        ++this_element;
    }
    return *this;
}

template<typename T>
template<typename U>
Sequence<T>& Sequence<T>::operator/= (Sequence<U> const& rhs)
{
    auto this_element = this->begin();
    for(auto const& rhs_element : rhs)
    {
        *this_element/=rhs_element;
        ++this_element;
    }
    return *this;
}

}

#endif
