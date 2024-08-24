#include <cstdint>

// Structure to represent a DNS query
typedef struct g_Query
{
    /*Queries*/
    const char *qname[100]; // Array to hold the query name
    int qtype;              // Query type (e.g., A, AAAA, MX)
    int qclass;             // Query class (usually IN for Internet)
};

// Structure to represent DNS flags
struct g_DNSFlags {
    std::uint16_t qr : 1;     // Query/Response flag
    std::uint16_t opcode : 4; // Operation code
    std::uint16_t aa : 1;     // Authoritative Answer flag
    std::uint16_t tc : 1;     // Truncation flag
    std::uint16_t rd : 1;     // Recursion Desired flag
    std::uint16_t ra : 1;     // Recursion Available flag
    std::uint16_t z : 3;      // Reserved for future use
    std::uint16_t rcode : 4;  // Response code
};

// Class to represent the DNS header
class Header {
    public:

    std::uint16_t tran_id;           // Transaction ID (16-bit)
    std::uint16_t flags;            // Flags (16-bit) to represent various control flags
    std::uint16_t question_count;   // Number of questions in the query
    std::uint16_t answer_count;     // Number of answers in the response
    std::uint16_t authority_count;  // Number of authority records in the response
    std::uint16_t additional_count; // Number of additional records in the response
    g_Query query;                    // Query structure to hold the query details

    // Constructor
    Header();

    // Function to set up a standard query with a given query ID
    void setStandardQery(std::uint16_t query_id);

    // Function to parse a DNS message from a buffer
    void parse(const std::uint8_t *buffer);

    // Function to serialize the DNS header into a buffer
    void serialize(std::uint8_t *buffer) const;

    // Function to print the DNS header for debugging purposes
    void print(std::uint8_t *buffer) const;
};