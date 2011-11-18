class Flags(object):
    """Represents a flag field."""
    __slots__ = ['_names','_field']

    @staticmethod
    def getNames(*names):
        """Returns dictionary mapping names to indices.
        Names are either strings or (index,name) tuples.
        E.g., Flags.getNames('isQuest','isHidden',None,(4,'isDark'),(7,'hasWater'))"""
        namesDict = {}
        for index,name in enumerate(names):
            if isinstance(name,tuple):
                namesDict[name[1]] = name[0]
            elif name: #--skip if "name" is 0 or None
                namesDict[name] = index
        return namesDict

    #--Generation
    def __init__(self,value=0,*names):
        """Initialize. Attrs, if present, is mapping of attribute names to indices. See getAttrs()"""
        object.__setattr__(self,'_field',int(value) | 0L)
        if len(names) == 1 and isinstance(names[0],dict):
            object.__setattr__(self,'_names',names[0])
        else:
            object.__setattr__(self,'_names',Flags.getNames(*names))

    def __call__(self,newValue=None):
        """Retuns a clone of self, optionally with new value."""
        if newValue is not None:
            return Flags(int(newValue) | 0L,self._names)
        else:
            return Flags(self._field,self._names)

    def __deepcopy__(self,memo={}):
        newFlags=Flags(self._field,self._names)
        memo[id(self)] = newFlags
        return newFlags

    #--As hex string
    def hex(self):
        """Returns hex string of value."""
        return '%08X' % (self._field,)
    def dump(self):
        """Returns value for packing"""
        return self._field

    #--A bit string
    def __str__(self):
        return 'Flags(%s)' % (bin(self._field)[2:],)

    #--As int
    def __int__(self):
        """Return as integer value for saving."""
        return self._field
    def __getstate__(self):
        """Return values for pickling."""
        return self._field, self._names
    def __setstate__(self,fields):
        """Used by unpickler."""
        self._field = fields[0]
        self._names = fields[1]

    #--As list
    def __getitem__(self, index):
        """Get value by index. E.g., flags[3]"""
        return bool((self._field >> index) & 1)

    def __setitem__(self,index,value):
        """Set value by index. E.g., flags[3] = True"""
        value = ((value or 0L) and 1L) << index
        mask = 1L << index
        self._field = ((self._field & ~mask) | value)

    #--As class
    def __getattr__(self,name):
        """Get value by flag name. E.g. flags.isQuestItem"""
        try:
            names = object.__getattribute__(self,'_names')
            index = names[name]
            return (object.__getattribute__(self,'_field') >> index) & 1 == 1
        except KeyError:
            raise AttributeError(name)

    def __setattr__(self,name,value):
        """Set value by flag name. E.g., flags.isQuestItem = False"""
        if name in ('_field','_names'):
            object.__setattr__(self,name,value)
        else:
            self.__setitem__(self._names[name],value)

    #--Native operations
    def __eq__( self, other):
        """Logical equals."""
        if isinstance(other,Flags):
            return self._field == other._field
        else:
            return self._field == other

    def __ne__( self, other):
        """Logical not equals."""
        if isinstance(other,Flags):
            return self._field != other._field
        else:
            return self._field != other

    def __and__(self,other):
        """Bitwise and."""
        if isinstance(other,Flags): other = other._field
        return self(self._field & other)

    def __invert__(self):
        """Bitwise inversion."""
        return self(~self._field)

    def __or__(self,other):
        """Bitwise or."""
        if isinstance(other,Flags): other = other._field
        return self(self._field | other)

    def __xor__(self,other):
        """Bitwise exclusive or."""
        if isinstance(other,Flags): other = other._field
        return self(self._field ^ other)

    def getTrueAttrs(self):
        """Returns attributes that are true."""
        trueNames = [name for name in self._names if getattr(self,name)]
        trueNames.sort(key = lambda xxx: self._names[xxx])
        return tuple(trueNames)

    def allNames(self):
        maxIndex = max(self._names.values())
        names = ['???'] * (maxIndex+1)
        for x in self._names:
            names[self._names[x]] = x
        return names
            
    def getName(self,index):
        i = [x for x in self._names if self._names[x] == index]
        if len(i) > 0: return i[0]
        return '???'