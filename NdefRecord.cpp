#include "NdefRecord.h"

#include "internal/Ndef.h"

NdefRecord::NdefRecord()
{
    DMSG_STR("NdefRecord Constructor 1");
    _tnf = 0;
    _typeLength = 0;
    _payloadLength = 0;
    _idLength = 0;
    _type = (byte *)NULL;
    _payload = (byte *)NULL;
    _id = (byte *)NULL;
}

NdefRecord::NdefRecord(const NdefRecord& rhs)
{
    DMSG_STR("NdefRecord Constructor 2 (copy)");

    _tnf = rhs._tnf;
    _typeLength = rhs._typeLength;
    _payloadLength = rhs._payloadLength;
    _idLength = rhs._idLength;
    _type = (byte *)NULL;
    _payload = (byte *)NULL;
    _id = (byte *)NULL;

    if (_typeLength)
    {
        _type = (byte*)malloc(_typeLength);
        memcpy(_type, rhs._type, _typeLength);
    }

    if (_payloadLength)
    {
        _payload = (byte*)malloc(_payloadLength);
        memcpy(_payload, rhs._payload, _payloadLength);
    }

    if (_idLength)
    {
        _id = (byte*)malloc(_idLength);
        memcpy(_id, rhs._id, _idLength);
    }

}

// TODO NdefRecord::NdefRecord(tnf, type, payload, id)

NdefRecord::~NdefRecord()
{
	DMSG_STR("NdefRecord Destructor");
    if (_typeLength)
    {
        free(_type);
    }

    if (_payloadLength)
    {
        free(_payload);
    }

    if (_idLength)
    {
        free(_id);
    }
}

NdefRecord& NdefRecord::operator=(const NdefRecord& rhs)
{
	DMSG_STR("NdefRecord ASSIGN");

    if (this != &rhs)
    {
        // free existing
        if (_typeLength)
        {
            free(_type);
        }

        if (_payloadLength)
        {
            free(_payload);
        }

        if (_idLength)
        {
            free(_id);
        }

        _tnf = rhs._tnf;
        _typeLength = rhs._typeLength;
        _payloadLength = rhs._payloadLength;
        _idLength = rhs._idLength;

        if (_typeLength)
        {
            _type = (byte*)malloc(_typeLength);
            memcpy(_type, rhs._type, _typeLength);
        }

        if (_payloadLength)
        {
            _payload = (byte*)malloc(_payloadLength);
            memcpy(_payload, rhs._payload, _payloadLength);
        }

        if (_idLength)
        {
            _id = (byte*)malloc(_idLength);
            memcpy(_id, rhs._id, _idLength);
        }
    }
    return *this;
}

// size of records in bytes
int NdefRecord::getEncodedSize()
{
    int size = 2; // tnf + typeLength
    if (_payloadLength > 0xFF)
    {
        size += 4;
    }
    else
    {
        size += 1;
    }

    if (_idLength)
    {
        size += 1;
    }

    size += (_typeLength + _payloadLength + _idLength);

    return size;
}

void NdefRecord::encode(byte *data, bool firstRecord, bool lastRecord)
{
    // assert data > getEncodedSize()

    uint8_t* data_ptr = &data[0];

    *data_ptr = getTnfByte(firstRecord, lastRecord);
    data_ptr += 1;

    *data_ptr = _typeLength;
    data_ptr += 1;

    if (_payloadLength <= 0xFF) {  // short record
        *data_ptr = _payloadLength;
        data_ptr += 1;
    } else { // long format
        // 4 bytes but we store length as an int
        data_ptr[0] = 0x0; // (_payloadLength >> 24) & 0xFF;
        data_ptr[1] = 0x0; // (_payloadLength >> 16) & 0xFF;
        data_ptr[2] = (_payloadLength >> 8) & 0xFF;
        data_ptr[3] = _payloadLength & 0xFF;
        data_ptr += 4;
    }

    if (_idLength)
    {
        *data_ptr = _idLength;
        data_ptr += 1;
    }

    memcpy(data_ptr, _type, _typeLength);
    data_ptr += _typeLength;

    memcpy(data_ptr, _payload, _payloadLength);
    data_ptr += _payloadLength;

    if (_idLength)
    {
        memcpy(data_ptr, _id, _idLength);
        data_ptr += _idLength;
    }
}

