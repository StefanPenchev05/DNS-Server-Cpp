#include "dns_message.hpp"
#include <cstring>
#include <arpa/inet.h>

// Constructor to initialize the DNS header with default values
Header::Header()
{
    this->tran_id = 0;
    this->flags = {0, 0, 0, 0, 0, 0, 0, 0}; // Initialize all flags to 0
    this->question_count = 0;
    this->answer_count = 0;
    this->authority_count = 0;
    this->additional_count = 0;
    memset(&this->query, 0, sizeof(this->query));
}

// Function to set the header for a standard query
void Header::setStandardQery(std::uint16_t query_id)
{
    this->tran_id = htons(query_id);
    this->flags.qr = 0;     // Query
    this->flags.opcode = 0; // Standard query
    this->flags.aa = 0;     // Not Authoritative
    this->flags.tc = 0;     // Not Truncated
    this->flags.rd = 1;     // Recursion Desired
    this->flags.ra = 0;     // Recursion not available
    this->flags.z = 0;      // Reserved
    this->flags.rcode = 0;  // No error
    this->question_count = htons(1); // One question
    this->answer_count = 0;
    this->authority_count = 0;
    this->additional_count = 0;
}

// Function to parse a DNS message from a buffer
void Header::parse(const std::uint8_t *buffer)
{
    this->tran_id = ntohs(*reinterpret_cast<const std::uint16_t *>(buffer));
    std::uint16_t flags = ntohs(*reinterpret_cast<const std::uint16_t *>(buffer + 2));
    this->flags.qr = (flags >> 15) & 0x1;
    this->flags.opcode = (flags >> 11) & 0xF;
    this->flags.aa = (flags >> 10) & 0x1;
    this->flags.tc = (flags >> 9) & 0x1;
    this->flags.rd = (flags >> 8) & 0x1;
    this->flags.ra = (flags >> 7) & 0x1;
    this->flags.z = (flags >> 4) & 0x7;
    this->flags.rcode = flags & 0xF;
    this->question_count = ntohs(*reinterpret_cast<const std::uint16_t *>(buffer + 4));
    this->answer_count = ntohs(*reinterpret_cast<const std::uint16_t *>(buffer + 6));
    this->authority_count = ntohs(*reinterpret_cast<const std::uint16_t *>(buffer + 8));
    this->additional_count = ntohs(*reinterpret_cast<const std::uint16_t *>(buffer + 10));
}

// Function to serialize the DNS header into a buffer
void Header::serialize(uint8_t *buffer) const
{
    *reinterpret_cast<std::uint16_t *>(buffer) = htons(this->tran_id);
    std::uint16_t flags = (this->flags.qr << 15) |
                          (this->flags.opcode << 11) |
                          (this->flags.aa << 10) |
                          (this->flags.tc << 9) |
                          (this->flags.rd << 8) |
                          (this->flags.ra << 7) |
                          (this->flags.z << 4) |
                          this->flags.rcode;
    *reinterpret_cast<std::uint16_t *>(buffer + 2) = htons(flags);
    *reinterpret_cast<std::uint16_t *>(buffer + 4) = htons(this->question_count);
    *reinterpret_cast<std::uint16_t *>(buffer + 6) = htons(this->answer_count);
    *reinterpret_cast<std::uint16_t *>(buffer + 8) = htons(this->authority_count);
    *reinterpret_cast<std::uint16_t *>(buffer + 10) = htons(this->additional_count);
}