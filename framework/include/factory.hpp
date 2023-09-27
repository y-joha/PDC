/*********************************
 * Author: Yohai Azoulai			
 * File: factory.hpp					
 * ******************************/

#pragma once

#define UNUSED(expr) do { (void)(expr); } while (0)

#include <unordered_map>
#include <memory>

#include "yoh_color.h"


template<typename BASE,typename KEY,typename PARAM>
class Factory
{
public :
    template<typename DERIVED>
    void Add(const KEY& key);
    
    std::shared_ptr<BASE> Create(const KEY& key, PARAM param);

private :
    
    template<typename DERIVED>
    static std::shared_ptr<BASE> Creator(PARAM param);
    std::unordered_map<KEY, std::shared_ptr<BASE> (*)(PARAM param)> m_creators;

}; // Class Factory

template <typename BASE, typename KEY, typename PARAM>
template <typename DERIVED>
inline void Factory<BASE, KEY, PARAM>::Add(const KEY& key)
{
    m_creators[key] = &Creator<DERIVED>;
}

template <typename BASE, typename KEY, typename PARAM>
inline std::shared_ptr<BASE> Factory<BASE, KEY, PARAM>::Create(const KEY &key, PARAM param)
{
    return m_creators[key](param);
}

template <typename BASE, typename KEY, typename PARAM>
template <typename DERIVED>
inline std::shared_ptr<BASE> Factory<BASE, KEY, PARAM>::Creator(PARAM param)
{
    return std::shared_ptr<BASE>(new DERIVED(param));
}


