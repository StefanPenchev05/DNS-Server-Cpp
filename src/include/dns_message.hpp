#include <cstdint>

typedef struct g_Question
{
    char *qname[100];
    int qtype;
    int qclass;
};

struct g_DNSFlags {
    std::uint16_t qr : 1;
    std::uint16_t opcode : 4;
    std::uint16_t aa : 1;
    std::uint16_t tc : 1;
    std::uint16_t rd : 1;
    std::uint16_t ra : 1;
    std::uint16_t z : 3;
    std::uint16_t rcode : 4;

    // Function to convert the flag struct to a 16-bit value
    std::uint16_t toUint16() const {
         return (qr << 15) | (opcode << 11) | (aa << 10) | 
               (tc << 9) | (rd << 8) | (ra << 7) | (z << 4) | rcode;
    }

    // Function to set flags from a 16-bit value
    void fromUint16(std::uint16_t value) {
        qr = (value >> 15) & 0x01;
        opcode = (value >> 11) & 0x0F;
        aa = (value >> 10) & 0x01;
        tc = (value >> 9) & 0x01;
        rd = (value >> 8) & 0x01;
        ra = (value >> 7) & 0x01;
        z = (value >> 4) & 0x07;
        rcode = value & 0x0F;
    }
};

class Header {
    public:

    std::uint16_t tran_id;
    g_DNSFlags flags;
    std::uint16_t question_count;
    std::uint16_t answer_count;
    std::uint16_t authority_count;
    std::uint16_t additional_count;
    g_Question question;

    // Constructor
    Header();

    // Function to set up a standard question with a given question ID
    void setStandardQery(std::uint16_t query_id);

    // Function to parse a DNS message from a buffer
    void parse(const std::uint8_t *buffer);

    // Function to serialize the DNS header into a buffer
    void serialize(std::uint8_t *buffer) const;

    // Function to print the DNS header for debugging purposes
    void print(std::uint8_t *buffer) const;
};