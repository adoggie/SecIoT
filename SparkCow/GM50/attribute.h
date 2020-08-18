//
// Created by scott on 7/30/20.
//

#ifndef MONKEY_ATTRIBUTE_H
#define MONKEY_ATTRIBUTE_H

#include "base.h"
#include "datatype.h"
#include "bytearray.h"
#include <boost/any.hpp>


typedef struct AttributeIdDetail{
    std::uint8_t  group;
    std::uint16_t  app_id;
    std::uint8_t  num;
    std::uint32_t  row;
    std::uint8_t   sub;
}  AID;


enum class  AttributeGroupId{
    Basic = 1,
    Interface = 2,
    MGR_Entity = 3,
    Private = 4
};

enum class AttributeAppId{
    Default = 0,
};

#define DECL_AID(group,app_id,attr,row,sub) ( ( std::uint8_t(group) & 0xff) | ( std::uint64_t(app_id) << 8) | ( std::uint64_t(attr) << 24) | ( std::uint64_t(row) << 32)  | ( std::uint64_t(sub) << 56)  )


// low byte means  bit width
enum  class AttributeValueType{
    String = 0,
    Int64 = 0x0108,
    Int32 = 0x0204 ,
    Int16 = 0x0302,
    Int8 =  0x0401,

    UInt64 = 0x0508,
    UInt32 = 0x0604 ,
    UInt16 = 0x0702,
    UInt8 =  0x0801,

    SDM_TimeTicks = 0x0904,
    SDM_Cert = 0x0a00,
    SDM_RowStatus = 0x0b01,
    SDM_AID = 0x0c08,

};




enum class ReadOrWriteType{
    Read = 0x01,
    Write = 0x10,
    ReadWrite = 0x11
};

typedef std::uint64_t  AttributeId;


struct AttributeDetail{
    AttributeId id;
    std::string name;
    AttributeValueType type;
    ReadOrWriteType  rw;
};

//template < typename T , typename V >
//struct AttributeValue{
//    V  type;
//    T  value;
//    void write(char * data );
////    static void read(const * data);
//};
//
//typedef AttributeValue<>

struct Attribute{
    typedef std::shared_ptr< Attribute> Ptr;
    AttributeId  aid;
    boost::any   value;
    void marshall( ByteStream & stream );
    static Attribute::Ptr  parse(ByteStream& stream);
};


struct AttributeTable{
    static AttributeDetail* get(AttributeId aid) ;
    static std::uint32_t  getAttributeValueLength(AttributeId aid);

};

#define AID_Vendor DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,1,0,0)
#define AID_Model DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,2,0,0)
#define AID_SN DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,3,0,0)
#define AID_SysVer DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,4,0,0)

#define AID_MgmVer DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,5,0,0)
#define AID_DevDesc DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,6,0,0)

#define AID_RunTimeTicks DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,7,0,0)
#define AID_Contact DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,8,0,0)
#define AID_DevName DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,9,0,0)

#define AID_DevPos DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,0x0a,0,0)
#define AID_ASYM_Algorithm DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,0x0b,0,0)
#define AID_SYM_Algorithm DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,0x0C,0,0)
#define AID_HASH_Algorithm DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,0x0d,0,0)

#define AID_GroupID DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,0x0e,0,1)
#define AID_GroupDesc DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,0x0e,0,2)
#define AID_GroupLastUpdateTime DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,0x0e,0,3)
#define AID_GroupControlStatus DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,0x0e,0,4)

#define AID_DeviceID DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,0x0f,0,0)
#define AID_CertApplication DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,0x11,0,0)

#define AID_CenterCert DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,0x12,0,0)
#define AID_ParentCert DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,0x13,0,0)
#define AID_MgtDeviceCert DECL_AID(AttributeGroupId::Basic,AttributeAppId::Default,0x14,0,0)

#define AID_IfNum DECL_AID(AttributeGroupId::Interface,AttributeAppId::Default,1,0,0)
#define AID_IfName DECL_AID(AttributeGroupId::Interface,AttributeAppId::Default,2,0,1)
#define AID_IfDesc DECL_AID(AttributeGroupId::Interface,AttributeAppId::Default,2,0,2)
#define AID_IfType DECL_AID(AttributeGroupId::Interface,AttributeAppId::Default,2,0,3)
#define AID_IfAddr DECL_AID(AttributeGroupId::Interface,AttributeAppId::Default,2,0,4)
#define AID_IfMTU DECL_AID(AttributeGroupId::Interface,AttributeAppId::Default,2,0,5)
#define AID_IfSpeed DECL_AID(AttributeGroupId::Interface,AttributeAppId::Default,2,0,6)
#define AID_IfInPacket DECL_AID(AttributeGroupId::Interface,AttributeAppId::Default,2,0,7)
#define AID_IfOutPacket DECL_AID(AttributeGroupId::Interface,AttributeAppId::Default,2,0,8)
#define AID_IfErrorPacket DECL_AID(AttributeGroupId::Interface,AttributeAppId::Default,2,0,9)
#define AID_IfUpDownTrapEnable DECL_AID(AttributeGroupId::Interface,AttributeAppId::Default,2,0,0x0a)
#define AID_IfTimeTicks DECL_AID(AttributeGroupId::Interface,AttributeAppId::Default,2,0,0x0b)
#define AID_IfRowStatus DECL_AID(AttributeGroupId::Interface,AttributeAppId::Default,2,0,0x0c)

#define AID_En_MgrRecvNum DECL_AID(AttributeGroupId::MGR_Entity,AttributeAppId::Default,1,0,0x0)
#define AID_En_MgrSentNum DECL_AID(AttributeGroupId::MGR_Entity,AttributeAppId::Default,2,0,0x0)
#define AID_En_MgrErrorNum DECL_AID(AttributeGroupId::MGR_Entity,AttributeAppId::Default,3,0,0x0)
#define AID_En_MgrSentAlarmNum DECL_AID(AttributeGroupId::MGR_Entity,AttributeAppId::Default,4,0,0x0)

#define AID_En_TrapOID DECL_AID(AttributeGroupId::MGR_Entity,AttributeAppId::Default,5,0,0x01)
#define AID_En_TrapLValue DECL_AID(AttributeGroupId::MGR_Entity,AttributeAppId::Default,5,0,0x02)
#define AID_En_TrapRValue DECL_AID(AttributeGroupId::MGR_Entity,AttributeAppId::Default,5,0,0x03)
#define AID_En_TrapSetTime DECL_AID(AttributeGroupId::MGR_Entity,AttributeAppId::Default,5,0,0x04)
#define AID_En_TrapMapControlStatus DECL_AID(AttributeGroupId::MGR_Entity,AttributeAppId::Default,5,0,0x05)

#endif //MONKEY_ATTRIBUTE_H
