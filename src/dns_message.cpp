#include "dns_message.hpp"
#include <cstring>
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
    memset(&this->query, 0, sizeof(this->query));
}

// Function to set the header for a standard query
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
    this->question_count = htons(1); // One question
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
}
