# Parser.py =======================================
#  A generic class for handling parsing of
#  scripts and equations.
#  - The following operators are supported by default:
#   + Addition
#   - Subtraction
#   * Multiplication
#   / Division
#   % Modulus
#   ^ Exponent
#   () Parenthesis
#  - The parser follows the order of operations
#  - Variables are also handled, all are treated
#    as float's.  The variable is initialized
#    on its first appearance to '0.0'.  Multiple
#    assignment is allowed, but only the default
#    assignment operator is defined by default
#  - Constants can be defined
#  - Keywords can be defined
#  - Functions can be defined
#
# Defined functions to use are:
#  SetOperator
#  SetKeyword
#  SetFunction
#  SetConstant
#  SetVariable
#  PushFlow
#  PopFlow
#  PeekFlow
#  LenFlow
#  PurgeFlow
#  RunLine
#  error
#  ExecuteTokens
#  TokensToRPN
#  ExecuteRPN
#==================================================
from string import digits, whitespace, letters
import types
#--------------------------------------------------
name_start = letters + '_'
name_chars = name_start + digits

# validName ---------------------------------------
#  Test if a string can be used as a valid name
#--------------------------------------------------
def validName(name):
    try:
        if name[0] not in name_start: return False
        for i in name:
            if i not in name_chars: return False
        return True
    except:
        return False

# validNumber -------------------------------------
#  Test if a string can be used as a valid number
#--------------------------------------------------
def validNumber(string):
    try:
        bDecimal = False
        for i in string:
            if i == '.':
                if bDecimal: return False
                bDecimal = True
            elif i not in digits:
                return False
        return True
    except:
        return False

# Define Some Constants ---------------------------

# Some error string
ERR_CANNOT_SET = "Cannot set %s '%s': type is '%s'."
ERR_TOO_FEW_ARGS = "Too few arguments to %s '%s':  got %s, expected %s."
ERR_TOO_MANY_ARGS = "Too many arguments to %s '%s':  got %s, expected %s."

class KEY:
    # Constants for keyword args
    NO_MAX = -1     # No maximum arguments
    NA = 0          # Not a variable argument keyword
    
class OP:
    # Constants for operator precedences
    PAR = 0     # Parenthesis
    EXP = 1     # Exponent
    UNA = 2     # Unary (++, --)
    MUL = 3     # Multiplication (*, /, %)
    ADD = 4     # Addition (+, -)
    CO1 = 5     # Comparison (>=,<=,>,<)
    CO2 = 6     # Comparison (!=, ==)
    MEM = 7     # Membership test (a in b)
    NOT = 8     # Logical not (not, !)
    AND = 9     # Logical and (and, &)
    OR  = 10    # Locical or (or, |)
    ASS = 11    # Assignment (=,+=,etc

# Constants for operator associations
LEFT = 0
RIGHT = 1

# Constants for the type of a token
UNKNOWN = 0
NAME = 1            # Can be a name token, but not used yet
CONSTANT = 2
VARIABLE = 3
FUNCTION = 4
KEYWORD = 5
OPERATOR = 6
INTEGER = 7
DECIMAL = 8
OPEN_PARENS = 9
CLOSE_PARENS = 10
COMMA = 11
WHITESPACE = 12
STRING = 13

Types = {UNKNOWN:'UNKNOWN',
         NAME:'NAME',
         CONSTANT:'CONSTANT',
         VARIABLE:'VARIABLE',
         FUNCTION:'FUNCTION',
         KEYWORD:'KEYWORD',
         OPERATOR:'OPERATOR',
         INTEGER:'INTEGER',
         DECIMAL:'DECIMAL',
         OPEN_PARENS:'OPEN_PARENS',
         CLOSE_PARENS:'CLOSE_PARENS',
         COMMA:'COMMA',
         WHITESPACE:'WHITESPACE',
         STRING:'STRING'
         }

