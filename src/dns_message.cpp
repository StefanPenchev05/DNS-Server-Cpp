#include "dns_message.hpp"
#include <cstring>
#include <string>
#include <arpa/inet.h>

// Constructor to initialize the DNS header with default values
Header::Header()
{
    this->tran_id = 0;
    this->flags = {0};
    this->question_count = 0;
    this->additional_count = 0;
    this->authority_count = 0;
    this->additional_count = 0;
    memset(&this->question, 0, sizeof(this->question));
}

// Function to set the header for a standard question
void Header::setStandardQery(std::uint16_t query_id)
{
    this->tran_id = htons(query_id);
    this->flags = {
        .qr = 1,
        .opcode = 0,
        .aa = 0,
        .tc = 0,
        .rd = 0,
        .ra = 0,
        .z = 0,
        .rcode = 0};
    this->question_count = htons(1);
    this->answer_count = 0;
    this->authority_count = 0;
    this->additional_count = 0;
}

// Function to parse a DNS message from a buffer
void Header::parse(const std::uint8_t *buffer)
{
    this->tran_id = ntohs(*reinterpret_cast<const std::uint16_t *>(buffer));
    this->flags.fromUint16(ntohs(*reinterpret_cast<const std::uint16_t *>(buffer + 2)));
    this->question_count = ntohs(*reinterpret_cast<const std::uint16_t *>(buffer + 4));
    this->answer_count = ntohs(*reinterpret_cast<const std::uint16_t *>(buffer + 6));
    this->authority_count = ntohs(*reinterpret_cast<const std::uint16_t *>(buffer + 8));
    this->additional_count = ntohs(*reinterpret_cast<const std::uint16_t *>(buffer + 10));

    const std::uint8_t *qname_ptr = buffer + 12;
    int label_index = 0;

    while (*qname_ptr != 0)
    {                                    
        int label_length = *qname_ptr++;

        this->question.qname[label_index] = new char[label_length + 1];
        std::memcpy(this->question.qname[label_index], qname_ptr, label_length);

        this->question.qname[label_index][label_length] = '\0'; 
        label_index++;
        qname_ptr += label_length; 
    }

    this->question.qtype = ntohs(*reinterpret_cast<const std::uint16_t *>(qname_ptr + 1));
    this->question.qclass = ntohs(*reinterpret_cast<const std::uint16_t *>(qname_ptr + 3));
}

// Function to serialize the DNS header into a buffer
void Header::serialize(uint8_t *buffer) const
{
    *reinterpret_cast<std::uint16_t *>(buffer) = htons(this->tran_id);
    *reinterpret_cast<std::uint16_t *>(buffer + 2) = htons(this->flags.toUint16());
    *reinterpret_cast<std::uint16_t *>(buffer + 4) = htons(this->question_count);
    *reinterpret_cast<std::uint16_t *>(buffer + 6) = htons(this->answer_count);
    *reinterpret_cast<std::uint16_t *>(buffer + 8) = htons(this->authority_count);
    *reinterpret_cast<std::uint16_t *>(buffer + 10) = htons(this->additional_count);

    // Serialize the question
    uint8_t *qname_ptr = buffer + 12;
    int offset = 0;

    for(int i = 0; i < 100 && this->question.qname[i] != nullptr; i++){
        const char *label = this->question.qname[i];
        size_t label_length = strlen(label);
        
        qname_ptr[offset] = static_cast<uint8_t>(label_length);
        offset += 1;
        std::memcpy(qname_ptr + offset, label, label_length);    
        offset += label_length;
    }

    qname_ptr[offset] = 0;
    offset += 1;
    
    *reinterpret_cast<std::uint16_t *>(qname_ptr + offset) = htons(this->question.qtype);
    offset += 2;
    
    // Serialize QCLASS
    *reinterpret_cast<std::uint16_t *>(qname_ptr + offset) = htons(this->question.qclass);
    offset += 2;
    
}
