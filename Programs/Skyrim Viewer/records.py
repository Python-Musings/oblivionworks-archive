# For reading/writing record definitions
import ConfigParser
import os
from util.path import GPath
from util.flags import Flags
import util.binaryfile as bf

class StringIdLookup(object):
    def __init__(self):
        self.SetStrings()

    def SetStrings(self,strings={}):
        self.strings = strings

    def Lookup(self,id):
        return self.strings.get(id)

    def EvalStringId(self,id):
        if not id:
            return ''
        elif id in self.strings:
            return "[%08X] '%s'" % (id,self.strings[id])
        else:
            return '[%08X] ERROR: cannot resolve StringId' % id
StringTable = StringIdLookup()

class FormIdLookup(object):
    def __init__(self):
        self.SetData()
        self.SetPlugin()
        self.SetPlugins()

    def SetData(self,data={}):
        self.data = data

    def SetPlugin(self,plugin=None):
        self.plugin = plugin

    def SetPlugins(self,plugins=[]):
        self.plugins = plugins

    def ShortToLongFid(self,formId):
        """returns (shortModId,longModId,recordId)"""
        masters = self.data[self.plugin]['masters']
        modIndex = formId >> 24
        recordId = formId & 0xFFFFFF
        if modIndex > len(masters):
            return (modIndex,None,recordId)
        elif modIndex == len(masters):
            master = self.plugin
        else:
            master = masters[modIndex]
        modId = self.plugins.index(master)
        return (modIndex,modId,recordId)

    def FindWinningRecord(self,shortId,longId,recordId):
        if self.plugin.cext == '.ess':
            thisId = 0xFF
        else:
            thisId = self.plugins.index(self.plugin)

        def _Eval(file):
            data = self.data[file]
            formId = (shortId<<24) | recordId
            if formId in data:
                recData = data[formId]
                recType = recData[0]
                edid = ''
                for subType,subData in recData[1:]:
                    if subType == 'EDID':
                        edid = bf.bfRead(subData,bf.CString)
                        subData.seek(0)
                        break
                masterIdex = self.plugins.index(file)
                return (recType,edid,masterIdex,file)
            return None

        masters = self.data[self.plugin]['masters']
        for file in reversed(masters):
            if file in self.data:
                ret = _Eval(file)
                if ret: return ret
        ret = _Eval(self.plugin)
        if ret: return ret
        if longId == thisId:
            return (None,None,thisId,self.plugin)
        master = masters[shortId]
        masterIdex = self.plugins.index(master)
        return (None,None,masterIdex,master)

    def EvalFormId(self,formId):
        shortId,longId,recordId = self.ShortToLongFid(formId)
        if longId is None:
            return '[XX%06X] ERROR: Mod index out of bounds [%02X]' % (
                recordId,shortId)

        recType,edid,masterIdex,master = self.FindWinningRecord(shortId,longId,recordId)
        if recType:
            return "[%02X%06X] %s record in '[%02X] %s': %s" % (
                longId,recordId,recType,masterIdex,master.s,edid)
        if master not in self.data:
            return "[%02X%06X] Cannot resolve - '[%02X] %s' is not loaded" % (
                longId,recordId,masterIdex,master.s)
        return '[%02X%06X] Record not found' % (longId,recordId)
FormIdTable = FormIdLookup()

class TypeArray(bf.CustomStruct):
    def __init__(self,unpacker,Eval):
        self.unpacker = unpacker
        if Eval:
            self._Eval = Eval
        else:
            self.Eval = None

    def unpack(self,ins):
        pos = ins.tell()
        ins.seek(0,os.SEEK_END)
        eof = ins.tell()
        ins.seek(pos)

        ret = []
        while ins.tell() < eof:
            pos = ins.tell()
            try:
                ret.append(bf.bfRead(ins,self.unpacker))
            except:
                ins.seek(pos)
                raise
        return ret

    def Eval(self,value):
        return [self._Eval(x) for x in value]
CStringA = TypeArray(bf.CString,None)
FormIdA = TypeArray(bf.UInt32,FormIdTable.EvalFormId)

class LongStringArray(bf.CustomStruct):
    def unpack(self,ins):
        numStrings = bf.bfRead(ins,bf.UInt32)
        strLens = []
        for x in xrange(numStrings):
            strLens.append(bf.bfRead(ins,bf.UInt32))
        ret = []
        for x,strLen in enumerate(strLens):
            if strLen:
                ret.append(unicode(ins.read(strLen).strip('\x00'),'cp1252'))
            else:
                ret.append(unicode(ins.read().strip('\x00'),'cp1252'))
        return ret

LStringA = LongStringArray()

Types = {
    #type: (unpacker,required_size,eval_function,display_format)
    'cstring': (bf.CString,None,None,None),
    'pstring': (bf.PascalString,None,None,None),
    'bstring': (bf.PascalStringStruct(bf.UInt16),None,None,None),
    'lstring': (bf.PascalStringStruct(bf.UInt32),None,None,None),
    'lstringa': (LStringA,None,None,None),
    'uint16': (bf.UInt16,2,None,None),
    'int16': (bf.Int16,2,None,None),
    'uint32': (bf.UInt32,4,None,None),
    'int32': (bf.Int32,4,None,None),
    'uint64': (bf.UInt64,8,None,None),
    'int64': (bf.Int64,8,None,None),
    'ubyte': (bf.UByte,1,None,None),
    'byte': (bf.Byte,1,None,None),
    'float': (bf.Float,4,lambda x: round(x,6),None),
    'stringid': (bf.UInt32,4,StringTable.EvalStringId,None),
    'formid': (bf.UInt32,4,FormIdTable.EvalFormId,None),
    'formida': (FormIdA,None,FormIdA.Eval,None),
    'raw': (-1,None,lambda x:' '.join('%02X' % ord(c) for c in x),None),
    }