# getType ---------------------------------------
#  determines the type of a string.  If 'parser'
#  is passed, then it will attempt it against
#  vairious names as well.
#------------------------------------------------
def getType(item, parser=None):
    if type(item) == types.StringType:
        if not parser: return STRING
        if item in parser.constants: return CONSTANT
        if item in parser.variables: return VARIABLE
        if item in parser.keywords : return KEYWORD
        if item in parser.functions: return FUNCTION
        if item in parser.operators: return OPERATOR
        if item == '(': return OPEN_PARENS
        if item == ')': return CLOSE_PARENS
        if item == ',': return COMMA
        if validName(item): return NAME
        if validNumber(item):
            if '.' in item: return DECIMAL
            return INTEGER
        for i in item:
            if i not in whitespace: return UNKNOWN
        return WHITESPACE
    if type(item) == types.IntType: return INTEGER
    if type(item) == types.FloatType: return DECIMAL
    return UNKNOWN

# FlowControl -------------------------------------
#  Flow control object, to hold info about a flow
#  control statement
#--------------------------------------------------
class FlowControl:
    def __init__(self, type, active, keywords=[], **attribs):
        self.type = type
        self.active = active
        self.keywords = keywords
        for i in attribs:
            setattr(self, i, attribs[i])

# Token -------------------------------------------
#  Token object, to hold info about a token
#--------------------------------------------------

# ParserError -------------------------------------
#  So when we catch exceptions we know if it's a
#  problem with the parser, or a problem with the
#  script
#--------------------------------------------------
class ParserError(SyntaxError): pass
    
