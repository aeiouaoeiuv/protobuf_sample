#include <string>
#include <iostream>
#include "header.pb.h"
#include "register.pb.h"

using namespace std;

int SetInfo(pkgRegister::RegisterReq *req, pkgRegister::RegisterAck *ack)
{
    // header
    pkgHeader::Header *header = req->mutable_header();
    header->set_msgcode(111);

    // int32
    req->set_int32_id(222);

    // sint32,数值可能有负数的话使用sint32类型
    req->set_sint32_id(-333);

    // double
    req->set_double_id(4.444444444);

    // float
    req->set_float_id(5.555555555555555);

    // bool
    req->set_bool_flag(true);

    // string
    req->set_string_name((char *)"mike", 4);

    // bytes
    char imei[8] = {0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18};
    req->set_bytes_data(imei, sizeof(imei));

    // optional string
    // req->set_optional_str((char *)"www", 3);// 此处不设置值,解析时则拿到空字符串

    // 有默认值的optional string
    // req->set_optional_default_str((char *)"www.qq.com", 10);// 此处不设置值,则使用proto文件中默认填入的string

    // 枚举
    req->set_enum_corpus(pkgRegister::RegisterReq_Corpus_IMAGES);

    // repeated字符串
    req->add_repeated_string((char *)"repeatedString1", 15);
    req->add_repeated_string((char *)"repeatedString2", 15);

    // oneof
    req->set_oneof_name((char *)"oneof_name_str", 14);

    //////////////////////////////////////////另一个消息的//////////////////////////////////////////

    // repeated消息
    pkgRegister::RegisterReq::NestedMessage *nestedMessage = NULL;
    nestedMessage = ack->add_nested_message();// 第一个数组元素
    nestedMessage->set_nested_id(100);
    nestedMessage->set_nested_name((char *)"100_name", 8);

    nestedMessage = ack->add_nested_message();// 第二个数组元素
    nestedMessage->set_nested_id(200);
    nestedMessage->set_nested_name((char *)"200_name", 8);

    return 0;
}

int GetInfo(string *reqStr, string *ackStr)
{
    pkgRegister::RegisterReq registerReq;
    pkgRegister::RegisterAck registerAck;

    if (registerReq.ParseFromString(*reqStr)) {
        // header
        pkgHeader::Header *header = registerReq.mutable_header();
        printf("header: msgcode=%d\n\n", header->msgcode());

        // int32
        printf("int32_id=%d\n\n", registerReq.int32_id());

        // sint32,数值可能有负数的话使用sint32类型
        printf("sint32_id=%d\n\n", registerReq.sint32_id());

        // double
        printf("double_id=%f\n\n", registerReq.double_id());

        // float
        printf("float_id=%f\n\n", registerReq.float_id());

        // bool
        printf("bool_flag=%d\n\n", registerReq.bool_flag());

        // string
        printf("string_name=%s, %s\n\n", registerReq.string_name().c_str(), registerReq.mutable_string_name()->c_str());// string_name()返回的是const,用mutable_修饰则不是const

        // bytes
        char imei[8];
        memcpy(imei, registerReq.bytes_data().data(), registerReq.bytes_data().length());
        printf("bytes_data=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n\n", imei[0], imei[1], imei[2], imei[3], imei[4], imei[5], imei[6], imei[7]);

        // optional string
        printf("optional_str=>>>%s<<<, length=%lu\n\n", registerReq.optional_str().c_str(), registerReq.optional_str().length());// 调用设置函数时未设置该串,默认返回空字符串

        // 有默认值的optional string
        printf("optional_default_str=%s, length=%lu\n\n", registerReq.optional_default_str().c_str(), registerReq.optional_default_str().length());// 默认使用proto文件中的字符串

        // 枚举
        printf("enum_corpus=%d\n\n", registerReq.enum_corpus());

        // repeated字符串
        for (int i = 0; i < registerReq.repeated_string_size(); i++) {
            printf("repeated_string[%d]=%s\n", i, registerReq.repeated_string(i).c_str());
        }
        printf("\n");

        // oneof
        printf("oneof_name=%s, has_oneof_name=%d\n", registerReq.oneof_name().c_str(), registerReq.has_oneof_name());
        printf("oneof_email=%s, has_oneof_email=%d\n\n", registerReq.oneof_email().c_str(), registerReq.has_oneof_email());
    }
    else {
        printf("parsing request string failed\n");
    }

    //////////////////////////////////////////另一个消息的//////////////////////////////////////////

    if (registerAck.ParseFromString(*ackStr)) {
        // repeated消息
        for (int i = 0; i < registerAck.nested_message_size(); i++) {
            printf("nested_id[%d]=%d\n", i, registerAck.nested_message(i).nested_id());
            printf("nested_name[%d]=%s\n", i, registerAck.nested_message(i).nested_name().c_str());
        }
    }
    else {
        printf("parsing response string failed\n");
    }

    return 0;
}

int main(int argc, char *argv[])
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    pkgRegister::RegisterReq registerReq;
    pkgRegister::RegisterAck registerAck;
    string reqStr;
    string ackStr;

    // 设置数据
    SetInfo(&registerReq, &registerAck);

    // 将数据序列化成string
    registerReq.SerializeToString(&reqStr);
    registerAck.SerializeToString(&ackStr);

    // 打印str的大小
    printf("reqStr.size()=%lu, ackStr.size()=%lu\n", reqStr.size(), ackStr.size());
    // 打印序列化后的大小
    printf("registerReq.ByteSize()=%d, registerAck.ByteSize()=%d\n\n", registerReq.ByteSize(), registerAck.ByteSize());

    // 获取数据
    GetInfo(&reqStr, &ackStr);

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}