class RecordDef(dict):
    globalConfig = None

    def __init__(self,*args,**kwdargs):
        super(RecordDef,self).__init__(*args,**kwdargs)
        self.changed = False

    
    def load(self,path):
        self.clear()
        try:
            config = ConfigParser.RawConfigParser()
            config.read(GPath(path).s)

            for section in config.sections():
                if '.' in section:
                    subType,Name = section.split('.')
                    subType = subType.strip()
                    Name = Name.strip()
                else:
                    subType = section.strip()
                    Name = ''
                if len(subType) == 4:
                    subType = subType.upper()

                Type,Format,flags = None,None,None
                if config.has_option(section,'Type'):
                    value = config.get(section,'Type').strip().lower()
                    if value in Types:
                        Type = value
                if config.has_option(section,'Format'):
                    Format = config.get(section,'Format').strip()
                if config.has_option(section,'Name'):
                    Name = config.get(section,'Name').strip()
                if config.has_option(section,'Flags'):
                    names = config.get(section,'Flags')
                    names = [x.strip() for x in names.split(',')]
                    if len(names) <= 8:
                        Type = 'ubyte'
                        num = 8
                    elif len(names) <= 16:
                        Type = 'uint16'
                        num = 16
                    elif len(names) <= 32:
                        type = 'uint32'
                        num = 32
                    elif len(names) <= 64:
                        type = 'uint64'
                        num = 64
                    else:
                        continue
                    if len(names) < num:
                        names.extend(['' for x in xrange(num-len(names))])
                    names = [(x,'unk%i'%i)[not x] for i,x in enumerate(names)]
                    flags = Flags(0,*names)
                self.setdefault(subType,[]).append((Type,Name,Format,flags))
        except Exception as e:
            pass

    def save(self,out):
        for subType in self:
            subConfig = self[subType]
            newConfig = []
            for u_n_f_f in subConfig:
                if (u_n_f_f == (None,'',None,None) or
                    u_n_f_f == ('raw','',None,None)):
                    continue
                newConfig.append(u_n_f_f)
            if not newConfig:
                continue
            lines = []
            if len(newConfig) == 1:
                lines.append('[%s]\n' % subType)
                unpacker,name,format,flags = newConfig[0]
                lines.append('Type=%s\n' % unpacker)
                if name:
                    lines.append('Name=%s\n' % name)
                if format:
                    lines.append('Format=%s\n' % format)
                if flags:
                    lines.append('Flags=%s\n' % ','.join(flags.allNames()))
            else:
                for unpacker,name,format,flags in newConfig:
                    lines.append('[%s.%s]\n' % (subType,name))
                    lines.append('Type=%s\n' % unpacker)
                    if format:
                        lines.append('Format=%s\n' % format)
                    if flags:
                        lines.append('Flags=%s\n' % ','.join(flags.allNames()))
                    lines.append('\n')
            lines.append('\n')
            out.writelines(lines)

    def setStrings(self,strings={}):
        StringTable.SetStrings(strings)

    def unpackSubrecord(self,subType,ins):
        """ins is assumbed to be a StringIO object containing the
           subrecord data"""
        ins.seek(0,os.SEEK_END)
        eor = ins.tell()
        ins.seek(0)

        ret = []
        try:
            if subType in self:
                for Type,Name,Format,flags in self[subType]:
                    value,DefaultFormat = RecordDef.unpackElement(ins,Type)
                    if Format is None: Format = DefaultFormat
                    if flags:
                        value = flags(value)
                        ext = [(getattr(value,x), '%s.%s' % (Name,x),Format) for x in value.getTrueAttrs()]
                        if not ext:
                            ext = [('',Name,Format)]
                        ret.extend(ext)
                    else:
                        ret.append((value,Name,Format))
            elif subType in RecordDef.globalConfig:
                return RecordDef.globalConfig.unpackSubrecord(subType,ins)
            if ins.tell() < eor:
                value,Format = RecordDef.unpackElement(ins,None)
                ret.append((value,'',Format))
        except Exception as e:
            raise
            ret = [('An error occured while unpacking this subrecord: %s' % e,'ERROR',None)]
        finally:
            ins.seek(0)
        return ret

    @staticmethod
    def unpackElement(ins,Type):
        if Type not in Types: Type = 'raw'
        Unpacker,ReqSize,Eval,DefaultFormat = Types[Type]
        value = bf.bfRead(ins,Unpacker)
        if Eval: value = Eval(value)
        return value,DefaultFormat


def LoadRecordDefs(path='.'):
    path = GPath(path)
    if not path.exists: return {}
    if path.isdir:
        files = (path.join(x) for x in path.list()
                 if x.cext == '.ini' and len(x.sbody) == 4)
    elif x.cext == '.ini' and len(x.sbody) == 4:
        files = [path]
    else:
        files = []

    record_configs = {}
    # Get global config
    allFile = path.join('All.ini')
    allConfig = RecordDef()
    if allFile.exists:
        allConfig.load(allFile.s)
    RecordDef.globalConfig = allConfig

    for file in files:
        config = RecordDef()
        config.load(file.s)
        record_configs[file.sbody.upper()] = config
    return record_configs