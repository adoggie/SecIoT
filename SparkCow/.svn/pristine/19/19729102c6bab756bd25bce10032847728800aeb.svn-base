//
// Created by scott on 7/30/20.
//

#include "attribute.h"
#include "serialize.h"
#include <map>


std::map< AttributeId , AttributeDetail> attr_map = {
        { AID_Vendor,{0,"Vendor",AttributeValueType::String,ReadOrWriteType::Read } },
        { AID_Model,{0,"Model",AttributeValueType::String,ReadOrWriteType::Read} },
        { AID_SN,{0,"SN",AttributeValueType::String,ReadOrWriteType::Read} },
        { AID_SysVer,{0,"SYSTEM_VER",AttributeValueType::String,ReadOrWriteType::Read} },
        { AID_MgmVer,{0,"MGR_VER",AttributeValueType::String,ReadOrWriteType::Read} },
        { AID_DevDesc,{0,"DEV_DESC",AttributeValueType::String,ReadOrWriteType::Read} },
        { AID_RunTimeTicks,{0,"RUN_TIME_TICKS",AttributeValueType::SDM_TimeTicks,ReadOrWriteType::Read} },
        { AID_Contact,{0,"CONTACT",AttributeValueType::SDM_TimeTicks,ReadOrWriteType::ReadWrite} },
        { AID_DevName,{0,"CONTACT",AttributeValueType::SDM_TimeTicks,ReadOrWriteType::ReadWrite} },
        { AID_DevPos,{0,"CONTACT",AttributeValueType::SDM_TimeTicks,ReadOrWriteType::ReadWrite} },
        { AID_ASYM_Algorithm,{0,"CONTACT",AttributeValueType::SDM_TimeTicks,ReadOrWriteType::ReadWrite} },
        { AID_SYM_Algorithm,{0,"CONTACT",AttributeValueType::SDM_TimeTicks,ReadOrWriteType::ReadWrite} },
        { AID_HASH_Algorithm,{0,"CONTACT",AttributeValueType::SDM_TimeTicks,ReadOrWriteType::ReadWrite} },
        { AID_GroupID,{0,"CONTACT",AttributeValueType::Int64,ReadOrWriteType::ReadWrite} },
        { AID_GroupDesc,{0,"CONTACT",AttributeValueType::String,ReadOrWriteType::ReadWrite} },
        { AID_GroupLastUpdateTime,{0,"CONTACT",AttributeValueType::SDM_TimeTicks,ReadOrWriteType::ReadWrite} },
        { AID_GroupControlStatus,{0,"CONTACT",AttributeValueType::SDM_RowStatus,ReadOrWriteType::ReadWrite} },
        { AID_DeviceID,{0,"CONTACT",AttributeValueType::String,ReadOrWriteType::ReadWrite} },
        { AID_CertApplication,{0,"CONTACT",AttributeValueType::String,ReadOrWriteType::ReadWrite} },

        { AID_CenterCert,{0,"CONTACT",AttributeValueType::String,ReadOrWriteType::ReadWrite} },
        { AID_ParentCert,{0,"CONTACT",AttributeValueType::String,ReadOrWriteType::ReadWrite} },
        { AID_MgtDeviceCert,{0,"CONTACT",AttributeValueType::String,ReadOrWriteType::ReadWrite} },

        // Interface
        { AID_IfNum,{0,"IF_NUM",AttributeValueType::Int32,ReadOrWriteType::ReadWrite} },
        { AID_IfName,{0,"IF_NAME",AttributeValueType::String,ReadOrWriteType::Read} },
        { AID_IfDesc,{0,"IF_DESC",AttributeValueType::String,ReadOrWriteType::Read} },
        { AID_IfType,{0,"IF_TYPE",AttributeValueType::Int32,ReadOrWriteType::Read} },
        { AID_IfAddr,{0,"IF_ADDR",AttributeValueType::String,ReadOrWriteType::Read} },
        { AID_IfMTU,{0,"IF_MTU",AttributeValueType::Int32,ReadOrWriteType::Read} },
        { AID_IfSpeed,{0,"IF_SPEED",AttributeValueType::Int64,ReadOrWriteType::Read} },
        { AID_IfInPacket,{0,"IF_IN_PACKET",AttributeValueType::Int64,ReadOrWriteType::Read} },
        { AID_IfOutPacket,{0,"IF_OUT_PACKET",AttributeValueType::Int64,ReadOrWriteType::Read} },
        { AID_IfErrorPacket,{0,"IF_ERROR_PACKET",AttributeValueType::Int64,ReadOrWriteType::Read} },
        { AID_IfUpDownTrapEnable,{0,"IF_UP_DOWN_TRAP_ENABLE",AttributeValueType::Int32,ReadOrWriteType::Read} },
        { AID_IfTimeTicks,{0,"IF_TIME_TICKS",AttributeValueType::SDM_TimeTicks,ReadOrWriteType::ReadWrite} },
        { AID_IfRowStatus,{0,"IF_RowStatus",AttributeValueType::SDM_RowStatus,ReadOrWriteType::ReadWrite} },

        // MgrEntity
        { AID_En_MgrRecvNum,{0,"MGR_RECV_NUM",AttributeValueType::Int32,ReadOrWriteType::Read} },
        { AID_En_MgrSentNum,{0,"MGR_SENT_NUM",AttributeValueType::Int32,ReadOrWriteType::Read} },
        { AID_En_MgrErrorNum,{0,"MGR_ERR_NUM",AttributeValueType::Int32,ReadOrWriteType::Read} },
        { AID_En_MgrSentAlarmNum,{0,"MGR_SENT_ALARM_NUM",AttributeValueType::Int32,ReadOrWriteType::Read} },

        { AID_En_TrapOID,{0,"TrapOid",AttributeValueType::SDM_AID,ReadOrWriteType::Read} },
        { AID_En_TrapLValue,{0,"TrapLValue",AttributeValueType::Int32,ReadOrWriteType::ReadWrite} },
        { AID_En_TrapRValue,{0,"TrapRValue",AttributeValueType::SDM_AID,ReadOrWriteType::ReadWrite} },
        { AID_En_TrapSetTime,{0,"TrapSetTime",AttributeValueType::SDM_TimeTicks,ReadOrWriteType::ReadWrite} },
        { AID_En_TrapMapControlStatus,{0,"TrapMapControlStatus",AttributeValueType::SDM_RowStatus,ReadOrWriteType::ReadWrite} },

};

