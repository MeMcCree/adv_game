function GetFontDefault(): TFont; cdecl; external;
function LoadFont(fileName: PChar): TFont; cdecl; external;
function LoadFontEx(fileName: PChar; fontSize: Integer; codepoints: PInt; codepointCount: Integer): TFont; cdecl; external;
function LoadFontFromImage(image: TImage; key: TColor; firstChar: Integer): TFont; cdecl; external;
function LoadFontFromMemory(fileType: PChar; fileData: PByte; dataSize, fontSize: Integer; codepoints: PInt; codepointCount: Integer): TFont; cdecl; external;
function IsFontValid(font: TFont): Boolean; cdecl; external;
function LoadFontData(fileData: PByte; dataSize, fontSize: Integer; codepoints: PInt; codepointCount, atype: Integer): PTGlyphInfo; cdecl; external;
function GenImageFontAtlas(glyphs: PTGlyphInfo; glyphRecs: PPTRectangle; glyphCount, fontSize, padding, packMethod: Integer): TImage; cdecl; external;
procedure UnloadFontData(glyphs: PTGlyphInfo; glyphCount: Integer); cdecl; external;
procedure UnloadFont(font: TFont); cdecl; external;
function ExportFontAsCode(font: TFont; fileName: PChar): Boolean; cdecl; external;

procedure DrawFPS(posX, posY: Integer); cdecl; external;
procedure DrawText(text: PChar; posX, posY, fontSize: Integer; color: TColor); cdecl; external;
procedure DrawTextEx(font: TFont; text: PChar; position: TVector2; fontSize, spacing: Single; tint: TColor); cdecl; external;
procedure DrawTextPro(font: TFont; text: PChar; position, origin: TVector2; rotation, fontSize, spacing: Single; tint: TColor); cdecl; external;
procedure DrawTextCodepoint(font: TFont; codepoint: Integer; position: TVector2; fontSize: Single; tint: TColor); cdecl; external;
procedure DrawTextCodepoints(font: TFont; codepoints: PInt; codepointCount: Integer; position: TVector2; fontSize, spacing: Single; tint: TColor); cdecl; external;

procedure SetTextLineSpacing(spacing: Integer); cdecl; external;
function MeasureText(text: PChar; fontSize: Integer): Integer; cdecl; external;
function MeasureTextEx(font: TFont; text: PChar; fontSize, spacing: Single): TVector2; cdecl; external;
function GetGlyphIndex(font: TFont; codepoint: Integer): Integer; cdecl; external;
function GetGlyphInfo(font: TFont; codepoint: Integer): TGlyphInfo; cdecl; external;
function GetGlyphAtlasRec(font: TFont; codepoint: Integer): TRectangle; cdecl; external;

function LoadUTF8(codepoints: PInt; len: Integer): PChar; cdecl; external;
procedure UnloadUTF8(text: PChar); cdecl; external;
function LoadCodepoints(text: PChar; count: PInt): PInt; cdecl; external;
procedure UnloadCodepoints(codepoints: PInt); cdecl; external;
function GetCodepointCount(text: PChar): Integer; cdecl; external;
function GetCodepoint(text: PChar; codepointSize: PInt): Integer; cdecl; external;
function GetCodepointNext(text: PChar; codepointSize: PInt): Integer; cdecl; external;
function GetCodepointPrevious(text: PChar; codepointSize: PInt): Integer; cdecl; external;
function CodepointToUTF8(codepoint: Integer; utf8Size: PInt): PChar; cdecl; external;

function TextCopy(dst: PChar; src: PChar): Integer; cdecl; external;
function TextIsEqual(text1, text2: PChar): Boolean; cdecl; external;
function TextLength(text: PChar): dWord; cdecl; external;
function TextFormat(text: PChar): PChar; cdecl; varargs; external;
function TextSubtext(text: PChar; position, len: Integer): PChar; cdecl; external;
function TextReplace(text, replace, by: PChar): PChar; cdecl; external;
function TextInsert(text, ins: PChar; position: Integer): PChar; cdecl; external;
function TextJoin(textList: PPChar; count: Integer; delimiter: PChar): PChar; cdecl; external;
function TextSplit(text: PChar; delimiter: char; count: PInt): PPChar; cdecl; external;
procedure TextAppend(text: PChar; append: PChar; position: PInt); cdecl; external;
function TextFindIndex(text, find: PChar): Integer; cdecl; external;
function TextToUpper(text: PChar): PChar; cdecl; external;
function TextToLower(text: PChar): PChar; cdecl; external;
function TextToPascal(text: PChar): PChar; cdecl; external;
function TextToSnake(text: PChar): PChar; cdecl; external;
function TextToCamel(text: PChar): PChar; cdecl; external;

function TextToInteger(text: PChar): Integer; cdecl; external;
function TextToFloat(text: PChar): Single; cdecl; external;
