﻿#ifndef CSIGNAL_HPP
#define CSIGNAL_HPP

#include <functional>
#include <map>
#include <vector>

template <typename... Args>
class CSignal
{
public:
    CSignal():m_nCurrentId(0){}
    CSignal(CSignal const & other):m_nCurrentId(0){}

    template <typename T>
    int connect_member(T * inst,void (T::*func)(Args...)){
        return s_Connect([=](Args... args){
            (inst->*func)(args...);
        });
    }

    template <typename T>
    int connect_member(T * inst, void (T::*func)(Args...)const){
        return s_Connect([=](Args... args){
            (inst->*func)(args...);
        });
    }

    int s_Connect(std::function<void(Args...)>const & slot)const{
        m_slots.insert(std::make_pair(++m_nCurrentId,slot));
        return m_nCurrentId;
    }
    void s_Disconnect(int id)const{
        m_slots.erase(id);
    }
    void s_DisconnectAll()const{
        m_slots.clear();
    }

    void s_Emit(Args... p){
        for (auto it : m_slots) {
            it.second(p...);
        }
    }

    void s_Disconnect(const std::vector<int> idList)const{
        for(int i=0;i<idList.size();++i)
            s_Disconnect(idList[i]);
    }
//    CSignal & operator =(CSignal const & other){
//        s_DisconnectAll();
//        return *this;
//    }

private:
    mutable int m_nCurrentId;
    mutable std::map<int,std::function<void (Args...)>> m_slots;
};
#endif // CSIGNAL_HPP