AttributeDetail* AttributeTable::get(AttributeId aid) {

    auto it = attr_map.find(aid);
    if(it!=attr_map.end()){
        return &it->second;
    }
    return NULL;
}


std::uint32_t  AttributeTable::getAttributeValueLength(AttributeId aid){
    std::uint32_t  length =0;
    auto detail = AttributeTable::get( aid);
    return std::uint32_t (detail->type);
}


/// Attribute

Attribute::Ptr  Attribute::parse(ByteStream& stream){
    Attribute::Ptr  attr = std::make_shared<Attribute>();
    stream >> attr->aid;
    size_t val_len;
    val_len = stream.readUnsignedInt();

    auto type = AttributeTable::get( attr->aid )->type;

    if( type == AttributeValueType::String){
        attr->value = std::string(stream.data(), val_len);
        stream.skip(val_len);

    }else if (type == AttributeValueType::Int8){
        attr->value = stream.readByte();
    }else if (type == AttributeValueType::UInt8){
        attr->value = stream.readByte();
    }else if (type == AttributeValueType::Int16){
        attr->value = stream.readShort();
    }else if (type == AttributeValueType::UInt16){
        attr->value = stream.readShort();
    }else if (type == AttributeValueType::Int32){
        attr->value = stream.readInt();
    }else if (type == AttributeValueType::UInt32){
        attr->value = stream.readUnsignedInt();
    }else if (type == AttributeValueType::Int64){
        attr->value = stream.readInt64();
    }else if (type == AttributeValueType::UInt64){
        attr->value = stream.readUInt64();
    }

    return attr;
//    return Attribute::Ptr ();
}

void Attribute::marshall( ByteStream & stream ){
    auto type = AttributeTable::get( aid)->type;
    std::uint32_t len = std::uint32_t ( std::uint16_t (type)&0xff); // low 8 bit
    stream << aid ;


    if( type == AttributeValueType::String){
        auto ss = boost::any_cast<std::string>(value);
        len = ss.size();
        stream << len;
        stream.writeBytes(&*ss.begin() , ss.size());
    }else if (type == AttributeValueType::Int8){
        stream << 1 << boost::any_cast< std::int8_t>(value);
    }else if (type == AttributeValueType::UInt8){
        stream << 1 << boost::any_cast< std::uint8_t>(value);
    }else if (type == AttributeValueType::Int16){
        stream << 2 << boost::any_cast< std::int16_t>(value);
    }else if (type == AttributeValueType::UInt16){
        stream << 2 << boost::any_cast< std::uint16_t>(value);
    }else if (type == AttributeValueType::Int32){
        stream << 4 << boost::any_cast< std::int32_t>(value);
    }else if (type == AttributeValueType::UInt32){
        stream << 4 << boost::any_cast< std::uint32_t>(value);
    }else if (type == AttributeValueType::Int64){
        stream << 8 << boost::any_cast< std::int64_t>(value);
    }else if (type == AttributeValueType::UInt64){
        stream << 8 << boost::any_cast< std::uint64_t>(value);
    }
}
