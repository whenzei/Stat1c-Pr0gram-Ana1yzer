# Frontend Abstract API (Ver.1)

## Tokenizer
**Overview** Tokenizer splits input into tokens for Parser.

|**Constructor**|
|---|
|Tokenizer() <br> ***Description:*** Creates a new instance of Tokenizer.|

| **API** | 
| -------- | 
| **TOKEN_LIST** Tokenize(**STRING** input) <br> ***Description:*** Uses various tokenizer functions such as `SkipWhiteSpace` or `TokenizeDigits` to tokenize the supplied input, returning a list of tokens. If nothing can be tokenized, returns empty list. | 
| **RESULT** SkipWhitespace(**STRING** input, **INDEX** index) <br> ***Description:*** If the current character at the given index of input is a whitespace, returns `RESULT` consisting of the number of characters consumed (which is 1), along with a `IGNORE` type. Else return an empty `RESULT`. | 
| **RESULT** SkipComments(**STRING** input, **INDEX** index) <br> ***Description:*** If the current character at the given index of input matches the syntax of SIMPLE's comments, returns `RESULT` consisting of the number of characters consumed, along with a `IGNORE` type. Else return an empty `RESULT`. | 
| **RESULT** TokenizeDigits(**STRING** input, **INDEX** index) <br> ***Description:*** If the current character at the given index of input matches the syntax of SIMPLE's digits, keep appending the next character to the current string until the next character stops matching, then returns `RESULT` consisting of the number of characters consumed, along with the `DIGIT` type and appended digit string. Else return an empty `RESULT`. |
| **RESULT** TokenizeNames(**STRING** input, **INDEX** index) <br> ***Description:*** If the current character at the given index of input matches the syntax of SIMPLE's names, keep appending the next character to the current string until the next character stops matching, then returns `RESULT` consisting of the number of characters consumed, along with the `NAME` type and appended name string. Else return an empty `RESULT`. |
| **RESULT** TokenizeBraces(**STRING** input, **INDEX** index) <br> ***Description:*** If the current character at the given index of input is either an opening or closing brace (`{` or `}`), returns `RESULT` consisting of the number of characters consumed (which is 1), along with a `BRACE` type. Else return an empty `RESULT`. |
| **RESULT** TokenizeOperators(**STRING** input, **INDEX** index) <br> ***Description:*** If the current character at the given index of input matches any of SIMPLE's operators, returns `RESULT` consisting of the number of characters consumed (which is 1), along with a `OPERATOR` type. Else return an empty `RESULT`. |
| **RESULT** TokenizeSemicolon(**STRING** input, **INDEX** index) <br> ***Description:*** If the current character at the given index of input is a semicolon (`;`), returns `RESULT` consisting of the number of characters consumed (which is 1), along with a `SEMICOLON` type. Else return an empty `RESULT`. |
| **RESULT** TokenizeEquals(**STRING** input, **INDEX** index) <br> ***Description:*** If the current character at the given index of input is an equal sign (`=`), returns `RESULT` consisting of the number of characters consumed (which is 1), along with a `ASSIGNMENT` type. Else return an empty `RESULT`. |
|**RESULT** EmptyResult()<br> ***Description:*** Returns an empty `RESULT` with an `IGNORE` type.|
## Parser
**Overview:** Parser reads content provided and updates PKB accordingly.

| **Constructor** 
| -------- | 
| Parser(**PKB** pkb)<br>***Description:***  Creates a new instance of `Parser` and connects to the referenced `PKB`.

| **API** | 
| -------- | 
| **VOID** Parse(STRING filepath) <br> ***Description:*** Parses the file located at filepath and stores the resulting `TOKEN_LIST` into member variable. If file is not found, stores empty list. |
| **TOKEN_LIST** GetTokenList() <br> ***Description:*** Getter method, returns the Parser's token list. |
| **PKB** GetPkb() <br> ***Description:*** Getter method, returns the Parser's `PKB` instance. |
| **VOID** SetPkb(PKB pkb) <br> ***Description:*** Setter method, sets the Parser's `PKB` instance to the referenced pkb. |

## SimpleValidator
**Overview:** SimpleValidator performs syntax validation.

| **API** | 
| -------- | 
| **BOOLEAN** ValidateProcedure(**TOKEN_LIST** tokens) <br> ***Description:*** Checks the syntax of a procedure in the token form. If procedure is syntactically correct in SIMPLE language, returns true. Else returns false. | 
| **BOOLEAN** IsKeyword(**STRING** input) <br> ***Description:*** If input is matches a keyword in the SIMPLE language, returns true. Else returns false. |
| **BOOLEAN** IsKeyword(**KEYWORD_TYPE** type, **STRING** input)<br> ***Requires:*** Keyword type provided is a valid type in the Simple Validator.  <br> ***Description:*** If input matches the provided keyword type in the SIMPLE language, returns true. Else returns false. |

