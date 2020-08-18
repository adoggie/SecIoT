//
// Created by scott on 8/4/20.
//

#include "ComplianceTest.h"

void CommandComplianceTestRequest::marshall( ByteStream& stream){
    Command::marshall(stream);
    stream.writeBytes(device_id.data(),device_id.size());
    stream.writeByte(direction);
    stream.writeByte(schema.algorithm);
    stream.writeByte(schema.mode);
    stream.writeByte(schema.cipher);
    stream.writeByte(schema.compose);
    stream.writeUnsignedInt(len);
    stream.writeBytes(data.data(),data.size());
}

CommandComplianceTestRequest::Ptr CommandComplianceTestRequest::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandComplianceTestRequest>();
    stream.skip(1);
    stream.readBytes( cp->device_id.data(),cp->device_id.size());
    cp->direction = stream.readByte();
    cp->schema.algorithm = stream.readByte();
    cp->schema.mode = stream.readByte();
    cp->schema.cipher = stream.readByte();
    cp->schema.compose = stream.readByte();
    cp->len = stream.readUnsignedInt();
    cp->data.resize(cp->len);
    stream.readBytes( cp->data.data() , cp->data.size());
    return cp;
}

void CommandComplianceTestResponse::marshall( ByteStream& stream){
    Command::marshall(stream);
    stream.writeBytes(device_id.data(),device_id.size());
    stream.writeByte(direction);
    stream.writeByte(check);
    stream.writeByte(schema.algorithm);
    stream.writeByte(schema.mode);
    stream.writeByte(schema.cipher);
    stream.writeByte(schema.compose);
    stream.writeUnsignedInt(len_or_data);
    stream.writeBytes(data.data(),data.size());
}

CommandComplianceTestResponse::Ptr CommandComplianceTestResponse::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandComplianceTestResponse>();
    stream.skip(1);
    stream.readBytes( cp->device_id.data(),cp->device_id.size());
    cp->direction = stream.readByte();
    cp->schema.algorithm = stream.readByte();
    cp->schema.mode = stream.readByte();
    cp->schema.cipher = stream.readByte();
    cp->schema.compose = stream.readByte();
    cp->len_or_data = stream.readUnsignedInt();
    cp->data.resize(cp->len_or_data);
    stream.readBytes( cp->data.data() , cp->data.size());
    return cp;
}


