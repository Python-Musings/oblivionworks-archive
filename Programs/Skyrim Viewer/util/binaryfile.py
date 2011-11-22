import struct
import types

# Easy unpackers
Byte = struct.Struct('b')
Int16 = struct.Struct('h')
Int32 = struct.Struct('i')
Int64 = struct.Struct('q')
UByte = struct.Struct('B')
UInt16 = struct.Struct('H')
UInt32 = struct.Struct('I')
UInt64 = struct.Struct('Q')
Float = struct.Struct('f')

# Custom unpackers
class CustomStruct(object):
    pass

class StrippedStruct(CustomStruct):
    def __init__(self,strip='\x00'):
        self._strip = strip

    def strip(self,string):
        return string.strip(self._strip)

class NETStringStruct(StrippedStruct):
    def unpack(self,file):
        pos = file.tell()
        strLen = bfRead(file,UByte)
        if strLen >= 128:
            self.seek(pos)
            strLen = bfRead(file,UInt16)
            strLen = strLen & 0x7F | (strLen >> 1) & 0xFF80
            if strLen > 0x7FFF:
                raise Exception, "String too long to convert."
        return self.strip(file.read(strLen))

    def pack(self,file,string):
        strLen = len(string)
        if strLen < 128:
            bfWrite(file,UByte,strLen)
        elif strLen > 0x7FFF:
            raise Exception, "String too long to convert."
        else:
            strLen = 0x80 | strLen & 0x7F | (strLen & 0xFF80) << 1
            bfWrite(file,UInt16,strLen)
        file.write(string)

class CStringStruct(StrippedStruct):
    def unpack(self,file):
        start = file.tell()
        while bfRead(file,UByte) != 0: pass
        strLen = file.tell() - start - 1
        file.seek(start)
        string = file.read(strLen)
        bfRead(file,UByte) # Lop off the NULL terminator
        return self.strip(string)

    def pack(self,file,string):
        file.write(string)
        bfWrite(file,UByte,0)

class PascalStringStruct(StrippedStruct):
    def __init__(self,sizeSize=UByte,*args):
        StrippedStruct.__init__(self,*args)
        self.sizeSize = sizeSize

    def unpack(self,file):
        return self.strip(file.read(bfRead(file,self.sizeSize)))

    def pack(self,file,string):
        numBytes = self.sizeSize.size
        maxLen = (1 << numBytes) - 1
        strLen = min(len(string),maxLen)
        bfWrite(file,self.sizeSize,strLen)
        file.write(string[:strLen])

# Custom unpackers (with NULL characters stripped)
NETString = NETStringStruct()
CString = CStringStruct()
PascalString = PascalStringStruct(UByte)

# Raw versions (with NULL characters intact)
NETStringRaw = NETStringStruct('')
CStringRaw = CStringStruct('')
PascalStringRaw = PascalStringStruct(UByte,'')

def bfUnpack(ins,format):
    return struct.unpack(format,ins.read(struct.calcsize(format)))
def bfPack(out,format,*data):
    out.write(struct.pack(format,*data))
def bfRead(ins,unpacker):
    if isinstance(unpacker,struct.Struct):
        ret = unpacker.unpack(ins.read(unpacker.size))
        if len(ret) == 1: return ret[0]
        else: return ret
    elif isinstance(unpacker, CustomStruct):
        return unpacker.unpack(ins)
    else:
        return ins.read(unpacker)

class BinaryFile(file):
    def __init__(self,*args,**kwdargs):
        # Ensure we're reading/writing in binary mode
        if len(args) < 2:
            mode = kwdargs.get('mode',None)
            if mode =='r': mode = 'rb'
            elif mode == 'w': mode = 'wb'
            elif mode == 'rb' or mode == 'wb':
                pass
            else: mode = 'rb'
            kwdargs['mode'] = mode
        else:
            new_args = list(args)
            if args[1] == 'r': new_args[1] == 'rb'
            elif args[1] == 'w': new_args[1] == 'wb'
            elif args[1] == 'rb' or args[1] == 'wb':
                pass
            else: new_args[1] = 'rb'
            args = tuple(new_args)
        super(BinaryFile,self).__init__(*args,**kwdargs)
        #types.FileType.__init__(self,*args,**kwdargs)

    def unpack(self,format):
        """Reads and unpacks according to format."""
        return struct.unpack(format,self.read(struct.calcsize(format)))

    def pack(self,format,*data):
        """Packs data according to format and writes."""
        self.write(struct.pack(format,*data))

    def read(self,unpacker):
        """If unpacker is a struct.Struct, reads data according
           to the object.  Otherwise, reads 'unpacker' bytes."""
        if isinstance(unpacker, struct.Struct):
            ret = unpacker.unpack(super(BinaryFile,self).read(unpacker.size))
            if len(ret) == 1: return ret[0]
            else: return ret
        elif isinstance(unpacker, CustomStruct):
            return unpacker.unpack(self)
        else:
            return super(BinaryFile,self).read(unpacker)

    def write(self,packer,*data):
        """If packer is a struct.Struct, packs data according to
           the object and writes.  Otherwise, writes 'unpacker'."""
        if isinstance(packer, struct.Struct):
            super(BinaryFile,self).write(packer.pack(*data))
        elif isinstance(packer, CustomStruct):
            packer.pack(self,*data)
        else:
            super(BinaryFile,self).write(packer)