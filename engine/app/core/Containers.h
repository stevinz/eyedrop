/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_CONTAINERS_H
#define DR_CONTAINERS_H

#include <deque>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>


//####################################################################################
//##    Deque Functions
//############################
template<class T> bool DequeContains(const std::deque<T>& my_deque, T variable_to_check) {
    bool found = (std::find(my_deque.begin(), my_deque.end(), variable_to_check) != my_deque.end());
    return found;
}


//####################################################################################
//##    List Functions
//############################
template<class T> bool ListContains(const std::list<T>& my_list, T variable_to_check) {
    bool found = (std::find(my_list.begin(), my_list.end(), variable_to_check) != my_list.end());
    return found;
}


//####################################################################################
//##    Map Functions
//############################
template<class T, class U> bool MapHasKey(const std::map<T, U>& my_map, T key_to_find) {
    auto it = my_map.find(key_to_find);
    return (it != my_map.end());
}


//####################################################################################
//##    Set Functions
//############################
template<class T> bool SetContains(const std::set<T>& my_set, T variable_to_check) {
    bool found = (std::find(my_set.begin(), my_set.end(), variable_to_check) != my_set.end());
    return found;
}


//####################################################################################
//##    Vector Functions
//############################
template<class T> bool VectorContains(const std::vector<T>& my_vector, T variable_to_check) {
    bool found = (std::find(my_vector.begin(), my_vector.end(), variable_to_check) != my_vector.end());
    return found;
}

template<class T> void VectorCopy(std::vector<T>& copy_from, std::vector<T>& copy_to) {
    copy_to.clear();
    if (copy_from.size() > 0) {
        for (auto element : copy_from) {
            copy_to.push_back(element);
        }
    }
}


#endif // DR_CONTAINERS_H
