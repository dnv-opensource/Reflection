#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <iterator>
#include <memory>
class jsValue;

struct iterator_holder_base 
{
    iterator_holder_base(iterator_holder_base const& rhs) {}
    iterator_holder_base() {}
    virtual ~iterator_holder_base() {}
    virtual void inc() = 0;
    virtual void dec() = 0;
    virtual jsValue*        get() const= 0;
    virtual bool            equals(iterator_holder_base const* rhs) const= 0;
    virtual iterator_holder_base* construct() = 0;
};

template<typename IteratorT>
struct iterator_holder : iterator_holder_base
{
    iterator_holder(iterator_holder const& rhs) : iterator_holder_base(rhs) ,iterator_(rhs.iterator_) {}
    iterator_holder(IteratorT const& iterator) : iterator_holder_base() , iterator_(iterator) {}
    virtual ~iterator_holder() {}
    virtual void inc() {++iterator_;}
    virtual void dec() {--iterator_;}
    virtual jsValue*          get() const {return toJScript(*iterator_);}
    virtual bool            equals(iterator_holder_base const* rhs) const {
        if(iterator_holder const * cmp = dynamic_cast<iterator_holder const *>(rhs)) {
            return cmp->iterator_==iterator_;
        }
        return false;
    }
    virtual iterator_holder_base* construct() {return new iterator_holder(*this);}
    IteratorT iterator_;
};

template<typename IteratorT>
struct iterator_ref_pair_holder : iterator_holder_base
{
    //iterator_ref_pair_holder(iterator_holder const& rhs) : iterator_holder_base(rhs) ,iterator_(rhs.iterator_) {}
    iterator_ref_pair_holder(IteratorT const& iterator) : iterator_holder_base() , iterator_(iterator) {}
    virtual ~iterator_ref_pair_holder() {}
    virtual void inc() {++iterator_;}
    virtual void dec() {--iterator_;}
    virtual jsValue*          get() const {return toJScript(iterator_->second.get());}
    virtual bool            equals(iterator_holder_base const* rhs) const {
        if(iterator_ref_pair_holder const * cmp = dynamic_cast<iterator_ref_pair_holder const *>(rhs)) {
            return cmp->iterator_==iterator_;
        }
        return false;
    }
    virtual iterator_holder_base* construct() {return new iterator_ref_pair_holder(*this);}
    IteratorT iterator_;
};

template<typename IteratorT>
struct iterator_ref_holder : public iterator_holder_base
{
    iterator_ref_holder(IteratorT const& iterator) : iterator_holder_base() , iterator_(iterator) {}
    iterator_ref_holder(iterator_ref_holder const& rhs) : iterator_holder_base(rhs) ,iterator_(rhs.iterator_) {}
    virtual ~iterator_ref_holder() {}
    virtual void inc() {++iterator_;}
    virtual void dec() {--iterator_;}
    virtual jsValue*          get() const {return toJScript(iterator_->get());}
    virtual bool            equals(iterator_holder_base const* rhs) const {
        if(iterator_ref_holder const * cmp = dynamic_cast<iterator_ref_holder const *>(rhs)) {
            return cmp->iterator_==iterator_;
        }
        return false;
    }
    virtual iterator_holder_base* construct() {return new iterator_ref_holder(*this);}
    IteratorT iterator_;
};

struct jsValueIterator : 
std::iterator<std::bidirectional_iterator_tag,jsValue*,jsValue*>
{
    jsValueIterator(jsValueIterator const& rhs) : m_holder(rhs.m_holder->construct()) 
    {}    
    template<typename IteratorT>
    jsValueIterator(IteratorT const& iterator,bool dummy) 
    :  m_holder(new iterator_holder<IteratorT>(iterator))
    {
    }
    jsValueIterator(iterator_holder_base* iterator) 
    :  m_holder(iterator)
    {
    }

    ~jsValueIterator() {}
    const jsValue* operator*() const {return m_holder->get();}
    jsValue* operator*()             {return m_holder->get();}
    jsValueIterator& operator++() {m_holder->inc();return *this;}
    jsValueIterator& operator--() {m_holder->dec();return *this;}
    jsValueIterator operator++(int) {
        jsValueIterator tmp(*this);
        m_holder->inc();
        return tmp;
    }
    jsValueIterator operator--(int) {
        jsValueIterator tmp(*this);
        m_holder->dec();
        return tmp;
    }

    friend bool operator==(jsValueIterator const& lhs,jsValueIterator const& rhs) {
        return lhs.m_holder->equals(rhs.m_holder.get());            
    }
    friend bool operator!=(jsValueIterator const& lhs,jsValueIterator const& rhs) {
        return !lhs.m_holder->equals(rhs.m_holder.get());
    }
protected:
    std::auto_ptr<iterator_holder_base> m_holder;
};