class Parser(object):
    class Operator:
        def __init__(self, function, precedence, association=LEFT, passTokens=True):
            self.function = function
            self.precedence = precedence
            self.association = association
            if self.precedence in [OP.UNA, OP.NOT]:
                self.numArgs = 1
            else:
                self.numArgs = 2
            self.passTokens = passTokens

        def __call__(self, l, r=None):
            if self.numArgs == 1:
                return self.function(l)
            return self.function(l, r)

    class Keyword:
        def __init__(self, function, min_args=0, max_args=KEY.NA, keepCommas=False, passTokens=False):
            self.function = function
            if max_args == KEY.NA: max_args = min_args
            if max_args < min_args and max_args > 0: max_args = min_args
            self.minArgs = min_args
            self.maxArgs = max_args
            self.commas = keepCommas
            self.passTokens = passTokens

        def __call__(self, *args): return self.function(*args)

    class Function:
        def __init__(self, function, num_args, passTokens=False):
            self.function = function
            self.numArgs = num_args
            self.passTokens = passTokens

        def __call__(self, *args): return self.function(*args)

    class Token:
        def __init__(self, text, Type=None, parser=None, line=None, pos=(None,None)):
            if isinstance(text, Parser.Token):
                self.text = text.text
                self.type = text.type
                self.parser = text.parser
                self.line = text.line
                self.pos = text.pos
            else:
                self.text = text
                self.type = Type or getType(text,parser)
                self.parser = parser
                self.line = line
                self.pos = pos

        def GetData(self):
            if self.parser:
                if self.type == FUNCTION: return self.parser.functions[self.text]
                if self.type == KEYWORD : return self.parser.keywords[self.text]
                if self.type == OPERATOR: return self.parser.operators[self.text]
                if self.type == VARIABLE: return self.parser.variables[self.text]
                if self.type == CONSTANT: return self.parser.constants[self.text]
                if self.type == DECIMAL : return float(self.text)
                if self.type == INTEGER : return int(self.text)
            return self.text
        data = property(GetData)

        def __cmp__(self, other): return cmp(self.data, other.data)
        def __add__(self, other): return Parser.Token(self.data + other.data)
        def __sub__(self, other): return Parser.Token(self.data - other.data)
        def __mul__(self, other): return Parser.Token(self.data * other.data)
        def __div__(self, other): return Parser.Token(self.data / other.data)
        def __truediv__(self, other): return Parser.Token(self.data / other.data)
        def __floordiv__(self, other): return Parser.Token(self.data // other.data)
        def __divmod__(self, other): return Parser.Token(divmod(self.data, other.data))
        def __pow__(self, other): return Parser.Token(self.data ** other.data)
        def __lshift__(self, other): return Parser.Token(self.data << other.data)
        def __rshift__(self, other): return Parser.Token(self.data >> other.data)
        def __and__(self, other): return Parser.Token(self.data & other.data)
        def __xor__(self, other): return Parser.Token(self.data ^ other.data)
        def __or__(self, other): return Parser.Token(self.data | other.data)
        def __nonzero__(self): return bool(self.data)
        def __neg__(self): return Parser.Token(-self.data)
        def __pos__(self): return Parser.Token(+self.data)
        def __abs__(self): return abs(self.data)
        def __int__(self): return int(self.data)
        def __long__(self): return long(self.data)
        def __float__(self): return float(self.data)
        def __str__(self): return str(self.data)

        def __repr__(self): return '<Token-%s:%s>' % (Types[self.type],self.text)
        

    # Now for the Parser class
    def __init__(self,
                 doImplicit='*',
                 comment=';',
                 constants={'True':True,'False':False},
                 variables=None
                 ):
        self.doImplicit = doImplicit
        self.comment = comment

        self.runon = False
        self.cCol = 0
        self.cLine = 0
        self.tokens = []
        self.flow = []

        self.opChars = ''
        self.operators = {}
        self.keywords = {}
        self.functions = {}
        self.constants = constants or {}
        self.variables = variables or {}
        self.escapes = {'n':'\n',
                        't':'\t'
                        }

        self.word = None
        self.wordStart = None

    def SetOperator(self, name, *args, **kwdargs):
        type = getType(name, self)
        if type not in [NAME,OPERATOR,UNKNOWN]:
            self.error(ERR_CANNOT_SET % ('operator', name, Types[type]))
        self.operators[name] = Parser.Operator(*args, **kwdargs)
        for i in name:
            if i not in self.opChars: self.opChars += i
    def SetKeyword(self, name, *args, **kwdargs):
        type = getType(name, self)
        if type not in [NAME,KEYWORD]:
            self.error(ERR_CANNOT_SET % ('keyword', name, Types[type]))
        self.keywords[name] = Parser.Keyword(*args, **kwdargs)
    def SetFunction(self, name, *args, **kwdargs):
        type = getType(name, self)
        if type not in [NAME,FUNCTION]:
            self.error(ERR_CANNOT_SET % ('function', name, Types[type]))
        self.functions[name] = Parser.Function(*args, **kwdargs)
    def SetConstant(self, name, value):
        type = getType(name, self)
        if type not in [NAME,CONSTANT]:
            self.error(ERR_CANNOT_SET % ('constant', name, Types[type]))
        self.constants[name] = value
    def SetVariable(self, name, value):
        type = getType(name, self)
        if type not in [NAME, VARIABLE]:
            self.error(ERR_CANNOT_SET % ('variable', name, Types[type]))

    # Flow control stack
    def PushFlow(self, type, active, keywords, **attribs): self.Flow.append(FlowControl(type,active,keywords,**attribs))
    def PopFlow(self): return self.Flow.pop()
    def PopFrontFlow(self): return self.Flow.pop(0)
    def PeekFlow(self,index=-1): return self.Flow[index]
    def LenFlow(self): return len(self.Flow)
    def PurgeFlow(self): self.Flow = []

    # Run a line of code: returns True if more lines are needed to make a complete line, False if not
    def RunLine(self, line):
        self.cLine += 1

        # First parse the line into tokens
        if not self.runon:
            self.tokens = []
        self.TokenizeLine(line)
        if self.runon: return True

        # No tokens?
        if len(self.tokens) == 0: return False

        # See if we're in currently within a flow control construct
        if self.LenFlow() > 0:
            i = self.PeekFlow()
            if not i.active and self.tokens[0].text not in i.keywords:
                return False

        # If we have a keyword, just run it
        if self.tokens[0].type == KEYWORD:
            key = self.tokens.pop(0)
            if not key.data.commas: self.SkipCommas()
            if not key.data.passTokens: self.ConvertToValues()
            numArgs = len(self.tokens)
            if numArgs > key.data.maxArgs and key.data.maxArgs != KEY.NO_MAX:
                self.error(ERR_TOO_MANY_ARGS % ('keyword', key.text, numArgs, 'min: %s, max: %s' % (key.data.minArgs,key.data.maxArgs)))
            if numArgs < key.data.minArgs:
                self.error(ERR_TOO_FEW_ARGS % ('keyword', key.text, numArgs, 'min: %s, max: %s' % (key.data.minArgs,key.data.maxArgs)))
            key.data(*self.tokens)
        # It's just an expression, didnt start with a keyword
        else:
            # Convert to reverse-polish notation and execute
            self.ExecuteTokens()
        return False

    # Removes any commas from a list of tokesn
    def SkipCommas(self, tokens=None):
        if tokens is None:
            self.tokens = [x for x in self.tokens if x.type != COMMA]
            return self.tokens
        tokens = [x for x in tokens if x.type != COMMA]
        return tokens

    # Changes out tokens for their values
    def ConvertToValues(self, tokens=None):
        if tokens is None:
            self.tokens = [x.data for x in self.tokens]
            return self.tokens
        tokens = [x.data for x in tokens]
        return tokens

    # Split a string into tokens
    def TokenizeLine(self, line):
        self.word = None
        self.wordStart = None
        self.cCol = 0
        self.runon = False

        state = self._stateSpace
        for i in line:
            state = state(i)
            if not state: return None
            self.cCol += 1
        self._emit()

        return self.tokens

    # Run a list of tokens
    def ExecuteTokens(self, tokens=None):
        tokens = tokens or self.tokens
        self.TokensToRPN(tokens)
        return self.ExecuteRPN()

    # Convert a list of tokens to rpn
    def TokensToRPN(self, tokens=None):
        tokens = tokens or self.tokens
        rpn = []
        stack = []

        for i in tokens:
            if i.type in [INTEGER,DECIMAL,CONSTANT,VARIABLE,NAME,STRING]:
                rpn.append(i)
            elif i.type == COMMA:
                while len(stack) > 0 and stack[-1].type != OPEN_PARENS:
                    rpn.append(stack.pop())
                if len(stack) == 0:
                    self.error("Misplaced ',' or missing parenthesis.")
            elif i.type == FUNCTION:
                stack.append(i)
            elif i.type == OPERATOR:
                while len(stack) > 0 and stack[-1].type == OPERATOR:
                    if i.data.association == LEFT and i.data.precedence >= stack[-1].data.precedence:
                        rpn.append(stack.pop())
                    elif i.data.association == RIGHT and i.data.precedence > stack[-1].data.precedence:
                        rpn.append(stack.pop())
                    else:
                        break
                stack.append(i)
            elif i.type == OPEN_PARENS:
                stack.append(i)
            elif i.type == CLOSE_PARENS:
                while len(stack) > 0 and stack[-1].type != OPEN_PARENS:
                    rpn.append(stack.pop())
                if len(stack) == 0:
                    self.error('Unmatched parenthesis.')
                stack.pop()
                if len(stack) > 0 and stack[-1].type == FUNCTION:
                    rpn.append(stack.pop())
            else:
                self.error("Unrecognized token: '%s', type: %" % (i.text, Types[i.type]))
        while len(stack) > 0:
            i = stack.pop()
            if i.type in [OPEN_PARENS,CLOSE_PARENS]:
                self.error('Unmatched parenthesis.')
            rpn.append(i)
        self.rpn = rpn
        return rpn

    def ExecuteRPN(self, rpn=None):
        rpn = rpn or self.rpn

        stack = []
        for i in rpn:
            if i.type == OPERATOR:
                if len(stack) < i.data.numArgs:
                    self.error(ERR_TOO_FEW_ARGS % ('operator', i.text, len(stack), i.data.numArgs))
                args = []
                while len(args) < i.data.numArgs:
                    args.append(stack.pop())
                args.reverse()
                if not i.data.passTokens:
                    args = [x.data for x in args]
                ret = i.data(*args)
                stack.append(Parser.Token(ret))
            elif i.type == FUNCTION:
                if len(stack) < i.data.numArgs:
                    self.error(ERR_TOO_FEW_ARGS % ('function', i.text, len(stack), i.data.numArgs))
                args = []
                while len(args) < i.data.numArgs:
                    args.append(stack.pop())
                args.reverse()
                if not i.data.passTokens: args = self.ConvertToValues(args)
                ret = i.data(*args)
                stack.append(Parser.Token(ret))
            else:
                stack.append(i)
        if len(stack) == 1:
            return stack[0].data
        self.error('Too many values left at the end of evaluation.')

    def error(self, msg):
        raise ParserError, '(Line %s, Column %s): %s' % (self.cLine, self.cCol, msg)

    #Functions for parsing a line into tokens
    def _grow(self, c):
        if self.word: self.word += c
        else:
            self.word = c
            self.wordStart = self.cCol

    def _emit(self, word=None, type=None):
        word = word or self.word
        if word is None: return
        self.wordStart = self.wordStart or self.cCol - 1
        type = type or getType(word, self)

        # Implicit multiplication
        if self.doImplicit:
            if len(self.tokens) > 0:
                left = self.tokens[-1].type
                if left == CLOSE_PARENS:
                    if type in [OPEN_PARENS,DECIMAL,INTEGER,FUNCTION,VARIABLE,CONSTANT,NAME]:
                        self.tokens.append(Parser.Token(self.doImplicit,OPERATOR,self,self.cLine))
                elif left in [DECIMAL,INTEGER]:
                    if type in [OPEN_PARENS,FUNCTION,VARIABLE,CONSTANT,NAME]:
                        self.tokens.append(Parser.Token(self.doImplicit,OPERATOR,self,self.cLine))
        self.tokens.append(Parser.Token(word,type,self,self.cLine,(self.wordStart,self.cCol)))
        self.word = None
        self.wordStart = None
        
    def _stateSpace(self, c):
        self._emit()
        if c in whitespace: return self._stateSpace
        if c == "'": return self._stateSQuote
        if c == '"': return self._stateDQuote
        if c == '\\': return self._stateEscape
        if c == self.comment: return self._stateComment
        self._grow(c)
        if c in name_start: return self._stateName
        if c in self.opChars: return self._stateOperator
        if c in digits: return self._stateNumber
        if c == '.': return self._stateDecimal
        if c == '(': return self._stateSpace
        if c == ')': return self._stateEndBracket
        if c == ',': return self._stateSpace
        self.error("Invalid character: '%s'" % c)

    def _stateSQuote(self, c):
        if c == '\\': return self._stateSQuoteEscape
        if c == "'":
            if not self.word: self.word = ''
            self._emit(type=STRING)
            return self._stateSpace
        if c == '\n':
            self.error('Unterminated single quote.')
        self._grow(c)
        return self._stateSQuote
    def _stateSQuoteEscape(self, c):
        if c in self.escapes: self._grow(self.escapes[c])
        else: self._grow(c)
        return self._stateSQuote

    def _stateDQuote(self, c):
        if c == '\\': return self._stateDQuoteEscape
        if c == '"':
            if not self.word: self.word = ""
            self._emit(type=STRING)
            return self._stateSpace
        if c == '\n':
            self.error("Unterminated double quote.")
        self._grow(c)
        return self._stateDQuote
    def _stateDQuoteEscape(self, c):
        if c in self.escapes: self._grow(self.escapes[c])
        else: self._grow(c)
        return self._stateDQuote

    def _stateEscape(self, c):
        if c == '\n':
            self.runon = True
            return
        return self._stateSpace(c)

    def _stateComment(self, c): return self._stateComment

    def _stateName(self, c):
        if c in name_chars:
            self._grow(c)
            return self._stateName
        if c in ["'",'"']:
            self.error('Unexpected quotation %s following name token.' % c)
        return self._stateSpace(c)

    def _stateOperator(self, c):
        if c in self.opChars:
            self._grow(c)
            return self._stateOperator
        return self._stateSpace(c)

    def _stateNumber(self, c):
        if c in digits:
            self._grow(c)
            return self._stateNumber
        if c == '.':
            self._grow(c)
            return self._stateDecimal
        if c in ['"',"'"]:
            self.error('Unexpected quotation %s following number token.' % c)
        return self._stateSpace(c)
    def _stateDecimal(self, c):
        if c in digits:
            self._grow(c)
            return self._stateDecimal
        if c in ['"',"'",'.']:
            self.error('Unexpected %s following decimal token.' % c)
        return self._stateSpace(c)

    def _stateEndBracket(self, c):
        if c in ['"',"'"]:
            self.error('Unexpected quotation %s following parenthesis.' % c)
        return self._stateSpace(c)