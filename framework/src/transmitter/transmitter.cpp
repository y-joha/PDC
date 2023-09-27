/*********************************
 * Reviewer: No body				
 * Author: Yohai Azoulai			
 * File: transmitter.cpp					
 * ******************************/
 
 /*  */

#include "factory.hpp"
#include "transmitter.hpp"
#include "singleton.hpp"
/*******Defines******/



/*******Structs******/



/*******Service Funcs******/



/*******Funcs Funcs******/


Dispatcher<slice_t>* Transmitter::m_dispatcher(new Dispatcher<slice_t>);

bool Transmitter::HandleReply(int frg_indx)
{
    m_set_fregs_to_publish.erase(frg_indx);
    return m_set_fregs_to_publish.empty();
}

void Transmitter::NotifyTheSubs(slice_t messeg)
{
    PrintColor(On_IRed);
    printf("Notifying\n");
    PrintColor(Color_Off);
    m_dispatcher->Notify(messeg);
}

void Transmitter::Add(const Callback<slice_t> &callback)
{
    m_dispatcher->Add(callback);
}

int Transmitter::SplitNSendTask(std::shared_ptr<slice_t> params_struct)
{
    
    Singleton<Factory<Command,uint32_t,std::shared_ptr<slice_t> > > factory;
    uint32_t frags = (params_struct->request->dataLen)/Info_t::FragmentLength;
    
    // fix logic for specific number of fregments per IOT
    // for(int i = 0 , j = 0; i < FragCount() ; i += NUM OF IOTS , j++)
    for (uint32_t i = params_struct->m_header.m_fragment_Indx ; i < frags; i += Info_t::number_of_iot)
    {
        m_set_fregs_to_publish.insert(i);
    }

    for( ; params_struct->m_header.m_fragment_Indx < frags ; 
            params_struct->m_header.m_fragment_Indx += Info_t::number_of_iot,
            params_struct->m_header.m_iotOffset += Info_t::FragmentLength)
    {
        std::shared_ptr<Command> com = factory->Create(params_struct->m_header.m_type,params_struct);
        com->Build();
        NotifyTheSubs(*params_struct);
        com->Send();
    }

    return 0;
}