byte NdefRecord::getTnfByte(bool firstRecord, bool lastRecord)
{
    int value = _tnf;

    if (firstRecord) { // mb
        value = value | 0x80;
    }

    if (lastRecord) { //
        value = value | 0x40;
    }

    // chunked flag is always false for now
    // if (cf) {
    //     value = value | 0x20;
    // }

    if (_payloadLength <= 0xFF) {
        value = value | 0x10;
    }

    if (_idLength) {
        value = value | 0x8;
    }

    return value;
}

byte NdefRecord::getTnf()
{
    return _tnf;
}

void NdefRecord::setTnf(byte tnf)
{
    _tnf = tnf;
}

unsigned int NdefRecord::getTypeLength()
{
    return _typeLength;
}

int NdefRecord::getPayloadLength()
{
    return _payloadLength;
}

unsigned int NdefRecord::getIdLength()
{
    return _idLength;
}

String NdefRecord::getType()
{
    char type[_typeLength + 1];
    memcpy(type, _type, _typeLength);
    type[_typeLength] = '\0'; // null terminate
    return String(type);
}

// this assumes the caller created type correctly
void NdefRecord::getType(uint8_t* type)
{
    memcpy(type, _type, _typeLength);
}

void NdefRecord::setType(const byte * type, const unsigned int numBytes)
{
    if(_typeLength)
    {
        free(_type);
    }

    _type = (uint8_t*)malloc(numBytes);
    memcpy(_type, type, numBytes);
    _typeLength = numBytes;
}

// assumes the caller sized payload properly
void NdefRecord::getPayload(byte *payload)
{
    memcpy(payload, _payload, _payloadLength);
}

void NdefRecord::setPayload(const byte * payload, const int numBytes)
{
    if (_payloadLength)
    {
        free(_payload);
    }

    _payload = (byte*)malloc(numBytes);
    memcpy(_payload, payload, numBytes);
    _payloadLength = numBytes;
}

String NdefRecord::getId()
{
    char id[_idLength + 1];
    memcpy(id, _id, _idLength);
    id[_idLength] = '\0'; // null terminate
    return String(id);
}

void NdefRecord::getId(byte *id)
{
    memcpy(id, _id, _idLength);
}

void NdefRecord::setId(const byte * id, const unsigned int numBytes)
{
    if (_idLength)
    {
        free(_id);
    }

    _id = (byte*)malloc(numBytes);
    memcpy(_id, id, numBytes);
    _idLength = numBytes;
}

void NdefRecord::print()
{
#ifdef DEBUG
    DMSG_STR(F("  NDEF Record"));
    DMSG(F("    TNF 0x"));DMSG_HEX(_tnf);DMSG(" ");
    switch (_tnf) {
    case TNF_EMPTY:
        DMSG_STR(F("Empty"));
        break;
    case TNF_WELL_KNOWN:
    	DMSG_STR(F("Well Known"));
        break;
    case TNF_MIME_MEDIA:
    	DMSG_STR(F("Mime Media"));
        break;
    case TNF_ABSOLUTE_URI:
    	DMSG_STR(F("Absolute URI"));
        break;
    case TNF_EXTERNAL_TYPE:
    	DMSG_STR(F("External"));
        break;
    case TNF_UNKNOWN:
    	DMSG_STR(F("Unknown"));
        break;
    case TNF_UNCHANGED:
    	DMSG_STR(F("Unchanged"));
        break;
    case TNF_RESERVED:
    	DMSG_STR(F("Reserved"));
        break;
    default:
    	DMSG_STR("");
    }
    DMSG(F("    Type Length 0x"));DMSG_HEX(_typeLength);DMSG(" ");DMSG_INT(_typeLength);DMSG_STR("");
    DMSG(F("    Payload Length 0x"));DMSG_HEX(_payloadLength);DMSG(" ");DMSG_INT(_payloadLength);DMSG_STR("");
    if (_idLength)
    {
        NDEF_DMSG_HEX(F("    Id Length 0x"), _idLength);
    }
    DMSG(F("    Type "));PrintHexChar(_type, _typeLength);
    // TODO chunk large payloads so this is readable
    DMSG(F("    Payload "));PrintHexChar(_payload, _payloadLength);
    if (_idLength)
    {
        DMSG(F("    Id "));PrintHexChar(_id, _idLength);
    }
    DMSG(F("    Record is"));DMSG_INT(getEncodedSize());DMSG_STR(" bytes");
#endif // DEBUG
}
